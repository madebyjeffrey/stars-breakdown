#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 repair_calls;
    StarsI16 plan_calls;
    StarsI16 apply_calls;
    StarsI16 visit_calls;
    StarsI16 valid;
    StarsI16 reject_plan;
    StarsI16 reject_apply;
    StarsI16 events[16];
    StarsI16 event_count;
} Trace;

static void event(Trace *trace, StarsI16 value)
{
    if (trace->event_count < 16)
        trace->events[trace->event_count] = value;
    ++trace->event_count;
}

static StarsI16 merge(void *opaque, StarsI16 mask)
{
    (void)opaque;
    (void)mask;
    return 1;
}

static StarsI16 repair(void *opaque, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->repair_calls;
    event(trace, 1);
    if (fleet->id == 2 && policy->needs_order_repair && potency == 4U &&
            fleet->raw_plord && fleet->raw_plord[3] == 1U &&
            (fleet->raw_plord[10] & 0x0fU) == 6U &&
            fleet->raw_plord[12] == 5U && fleet->raw_plord[14] == 5U)
        ++trace->valid;
    return 1;
}

static StarsI16 plan(void *opaque, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency,
    StarsCyberFleetPlan *out)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->plan_calls;
    event(trace, fleet->id == 2 ? 2 : 5);
    if (potency != 4U ||
            (!policy->needs_order_repair && fleet->id == 2))
        return 0;
    if (fleet->id == 2) {
        out->action = STARS_CYBER_FLEET_ACTION_COLONIZE;
        out->target_planet = 7;
        out->order_kind = 2;
        out->append = 0;
        out->reason = 41;
    } else if (fleet->id == 3) {
        out->action = STARS_CYBER_FLEET_ACTION_MOVE;
        out->target_planet = 9;
        out->order_kind = 1;
        out->append = 1;
        out->reason = 42;
    } else {
        return 0;
    }
    return trace->reject_plan ? 0 : 1;
}

static StarsI16 apply(void *opaque, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPlan *plan_in)
{
    Trace *trace;
    StarsMoveOrder order;
    StarsI16 expected_x;

    trace = (Trace *)opaque;
    ++trace->apply_calls;
    event(trace, 3);
    memset(&order, 0, sizeof order);
    order.id = plan_in->target_planet;
    expected_x = fleet->id == 2 ? 10 : 90;
    order.point.x = expected_x;
    order.point.y = (StarsI16)(expected_x + 1);
    order.flags = fleet->id == 2 ? 0x1140U : 0x1100U;
    if (!fleet->raw_plord ||
            !stars_ai_move_order_to_plord(fleet->raw_plord, 3U, &order,
            plan_in->append))
        return 0;
    if (fleet->id == 2 && plan_in->action ==
            STARS_CYBER_FLEET_ACTION_COLONIZE &&
            plan_in->target_planet == 7 && plan_in->order_kind == 2 &&
            plan_in->append == 0 && plan_in->reason == 41)
        ++trace->valid;
    if (fleet->id == 3 && plan_in->action ==
            STARS_CYBER_FLEET_ACTION_MOVE &&
            plan_in->target_planet == 9 && plan_in->order_kind == 1 &&
            plan_in->append == 1 && plan_in->reason == 42)
        ++trace->valid;
    return trace->reject_apply ? 0 : 1;
}

static StarsI16 visit(void *opaque, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->visit_calls;
    event(trace, fleet->id == 2 ? 4 : 6);
    if ((fleet->id == 2 || fleet->id == 3) && potency == 4U &&
            policy->total_ships != 0)
        ++trace->valid;
    return 1;
}

static void seed_order(StarsU8 *plord, StarsI16 id, StarsI16 x, StarsI16 y)
{
    StarsMoveOrder order;
    memset(&order, 0, sizeof order);
    order.id = id;
    order.point.x = x;
    order.point.y = y;
    memcpy(plord + STARS_AI_PLORD_HEADER_SIZE, &order, sizeof order);
    plord[3] = 1U;
}

static StarsI16 sequence_ok(const Trace *trace)
{
    return trace->event_count == 7 && trace->events[0] == 1 &&
        trace->events[1] == 2 && trace->events[2] == 3 &&
        trace->events[3] == 4 && trace->events[4] == 5 &&
        trace->events[5] == 3 && trace->events[6] == 6;
}

int main(void)
{
    StarsCyberTurnFleet fleets[3];
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    StarsU8 first_plord[STARS_AI_PLORD_HEADER_SIZE +
        3U * STARS_AI_PLORD_ORDER_SIZE];
    StarsU8 second_plord[STARS_AI_PLORD_HEADER_SIZE +
        3U * STARS_AI_PLORD_ORDER_SIZE];
    StarsMoveOrder expected;
    Trace trace;
    StarsI16 failures;

    memset(fleets, 0, sizeof fleets);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    memset(first_plord, 0xa5, sizeof first_plord);
    memset(second_plord, 0x5a, sizeof second_plord);
    seed_order(first_plord, 4, 10, 11);
    seed_order(second_plord, 8, 30, 31);
    fleets[0].id = 1;
    fleets[0].owner = 9;
    fleets[0].ship_counts[2] = 1;
    fleets[1].id = 2;
    fleets[1].owner = 4;
    fleets[1].cord = 1;
    fleets[1].raw_plord = first_plord;
    fleets[1].ship_counts[0] = 1;
    fleets[2].id = 3;
    fleets[2].owner = 4;
    fleets[2].cord = 0;
    fleets[2].order_kind = 5;
    fleets[2].raw_plord = second_plord;
    fleets[2].ship_counts[2] = 1;
    ops.merge_shdefs = merge;
    ops.repair_order = repair;
    ops.plan_fleet_action = plan;
    ops.colonize = apply;
    ops.move = apply;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 3;
    input.fleets = fleets;
    input.ops = &ops;

    failures = !stars_do_cyber_ai_turn(&input);
    failures += trace.repair_calls != 1;
    failures += trace.plan_calls != 2;
    failures += trace.apply_calls != 2;
    failures += trace.visit_calls != 2;
    failures += trace.valid != 5;
    failures += !sequence_ok(&trace);
    memset(&expected, 0, sizeof expected);
    expected.id = 7;
    expected.point.x = 10;
    expected.point.y = 11;
    expected.flags = 0x1140U;
    failures += first_plord[3] != 1U ||
        memcmp(first_plord + 4, &expected, sizeof expected) != 0;
    memset(&expected, 0, sizeof expected);
    expected.id = 9;
    expected.point.x = 90;
    expected.point.y = 91;
    expected.flags = 0x1100U;
    failures += second_plord[3] != 2U ||
        memcmp(second_plord + 4 + STARS_AI_PLORD_ORDER_SIZE,
        &expected, sizeof expected) != 0;
    memset(&trace, 0, sizeof trace);
    trace.reject_plan = 1;
    failures += stars_do_cyber_ai_turn(&input) != 0;
    failures += trace.plan_calls != 1 || trace.apply_calls != 0 ||
        trace.visit_calls != 0;
    memset(&trace, 0, sizeof trace);
    trace.reject_apply = 1;
    failures += stars_do_cyber_ai_turn(&input) != 0;
    failures += trace.plan_calls != 1 || trace.apply_calls != 1 ||
        trace.visit_calls != 0;

#ifndef TARGET_NATIVE
    printf("CYBER_AI_TURN_ORDER_WRITE_CHECKS=24 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
