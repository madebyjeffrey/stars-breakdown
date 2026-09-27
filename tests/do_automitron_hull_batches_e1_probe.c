#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 budget_calls;
    StarsI16 cost_calls;
    StarsI16 queue_calls;
    StarsI16 designs[12];
    StarsI16 bad_args;
    StarsI16 budget_amount;
    StarsI16 queue_cost;
    StarsI16 fail_queue_call;
} Trace;

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

static StarsI16 status(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected)
{
    (void)opaque;
    (void)last;
    (void)budget;
    *selected = first == 11 ? 4 : (first == 9 ? 8 : -1);
    return 0;
}

static StarsI16 get_budget(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI32 available[4],
    StarsI32 queue_cost[4])
{
    Trace *trace;
    StarsI16 resource;
    trace = (Trace *)opaque;
    if (planet->id != 6)
        trace->bad_args = 1;
    ++trace->budget_calls;
    for (resource = 0; resource < 4; ++resource) {
        available[resource] = trace->budget_amount -
            (StarsI32)trace->queue_calls * 10;
        queue_cost[resource] = trace->queue_cost;
    }
    return 1;
}

static StarsI16 hull_cost(void *opaque, StarsI16 player,
    StarsI16 design, StarsI32 cost[4])
{
    Trace *trace;
    StarsI16 resource;
    trace = (Trace *)opaque;
    ++trace->cost_calls;
    if (player != 2 || (design != 4 && design != 8))
        trace->bad_args = 1;
    for (resource = 0; resource < 4; ++resource)
        cost[resource] = 10;
    return 1;
}

static StarsI16 queue(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI16 design,
    StarsI16 count, StarsI16 kind, StarsI16 flag)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->queue_calls < 12)
        trace->designs[trace->queue_calls] = design;
    ++trace->queue_calls;
    if (planet->id != 6 || count != 1 || kind != 2 || flag != 1)
        trace->bad_args = 1;
    return trace->queue_calls != trace->fail_queue_call;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnPlanet planet;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    planet.id = 6;
    planet.owner = 2;
    planet.visibility_flags = 0x0200U;
    planet.mineral_rate = 1;
    planet.support_queue_eligible = 1;
    ops.check_design_status = status;
    ops.get_production_budget = get_budget;
    ops.get_hull_cost = hull_cost;
    ops.add_raw_queue_item = queue;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 2;
    input.planet_count = 1;
    input.planets = &planet;
    input.ops = &ops;
    input.hull_batch_inputs_bound = 1;
    input.game_planet_max = 120;
    trace.budget_amount = 100;

    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.budget_calls == 2 && trace.cost_calls == 10 &&
        trace.queue_calls == 10 && !trace.bad_args, &checks, &failures);
    for (index = 0; index < 5; ++index)
        check(trace.designs[index] == 4 && trace.designs[index + 5] == 8,
            &checks, &failures);
    check(input.production_hulls_queued == 10 &&
        input.production_queued == 1, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.budget_amount = 25;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.budget_calls == 1 && trace.cost_calls == 3 &&
        trace.queue_calls == 2 && input.production_hulls_queued == 2,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.budget_amount = 100;
    input.is_design_existing = 18;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.budget_calls == 1 && trace.cost_calls == 5 &&
        trace.queue_calls == 5 && trace.designs[0] == 8,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.budget_amount = 10;
    trace.queue_cost = 11;
    input.is_design_existing = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.budget_calls == 1 && trace.cost_calls == 0 &&
        trace.queue_calls == 0, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.budget_amount = 100;
    trace.fail_queue_call = 2;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.budget_calls == 1 && trace.queue_calls == 2 &&
        trace.designs[0] == 4 && trace.designs[1] == 4,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    planet.support_queue_eligible = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.budget_calls == 0 && trace.queue_calls == 0,
        &checks, &failures);

    printf("AUTOMITRON_HULL_BATCH_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
