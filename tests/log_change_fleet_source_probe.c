#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 count;
    StarsU16 type[8];
    StarsU16 length[8];
    StarsU8 record[8][40];
    StarsU16 previous_calls;
} Trace;

static void write_memory(void *context, StarsU16 type, StarsU16 length,
    const StarsU8 *record)
{
    Trace *trace = (Trace *)context;
    StarsU16 n = trace->count++;
    trace->type[n] = type;
    trace->length[n] = length;
    memcpy(trace->record[n], record, length);
}

static StarsI16 previous_order(void *context, StarsU16 fleet, StarsU16 index)
{
    Trace *trace = (Trace *)context;
    (void)fleet;
    (void)index;
    trace->previous_calls++;
    return 1;
}

static void init_fleet(StarsLogFleet *fleet)
{
    memset(fleet, 0, sizeof *fleet);
    fleet->object_id = 0x1234U;
}

int main(void)
{
    StarsLogFleet old_fleet, new_fleet;
    StarsLogChangeFleetState state;
    StarsLogChangeFleetOps ops;
    Trace trace;
    StarsU8 old_orders[36];
    StarsU8 new_orders[36];

    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.write_memory = write_memory;
    ops.previous_order_replace = previous_order;
    init_fleet(&old_fleet);
    init_fleet(&new_fleet);
    new_fleet.ship_counts[2] = 3U;
    stars_log_change_fleet(&old_fleet, &new_fleet, 0U, &state, &ops, &trace);
    if (!state.valid_xferf || trace.count != 0U) return 1;
    init_fleet(&old_fleet);
    init_fleet(&new_fleet);
    new_fleet.ship_counts[2] = 4U;
    stars_log_change_fleet(&old_fleet, &new_fleet, 0U, &state, &ops, &trace);
    if (state.valid_xferf || trace.count != 1U || trace.type[0] != 0x17U ||
            trace.length[0] != 9U || trace.record[0][7] != 3U) return 2;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    init_fleet(&old_fleet);
    init_fleet(&new_fleet);
    new_fleet.minerals[1] = 10U;
    stars_log_change_fleet(&old_fleet, &new_fleet, 0U, &state, &ops, &trace);
    init_fleet(&old_fleet);
    init_fleet(&new_fleet);
    new_fleet.minerals[1] = 20U;
    stars_log_change_fleet(&old_fleet, &new_fleet, 0U, &state, &ops, &trace);
    if (state.valid_xfer || trace.count != 1U || trace.type[0] != 1U ||
            trace.length[0] != 7U || trace.record[0][6] != 30U) return 3;

    memset(&trace, 0, sizeof trace);
    init_fleet(&old_fleet);
    init_fleet(&new_fleet);
    new_fleet.plan = 8U;
    new_fleet.flags = 0x0200U;
    new_fleet.field_60 = 7U;
    stars_log_change_fleet(&old_fleet, &new_fleet, 0U, &state, &ops, &trace);
    if (trace.count != 3U || trace.type[0] != 0x2aU ||
            trace.type[1] != 0x0aU || trace.type[2] != 0x2aU) return 4;

    memset(&trace, 0, sizeof trace);
    memset(&state, 0, sizeof state);
    memset(old_orders, 1, sizeof old_orders);
    memset(new_orders, 1, sizeof new_orders);
    init_fleet(&old_fleet);
    init_fleet(&new_fleet);
    old_fleet.orders = old_orders;
    new_fleet.orders = new_orders;
    old_fleet.order_count = 1U;
    new_fleet.order_count = 2U;
    new_orders[18] = 4U;
    stars_log_change_fleet(&old_fleet, &new_fleet, 0U, &state, &ops, &trace);
    if (trace.count != 1U || trace.type[0] != 4U ||
            trace.length[0] != 22U || trace.record[0][2] != 1U) return 5;
    memset(&trace, 0, sizeof trace);
    ops.previous_order_replace = 0;
    old_fleet.order_count = 2U;
    new_fleet.order_count = 1U;
    stars_log_change_fleet(&old_fleet, &new_fleet, 0U, &state, &ops, &trace);
    if (trace.count != 1U || trace.type[0] != 3U) return 6;

    puts("LOG_CHANGE_FLEET_SOURCE=PASS");
    return 0;
}
