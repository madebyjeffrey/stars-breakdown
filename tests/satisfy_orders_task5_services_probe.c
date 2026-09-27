#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    char events[16];
    StarsI16 event_count;
    StarsI16 codes[2];
    StarsI16 objects[2];
    StarsI16 tech_arg;
    StarsI16 tech_calls;
    StarsI16 payload[2][8];
    StarsI16 salvage_id;
    StarsU32 salvage[3];
    StarsI16 removed_owner;
    StarsI16 removed_code;
    StarsU16 removed_object;
    StarsU8 *planet;
    StarsU8 *fleet;
    StarsU16 colonists_at_remove;
    StarsU16 dead_at_remove;
    StarsI16 done;
} Trace;

static int checks;
static int failures;
#define CHECK(test) do { ++checks; if (!(test)) ++failures; } while (0)

static void put_word(StarsU8 *bytes, StarsU16 at, StarsU16 value)
{
    bytes[at] = (StarsU8)value;
    bytes[at + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 word(const StarsU8 *bytes, StarsU16 at)
{
    return (StarsU16)(bytes[at] | ((StarsU16)bytes[at + 1U] << 8));
}

static void event(Trace *trace, char value)
{
    if (trace->event_count < 15)
        trace->events[trace->event_count++] = value;
}

static void message(void *context, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    Trace *trace;
    StarsI16 index;
    trace = (Trace *)context;
    index = trace->codes[0] == 0 ? 0 : 1;
    trace->codes[index] = code;
    trace->objects[index] = recipient;
    memcpy(trace->payload[index], payload, sizeof trace->payload[index]);
    event(trace, 'M');
}

static void message2(void *context, StarsI16 recipient, StarsI16 code,
    StarsI16 object, StarsI16 arg0, StarsI16 arg1)
{
    Trace *trace;
    trace = (Trace *)context;
    trace->codes[0] = code;
    trace->objects[0] = recipient;
    trace->payload[0][0] = object;
    trace->payload[0][1] = arg0;
    trace->payload[0][2] = arg1;
    event(trace, 'm');
}

static StarsI16 salvage(void *context, const StarsU8 *fleet,
    const StarsU32 minerals[3])
{
    Trace *trace;
    trace = (Trace *)context;
    CHECK(word(fleet, 6U) == 0xffffU);
    memcpy(trace->salvage, minerals, sizeof trace->salvage);
    event(trace, 'S');
    return trace->salvage_id;
}

static StarsI16 learn(void *context, StarsI16 owner,
    StarsU16 *message_object)
{
    Trace *trace;
    trace = (Trace *)context;
    CHECK(owner == 2);
    ++trace->tech_calls;
    *message_object = 33U;
    event(trace, 'T');
    return trace->tech_arg;
}

static void remove_message(void *context, StarsI16 owner, StarsI16 code,
    StarsU16 object)
{
    Trace *trace;
    trace = (Trace *)context;
    trace->removed_owner = owner;
    trace->removed_code = code;
    trace->removed_object = object;
    if (trace->planet)
        trace->colonists_at_remove = word(trace->planet, 0x28U);
    if (trace->fleet)
        trace->dead_at_remove = (StarsU16)(word(trace->fleet, 4U) & 0x0400U);
    event(trace, 'R');
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    Trace *trace;
    trace = (Trace *)context;
    CHECK(fleet->id == 7 && fleet->owner == 1 && index == 0);
    CHECK(order->active == 0);
    ++trace->done;
    event(trace, 'D');
}

int main(void)
{
    StarsU8 fleet[124];
    StarsU8 order[18];
    StarsU8 planet[60];
    StarsU8 design[147];
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
    put_word(fleet, 0U, 7U);
    put_word(fleet, 2U, 1U);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 6U, 10U);
    put_word(fleet, 12U, 1U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 6U, 0x1105U);
    put_word(planet, 0U, 10U);
    put_word(planet, 2U, 2U);
    put_word(planet, 4U, 0x0200U);
    put_word(design, 0x2aU, 20U);
    put_word(design, 0x2cU, 12U);
    put_word(design, 0x2eU, 7U);
    put_word(design, 0x30U, 7U);
    scrap.planet_record = planet;
    scrap.planet_size = sizeof planet;
    scrap.design_records[0] = design;
    scrap.design_size = sizeof design;
    ops.raw_message = message;
    ops.raw_message2 = message2;
    ops.scrap_salvage = salvage;
    ops.scrap_learn = learn;
    ops.scrap_remove_message = remove_message;
    ops.order_done = done;
    ops.context = &trace;
    input.fleet_record = fleet;
    input.fleet_size = sizeof fleet;
    input.order_record = order;
    input.order_size = sizeof order;
    input.scrap_state = &scrap;
    input.pass = 1;
    input.ops = &ops;

    trace.tech_arg = 3;
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(strcmp(trace.events, "MTMRD") == 0);
    CHECK(trace.codes[0] == 0x5a && trace.codes[1] == 0x13d);
    CHECK(trace.objects[0] == 1 && trace.objects[1] == 2);
    CHECK(trace.payload[1][0] == 33 && trace.payload[1][5] == 2);
    CHECK(trace.removed_owner == 1 && trace.removed_code == 0x4e &&
        trace.removed_object == 0x8007U);
    CHECK(trace.done == 1 && (word(fleet, 4U) & 0x0400U) != 0U &&
        (word(order, 6U) & 0x1000U) == 0U);

    memset(&trace, 0, sizeof trace);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 6U, 0x1105U);
    trace.tech_arg = -2;
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(trace.codes[0] == 0x5a && trace.codes[1] == 0x13c &&
        trace.payload[1][5] == 1 && strcmp(trace.events, "MTMRD") == 0);

    /* Original service order: remove the old message while the fleet is
     * alive and planet colonists are unchanged, then mark dead and transfer. */
    memset(&trace, 0, sizeof trace);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 0x58U, 123U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 6U, 0x1105U);
    put_word(planet, 2U, 1U);
    put_word(planet, 4U, 0U);
    put_word(planet, 0x28U, 500U);
    trace.planet = planet;
    trace.fleet = fleet;
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(strcmp(trace.events, "MMRD") == 0 &&
        trace.colonists_at_remove == 500U && trace.dead_at_remove == 0U);
    CHECK(word(planet, 0x28U) == 623U &&
        (word(fleet, 4U) & 0x0400U) != 0U);

    memset(&trace, 0, sizeof trace);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 6U, 0xffffU);
    put_word(fleet, 118U, 0x0060U);
    put_word(fleet, 76U, 5U);
    put_word(fleet, 80U, 6U);
    put_word(fleet, 84U, 7U);
    put_word(order, 6U, 0x1105U);
    scrap.planet_record = 0;
    trace.salvage_id = 77;
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(strcmp(trace.events, "SmRD") == 0);
    CHECK(trace.salvage[0] == 9U && trace.salvage[1] == 8U &&
        trace.salvage[2] == 9U);
    CHECK(trace.codes[0] == 0x5b && trace.objects[0] == 1 &&
        trace.payload[0][0] == -6 && trace.payload[0][1] == 77 &&
        trace.payload[0][2] == 7);
    CHECK(trace.tech_calls == 0 && trace.done == 1);

    memset(&trace, 0, sizeof trace);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 6U, 0x1105U);
    ops.scrap_salvage = 0;
    CHECK(stars_satisfy_orders_packed(&input) == 0);
    CHECK(trace.event_count == 0 && word(order, 6U) == 0x1105U &&
        (word(fleet, 4U) & 0x0400U) == 0U);

#ifndef TARGET_NATIVE
    printf("TASK5_SERVICES_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#else
    {
        FILE *report;
        report = fopen("C:\\SOE2A\\SOE2RES.TXT", "w");
        if (!report)
            return 4;
        fprintf(report, "TASK5_SERVICES_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    return failures ? 1 : 0;
}
