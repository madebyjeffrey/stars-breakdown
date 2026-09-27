#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsI16 random_values[4];
    StarsI16 random_count;
    StarsI16 random_index;
    StarsI16 destinations;
    StarsI16 destination_kind;
    StarsI16 moves;
    StarsI16 clears;
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
    (void)context;
    planet->id = id;
    planet->owner = 2;
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
    ++state->destinations;
    state->destination_kind = kind;
    if (kind != 2)
        return 0;
    destination->id = 6U;
    destination->owner = 1;
    destination->status_flags = 0;
    destination->turn_seen = 20U;
    destination->point.x = 60;
    destination->point.y = 60;
    return 1;
}

static void clear_task(void *context, StarsTargetCyberFleet *fleet,
    StarsI16 reason)
{
    State *state = (State *)context;
    (void)fleet;
    (void)reason;
    ++state->clears;
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
    StarsTargetCyberArmadaOps *ops, State *state, StarsI16 damage)
{
    memset(fleet, 0, sizeof(*fleet));
    memset(ops, 0, sizeof(*ops));
    memset(state, 0, sizeof(*state));
    fleet->id = 7U;
    fleet->planet_id = 2;
    fleet->cord = 1;
    fleet->ship_counts[6] = (StarsU8)damage;
    fleet->ship_counts[9] = 1U;
    ops->random_value = random_value;
    ops->lookup_planet = lookup_planet;
    ops->find_destination = find_destination;
    ops->clear_task = clear_task;
    ops->move_fleet = move_fleet;
    ops->context = state;
}

int main(void)
{
    StarsTargetCyberFleet fleet;
    StarsTargetCyberArmadaOps ops;
    StarsI16 potency[4] = { 10, 200, 10, 200 };
    State state;
    int failures = 0;

    setup(&fleet, &ops, &state, 5);
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.random_count != 0 || state.destinations != 1 ||
        state.destination_kind != 2 || state.moves != 1 || state.clears != 1;

    setup(&fleet, &ops, &state, 25);
    state.random_values[0] = 4;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.random_count != 1 || state.destinations != 2 ||
        state.moves != 0 || state.clears != 1;

    setup(&fleet, &ops, &state, 45);
    state.random_values[0] = 5;
    state.random_values[1] = 6;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.random_count != 2 || state.destinations != 2 ||
        state.moves != 0 || state.clears != 1;

    setup(&fleet, &ops, &state, 130);
    state.random_values[0] = 5;
    state.random_values[1] = 7;
    state.random_values[2] = 7;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures += state.random_count != 3 || state.destinations != 1 ||
        state.destination_kind != 2 || state.moves != 1 || state.clears != 1;

    printf("TARGET_CYBER_ARMADA_HOSTILE_RANDOM_CHECKS=20 FAILURES=%d RESULT=%s\n",
        failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
