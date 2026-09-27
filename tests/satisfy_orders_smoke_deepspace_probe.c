#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct SmokeScrap {
    StarsSalvageThing thing;
    StarsSalvageThing *current;
    StarsU32 supplied[3];
    StarsI16 salvage_calls;
    StarsI16 message_calls;
    StarsI16 remove_calls;
    StarsI16 done_calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[3];
    StarsU8 raw_thing[18];
} SmokeScrap;

static int checks;
static int failures;
#define CHECK(test) do { ++checks; if (!(test)) ++failures; } while (0)

static void put_word(StarsU8 *bytes, StarsU16 at, StarsU16 value)
{
    bytes[at] = (StarsU8)value;
    bytes[at + 1U] = (StarsU8)(value >> 8);
}

static void put_long(StarsU8 *bytes, StarsU16 at, StarsU32 value)
{
    put_word(bytes, at, (StarsU16)value);
    put_word(bytes, (StarsU16)(at + 2U), (StarsU16)(value >> 16));
}

static StarsU16 word(const StarsU8 *bytes, StarsU16 at)
{
    return (StarsU16)(bytes[at] | ((StarsU16)bytes[at + 1U] << 8));
}

static StarsSalvageThing *new_thing(void *context, StarsI16 owner,
    StarsI16 kind)
{
    SmokeScrap *state;
    state = (SmokeScrap *)context;
    CHECK(owner == 1 && kind == 1);
    memset(&state->thing, 0, sizeof state->thing);
    state->thing.flags = 0x8000U;
    return &state->thing;
}

static StarsI16 salvage(void *context, const StarsU8 *fleet,
    const StarsU32 minerals[3])
{
    SmokeScrap *state;
    StarsDropSalvageInput input;
    state = (SmokeScrap *)context;
    CHECK(word(fleet, 2U) == 1U && word(fleet, 6U) == 0xffffU);
    CHECK(word(fleet, 8U) == 1276U && word(fleet, 10U) == 1380U);
    CHECK(minerals[0] == 5UL && minerals[1] == 0UL && minerals[2] == 2UL);
    memcpy(state->supplied, minerals, sizeof state->supplied);
    memset(&input, 0, sizeof input);
    state->current = 0;
    input.thing = &state->current;
    memcpy(input.minerals, minerals, sizeof input.minerals);
    input.player = 1;
    input.point.x = 1276;
    input.point.y = 1380;
    input.new_thing = new_thing;
    input.context = state;
    stars_drop_salvage(&input);
    ++state->salvage_calls;
    CHECK(state->current == &state->thing);
    return 0x2200;
}

static void unexpected_free(void *context, StarsThingDecayThing *thing)
{
    (void)context;
    (void)thing;
    CHECK(0);
}

static void message2(void *context, StarsI16 recipient, StarsI16 code,
    StarsI16 first, StarsI16 second, StarsI16 third)
{
    SmokeScrap *state;
    state = (SmokeScrap *)context;
    CHECK(state->salvage_calls == 1);
    ++state->message_calls;
    state->recipient = recipient;
    state->code = code;
    state->payload[0] = first;
    state->payload[1] = second;
    state->payload[2] = third;
}

static void remove_message(void *context, StarsI16 owner, StarsI16 code,
    StarsU16 fleet_word)
{
    SmokeScrap *state;
    state = (SmokeScrap *)context;
    CHECK(state->message_calls == 1);
    CHECK(owner == 1 && code == 0x4e && fleet_word == 0x8000U);
    ++state->remove_calls;
}

static void done(void *context, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    SmokeScrap *state;
    state = (SmokeScrap *)context;
    CHECK(state->remove_calls == 1);
    CHECK(fleet->id == 0 && fleet->owner == 1 && index == 0 &&
        order->active == 0);
    ++state->done_calls;
}

int main(void)
{
    SmokeScrap state;
    StarsU8 fleet[124];
    StarsU8 order[18];
    StarsU8 design[147];
    StarsSatisfyScrapPackedState scrap;
    StarsSatisfyPackedInput input;
    StarsSatisfyOrdersOps ops;
    StarsThingDecayThing decay_thing;
    StarsThingDecayInput decay;
    static const StarsU8 original_thing[18] = {
        0x00U, 0x22U, 0xfcU, 0x04U, 0x64U, 0x05U,
        0xffU, 0x83U, 0x05U, 0x00U, 0x00U, 0x00U,
        0x02U, 0x00U, 0x02U, 0x00U, 0x00U, 0x00U
    };
    FILE *report;

    memset(&state, 0, sizeof state);
    memset(fleet, 0, sizeof fleet);
    memset(order, 0, sizeof order);
    memset(design, 0, sizeof design);
    memset(&scrap, 0, sizeof scrap);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    put_word(fleet, 0U, 0U);
    put_word(fleet, 2U, 1U);
    put_word(fleet, 4U, 0x0907U);
    put_word(fleet, 6U, 0xffffU);
    put_word(fleet, 8U, 1276U);
    put_word(fleet, 10U, 1380U);
    put_word(fleet, 12U, 1U);
    put_long(fleet, 108U, 298UL);
    put_word(fleet, 118U, 0x0060U);
    put_word(order, 0U, 1276U);
    put_word(order, 2U, 1380U);
    put_word(order, 4U, 0U);
    put_word(order, 6U, 0x1405U);
    put_word(design, 0U, 4U);
    put_word(design, 0x2aU, 35U);
    put_word(design, 0x2cU, 16U);
    put_word(design, 0x2eU, 2U);
    put_word(design, 0x30U, 8U);
    scrap.design_records[0] = design;
    scrap.design_size = sizeof design;
    ops.scrap_salvage = salvage;
    ops.raw_message2 = message2;
    ops.scrap_remove_message = remove_message;
    ops.order_done = done;
    ops.context = &state;
    input.fleet_record = fleet;
    input.fleet_size = sizeof fleet;
    input.order_record = order;
    input.order_size = sizeof order;
    input.scrap_state = &scrap;
    input.pass = 1;
    input.ops = &ops;
    CHECK(stars_satisfy_orders_packed(&input) == 1);
    CHECK(state.salvage_calls == 1 && state.current == &state.thing);
    CHECK(state.supplied[0] == 5UL && state.supplied[1] == 0UL &&
        state.supplied[2] == 2UL);
    CHECK(state.thing.x == 1276 && state.thing.y == 1380 &&
        state.thing.minerals[0] == 5UL &&
        state.thing.minerals[1] == 0UL &&
        state.thing.minerals[2] == 2UL);
    CHECK(state.thing.flags == 0xc3ffU && state.thing.value == 2U);
    memset(&decay_thing, 0, sizeof decay_thing);
    memset(&decay, 0, sizeof decay);
    decay_thing.kind = 1U;
    decay_thing.packet_flags = state.thing.flags;
    decay_thing.packet_resources[0] = state.thing.minerals[0];
    decay_thing.packet_resources[1] = state.thing.minerals[1];
    decay_thing.packet_resources[2] = state.thing.minerals[2];
    decay_thing.strength = state.thing.value;
    decay_thing.active = 1U;
    decay.things = &decay_thing;
    decay.thing_count = 1U;
    decay.ops.free_thing = unexpected_free;
    stars_thing_decay(&decay);
    state.thing.flags = decay_thing.packet_flags;
    CHECK(state.thing.flags == 0x83ffU && state.thing.value == 2U);
    CHECK(decay_thing.packet_resources[0] == 5L &&
        decay_thing.packet_resources[1] == 0L &&
        decay_thing.packet_resources[2] == 2L);
    put_word(state.raw_thing, 0U, 0x2200U);
    put_word(state.raw_thing, 2U, (StarsU16)state.thing.x);
    put_word(state.raw_thing, 4U, (StarsU16)state.thing.y);
    put_word(state.raw_thing, 6U, state.thing.flags);
    put_word(state.raw_thing, 8U, (StarsU16)state.thing.minerals[0]);
    put_word(state.raw_thing, 10U, (StarsU16)state.thing.minerals[1]);
    put_word(state.raw_thing, 12U, (StarsU16)state.thing.minerals[2]);
    put_word(state.raw_thing, 14U, state.thing.value);
    put_word(state.raw_thing, 16U, 0U);
    CHECK(memcmp(state.raw_thing, original_thing,
        sizeof original_thing) == 0);
    CHECK(state.message_calls == 1 && state.recipient == 1 &&
        state.code == 0x5b && state.payload[0] == -6 &&
        state.payload[1] == 0x2200 && state.payload[2] == 0);
    CHECK(state.remove_calls == 1 && state.done_calls == 1);
    CHECK((word(fleet, 4U) & 0x0400U) != 0U &&
        (word(order, 6U) & 0x1000U) == 0U);
#ifndef TARGET_NATIVE
    report = stdout;
#else
    report = fopen("C:\\SOE2Q\\SOE2QRES.TXT", "w");
    if (!report) return 4;
#endif
    fprintf(report, "SMOKE_SALVAGE_CHECKS=%d FAILURES=%d RESULT=%s "
        "ORE=5,0,2 THING=0022fc046405ff8305000000020002000000 "
        "MSG=91 FLEET=DEAD\n",
        checks, failures, failures ? "FAIL" : "PASS");
#ifdef TARGET_NATIVE
    fclose(report);
#endif
    return failures ? 1 : 0;
}
