#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 repair_calls;
    StarsI16 visit_calls;
    StarsI16 valid;
    StarsI16 mutation_pass;
    StarsI16 reject;
} Trace;

static StarsI16 repair(void *opaque,
    const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->repair_calls;
    if (fleet->id == 2 && policy->needs_order_repair &&
            potency[0] == 5 && potency[2] == 8)
        ++trace->valid;
    if (fleet->raw_plord &&
            fleet->raw_plord[3] == 1U &&
            (fleet->raw_plord[10] & 0x0fU) == 6U &&
            fleet->raw_plord[12] == 5U && fleet->raw_plord[14] == 5U)
        ++trace->mutation_pass;
    return trace->reject ? 0 : 1;
}

static StarsI16 visit(void *opaque,
    const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->visit_calls;
    if (fleet->id == 2 && policy->needs_order_repair && potency[0] == 5)
        ++trace->valid;
    return 1;
}

int main(void)
{
    StarsRobotoidTurnFleet fleets[7];
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
    fleets[0].owner = 4;
    fleets[0].cord = 0;
    fleets[0].order_kind = 5;
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
    fleets[3].id = 4;
    fleets[3].owner = 4;
    fleets[3].cord = 1;
    fleets[3].order_kind = 5;
    fleets[3].ship_counts[0] = 1;
    fleets[4].id = 5;
    fleets[4].owner = 4;
    fleets[4].cord = 0;
    fleets[4].ship_counts[0] = 1;
    fleets[5].id = 6;
    fleets[5].owner = 4;
    fleets[5].cord = 1;
    fleets[5].ship_counts[2] = 1;
    fleets[6].id = 7;
    fleets[6].owner = 7;
    fleets[6].cord = 1;
    fleets[6].ship_counts[0] = 1;
    ops.repair_order = repair;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 7;
    input.fleets = fleets;
    input.ops = &ops;
    failures = !stars_do_robotoid_ai_turn(&input);
    failures += trace.repair_calls != 1;
    failures += trace.visit_calls != 6;
    failures += trace.valid != 2;
    failures += trace.mutation_pass != 1;

    memset(&trace, 0, sizeof trace);
    trace.reject = 1;
    failures += stars_do_robotoid_ai_turn(&input) != 0;
    failures += trace.repair_calls != 1 || trace.visit_calls != 1;
    memset(&trace, 0, sizeof trace);
    input.turn = 40;
    failures += !stars_do_robotoid_ai_turn(&input);
    failures += trace.repair_calls != 0 || trace.visit_calls != 6 ||
        trace.valid != 0;
    printf("ROBOTOID_AI_REPAIR_BOUNDARY_CHECKS=7 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
