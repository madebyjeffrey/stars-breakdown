#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 merge_calls;
    StarsI16 repair_calls;
    StarsI16 visit_calls;
    StarsI16 valid;
    StarsI16 reject;
} Trace;

static StarsI16 merge(void *opaque, StarsI16 mask)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (mask == 1 || mask == 0x30 || mask == -0x4000 ||
            mask == 0x03c0 || mask == 0x3c00)
        ++trace->merge_calls;
    return 1;
}

static StarsI16 repair(void *opaque,
    const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->repair_calls;
    if (fleet->id == 2 && policy->needs_order_repair && potency == 4)
        ++trace->valid;
    return trace->reject ? 0 : 1;
}

static StarsI16 visit(void *opaque,
    const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency)
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
    StarsCyberTurnFleet fleets[3];
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    Trace trace;
    StarsI16 failures;

    memset(fleets, 0, sizeof fleets);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleets[0].id = 1;
    fleets[0].owner = 4;
    fleets[0].order_kind = 5;
    fleets[0].ship_counts[2] = 1;
    fleets[1].id = 2;
    fleets[1].owner = 4;
    fleets[1].cord = 1;
    fleets[1].ship_counts[0] = 1;
    fleets[2].id = 3;
    fleets[2].owner = 7;
    fleets[2].ship_counts[2] = 1;
    ops.merge_shdefs = merge;
    ops.repair_order = repair;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 3;
    input.fleets = fleets;
    input.ops = &ops;
    failures = !stars_do_cyber_ai_turn(&input);
    failures += trace.merge_calls != 5;
    failures += trace.repair_calls != 1;
    failures += trace.visit_calls != 2;
    failures += trace.valid != 2;

    memset(&trace, 0, sizeof trace);
    trace.reject = 1;
    failures += stars_do_cyber_ai_turn(&input) != 0;
    failures += trace.repair_calls != 1 || trace.visit_calls != 1;
    printf("CYBER_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
