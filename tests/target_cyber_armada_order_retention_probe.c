#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsI16 target_owner;
    StarsI16 target_starbase;
    StarsU16 target_turn;
    StarsI16 lookups;
    StarsI16 moves;
    StarsU16 kind;
    StarsU16 target;
} State;

static StarsI16 lookup_planet(void *context, StarsU16 id,
    StarsTargetCyberPlanet *planet)
{
    State *state = (State *)context;
    ++state->lookups;
    planet->id = id;
    if (id == 5U) {
        planet->owner = state->target_owner;
        planet->status_flags = state->target_starbase ?
            (StarsU16)(1U << 9) : 0U;
        planet->turn_seen = state->target_turn;
    } else {
        planet->owner = 1;
        planet->status_flags = 0;
        planet->turn_seen = 20U;
    }
    planet->point.x = 10;
    planet->point.y = 10;
    return 1;
}

static StarsI16 find_destination(void *context,
    const StarsTargetCyberPlanet *source, StarsI16 kind,
    StarsTargetCyberPlanet *destination)
{
    (void)context;
    (void)source;
    if (kind != 0)
        return 0;
    destination->id = 4U;
    destination->owner = 1;
    destination->status_flags = 0;
    destination->turn_seen = 20U;
    destination->point.x = 40;
    destination->point.y = 40;
    return 1;
}

static void move_fleet(void *context, StarsTargetCyberFleet *fleet,
    StarsU16 kind, StarsU16 target, StarsPoint point)
{
    State *state = (State *)context;
    (void)fleet;
    (void)point;
    ++state->moves;
    state->kind = kind;
    state->target = target;
}

static StarsI16 random_value(void *context, StarsI16 limit)
{
    (void)context;
    (void)limit;
    return 0;
}

static void setup_ops(StarsTargetCyberArmadaOps *ops, State *state)
{
    memset(ops, 0, sizeof(*ops));
    ops->random_value = random_value;
    ops->lookup_planet = lookup_planet;
    ops->find_destination = find_destination;
    ops->move_fleet = move_fleet;
    ops->context = state;
}

static void setup_fleet(StarsTargetCyberFleet *fleet)
{
    memset(fleet, 0, sizeof(*fleet));
    fleet->id = 7U;
    fleet->planet_id = 2;
    fleet->cord = 2;
    fleet->point.x = 0;
    fleet->point.y = 0;
    fleet->order_target = 5U;
    fleet->ship_counts[6] = 100U;
    fleet->ship_counts[9] = 1U;
}

int main(void)
{
    StarsTargetCyberFleet fleet;
    StarsTargetCyberArmadaOps ops;
    StarsI16 potency[4] = { 1, 1, 1, 1 };
    State state;
    int failures = 0;

    setup_fleet(&fleet);
    memset(&state, 0, sizeof(state));
    state.target_owner = 1;
    state.target_turn = 20U;
    setup_ops(&ops, &state);
    fleet.order_kind = 1U;
    fleet.order_point.x = 0;
    fleet.order_point.y = 0;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.moves != 1 || state.kind != 1U || state.target != 4U;
    failures += state.lookups != 2;

    setup_fleet(&fleet);
    memset(&state, 0, sizeof(state));
    state.target_owner = 2;
    state.target_turn = 20U;
    setup_ops(&ops, &state);
    fleet.order_kind = 1U;
    fleet.order_point.x = 0;
    fleet.order_point.y = 0;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.moves != 0 || state.lookups != 1;

    setup_fleet(&fleet);
    memset(&state, 0, sizeof(state));
    state.target_owner = 1;
    state.target_turn = 20U;
    setup_ops(&ops, &state);
    fleet.order_kind = 2U;
    fleet.order_point.x = 0;
    fleet.order_point.y = 0;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.moves != 0 || state.lookups != 0;

    setup_fleet(&fleet);
    memset(&state, 0, sizeof(state));
    state.target_owner = 1;
    state.target_turn = 20U;
    setup_ops(&ops, &state);
    fleet.order_kind = 2U;
    fleet.order_point.x = 251;
    fleet.order_point.y = 0;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.moves != 1 || state.kind != 1U || state.target != 4U;

    printf("TARGET_CYBER_ARMADA_ORDER_RETENTION_CHECKS=12 FAILURES=%d RESULT=%s\n",
        failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
