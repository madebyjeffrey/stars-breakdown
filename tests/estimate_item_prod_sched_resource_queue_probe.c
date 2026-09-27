#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 planet_raw[56];
    StarsU8 queue_raw[12];
    StarsI16 mined_calls;
    StarsI16 resource_calls;
    StarsI16 research_calls;
    StarsI16 queue_calls;
    StarsI16 cost_calls;
    StarsI16 build_calls;
    StarsI32 available;
    StarsI32 queued_cost[4];
    StarsI32 stored_mineral0;
    StarsI32 seen_mineral0;
} Trace;

static void put_long(StarsU8 *raw, StarsU16 offset, StarsI32 value)
{
    StarsU32 bits = (StarsU32)value;
    raw[offset] = (StarsU8)bits;
    raw[offset + 1U] = (StarsU8)(bits >> 8);
    raw[offset + 2U] = (StarsU8)(bits >> 16);
    raw[offset + 3U] = (StarsU8)(bits >> 24);
}

static void mined(void *context, const StarsU8 *planet, StarsI32 *values,
    StarsI16 player, StarsI16 flag)
{
    Trace *trace = (Trace *)context;
    (void)planet;
    (void)player;
    (void)flag;
    ++trace->mined_calls;
    values[0] = 10L;
    values[1] = 20L;
    values[2] = 30L;
}

static StarsI16 resources_at_planet(void *context, const StarsU8 *planet,
    StarsI16 player)
{
    Trace *trace = (Trace *)context;
    (void)planet;
    (void)player;
    ++trace->resource_calls;
    return 1000;
}

static StarsI16 research_percent(void *context, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    (void)player;
    ++trace->research_calls;
    return 10;
}

static void schedule_mined(void *context, StarsEstimateProdPlanet *planet,
    StarsI32 minerals[3])
{
    (void)context;
    planet->minerals[0] = 77L;
    planet->minerals[1] = 88L;
    planet->minerals[2] = 99L;
    minerals[0] = 77L;
    minerals[1] = 88L;
    minerals[2] = 99L;
}

static const StarsU8 *get_queue(void *context, const StarsU8 *planet,
    StarsU16 *count)
{
    Trace *trace = (Trace *)context;
    (void)planet;
    ++trace->queue_calls;
    *count = 2;
    return trace->queue_raw;
}

static void get_costs(void *context, const StarsU8 *planet,
    const StarsU8 *item, StarsI32 *costs, StarsI16 player,
    StarsI16 only_one)
{
    Trace *trace = (Trace *)context;
    (void)planet;
    (void)item;
    (void)player;
    (void)only_one;
    ++trace->cost_calls;
    costs[0] = 5L;
    costs[1] = 10L;
    costs[2] = 15L;
    costs[3] = 20L;
}

static StarsI32 resources(void *context,
    const StarsEstimateProdPlanet *planet)
{
    Trace *trace = (Trace *)context;
    StarsI32 values[4];
    StarsGetResourcesAvailableOps ops;

    memset(&ops, 0, sizeof ops);
    ops.estimate_minerals = mined;
    ops.resources_at_planet = resources_at_planet;
    ops.research_percent = research_percent;
    trace->seen_mineral0 = planet->minerals[0];
    stars_get_resources_available(trace->planet_raw, values, 2,
        0x00800000UL, &ops, context);
    trace->available = values[3];
    return values[3];
}

static StarsI16 build(void *context, StarsEstimateProdPlanet *planet,
    const StarsEstimateProdItem *item, StarsEstimateProdItem *partial,
    const StarsI32 minerals[3], StarsI16 partial_flag, StarsI16 *phase)
{
    Trace *trace = (Trace *)context;
    StarsI32 costs[4];
    StarsGetProdQCostOps ops;

    (void)planet;
    (void)item;
    (void)partial;
    (void)minerals;
    (void)partial_flag;
    memset(&ops, 0, sizeof ops);
    ops.get_queue = get_queue;
    ops.get_costs = get_costs;
    stars_get_prod_q_cost(trace->planet_raw, costs, 2, &ops, context);
    trace->queued_cost[0] = costs[0];
    trace->queued_cost[1] = costs[1];
    trace->queued_cost[2] = costs[2];
    trace->queued_cost[3] = costs[3];
    ++trace->build_calls;
    *phase = 1;
    return 1;
}

int main(void)
{
    Trace trace;
    StarsEstimateProdPlanet planet;
    StarsEstimateProdItem queue;
    StarsEstimateProdInput input;
    StarsI16 first;
    StarsI16 last;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    memset(&trace, 0, sizeof trace);
    memset(&planet, 0, sizeof planet);
    memset(&queue, 0, sizeof queue);
    memset(&input, 0, sizeof input);
    put_long(trace.planet_raw, 0x1cU, 100L);
    put_long(trace.planet_raw, 0x20U, 200L);
    put_long(trace.planet_raw, 0x24U, 300L);
    trace.stored_mineral0 = 100L;
    trace.queue_raw[0] = 2U;
    trace.queue_raw[2] = 2U;
    trace.queue_raw[3] = 2U;
    planet.minerals[0] = 77L;
    planet.research_percent = 10;
    planet.flags = (StarsU32)(1UL << 23);
    queue.quantity = 1;
    input.planet = &planet;
    input.queue = &queue;
    input.queue_count = 1;
    input.item_index = 0;
    input.first_turn = &first;
    input.last_turn = &last;
    input.estimate_mined = schedule_mined;
    input.resources_at_planet = resources;
    input.build_item = build;
    input.context = &trace;

    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 1 && last == 1);
    CHECK(trace.build_calls == 1 && trace.available == 1000L);
    CHECK(trace.resource_calls == 1 && trace.research_calls == 0);
    CHECK(trace.mined_calls == 1 && trace.seen_mineral0 == 77L);
    CHECK(trace.queue_calls == 1 && trace.cost_calls == 2);
    CHECK(trace.queued_cost[0] == 10L && trace.queued_cost[1] == 20L);
    CHECK(trace.queued_cost[2] == 30L && trace.queued_cost[3] == 40L);
    CHECK(planet.minerals[0] == 77L && planet.population == 0);
#undef CHECK
    printf("ESTIMATE_ITEM_PROD_SCHED_RESOURCE_QUEUE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures != 0;
}
