#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct SeedTrace {
    StarsI16 queues;
    StarsI16 finishes;
    StarsI16 last_changed;
    StarsI16 reject_queue_at;
    StarsI16 reject_finish;
    StarsI16 events[16];
    StarsI16 event_count;
} SeedTrace;

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
#ifndef TURIN_SEED_NATIVE
        fprintf(stderr, "Turin seed check failed at line %d\n", line);
#else
        (void)line;
#endif
    }
}

#define check(condition) check_at((condition), __LINE__)

static StarsI16 queue(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 design, StarsI16 quantity)
{
    SeedTrace *trace;
    (void)planet;
    trace = (SeedTrace *)opaque;
    if (design != 0 || quantity != 1)
        return 0;
    ++trace->queues;
    if (trace->event_count < 16)
        trace->events[trace->event_count] = 1;
    ++trace->event_count;
    return trace->queues != trace->reject_queue_at;
}

static StarsI16 finish(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 changed)
{
    SeedTrace *trace;
    (void)planet;
    trace = (SeedTrace *)opaque;
    ++trace->finishes;
    trace->last_changed = changed;
    if (trace->event_count < 16)
        trace->events[trace->event_count] = 2;
    ++trace->event_count;
    return (StarsI16)(!trace->reject_finish);
}

static void seed(StarsTurinDroneAiTurnInput *input,
    StarsTurinDroneAiTurnOps *ops, StarsTurinTurnPlanet *planet,
    SeedTrace *trace)
{
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    memset(planet, 0, sizeof *planet);
    memset(trace, 0, sizeof *trace);
    input->turn = 0;
    input->player_id = 4;
    input->planet_count = 1;
    input->planets = planet;
    input->ops = ops;
    input->plan_max = 240U;
    planet->owner = 4;
    planet->production_slot_open = 1;
    ops->queue_production = queue;
    ops->finish_production = finish;
    ops->context = trace;
}

int turin_seed_finish_probe(void)
{
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinTurnPlanet planet;
    SeedTrace trace;
    StarsI16 i;

    checks = failures = 0;
    seed(&input, &ops, &planet, &trace);
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queues == 6 && trace.finishes == 1 &&
        trace.last_changed == 1 && input.production_hulls_queued == 6 &&
        input.production_queued == 1);
    for (i = 0; i < 6; ++i)
        check(trace.events[i] == 1);
    check(trace.events[6] == 2 && trace.event_count == 7);

    /* The retained E3 map has 32 plan points.  At planet 24, the
     * turn-zero seed loop requests two design-0 hulls when the production
     * slot is open and no ship queue is present (32 -> 2 -> -28). */
    seed(&input, &ops, &planet, &trace);
    input.player_id = 1;
    input.plan_max = 32U;
    planet.id = 24;
    planet.owner = 1;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queues == 2 && trace.finishes == 1 &&
        trace.last_changed == 1 && input.production_hulls_queued == 2 &&
        input.production_queued == 1);
    check(trace.events[0] == 1 && trace.events[1] == 1 &&
        trace.events[2] == 2 && trace.event_count == 3);

    seed(&input, &ops, &planet, &trace);
    planet.production_queue_has_ship = 1;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queues == 0 && trace.finishes == 1 &&
        trace.last_changed == 0 && trace.events[0] == 2);

    seed(&input, &ops, &planet, &trace);
    input.turn = 150;
    input.base_design_inactive = 1;
    input.base_design_hull_id = 5;
    input.base_design_existing = 10U;
    input.base_design_built = 50U;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queues == 1 && trace.finishes == 1 &&
        trace.last_changed == 1 && trace.events[0] == 1 &&
        trace.events[1] == 2);

    seed(&input, &ops, &planet, &trace);
    input.turn = 150;
    input.base_design_inactive = 1;
    input.base_design_hull_id = 5;
    input.base_design_existing = 32U;
    input.base_design_built = 50U;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queues == 0 && trace.finishes == 1 &&
        trace.last_changed == 0);

    seed(&input, &ops, &planet, &trace);
    input.turn = 150;
    input.base_design_inactive = 1;
    input.base_design_hull_id = 5;
    input.base_design_existing = 10U;
    input.base_design_built = 100U;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queues == 0 && trace.last_changed == 0);

    seed(&input, &ops, &planet, &trace);
    trace.reject_queue_at = 3;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(trace.queues == 3 && trace.finishes == 0 &&
        input.production_hulls_queued == 2);

    seed(&input, &ops, &planet, &trace);
    trace.reject_finish = 1;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(trace.queues == 6 && trace.finishes == 1);

#ifndef TURIN_SEED_NATIVE
    printf("TURIN_SEED_FINISH_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef TURIN_SEED_NATIVE
int main(void)
{
    return turin_seed_finish_probe();
}
#endif
