#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

typedef struct CopyTrace {
    StarsFleetState source;
    StarsFleetState destination;
    StarsFleetOrders source_orders;
    StarsU8 source_records[36];
    StarsU8 destination_records[36];
    StarsI16 callback_count;
    StarsI16 copy_result;
    StarsI16 matched_design;
    StarsI16 missing_design;
    StarsI16 done_count;
    StarsI16 messages;
} CopyTrace;

static const StarsU16 target_kinds[2] = {0x11U, 0x22U};
static const StarsU16 missing_kinds[2] = {0x99U, 0x22U};
static const StarsU16 target_packed[2] = {0x0102U, 0x0304U};
static const StarsU16 candidate_hulls[3] = {7U, 7U, 8U};
static const StarsU16 candidate_flags[3] = {0x8000U, 0x8000U, 0x8000U};
static const StarsU16 candidate_counts[3] = {2U, 2U, 2U};
static const StarsU16 candidate_kinds[6] = {
    0x11U, 0x22U, 0x11U, 0x33U, 0x44U, 0x55U
};
static const StarsU16 candidate_packed[6] = {
    0x0102U, 0x0304U, 0x0102U, 0x0304U, 0x0102U, 0x0304U
};

static StarsI16 transfer_player(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)context; (void)fleet; (void)order;
    return STARS_SATISFY_PLAYER_TRANSFER_CREATED;
}

static StarsI16 copy_fleet_state(void *context, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 result)
{
    CopyTrace *trace = (CopyTrace *)context;
    (void)fleet; (void)order; (void)result;
    ++trace->callback_count;
    trace->matched_design = stars_find_similar_design(7U, target_kinds,
        target_packed, 2U, candidate_hulls, candidate_flags, candidate_counts,
        candidate_kinds, candidate_packed, 3U, 2U);
    trace->missing_design = stars_find_similar_design(7U, missing_kinds,
        target_packed, 2U, candidate_hulls, candidate_flags, candidate_counts,
        candidate_kinds, candidate_packed, 1U, 2U);
    trace->copy_result = stars_duplicate_fleet(&trace->source,
        &trace->destination);
    return trace->copy_result;
}

static void message(void *context, StarsI16 recipient, StarsI16 code,
    StarsI16 target, StarsI16 resource, StarsI32 amount)
{
    CopyTrace *trace = (CopyTrace *)context;
    (void)recipient; (void)code; (void)target; (void)resource;
    (void)amount;
    ++trace->messages;
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    CopyTrace *trace = (CopyTrace *)context;
    (void)fleet; (void)order; (void)index;
    ++trace->done_count;
}

int main(void)
{
    CopyTrace trace;
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersOps ops;
    StarsSatisfyOrdersInput input;
    StarsI16 index;
    StarsI16 failures;

    memset(&trace, 0, sizeof trace);
    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    for (index = 0; index < 124; ++index)
        trace.source.bytes[index] = (StarsU8)(index + 1);
    for (index = 0; index < 36; ++index)
        trace.source_records[index] = (StarsU8)(0xa0 + index);
    trace.source_orders.count = 2;
    trace.source_orders.capacity = 2;
    trace.source_orders.records = trace.source_records;
    trace.source.orders = &trace.source_orders;
    trace.destination.orders = 0;
    fleet.owner = 2;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_TRANSFER_PLAYER;
    order.target_owner = 4;
    order.target_id = 9;
    order.active = 1;
    ops.transfer_player = transfer_player;
    ops.copy_fleet_state = copy_fleet_state;
    ops.send_message = message;
    ops.order_done = done;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 4;
    input.ops = &ops;
    stars_satisfy_orders(&input);

    failures = trace.callback_count != 1 || trace.matched_design != 0 ||
        trace.missing_design != -1 || trace.copy_result != 1 ||
        memcmp(trace.destination.bytes, trace.source.bytes, 124) != 0 ||
        !trace.destination.orders || trace.destination.orders->count != 2 ||
        trace.destination.orders->capacity != 2 ||
        memcmp(trace.destination.orders->records, trace.source_records, 36) != 0 ||
        trace.done_count != 1 || trace.messages != 1 || order.active != 0;
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("SYOE9.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_FLEET_COPY_E2_CHECKS=10 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_FLEET_COPY_E2_CHECKS=10 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    if (trace.destination.orders) {
        free(trace.destination.orders->records);
        free(trace.destination.orders);
    }
    return failures ? 1 : 0;
}
