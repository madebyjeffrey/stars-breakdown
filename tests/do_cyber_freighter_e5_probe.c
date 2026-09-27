#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct State {
    StarsI16 owner;
    StarsI16 rate;
    StarsI32 stock;
    StarsI16 target_kind;
    StarsI16 target_calls;
    StarsI16 moves;
    StarsI16 transfers;
    StarsI32 last_amount;
    StarsI16 starbase_moves;
    StarsU16 last_flags;
    StarsU8 *plord;
} State;

static void check(StarsI16 condition, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!condition)
        ++*failures;
}

static StarsI16 lookup_planet(void *context, StarsI16 id,
    StarsCyberFreighterPlanet *planet)
{
    State *state;
    state = (State *)context;
    if (id < 0)
        return 0;
    planet->id = (StarsU16)id;
    planet->owner = state->owner;
    planet->status_flags = 0;
    planet->point.x = 10;
    planet->point.y = 10;
    planet->mineral_total = 5;
    planet->mining_rate = state->rate;
    planet->mineral_stock = state->stock;
    return 1;
}

static StarsI16 find_target(void *context,
    const StarsCyberFreighterPlanet *source, StarsI16 kind,
    StarsCyberFreighterPlanet *target)
{
    State *state;
    (void)source;
    state = (State *)context;
    ++state->target_calls;
    if (kind != state->target_kind)
        return 0;
    target->id = 8;
    target->owner = 1;
    target->status_flags = 0;
    target->point.x = 20;
    target->point.y = 20;
    target->mineral_total = 5;
    target->mining_rate = 1;
    target->mineral_stock = 5000;
    return 1;
}

static void transfer_supply(void *context, StarsI16 player,
    StarsI16 planet, StarsI16 cargo_type, StarsI16 resource_type,
    StarsI32 amount)
{
    State *state;
    (void)player;
    (void)planet;
    (void)cargo_type;
    (void)resource_type;
    state = (State *)context;
    ++state->transfers;
    state->last_amount = amount;
}

static void move_to_starbase(void *context, StarsCyberFreighterFleet *fleet)
{
    State *state;
    (void)fleet;
    state = (State *)context;
    ++state->starbase_moves;
}

static StarsU16 ideal_warp(void *context,
    const StarsCyberFreighterFleet *fleet, StarsI16 purpose)
{
    (void)context;
    (void)fleet;
    (void)purpose;
    return 11;
}

static StarsI16 move_fleet(void *context, StarsCyberFreighterFleet *fleet,
    StarsU16 kind, StarsU16 target, StarsPoint point, StarsU16 flags)
{
    State *state;
    StarsMoveOrder order;
    (void)fleet;
    (void)kind;
    (void)target;
    state = (State *)context;
    ++state->moves;
    state->last_flags = flags;
    if (state->plord) {
        memset(&order, 0, sizeof order);
        order.id = (StarsI16)target;
        order.point = point;
        order.flags = flags;
        return stars_ai_move_order_to_plord(state->plord, 2, &order, 0);
    }
    return 1;
}

static void setup(StarsCyberFreighterFleet *fleet,
    StarsCyberFreighterPlanState *plan, StarsDoCyberFreighterOps *ops,
    State *state)
{
    memset(fleet, 0, sizeof *fleet);
    memset(plan, 0, sizeof *plan);
    memset(ops, 0, sizeof *ops);
    memset(state, 0, sizeof *state);
    fleet->id = 3;
    fleet->planet_id = 2;
    fleet->available_cargo = 2;
    ops->lookup_planet = lookup_planet;
    ops->find_target = find_target;
    ops->transfer_supply = transfer_supply;
    ops->move_to_starbase = move_to_starbase;
    ops->ideal_warp = ideal_warp;
    ops->move_fleet = move_fleet;
    ops->context = state;
}

static void run_gate_case(StarsI16 rate, StarsI32 stock,
    StarsI16 target_kind, StarsI32 amount, StarsI16 *checks,
    StarsI16 *failures)
{
    StarsCyberFreighterFleet fleet;
    StarsCyberFreighterPlanState plan;
    StarsDoCyberFreighterOps ops;
    State state;

    setup(&fleet, &plan, &ops, &state);
    state.owner = 1;
    state.rate = rate;
    state.stock = stock;
    state.target_kind = target_kind;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    check(state.target_calls == 1, checks, failures);
    check(state.moves == 1, checks, failures);
    check(state.transfers == 1 && state.last_amount == amount,
        checks, failures);
}

int main(void)
{
    StarsCyberFreighterFleet fleet;
    StarsCyberFreighterPlanState plan;
    StarsDoCyberFreighterOps ops;
    State state;
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE];
    StarsMoveOrder first;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    run_gate_case(-1, 5000L, 0, -1000L, &checks, &failures);
    run_gate_case(0, 2000L, 0, -1000L, &checks, &failures);
    run_gate_case(0, 2001L, 1, 1000L, &checks, &failures);
    run_gate_case(1, 0L, 1, 1000L, &checks, &failures);

    setup(&fleet, &plan, &ops, &state);
    state.owner = 1;
    state.rate = 1;
    state.stock = 5000L;
    state.target_kind = 1;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    check((plan.flags & 0x18U) == 0x08U, &checks, &failures);
    check(state.moves == 1 && state.last_flags == 0x11b0U,
        &checks, &failures);

    setup(&fleet, &plan, &ops, &state);
    state.owner = 1;
    state.rate = 1;
    state.stock = 5000L;
    state.target_kind = -1;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    check(state.target_calls == 2 && state.moves == 0,
        &checks, &failures);
    check(state.transfers == 2 && state.last_amount == -1000L,
        &checks, &failures);
    check((plan.flags & 0x06U) == 0x02U, &checks, &failures);

    setup(&fleet, &plan, &ops, &state);
    state.owner = 2;
    state.target_kind = -1;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    check(state.starbase_moves == 1, &checks, &failures);
    check(state.moves == 0 && state.transfers == 0, &checks, &failures);

    setup(&fleet, &plan, &ops, &state);
    state.owner = -1;
    state.target_kind = 1;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    check(state.moves == 1 && state.target_calls == 1,
        &checks, &failures);
    check((plan.flags & 0x18U) == 0x08U, &checks, &failures);

    setup(&fleet, &plan, &ops, &state);
    fleet.planet_id = -1;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    check(state.moves == 0 && state.target_calls == 0,
        &checks, &failures);

    setup(&fleet, &plan, &ops, &state);
    memset(plord, 0, sizeof plord);
    memset(&first, 0, sizeof first);
    first.point.x = 20;
    first.point.y = 20;
    memcpy(plord + STARS_AI_PLORD_HEADER_SIZE, &first, sizeof first);
    plord[3] = 1;
    state.owner = 1;
    state.rate = 1;
    state.stock = 5000L;
    state.target_kind = 1;
    state.plord = plord;
    stars_do_cyber_freighter(&fleet, &plan, 1, &ops);
    memset(&first, 0, sizeof first);
    first.id = 8;
    first.point.x = 20;
    first.point.y = 20;
    first.flags = 0x1100U | 0x00b0U;
    check(state.moves == 1 && state.last_flags == 0x11b0U,
        &checks, &failures);
    check((fleet.moving_flags & 0x8000U) != 0U, &checks, &failures);
    check(plord[3] == 1U, &checks, &failures);
    check(memcmp(plord + STARS_AI_PLORD_HEADER_SIZE, &first,
        sizeof first) == 0, &checks, &failures);

#ifndef TARGET_NATIVE
    printf("DO_CYBER_FREIGHTER_E5_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
