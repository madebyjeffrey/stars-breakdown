#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 mined_calls;
    StarsI16 resource_calls;
    StarsI16 build_calls;
    StarsI16 population_calls;
    StarsI16 partial_flags[4];
    StarsU16 quantities[4];
    StarsI32 mineral0_seen;
    StarsI16 population_seen;
    StarsI16 phase;
    StarsI16 result;
} Trace;

static void mined(void *context, StarsEstimateProdPlanet *planet,
    StarsI32 minerals[3])
{
    Trace *trace = (Trace *)context;
    ++trace->mined_calls;
    trace->population_seen = planet->population;
    planet->population = 77;
    minerals[0] = 11L;
    minerals[1] = 22L;
    minerals[2] = 33L;
}

static StarsI32 resources(void *context,
    const StarsEstimateProdPlanet *planet)
{
    Trace *trace = (Trace *)context;
    ++trace->resource_calls;
    trace->mineral0_seen = planet->minerals[0];
    return 40L;
}

static StarsI16 build(void *context, StarsEstimateProdPlanet *planet,
    const StarsEstimateProdItem *item, StarsEstimateProdItem *partial,
    const StarsI32 minerals[3], StarsI16 partial_flag, StarsI16 *phase)
{
    Trace *trace = (Trace *)context;
    StarsI16 index = trace->build_calls;
    (void)planet;
    (void)partial;
    (void)minerals;
    if (index < 4) {
        trace->partial_flags[index] = partial_flag;
        trace->quantities[index] = item->quantity;
    }
    ++trace->build_calls;
    *phase = trace->phase;
    return trace->result;
}

static void population(void *context, StarsEstimateProdPlanet *planet,
    StarsI16 amount)
{
    Trace *trace = (Trace *)context;
    (void)planet;
    (void)amount;
    ++trace->population_calls;
}

static void setup(StarsEstimateProdInput *input,
    StarsEstimateProdPlanet *planet, StarsEstimateProdItem *queue,
    StarsI16 *first, StarsI16 *last, Trace *trace)
{
    memset(planet, 0, sizeof *planet);
    memset(queue, 0, sizeof *queue * 2U);
    memset(input, 0, sizeof *input);
    memset(trace, 0, sizeof *trace);
    planet->minerals[0] = 99L;
    planet->population = 12;
    input->planet = planet;
    input->queue = queue;
    input->queue_count = 1;
    input->item_index = 0;
    input->first_turn = first;
    input->last_turn = last;
    input->estimate_mined = mined;
    input->resources_at_planet = resources;
    input->build_item = build;
    input->change_population = population;
    input->context = trace;
    trace->phase = 1;
    trace->result = 1;
}

static StarsU32 special_word(void)
{
    return (StarsU32)((3UL << 10) | (1UL << 17));
}

int main(void)
{
    StarsEstimateProdPlanet planet;
    StarsEstimateProdItem queue[2];
    StarsEstimateProdInput input;
    StarsI16 first;
    StarsI16 last;
    Trace trace;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    setup(&input, &planet, queue, &first, &last, &trace);
    queue[0].quantity = 2;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 1 && last == 1);
    CHECK(trace.mined_calls == 1 && trace.resource_calls == 1);
    CHECK(trace.build_calls == 1 && trace.population_calls == 0);
    CHECK(trace.mineral0_seen == 11L && trace.population_seen == 12);
    CHECK(planet.minerals[0] == 99L && planet.population == 12);

    setup(&input, &planet, queue, &first, &last, &trace);
    queue[0].quantity = 1;
    trace.phase = 2;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 1 && last == 0);
    CHECK(trace.build_calls == 1 && trace.population_calls == 0);

    setup(&input, &planet, queue, &first, &last, &trace);
    input.item_index = 2;
    queue[0].quantity = 1;
    trace.phase = 5;
    trace.result = 0;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 100 && last == 100);
    CHECK(trace.mined_calls == 100 && trace.resource_calls == 100);
    CHECK(trace.build_calls == 100 && trace.population_calls == 100);

    setup(&input, &planet, queue, &first, &last, &trace);
    queue[0].quantity = 1;
    queue[0].packed = special_word();
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 1 && last == 1 && queue[0].quantity == 0x3fcU);
    CHECK(trace.build_calls == 1 && trace.quantities[0] == 0x3fcU);

    setup(&input, &planet, queue, &first, &last, &trace);
    input.queue_count = 2;
    input.item_index = 0;
    queue[0].quantity = 1;
    queue[0].packed = special_word();
    queue[1].quantity = 1;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == -1 && last == -1 && trace.build_calls == 0);
    CHECK(trace.population_calls == 0 && queue[0].quantity == 1);

    setup(&input, &planet, queue, &first, &last, &trace);
    input.queue_count = 2;
    input.item_index = 1;
    queue[0].quantity = 1;
    queue[0].packed = special_word();
    queue[1].quantity = 1;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 1 && last == 1 && trace.build_calls == 2);
    CHECK(trace.partial_flags[0] == 1 && trace.partial_flags[1] == 0);

    setup(&input, &planet, queue, &first, &last, &trace);
    input.item_index = -1;
    input.partial_item.quantity = 1;
    input.partial_item.packed = special_word();
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == -1 && last == -1 && trace.build_calls == 0);

    setup(&input, &planet, queue, &first, &last, &trace);
    input.item_index = -2;
    queue[0].quantity = 1;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 40 && last == 0);
    CHECK(trace.build_calls == 1 && trace.population_calls == 0);

    setup(&input, &planet, queue, &first, &last, &trace);
    input.item_index = -1;
    planet.research_percent = 25;
    queue[0].quantity = 1;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 40 && last == 0);
    planet.flags = (StarsU32)(1UL << 23);
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 40 && last == 0 && trace.population_calls == 0);

    printf("ESTIMATE_ITEM_PROD_SCHED_DIFFERENTIAL_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures != 0;
}
