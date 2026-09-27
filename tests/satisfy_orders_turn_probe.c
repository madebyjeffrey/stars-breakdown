#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 events[16];
    StarsI16 event_count;
    StarsI16 satisfy_calls;
    StarsI16 transfer_calls;
    StarsI16 done_calls;
    StarsI16 last_pass;
    StarsU16 battle_id;
    StarsI16 moved;
    StarsSatisfyFleet *fleet;
    const StarsSatisfyOrdersOps *ops;
} Trace;

static void event(Trace *trace, StarsI16 code)
{
    if (trace->event_count < 16)
        trace->events[trace->event_count] = code;
    ++trace->event_count;
}

static StarsI32 transfer(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    Trace *trace;
    (void)fleet;
    (void)order;
    (void)resource;
    trace = (Trace *)context;
    ++trace->transfer_calls;
    trace->moved += (StarsI16)amount;
    return amount;
}

static void order_done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    Trace *trace;
    (void)fleet;
    (void)order;
    (void)index;
    trace = (Trace *)context;
    ++trace->done_calls;
}

static void set_battle_id(void *context, StarsU16 battle_id)
{
    Trace *trace;
    trace = (Trace *)context;
    trace->battle_id = battle_id;
    event(trace, 1);
}

static void do_battles(void *context, StarsI16 post_movement)
{
    Trace *trace;
    trace = (Trace *)context;
    if (post_movement != 0)
        event(trace, 2);
}

static void do_things(void *context, StarsI16 post_movement)
{
    Trace *trace;
    (void)post_movement;
    trace = (Trace *)context;
    event(trace, 3);
}

static void satisfy(void *context, StarsI16 pass)
{
    Trace *trace;
    StarsSatisfyFleet *fleet;
    StarsSatisfyOrdersInput input;

    trace = (Trace *)context;
    fleet = trace->fleet;
    memset(&input, 0, sizeof input);
    input.fleets = fleet;
    input.fleet_count = 1;
    input.pass = pass;
    input.ops = trace->ops;
    ++trace->satisfy_calls;
    trace->last_pass = pass;
    event(trace, (StarsI16)(10 + pass));
    stars_satisfy_orders(&input);
}

static void drop_colonists(void *context, StarsI16 value)
{
    Trace *trace;
    (void)value;
    trace = (Trace *)context;
    event(trace, 4);
}

static void update_research(void *context, StarsI16 value)
{
    Trace *trace;
    (void)value;
    trace = (Trace *)context;
    event(trace, 5);
}

static void transfer_to_others(void *context, StarsI16 value)
{
    Trace *trace;
    (void)value;
    trace = (Trace *)context;
    event(trace, 6);
}

static void reset_trace(Trace *trace)
{
    memset(trace, 0, sizeof *trace);
}

static StarsI16 sequence_is(const Trace *trace, const StarsI16 *expected,
    StarsI16 count)
{
    StarsI16 index;

    if (trace->event_count != count)
        return 0;
    for (index = 0; index < count; ++index)
        if (trace->events[index] != expected[index])
            return 0;
    return 1;
}

int main(void)
{
    struct {
        Trace trace;
        StarsSatisfyFleet fleet;
        StarsSatisfyOrder order;
        StarsSatisfyOrdersOps ops;
    } state;
    StarsOrdersPlanet planets[2];
    StarsDoOrdersInput input;
    StarsI16 expected_before[6];
    StarsI16 expected_after[7];
    StarsI16 failures;

    memset(&state, 0, sizeof state);
    memset(planets, 0, sizeof planets);
    memset(&input, 0, sizeof input);
    state.fleet.owner = 2;
    state.fleet.cargo[STARS_SATISFY_RESOURCE_MINERAL_0] = 7;
    state.fleet.capacity[STARS_SATISFY_RESOURCE_MINERAL_0] = 20;
    state.fleet.orders = &state.order;
    state.fleet.order_count = 1;
    state.order.kind = STARS_SATISFY_ORDER_TRANSFER;
    state.order.phase_mask = (StarsI16)(1 << 2);
    state.order.target_kind = STARS_SATISFY_TARGET_FLEET;
    state.order.requested[STARS_SATISFY_RESOURCE_MINERAL_0] = 5;
    state.order.active = 1;
    state.ops.transfer = transfer;
    state.ops.order_done = order_done;
    state.ops.context = &state.trace;
    state.trace.fleet = &state.fleet;
    state.trace.ops = &state.ops;
    planets[0].id = 1;
    planets[0].player = 2;
    planets[0].flags = 0xffffU;
    planets[0].turn = 44;
    planets[1].id = 2;
    planets[1].player = -1;
    planets[1].flags = 0xffffU;
    planets[1].turn = 45;
    input.planets = planets;
    input.planet_count = 2;
    input.game_turn = 0x120bU;
    input.set_battle_id = set_battle_id;
    input.do_battles = do_battles;
    input.do_thing_interactions = do_things;
    input.satisfy_orders = satisfy;
    input.drop_colonists = drop_colonists;
    input.update_research = update_research;
    input.transfer_to_others = transfer_to_others;
    input.context = &state.trace;

    /* The callback context is intentionally a contiguous integration state;
     * satisfy() resolves the typed fleet and service table after Trace. */
    expected_before[0] = 3;
    expected_before[1] = 11;
    expected_before[2] = 4;
    expected_before[3] = 5;
    expected_before[4] = 12;
    expected_before[5] = 6;
    /* Rebind the callback context to the contiguous state for the turn. */
    input.context = &state.trace;
    stars_do_orders(0, &input);
    failures = !sequence_is(&state.trace, expected_before, 6) ||
        state.trace.satisfy_calls != 2 || state.trace.last_pass != 2 ||
        state.trace.transfer_calls != 1 || state.trace.done_calls != 1 ||
        state.trace.moved != 5 || state.fleet.cargo[0] != 2 ||
        state.order.requested[0] != 0 || state.order.active != 0 ||
        (planets[0].flags & 0x2000U) == 0U ||
        (planets[1].flags & 0x2000U) != 0U || planets[0].turn != 44 ||
        planets[1].turn != 45;

    reset_trace(&state.trace);
    state.trace.fleet = &state.fleet;
    state.trace.ops = &state.ops;
    state.order.active = 1;
    state.order.phase_mask = (StarsI16)(1 << 4);
    state.order.requested[0] = 2;
    state.fleet.cargo[0] = 5;
    planets[0].flags = 0xffffU;
    planets[1].flags = 0xffffU;
    planets[0].turn = 44;
    planets[1].turn = 45;
    expected_after[0] = 1;
    expected_after[1] = 2;
    expected_after[2] = 3;
    expected_after[3] = 13;
    expected_after[4] = 4;
    expected_after[5] = 5;
    expected_after[6] = 14;
    stars_do_orders(1, &input);
    if (!sequence_is(&state.trace, expected_after, 7) ||
            state.trace.satisfy_calls != 2 || state.trace.last_pass != 4 ||
            state.trace.battle_id != 0x0b01U || state.trace.transfer_calls != 1 ||
            state.trace.moved != 2 || state.fleet.cargo[0] != 3 ||
            state.order.requested[0] != 0 || state.order.active != 0 ||
            planets[0].turn != 0 || planets[1].turn != 0)
        failures = 1;

    printf("SATISFY_ORDERS_TURN_CHECKS=8 FAILURES=%d RESULT=%s\n",
        failures, failures == 0 ? "PASS" : "FAIL");
    return failures != 0;
}
