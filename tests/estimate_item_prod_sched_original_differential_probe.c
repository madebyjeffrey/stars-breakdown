#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 build_calls;
    StarsI16 mined_calls;
    StarsI16 resource_calls;
    StarsI16 population_calls;
    StarsI16 item_index[8];
    StarsI16 partial_flag[8];
    StarsI16 quantity[8];
    StarsI16 population[8];
    StarsI16 mineral0[8];
    StarsI16 script_mode;
} Trace;

static void mined(void *context, StarsEstimateProdPlanet *planet,
    StarsI32 minerals[3])
{
    Trace *trace = (Trace *)context;
    ++trace->mined_calls;
    minerals[0] = 10L;
    minerals[1] = 20L;
    minerals[2] = 30L;
    planet->minerals[0] = 10L;
    planet->minerals[1] = 20L;
    planet->minerals[2] = 30L;
}

static StarsI32 resources(void *context,
    const StarsEstimateProdPlanet *planet)
{
    Trace *trace = (Trace *)context;
    ++trace->resource_calls;
    (void)planet;
    return 100L;
}

static StarsI16 build(void *context, StarsEstimateProdPlanet *planet,
    const StarsEstimateProdItem *item, StarsEstimateProdItem *partial,
    const StarsI32 minerals[3], StarsI16 partial_flag, StarsI16 *phase)
{
    Trace *trace = (Trace *)context;
    StarsI16 call;

    (void)partial;
    /* The two queue items are adjacent; identify the partial sentinel by
     * its quantity and use the scripted call order for the trace. */
    if (trace->build_calls < 8) {
        trace->item_index[trace->build_calls] =
            item->packed == 0x200001UL ? 0 : 1;
        trace->partial_flag[trace->build_calls] = partial_flag;
        trace->quantity[trace->build_calls] = (StarsI16)item->quantity;
        trace->population[trace->build_calls] = planet->population;
        trace->mineral0[trace->build_calls] = (StarsI16)minerals[0];
    }
    call = trace->build_calls;
    ++trace->build_calls;
    if (trace->script_mode == 1) {
        if (call < 2)
            *phase = 5;
        else
            *phase = item->packed == 0x300001UL ? 0 : 1;
        return item->packed == 0x300001UL ? 1 : 0;
    }
    *phase = 5;
    return 0;
}

static void population(void *context, StarsEstimateProdPlanet *planet,
    StarsI16 amount)
{
    Trace *trace = (Trace *)context;
    ++trace->population_calls;
    planet->population = (StarsI16)(planet->population + amount);
}

static void setup(StarsEstimateProdInput *input,
    StarsEstimateProdPlanet *planet, StarsEstimateProdItem *queue,
    StarsI16 *first, StarsI16 *last, Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(planet, 0, sizeof *planet);
    memset(queue, 0, sizeof *queue * 2U);
    memset(trace, 0, sizeof *trace);
    planet->population = 10;
    queue[0].packed = 0x200001UL;
    queue[0].quantity = 1;
    queue[1].packed = 0x300001UL;
    queue[1].quantity = 1;
    input->planet = planet;
    input->queue = queue;
    input->queue_count = 2;
    input->item_index = 1;
    input->first_turn = first;
    input->last_turn = last;
    input->estimate_mined = mined;
    input->resources_at_planet = resources;
    input->build_item = build;
    input->change_population = population;
    input->context = trace;
}

int main(void)
{
    StarsEstimateProdInput input;
    StarsEstimateProdPlanet planet;
    StarsEstimateProdItem queue[2];
    StarsI16 first;
    StarsI16 last;
    Trace trace;
    int checks;
    int failures;

    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    setup(&input, &planet, queue, &first, &last, &trace);
    trace.script_mode = 1;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 3 && last == 3);
    CHECK(trace.mined_calls == 3 && trace.resource_calls == 3 &&
        trace.population_calls == 2 && trace.build_calls == 4);
    CHECK(trace.item_index[0] == 0 && trace.item_index[1] == 0 &&
        trace.item_index[2] == 0 && trace.item_index[3] == 1);
    CHECK(trace.population[0] == 10 && trace.population[1] == 11 &&
        trace.population[2] == 12 && trace.population[3] == 12);
    CHECK(trace.mineral0[0] == 10 && trace.mineral0[3] == 10 &&
        trace.partial_flag[0] == 0 && trace.partial_flag[3] == 0);

    setup(&input, &planet, queue, &first, &last, &trace);
    queue[0].packed = (3UL << 10) | (1UL << 17) | 1UL;
    input.item_index = 0;
    trace.script_mode = 1;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == -1 && last == -1 && trace.build_calls == 0 &&
        trace.population_calls == 0 && trace.mined_calls == 1);

    setup(&input, &planet, queue, &first, &last, &trace);
    input.queue_count = 1;
    input.item_index = 0;
    trace.script_mode = 2;
    stars_estimate_item_prod_schedule(&input);
    CHECK(first == 100 && last == 100 && trace.build_calls == 100 &&
        trace.mined_calls == 100 && trace.resource_calls == 100 &&
        trace.population_calls == 100);

    CHECK(checks == 8);
    if (failures == 0)
        puts("ESTIMATE_ITEM_PROD_SCHED_ORIGINAL_DIFFERENTIAL_CHECKS=8 FAILURES=0 RESULT=PASS");
    else
        puts("ESTIMATE_ITEM_PROD_SCHED_ORIGINAL_DIFFERENTIAL_CHECKS=8 FAILURES=1 RESULT=FAIL");
    return failures ? 1 : 0;
#undef CHECK
}
