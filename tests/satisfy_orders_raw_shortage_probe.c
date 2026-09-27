#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RawShortageTrace {
    StarsI16 calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[3];
} RawShortageTrace;

static StarsI32 transfer(void *opaque, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    (void)opaque;
    (void)fleet;
    (void)order;
    (void)resource;
    (void)amount;
    return 0;
}

static void raw_message2(void *opaque, StarsI16 recipient, StarsI16 code,
    StarsI16 payload1, StarsI16 payload2, StarsI16 payload3)
{
    RawShortageTrace *trace;
    trace = (RawShortageTrace *)opaque;
    ++trace->calls;
    trace->recipient = recipient;
    trace->code = code;
    trace->payload[0] = payload1;
    trace->payload[1] = payload2;
    trace->payload[2] = payload3;
}

static StarsI16 run_case(StarsI16 target_kind, StarsI16 resource,
    StarsI16 expected_code)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput input;
    StarsSatisfyOrdersOps ops;
    RawShortageTrace trace;

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    fleet.id = 11;
    fleet.owner = 2;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_TRANSFER;
    order.target_kind = target_kind;
    order.target_id = 7;
    order.requested[resource] = 5;
    order.active = 1;
    ops.transfer = transfer;
    ops.raw_message2 = raw_message2;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 4;
    input.ops = &ops;
    stars_satisfy_orders(&input);
    if (trace.calls != 1 || trace.recipient != 2)
        return 0;
    if (trace.code != expected_code)
        return 0;
    if (trace.payload[0] != (StarsI16)0x800b ||
            trace.payload[1] != 11 || trace.payload[2] != resource)
        return 0;
    if (!order.active)
        return 0;
    return 1;
}

int main(void)
{
    StarsI16 failures;

    failures = 0;
    if (!run_case(STARS_SATISFY_TARGET_PLANET, 1,
            STARS_SATISFY_MESSAGE_SHORTAGE))
        failures = 1;
    if (!run_case(STARS_SATISFY_TARGET_FLEET, 3,
            STARS_SATISFY_MESSAGE_FLEET_SHORTAGE))
        failures = 1;
    if (!run_case(STARS_SATISFY_TARGET_POINT, 4,
            STARS_SATISFY_MESSAGE_POINT_SHORTAGE))
        failures = 1;

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYOSHORT.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_SHORTAGE_CHECKS=12 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_SHORTAGE_CHECKS=12 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
