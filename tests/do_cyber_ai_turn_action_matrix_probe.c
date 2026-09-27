#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 plan_calls;
    StarsI16 apply_calls;
    StarsI16 visit_calls;
    StarsI16 valid;
    StarsI16 events[32];
    StarsI16 event_count;
} Trace;

static void event(Trace *trace, StarsI16 value)
{
    if (trace->event_count < 32)
        trace->events[trace->event_count] = value;
    ++trace->event_count;
}

static StarsI16 merge(void *opaque, StarsI16 mask)
{
    (void)opaque;
    (void)mask;
    return 1;
}

static StarsI16 plan(void *opaque, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency,
    StarsCyberFleetPlan *out)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->plan_calls;
    event(trace, 1);
    if (fleet->id < 1 || fleet->id > 9 || policy->total_ships == 0 ||
            potency != 4U)
        return 0;
    out->action = fleet->id;
    out->target_planet = (StarsI16)(100 + fleet->id);
    out->order_kind = fleet->id;
    out->append = 0;
    out->reason = (StarsI16)(200 + fleet->id);
    return 1;
}

static StarsI16 apply_expected(void *opaque,
    const StarsCyberTurnFleet *fleet, const StarsCyberFleetPlan *plan_in,
    StarsI16 expected_action)
{
    Trace *trace;
    StarsMoveOrder order;

    trace = (Trace *)opaque;
    ++trace->apply_calls;
    event(trace, 2);
    if (plan_in->action != expected_action ||
            plan_in->target_planet != 100 + expected_action ||
            plan_in->order_kind != expected_action ||
            plan_in->reason != 200 + expected_action)
        return 0;
    if (expected_action != STARS_CYBER_FLEET_ACTION_CLEAR_TASK) {
        memset(&order, 0, sizeof order);
        order.id = plan_in->target_planet;
        order.point.x = (StarsI16)(10 + expected_action);
        order.point.y = (StarsI16)(20 + expected_action);
        order.flags = (StarsU16)(0x1000U + expected_action);
        if (!fleet->raw_plord ||
                !stars_ai_move_order_to_plord(fleet->raw_plord, 2U,
                &order, 0))
            return 0;
        if (fleet->raw_plord[3] != 1U ||
                fleet->raw_plord[4] != (StarsU8)order.point.x ||
                fleet->raw_plord[STARS_AI_PLORD_HEADER_SIZE + 2U] !=
                (StarsU8)order.point.y)
            return 0;
    }
    ++trace->valid;
    return 1;
}

#define ACTION_WRAPPER(name, value) \
static StarsI16 name(void *opaque, const StarsCyberTurnFleet *fleet, \
    const StarsCyberFleetPlan *plan_in) \
{ return apply_expected(opaque, fleet, plan_in, value); }

ACTION_WRAPPER(clear_task, STARS_CYBER_FLEET_ACTION_CLEAR_TASK)
ACTION_WRAPPER(move, STARS_CYBER_FLEET_ACTION_MOVE)
ACTION_WRAPPER(colonize, STARS_CYBER_FLEET_ACTION_COLONIZE)
ACTION_WRAPPER(wormhole, STARS_CYBER_FLEET_ACTION_WORMHOLE)
ACTION_WRAPPER(move_starbase, STARS_CYBER_FLEET_ACTION_MOVE_STARBASE)
ACTION_WRAPPER(attack, STARS_CYBER_FLEET_ACTION_ATTACK)
ACTION_WRAPPER(target_armada, STARS_CYBER_FLEET_ACTION_TARGET_ARMADA)
ACTION_WRAPPER(transport, STARS_CYBER_FLEET_ACTION_TRANSPORT)
ACTION_WRAPPER(target_freighter, STARS_CYBER_FLEET_ACTION_TARGET_FREIGHTER)

static StarsI16 visit(void *opaque, const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPolicy *policy, StarsU8 potency)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->visit_calls;
    event(trace, 3);
    if (fleet->id >= 1 && fleet->id <= 9 && policy->total_ships != 0 &&
            potency == 4U)
        ++trace->valid;
    return 1;
}

int main(void)
{
    StarsCyberTurnFleet fleets[9];
    StarsU8 plord[9][STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE];
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    Trace trace;
    StarsI16 i;
    StarsI16 failures;

    memset(fleets, 0, sizeof fleets);
    memset(plord, 0, sizeof plord);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    for (i = 0; i < 9; ++i) {
        StarsMoveOrder seed;

        fleets[i].id = (StarsI16)(i + 1);
        fleets[i].owner = 4;
        fleets[i].cord = 2;
        fleets[i].raw_plord = plord[i];
        fleets[i].ship_counts[2] = 1;
        memset(&seed, 0, sizeof seed);
        seed.point.x = (StarsI16)(10 + i + 1);
        seed.point.y = (StarsI16)(20 + i + 1);
        memcpy(plord[i] + STARS_AI_PLORD_HEADER_SIZE, &seed,
            sizeof seed);
        plord[i][3] = 1U;
    }
    ops.merge_shdefs = merge;
    ops.plan_fleet_action = plan;
    ops.clear_task = clear_task;
    ops.move = move;
    ops.colonize = colonize;
    ops.goto_wormhole = wormhole;
    ops.move_starbase = move_starbase;
    ops.attack = attack;
    ops.target_armada = target_armada;
    ops.transport = transport;
    ops.target_freighter = target_freighter;
    ops.visit_fleet_typed = visit;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.fleet_count = 9;
    input.fleets = fleets;
    input.ops = &ops;

    failures = !stars_do_cyber_ai_turn(&input);
    failures += trace.plan_calls != 9;
    failures += trace.apply_calls != 9;
    failures += trace.visit_calls != 9;
    failures += trace.valid != 18;
    failures += trace.event_count != 27;
    for (i = 0; i < 9; ++i)
        failures += plord[i][3] != 1U;
#ifndef TARGET_NATIVE
    printf("CYBER_AI_TURN_ACTION_MATRIX_CHECKS=34 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
