#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 action;
    StarsI16 reject_apply;
    StarsI16 append;
    StarsI16 calls;
} Trace;

static StarsI16 merge(void *context, StarsI16 mask)
{
    (void)context;
    (void)mask;
    return 1;
}

static StarsI16 plan(void *context, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency,
    StarsCyberFleetPlan *result)
{
    Trace *trace;
    trace = (Trace *)context;
    if (fleet->id != 5 || policy->total_ships != 1 || potency != 4U)
        return 0;
    result->action = trace->action;
    result->target_planet = 9;
    result->append = trace->append;
    return 1;
}

static StarsI16 apply(void *context, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPlan *result)
{
    Trace *trace;
    StarsMoveOrder order;
    trace = (Trace *)context;
    ++trace->calls;
    if (trace->reject_apply)
        return 0;
    memset(&order, 0, sizeof order);
    order.id = result->target_planet;
    order.point.x = 90;
    order.point.y = 91;
    order.flags = 0x1146U;
    return stars_ai_move_order_to_plord(fleet->raw_plord, 4U,
        &order, result->append);
}

static StarsI16 visit(void *context, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency)
{
    (void)context;
    (void)fleet;
    (void)policy;
    (void)potency;
    return 1;
}

static void seed(StarsU8 *raw, StarsI16 first, StarsI16 second)
{
    StarsMoveOrder order;
    memset(raw, 0xa5, 4U + 4U * 18U);
    raw[3] = 0;
    memset(&order, 0, sizeof order);
    order.id = first;
    order.point.x = 10;
    order.point.y = 11;
    memcpy(raw + 4U, &order, sizeof order);
    raw[3] = 1U;
    if (second >= 0) {
        order.id = second;
        order.point.x = 20;
        order.point.y = 21;
        stars_ai_move_order_to_plord(raw, 4U, &order, 1);
    }
}

static void check(StarsI16 condition, StarsI16 *checks,
    StarsI16 *failures)
{
    ++*checks;
    if (!condition) {
#ifndef TARGET_NATIVE
        fprintf(stderr, "failed check %d\n", *checks);
#endif
        ++*failures;
    }
}

int main(void)
{
    StarsCyberAiTurnInput input;
    StarsCyberAiTurnOps ops;
    StarsCyberTurnFleet fleet;
    StarsU8 orders[4U + 4U * 18U];
    StarsU8 before[sizeof orders];
    StarsU8 table[16U * 12U];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsU16 i;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    ops.merge_shdefs = merge;
    ops.plan_fleet_action = plan;
    ops.colonize = apply;
    ops.move = apply;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    fleet.id = 5;
    fleet.owner = 4;
    fleet.cord = 2;
    fleet.ship_counts[0] = 1;
    fleet.raw_plord = orders;
    fleet.raw_plord_bytes = sizeof orders;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    input.raw_ai_planet_table = table;
    input.raw_ai_planet_record_count = 12;
    trace.action = STARS_CYBER_FLEET_ACTION_COLONIZE;
    trace.append = 1;

    seed(orders, 4, 7);
    memcpy(before, orders, sizeof orders);
    memset(table, 0x5a, sizeof table);
    check(stars_do_cyber_ai_turn(&input), &checks, &failures);
    check(trace.calls == 1 && orders[3] == 3U, &checks, &failures);
    check(memcmp(orders + 4U, before + 4U, 36U) == 0,
        &checks, &failures);
    check(orders[4U + 36U + 4U] == 9U &&
        orders[4U + 36U] == 90U, &checks, &failures);
    check(table[7U * 16U + 15U] == 4U, &checks, &failures);
    for (i = 0; i < sizeof table; ++i)
        if (i != 7U * 16U + 15U)
            check(table[i] == 0x5aU, &checks, &failures);

    seed(orders, 4, 7);
    memset(table, 0x5a, sizeof table);
    trace.action = STARS_CYBER_FLEET_ACTION_MOVE;
    check(stars_do_cyber_ai_turn(&input) &&
        table[7U * 16U + 15U] == 0x5aU, &checks, &failures);

    seed(orders, 4, 7);
    trace.action = STARS_CYBER_FLEET_ACTION_COLONIZE;
    trace.reject_apply = 1;
    check(!stars_do_cyber_ai_turn(&input) &&
        table[7U * 16U + 15U] == 0x5aU, &checks, &failures);
    trace.reject_apply = 0;

    seed(orders, 4, -1);
    trace.append = 0;
    check(stars_do_cyber_ai_turn(&input) && orders[3] == 2U &&
        table[9U * 16U + 15U] == 4U, &checks, &failures);

    seed(orders, 4, 15);
    trace.append = 1;
    memset(table, 0x5a, sizeof table);
    check(!stars_do_cyber_ai_turn(&input) &&
        table[7U * 16U + 15U] == 0x5aU, &checks, &failures);

#ifndef TARGET_NATIVE
    printf("CYBER_SECOND_ORDER_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
