#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 planned;
    StarsI16 applied;
    StarsI16 first_action;
    StarsI16 second_action;
    StarsI16 first_target;
    StarsI16 routes;
    StarsI16 finalizations;
    StarsI16 basic_tasks;
    StarsI16 filled;
} Trace;

static StarsI16 visit_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    (void)opaque;
    (void)fleet;
    (void)policy;
    (void)potency;
    return 1;
}

static StarsI16 plan_action(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, StarsI16 turn,
    StarsMacintiFleetPlan *plan)
{
    Trace *trace = (Trace *)opaque;
    (void)policy;
    (void)turn;
    ++trace->planned;
    if (fleet->id == 8) {
        plan->action = STARS_MACINTI_FLEET_ACTION_ATTACK;
        plan->target_planet = 12;
        plan->reason = 7;
    } else {
        plan->action = STARS_MACINTI_FLEET_ACTION_COLONIZE;
        plan->target_planet = 19;
        plan->reason = 4;
    }
    return 1;
}

static StarsI16 apply_action(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPlan *plan)
{
    Trace *trace = (Trace *)opaque;
    (void)fleet;
    ++trace->applied;
    if (trace->applied == 1) {
        trace->first_action = plan->action;
        trace->first_target = plan->target_planet;
    } else
        trace->second_action = plan->action;
    return 1;
}

static StarsI16 attack_action(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPlan *plan)
{
    Trace *trace = (Trace *)opaque;
    (void)fleet;
    ++trace->applied;
    trace->first_action = plan->action;
    trace->first_target = plan->target_planet;
    return 1;
}

static StarsI16 route_transport(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace = (Trace *)opaque;
    (void)fleet;
    (void)policy;
    (void)potency;
    ++trace->routes;
    return 1;
}

static StarsI16 finalize_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace = (Trace *)opaque;
    (void)fleet;
    (void)policy;
    (void)potency;
    ++trace->finalizations;
    return 1;
}

static StarsI16 phase(void *opaque)
{
    Trace *trace = (Trace *)opaque;
    ++trace->basic_tasks;
    return 1;
}

static StarsI16 fill(void *opaque)
{
    Trace *trace = (Trace *)opaque;
    ++trace->filled;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnOps ops;
    StarsMacintiAiTurnInput input;
    StarsMacintiTurnFleet fleets[3];
    StarsU8 raw_fleet[3];
    Trace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(fleets, 0, sizeof fleets);
    memset(raw_fleet, 0, sizeof raw_fleet);
    memset(&trace, 0, sizeof trace);
    fleets[0].id = 8;
    fleets[0].owner = 3;
    fleets[0].raw_record = raw_fleet;
    fleets[0].point.x = 31;
    fleets[0].ship_counts[2] = 3;
    fleets[0].ship_counts[1] = 1;
    fleets[1].id = 9;
    fleets[1].owner = 3;
    fleets[1].ship_counts[0] = 1;
    fleets[2].id = 10;
    fleets[2].owner = 4;
    fleets[2].ship_counts[2] = 5;
    ops.visit_fleet_typed = visit_fleet;
    ops.plan_fleet_action = plan_action;
    ops.apply_fleet_action = apply_action;
    ops.attack = attack_action;
    ops.route_transport_fleet = route_transport;
    ops.finalize_fleet = finalize_fleet;
    ops.handle_basic_tasks = phase;
    ops.fill_production_queue = fill;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 3;
    input.fleet_count = 3;
    input.fleets = fleets;
    input.ops = &ops;
    failures = !stars_do_macinti_ai_turn(&input);
    failures += trace.planned != 2 || trace.applied != 2;
    failures += fleets[0].raw_record != 0 && fleets[0].point.x == 31 ? 0 : 1;
    failures += trace.first_action != STARS_MACINTI_FLEET_ACTION_ATTACK ||
        trace.first_target != 12 ||
        trace.second_action != STARS_MACINTI_FLEET_ACTION_COLONIZE;
    failures += input.typed_fleet_actions != 2 ||
        input.typed_fleet_actions_rejected != 0 ||
        input.typed_fleets_visited != 2;
    failures += trace.routes != 1 || trace.finalizations != 2 ||
        trace.basic_tasks != 1 || trace.filled != 1;
    failures += input.typed_transport_routes != 1 ||
        input.typed_fleet_finalizations != 2 ||
        input.basic_tasks_run != 1 || input.production_queue_filled != 1;
    printf("MACINTI_AI_FLEET_ACTION_CHECKS=3 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
