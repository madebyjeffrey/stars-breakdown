#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 step;
    StarsI16 ensure_calls;
    StarsI16 basic_step;
    StarsI16 fill_step;
} Trace;

static StarsI16 ensure_ai(void *opaque, StarsI16 percent,
    StarsI16 *latest_starbase_design)
{
    Trace *trace = (Trace *)opaque;
    if (percent != 20)
        return -1;
    ++trace->ensure_calls;
    *latest_starbase_design = 7;
    return 19;
}

static StarsI16 ensure(void *opaque, StarsI16 research_order)
{
    (void)opaque;
    return research_order == 19;
}

static StarsI16 basic(void *opaque, StarsI16 research_order,
    StarsI16 latest_starbase_design, StarsI16 player_id)
{
    Trace *trace = (Trace *)opaque;
    if (research_order != 19 || latest_starbase_design != 7 ||
            player_id != 4 || trace->step != 0)
        return 0;
    trace->basic_step = ++trace->step;
    return 1;
}

static StarsI16 fill(void *opaque)
{
    Trace *trace = (Trace *)opaque;
    if (trace->step != 1)
        return 0;
    trace->fill_step = ++trace->step;
    return 1;
}

int main(void)
{
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronAiTurnInput input;
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
    failures = !stars_do_automitron_ai_turn(&input);
    failures += trace.ensure_calls != 1 || trace.basic_step != 1 ||
        trace.fill_step != 2 || input.research_order != 19 ||
        input.latest_starbase_design != 7;
    printf("AUTOMITRON_AI_PHASE_CHECKS=6 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
