#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct FullTurnTrace {
    StarsI16 events[64];
    StarsI16 event_count;
    StarsI16 message_codes[16];
    StarsI16 message_recipients[16];
    StarsI16 message_count;
    StarsI16 player_codes[8];
    StarsI16 player_recipients[8];
    StarsI16 player_message_count;
    StarsI16 satisfy_passes[4];
    StarsI16 satisfy_count;
    StarsI16 transfer_calls;
    StarsI16 battle_plan_calls;
    StarsI16 mine_calls;
    StarsI16 lay_mines_calls;
    StarsI16 route_calls;
    StarsI16 transfer_player_calls;
    StarsI16 done_calls;
    StarsI16 drop_calls;
    StarsI16 research_calls;
    StarsI16 transfer_to_others_calls;
    StarsU16 battle_id;
    StarsSatisfyFleet fleets[1];
    StarsSatisfyOrder orders[6];
    StarsU16 fleet_count;
    StarsSatisfyOrdersOps ops;
} FullTurnTrace;

static void record_event(FullTurnTrace *trace, StarsI16 code)
{
    if (trace->event_count < 64)
        trace->events[trace->event_count] = code;
    ++trace->event_count;
}

static void check(StarsI16 condition, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!condition)
        ++*failures;
}

static StarsI32 transfer(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    FullTurnTrace *trace;
    (void)fleet;
    (void)order;
    (void)resource;
    trace = (FullTurnTrace *)context;
    ++trace->transfer_calls;
    record_event(trace, 30);
    return amount;
}

static StarsI16 battle_plan(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    FullTurnTrace *trace;
    (void)fleet;
    (void)order;
    trace = (FullTurnTrace *)context;
    ++trace->battle_plan_calls;
    record_event(trace, 40);
    return 1;
}

static StarsI16 mine(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    FullTurnTrace *trace;
    (void)fleet;
    (void)order;
    trace = (FullTurnTrace *)context;
    ++trace->mine_calls;
    record_event(trace, 43);
    return STARS_SATISFY_MINE_NO_SOURCE;
}

static StarsI16 lay_mines(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    FullTurnTrace *trace;
    (void)fleet;
    (void)order;
    trace = (FullTurnTrace *)context;
    ++trace->lay_mines_calls;
    record_event(trace, 44);
    return 2;
}

static StarsI16 auto_route(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    FullTurnTrace *trace;
    (void)fleet;
    (void)order;
    trace = (FullTurnTrace *)context;
    ++trace->route_calls;
    record_event(trace, 42);
    return 1;
}

static StarsI16 transfer_player(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    FullTurnTrace *trace;
    (void)fleet;
    (void)order;
    trace = (FullTurnTrace *)context;
    ++trace->transfer_player_calls;
    record_event(trace, 43);
    return STARS_SATISFY_PLAYER_TRANSFER_CREATED;
}

static void player_message(void *context, StarsI16 recipient, StarsI16 code,
    const StarsSatisfyFleet *fleet, const StarsSatisfyOrder *order)
{
    FullTurnTrace *trace;
    (void)fleet;
    (void)order;
    trace = (FullTurnTrace *)context;
    if (trace->player_message_count < 8) {
        trace->player_codes[trace->player_message_count] = code;
        trace->player_recipients[trace->player_message_count] = recipient;
    }
    ++trace->player_message_count;
    record_event(trace, 60);
}

static void message(void *context, StarsI16 recipient, StarsI16 code,
    StarsI16 target, StarsI16 resource, StarsI32 amount)
{
    FullTurnTrace *trace;
    (void)target;
    (void)resource;
    (void)amount;
    trace = (FullTurnTrace *)context;
    if (trace->message_count < 16) {
        trace->message_codes[trace->message_count] = code;
        trace->message_recipients[trace->message_count] = recipient;
    }
    ++trace->message_count;
    record_event(trace, 50);
}

static void order_done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    FullTurnTrace *trace;
    (void)fleet;
    (void)order;
    (void)index;
    trace = (FullTurnTrace *)context;
    ++trace->done_calls;
    record_event(trace, 41);
}

static void set_battle_id(void *context, StarsU16 battle_id)
{
    FullTurnTrace *trace;
    trace = (FullTurnTrace *)context;
    trace->battle_id = battle_id;
    record_event(trace, 23);
}

static void do_battles(void *context, StarsI16 post_movement)
{
    FullTurnTrace *trace;
    trace = (FullTurnTrace *)context;
    if (post_movement != 0)
        record_event(trace, 24);
}

static void do_things(void *context, StarsI16 post_movement)
{
    FullTurnTrace *trace;
    (void)post_movement;
    trace = (FullTurnTrace *)context;
    record_event(trace, 25);
}

static void satisfy(void *context, StarsI16 pass)
{
    FullTurnTrace *trace;
    StarsSatisfyOrdersInput input;
    trace = (FullTurnTrace *)context;
    if (trace->satisfy_count < 4)
        trace->satisfy_passes[trace->satisfy_count] = pass;
    ++trace->satisfy_count;
    record_event(trace, (StarsI16)(10 + pass));
    memset(&input, 0, sizeof input);
    input.fleets = trace->fleets;
    input.fleet_count = trace->fleet_count;
    input.pass = pass;
    input.ops = &trace->ops;
    stars_satisfy_orders(&input);
}

static void drop_colonists(void *context, StarsI16 value)
{
    FullTurnTrace *trace;
    (void)value;
    trace = (FullTurnTrace *)context;
    ++trace->drop_calls;
    record_event(trace, 20);
}

static void update_research(void *context, StarsI16 value)
{
    FullTurnTrace *trace;
    (void)value;
    trace = (FullTurnTrace *)context;
    ++trace->research_calls;
    record_event(trace, 21);
}

static void transfer_to_others(void *context, StarsI16 value)
{
    FullTurnTrace *trace;
    (void)value;
    trace = (FullTurnTrace *)context;
    ++trace->transfer_to_others_calls;
    record_event(trace, 22);
}

static void initialize_ops(FullTurnTrace *trace)
{
    memset(&trace->ops, 0, sizeof trace->ops);
    trace->ops.transfer = transfer;
    trace->ops.battle_plan = battle_plan;
    trace->ops.mine = mine;
    trace->ops.lay_mines = lay_mines;
    trace->ops.auto_route = auto_route;
    trace->ops.transfer_player = transfer_player;
    trace->ops.transfer_player_message = player_message;
    trace->ops.send_message = message;
    trace->ops.order_done = order_done;
    trace->ops.context = trace;
}

static void initialize_do_orders_input(StarsDoOrdersInput *input,
    StarsOrdersPlanet *planets, FullTurnTrace *trace)
{
    memset(input, 0, sizeof *input);
    input->planets = planets;
    input->planet_count = 2;
    input->game_turn = 0x120bU;
    input->set_battle_id = set_battle_id;
    input->do_battles = do_battles;
    input->do_thing_interactions = do_things;
    input->satisfy_orders = satisfy;
    input->drop_colonists = drop_colonists;
    input->update_research = update_research;
    input->transfer_to_others = transfer_to_others;
    input->context = trace;
}

static StarsI16 sequence_is(const FullTurnTrace *trace,
    const StarsI16 *expected, StarsI16 count)
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
    FullTurnTrace trace;
    StarsOrdersPlanet planets[2];
    StarsDoOrdersInput input;
    StarsI16 pre_events[11];
    StarsI16 post_events[20];
    StarsI16 checks;
    StarsI16 failures;

    memset(&trace, 0, sizeof trace);
    memset(planets, 0, sizeof planets);
    initialize_ops(&trace);
    trace.fleet_count = 1;
    trace.fleets[0].owner = 2;
    trace.fleets[0].cargo[STARS_SATISFY_RESOURCE_MINERAL_0] = 10;
    trace.fleets[0].orders = &trace.orders[0];
    trace.fleets[0].order_count = 2;
    trace.orders[0].kind = STARS_SATISFY_ORDER_BATTLE_PLAN;
    trace.orders[0].phase_mask = (StarsI16)(1 << 1);
    trace.orders[0].active = 1;
    trace.orders[1].kind = STARS_SATISFY_ORDER_TRANSFER;
    trace.orders[1].phase_mask = (StarsI16)(1 << 2);
    trace.orders[1].target_kind = STARS_SATISFY_TARGET_FLEET;
    trace.orders[1].target_owner = 2;
    trace.orders[1].requested[STARS_SATISFY_RESOURCE_MINERAL_0] = 5;
    trace.orders[1].active = 1;
    planets[0].player = 2;
    planets[0].flags = 0xffffU;
    planets[0].turn = 44;
    planets[1].player = -1;
    planets[1].flags = 0xffffU;
    planets[1].turn = 45;
    initialize_do_orders_input(&input, planets, &trace);
    pre_events[0] = 25;
    pre_events[1] = 11;
    pre_events[2] = 40;
    pre_events[3] = 41;
    pre_events[4] = 20;
    pre_events[5] = 21;
    pre_events[6] = 12;
    pre_events[7] = 30;
    pre_events[8] = 50;
    pre_events[9] = 41;
    pre_events[10] = 22;
    stars_do_orders(0, &input);
    checks = 0;
    failures = 0;
    check(sequence_is(&trace, pre_events, 11), &checks, &failures);
    check(trace.satisfy_count == 2 && trace.satisfy_passes[0] == 1 &&
        trace.satisfy_passes[1] == 2, &checks, &failures);
    check(trace.battle_plan_calls == 1 && trace.transfer_calls == 1,
        &checks, &failures);
    check(trace.done_calls == 2 && trace.message_count == 1,
        &checks, &failures);
    check(trace.message_codes[0] == STARS_SATISFY_MESSAGE_FLEET_TRANSFER,
        &checks, &failures);
    check(trace.fleets[0].cargo[0] == 5 &&
        trace.orders[1].requested[0] == 0 && !trace.orders[1].active,
        &checks, &failures);
    check((planets[0].flags & 0x2000U) != 0U &&
        (planets[1].flags & 0x2000U) == 0U && planets[0].turn == 44 &&
        planets[1].turn == 45, &checks, &failures);
    check(trace.transfer_to_others_calls == 1 && trace.drop_calls == 1 &&
        trace.research_calls == 1, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    initialize_ops(&trace);
    trace.fleet_count = 1;
    trace.fleets[0].owner = 2;
    trace.fleets[0].id = 11;
    trace.fleets[0].flags = STARS_SATISFY_FLEET_CAN_MINE;
    trace.fleets[0].orders = &trace.orders[2];
    trace.fleets[0].order_count = 4;
    trace.orders[2].kind = STARS_SATISFY_ORDER_MINE;
    trace.orders[2].phase_mask = (StarsI16)(1 << 3);
    trace.orders[2].target_id = 8;
    trace.orders[2].active = 1;
    trace.orders[3].kind = STARS_SATISFY_ORDER_LAY_MINES;
    trace.orders[3].phase_mask = (StarsI16)(1 << 3);
    trace.orders[3].target_id = 8;
    trace.orders[3].active = 1;
    trace.orders[4].kind = STARS_SATISFY_ORDER_AUTO_ROUTE;
    trace.orders[4].phase_mask = (StarsI16)(1 << 4);
    trace.orders[4].source_cord = 1;
    trace.orders[4].target_id = 5;
    trace.orders[4].target_owner = 2;
    trace.orders[4].target_starbase_flags = 1;
    trace.orders[4].auto_route_message_variant = 1;
    trace.orders[4].active = 1;
    trace.orders[5].kind = STARS_SATISFY_ORDER_TRANSFER_PLAYER;
    trace.orders[5].phase_mask = (StarsI16)(1 << 4);
    trace.orders[5].target_owner = 4;
    trace.orders[5].target_id = 9;
    trace.orders[5].active = 1;
    planets[0].player = 2;
    planets[0].flags = 0xffffU;
    planets[0].turn = 44;
    planets[1].player = -1;
    planets[1].flags = 0xffffU;
    planets[1].turn = 45;
    initialize_do_orders_input(&input, planets, &trace);
    post_events[0] = 23;
    post_events[1] = 24;
    post_events[2] = 25;
    post_events[3] = 13;
    post_events[4] = 43;
    post_events[5] = 50;
    post_events[6] = 41;
    post_events[7] = 44;
    post_events[8] = 50;
    post_events[9] = 41;
    post_events[10] = 20;
    post_events[11] = 21;
    post_events[12] = 14;
    post_events[13] = 42;
    post_events[14] = 50;
    post_events[15] = 41;
    post_events[16] = 43;
    post_events[17] = 60;
    post_events[18] = 60;
    post_events[19] = 41;
    stars_do_orders(1, &input);
    check(sequence_is(&trace, post_events, 20), &checks, &failures);
    check(trace.satisfy_count == 2 && trace.satisfy_passes[0] == 3 &&
        trace.satisfy_passes[1] == 4, &checks, &failures);
    check(trace.battle_id == 0x0b01U && trace.mine_calls == 1 &&
        trace.lay_mines_calls == 1 && trace.route_calls == 1,
        &checks, &failures);
    check(trace.transfer_player_calls == 1 && trace.player_message_count == 2,
        &checks, &failures);
    check(trace.message_count == 3 &&
        trace.message_codes[0] == STARS_SATISFY_MESSAGE_MINE_EMPTY &&
        trace.message_codes[1] == STARS_SATISFY_MESSAGE_LAY_MINES &&
        trace.message_codes[2] == STARS_SATISFY_MESSAGE_AUTO_ROUTE,
        &checks, &failures);
    check(trace.player_codes[0] == STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_CREATED &&
        trace.player_codes[1] == STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_RECEIVED,
        &checks, &failures);
    check(trace.player_recipients[0] == 2 && trace.player_recipients[1] == 4,
        &checks, &failures);
    check(trace.done_calls == 4 && !trace.orders[2].active &&
        !trace.orders[3].active && !trace.orders[4].active &&
        !trace.orders[5].active, &checks, &failures);
    check(trace.drop_calls == 1 && trace.research_calls == 1 &&
        trace.transfer_to_others_calls == 0, &checks, &failures);
    check(planets[0].turn == 0 && planets[1].turn == 0 &&
        (planets[0].flags & 0x2000U) != 0U &&
        (planets[1].flags & 0x2000U) == 0U, &checks, &failures);

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYFTE3.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_FULL_TURN_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
                checks, failures, failures == 0 ? "PASS" : "FAIL");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_FULL_TURN_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures != 0;
}
