#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 valid;
} Trace;

static StarsI16 visit(void *opaque, const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->calls;
    if (fleet->owner == 4 && potency[0] == 5 && potency[2] == 8 &&
            policy->total_ships == 12 && policy->colonizer_ships == 1 &&
            policy->transport_ships == 2 && policy->armed_ships == 3 &&
            policy->support_design_ships == 3 &&
            policy->attack_design_ships == 3 &&
            !policy->needs_order_repair)
        ++trace->valid;
    return 1;
}

int main(void)
{
    StarsRobotoidAiTurnOps ops;
    StarsRobotoidAiTurnInput input;
    StarsRobotoidTurnFleet fleet;
    Trace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    fleet.owner = 4;
    fleet.cord = 0;
    fleet.order_kind = 0;
    fleet.ship_counts[0] = 1;
    fleet.ship_counts[1] = 2;
    fleet.ship_counts[2] = 3;
    fleet.ship_counts[0xc] = 2;
    fleet.ship_counts[0xd] = 1;
    fleet.ship_counts[0xe] = 2;
    fleet.ship_counts[0xf] = 1;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    failures = !stars_do_robotoid_ai_turn(&input);
    failures += trace.calls != 1 || trace.valid != 1;
    fleet.owner = 7;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_robotoid_ai_turn(&input) || trace.calls != 0;
    printf("ROBOTOID_AI_TYPED_FLEET_CHECKS=2 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
