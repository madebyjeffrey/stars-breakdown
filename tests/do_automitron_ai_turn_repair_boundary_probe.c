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
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->repair_calls;
    if (fleet->id == 2 && policy->needs_order_repair &&
            policy->total_ships == 1 && policy->slot_6_ships == 1 &&
            potency[0] == 4U &&
            fleet->raw_plord && fleet->raw_plord[3] == 1U &&
            (fleet->raw_plord[10] & 0x0fU) == 6U &&
            fleet->raw_plord[12] == 5U && fleet->raw_plord[14] == 5U)
        ++trace->valid;
    return trace->reject ? 0 : 1;
}

static StarsI16 visit(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 potency)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->visit_calls;
    if (fleet->id == 2 && policy->needs_order_repair && potency == 4)
        ++trace->valid;
    return 1;
}

int main(void)
{
    StarsAutomitronTurnFleet fleets[4];
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronAiTurnInput input;
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE * 2U];
    Trace trace;
    StarsI16 failures;

    memset(fleets, 0, sizeof fleets);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(plord, 0xa5, sizeof plord);
    memset(&trace, 0, sizeof trace);
    plord[3] = 1U;
    fleets[0].id = 1;
    fleets[0].owner = 7;
    fleets[0].ship_counts[2] = 1;
    fleets[1].id = 2;
    fleets[1].owner = 4;
    fleets[1].cord = 1;
    fleets[1].raw_plord = plord;
    fleets[1].ship_counts[6] = 1;
    fleets[2].id = 3;
    fleets[2].owner = 4;
    fleets[2].cord = 1;
    fleets[2].order_kind = 5;
    fleets[2].ship_counts[6] = 1;
    fleets[3].id = 4;
    fleets[3].owner = 4;
    fleets[3].cord = 1;
    fleets[3].raw_plord = plord;
    fleets[3].ship_counts[2] = 1;
    ops.repair_order = repair;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 4;
    input.fleets = fleets;
    input.ops = &ops;

    failures = !stars_do_automitron_ai_turn(&input);
    failures += trace.repair_calls != 1;
    failures += trace.visit_calls != 3;
    failures += trace.valid != 2;
    failures += (plord[10] & 0x0fU) != 6U || plord[12] != 5U ||
        plord[14] != 5U;
    memset(&trace, 0, sizeof trace);
    trace.reject = 1;
    failures += stars_do_automitron_ai_turn(&input) != 0;
    failures += trace.repair_calls != 1 || trace.visit_calls != 0;

    printf("AUTOMITRON_AI_REPAIR_BOUNDARY_CHECKS=8 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
