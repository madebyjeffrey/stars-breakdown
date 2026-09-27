#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsI16 random_values[4];
    StarsI16 random_count;
    StarsI16 random_index;
    StarsI16 lookups;
    StarsI16 destinations;
    StarsI16 moves;
} State;

static StarsI16 random_value(void *context, StarsI16 limit)
{
    State *state = (State *)context;
    (void)limit;
    ++state->random_count;
    if (state->random_index >= 4)
        return 0;
    return state->random_values[state->random_index++];
}

static StarsI16 lookup_planet(void *context, StarsU16 id,
    StarsTargetCyberPlanet *planet)
{
    State *state = (State *)context;
    ++state->lookups;
    planet->id = id;
    planet->owner = 1;
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
    if (kind != 0)
        return 0;
    ++state->destinations;
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
    (void)kind;
    (void)target;
    (void)point;
    ++state->moves;
}

static void setup(StarsTargetCyberFleet *fleet,
    StarsTargetCyberArmadaOps *ops, State *state)
{
    memset(fleet, 0, sizeof(*fleet));
    memset(ops, 0, sizeof(*ops));
    memset(state, 0, sizeof(*state));
    fleet->id = 7U;
    fleet->planet_id = 2;
    fleet->cord = 1;
    fleet->ship_counts[6] = 25U;
    fleet->ship_counts[9] = 1U;
    ops->random_value = random_value;
    ops->lookup_planet = lookup_planet;
    ops->find_destination = find_destination;
    ops->move_fleet = move_fleet;
    ops->context = state;
}

int main(void)
{
    StarsTargetCyberFleet fleet;
    StarsTargetCyberArmadaOps ops;
    StarsI16 potency[4] = { 10, 20, 10, 20 };
    State state;
    int failures;

    setup(&fleet, &ops, &state);
    state.random_values[0] = 0;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures = state.random_count != 1 || state.destinations != 1 ||
        state.moves != 1;

    setup(&fleet, &ops, &state);
    state.random_values[0] = 5;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.random_count != 1 || state.destinations != 0 ||
        state.moves != 0;

    setup(&fleet, &ops, &state);
    fleet.ship_counts[6] = 35U;
    state.random_values[0] = 5;
    state.random_values[1] = 0;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.random_count != 2 || state.destinations != 1 ||
        state.moves != 1;

    printf("TARGET_CYBER_ARMADA_FRIENDLY_RANDOM_CHECKS=9 FAILURES=%d RESULT=%s\n",
        failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
