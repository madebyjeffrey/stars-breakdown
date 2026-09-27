#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 design;
    StarsI16 quantity;
    StarsI16 reject_at;
} Trace;

static StarsI16 failures;

static StarsI16 status(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    (void)opaque;
    (void)first;
    (void)last;
    (void)budget;
    (void)mask;
    *selected = 9;
    return 1;
}

static StarsI16 queue_production(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 design, StarsI16 quantity)
{
    Trace *trace;
    (void)planet;
    trace = (Trace *)opaque;
    ++trace->calls;
    trace->design = design;
    trace->quantity = quantity;
    return trace->calls != trace->reject_at;
}

int turin_production_probe(void)
{
    StarsTurinDroneAiTurnOps ops;
    StarsTurinDroneAiTurnInput input;
    StarsTurinTurnPlanet planet;
    Trace trace;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    failures = 0;
    ops.check_design_status = status;
    ops.queue_production = queue_production;
    ops.context = &trace;
    planet.owner = 4;
    planet.production_slot_open = 1;
    planet.resources_available[0] = 100;
    planet.resources_available[1] = 100;
    planet.resources_available[2] = 100;
    planet.resources_available[3] = 100;
    planet.production_queue_cost[0] = 10;
    planet.production_queue_cost[1] = 10;
    planet.production_queue_cost[2] = 10;
    planet.production_queue_cost[3] = 10;
    planet.hull_cost[0] = 10;
    planet.hull_cost[1] = 10;
    planet.hull_cost[2] = 10;
    planet.hull_cost[3] = 10;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 1;
    input.planets = &planet;
    input.ops = &ops;
    failures = !stars_do_turin_drone_ai_turn(&input);
    failures += trace.calls != 5 || trace.design != 9 || trace.quantity != 1;
    failures += input.production_queued != 1 ||
        input.production_hulls_queued != 5 || input.design_selection[0] != 9;
    planet.hull_cost[0] = 30;
    planet.hull_cost[1] = 30;
    planet.hull_cost[2] = 30;
    planet.hull_cost[3] = 30;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_turin_drone_ai_turn(&input);
    failures += trace.calls != 3 || input.production_queued != 1 ||
        input.production_hulls_queued != 3;
    planet.hull_cost[0] = 10;
    planet.hull_cost[1] = 10;
    planet.hull_cost[2] = 10;
    planet.hull_cost[3] = 10;
    planet.resources_available[0] = 59;
    planet.resources_available[1] = 59;
    planet.resources_available[2] = 59;
    planet.resources_available[3] = 59;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_turin_drone_ai_turn(&input);
    failures += trace.calls != 4 || input.production_hulls_queued != 4;
    planet.resources_available[0] = 9;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_turin_drone_ai_turn(&input);
    failures += trace.calls != 0 || input.production_queued != 0;
    planet.resources_available[0] = 100;
    memset(&trace, 0, sizeof trace);
    trace.reject_at = 3;
    failures += stars_do_turin_drone_ai_turn(&input) != 0;
    failures += trace.calls != 3 || input.production_queued != 1 ||
        input.production_hulls_queued != 2;
#ifndef TURIN_PRODUCTION_NATIVE
    printf("TURIN_DRONE_AI_PRODUCTION_CHECKS=11 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef TURIN_PRODUCTION_NATIVE
int main(void)
{
    return turin_production_probe();
}
#endif
