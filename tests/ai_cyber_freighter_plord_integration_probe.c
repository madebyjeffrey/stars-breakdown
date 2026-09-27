#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsU8 *plord;
    StarsI16 moves;
    StarsU16 kind;
    StarsU16 target;
    StarsPoint point;
    StarsU16 flags;
} State;

static StarsI16 lookup_planet(void *context, StarsI16 id,
    StarsCyberFreighterPlanet *planet)
{
    (void)context;
    if (id < 0)
        return 0;
    planet->id = (StarsU16)id;
    planet->owner = 1;
    planet->status_flags = 0;
    planet->point.x = 10;
    planet->point.y = 10;
    planet->mineral_total = 5;
    planet->mining_rate = 1;
    planet->mineral_stock = 5000;
    return 1;
}

static StarsI16 find_target(void *context,
    const StarsCyberFreighterPlanet *source, StarsI16 kind,
    StarsCyberFreighterPlanet *target)
{
    State *state = (State *)context;
    (void)source;
    ++state->kind;
    if (kind != 1)
        return 0;
    target->id = 8;
    target->owner = 1;
    target->point.x = 20;
    target->point.y = 20;
    return 1;
}

static void transfer_supply(void *context, StarsI16 player,
    StarsI16 planet, StarsI16 cargo_type, StarsI16 resource_type,
    StarsI32 amount)
{
    (void)context;
    (void)player;
    (void)planet;
    (void)cargo_type;
    (void)resource_type;
    (void)amount;
}

static StarsU16 ideal_warp(void *context,
    const StarsCyberFreighterFleet *fleet, StarsI16 purpose)
{
    (void)context;
    (void)fleet;
    (void)purpose;
    return 11U;
}

static StarsI16 move_fleet(void *context, StarsCyberFreighterFleet *fleet,
    StarsU16 kind, StarsU16 target, StarsPoint point, StarsU16 flags)
{
    State *state;
    StarsMoveOrder order;

    state = (State *)context;
    ++state->moves;
    state->kind = kind;
    state->target = target;
    state->point = point;
    state->flags = flags;
    memset(&order, 0, sizeof(order));
    order.id = (StarsI16)target;
    order.point = point;
    order.flags = flags;
    (void)fleet;
    return stars_ai_move_order_to_plord(state->plord, 2, &order, 0);
}

static void seed_first_order(StarsU8 *plord, StarsPoint point)
{
    StarsMoveOrder order;

    memset(&order, 0, sizeof(order));
    order.point = point;
    memcpy(plord + STARS_AI_PLORD_HEADER_SIZE, &order, sizeof(order));
    plord[3] = 1U;
}

int main(void)
{
    StarsCyberFreighterFleet fleet;
    StarsCyberFreighterPlanState plan;
    StarsDoCyberFreighterOps ops;
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE];
    State state;
    StarsMoveOrder expected;
    StarsI16 failures;

    memset(&fleet, 0, sizeof(fleet));
    memset(&plan, 0, sizeof(plan));
    memset(&ops, 0, sizeof(ops));
    memset(plord, 0, sizeof(plord));
    memset(&state, 0, sizeof(state));
    fleet.id = 3;
    fleet.planet_id = 2;
    fleet.available_cargo = 2;
    state.plord = plord;
    ops.lookup_planet = lookup_planet;
    ops.find_target = find_target;
    ops.transfer_supply = transfer_supply;
    ops.ideal_warp = ideal_warp;
    ops.move_fleet = move_fleet;
    ops.context = &state;

    {
        StarsPoint point;
        point.x = 20;
        point.y = 20;
        seed_first_order(plord, point);
    }
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    failures = state.moves != 1 || state.kind != 1U ||
        state.target != 8U || state.point.x != 20 ||
        state.flags != 0x11b0U || (fleet.moving_flags & 0x8000U) == 0;
    memset(&expected, 0, sizeof(expected));
    expected.id = 8;
    expected.point.x = 20;
    expected.point.y = 20;
    expected.flags = 0x11b0U;
    failures += plord[3] != 1U || memcmp(plord + 4, &expected,
        sizeof(expected)) != 0;

    printf("AI_CYBER_FREIGHTER_PLORD_INTEGRATION_CHECKS=8 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
