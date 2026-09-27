#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RawMergeTrace {
    StarsI16 calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[3];
    StarsI16 merge_result;
} RawMergeTrace;

static StarsI16 merge(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    return ((RawMergeTrace *)opaque)->merge_result;
}

static void raw_message2(void *opaque, StarsI16 recipient, StarsI16 code,
    StarsI16 payload1, StarsI16 payload2, StarsI16 payload3)
{
    RawMergeTrace *trace;
    trace = (RawMergeTrace *)opaque;
    ++trace->calls;
    trace->recipient = recipient;
    trace->code = code;
    trace->payload[0] = payload1;
    trace->payload[1] = payload2;
    trace->payload[2] = payload3;
}

static StarsI16 run_case(StarsI16 target_present, StarsI16 target_owner,
    StarsI16 merge_result, StarsI16 expected_code, StarsI16 expected_payload1,
    StarsI16 expected_payload2, StarsI16 expected_payload3)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput input;
    StarsSatisfyOrdersOps ops;
    RawMergeTrace trace;

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    fleet.id = 11;
    fleet.owner = 2;
    fleet.message_word = (StarsU16)0x2a55;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_MERGE;
    order.target_id = 7;
    order.target_owner = target_owner;
    order.target_present = target_present;
    order.active = 1;
    trace.merge_result = merge_result;
    ops.merge = merge;
    ops.raw_message2 = raw_message2;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 2;
    input.ops = &ops;
    stars_satisfy_orders(&input);
    if (trace.calls != 1 || trace.recipient != 2 ||
            trace.code != expected_code)
        return 0;
    if (trace.payload[0] != expected_payload1 ||
            trace.payload[1] != expected_payload2 ||
            trace.payload[2] != expected_payload3)
        return 0;
    if (order.active)
        return 0;
    return 1;
}

int main(void)
{
    StarsI16 failures;

    failures = 0;
    if (!run_case(1, 2, 1, STARS_SATISFY_MESSAGE_MERGE_OK,
            (StarsI16)0x8007, (StarsI16)0x2a55, 7))
        failures = 1;
    if (!run_case(1, 4, 0, STARS_SATISFY_MESSAGE_MERGE_ENEMY,
            (StarsI16)0x800b, 11, 0))
        failures = 1;
    if (!run_case(0, 2, 1, STARS_SATISFY_MESSAGE_MERGE_INVALID,
            (StarsI16)0x800b, 11, 0))
        failures = 1;

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYOMERGE.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_MERGE_CHECKS=12 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_MERGE_CHECKS=12 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
