#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RawMineTrace {
    StarsI16 result;
    StarsI16 calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[3];
} RawMineTrace;

static StarsI16 mine(void *opaque, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    return ((RawMineTrace *)opaque)->result;
}

static void raw_message2(void *opaque, StarsI16 recipient, StarsI16 code,
    StarsI16 payload1, StarsI16 payload2, StarsI16 payload3)
{
    RawMineTrace *trace;
    trace = (RawMineTrace *)opaque;
    ++trace->calls;
    trace->recipient = recipient;
    trace->code = code;
    trace->payload[0] = payload1;
    trace->payload[1] = payload2;
    trace->payload[2] = payload3;
}

int main(void)
{
    static const StarsI16 results[3] = {
        STARS_SATISFY_MINE_NO_PLANET,
        STARS_SATISFY_MINE_NO_SOURCE,
        STARS_SATISFY_MINE_HOSTILE_PLANET
    };
    static const StarsI16 codes[3] = { 0x77, 0x75, 0x76 };
    static const StarsI16 last_payloads[3] = { 0, 7, 7 };
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput input;
    StarsSatisfyOrdersOps ops;
    RawMineTrace trace;
    StarsI16 failures;
    StarsI16 i;

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    fleet.id = 11;
    fleet.owner = 2;
    fleet.flags = STARS_SATISFY_FLEET_CAN_MINE;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_MINE;
    order.target_id = 7;
    ops.mine = mine;
    ops.raw_message2 = raw_message2;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 3;
    input.ops = &ops;
    failures = 0;

    for (i = 0; i < 3; ++i) {
        memset(&trace, 0, sizeof trace);
        trace.result = results[i];
        order.active = 1;
        stars_satisfy_orders(&input);
        if (trace.calls != 1 || trace.recipient != 2 ||
                trace.code != codes[i] ||
                trace.payload[0] != (StarsI16)0x800b ||
                trace.payload[1] != 11 ||
                trace.payload[2] != last_payloads[i] || order.active)
            failures = 1;
    }

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYOMINE.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_MINE_CHECKS=12 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_MINE_CHECKS=12 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
