#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 count;
    StarsI16 recipient[2];
    StarsI16 code[2];
    StarsI16 payload[2][8];
    StarsI16 done;
} Trace;

static int checks;
static int failures;
#define CHECK(test) do { ++checks; if (!(test)) ++failures; } while (0)

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 word(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU16)(bytes[offset] | ((StarsU16)bytes[offset + 1U] << 8));
}

static StarsU32 long_value(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU32)word(bytes, offset) |
        ((StarsU32)word(bytes, (StarsU16)(offset + 2U)) << 16);
}

static void message(void *opaque, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    Trace *trace;
    StarsI16 index;
    trace = (Trace *)opaque;
    index = trace->count;
    if (index >= 2)
        return;
    trace->recipient[index] = recipient;
    trace->code[index] = code;
    memcpy(trace->payload[index], payload, sizeof trace->payload[index]);
    ++trace->count;
}

static void done(void *opaque, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    Trace *trace;
    trace = (Trace *)opaque;
    CHECK(fleet->id == 0 && fleet->owner == 0 &&
        (fleet->flags & 0x0400U) != 0U);
    CHECK(order->kind == 5 && order->active == 0 && index == 0);
    ++trace->done;
}

int main(void)
{
    static const StarsU8 original_order[18] = {
        0x68, 0x04, 0x14, 0x05, 0x0a, 0x00, 0x05, 0x11,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    StarsU8 fleet[124];
    StarsU8 order[18];
    StarsU8 planet[60];
    StarsU8 design[147];
    StarsSatisfyPackedInput input;
    StarsSatisfyScrapPackedState scrap;
    StarsSatisfyOrdersOps ops;
    Trace trace;

    memset(fleet, 0, sizeof fleet);
    memcpy(order, original_order, sizeof order);
    memset(planet, 0, sizeof planet);
    memset(design, 0, sizeof design);
    memset(&input, 0, sizeof input);
    memset(&scrap, 0, sizeof scrap);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    put_word(fleet, 4, 0x0907U);
    put_word(fleet, 6, 10U);
    put_word(fleet, 12, 1U);
    put_word(fleet, 0x58U, 123U);
    put_word(fleet, 118, 0x0060U);
    put_word(planet, 0, 10U);
    put_word(planet, 2, 0U);
    put_word(planet, 4, 0x0200U);
    /* Clean original turn-1 planet-10 levels. The isolated source branch
     * returns 9/6/5; the full original action output differs by 9/5/5,
     * so a separate turn service changes one boranium. */
    put_word(planet, 0x1cU, 476U);
    put_word(planet, 0x20U, 902U);
    put_word(planet, 0x24U, 383U);
    put_word(planet, 0x28U, 500U);
    put_word(design, 0x2aU, 24U);
    put_word(design, 0x2cU, 12U);
    put_word(design, 0x2eU, 8U);
    put_word(design, 0x30U, 7U);
    scrap.planet_record = planet;
    scrap.planet_size = sizeof planet;
    scrap.design_records[0] = design;
    scrap.design_size = sizeof design;
    input.fleet_record = fleet;
    input.fleet_size = sizeof fleet;
    input.order_record = order;
    input.order_size = sizeof order;
    input.scrap_state = &scrap;
    input.pass = 1;
    input.ops = &ops;
    ops.raw_message = message;
    ops.order_done = done;
    ops.context = &trace;

    put_word(fleet, 118, 0x00e0U);
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(long_value(planet, 0x1cU) == 476U && trace.count == 0 &&
        word(fleet, 118) == 0x0080U);
    put_word(fleet, 118, 0x0060U);
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(long_value(planet, 0x1cU) == 485U &&
        long_value(planet, 0x20U) == 908U &&
        long_value(planet, 0x24U) == 388U &&
        long_value(planet, 0x28U) == 623U);
    CHECK((word(fleet, 4) & 0x0400U) != 0U &&
        (word(order, 6) & 0x1000U) == 0U &&
        word(fleet, 118) == 0U);
    CHECK(trace.count == 2 && trace.done == 1);
    CHECK(trace.code[0] == 0x5a && trace.recipient[0] == 0 &&
        trace.payload[0][0] == 10 && trace.payload[0][1] == 0 &&
        trace.payload[0][2] == 20 && trace.payload[0][3] == 0 &&
        trace.payload[0][4] == 10);
    CHECK(trace.code[1] == 0x141 && trace.recipient[1] == 0 &&
        trace.payload[1][0] == 10 && trace.payload[1][1] == 0 &&
        trace.payload[1][2] == 20 && trace.payload[1][4] == 10);
    {
        static const StarsU8 original_prefix[16] = {
            0x5a, 0x00, 0x0a, 0x00, 0x00, 0x14, 0x00, 0x0a,
            0x41, 0x01, 0x0a, 0x00, 0x00, 0x14, 0x00, 0x0a
        };
        StarsU8 packed[16];
        StarsI16 message_index;
        StarsI16 parameter;
        memset(packed, 0, sizeof packed);
        for (message_index = 0; message_index < 2; ++message_index) {
            StarsU16 base;
            base = (StarsU16)(message_index * 8);
            put_word(packed, base, (StarsU16)trace.code[message_index]);
            put_word(packed, (StarsU16)(base + 2U),
                (StarsU16)trace.payload[message_index][0]);
            for (parameter = 0; parameter < 4; ++parameter)
                packed[base + 4U + (StarsU16)parameter] =
                    (StarsU8)trace.payload[message_index][parameter + 1];
        }
        CHECK(memcmp(packed, original_prefix, sizeof packed) == 0);
    }
    CHECK(stars_satisfy_orders_packed(&input) == 1 && trace.count == 2 &&
        trace.done == 1 && long_value(planet, 0x1cU) == 485U);

    /* The pass-1 branch must preflight every occupied design. */
    memcpy(order, original_order, sizeof order);
    put_word(fleet, 4, 0x0907U);
    scrap.design_records[0] = 0;
    memset(&trace, 0, sizeof trace);
    CHECK(stars_satisfy_orders_packed(&input) == 0);
    CHECK(word(fleet, 4) == 0x0907U && word(order, 6) == 0x1105U &&
        long_value(planet, 0x1cU) == 485U && trace.count == 0);

    /* Without a starbase, ordinary scrap returns one third of ship ore. */
    scrap.design_records[0] = design;
    put_word(planet, 4, 0U);
    put_word(fleet, 0x4cU, 2U);
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(long_value(planet, 0x1cU) == 491U &&
        long_value(planet, 0x20U) == 910U &&
        long_value(planet, 0x24U) == 390U);
    CHECK(trace.code[0] == 0x59 && trace.code[1] == 0x140);

#ifndef TARGET_NATIVE
    printf("TASK5_ACCOUNTING_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#else
    {
        FILE *report;
        report = fopen("C:\\SOE1A\\SOE1RES.TXT", "w");
        if (!report)
            return 4;
        fprintf(report, "TASK5_ACCOUNTING_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    return failures ? 1 : 0;
}
