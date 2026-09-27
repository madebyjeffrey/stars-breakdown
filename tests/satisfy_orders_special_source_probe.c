#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct SpecialTrace {
    StarsI16 battle;
    StarsI16 lay;
    StarsI16 route;
    StarsI16 transfer_player;
    StarsI16 messages;
    StarsI16 completed;
    StarsI16 last_code;
    StarsI16 player_messages;
    StarsI16 player_recipients;
    StarsI16 player_result;
} SpecialTrace;

static StarsI16 battle_plan(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    SpecialTrace *trace;
    (void)fleet;
    (void)order;
    trace = (SpecialTrace *)opaque;
    ++trace->battle;
    return 1;
}

static StarsI16 lay_mines(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    SpecialTrace *trace;
    (void)fleet;
    (void)order;
    trace = (SpecialTrace *)opaque;
    ++trace->lay;
    return 1;
}

static StarsI16 auto_route(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    SpecialTrace *trace;
    (void)fleet;
    (void)order;
    trace = (SpecialTrace *)opaque;
    ++trace->route;
    return 1;
}

static StarsI16 auto_fleet_order(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    return auto_route(opaque, fleet, order);
}

static StarsI16 transfer_player(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    SpecialTrace *trace;
    (void)fleet;
    (void)order;
    trace = (SpecialTrace *)opaque;
    ++trace->transfer_player;
    return trace->player_result;
}

static void transfer_player_message(void *opaque, StarsI16 recipient,
    StarsI16 code, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    SpecialTrace *trace;
    (void)fleet;
    (void)order;
    trace = (SpecialTrace *)opaque;
    ++trace->player_messages;
    if (recipient == 4)
        ++trace->player_recipients;
    trace->last_code = code;
}

static StarsI16 merge(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)opaque;
    (void)fleet;
    (void)order;
    return 1;
}

static void message(void *opaque, StarsI16 owner, StarsI16 code,
    StarsI16 target, StarsI16 resource, StarsI32 amount)
{
    SpecialTrace *trace;
    (void)owner;
    (void)target;
    (void)resource;
    (void)amount;
    trace = (SpecialTrace *)opaque;
    ++trace->messages;
    trace->last_code = code;
}

static void order_done(void *opaque, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    SpecialTrace *trace;
    (void)fleet;
    (void)order;
    (void)index;
    trace = (SpecialTrace *)opaque;
    ++trace->completed;
}

int main(void)
{
    SpecialTrace trace;
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder orders[4];
    StarsSatisfyOrdersOps ops;
    StarsSatisfyOrdersInput input;
    StarsI16 failures;

    memset(&trace, 0, sizeof trace);
    memset(&fleet, 0, sizeof fleet);
    memset(orders, 0, sizeof orders);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    fleet.owner = 4;
    fleet.flags = STARS_SATISFY_FLEET_CAN_MINE;
    fleet.orders = orders;
    fleet.order_count = 4;
    orders[0].kind = STARS_SATISFY_ORDER_BATTLE_PLAN;
    orders[0].active = 1;
    orders[1].kind = STARS_SATISFY_ORDER_LAY_MINES;
    orders[1].active = 1;
    orders[2].kind = STARS_SATISFY_ORDER_AUTO_ROUTE;
    orders[2].active = 1;
    orders[3].kind = STARS_SATISFY_ORDER_TRANSFER_PLAYER;
    orders[3].active = 1;
    ops.battle_plan = battle_plan;
    ops.lay_mines = lay_mines;
    ops.auto_route = auto_route;
    ops.auto_route_at_starbase = auto_route;
    ops.auto_fleet_order = auto_fleet_order;
    ops.transfer_player = transfer_player;
    ops.transfer_player_message = transfer_player_message;
    ops.send_message = message;
    ops.order_done = order_done;
    ops.context = &trace;
    trace.player_result = STARS_SATISFY_PLAYER_TRANSFER_NO_DESIGN;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 1;
    input.ops = &ops;
    stars_satisfy_orders(&input);
    input.pass = 3;
    stars_satisfy_orders(&input);
    input.pass = 4;
    stars_satisfy_orders(&input);
    failures = trace.battle != 1 || trace.lay != 1 || trace.route != 1 ||
        trace.transfer_player != 1 || trace.messages != 1 ||
        trace.player_messages != 2 || trace.player_recipients != 1 ||
        trace.completed != 4 || orders[0].active || orders[1].active ||
        orders[2].active || orders[3].active ||
        trace.last_code != STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_SHIP;
    printf("SATISFY_ORDERS_SPECIAL_CHECKS=16 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    if (failures != 0)
        return 1;

    /* The raw branch permits the race-stat path even when the fleet flag is
     * absent, and leaves the order untouched when neither capability exists. */
    memset(&trace, 0, sizeof trace);
    memset(orders, 0, sizeof orders);
    fleet.flags = 0;
    orders[0].kind = STARS_SATISFY_ORDER_LAY_MINES;
    orders[0].active = 1;
    fleet.order_count = 1;
    input.pass = 3;
    input.lay_mines_race_stat = 0;
    stars_satisfy_orders(&input);
    if (!orders[0].active || trace.lay != 0 || trace.completed != 0)
        return 2;
    input.lay_mines_race_stat = 5;
    stars_satisfy_orders(&input);
    if (orders[0].active || trace.lay != 1 || trace.completed != 1)
        return 3;

    memset(&trace, 0, sizeof trace);
    memset(orders, 0, sizeof orders);
    fleet.flags = 0;
    fleet.order_count = 1;
    orders[0].kind = STARS_SATISFY_ORDER_AUTO_ROUTE;
    orders[0].source_cord = 1;
    orders[0].target_id = 9;
    orders[0].target_owner = fleet.owner;
    orders[0].target_starbase_flags = 1;
    orders[0].active = 1;
    input.pass = 3;
    stars_satisfy_orders(&input);
    if (orders[0].active || trace.route != 1 || trace.completed != 1 ||
            trace.messages != 1 ||
            trace.last_code != STARS_SATISFY_MESSAGE_AUTO_ROUTE_NO_PATH)
        return 4;
    memset(&trace, 0, sizeof trace);
    memset(orders, 0, sizeof orders);
    fleet.flags = STARS_SATISFY_FLEET_CAN_MINE;
    fleet.order_count = 1;
    orders[0].kind = STARS_SATISFY_ORDER_LAY_MINES;
    orders[0].active = 1;
    orders[0].repeat_count = 2;
    input.pass = 3;
    ops.lay_mines = lay_mines;
    stars_satisfy_orders(&input);
    if (!orders[0].active || orders[0].repeat_count != 1 ||
            trace.lay != 1 || trace.messages != 1 || trace.completed != 0)
        return 5;
    orders[0].repeat_count = 5;
    stars_satisfy_orders(&input);
    if (!orders[0].active || orders[0].repeat_count != 5 ||
            trace.lay != 2 || trace.messages != 2 || trace.completed != 0)
        return 6;
    orders[0].repeat_count = 0;
    stars_satisfy_orders(&input);
    if (orders[0].active || trace.lay != 3 || trace.completed != 1)
        return 7;
    memset(&trace, 0, sizeof trace);
    memset(orders, 0, sizeof orders);
    fleet.order_count = 1;
    orders[0].kind = STARS_SATISFY_ORDER_MERGE;
    orders[0].active = 1;
    orders[0].target_owner = fleet.owner;
    orders[0].target_present = 1;
    input.pass = 2;
    ops.merge = 0;
    stars_satisfy_orders(&input);
    if (!orders[0].active || trace.messages != 0 || trace.completed != 0)
        return 8;
    memset(&trace, 0, sizeof trace);
    memset(orders, 0, sizeof orders);
    fleet.order_count = 1;
    orders[0].kind = STARS_SATISFY_ORDER_MERGE;
    orders[0].target_owner = (StarsI16)(fleet.owner + 1);
    orders[0].target_present = 1;
    orders[0].active = 1;
    ops.merge = merge;
    stars_satisfy_orders(&input);
    if (orders[0].active || trace.messages != 1 ||
            trace.last_code != STARS_SATISFY_MESSAGE_MERGE_ENEMY ||
            trace.completed != 1)
        return 9;
    memset(&trace, 0, sizeof trace);
    memset(orders, 0, sizeof orders);
    orders[0].kind = STARS_SATISFY_ORDER_MERGE;
    orders[0].active = 1;
    stars_satisfy_orders(&input);
    if (orders[0].active || trace.messages != 1 ||
            trace.last_code != STARS_SATISFY_MESSAGE_MERGE_INVALID ||
            trace.completed != 1)
        return 10;
    puts("SATISFY_ORDERS_SPECIAL_MINE_GATE=PASS");
    puts("SATISFY_ORDERS_SPECIAL_CALLBACK_GATES=PASS");

    /* The remaining player-transfer results preserve the raw source-side
     * message selection when no paired recipient message is required. */
    {
        StarsI16 results[3];
        StarsI16 expected[3];
        StarsI16 i;
        results[0] = STARS_SATISFY_PLAYER_TRANSFER_INVALID_TARGET;
        results[1] = STARS_SATISFY_PLAYER_TRANSFER_NO_SHIP;
        results[2] = STARS_SATISFY_PLAYER_TRANSFER_BLOCKED;
        expected[0] = STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_INVALID;
        expected[1] = STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_SHIP;
        expected[2] = STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_BLOCKED;
        for (i = 0; i < 3; ++i) {
            memset(&trace, 0, sizeof trace);
            memset(orders, 0, sizeof orders);
            orders[0].kind = STARS_SATISFY_ORDER_TRANSFER_PLAYER;
            orders[0].active = 1;
            orders[0].target_owner = 7;
            input.pass = 4;
            trace.player_result = results[i];
            ops.transfer_player = transfer_player;
            stars_satisfy_orders(&input);
            if (trace.last_code != expected[i] || trace.player_messages != 1 ||
                    trace.completed != 1 || orders[0].active)
                return 11;
        }
        memset(&trace, 0, sizeof trace);
        memset(orders, 0, sizeof orders);
        orders[0].kind = STARS_SATISFY_ORDER_TRANSFER_PLAYER;
        orders[0].active = 1;
        orders[0].target_owner = 7;
        trace.player_result = STARS_SATISFY_PLAYER_TRANSFER_CREATED;
        stars_satisfy_orders(&input);
        if (trace.last_code != STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_RECEIVED ||
                trace.player_messages != 2 || trace.player_recipients != 1 ||
                trace.completed != 1 || orders[0].active)
            return 12;
    }
    return 0;
}
