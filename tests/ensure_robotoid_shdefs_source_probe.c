#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 creates;
    StarsI16 retries;
    StarsI16 changed;
    StarsI16 last_slot;
    StarsU16 last_hull;
} Trace;

static StarsI16 random_value(void *context, StarsI16 limit)
{
    (void)context;
    return limit > 1 ? 1 : 0;
}

static StarsI16 create_shdef(void *context, StarsI16 slot, StarsU16 hull,
    StarsI16 name_id)
{
    Trace *trace = (Trace *)context;
    ++trace->creates;
    trace->last_slot = slot;
    trace->last_hull = hull;
    if (name_id == -1 && hull != 5U)
        return 0;
    if (trace->retries < 2 && hull == 31U) {
        ++trace->retries;
        return 0;
    }
    return 1;
}

static void change_shdef(void *context, StarsI16 slot,
    const StarsRobotoidShdef *design)
{
    Trace *trace = (Trace *)context;
    (void)slot;
    (void)design;
    ++trace->changed;
}

int main(void)
{
    StarsEnsureRobotoidShdefsInput input;
    StarsRobotoidShdef designs[16];
    Trace trace;
    int failures = 0;

    memset(&input, 0, sizeof input);
    memset(designs, 0, sizeof designs);
    memset(&trace, 0, sizeof trace);
    input.designs = designs;
    input.design_count = 16;
    input.turn = 40;
    input.random_value = random_value;
    input.create_shdef = create_shdef;
    input.change_shdef = change_shdef;
    input.context = &trace;
    input.tech[0] = 6;
    input.tech[1] = 15;
    input.tech[2] = 12;
    input.tech[3] = 15;
    input.tech[4] = 10;
    input.tech[5] = 4;
    designs[2].active = 0x0200;
    designs[6].active = 0x0200;
    designs[9].active = 0x0200;
    designs[11].active = 0x0200;
    designs[14].active = 0x0200;
    designs[15].active = 0x0200;
    designs[0].hull_id = 3;
    input.starter_hull = 3;
    stars_ensure_robotoid_shdefs(&input);
    if (trace.creates < 8 || trace.changed != 1 || trace.last_hull != 5U)
        ++failures;

    memset(&trace, 0, sizeof trace);
    memset(input.tech, 0, sizeof input.tech);
    stars_ensure_robotoid_shdefs(&input);
    if (trace.creates != 0 || trace.changed != 0)
        ++failures;
    printf("ENSURE_ROBOTOID_SHDEFS_CHECKS=2 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
