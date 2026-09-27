#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct MutationTrace {
    StarsU8 queue_raw[16];
    StarsEstimateProdItem queue[3];
    StarsI32 resources[4];
    StarsI32 available;
    StarsI16 capacity;
    StarsI16 fail_build;
    StarsI16 build_calls;
    StarsI16 object_calls;
    StarsI16 message_calls;
    StarsI16 callback_order;
    StarsI16 first_turn;
    StarsI16 last_turn;
    StarsI16 status;
    StarsI16 failure;
} MutationTrace;

static void put_long(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
    raw[offset + 2U] = (StarsU8)(value >> 16);
    raw[offset + 3U] = (StarsU8)(value >> 24);
}

static StarsU32 get_long(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU32)raw[offset] |
        ((StarsU32)raw[offset + 1U] << 8) |
        ((StarsU32)raw[offset + 2U] << 16) |
        ((StarsU32)raw[offset + 3U] << 24);
}

static void decode_queue(MutationTrace *trace, StarsI16 count)
{
    StarsI16 index;
    for (index = 0; index < count; ++index) {
        trace->queue[index].packed = get_long(trace->queue_raw,
            (StarsU16)((index + 1) * 4));
        trace->queue[index].quantity = (StarsU16)(
            trace->queue[index].packed & 0x3ffUL);
        trace->queue[index].progress_kind = (StarsI16)(
            (trace->queue[index].packed >> 17) & 7UL);
        trace->queue[index].max_progress = (StarsI16)(
            (trace->queue[index].packed >> 20) & 0x7fUL);
    }
}

static void encode_item(MutationTrace *trace, StarsI16 index)
{
    StarsU32 value;

    value = trace->queue[index].packed & ~0x3ffUL;
    value |= (StarsU32)trace->queue[index].quantity & 0x3ffUL;
    put_long(trace->queue_raw, (StarsU16)((index + 1) * 4), value);
}

static void mined(void *context, StarsEstimateProdPlanet *planet,
    StarsI32 minerals[3])
{
    (void)context;
    (void)planet;
    minerals[0] = 10L;
    minerals[1] = 20L;
    minerals[2] = 30L;
}

static StarsI32 resources_at_planet(void *context,
    const StarsEstimateProdPlanet *planet)
{
    MutationTrace *trace;
    (void)planet;
    trace = (MutationTrace *)context;
    return trace->available;
}

static StarsI16 costs(void *context, const StarsProducePlanet *planet,
    const StarsProduceItem *item, StarsI32 result[4])
{
    (void)context;
    (void)planet;
    (void)item;
    result[0] = 1L;
    result[1] = 1L;
    result[2] = 1L;
    result[3] = 1L;
    return 1;
}

static StarsI16 capacity(void *context, const StarsProducePlanet *planet,
    StarsI16 type, StarsI16 one)
{
    MutationTrace *trace;
    (void)planet;
    (void)type;
    (void)one;
    trace = (MutationTrace *)context;
    return trace->capacity;
}

static StarsI16 build_object(void *context, StarsProducePlanet *planet,
    StarsI16 object_kind, StarsI16 type, StarsI16 count,
    StarsI32 minerals[3])
{
    MutationTrace *trace;
    (void)planet;
    (void)object_kind;
    (void)type;
    (void)count;
    (void)minerals;
    trace = (MutationTrace *)context;
    ++trace->object_calls;
    if (trace->fail_build)
        return 0;
    return 1;
}

static void send_message(void *context, StarsI16 player, StarsI16 message,
    StarsI16 planet, StarsI16 count, StarsI16 status)
{
    MutationTrace *trace;
    (void)player;
    (void)message;
    (void)planet;
    (void)count;
    (void)status;
    trace = (MutationTrace *)context;
    ++trace->message_calls;
}

static StarsI16 build_item(void *context, StarsEstimateProdPlanet *planet,
    const StarsEstimateProdItem *item, StarsEstimateProdItem *partial,
    const StarsI32 minerals[3], StarsI16 partial_flag, StarsI16 *phase)
{
    MutationTrace *trace;
    StarsProducePlanet produce_planet;
    StarsProduceItem produce_item;
    StarsBuildProductionOps ops;
    StarsBuildProductionInput input;
    StarsI16 index;
    StarsI16 result;

    trace = (MutationTrace *)context;
    (void)planet;
    (void)partial;
    (void)minerals;
    (void)partial_flag;
    memset(&produce_planet, 0, sizeof produce_planet);
    memset(&produce_item, 0, sizeof produce_item);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    produce_planet.id = 7;
    produce_planet.owner = 2;
    produce_item.type = (StarsI16)((item->packed >> 10) & 0x7fUL);
    produce_item.object_kind = (StarsI16)((item->packed >> 17) & 7UL);
    produce_item.quantity = (StarsI16)item->quantity;
    produce_item.cost[0] = produce_item.cost[1] = 1L;
    produce_item.cost[2] = produce_item.cost[3] = 1L;
    ops.costs = costs;
    ops.capacity = capacity;
    ops.build_object = build_object;
    ops.send_message = send_message;
    ops.context = trace;
    input.planet = &produce_planet;
    input.item = &produce_item;
    input.resources[0] = trace->resources[0];
    input.resources[1] = trace->resources[1];
    input.resources[2] = trace->resources[2];
    input.resources[3] = trace->resources[3];
    input.status = &trace->status;
    input.ops = &ops;
    ++trace->build_calls;
    trace->callback_order = (StarsI16)(trace->callback_order * 10 + 1);
    result = stars_build_production_item(&input);
    trace->resources[0] = input.resources[0];
    trace->resources[1] = input.resources[1];
    trace->resources[2] = input.resources[2];
    trace->resources[3] = input.resources[3];
    index = (StarsI16)(item - trace->queue);
    trace->queue[index].quantity = (StarsU16)produce_item.quantity;
    encode_item(trace, index);
    if (produce_item.quantity == 0)
        *phase = 2;
    else
        *phase = 1;
    return result;
}

static int run_case_complete(void)
{
    MutationTrace trace;
    StarsEstimateProdPlanet planet;
    StarsEstimateProdInput input;
    StarsI16 first;
    StarsI16 last;
    StarsU32 packed0;
    StarsU32 packed1;

    memset(&trace, 0, sizeof trace);
    memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input);
    put_long(trace.queue_raw, 0, 2UL);
    packed0 = 1UL | (5UL << 10) | (2UL << 17) | (11UL << 20);
    packed1 = 1UL | (6UL << 10) | (1UL << 17) | (9UL << 20);
    put_long(trace.queue_raw, 4, packed0);
    put_long(trace.queue_raw, 8, packed1);
    decode_queue(&trace, 2);
    trace.resources[0] = trace.resources[1] = 2L;
    trace.resources[2] = trace.resources[3] = 2L;
    trace.available = 8L;
    trace.capacity = 2;
    planet.minerals[0] = 100L;
    planet.minerals[1] = 200L;
    planet.minerals[2] = 300L;
    input.planet = &planet;
    input.queue = trace.queue;
    input.queue_count = 2;
    input.item_index = 1;
    input.first_turn = &first;
    input.last_turn = &last;
    input.estimate_mined = mined;
    input.resources_at_planet = resources_at_planet;
    input.build_item = build_item;
    input.context = &trace;
    stars_estimate_item_prod_schedule(&input);
    return first == 1 && last == 0 && trace.queue[0].quantity == 0U &&
        trace.queue[1].quantity == 0U &&
        get_long(trace.queue_raw, 4) == (packed0 & ~0x3ffUL) &&
        get_long(trace.queue_raw, 8) == (packed1 & ~0x3ffUL) &&
        trace.resources[0] == 0L && trace.resources[1] == 0L &&
        trace.resources[2] == 0L && trace.resources[3] == 0L &&
        trace.object_calls == 2 && trace.message_calls == 2 &&
        planet.minerals[0] == 100L;
}

static int run_case_partial(void)
{
    MutationTrace trace;
    StarsEstimateProdPlanet planet;
    StarsEstimateProdInput input;
    StarsI16 first;
    StarsI16 last;
    StarsU32 packed;

    memset(&trace, 0, sizeof trace);
    memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input);
    packed = 3UL | (7UL << 10) | (2UL << 17) | (12UL << 20);
    put_long(trace.queue_raw, 0, 1UL);
    put_long(trace.queue_raw, 4, packed);
    decode_queue(&trace, 1);
    trace.resources[0] = trace.resources[1] = 1L;
    trace.resources[2] = trace.resources[3] = 1L;
    trace.available = 8L;
    trace.capacity = 4;
    input.planet = &planet;
    input.queue = trace.queue;
    input.queue_count = 1;
    input.item_index = 0;
    input.first_turn = &first;
    input.last_turn = &last;
    input.resources_at_planet = resources_at_planet;
    input.build_item = build_item;
    input.context = &trace;
    stars_estimate_item_prod_schedule(&input);
    return first == 1 && last == 1 && trace.queue[0].quantity == 2U &&
        (get_long(trace.queue_raw, 4) & 0x3ffUL) == 2UL &&
        trace.resources[0] == 0L && trace.resources[3] == 0L &&
        trace.object_calls == 1 && trace.message_calls == 1;
}

static int run_case_failure_rollback(void)
{
    MutationTrace trace;
    StarsEstimateProdPlanet planet;
    StarsEstimateProdInput input;
    StarsI16 first;
    StarsI16 last;
    StarsU32 packed;

    memset(&trace, 0, sizeof trace);
    memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input);
    packed = 2UL | (8UL << 10) | (2UL << 17) | (12UL << 20);
    put_long(trace.queue_raw, 0, 1UL);
    put_long(trace.queue_raw, 4, packed);
    decode_queue(&trace, 1);
    trace.resources[0] = trace.resources[1] = 2L;
    trace.resources[2] = trace.resources[3] = 2L;
    trace.available = 8L;
    trace.capacity = 4;
    trace.fail_build = 1;
    input.planet = &planet;
    input.queue = trace.queue;
    input.queue_count = 1;
    input.item_index = 0;
    input.first_turn = &first;
    input.last_turn = &last;
    input.resources_at_planet = resources_at_planet;
    input.build_item = build_item;
    input.context = &trace;
    stars_estimate_item_prod_schedule(&input);
    return first == 0 && last == 1 && trace.queue[0].quantity == 2U &&
        (get_long(trace.queue_raw, 4) & 0x3ffUL) == 2UL &&
        trace.resources[0] == 2L && trace.resources[3] == 2L &&
        trace.object_calls == 1 && trace.message_calls == 0;
}

int main(void)
{
    int checks;
    int failures;

    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(run_case_complete());
    CHECK(run_case_partial());
    CHECK(run_case_failure_rollback());
#undef CHECK
#undef CHECK
    printf("ESTIMATE_ITEM_PROD_SCHED_MUTATION_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
