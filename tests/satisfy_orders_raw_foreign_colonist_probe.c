#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ForeignTrace {
    StarsI16 raw_calls;
    StarsI16 generic_calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[3];
    StarsI16 transfer_calls;
} ForeignTrace;

static StarsI32 transfer(void *opaque, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    ForeignTrace *trace;
    trace = (ForeignTrace *)opaque;
    (void)fleet;
    (void)order;
    (void)resource;
    ++trace->transfer_calls;
    return amount;
}

static void raw_message2(void *opaque, StarsI16 recipient, StarsI16 code,
    StarsI16 first, StarsI16 second, StarsI16 third)
{
    ForeignTrace *trace;
    trace = (ForeignTrace *)opaque;
    ++trace->raw_calls;
    trace->recipient = recipient;
    trace->code = code;
    trace->payload[0] = first;
    trace->payload[1] = second;
    trace->payload[2] = third;
}

static void raw_message(void *opaque, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    ForeignTrace *trace;
    (void)recipient;
    (void)code;
    (void)payload;
    trace = (ForeignTrace *)opaque;
    ++trace->generic_calls;
}

int main(void)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput input;
    StarsSatisfyOrdersOps ops;
    ForeignTrace trace;
    StarsI16 failures;

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    fleet.id = 11;
    fleet.owner = 2;
    fleet.planet_id = 44;
    fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] = 7;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_TRANSFER;
    order.target_kind = STARS_SATISFY_TARGET_FLEET;
    order.target_id = 19;
    order.target_owner = 3;
    order.requested[STARS_SATISFY_RESOURCE_COLONISTS] = 7;
    order.active = 1;
    ops.transfer = transfer;
    ops.raw_message2 = raw_message2;
    ops.raw_message = raw_message;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 2;
    input.ops = &ops;
    stars_satisfy_orders(&input);

    failures = 0;
    if (trace.raw_calls != 1)
        failures = 1;
    if (trace.recipient != 2 || trace.code !=
            STARS_SATISFY_MESSAGE_FOREIGN_FLEET_COLONISTS)
        failures = 1;
    if (trace.payload[0] != (StarsI16)0x800b || trace.payload[1] != 11 ||
            trace.payload[2] != 44)
        failures = 1;
    if (trace.generic_calls != 0 || trace.transfer_calls != 0)
        failures = 1;
    if (order.active != 0 ||
            order.requested[STARS_SATISFY_RESOURCE_COLONISTS] != 7 ||
            fleet.cargo[STARS_SATISFY_RESOURCE_COLONISTS] != 7)
        failures = 1;

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYOFOR.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_FOREIGN_COLONIST_CHECKS=12 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_FOREIGN_COLONIST_CHECKS=12 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
