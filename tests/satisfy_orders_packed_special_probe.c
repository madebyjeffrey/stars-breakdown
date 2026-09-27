#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 battle;
    StarsI16 mine;
    StarsI16 route;
    StarsI16 player_transfer;
    StarsI16 player_messages;
    StarsI16 messages;
    StarsI16 done;
    StarsI16 last_code;
} Trace;

static void put_word(StarsU8 *record, StarsI16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1] = (StarsU8)(value >> 8);
}

static void set_order(StarsU8 *record, StarsI16 kind)
{
    put_word(record, 6, (StarsU16)(kind | (2U << 8) | (1U << 12)));
}

static StarsI16 battle(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    ++((Trace *)context)->battle;
    return 1;
}

static StarsI16 mine(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    ++((Trace *)context)->mine;
    return STARS_SATISFY_MINE_SUCCESS;
}

static StarsI16 merge(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)context;
    (void)fleet;
    (void)order;
    return 1;
}

static StarsI16 route(void *context, StarsI16 unused,
    const StarsSatisfyOrder *order)
{
    (void)unused;
    (void)order;
    ++((Trace *)context)->route;
    return 1;
}

static StarsI16 route_fleet(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    return route(context, fleet->owner, order);
}

static StarsI16 player_transfer(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    ++((Trace *)context)->player_transfer;
    return STARS_SATISFY_PLAYER_TRANSFER_CREATED;
}

static void player_message(void *context, StarsI16 recipient,
    StarsI16 code, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    Trace *trace;
    (void)recipient;
    (void)fleet;
    (void)order;
    trace = (Trace *)context;
    ++trace->player_messages;
    trace->last_code = code;
}

static void message(void *context, StarsI16 owner, StarsI16 code,
    StarsI16 target, StarsI16 resource, StarsI32 amount)
{
    Trace *trace;
    (void)owner;
    (void)target;
    (void)resource;
    (void)amount;
    trace = (Trace *)context;
    ++trace->messages;
    trace->last_code = code;
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    (void)fleet;
    (void)order;
    (void)index;
    ++((Trace *)context)->done;
}

static StarsI16 run_special(StarsU8 *fleet_record, StarsU8 *order_record,
    StarsI16 pass, StarsI16 kind, StarsSatisfyOrdersOps *ops,
    Trace *trace)
{
    StarsSatisfyPackedInput input;

    memset(&input, 0, sizeof input);
    set_order(order_record, kind);
    input.fleet_record = fleet_record;
    input.fleet_size = 124;
    input.order_record = order_record;
    input.order_size = 18;
    input.pass = pass;
    input.target_owner = 4;
    input.target_present = 1;
    input.source_cord = 0;
    input.ops = ops;
    ops->context = trace;
    return stars_satisfy_orders_packed(&input);
}

int main(void)
{
    StarsU8 fleet_record[124];
    StarsU8 order_record[18];
    StarsSatisfyOrdersOps ops;
    Trace trace;
    StarsI16 failures;

    memset(fleet_record, 0, sizeof fleet_record);
    memset(order_record, 0, sizeof order_record);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    put_word(fleet_record, 2, 4);
    put_word(fleet_record, 4, STARS_SATISFY_FLEET_CAN_MINE);
    ops.battle_plan = battle;
    ops.mine = mine;
    ops.merge = merge;
    ops.auto_fleet_order = route_fleet;
    ops.transfer_player = player_transfer;
    ops.transfer_player_message = player_message;
    ops.send_message = message;
    ops.order_done = done;

    failures = !run_special(fleet_record, order_record, 1,
        STARS_SATISFY_ORDER_BATTLE_PLAN, &ops, &trace) ||
        trace.battle != 1 || trace.done != 1;
    memset(&trace, 0, sizeof trace);
    failures = failures || !run_special(fleet_record, order_record, 3,
        STARS_SATISFY_ORDER_MINE, &ops, &trace) ||
        trace.mine != 1 || trace.done != 1;
    memset(&trace, 0, sizeof trace);
    failures = failures || !run_special(fleet_record, order_record, 2,
        STARS_SATISFY_ORDER_MERGE, &ops, &trace) ||
        trace.messages != 1 || trace.last_code != STARS_SATISFY_MESSAGE_MERGE_OK;
    memset(&trace, 0, sizeof trace);
    failures = failures || !run_special(fleet_record, order_record, 4,
        STARS_SATISFY_ORDER_AUTO_ROUTE, &ops, &trace) ||
        trace.route != 1 || trace.done != 1;
    memset(&trace, 0, sizeof trace);
    failures = failures || !run_special(fleet_record, order_record, 4,
        STARS_SATISFY_ORDER_TRANSFER_PLAYER, &ops, &trace) ||
        trace.player_transfer != 1 || trace.player_messages != 2 ||
        trace.last_code != STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_RECEIVED;
    printf("SATISFY_ORDERS_PACKED_SPECIAL_CHECKS=10 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
