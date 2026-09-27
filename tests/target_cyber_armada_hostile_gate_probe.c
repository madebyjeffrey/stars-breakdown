#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsI16 lookups;
    StarsI16 destinations;
    StarsI16 moves;
    StarsI16 clears;
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
    State *state = (State *)context;
    ++state->lookups;
    planet->id = id;
    planet->owner = id == 2U ? 2 : 1;
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
    (void)kind;
    (void)destination;
    ++state->destinations;
    return 0;
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

static void clear_task(void *context, StarsTargetCyberFleet *fleet,
    StarsI16 reason)
{
    State *state = (State *)context;
    (void)fleet;
    (void)reason;
    ++state->clears;
}

int main(void)
{
    StarsTargetCyberFleet fleet;
    StarsTargetCyberArmadaOps ops;
    StarsI16 potency[4] = { 1, 1, 1, 1 };
    State state;
    int failures;

    memset(&fleet, 0, sizeof(fleet));
    memset(&ops, 0, sizeof(ops));
    memset(&state, 0, sizeof(state));
    fleet.id = 7U;
    fleet.planet_id = 2;
    fleet.cord = 1;
    fleet.ship_counts[6] = 100U;
    fleet.ship_counts[9] = 1U;
    ops.random_value = random_value;
    ops.lookup_planet = lookup_planet;
    ops.find_destination = find_destination;
    ops.move_fleet = move_fleet;
    ops.clear_task = clear_task;
    ops.context = &state;
    stars_target_cyber_armada(&fleet, 1, 20, 2, potency, &ops);
    failures = state.lookups != 1 || state.destinations != 0 ||
        state.moves != 0 || state.clears != 0;
    printf("TARGET_CYBER_ARMADA_HOSTILE_GATE_CHECKS=4 FAILURES=%d RESULT=%s\n",
        failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
