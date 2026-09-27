#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsI16 mining_rate;
    StarsI32 mineral_stock;
    StarsI16 target_kind;
    StarsI16 moves;
    StarsI16 seen_kind;
    StarsI32 transfer_amount;
} State;

static StarsI16 lookup_planet(void *context, StarsI16 id,
    StarsCyberFreighterPlanet *planet)
{
    State *state = (State *)context;
    planet->id = (StarsU16)id;
    planet->owner = 1;
    planet->status_flags = 0;
    planet->point.x = 10;
    planet->point.y = 10;
    planet->mineral_total = 0;
    planet->mining_rate = state->mining_rate;
    planet->mineral_stock = state->mineral_stock;
    return 1;
}

static StarsI16 find_target(void *context,
    const StarsCyberFreighterPlanet *source, StarsI16 kind,
    StarsCyberFreighterPlanet *target)
{
    State *state = (State *)context;
    (void)source;
    state->seen_kind = kind;
    if (kind != state->target_kind)
        return 0;
    target->id = 8U;
    target->owner = 1;
    target->status_flags = 0;
    target->point.x = 20;
    target->point.y = 20;
    target->mineral_total = 0;
    target->mining_rate = 1;
    target->mineral_stock = 5000;
    return 1;
}

static void transfer_supply(void *context, StarsI16 player,
    StarsI16 planet, StarsI16 cargo_type, StarsI16 resource_type,
    StarsI32 amount)
{
    State *state = (State *)context;
    (void)player;
    (void)planet;
    (void)cargo_type;
    (void)resource_type;
    state->transfer_amount = amount;
}

static StarsI16 move_fleet(void *context, StarsCyberFreighterFleet *fleet,
    StarsU16 kind, StarsU16 target, StarsPoint point, StarsU16 flags)
{
    State *state = (State *)context;
    (void)fleet;
    (void)kind;
    (void)target;
    (void)point;
    (void)flags;
    ++state->moves;
    return 1;
}

static void setup(StarsCyberFreighterFleet *fleet,
    StarsCyberFreighterPlanState *plan, StarsDoCyberFreighterOps *ops,
    State *state)
{
    memset(fleet, 0, sizeof(*fleet));
    memset(plan, 0, sizeof(*plan));
    memset(ops, 0, sizeof(*ops));
    memset(state, 0, sizeof(*state));
    fleet->id = 3U;
    fleet->planet_id = 2;
    ops->lookup_planet = lookup_planet;
    ops->find_target = find_target;
    ops->transfer_supply = transfer_supply;
    ops->move_fleet = move_fleet;
    ops->context = state;
}

static int run_case(StarsI16 rate, StarsI32 stock, StarsI16 expected_kind,
    StarsI32 expected_amount)
{
    StarsCyberFreighterFleet fleet;
    StarsCyberFreighterPlanState plan;
    StarsDoCyberFreighterOps ops;
    State state;

    setup(&fleet, &plan, &ops, &state);
    state.mining_rate = rate;
    state.mineral_stock = stock;
    state.target_kind = expected_kind;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    return state.seen_kind == expected_kind && state.moves == 1 &&
        state.transfer_amount == expected_amount;
}

int main(void)
{
    int failures;

    failures = !run_case(-1, 5000L, 0, -1000L);
    failures += !run_case(0, 2000L, 0, -1000L);
    failures += !run_case(0, 2001L, 1, 1000L);
    failures += !run_case(1, 0L, 1, 1000L);
    printf("DO_CYBER_FREIGHTER_OWNED_GATE_CHECKS=12 FAILURES=%d RESULT=%s\n",
        failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
