#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 messages;
    StarsI16 completed;
    StarsI16 drops;
    StarsI32 moved;
    StarsI16 last_code;
    StarsI16 rejection_55;
    StarsI16 rejection_56;
    StarsI16 rejection_135;
    StarsI16 mine_result;
    StarsI16 fleet_stat_calls;
    StarsI16 fleet_stat_selector;
} Trace;

static StarsI16 mine(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    return ((Trace *)context)->mine_result;
}

static StarsI16 merge(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)context;
    (void)fleet;
    (void)order;
    return 1;
}

static StarsI32 transfer(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    Trace *trace;
    (void)resource;
    (void)fleet;
    (void)order;
    trace = (Trace *)context;
    trace->moved += amount;
    return amount;
}

static StarsI32 target_capacity(void *context, const StarsSatisfyOrder *order,
    StarsI16 resource)
{
    (void)context;
    (void)order;
    return resource == STARS_SATISFY_RESOURCE_MINERAL_0 ? 3 : 100;
}

static StarsI32 fleet_stat(void *context, const StarsSatisfyFleet *fleet,
    StarsI16 selector)
{
    Trace *trace;
    (void)fleet;
    trace = (Trace *)context;
    ++trace->fleet_stat_calls;
    trace->fleet_stat_selector = selector;
    return 50;
}

static void drop(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI32 amount)
{
    Trace *trace;
    (void)fleet;
    (void)order;
    (void)amount;
    trace = (Trace *)context;
    ++trace->drops;
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
    if (code == STARS_SATISFY_MESSAGE_COLONISTS_NO_UNOWNED_DROP)
        ++trace->rejection_55;
    if (code == STARS_SATISFY_MESSAGE_COLONISTS_RACE_BLOCKED)
        ++trace->rejection_56;
    if (code == STARS_SATISFY_MESSAGE_COLONISTS_PLANET_BLOCKED)
        ++trace->rejection_135;
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 order_index)
{
    Trace *trace;
    (void)fleet;
    (void)order;
    (void)order_index;
    trace = (Trace *)context;
    ++trace->completed;
}

int main(void)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder orders[4];
    StarsSatisfyOrdersOps ops;
    StarsSatisfyOrdersInput input;
    Trace trace;

    memset(&fleet, 0, sizeof fleet);
    memset(orders, 0, sizeof orders);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleet.owner = 3;
    trace.mine_result = STARS_SATISFY_MINE_NO_PLANET;
    fleet.flags = STARS_SATISFY_FLEET_CAN_MINE;
    fleet.cargo[0] = 12;
    fleet.cargo[3] = 9;
    fleet.cargo[4] = 4;
    fleet.capacity[0] = 10;
    fleet.capacity[3] = 20;
    fleet.capacity[4] = 4;
    fleet.orders = orders;
    fleet.order_count = 4;
    orders[0].kind = STARS_SATISFY_ORDER_TRANSFER;
    orders[0].phase_mask = 1 << 2;
    orders[0].target_kind = 1;
    orders[0].target_id = 7;
    orders[0].requested[0] = 5;
    orders[0].requested[3] = 4;
    orders[0].active = 1;
    orders[1].kind = STARS_SATISFY_ORDER_TRANSFER;
    orders[1].phase_mask = 1 << 1;
    orders[1].target_kind = STARS_SATISFY_TARGET_FLEET;
    orders[1].requested[4] = 4;
    orders[1].active = 1;
    orders[2].kind = STARS_SATISFY_ORDER_MERGE;
    orders[2].phase_mask = 1 << 2;
    orders[2].target_id = 8;
    orders[2].target_owner = fleet.owner;
    orders[2].target_present = 1;
    orders[2].active = 1;
    orders[3].kind = STARS_SATISFY_ORDER_MINE;
    orders[3].phase_mask = 1 << 3;
    orders[3].target_id = 9;
    orders[3].active = 1;
    ops.transfer = transfer;
    ops.mine = mine;
    ops.merge = merge;
    ops.queue_colonist_drop = drop;
    ops.send_message = message;
    ops.order_done = done;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 2;
    input.ops = &ops;
    stars_satisfy_orders(&input);
    if (trace.messages != 3 || trace.drops != 1 || trace.completed != 2 ||
            trace.moved != 9 || fleet.cargo[0] != 7 || fleet.cargo[3] != 5 ||
            orders[0].active != 0 || orders[1].active != 1 ||
            orders[2].active != 0 || orders[3].active != 1 ||
            trace.last_code != STARS_SATISFY_MESSAGE_MERGE_OK)
        return 1;
    input.pass = 1;
    stars_satisfy_orders(&input);
    if (trace.messages != 4 || trace.completed != 3 || fleet.cargo[4] != 0 ||
            orders[1].active != 0 ||
            trace.last_code != STARS_SATISFY_MESSAGE_FLEET_TRANSFER)
        return 2;
    input.pass = 3;
    stars_satisfy_orders(&input);
    if (trace.messages != 5 || trace.completed != 4 || orders[3].active != 0 ||
            trace.last_code != STARS_SATISFY_MESSAGE_MINE_NO_PLANET)
        return 3;
    memset(&orders[3], 0, sizeof orders[3]);
    orders[3].kind = STARS_SATISFY_ORDER_MINE;
    orders[3].active = 1;
    fleet.flags = 0;
    input.pass = 3;
    stars_satisfy_orders(&input);
    if (orders[3].active != 1 || trace.completed != 4)
        return 7;
    fleet.flags = STARS_SATISFY_FLEET_CAN_MINE;
    trace.mine_result = STARS_SATISFY_MINE_SUCCESS;
    stars_satisfy_orders(&input);
    if (orders[3].active != 0 || trace.messages != 5 || trace.completed != 5)
        return 8;
    fleet.turn_flags = 0x00e0U;
    input.pass = 1;
    stars_satisfy_orders(&input);
    if (fleet.turn_flags != 0x0080U)
        return 4;
    fleet.turn_flags = 0x0080U;
    orders[0].active = 1;
    stars_satisfy_orders(&input);
    if (orders[0].active != 1)
        return 5;
    memset(&trace, 0, sizeof trace);
    memset(&orders[0], 0, sizeof orders[0]);
    fleet.turn_flags = 0;
    fleet.cargo[0] = 5;
    orders[0].kind = STARS_SATISFY_ORDER_TRANSFER;
    orders[0].target_kind = STARS_SATISFY_TARGET_PLANET;
    orders[0].target_id = 12;
    orders[0].requested[0] = 5;
    orders[0].active = 1;
    fleet.order_count = 1;
    input.pass = 2;
    ops.target_capacity = target_capacity;
    stars_satisfy_orders(&input);
    if (fleet.cargo[0] != 2 || orders[0].requested[0] != 2 ||
            orders[0].active != 1 || trace.moved != 3)
        return 6;
    memset(&trace, 0, sizeof trace);
    fleet.cargo[0] = 5;
    orders[0].requested[0] = 5;
    orders[0].active = 1;
    ops.transfer = 0;
    stars_satisfy_orders(&input);
    if (fleet.cargo[0] != 5 || orders[0].requested[0] != 5 ||
            orders[0].active != 1 || trace.messages != 0 ||
            trace.completed != 0)
        return 8;
    memset(&trace, 0, sizeof trace);
    fleet.cargo[STARS_SATISFY_RESOURCE_FUEL] = 4;
    orders[0].target_kind = STARS_SATISFY_TARGET_PLANET;
    orders[0].requested[0] = 0;
    orders[0].requested[STARS_SATISFY_RESOURCE_FUEL] = 3;
    orders[0].active = 1;
    ops.transfer = transfer;
    stars_satisfy_orders(&input);
    if (fleet.cargo[STARS_SATISFY_RESOURCE_FUEL] != 4 ||
            orders[0].requested[STARS_SATISFY_RESOURCE_FUEL] != 3 ||
            orders[0].active != 1 || trace.moved != 0 ||
            trace.completed != 0)
        return 9;
    memset(&trace, 0, sizeof trace);
    fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] = 4;
    orders[0].target_kind = STARS_SATISFY_TARGET_THING;
    orders[0].requested[STARS_SATISFY_RESOURCE_FUEL] = 0;
    orders[0].requested[STARS_SATISFY_RESOURCE_COLONISTS] = 3;
    orders[0].active = 1;
    stars_satisfy_orders(&input);
    if (fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] != 4 ||
            orders[0].requested[STARS_SATISFY_RESOURCE_COLONISTS] != 3 ||
            orders[0].active != 1 || trace.moved != 0 ||
            trace.completed != 0)
        return 10;
    memset(&trace, 0, sizeof trace);
    memset(&orders[0], 0, sizeof orders[0]);
    fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] = 4;
    fleet.order_count = 1;
    orders[0].kind = STARS_SATISFY_ORDER_TRANSFER;
    orders[0].target_kind = STARS_SATISFY_TARGET_PLANET;
    orders[0].target_owner = -1;
    orders[0].requested[STARS_SATISFY_RESOURCE_COLONISTS] = 2;
    orders[0].active = 1;
    stars_satisfy_orders(&input);
    if (trace.rejection_55 != 1 || trace.drops != 0 ||
            trace.messages != 2 || orders[0].active)
        return 11;
    memset(&trace, 0, sizeof trace);
    fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] = 4;
    orders[0].target_planet_flags = 0x2000U;
    orders[0].colonist_race_stat = 8;
    orders[0].active = 1;
    orders[0].requested[STARS_SATISFY_RESOURCE_COLONISTS] = 2;
    stars_satisfy_orders(&input);
    if (trace.rejection_56 != 1 || trace.rejection_55 != 0 ||
            trace.drops != 0 || orders[0].active)
        return 12;
    memset(&trace, 0, sizeof trace);
    fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] = 4;
    orders[0].target_planet_flags = 0x2200U;
    orders[0].colonist_race_stat = 7;
    orders[0].active = 1;
    orders[0].requested[STARS_SATISFY_RESOURCE_COLONISTS] = 2;
    stars_satisfy_orders(&input);
    if (trace.rejection_135 != 1 || trace.drops != 0 || orders[0].active)
        return 13;
    memset(&trace, 0, sizeof trace);
    fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] = 4;
    orders[0].target_planet_flags = 0x2000U;
    orders[0].colonist_race_stat = 7;
    orders[0].active = 1;
    orders[0].requested[STARS_SATISFY_RESOURCE_COLONISTS] = 2;
    stars_satisfy_orders(&input);
    if (trace.drops != 1 || trace.rejection_55 != 0 ||
            trace.rejection_56 != 0 || trace.rejection_135 != 0)
        return 14;
    memset(&trace, 0, sizeof trace);
    fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] = 4;
    orders[0].target_kind = STARS_SATISFY_TARGET_FLEET;
    orders[0].target_owner = fleet.owner;
    orders[0].requested[STARS_SATISFY_RESOURCE_COLONISTS] = 2;
    orders[0].active = 1;
    stars_satisfy_orders(&input);
    if (trace.messages != 1 ||
            trace.last_code != STARS_SATISFY_MESSAGE_FLEET_TRANSFER_COLONISTS)
        return 15;
    memset(&trace, 0, sizeof trace);
    fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] = 4;
    orders[0].target_owner = (StarsI16)(fleet.owner + 1);
    orders[0].requested[STARS_SATISFY_RESOURCE_COLONISTS] = 2;
    orders[0].active = 1;
    stars_satisfy_orders(&input);
    if (trace.messages != 1 ||
            trace.last_code != STARS_SATISFY_MESSAGE_FOREIGN_FLEET_COLONISTS)
        return 16;
    memset(&trace, 0, sizeof trace);
    fleet.cargo[STARS_SATISFY_RESOURCE_MINERAL_0] = 0;
    orders[0].target_kind = STARS_SATISFY_TARGET_FLEET;
    orders[0].target_owner = fleet.owner;
    orders[0].requested[STARS_SATISFY_RESOURCE_COLONISTS] = 0;
    orders[0].requested[STARS_SATISFY_RESOURCE_MINERAL_0] = 2;
    orders[0].active = 1;
    stars_satisfy_orders(&input);
    if (trace.messages != 0 || orders[0].active != 1)
        return 17;
    input.pass = 4;
    memset(&trace, 0, sizeof trace);
    stars_satisfy_orders(&input);
    if (trace.messages != 1 ||
            trace.last_code != STARS_SATISFY_MESSAGE_FLEET_SHORTAGE)
        return 17;
    memset(&trace, 0, sizeof trace);
    orders[0].target_kind = STARS_SATISFY_TARGET_POINT;
    orders[0].active = 1;
    stars_satisfy_orders(&input);
    if (trace.messages != 1 ||
            trace.last_code != STARS_SATISFY_MESSAGE_POINT_SHORTAGE)
        return 18;
    memset(&trace, 0, sizeof trace);
    memset(&orders[0], 0, sizeof orders[0]);
    fleet.cargo[STARS_SATISFY_RESOURCE_MINERAL_0] = 10;
    fleet.capacity[STARS_SATISFY_RESOURCE_MINERAL_0] = 100;
    orders[0].kind = STARS_SATISFY_ORDER_TRANSFER;
    orders[0].target_kind = STARS_SATISFY_TARGET_PLANET;
    orders[0].requested[STARS_SATISFY_RESOURCE_MINERAL_0] = 40;
    orders[0].resource_mode[STARS_SATISFY_RESOURCE_MINERAL_0] =
        STARS_SATISFY_RESOURCE_MODE_STAT_PERCENT;
    orders[0].active = 1;
    fleet.order_count = 1;
    input.pass = 1;
    ops.target_capacity = 0;
    ops.fleet_stat = fleet_stat;
    stars_satisfy_orders(&input);
    if (trace.fleet_stat_calls != 0 || trace.moved != 0 ||
            orders[0].active != 1)
        return 19;
    input.pass = 2;
    stars_satisfy_orders(&input);
    if (trace.fleet_stat_calls != 1 || trace.fleet_stat_selector != 2 ||
            trace.moved != 10 || fleet.cargo[0] != 0 ||
            orders[0].requested[0] != 30 || orders[0].active != 1)
        return 20;
    puts("SATISFY_ORDERS_CHECKS=32 FAILURES=0 RESULT=PASS");
    puts("SATISFY_ORDERS_STAT_PERCENT_MODE=PASS");
    puts("SATISFY_ORDERS_TRANSFER_CALLBACK_GATE=PASS");
    return 0;
}
