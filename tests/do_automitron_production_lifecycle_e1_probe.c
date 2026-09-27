#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 events[16];
    StarsI16 count;
    StarsI16 queue_calls;
    StarsI16 fail_init;
    StarsI16 fail_finish;
} Trace;

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

static void event(Trace *trace, StarsI16 value)
{
    if (trace->count < 16)
        trace->events[trace->count] = value;
    ++trace->count;
}

static StarsI16 status(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected)
{
    (void)opaque;
    (void)last;
    (void)budget;
    *selected = first == 4 ? 7 : -1;
    return 0;
}

static StarsI16 visit(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI16 budget)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (budget != 50)
        return 0;
    event(trace, (StarsI16)(10 + planet->id));
    return 1;
}

static StarsI16 init(void *opaque,
    const StarsAutomitronTurnPlanet *planet)
{
    Trace *trace;
    trace = (Trace *)opaque;
    event(trace, (StarsI16)(20 + planet->id));
    return !trace->fail_init;
}

static StarsI16 queue(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI16 design,
    StarsI16 count, StarsI16 kind, StarsI16 flag)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (planet->id != 0 || count != 1 || kind != 2 || flag != 1 ||
            (trace->queue_calls == 0 && design != 7) ||
            (trace->queue_calls == 1 && design != 1))
        return 0;
    ++trace->queue_calls;
    event(trace, 30);
    return 1;
}

static StarsI16 finish(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI16 changed)
{
    Trace *trace;
    trace = (Trace *)opaque;
    event(trace, (StarsI16)(40 + changed + planet->id * 2));
    return !trace->fail_finish;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnPlanet planets[3];
    StarsU8 first_queue[8];
    StarsU8 second_queue[8];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 expected[9];
    StarsI16 index;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(planets, 0, sizeof planets);
    memset(first_queue, 0, sizeof first_queue);
    memset(second_queue, 0, sizeof second_queue);
    memset(&trace, 0, sizeof trace);
    planets[0].id = 0;
    planets[0].owner = 2;
    planets[0].visibility_flags = 0x0200U;
    planets[0].mineral_rate = 1;
    planets[0].raw_prod_queue = first_queue;
    planets[0].raw_prod_queue_size = sizeof first_queue;
    planets[1].id = 1;
    planets[1].owner = 2;
    planets[1].visibility_flags = 0x0200U;
    planets[1].mineral_rate = 1;
    planets[1].raw_prod_queue = second_queue;
    planets[1].raw_prod_queue_size = sizeof second_queue;
    second_queue[3] = 1;
    second_queue[5] = 0x40U;
    second_queue[6] = 0x04U;
    planets[2].id = 2;
    planets[2].owner = -1;
    planets[2].planet_class = 3;
    planets[2].optimal_value = 1;
    ops.check_design_status = status;
    ops.visit_planet = visit;
    ops.init_production = init;
    ops.add_raw_queue_item = queue;
    ops.finish_production = finish;
    ops.context = &trace;
    input.turn = 20;
    input.player_id = 2;
    input.planet_count = 3;
    input.planets = planets;
    input.ops = &ops;
    input.early_queue_inputs_bound = 1;
    input.ai_data_group_count = 3;
    input.owned_planet_count = 10;
    input.tech_level_two = 5;
    expected[0] = 10;
    expected[1] = 20;
    expected[2] = 30;
    expected[3] = 30;
    expected[4] = 41;
    expected[5] = 11;
    expected[6] = 21;
    expected[7] = 42;
    expected[8] = 12;

    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.count == 9 && trace.queue_calls == 2,
        &checks, &failures);
    for (index = 0; index < 9; ++index)
        check(trace.events[index] == expected[index], &checks, &failures);
    check(input.production_queued == 1 &&
        input.tech_support_queued == 1 &&
        input.initial_colonizer_queued == 1, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.fail_init = 1;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.count == 2 && trace.queue_calls == 0 &&
        trace.events[0] == 10 && trace.events[1] == 20,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.fail_finish = 1;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.count == 5 && trace.queue_calls == 2 &&
        trace.events[4] == 41, &checks, &failures);

    printf("AUTOMITRON_LIFECYCLE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
