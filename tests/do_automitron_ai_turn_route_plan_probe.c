#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 plan_calls;
    StarsI16 apply_calls;
    StarsI16 visit_calls;
    StarsI16 valid;
    StarsI16 reject_plan;
    StarsI16 reject_apply;
    StarsI16 events[12];
    StarsI16 event_count;
} Trace;

static void event(Trace *trace, StarsI16 value)
{
    if (trace->event_count < 12)
        trace->events[trace->event_count] = value;
    ++trace->event_count;
}

static StarsI16 plan(void *opaque, const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 turn,
    StarsAutomitronFleetPlan *out)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->plan_calls;
    event(trace, fleet->id == 2 ? 1 : 4);
    if (turn != 150 || policy->total_ships == 0)
        return 0;
    out->target_planet = fleet->id == 2 ? 7 : 9;
    out->order_kind = fleet->id == 2 ? 2 : 1;
    out->append = fleet->id == 2 ? 0 : 1;
    out->reason = fleet->id == 2 ? 41 : 42;
    out->action = fleet->id == 2 ?
        STARS_AUTOMITRON_FLEET_ACTION_COLONIZE :
        STARS_AUTOMITRON_FLEET_ACTION_MOVE;
    return trace->reject_plan ? 0 : 1;
}

static StarsI16 apply(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPlan *plan_in)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->apply_calls;
    event(trace, 2);
    if ((fleet->id == 2 && plan_in->action ==
            STARS_AUTOMITRON_FLEET_ACTION_COLONIZE &&
            plan_in->target_planet == 7 && plan_in->order_kind == 2 &&
            plan_in->append == 0 && plan_in->reason == 41) ||
            (fleet->id == 3 && plan_in->action ==
            STARS_AUTOMITRON_FLEET_ACTION_MOVE &&
            plan_in->target_planet == 9 && plan_in->order_kind == 1 &&
            plan_in->append == 1 && plan_in->reason == 42))
        ++trace->valid;
    return trace->reject_apply ? 0 : 1;
}

static StarsI16 visit(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 potency)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->visit_calls;
    event(trace, fleet->id == 2 ? 3 : 5);
    if ((fleet->id == 2 || fleet->id == 3) && policy->total_ships != 0 &&
            potency == 4)
        ++trace->valid;
    return 1;
}

static StarsI16 sequence_ok(const Trace *trace)
{
    return trace->event_count == 6 && trace->events[0] == 1 &&
        trace->events[1] == 2 && trace->events[2] == 3 &&
        trace->events[3] == 4 && trace->events[4] == 2 &&
        trace->events[5] == 5;
}

int main(void)
{
    StarsAutomitronTurnFleet fleets[3];
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronAiTurnInput input;
    Trace trace;
    StarsI16 failures;

    memset(fleets, 0, sizeof fleets);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleets[0].id = 1;
    fleets[0].owner = 7;
    fleets[0].ship_counts[2] = 1;
    fleets[1].id = 2;
    fleets[1].owner = 4;
    fleets[1].ship_counts[0] = 1;
    fleets[2].id = 3;
    fleets[2].owner = 4;
    fleets[2].ship_counts[2] = 1;
    ops.plan_fleet_action = plan;
    ops.apply_fleet_action = apply;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 3;
    input.fleets = fleets;
    input.ops = &ops;

    failures = !stars_do_automitron_ai_turn(&input);
    failures += trace.plan_calls != 2 || trace.apply_calls != 2 ||
        trace.visit_calls != 2 || trace.valid != 4;
    failures += !sequence_ok(&trace);
    memset(&trace, 0, sizeof trace);
    trace.reject_plan = 1;
    failures += stars_do_automitron_ai_turn(&input) != 0;
    failures += trace.plan_calls != 1 || trace.apply_calls != 0 ||
        trace.visit_calls != 0;
    memset(&trace, 0, sizeof trace);
    trace.reject_apply = 1;
    failures += stars_do_automitron_ai_turn(&input) != 0;
    failures += trace.plan_calls != 1 || trace.apply_calls != 1 ||
        trace.visit_calls != 0;

    printf("AUTOMITRON_AI_ROUTE_PLAN_CHECKS=14 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
