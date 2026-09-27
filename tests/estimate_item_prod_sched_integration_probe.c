#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedProduction {
    StarsU8 planet_raw[56];
    StarsU8 prod_raw[12];
    StarsEstimateProdItem typed_queue[2];
    StarsI32 resources[4];
    StarsI16 build_calls;
    StarsI16 object_calls;
    StarsI16 population_calls;
    StarsI16 current_index;
} PackedProduction;

static StarsU16 get_word(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU16)(p[offset] | ((StarsU16)p[offset + 1U] << 8));
}

static void put_word(StarsU8 *p, StarsU16 offset, StarsU16 value)
{
    p[offset] = (StarsU8)value;
    p[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU32 get_long_word(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU32)get_word(p, offset) |
        ((StarsU32)get_word(p, (StarsU16)(offset + 2U)) << 16);
}

static void put_long_word(StarsU8 *p, StarsU16 offset, StarsU32 value)
{
    put_word(p, offset, (StarsU16)value);
    put_word(p, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

static StarsU32 packed_item(const StarsU8 *raw, StarsI16 index)
{
    return get_long_word(raw, (StarsU16)(4U + (StarsU16)index * 4U));
}

static void store_item(PackedProduction *packed, StarsI16 index,
    StarsU32 value)
{
    put_long_word(packed->prod_raw,
        (StarsU16)(4U + (StarsU16)index * 4U), value);
}

static void decode_queue(PackedProduction *packed)
{
    StarsI16 index;
    for (index = 0; index < 2; ++index) {
        packed->typed_queue[index].packed = packed_item(packed->prod_raw,
            index);
        packed->typed_queue[index].quantity = (StarsU16)(
            packed->typed_queue[index].packed & 0x3ffUL);
        packed->typed_queue[index].progress_kind = (StarsI16)(
            (packed->typed_queue[index].packed >> 17) & 7UL);
        packed->typed_queue[index].max_progress = (StarsI16)(
            (packed->typed_queue[index].packed >> 20) & 0x7fUL);
    }
}

static void encode_queue(PackedProduction *packed, StarsI16 index)
{
    StarsU32 value;
    value = packed->typed_queue[index].packed & ~0x3ffUL;
    value |= (StarsU32)packed->typed_queue[index].quantity & 0x3ffUL;
    store_item(packed, index, value);
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
    (void)context;
    (void)planet;
    return 40L;
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
    (void)context;
    (void)planet;
    (void)type;
    (void)one;
    return 1;
}

static StarsI16 build_object(void *context, StarsProducePlanet *planet,
    StarsI16 object_kind, StarsI16 type, StarsI16 count, StarsI32 minerals[3])
{
    PackedProduction *packed = (PackedProduction *)context;
    (void)planet;
    (void)object_kind;
    (void)type;
    (void)count;
    minerals[0] = minerals[1] = minerals[2] = 0L;
    ++packed->object_calls;
    return 1;
}

static void message(void *context, StarsI16 player, StarsI16 message_id,
    StarsI16 planet, StarsI16 count, StarsI16 status)
{
    (void)context;
    (void)player;
    (void)message_id;
    (void)planet;
    (void)count;
    (void)status;
}

static StarsI16 build_item(void *context, StarsEstimateProdPlanet *planet,
    const StarsEstimateProdItem *item, StarsEstimateProdItem *partial,
    const StarsI32 minerals[3], StarsI16 partial_flag, StarsI16 *phase)
{
    PackedProduction *packed = (PackedProduction *)context;
    StarsProducePlanet produce_planet;
    StarsProduceItem produce_item;
    StarsBuildProductionOps ops;
    StarsBuildProductionInput input;
    StarsI16 status;
    StarsI16 index;
    StarsI16 result;

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
    ops.send_message = message;
    ops.context = packed;
    input.planet = &produce_planet;
    input.item = &produce_item;
    input.resources[0] = packed->resources[0];
    input.resources[1] = packed->resources[1];
    input.resources[2] = packed->resources[2];
    input.resources[3] = packed->resources[3];
    input.status = &status;
    input.ops = &ops;
    result = stars_build_production_item(&input);
    packed->resources[0] = input.resources[0];
    packed->resources[1] = input.resources[1];
    packed->resources[2] = input.resources[2];
    packed->resources[3] = input.resources[3];
    index = (StarsI16)(item - packed->typed_queue);
    packed->typed_queue[index].quantity = (StarsU16)produce_item.quantity;
    ++packed->build_calls;
    *phase = produce_item.quantity == 0 ? 2 : 1;
    encode_queue(packed, index);
    return result;
}

static void population(void *context, StarsEstimateProdPlanet *planet,
    StarsI16 amount)
{
    PackedProduction *packed = (PackedProduction *)context;
    (void)planet;
    (void)amount;
    ++packed->population_calls;
}

static int run_case(void)
{
    PackedProduction packed;
    StarsEstimateProdPlanet planet;
    StarsEstimateProdInput input;
    StarsI16 first;
    StarsI16 last;
    StarsU32 original_planet_word;

    memset(&packed, 0, sizeof packed);
    memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input);
    put_word(packed.planet_raw, 0, 7U);
    put_word(packed.planet_raw, 2, 2U);
    put_long_word(packed.planet_raw, 28, 100UL);
    put_long_word(packed.planet_raw, 32, 200UL);
    put_long_word(packed.planet_raw, 36, 300UL);
    packed.prod_raw[0] = 2U;
    packed.prod_raw[2] = 2U;
    packed.prod_raw[3] = 2U;
    store_item(&packed, 0, (StarsU32)(1UL | (5UL << 10) |
        (2UL << 17) | (11UL << 20)));
    store_item(&packed, 1, (StarsU32)(2UL | (6UL << 10) |
        (1UL << 17) | (9UL << 20)));
    packed.resources[0] = packed.resources[1] = 5L;
    packed.resources[2] = packed.resources[3] = 5L;
    original_planet_word = get_long_word(packed.planet_raw, 28);
    decode_queue(&packed);
    planet.minerals[0] = 100L;
    planet.minerals[1] = 200L;
    planet.minerals[2] = 300L;
    planet.research_percent = 25;
    planet.flags = 0L;
    input.planet = &planet;
    input.queue = packed.typed_queue;
    input.queue_count = 2;
    input.item_index = 1;
    input.first_turn = &first;
    input.last_turn = &last;
    input.estimate_mined = mined;
    input.resources_at_planet = resources_at_planet;
    input.build_item = build_item;
    input.change_population = population;
    input.context = &packed;
    stars_estimate_item_prod_schedule(&input);
    if (first != 1 || last != 1 || packed.build_calls != 2 ||
        packed.object_calls != 2 || packed.population_calls != 0 ||
        (packed_item(packed.prod_raw, 0) & 0x3ffUL) != 0UL ||
        (packed_item(packed.prod_raw, 1) & 0x3ffUL) != 1UL ||
        packed.resources[0] != 3L || packed.resources[1] != 3L ||
        packed.resources[2] != 3L || packed.resources[3] != 3L ||
        get_long_word(packed.planet_raw, 28) != original_planet_word)
    {
        return 1;
    }
    return 0;
}

int main(void)
{
    int failures = 0;
    if (run_case() != 0)
        ++failures;
    if (failures == 0)
        puts("ESTIMATE_ITEM_PROD_SCHED_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS");
    else
        puts("ESTIMATE_ITEM_PROD_SCHED_INTEGRATION_CHECKS=2 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}
