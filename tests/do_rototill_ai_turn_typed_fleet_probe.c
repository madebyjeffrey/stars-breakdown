#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 valid;
    StarsI16 repairs;
} Trace;

static StarsI16 repair(void *opaque, const StarsRototillTurnFleet *fleet,
    StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (fleet->owner != player_id)
        return 0;
    ++trace->repairs;
    return 1;
}

static StarsI16 visit(void *opaque, const StarsRototillTurnFleet *fleet,
    const StarsRototillFleetPolicy *policy, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->calls;
    if (fleet->owner == player_id && policy->total_ships == 6 &&
            policy->colonizer_ships == 1 && policy->transport_ships == 2 &&
            policy->armed_ships == 3 && policy->needs_order_repair)
        ++trace->valid;
    return 1;
}

int main(void)
{
    StarsRototillAiTurnOps ops;
    StarsRototillAiTurnInput input;
    StarsRototillTurnFleet fleet;
    Trace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    ops.visit_fleet_typed = visit;
    ops.repair_order = repair;
    ops.context = &trace;
    fleet.owner = 4;
    fleet.cord = 0;
    fleet.order_kind = 0;
    fleet.ship_counts[0] = 1;
    fleet.ship_counts[1] = 2;
    fleet.ship_counts[2] = 3;
    input.turn = 30;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    failures = !stars_do_rototill_ai_turn(&input);
    failures += trace.calls != 1 || trace.valid != 1 || trace.repairs != 1;
    fleet.owner = 7;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_rototill_ai_turn(&input) || trace.calls != 0;
    printf("ROTOTILL_AI_TYPED_FLEET_CHECKS=3 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
