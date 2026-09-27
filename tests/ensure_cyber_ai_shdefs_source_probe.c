#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 creates;
    StarsI16 changes;
    StarsI16 failures_left;
    StarsI16 first_slot;
    StarsU16 first_hull;
    StarsU16 first_template;
    StarsCyberAiShdef *designs;
} Trace;

static StarsI16 random_value(void *context, StarsI16 limit)
{
    (void)context;
    return limit > 1 ? (StarsI16)(limit - 1) : 0;
}

static StarsI16 create_shdef(void *context, StarsI16 slot, StarsU16 hull,
    StarsU16 template_id)
{
    Trace *trace = (Trace *)context;
    if (trace->creates == 0) {
        trace->first_slot = slot;
        trace->first_hull = hull;
        trace->first_template = template_id;
    }
    ++trace->creates;
    if (trace->failures_left > 0) {
        --trace->failures_left;
        return 0;
    }
    return 1;
}

static void change_shdef(void *context, StarsI16 slot,
    const StarsCyberAiShdef *design)
{
    Trace *trace = (Trace *)context;
    (void)slot;
    trace->designs[slot] = *design;
    ++trace->changes;
}

int main(void)
{
    StarsEnsureCyberAiShdefsInput input;
    StarsCyberAiShdef designs[16];
    Trace trace;
    int failures = 0;

    memset(&input, 0, sizeof input);
    memset(designs, 0, sizeof designs);
    memset(&trace, 0, sizeof trace);
    input.designs = designs;
    input.design_count = 16;
    input.turn = 41;
    input.ai_level = 2;
    input.random_value = random_value;
    input.create_shdef = create_shdef;
    input.change_shdef = change_shdef;
    input.context = &trace;
    trace.designs = designs;
    designs[0].hull_id = 3;
    designs[0].existing = 0;
    designs[4].active = 0x0200;
    designs[6].active = 0x0200;
    designs[14].active = 0x0200;
    trace.failures_left = 2;
    stars_ensure_cyber_ai_shdefs(7, &input);
    if (trace.changes != 1 || trace.creates < 8 ||
            trace.first_slot != 0 || trace.first_hull != 5 ||
            trace.first_template != 12)
        ++failures;

    memset(&trace, 0, sizeof trace);
    memset(designs, 0, sizeof designs);
    input.turn = 10;
    input.ai_level = 1;
    designs[4].active = 0x0200;
    stars_ensure_cyber_ai_shdefs(0, &input);
    if (trace.creates != 0 || trace.changes != 0)
        ++failures;

    printf("ENSURE_CYBER_AI_SHDEFS_CHECKS=2 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
