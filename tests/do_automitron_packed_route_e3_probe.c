#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 mode;
    StarsI16 plans;
    StarsI16 refreshes;
    StarsI16 visits;
    StarsI16 specialized;
    StarsI16 expected_count;
    StarsI16 refreshed_exact;
    StarsMoveOrder order;
} Trace;

static StarsI16 plan(void *opaque, const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 turn,
    StarsAutomitronFleetPlan *out)
{
    Trace *trace;
    (void)fleet;
    (void)policy;
    (void)turn;
    trace = (Trace *)opaque;
    ++trace->plans;
    out->action = trace->mode == 2 ?
        STARS_AUTOMITRON_FLEET_ACTION_COLONIZE :
        STARS_AUTOMITRON_FLEET_ACTION_MOVE;
    out->append = trace->mode == 2;
    out->packed_order_valid = 1;
    out->packed_order = trace->order;
    return 1;
}

static StarsI16 refresh(void *opaque, const StarsAutomitronTurnFleet *fleet)
{
    Trace *trace;
    StarsU16 offset;
    trace = (Trace *)opaque;
    ++trace->refreshes;
    offset = (StarsU16)(STARS_AI_PLORD_HEADER_SIZE +
        (trace->expected_count - 1) * STARS_AI_PLORD_ORDER_SIZE);
    if (fleet->raw_plord[3] == trace->expected_count &&
            memcmp(fleet->raw_plord + offset, &trace->order,
                sizeof trace->order) == 0)
        ++trace->refreshed_exact;
    return 1;
}

static StarsI16 visit(void *opaque, const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 potency)
{
    Trace *trace;
    (void)fleet;
    (void)policy;
    (void)potency;
    trace = (Trace *)opaque;
    ++trace->visits;
    return 1;
}

static StarsI16 specialized(void *opaque,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPlan *plan_in)
{
    Trace *trace;
    (void)fleet;
    (void)plan_in;
    trace = (Trace *)opaque;
    ++trace->specialized;
    return 1;
}

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnFleet fleet;
    StarsU8 raw[STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE * 3U];
    StarsU8 before[sizeof raw];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    memset(raw, 0x5a, sizeof raw);
    raw[3] = 1;
    fleet.id = 7;
    fleet.owner = 4;
    fleet.ship_counts[2] = 1;
    fleet.raw_plord = raw;
    fleet.raw_plord_bytes = (StarsU16)sizeof raw;
    ops.plan_fleet_action = plan;
    ops.visit_fleet_typed = visit;
    ops.refresh_packed_order = refresh;
    ops.context = &trace;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    trace.expected_count = 2;
    trace.order.point.x = 123;
    trace.order.point.y = 456;
    trace.order.id = 10;
    trace.order.flags = 0x1140U;
    memset(trace.order.reserved, 0x33, sizeof trace.order.reserved);
    memcpy(before, raw, sizeof before);
    check(sizeof(StarsMoveOrder) == 18U, &checks, &failures);
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.plans == 1 && trace.refreshes == 1 &&
        trace.visits == 1 && trace.refreshed_exact == 1,
        &checks, &failures);
    check(raw[3] == 2 && memcmp(raw + 4, before + 4, 18) == 0,
        &checks, &failures);

    trace.mode = 2;
    trace.expected_count = 3;
    trace.order.point.x = 700;
    trace.order.point.y = 800;
    trace.order.id = 11;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(raw[3] == 3 && trace.refreshes == 2 &&
        trace.refreshed_exact == 2 && trace.visits == 2,
        &checks, &failures);

    memcpy(before, raw, sizeof before);
    trace.mode = 1;
    fleet.raw_plord_bytes = STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(memcmp(raw, before, sizeof raw) == 0 &&
        trace.refreshes == 2, &checks, &failures);

    fleet.raw_plord_bytes = (StarsU16)sizeof raw;
    ops.move = specialized;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.specialized == 1 && trace.refreshes == 2 &&
        memcmp(raw, before, sizeof raw) == 0, &checks, &failures);

    printf("AUTOMITRON_PACKED_ROUTE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
