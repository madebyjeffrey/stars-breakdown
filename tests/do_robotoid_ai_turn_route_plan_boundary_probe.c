#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 repair_calls;
    StarsI16 plan_calls;
    StarsI16 apply_calls;
    StarsI16 visit_calls;
    StarsI16 reject_plan;
    StarsI16 reject_apply;
    StarsI16 plan_noop;
    StarsI16 valid;
    StarsI16 events[8];
    StarsI16 event_count;
} Trace;

static void event(Trace *trace, StarsI16 value)
{
    if (trace->event_count < 8)
        trace->events[trace->event_count] = value;
    ++trace->event_count;
}

static StarsI16 repair(void *opaque,
    const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->repair_calls;
    event(trace, 1);
    if (fleet->id == 2 && policy->needs_order_repair &&
            potency[0] == 5U && potency[2] == 8U && fleet->raw_plord &&
            fleet->raw_plord[3] == 1U &&
            (fleet->raw_plord[10] & 0x0fU) == 6U &&
            fleet->raw_plord[12] == 5U && fleet->raw_plord[14] == 5U)
        ++trace->valid;
    return 1;
}

static StarsI16 plan(void *opaque,
    const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, StarsI16 turn,
    StarsRobotoidFleetPlan *plan_out)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->plan_calls;
    event(trace, fleet->id == 2 ? 2 : 5);
    if (fleet->id == 2) {
        if (policy->colonizer_ships != 1 || policy->total_ships != 1 ||
                !policy->needs_order_repair || turn != 150)
            return 0;
        plan_out->action = trace->plan_noop ?
            STARS_ROBOTOID_FLEET_ACTION_NONE :
            STARS_ROBOTOID_FLEET_ACTION_COLONIZE;
        plan_out->target_planet = 7;
        plan_out->order_kind = 2;
        plan_out->append = 0;
        plan_out->reason = 41;
    } else if (fleet->id == 3) {
        if (policy->armed_ships != 1 || policy->needs_order_repair ||
                turn != 150)
            return 0;
        plan_out->action = STARS_ROBOTOID_FLEET_ACTION_NONE;
    } else {
        return 0;
    }
    return trace->reject_plan ? 0 : 1;
}

static StarsI16 apply(void *opaque,
    const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPlan *plan_in)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->apply_calls;
    event(trace, 3);
    if (fleet->id == 2 && plan_in->action ==
            STARS_ROBOTOID_FLEET_ACTION_COLONIZE &&
            plan_in->target_planet == 7 && plan_in->order_kind == 2 &&
            plan_in->append == 0 && plan_in->reason == 41)
        ++trace->valid;
    return trace->reject_apply ? 0 : 1;
}

static StarsI16 visit(void *opaque,
    const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->visit_calls;
    event(trace, fleet->id == 2 ? 4 : 6);
    if (fleet->id == 2 && policy->needs_order_repair && potency[0] == 5U)
        ++trace->valid;
    if (fleet->id == 3 && !policy->needs_order_repair && potency[2] == 8U)
        ++trace->valid;
    return 1;
}

static StarsI16 sequence_ok(const Trace *trace)
{
    if (trace->plan_noop)
        return trace->event_count == 5 && trace->events[0] == 2 &&
            trace->events[1] == 1 && trace->events[2] == 4 &&
            trace->events[3] == 5 && trace->events[4] == 6;
    return trace->event_count == 5 && trace->events[0] == 2 &&
        trace->events[1] == 3 && trace->events[2] == 4 &&
        trace->events[3] == 5 && trace->events[4] == 6;
}

int main(void)
{
    StarsRobotoidTurnFleet fleets[3];
    StarsRobotoidAiTurnOps ops;
    StarsRobotoidAiTurnInput input;
    Trace trace;
    StarsI16 failures;
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE + STARS_AI_PLORD_ORDER_SIZE];

    memset(fleets, 0, sizeof fleets);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    memset(plord, 0, sizeof plord);
    plord[3] = 1U;
    fleets[0].id = 1;
    fleets[0].owner = 9;
    fleets[0].ship_counts[2] = 1;
    fleets[1].id = 2;
    fleets[1].owner = 4;
    fleets[1].cord = 1;
    fleets[1].raw_plord = plord;
    fleets[1].ship_counts[0] = 1;
    fleets[2].id = 3;
    fleets[2].owner = 4;
    fleets[2].cord = 0;
    fleets[2].order_kind = 5;
    fleets[2].ship_counts[2] = 1;
    ops.repair_order = repair;
    ops.plan_fleet_action = plan;
    ops.colonize = apply;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 3;
    input.fleets = fleets;
    input.ops = &ops;

    failures = !stars_do_robotoid_ai_turn(&input);
    failures += trace.repair_calls != 0;
    failures += trace.plan_calls != 2;
    failures += trace.apply_calls != 1;
    failures += trace.visit_calls != 2;
    failures += trace.valid != 3;
    failures += !sequence_ok(&trace);

    memset(&trace, 0, sizeof trace);
    trace.plan_noop = 1;
    failures += !stars_do_robotoid_ai_turn(&input);
    failures += trace.repair_calls != 1 || trace.apply_calls != 0 ||
        trace.visit_calls != 2;
    failures += trace.valid != 3;
    failures += !sequence_ok(&trace);

    memset(&trace, 0, sizeof trace);
    trace.reject_plan = 1;
    failures += stars_do_robotoid_ai_turn(&input) != 0;
    failures += trace.plan_calls != 1 || trace.apply_calls != 0 ||
        trace.visit_calls != 0;

    memset(&trace, 0, sizeof trace);
    trace.reject_apply = 1;
    failures += stars_do_robotoid_ai_turn(&input) != 0;
    failures += trace.plan_calls != 1 || trace.apply_calls != 1 ||
        trace.visit_calls != 0;

    printf("ROBOTOID_AI_ROUTE_PLAN_BOUNDARY_CHECKS=18 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
