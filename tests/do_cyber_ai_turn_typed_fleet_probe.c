#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 valid;
} Trace;

static StarsI16 visit(void *opaque, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 armada_potency)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->calls;
    if (fleet->owner == 4 && armada_potency == 4 &&
            policy->total_ships == 15 && policy->colonizer_ships == 1 &&
            policy->transport_ships == 2 && policy->armed_ships == 3 &&
            policy->auxiliary_design_ships == 3 &&
            policy->support_design_ships == 3 &&
            policy->attack_design_ships == 3 &&
            policy->needs_order_repair)
        ++trace->valid;
    return 1;
}

static StarsI16 merge(void *opaque, StarsI16 mask)
{
    (void)opaque;
    (void)mask;
    return 1;
}

int main(void)
{
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    StarsCyberTurnFleet fleet;
    Trace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    ops.merge_shdefs = merge;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    fleet.owner = 4;
    fleet.cord = 0;
    fleet.order_kind = 0;
    fleet.ship_counts[0] = 1;
    fleet.ship_counts[1] = 2;
    fleet.ship_counts[2] = 3;
    fleet.ship_counts[10] = 2;
    fleet.ship_counts[11] = 1;
    fleet.ship_counts[0xc] = 2;
    fleet.ship_counts[0xd] = 1;
    fleet.ship_counts[0xe] = 2;
    fleet.ship_counts[0xf] = 1;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    failures = !stars_do_cyber_ai_turn(&input);
    failures += trace.calls != 1 || trace.valid != 1;
    fleet.owner = 7;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_cyber_ai_turn(&input) || trace.calls != 0;
    printf("CYBER_AI_TYPED_FLEET_CHECKS=2 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
