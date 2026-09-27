#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 statuses;
    StarsI16 support_status;
    StarsI16 calls;
    StarsI16 designs[3];
    StarsI16 planets[3];
    StarsI16 random_calls;
    StarsI16 random_result;
    StarsI16 bad_tuple;
    StarsI16 fail_call;
} Trace;

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

static StarsI16 design_status(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected)
{
    Trace *trace;
    trace = (Trace *)opaque;
    (void)last;
    (void)budget;
    ++trace->statuses;
    *selected = first == 4 ? 7 : -1;
    return first == 4 ? trace->support_status : 0;
}

static StarsI16 random_value(void *opaque, StarsI16 maximum)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->random_calls;
    if (maximum != 4)
        trace->bad_tuple = 1;
    return trace->random_result;
}

static StarsI16 queue(void *opaque, const StarsAutomitronTurnPlanet *planet,
    StarsI16 design, StarsI16 count, StarsI16 kind, StarsI16 flag)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->calls < 3) {
        trace->designs[trace->calls] = design;
        trace->planets[trace->calls] = planet->id;
    }
    ++trace->calls;
    if (count != 1 || kind != 2 || flag != 1)
        trace->bad_tuple = 1;
    return trace->calls != trace->fail_call;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnPlanet planets[2];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(planets, 0, sizeof planets);
    memset(&trace, 0, sizeof trace);
    planets[0].id = 4;
    planets[0].owner = 2;
    planets[0].visibility_flags = 0x0200U;
    planets[0].mineral_rate = 1;
    planets[0].support_queue_eligible = 1;
    planets[1].id = 5;
    planets[1].owner = -1;
    planets[1].planet_class = 3;
    planets[1].optimal_value = 1;
    ops.check_design_status = design_status;
    ops.random = random_value;
    ops.add_raw_queue_item = queue;
    ops.context = &trace;
    input.turn = 20;
    input.player_id = 2;
    input.planet_count = 2;
    input.planets = planets;
    input.ops = &ops;
    input.early_queue_inputs_bound = 1;
    input.ai_data_group_count = 3;
    input.owned_planet_count = 10;
    input.tech_level_two = 5;
    trace.support_status = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.statuses == 4, &checks, &failures);
    check(trace.calls == 2 && trace.designs[0] == 7 &&
        trace.designs[1] == 1 && trace.planets[0] == 4 &&
        trace.planets[1] == 4 && !trace.bad_tuple, &checks, &failures);
    check(trace.random_calls == 0, &checks, &failures);
    check(input.tech_support_queued == 1 &&
        input.initial_colonizer_queued == 1 &&
        input.unowned_viable_count == 1, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.support_status = 6;
    input.design_one_exists = 1;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.random_calls == 1 && trace.calls == 1 &&
        trace.designs[0] == 7 && !trace.bad_tuple,
        &checks, &failures);
    memset(&trace, 0, sizeof trace);
    trace.support_status = 6;
    trace.random_result = 1;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.random_calls == 1 && trace.calls == 0,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    input.design_one_exists = 0;
    input.tech_level_two = 4;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 1 && trace.designs[0] == 1 &&
        trace.random_calls == 0, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    input.tech_level_two = 5;
    input.ai_data_group_count = 0;
    input.owned_planet_count = 0;
    input.design_one_exists = 1;
    trace.support_status = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 0 && trace.random_calls == 0,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    input.ai_data_group_count = 3;
    trace.fail_call = 1;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 1 && input.tech_support_queued == 0,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    input.design_one_exists = 0;
    planets[0].support_queue_eligible = 0;
    planets[1].visibility_flags = 0x0200U;
    planets[1].mineral_rate = 1;
    planets[1].support_queue_eligible = 1;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 2 && trace.planets[0] == 5 &&
        trace.planets[1] == 5, &checks, &failures);

    printf("AUTOMITRON_EARLY_QUEUE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
