#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct QueueTrace {
    StarsI16 calls;
    StarsI16 designs[24];
    StarsI16 bad_quantity;
} QueueTrace;

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
#ifndef TURIN_DESIGN_QUEUE_NATIVE
        fprintf(stderr, "Turin design queue check failed at line %d\n", line);
#else
        (void)line;
#endif
    }
}

#define check(condition) check_at((condition), __LINE__)

static StarsI16 status(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    (void)opaque;
    (void)last;
    (void)budget;
    (void)mask;
    *selected = first == 4 ? 43 :
        (first == 6 ? 16 : (first == 10 ? 107 :
        (first == 15 ? 115 : -1)));
    return 1;
}

static StarsI16 queue(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 design, StarsI16 quantity)
{
    QueueTrace *trace;
    (void)planet;
    trace = (QueueTrace *)opaque;
    if (trace->calls >= 24)
        return 0;
    trace->designs[trace->calls] = design;
    ++trace->calls;
    if (quantity != 1)
        ++trace->bad_quantity;
    return 1;
}

static void seed(StarsTurinDroneAiTurnInput *input,
    StarsTurinDroneAiTurnOps *ops, StarsTurinTurnPlanet *planet,
    QueueTrace *trace)
{
    StarsI16 role;
    StarsI16 resource;

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
    input->plan_max = 240U;
    input->design_15_inactive = 1;
    planet->owner = 4;
    planet->production_slot_open = 1;
    for (resource = 0; resource < 4; ++resource)
        planet->resources_available[resource] = 100;
    for (role = 0; role < 8; ++role)
        for (resource = 0; resource < 4; ++resource)
            input->design_hull_cost[role][resource] = 1;
    ops->check_design_status = status;
    ops->queue_production = queue;
    ops->context = trace;
}

int turin_design_queue_probe(void)
{
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinTurnPlanet planet;
    QueueTrace trace;
    StarsI16 i;

    checks = failures = 0;
    seed(&input, &ops, &planet, &trace);
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.calls == 20 && trace.bad_quantity == 0 &&
        input.production_queued == 1 && input.production_hulls_queued == 20);
    for (i = 0; i < 20; ++i)
        check(trace.designs[i] == (i < 5 ? 43 :
            (i < 10 ? 16 : (i < 15 ? 107 : 115))));

    seed(&input, &ops, &planet, &trace);
    input.design_existing[3] = 14U;
    input.design_existing[0] = 28U;
    input.design_existing[7] = 72U;
    input.design_existing[5] = 28U;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.calls == 0 && input.production_queued == 0);

    seed(&input, &ops, &planet, &trace);
    input.design_15_inactive = 0;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.calls == 15 && input.production_hulls_queued == 15);

    seed(&input, &ops, &planet, &trace);
    for (i = 0; i < 4; ++i) {
        planet.resources_available[i] = 65;
        input.design_hull_cost[3][i] = 20;
    }
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.calls == 3 && trace.designs[0] == 43 &&
        trace.designs[2] == 43 && input.production_hulls_queued == 3);

    seed(&input, &ops, &planet, &trace);
    for (i = 0; i < 4; ++i) {
        planet.resources_available[i] = 35;
        input.design_hull_cost[3][i] = 5;
        input.design_hull_cost[0][i] = 3;
    }
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.calls == 8 && input.production_hulls_queued == 8);
    check(trace.designs[4] == 43 && trace.designs[5] == 16 &&
        trace.designs[7] == 16);

#ifndef TURIN_DESIGN_QUEUE_NATIVE
    printf("TURIN_DESIGN_QUEUE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef TURIN_DESIGN_QUEUE_NATIVE
int main(void)
{
    return turin_design_queue_probe();
}
#endif
