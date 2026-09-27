#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RawResourceTrace {
    StarsI16 calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[8];
} RawResourceTrace;

static StarsI16 same(StarsI16 left, StarsI16 right)
{
    return left == right;
}

static StarsI32 transfer(void *opaque, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    (void)opaque;
    (void)fleet;
    (void)order;
    (void)resource;
    return amount;
}

static void raw_message(void *opaque, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    RawResourceTrace *trace;
    StarsI16 i;

    trace = (RawResourceTrace *)opaque;
    ++trace->calls;
    trace->recipient = recipient;
    trace->code = code;
    for (i = 0; i < 8; ++i)
        trace->payload[i] = payload[i];
}

static StarsI16 run_case(StarsI16 target_kind, StarsI16 resource,
    StarsI16 target_id, StarsU16 target_word, StarsI16 target_x,
    StarsI16 expected_code, StarsI16 amount)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput input;
    StarsSatisfyOrdersOps ops;
    RawResourceTrace trace;

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    fleet.id = 11;
    fleet.owner = 2;
    fleet.cargo[resource] = amount;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_TRANSFER;
    order.target_kind = target_kind;
    order.target_id = target_id;
    order.target_owner = fleet.owner;
    order.message_target_word = target_word;
    order.message_target_x = target_x;
    order.requested[resource] = amount;
    order.active = 1;
    ops.transfer = transfer;
    ops.raw_message = raw_message;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 2;
    input.ops = &ops;
    stars_satisfy_orders(&input);
    if (trace.calls != 1 || trace.recipient != 2 ||
            trace.code != expected_code || order.active)
        return 0;
    if (!same(trace.payload[0], (StarsI16)0x800b) ||
            !same(trace.payload[1], 11) ||
            !same(trace.payload[2], amount) ||
            !same(trace.payload[3], (StarsI16)target_word) ||
            !same(trace.payload[4], resource) ||
            !same(trace.payload[5], target_x) ||
            !same(trace.payload[6], target_id) ||
            !same(trace.payload[7], 0))
        return 0;
    return 1;
}

int main(void)
{
    StarsI16 failures;

    failures = 0;
    if (!run_case(STARS_SATISFY_TARGET_PLANET, 0, 7, (StarsU16)0x0123,
            14, STARS_SATISFY_MESSAGE_TRANSFER, 5))
        failures = 1;
    if (!run_case(STARS_SATISFY_TARGET_PLANET,
            STARS_SATISFY_RESOURCE_COLONISTS, 8, (StarsU16)0x0055, 22,
            STARS_SATISFY_MESSAGE_TRANSFER_COLONISTS, 4))
        failures = 1;
    if (!run_case(STARS_SATISFY_TARGET_FLEET,
            STARS_SATISFY_RESOURCE_COLONISTS, 9, (StarsU16)0x0456, -3,
            STARS_SATISFY_MESSAGE_FLEET_TRANSFER_COLONISTS, 7))
        failures = 1;
    if (!run_case(STARS_SATISFY_TARGET_FLEET, 1, 10, (StarsU16)0x0678,
            31, STARS_SATISFY_MESSAGE_FLEET_TRANSFER, 6))
        failures = 1;

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYORES.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_RESOURCE_CHECKS=24 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_RESOURCE_CHECKS=24 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
