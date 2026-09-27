#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RawTrace {
    StarsI16 result;
    StarsI16 count;
    StarsI16 recipient[8];
    StarsI16 code[8];
    StarsI16 payload[8][3];
} RawTrace;

static StarsI16 transfer_player(void *opaque, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order)
{
    (void)fleet;
    (void)order;
    return ((RawTrace *)opaque)->result;
}

static void raw_message2(void *opaque, StarsI16 recipient, StarsI16 code,
    StarsI16 payload1, StarsI16 payload2, StarsI16 payload3)
{
    RawTrace *trace;
    StarsI16 index;

    trace = (RawTrace *)opaque;
    index = trace->count;
    if (index >= 8)
        return;
    trace->recipient[index] = recipient;
    trace->code[index] = code;
    trace->payload[index][0] = payload1;
    trace->payload[index][1] = payload2;
    trace->payload[index][2] = payload3;
    trace->count = (StarsI16)(index + 1);
}

static StarsI16 same(StarsI16 actual, StarsI16 expected)
{
    return (StarsI16)(actual == expected);
}

int main(void)
{
    static const StarsI16 results[5] = {
        STARS_SATISFY_PLAYER_TRANSFER_CREATED,
        STARS_SATISFY_PLAYER_TRANSFER_NO_DESIGN,
        STARS_SATISFY_PLAYER_TRANSFER_NO_SHIP,
        STARS_SATISFY_PLAYER_TRANSFER_BLOCKED,
        STARS_SATISFY_PLAYER_TRANSFER_INVALID_TARGET
    };
    static const StarsI16 counts[5] = { 2, 2, 1, 1, 1 };
    static const StarsI16 recipients[5][2] = {
        { 2, 4 }, { 2, 4 }, { 4, 0 }, { 4, 0 }, { 4, 0 }
    };
    static const StarsI16 codes[5][2] = {
        { 0x14d, 0x14e }, { 0x14a, 0x14b }, { 0x149, 0 },
        { 0x14c, 0 }, { 0x148, 0 }
    };
    static const StarsI16 payloads[5][2][3] = {
        { { (StarsI16)0x8017, (StarsI16)0x2a55, 0x34 },
          { (StarsI16)0x8017, 0x32, (StarsI16)0x2b66 } },
        { { (StarsI16)0xaa55, 11, 0x34 },
          { -1, 0x32, 0 } },
        { { (StarsI16)0xaa55, 11, 0 }, { 0, 0, 0 } },
        { { (StarsI16)0xaa55, 0x34, 0 }, { 0, 0, 0 } },
        { { (StarsI16)0xaa55, 11, 0 }, { 0, 0, 0 } }
    };
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput input;
    StarsSatisfyOrdersOps ops;
    RawTrace trace;
    StarsI16 i;
    StarsI16 j;
    StarsI16 k;
    StarsI16 failures;

    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    fleet.id = 11;
    fleet.owner = 2;
    fleet.message_word = 0x2a55U;
    fleet.orders = &order;
    fleet.order_count = 1;
    order.kind = STARS_SATISFY_ORDER_TRANSFER_PLAYER;
    order.target_owner = 4;
    order.message_target_id = 23;
    order.message_target_word = 0x2b66U;
    ops.transfer_player = transfer_player;
    ops.raw_message2 = raw_message2;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.pass = 4;
    input.ops = &ops;
    failures = 0;

    for (i = 0; i < 5; ++i) {
        memset(&trace, 0, sizeof trace);
        order.active = 1;
        trace.result = results[i];
        stars_satisfy_orders(&input);
        if (trace.count != counts[i])
            failures = 1;
        for (j = 0; j < counts[i]; ++j) {
            if (!same(trace.recipient[j], recipients[i][j]) ||
                    !same(trace.code[j], codes[i][j]))
                failures = 1;
            for (k = 0; k < 3; ++k)
                if (!same(trace.payload[j][k], payloads[i][j][k]))
                    failures = 1;
        }
    }

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYORAW.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_TRANSFER_CHECKS=21 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_TRANSFER_CHECKS=21 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
