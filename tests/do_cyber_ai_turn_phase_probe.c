#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 step;
    StarsI16 basic_step;
    StarsI16 packet_step;
    StarsI16 fill_step;
    StarsI16 ensure_calls;
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
} Trace;

static StarsI16 merge(void *opaque, StarsI16 mask)
{
    (void)opaque;
    (void)mask;
    return 1;
}

static StarsI16 basic(void *opaque, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (player_id != 4 || trace->step != 0)
        return 0;
    trace->basic_step = ++trace->step;
    return 1;
}

static StarsI16 packets(void *opaque, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (player_id != 4 || trace->step != 1)
        return 0;
    trace->packet_step = ++trace->step;
    return 1;
}

static StarsI16 fill(void *opaque)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->step != 2)
        return 0;
    trace->fill_step = ++trace->step;
    return 1;
}

static StarsI16 ensure_ai(void *opaque, StarsI16 percent,
    StarsI16 *latest_starbase_design)
{
    Trace *trace = (Trace *)opaque;
    if (percent != 17)
        return -1;
    ++trace->ensure_calls;
    trace->research_order = 23;
    trace->latest_starbase_design = 4;
    *latest_starbase_design = trace->latest_starbase_design;
    return trace->research_order;
}

static StarsI16 basic_with_research(void *opaque, StarsI16 research_order,
    StarsI16 latest_starbase_design, StarsI16 player_id)
{
    Trace *trace = (Trace *)opaque;
    if (research_order != 23 || latest_starbase_design != 4 ||
            player_id != 4 || trace->step != 0)
        return 0;
    trace->basic_step = ++trace->step;
    return 1;
}

int main(void)
{
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    Trace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    ops.merge_shdefs = merge;
    ops.run_basic_tasks = basic;
    ops.ensure_ai = ensure_ai;
    ops.run_basic_tasks_with_research = basic_with_research;
    ops.run_packets = packets;
    ops.fill_production_queue = fill;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.ops = &ops;
    failures = !stars_do_cyber_ai_turn(&input);
    failures += trace.basic_step != 1 || trace.packet_step != 2 ||
        trace.fill_step != 3 || trace.ensure_calls != 1;
    input.planet_count = 1;
    input.planets = 0;
    failures += stars_do_cyber_ai_turn(&input) != 0;
    input.planet_count = 0;
    input.planets = 0;
    input.fleet_count = 1;
    input.fleets = 0;
    failures += stars_do_cyber_ai_turn(&input) != 0;
    printf("CYBER_AI_PHASE_CHECKS=7 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
