#include <stdio.h>
#include <string.h>
#include "common.h"

typedef struct DesignTrace {
    StarsU8 source_records[STARS_TRANSFER_DESIGN_COUNT *
        STARS_TRANSFER_DESIGN_BYTES];
    StarsU8 destination_records[STARS_TRANSFER_DESIGN_COUNT *
        STARS_TRANSFER_DESIGN_BYTES];
    StarsI16 source_counts[STARS_TRANSFER_DESIGN_COUNT];
    StarsI16 destination_counts[STARS_TRANSFER_DESIGN_COUNT];
    StarsI16 source_status[STARS_TRANSFER_DESIGN_COUNT];
    StarsI16 destination_status[STARS_TRANSFER_DESIGN_COUNT];
    StarsI16 mapping[STARS_TRANSFER_DESIGN_COUNT];
    StarsU16 active_design_count;
    StarsI16 materialize_calls;
    StarsI16 materialize_result;
    StarsI16 done_count;
    StarsI16 messages;
} DesignTrace;

static void put_word(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[offset + 1U] = (StarsU8)(value >> 8);
}

static void put_long(StarsU8 *record, StarsU16 offset, StarsU32 value)
{
    record[offset] = (StarsU8)value;
    record[offset + 1U] = (StarsU8)(value >> 8);
    record[offset + 2U] = (StarsU8)(value >> 16);
    record[offset + 3U] = (StarsU8)(value >> 24);
}

static StarsU32 get_long(const StarsU8 *record, StarsU16 offset)
{
    return (StarsU32)record[offset] |
        ((StarsU32)record[offset + 1U] << 8) |
        ((StarsU32)record[offset + 2U] << 16) |
        ((StarsU32)record[offset + 3U] << 24);
}

static void set_design(StarsU8 *records, StarsU16 index, StarsU16 hull,
    StarsU16 flags, StarsU16 kind, StarsU16 packed)
{
    StarsU8 *record = records + (StarsU16)(index *
        STARS_TRANSFER_DESIGN_BYTES);
    put_word(record, 0U, hull);
    put_word(record, 0x7bU, flags);
    record[0x7aU] = 1U;
    put_word(record, 0x3aU, kind);
    put_word(record, 0x3cU, packed);
}

static StarsI16 transfer_player(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)context; (void)fleet; (void)order;
    return STARS_SATISFY_PLAYER_TRANSFER_CREATED;
}

static void materialize(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 result)
{
    DesignTrace *trace = (DesignTrace *)context;
    (void)fleet; (void)order; (void)result;
    ++trace->materialize_calls;
    trace->materialize_result = stars_materialize_player_transfer_designs(
        trace->source_records, trace->source_counts, trace->source_status,
        trace->destination_records, trace->destination_counts,
        trace->destination_status, STARS_TRANSFER_DESIGN_COUNT,
        &trace->active_design_count, trace->mapping);
}

static void message(void *context, StarsI16 recipient, StarsI16 code,
    StarsI16 target, StarsI16 resource, StarsI32 amount)
{
    DesignTrace *trace = (DesignTrace *)context;
    (void)recipient; (void)code; (void)target; (void)resource;
    (void)amount;
    ++trace->messages;
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    DesignTrace *trace = (DesignTrace *)context;
    (void)fleet; (void)order; (void)index;
    ++trace->done_count;
}

int main(void)
{
    DesignTrace trace;
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersOps ops;
    StarsSatisfyOrdersInput input;
    StarsU8 source_before[STARS_TRANSFER_DESIGN_BYTES];
    StarsI16 failures;

    memset(&trace, 0, sizeof trace);
    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    set_design(trace.source_records, 0U, 7U, 0x8000U, 0x1000U,
        0x0103U);
    set_design(trace.source_records, 1U, 8U, 0x8000U, 0x2000U,
        0x0104U);
    set_design(trace.destination_records, 0U, 7U, 0x8000U, 0x1000U,
        0x0103U);
    set_design(trace.destination_records, 1U, 9U, 0x8200U, 0U, 0U);
    set_design(trace.destination_records, 2U, 10U, 0x8200U, 0U, 0U);
    trace.source_counts[0] = 4;
    trace.source_counts[1] = 3;
    trace.source_status[0] = 0x1234;
    trace.source_status[1] = 0x2345;
    trace.destination_counts[0] = 1;
    trace.destination_status[0] = 0x1111;
    put_long(trace.destination_records, 0x7fU, 10U);
    put_long(trace.destination_records, 0x83U, 20U);
    trace.active_design_count = 1U;
    memcpy(source_before, trace.source_records,
        STARS_TRANSFER_DESIGN_BYTES);
    trace.materialize_result = stars_materialize_player_transfer_designs(
        trace.source_records, trace.source_counts, trace.source_status,
        trace.destination_records, trace.destination_counts,
        trace.destination_status, STARS_TRANSFER_DESIGN_COUNT,
        &trace.active_design_count, trace.mapping);

    failures = trace.materialize_result != 1 || trace.mapping[0] != 0 ||
        trace.mapping[1] != 1 || trace.mapping[2] != -1 ||
        trace.destination_counts[0] != 5 || trace.destination_counts[1] != 3 ||
        trace.destination_status[0] != 0x1234 ||
        trace.destination_status[1] != 0x2345 ||
        trace.active_design_count != 2U ||
        memcmp(source_before, trace.source_records,
            STARS_TRANSFER_DESIGN_BYTES) != 0 ||
        trace.destination_records[0x7bU + 1U] != 0x80U ||
        get_long(trace.destination_records, 0x7fU) != 14U ||
        get_long(trace.destination_records, 0x83U) != 24U ||
        trace.destination_records[STARS_TRANSFER_DESIGN_BYTES + 0x7bU] !=
            0x00U ||
        trace.destination_records[STARS_TRANSFER_DESIGN_BYTES + 0x7cU] !=
            0x84U ||
        get_long(trace.destination_records + STARS_TRANSFER_DESIGN_BYTES,
            0x7fU) != 3U ||
        get_long(trace.destination_records + STARS_TRANSFER_DESIGN_BYTES,
            0x83U) != 3U;

    /* Exercise the actual created-player-transfer hook as well. */
    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    fleet.owner = 2;
    fleet.orders = &order;
    fleet.order_count = 1U;
    order.kind = STARS_SATISFY_ORDER_TRANSFER_PLAYER;
    order.target_owner = 4;
    order.target_id = 9;
    order.active = 1;
    ops.transfer_player = transfer_player;
    ops.materialize_designs = materialize;
    ops.send_message = message;
    ops.order_done = done;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1U;
    input.pass = 4;
    input.ops = &ops;
    stars_satisfy_orders(&input);
    failures = failures || trace.materialize_calls != 1 ||
        trace.done_count != 1 || trace.messages != 1 || order.active != 0;

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("SYO10.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_DESIGN_MATERIALIZATION_E2_CHECKS=15 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_DESIGN_MATERIALIZATION_E2_CHECKS=15 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
