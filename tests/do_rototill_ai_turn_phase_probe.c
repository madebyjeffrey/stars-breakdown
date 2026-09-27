#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 steps;
    StarsI16 ensure_calls;
} Trace;

static StarsI16 ensure_ai(void *opaque, StarsI16 percent,
    StarsI16 *latest_starbase_design)
{
    Trace *trace = (Trace *)opaque;
    if (percent != 15)
        return -1;
    ++trace->ensure_calls;
    *latest_starbase_design = 3;
    return 22;
}

static StarsI16 ensure(void *opaque, StarsI16 order)
{
    (void)opaque;
    return order == 22;
}

static StarsI16 basic(void *opaque, StarsI16 order,
    StarsI16 latest_starbase_design, StarsI16 player_id)
{
    Trace *trace = (Trace *)opaque;
    if (order != 22 || latest_starbase_design != 3 || player_id != 4 ||
            trace->steps != 0)
        return 0;
    ++trace->steps;
    return 1;
}

static StarsI16 fill(void *opaque)
{
    Trace *trace = (Trace *)opaque;
    if (trace->steps != 1)
        return 0;
    ++trace->steps;
    return 1;
}

int main(void)
{
    StarsRototillAiTurnOps ops;
    StarsRototillAiTurnInput input;
    Trace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    ops.ensure_ai = ensure_ai;
    ops.ensure_shdefs = ensure;
    ops.run_basic_tasks = basic;
    ops.fill_production_queue = fill;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.ops = &ops;
    failures = !stars_do_rototill_ai_turn(&input);
    failures += trace.ensure_calls != 1 || trace.steps != 2 ||
        input.current_order != 22 || input.latest_starbase_design != 3;
    input.planet_count = 1;
    input.planets = 0;
    failures += stars_do_rototill_ai_turn(&input) != 0;
    input.planet_count = 0;
    input.fleet_count = 1;
    input.fleets = 0;
    failures += stars_do_rototill_ai_turn(&input) != 0;
    printf("ROTOTILL_AI_PHASE_CHECKS=7 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
