#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RawTechnologyTrace {
    StarsI16 calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[8];
    StarsI16 target_kind;
} RawTechnologyTrace;

static StarsI32 transfer(void *opaque, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 resource, StarsI32 amount)
{
    (void)opaque;
    (void)fleet;
    (void)order;
    (void)resource;
    return amount;
}

static StarsI16 bind_technology(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI32 amount, StarsI16 negative_code,
    StarsI16 positive_code, StarsSatisfyTechnologyOutcome *outcome)
{
    RawTechnologyTrace *trace;
    StarsI16 i;

    trace = (RawTechnologyTrace *)opaque;
    (void)fleet;
    outcome->raw_recipient = 4;
    outcome->amount = amount;
    if (order->target_kind == STARS_SATISFY_TARGET_POINT) {
        outcome->result = -3;
        outcome->message_code = negative_code;
        outcome->result_id = 2;
    } else {
        outcome->result = 2;
        outcome->message_code = positive_code;
        outcome->result_id = 1;
    }
    trace->target_kind = order->target_kind;
    for (i = 0; i < 8; ++i)
        outcome->raw_payload[i] = 0;
    outcome->raw_payload[0] = order->target_id;
    outcome->raw_payload[1] = 11;
    outcome->raw_payload[2] = (StarsI16)amount;
    outcome->raw_payload[3] = order->target_kind;
    outcome->raw_payload[4] = outcome->result_id;
    return outcome->result;
}

static void raw_message(void *opaque, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    RawTechnologyTrace *trace;
    StarsI16 i;

    trace = (RawTechnologyTrace *)opaque;
    ++trace->calls;
    trace->recipient = recipient;
    trace->code = code;
    for (i = 0; i < 8; ++i)
        trace->payload[i] = payload[i];
}

static StarsI16 run_case(StarsI16 target_kind, StarsI16 expected_result,
    StarsI16 expected_code, StarsI16 expected_id)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput input;
    StarsSatisfyOrdersOps ops;
    RawTechnologyTrace trace;
    StarsI16 i;

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    fleet.id = 11;
    fleet.owner = 3;
    fleet.cargo[0] = 7;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_TRANSFER;
    order.target_kind = target_kind;
    order.target_id = 9;
    order.requested[0] = 7;
    order.technology_flag = 1;
    order.active = 1;
    ops.transfer = transfer;
    ops.technology_binding = bind_technology;
    ops.raw_message = raw_message;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 2;
    input.ops = &ops;
    stars_satisfy_orders(&input);
    if (trace.calls != 1 || trace.recipient != 4 ||
            trace.code != expected_code || order.active)
        return 0;
    if (trace.payload[0] != 9 || trace.payload[1] != 11 ||
            trace.payload[2] != 7 || trace.payload[3] != target_kind ||
            trace.payload[4] != expected_id)
        return 0;
    for (i = 5; i < 8; ++i)
        if (trace.payload[i] != 0)
            return 0;
    if (expected_result == 0)
        return 0;
    return 1;
}

int main(void)
{
    StarsI16 failures;

    failures = 0;
    if (!run_case(STARS_SATISFY_TARGET_POINT, -3, 0x013c, 2))
        failures = 1;
    if (!run_case(STARS_SATISFY_TARGET_PLANET, 2, 0x013f, 1))
        failures = 1;

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYOTECH.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_TECHNOLOGY_CHECKS=16 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_TECHNOLOGY_CHECKS=16 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
