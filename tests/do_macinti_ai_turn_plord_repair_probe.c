#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 visited;
    StarsI16 saw_repaired_order;
    StarsI16 saw_policy_repair;
} Trace;

static StarsI16 visit_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;

    trace = (Trace *)opaque;
    (void)potency;
    if (fleet->id != 8 || fleet->raw_plord == 0)
        return 0;
    ++trace->visited;
    trace->saw_policy_repair = policy->needs_order_repair;
    trace->saw_repaired_order = fleet->raw_plord[3] == 1U &&
        (StarsU16)(fleet->raw_plord[10] |
            ((StarsU16)fleet->raw_plord[11] << 8)) == 0xabf6U &&
        fleet->raw_plord[12] == 5U && fleet->raw_plord[13] == 0U &&
        fleet->raw_plord[14] == 5U && fleet->raw_plord[15] == 0U;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnOps ops;
    StarsMacintiAiTurnInput input;
    StarsMacintiTurnFleet fleet;
    StarsU8 plord[22];
    Trace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&fleet, 0, sizeof fleet);
    memset(plord, 0, sizeof plord);
    memset(&trace, 0, sizeof trace);
    plord[0] = 0xa5U;
    plord[1] = 0x5aU;
    plord[2] = 0x3cU;
    plord[3] = 1U;
    plord[10] = 0xf0U;
    plord[11] = 0xabU;
    fleet.id = 8;
    fleet.owner = 3;
    fleet.raw_plord = plord;
    fleet.ship_counts[2] = 2U;
    ops.visit_fleet_typed = visit_fleet;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 3;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    failures = !stars_do_macinti_ai_turn(&input);
    failures += trace.visited != 1;
    failures += !trace.saw_policy_repair;
    failures += !trace.saw_repaired_order;
    failures += plord[0] != 0xa5U || plord[1] != 0x5aU ||
        plord[2] != 0x3cU;
    printf("MACINTI_AI_PLORD_REPAIR_CHECKS=5 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
