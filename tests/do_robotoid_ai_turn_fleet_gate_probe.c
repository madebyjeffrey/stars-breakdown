#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 failures;
} Trace;

static StarsI16 inspect_fleet(void *opaque,
    const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->calls;
    if (potency[0] != 5 || potency[2] != 8)
        ++trace->failures;
    if (fleet->id == 1) {
        if (policy->total_ships != 0 || policy->needs_order_repair)
            ++trace->failures;
    } else if (fleet->id == 2) {
        if (policy->total_ships != 1 || policy->colonizer_ships != 1 ||
                !policy->needs_order_repair)
            ++trace->failures;
    } else if (fleet->id == 3) {
        if (policy->total_ships != 1 || policy->transport_ships != 1 ||
                policy->needs_order_repair)
            ++trace->failures;
    } else if (fleet->id == 4) {
        if (policy->total_ships != 1 || policy->armed_ships != 1 ||
                policy->needs_order_repair)
            ++trace->failures;
    } else if (fleet->id == 5) {
        if (policy->total_ships != 3 || policy->support_design_ships != 1 ||
                policy->attack_design_ships != 1 ||
                policy->needs_order_repair)
            ++trace->failures;
    } else if (fleet->id == 6) {
        if (policy->total_ships != 2 || policy->needs_order_repair)
            ++trace->failures;
    } else {
        ++trace->failures;
    }
    return 1;
}

int main(void)
{
    StarsRobotoidTurnFleet fleets[7];
    StarsRobotoidAiTurnOps ops;
    StarsRobotoidAiTurnInput input;
    Trace trace;
    StarsI16 failures;
    StarsI16 i;

    memset(fleets, 0, sizeof fleets);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    for (i = 0; i < 7; ++i) {
        fleets[i].id = (StarsI16)(i + 1);
        fleets[i].owner = 4;
    }
    /* Empty friendly fleet: no repair request. */
    fleets[0].cord = 0;
    fleets[0].order_kind = 0;
    /* In-orbit colonizer and transport fleets with no order. */
    fleets[1].cord = 1;
    fleets[1].ship_counts[0] = 1;
    fleets[2].cord = 0;
    fleets[2].ship_counts[1] = 1;
    /* A fleet already carrying an order is not marked for repair. */
    fleets[3].cord = 0;
    fleets[3].order_kind = 5;
    fleets[3].ship_counts[2] = 1;
    /* Support and attack design bins remain separately observable. */
    fleets[4].cord = 0;
    fleets[4].order_kind = 4;
    fleets[4].ship_counts[0xc] = 1;
    fleets[4].ship_counts[0xf] = 1;
    fleets[4].ship_counts[1] = 1;
    /* The raw fleet count fallback remains part of the typed boundary. */
    fleets[5].cord = 1;
    fleets[5].order_kind = 0;
    fleets[5].warship_count = 2;
    /* Non-owned fleets must not reach the typed order seam. */
    fleets[6].owner = 7;
    fleets[6].ship_counts[2] = 1;

    ops.visit_fleet_typed = inspect_fleet;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 7;
    input.fleets = fleets;
    input.ops = &ops;
    failures = !stars_do_robotoid_ai_turn(&input);
    failures += trace.calls != 6;
    failures += trace.failures;
    printf("ROBOTOID_AI_FLEET_GATE_CHECKS=10 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
