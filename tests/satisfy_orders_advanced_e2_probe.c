#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 technology;
    StarsI16 salvage_calls;
    StarsI16 ship_copy;
    StarsI16 layouts;
    StarsI16 layout_recipients[4];
    StarsI16 layout_codes[4];
    StarsI32 layout_amounts[4];
    StarsI16 layout_resources[4];
    StarsI16 transfer_messages;
    StarsI32 technology_amount;
    StarsI32 salvage_values[STARS_SATISFY_RESOURCE_COUNT];
    StarsI16 transfer_result;
} Trace;

static StarsI32 transfer(void *context, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    (void)context;
    (void)fleet;
    (void)order;
    (void)resource;
    return amount;
}

static void technology(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI32 amount)
{
    Trace *trace;
    (void)fleet;
    (void)order;
    trace = (Trace *)context;
    ++trace->technology;
    trace->technology_amount = amount;
}

static void salvage(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order,
    const StarsI32 minerals[STARS_SATISFY_RESOURCE_COUNT])
{
    Trace *trace;
    StarsI16 index;
    (void)fleet;
    (void)order;
    trace = (Trace *)context;
    ++trace->salvage_calls;
    for (index = 0; index < STARS_SATISFY_RESOURCE_COUNT; ++index)
        trace->salvage_values[index] = minerals[index];
}

static StarsI16 transfer_player(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    return ((Trace *)context)->transfer_result;
}

static void ship_copy(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 result)
{
    (void)fleet;
    (void)order;
    if (result == STARS_SATISFY_PLAYER_TRANSFER_CREATED)
        ++((Trace *)context)->ship_copy;
}

static void transfer_player_message(void *context, StarsI16 recipient,
    StarsI16 code, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)recipient;
    (void)code;
    (void)fleet;
    (void)order;
    ++((Trace *)context)->transfer_messages;
}

static void message_layout(void *context, StarsI16 recipient,
    StarsI16 code, StarsI16 target, StarsI16 resource, StarsI32 amount,
    StarsI16 variant)
{
    Trace *trace;
    StarsI16 index;
    (void)target;
    (void)variant;
    trace = (Trace *)context;
    index = trace->layouts;
    if (index < 4) {
        trace->layout_recipients[index] = recipient;
        trace->layout_codes[index] = code;
        trace->layout_resources[index] = resource;
        trace->layout_amounts[index] = amount;
    }
    ++trace->layouts;
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    (void)context;
    (void)fleet;
    (void)order;
    (void)index;
}

static void put_word(StarsU8 *record, StarsI16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1] = (StarsU8)(value >> 8);
}

int main(void)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersOps ops;
    StarsSatisfyOrdersInput input;
    StarsSatisfyPackedInput packed;
    StarsU8 fleet_record[124];
    StarsU8 order_record[18];
    Trace trace;
    StarsI16 failures;

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleet.owner = 2;
    fleet.cargo[0] = 5;
    fleet.cargo[1] = 2;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_TRANSFER;
    order.phase_mask = 1 << 2;
    order.target_kind = STARS_SATISFY_TARGET_PLANET;
    order.target_id = 7;
    order.requested[0] = 3;
    order.active = 1;
    order.technology_flag = 1;
    ops.transfer = transfer;
    ops.technology_discovery = technology;
    ops.create_salvage = salvage;
    ops.send_message = 0;
    ops.message_layout = message_layout;
    ops.order_done = done;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 2;
    input.ops = &ops;
    stars_satisfy_orders(&input);

    failures = trace.technology != 1 || trace.technology_amount != 3 ||
        trace.layouts != 1 || trace.layout_codes[0] !=
        STARS_SATISFY_MESSAGE_TRANSFER || trace.layout_amounts[0] != 3;

    memset(&trace, 0, sizeof trace);
    memset(&order, 0, sizeof order);
    fleet.cargo[0] = 5;
    fleet.cargo[1] = 2;
    order.kind = STARS_SATISFY_ORDER_TRANSFER;
    order.phase_mask = 1 << 2;
    order.target_kind = STARS_SATISFY_TARGET_POINT;
    order.target_id = -1;
    order.requested[0] = 3;
    order.requested[1] = 2;
    order.active = 1;
    input.pass = 2;
    stars_satisfy_orders(&input);
    failures = failures || trace.salvage_calls != 1 ||
        trace.salvage_values[0] != 3 || trace.salvage_values[1] != 2 ||
        trace.salvage_values[2] != 0 || trace.layouts != 2;

    memset(&trace, 0, sizeof trace);
    memset(&order, 0, sizeof order);
    order.kind = STARS_SATISFY_ORDER_TRANSFER_PLAYER;
    order.active = 1;
    order.target_owner = 4;
    order.target_id = 9;
    trace.transfer_result = STARS_SATISFY_PLAYER_TRANSFER_CREATED;
    ops.transfer = 0;
    ops.technology_discovery = 0;
    ops.create_salvage = 0;
    ops.transfer_player = transfer_player;
    ops.transfer_player_message = transfer_player_message;
    ops.ship_copy = ship_copy;
    input.pass = 4;
    stars_satisfy_orders(&input);
    failures = failures || trace.ship_copy != 1 ||
        trace.transfer_messages != 2 || trace.layouts != 2 ||
        trace.layout_codes[0] != STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_CREATED ||
        trace.layout_codes[1] != STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_RECEIVED ||
        trace.layout_recipients[0] != 2 || trace.layout_recipients[1] != 4;

    memset(&trace, 0, sizeof trace);
    memset(&order, 0, sizeof order);
    order.kind = STARS_SATISFY_ORDER_TRANSFER_PLAYER;
    order.active = 1;
    order.target_owner = 4;
    trace.transfer_result = STARS_SATISFY_PLAYER_TRANSFER_NO_DESIGN;
    stars_satisfy_orders(&input);
    failures = failures || trace.ship_copy != 0 || trace.layouts != 2 ||
        trace.layout_codes[0] != STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_DESIGN ||
        trace.layout_codes[1] != STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_SHIP;

    memset(&trace, 0, sizeof trace);
    memset(fleet_record, 0, sizeof fleet_record);
    memset(order_record, 0, sizeof order_record);
    put_word(fleet_record, 2, 2);
    put_word(order_record, 4, 7);
    put_word(order_record, 6, (StarsU16)(STARS_SATISFY_ORDER_TRANSFER |
        (STARS_SATISFY_TARGET_PLANET << 8) | (1U << 12)));
    memset(&packed, 0, sizeof packed);
    packed.fleet_record = fleet_record;
    packed.fleet_size = 124;
    packed.order_record = order_record;
    packed.order_size = 18;
    packed.cargo[0] = 4;
    packed.requested[0] = 4;
    packed.phase_mask = 1 << 2;
    packed.pass = 2;
    packed.technology_flag = 1;
    packed.ops = &ops;
    ops.context = &trace;
    ops.transfer = transfer;
    ops.technology_discovery = technology;
    ops.transfer_player = 0;
    ops.transfer_player_message = 0;
    ops.ship_copy = 0;
    ops.message_layout = message_layout;
    stars_satisfy_orders_packed(&packed);
    failures = failures || trace.technology != 1 ||
        trace.technology_amount != 4 || trace.layouts != 1 ||
        trace.layout_codes[0] != STARS_SATISFY_MESSAGE_TRANSFER;

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYOE7.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_ADVANCED_E2_CHECKS=20 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    fprintf(stdout,
        "SATISFY_ORDERS_ADVANCED_E2_CHECKS=20 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
