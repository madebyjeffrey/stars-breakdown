#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsI16 owner;
    StarsI32 mineral_total;
    StarsI16 mining_rate;
    StarsI32 mineral_stock;
    StarsI16 target_kind;
    StarsI16 target_calls;
    StarsI16 moves;
    StarsI16 transfers;
    StarsI32 last_amount;
} State;

static StarsI16 lookup_planet(void *context, StarsI16 id,
    StarsCyberFreighterPlanet *planet)
{
    State *state = (State *)context;
    planet->id = (StarsU16)id;
    planet->owner = state->owner;
    planet->status_flags = 0;
    planet->point.x = 10;
    planet->point.y = 10;
    planet->mineral_total = state->mineral_total;
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
    ++state->target_calls;
    if (kind != state->target_kind)
        return 0;
    target->id = 8U;
    target->owner = 1;
    target->status_flags = 0;
    target->point.x = 20;
    target->point.y = 20;
    target->mineral_total = 5;
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
    ++state->transfers;
    state->last_amount = amount;
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

int main(void)
{
    StarsCyberFreighterFleet fleet;
    StarsCyberFreighterPlanState plan;
    StarsDoCyberFreighterOps ops;
    State state;
    int failures = 0;

    setup(&fleet, &plan, &ops, &state);
    state.owner = -1;
    state.target_kind = 0;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    failures += state.moves != 1 || state.target_calls != 1 ||
        (plan.flags & 0x18U) != 0U;

    setup(&fleet, &plan, &ops, &state);
    state.owner = 1;
    state.mineral_total = 5;
    state.mining_rate = 1;
    state.mineral_stock = 5000;
    state.target_kind = 1;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    failures += state.moves != 1 || state.target_calls != 1 ||
        (plan.flags & 0x18U) != 0x08U;

    setup(&fleet, &plan, &ops, &state);
    state.owner = 1;
    state.mineral_total = 5;
    state.mining_rate = 1;
    state.mineral_stock = 5000;
    state.target_kind = -1;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    failures += state.moves != 0 || state.target_calls != 2 ||
        (plan.flags & 0x06U) != 0x02U || state.transfers != 2 ||
        state.last_amount != -1000L;

    printf("DO_CYBER_FREIGHTER_PROGRESS_CHECKS=9 FAILURES=%d RESULT=%s\n",
        failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
