#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsU8 *plord;
    StarsI16 allow_destination;
    StarsI16 moves;
    StarsI16 marks;
    StarsU16 kind;
    StarsU16 target;
    StarsPoint point;
    StarsU16 flags;
} State;

static StarsI16 random_value(void *context, StarsI16 limit)
{
    (void)context;
    (void)limit;
    return 0;
}

static StarsI16 lookup_planet(void *context, StarsU16 id,
    StarsTargetCyberPlanet *planet)
{
    (void)context;
    planet->id = id;
    planet->owner = id == 4U ? 1 : -1;
    planet->status_flags = 0;
    planet->turn_seen = 20U;
    planet->point.x = 10;
    planet->point.y = 10;
    return 1;
}

static StarsI16 find_destination(void *context,
    const StarsTargetCyberPlanet *source, StarsI16 kind,
    StarsTargetCyberPlanet *destination)
{
    State *state = (State *)context;
    (void)source;
    if (!state->allow_destination || kind != 0)
        return 0;
    destination->id = 4U;
    destination->owner = 1;
    destination->status_flags = 0;
    destination->turn_seen = 20U;
    destination->point.x = 40;
    destination->point.y = 40;
    return 1;
}

static StarsI16 find_enemy_fleet(void *context,
    const StarsTargetCyberFleet *fleet, StarsTargetCyberFleet *enemy)
{
    (void)context;
    (void)fleet;
    memset(enemy, 0, sizeof(*enemy));
    enemy->id = 9U;
    enemy->point.x = 33;
    enemy->point.y = 44;
    return 1;
}

static void mark_planet(void *context, StarsU16 id)
{
    State *state = (State *)context;
    (void)id;
    ++state->marks;
}

static void move_fleet(void *context, StarsTargetCyberFleet *fleet,
    StarsU16 kind, StarsU16 target, StarsPoint point)
{
    State *state;
    StarsMoveOrder order;

    (void)fleet;
    state = (State *)context;
    ++state->moves;
    state->kind = kind;
    state->target = target;
    state->point = point;
    state->flags = kind == 1U ? 0x1140U : 0x1240U;
    memset(&order, 0, sizeof(order));
    order.id = (StarsI16)target;
    order.point = point;
    order.flags = state->flags;
    stars_ai_move_order_to_plord(state->plord, 2, &order, 0);
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
    StarsTargetCyberFleet fleet;
    StarsTargetCyberArmadaOps ops;
    StarsI16 potency[4] = { 1, 1, 1, 1 };
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE];
    StarsPoint seed_point;
    StarsMoveOrder expected;
    State state;
    StarsI16 failures;

    memset(&fleet, 0, sizeof(fleet));
    memset(&ops, 0, sizeof(ops));
    memset(plord, 0, sizeof(plord));
    memset(&state, 0, sizeof(state));
    fleet.id = 7U;
    fleet.planet_id = 2;
    fleet.cord = 1;
    fleet.point.x = 0;
    fleet.point.y = 0;
    fleet.ship_counts[6] = 100U;
    fleet.ship_counts[9] = 1U;
    state.allow_destination = 1;
    state.plord = plord;
    ops.random_value = random_value;
    ops.lookup_planet = lookup_planet;
    ops.find_destination = find_destination;
    ops.find_enemy_fleet = find_enemy_fleet;
    ops.move_fleet = move_fleet;
    ops.mark_planet = mark_planet;
    ops.context = &state;
    seed_point.x = 40;
    seed_point.y = 40;
    seed_first_order(plord, seed_point);
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures = state.moves != 1 || state.kind != 1U || state.target != 4U ||
        state.point.x != 40 || state.point.y != 40 || state.flags != 0x1140U ||
        state.marks != 1 || plord[3] != 1U;
    memset(&expected, 0, sizeof(expected));
    expected.id = 4;
    expected.point.x = 40;
    expected.point.y = 40;
    expected.flags = 0x1140U;
    failures += memcmp(plord + 4, &expected, sizeof(expected)) != 0;

    memset(&state, 0, sizeof(state));
    state.allow_destination = 0;
    state.plord = plord;
    memset(plord, 0, sizeof(plord));
    seed_point.x = 33;
    seed_point.y = 44;
    seed_first_order(plord, seed_point);
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.moves != 1 || state.kind != 2U || state.target != 9U ||
        state.point.x != 33 || state.point.y != 44 ||
        state.flags != 0x1240U || plord[3] != 1U;

    printf("AI_CYBER_ARMADA_PLORD_INTEGRATION_CHECKS=12 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
