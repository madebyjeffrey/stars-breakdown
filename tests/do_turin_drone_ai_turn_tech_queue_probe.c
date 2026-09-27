#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct TechTrace {
    StarsI16 random_calls;
    StarsI16 random_bound;
    StarsI16 random_result;
    StarsI16 queue_calls;
    StarsI16 queued_design;
    StarsI16 reject_queue;
    StarsI16 finish_calls;
    StarsI16 finish_changed;
    StarsI16 selected_design;
} TechTrace;

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
#ifndef TURIN_TECH_NATIVE
        fprintf(stderr, "Turin tech check failed at line %d\n", line);
#else
        (void)line;
#endif
    }
}

#define check(condition) check_at((condition), __LINE__)

static StarsI16 status(void *opaque, StarsI16 begin, StarsI16 end,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    TechTrace *trace;
    (void)end;
    (void)budget;
    (void)mask;
    trace = (TechTrace *)opaque;
    *selected = begin == 8 ? trace->selected_design : -1;
    return 1;
}

static StarsI16 random_value(void *opaque, StarsI16 bound)
{
    TechTrace *trace;
    trace = (TechTrace *)opaque;
    ++trace->random_calls;
    trace->random_bound = bound;
    return trace->random_result;
}

static StarsI16 queue(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 design, StarsI16 quantity)
{
    TechTrace *trace;
    (void)planet;
    trace = (TechTrace *)opaque;
    ++trace->queue_calls;
    trace->queued_design = design;
    return (StarsI16)(design == 8 && quantity == 1 &&
        !trace->reject_queue);
}

static StarsI16 finish(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 changed)
{
    TechTrace *trace;
    (void)planet;
    trace = (TechTrace *)opaque;
    ++trace->finish_calls;
    trace->finish_changed = changed;
    return 1;
}

static void seed(StarsTurinDroneAiTurnInput *input,
    StarsTurinDroneAiTurnOps *ops, StarsTurinTurnPlanet *planet,
    TechTrace *trace)
{
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    memset(planet, 0, sizeof *planet);
    memset(trace, 0, sizeof *trace);
    input->turn = 150;
    input->player_id = 4;
    input->planet_count = 1;
    input->planets = planet;
    input->ops = ops;
    input->production_designs_valid = 1;
    input->production_policy_valid = 1;
    input->player_tech_2 = 5;
    input->player_planet_count = 100;
    input->ai_planet_scale = 4;
    planet->owner = 4;
    planet->production_slot_open = 1;
    trace->selected_design = 8;
    ops->check_design_status = status;
    ops->random_value = random_value;
    ops->queue_production = queue;
    ops->finish_production = finish;
    ops->context = trace;
}

int turin_tech_queue_probe(void)
{
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinTurnPlanet planet;
    TechTrace trace;

    checks = failures = 0;
    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 9;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 0 && trace.queue_calls == 1 &&
        trace.queued_design == 8 && trace.finish_changed == 1);
    check(input.production_queued == 1 && input.production_hulls_queued == 1);

    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 12;
    trace.random_result = 0;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 1 && trace.random_bound == 4 &&
        trace.queue_calls == 1 && trace.finish_changed == 1);

    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 12;
    trace.random_result = 1;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 1 && trace.queue_calls == 0 &&
        trace.finish_changed == 0);

    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 14;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 0 && trace.queue_calls == 0 &&
        trace.finish_calls == 1);

    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 0;
    input.player_tech_2 = 4;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 0 && trace.queue_calls == 0);

    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 0;
    trace.selected_design = -1;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 0 && trace.queue_calls == 0);

    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 9;
    input.ai_planet_scale = 6;
    input.player_planet_count = 50;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queue_calls == 1 && trace.random_calls == 0);

    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 9;
    trace.reject_queue = 1;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(trace.queue_calls == 1 && trace.finish_calls == 0 &&
        input.production_queued == 0);

    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 12;
    ops.random_value = 0;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(trace.queue_calls == 0 && trace.finish_calls == 0);

    seed(&input, &ops, &planet, &trace);
    input.design_8_9_existing_total = 9;
    ops.queue_production = 0;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 0 && trace.finish_calls == 0);

#ifndef TURIN_TECH_NATIVE
    printf("TURIN_TECH_QUEUE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef TURIN_TECH_NATIVE
int main(void)
{
    return turin_tech_queue_probe();
}
#endif
