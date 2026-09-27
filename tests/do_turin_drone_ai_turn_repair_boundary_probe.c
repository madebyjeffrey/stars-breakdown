#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 repair_calls;
    StarsI16 visit_calls;
    StarsI16 valid;
    StarsI16 reject;
} Trace;

static StarsI16 repair(void *opaque,
    const StarsTurinTurnFleet *fleet,
    const StarsTurinFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->repair_calls;
    if (fleet->id == 2 && policy->needs_order_repair &&
            policy->support_design_ships == 1 && potency[0] == 4 &&
            potency[2] == 8)
        ++trace->valid;
    return trace->reject ? 0 : 1;
}

static StarsI16 visit(void *opaque,
    const StarsTurinTurnFleet *fleet,
    const StarsTurinFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->visit_calls;
    if (fleet->id == 2 && policy->needs_order_repair && potency[0] == 4)
        ++trace->valid;
    return 1;
}

int main(void)
{
    StarsTurinTurnFleet fleets[15];
    StarsTurinDroneAiTurnOps ops;
    StarsTurinDroneAiTurnInput input;
    Trace trace;
    StarsI16 failures;

    memset(fleets, 0, sizeof fleets);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleets[0].id = 2;
    fleets[0].owner = 4;
    fleets[0].cord = 1;
    fleets[0].ship_counts[12] = 1;
    fleets[1].id = 3;
    fleets[1].owner = 4;
    fleets[1].cord = 1;
    fleets[1].ship_counts[12] = 1;
    fleets[1].ship_counts[1] = 1;
    fleets[2].id = 4;
    fleets[2].owner = 4;
    fleets[2].cord = 1;
    fleets[2].ship_counts[12] = 1;
    fleets[2].ship_counts[2] = 1;
    fleets[3].id = 5;
    fleets[3].owner = 4;
    fleets[3].cord = 1;
    fleets[3].ship_counts[12] = 1;
    fleets[3].ship_counts[3] = 1;
    fleets[4].id = 6;
    fleets[4].owner = 4;
    fleets[4].cord = 1;
    fleets[4].ship_counts[12] = 1;
    fleets[4].ship_counts[8] = 1;
    fleets[5].id = 7;
    fleets[5].owner = 4;
    fleets[5].cord = 1;
    fleets[5].ship_counts[12] = 1;
    fleets[5].ship_counts[9] = 1;
    fleets[6].id = 8;
    fleets[6].owner = 4;
    fleets[6].cord = 1;
    fleets[6].ship_counts[12] = 1;
    fleets[6].ship_counts[13] = 1;
    fleets[7].id = 9;
    fleets[7].owner = 4;
    fleets[7].cord = 1;
    fleets[7].ship_counts[12] = 1;
    fleets[7].ship_counts[14] = 1;
    fleets[8].id = 10;
    fleets[8].owner = 4;
    fleets[8].cord = 1;
    fleets[8].ship_counts[12] = 1;
    fleets[8].ship_counts[0] = 1;
    fleets[9].id = 11;
    fleets[9].owner = 4;
    fleets[9].cord = 1;
    fleets[9].ship_counts[12] = 1;
    fleets[9].ship_counts[10] = 1;
    fleets[10].id = 12;
    fleets[10].owner = 4;
    fleets[10].cord = 1;
    fleets[10].ship_counts[12] = 1;
    fleets[10].ship_counts[11] = 1;
    fleets[11].id = 13;
    fleets[11].owner = 4;
    fleets[11].cord = 2;
    fleets[11].ship_counts[12] = 1;
    fleets[12].id = 14;
    fleets[12].owner = 4;
    fleets[12].cord = 1;
    fleets[12].order_kind = 5;
    fleets[12].ship_counts[12] = 1;
    fleets[13].id = 15;
    fleets[13].owner = 7;
    fleets[13].cord = 1;
    fleets[13].ship_counts[12] = 1;
    fleets[14].id = 16;
    fleets[14].owner = 4;
    fleets[14].cord = 1;
    fleets[14].ship_counts[4] = 1;
    ops.repair_order = repair;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 15;
    input.fleets = fleets;
    input.ops = &ops;
    failures = !stars_do_turin_drone_ai_turn(&input);
    failures += trace.repair_calls != 1;
    failures += trace.visit_calls != 14;
    failures += trace.valid != 2;

    memset(&trace, 0, sizeof trace);
    trace.reject = 1;
    failures += stars_do_turin_drone_ai_turn(&input) != 0;
    failures += trace.repair_calls != 1 || trace.visit_calls != 0;
    printf("TURIN_DRONE_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
