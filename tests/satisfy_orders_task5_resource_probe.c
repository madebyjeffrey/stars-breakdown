#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    char events[32];
    StarsI16 n;
    StarsI16 reprice_calls;
    StarsI16 resource_calls;
    StarsI16 codes[2];
    StarsI16 payloads[2][8];
    StarsI16 messages;
    StarsI16 learn_result;
    StarsI16 done;
    StarsU8 tech_battle[6];
    StarsU8 tech_trader[12];
} Trace;

static int checks;
static int failures;
#define CHECK(test) do { ++checks; if (!(test)) ++failures; } while (0)

static void put_word(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 word(const StarsU8 *record, StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1U] << 8));
}

static void event(Trace *trace, char letter)
{
    if (trace->n < 31)
        trace->events[trace->n++] = letter;
}

static StarsI16 tech_lookup(void *context, StarsU16 group,
    StarsU16 subtype, StarsU8 tech[6])
{
    Trace *trace;
    trace = (Trace *)context;
    memset(tech, 0, 6U);
    if (group == 0x4000U && subtype == 7U) {
        tech[0] = 3U;
    } else if (group == 0x800U && subtype == 4U) {
        tech[0] = 20U;
    } else {
        return 0;
    }
    CHECK(trace != 0);
    return 1;
}

static void mark_techs(void *context, const StarsU8 *design,
    StarsI16 owner)
{
    Trace *trace;
    StarsMarkTechsInput input;
    trace = (Trace *)context;
    CHECK(owner == 1 && word(design, 0x2aU) == 20U);
    memset(&input, 0, sizeof input);
    memcpy(input.tech_battle, trace->tech_battle, sizeof input.tech_battle);
    memcpy(input.tech_trader, trace->tech_trader, sizeof input.tech_trader);
    input.lookup = tech_lookup;
    input.context = trace;
    CHECK(stars_mark_techs_seen_shdef(design, 147U, &input) == 1);
    memcpy(trace->tech_battle, input.tech_battle, sizeof input.tech_battle);
    memcpy(trace->tech_trader, input.tech_trader, sizeof input.tech_trader);
    event(trace, 'K');
}

static StarsI16 reprice(void *context, const StarsU8 *source,
    StarsU8 *destination)
{
    Trace *trace;
    trace = (Trace *)context;
    CHECK(word(source, 0x2aU) == 20U &&
        word(destination, 0x2aU) == 20U);
    put_word(destination, 0x2aU, 30U);
    put_word(destination, 0x2cU, 14U);
    put_word(destination, 0x2eU, 8U);
    put_word(destination, 0x30U, 8U);
    ++trace->reprice_calls;
    event(trace, 'U');
    return 1;
}

static StarsU16 resources(void *context, StarsI16 owner,
    StarsU16 planet)
{
    Trace *trace;
    trace = (Trace *)context;
    CHECK(owner == 2 && planet == 10U);
    ++trace->resource_calls;
    event(trace, 'C');
    return 100U;
}

static void message(void *context, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    Trace *trace;
    StarsI16 index;
    trace = (Trace *)context;
    index = trace->messages;
    if (index < 2) {
        trace->codes[index] = code;
        memcpy(trace->payloads[index], payload, sizeof trace->payloads[index]);
    }
    CHECK(recipient == (index == 0 ? 1 : 2));
    ++trace->messages;
    event(trace, 'M');
}

static StarsI16 learn(void *context, StarsI16 owner,
    StarsU16 *object)
{
    Trace *trace;
    trace = (Trace *)context;
    CHECK(owner == 2);
    *object = 33U;
    event(trace, 'T');
    return trace->learn_result;
}

static void remove_message(void *context, StarsI16 owner,
    StarsI16 code, StarsU16 object)
{
    Trace *trace;
    trace = (Trace *)context;
    CHECK(owner == 1 && code == 0x4e && object == 0x8007U);
    event(trace, 'R');
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    Trace *trace;
    trace = (Trace *)context;
    CHECK(fleet->id == 7 && order->active == 0 && index == 0);
    ++trace->done;
    event(trace, 'D');
}

int main(void)
{
    StarsU8 fleet[124];
    StarsU8 order[18];
    StarsU8 planet[60];
    StarsU8 design[147];
    StarsU16 extra;
    StarsU16 game_flags;
    StarsSatisfyPackedInput input;
    StarsSatisfyScrapPackedState scrap;
    StarsSatisfyOrdersOps ops;
    Trace trace;

    memset(fleet, 0, sizeof fleet);
    memset(order, 0, sizeof order);
    memset(planet, 0, sizeof planet);
    memset(design, 0, sizeof design);
    memset(&input, 0, sizeof input);
    memset(&scrap, 0, sizeof scrap);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    game_flags = 0U;
    put_word(fleet, 0U, 7U);
    put_word(fleet, 2U, 1U);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 6U, 10U);
    put_word(fleet, 12U, 2U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 6U, 0x1105U);
    put_word(planet, 0U, 10U);
    put_word(planet, 2U, 2U);
    put_word(planet, 4U, 0x0200U);
    put_word(planet, 0x1cU, 1000U);
    put_word(planet, 0x20U, 2000U);
    put_word(planet, 0x24U, 3000U);
    put_word(design, 0x2aU, 20U);
    put_word(design, 0x2cU, 12U);
    put_word(design, 0x2eU, 7U);
    put_word(design, 0x30U, 7U);
    put_word(design, 0U, 7U);
    put_word(design, 0x3aU, 0x800U);
    put_word(design, 0x3cU, 0x0104U);
    design[0x7aU] = 1U;
    extra = 10U;
    scrap.planet_record = planet;
    scrap.planet_size = sizeof planet;
    scrap.design_records[0] = design;
    scrap.design_size = sizeof design;
    scrap.fleet_race_reprice = 1;
    scrap.game_flags = &game_flags;
    scrap.planet_race_scrap_bonus = 1;
    scrap.planet_resource_extra = &extra;
    ops.scrap_mark_techs = mark_techs;
    ops.scrap_reprice = reprice;
    ops.scrap_planet_resources = resources;
    ops.scrap_learn = learn;
    ops.scrap_remove_message = remove_message;
    ops.raw_message = message;
    ops.order_done = done;
    ops.context = &trace;
    input.fleet_record = fleet;
    input.fleet_size = sizeof fleet;
    input.order_record = order;
    input.order_size = sizeof order;
    input.scrap_state = &scrap;
    input.pass = 1;
    input.ops = &ops;

    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(strcmp(trace.events, "KUUUUCMTMRD") == 0);
    CHECK(trace.reprice_calls == 4 && trace.resource_calls == 1 &&
        trace.messages == 2 && trace.done == 1);
    CHECK(game_flags == 0U);
    CHECK(trace.tech_battle[0] == 20U && trace.tech_trader[1] == 4U);
    CHECK(extra == 70U);
    CHECK(word(planet, 0x1cU) == 1025U &&
        word(planet, 0x20U) == 2014U &&
        word(planet, 0x24U) == 3014U);
    CHECK(trace.codes[0] == 0x5d && trace.codes[1] == 0x143);
    CHECK(trace.payloads[0][0] == 10 && trace.payloads[0][1] == 7 &&
        trace.payloads[0][2] == 53 && trace.payloads[0][4] == 10 &&
        trace.payloads[0][5] == 37);
    CHECK(trace.payloads[1][0] == 33 && trace.payloads[1][1] == 7 &&
        trace.payloads[1][2] == 53 && trace.payloads[1][5] == 37 &&
        trace.payloads[1][7] == 0);
    CHECK((word(fleet, 4U) & 0x0400U) != 0U &&
        (word(order, 6U) & 0x1000U) == 0U);

    memset(&trace, 0, sizeof trace);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 6U, 0x1105U);
    trace.learn_result = 2;
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(trace.codes[0] == 0x5d && trace.codes[1] == 0x13f &&
        trace.payloads[1][7] == 1 && trace.payloads[1][5] == 37);

    memset(&trace, 0, sizeof trace);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 6U, 0x1105U);
    trace.learn_result = -2;
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(trace.codes[1] == 0x13e && trace.payloads[1][7] == 1);

    memset(&trace, 0, sizeof trace);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 6U, 0x1105U);
    put_word(planet, 4U, 0U);
    put_word(design, 0x7bU, 0x8000U);
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(trace.codes[0] == 0x5c && trace.codes[1] == 0x142);
    CHECK(trace.payloads[0][2] == 5 && trace.payloads[0][5] == 13 &&
        trace.payloads[1][5] == 13 && extra == 205U);
    CHECK(word(planet, 0x1cU) == 1078U &&
        word(planet, 0x20U) == 2043U &&
        word(planet, 0x24U) == 3043U);

    memset(&trace, 0, sizeof trace);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 6U, 0x1105U);
    scrap.planet_resource_extra = 0;
    CHECK(stars_satisfy_orders_packed(&input) == 0);
    CHECK(trace.n == 0 && word(order, 6U) == 0x1105U &&
        (word(fleet, 4U) & 0x0400U) == 0U);

#ifndef TARGET_NATIVE
    printf("TASK5_RESOURCE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#else
    {
        FILE *report;
        report = fopen("C:\\SOE2B\\SOE2BRES.TXT", "w");
        if (!report)
            return 4;
        fprintf(report, "TASK5_RESOURCE_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    return failures ? 1 : 0;
}
