#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RawRouteTrace {
    StarsI16 route_result;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[8];
    StarsI16 calls;
} RawRouteTrace;

static StarsI16 route(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    return ((RawRouteTrace *)opaque)->route_result;
}

static void raw_message(void *opaque, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    RawRouteTrace *trace;
    StarsI16 i;

    trace = (RawRouteTrace *)opaque;
    trace->recipient = recipient;
    trace->code = code;
    for (i = 0; i < 8; ++i)
        trace->payload[i] = payload[i];
    ++trace->calls;
}

int main(void)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput input;
    StarsSatisfyOrdersOps ops;
    RawRouteTrace trace;
    StarsI16 failures;
    StarsI16 variant;
    StarsI16 i;
    static const StarsI16 expected_codes[2] = { 0x128, 0x127 };

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    fleet.id = 11;
    fleet.owner = 2;
    fleet.planet_id = 12;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_AUTO_ROUTE;
    order.source_cord = 1;
    order.target_id = 9;
    order.target_owner = fleet.owner;
    order.target_starbase_flags = 0x123;
    ops.auto_route_at_starbase = route;
    ops.raw_message = raw_message;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 3;
    input.ops = &ops;
    failures = 0;

    for (variant = 0; variant < 2; ++variant) {
        memset(&trace, 0, sizeof trace);
        trace.route_result = 1;
        order.active = 1;
        order.auto_route_message_variant = variant;
        stars_satisfy_orders(&input);
        if (trace.calls != 1 || trace.recipient != 2 ||
                trace.code != expected_codes[variant] ||
                trace.payload[0] != (StarsI16)0x800b ||
                trace.payload[1] != 11 || trace.payload[2] != 12 ||
                trace.payload[3] != 0x122)
            failures = 1;
        for (i = 4; i < 8; ++i)
            if (trace.payload[i] != 0)
                failures = 1;
    }

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYOROUTE.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_ROUTE_CHECKS=20 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_ROUTE_CHECKS=20 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
