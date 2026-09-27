#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProductionTrace {
    StarsI16 calls;
    StarsI16 design;
    StarsI16 quantity;
} ProductionTrace;

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
    const StarsRobotoidTurnPlanet *planet, StarsI16 design,
    StarsI16 quantity)
{
    ProductionTrace *trace;
    (void)planet;
    trace = (ProductionTrace *)opaque;
    ++trace->calls;
    trace->design = design;
    trace->quantity = quantity;
    return 1;
}

int main(void)
{
    StarsRobotoidAiTurnOps ops;
    StarsRobotoidAiTurnInput input;
    StarsRobotoidTurnPlanet planet;
    ProductionTrace trace;
    StarsI16 failures;
    StarsI16 i;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    ops.check_design_status = status;
    ops.queue_production = queue_production;
    ops.context = &trace;
    planet.owner = 2;
    planet.visibility_flags = 0x0200U;
    planet.mineral_rate = 1;
    planet.mineral_concentration = 0;
    planet.production_slot_open = 1;
    for (i = 0; i < 4; ++i) {
        planet.resources_available[i] = 100;
        planet.production_queue_cost[i] = 10;
        planet.hull_cost[i] = 10;
    }
    input.turn = 150;
    input.player_id = 2;
    input.planet_count = 1;
    input.planets = &planet;
    input.ops = &ops;
    failures = !stars_do_robotoid_ai_turn(&input);
    failures += trace.calls != 1 || trace.design != 9 || trace.quantity != 5;
    failures += input.production_queued != 1 ||
        input.production_hulls_queued != 5;
    for (i = 0; i < 4; ++i)
        planet.hull_cost[i] = 30;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_robotoid_ai_turn(&input);
    failures += trace.calls != 0 || input.production_queued != 0 ||
        input.production_hulls_queued != 0;
    for (i = 0; i < 4; ++i)
        planet.hull_cost[i] = 10;
    planet.mineral_rate = 0;
    planet.mineral_concentration = 199;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_robotoid_ai_turn(&input);
    failures += trace.calls != 0 || input.production_queued != 0;
    planet.mineral_concentration = 200;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_robotoid_ai_turn(&input);
    failures += trace.calls != 1 || input.production_hulls_queued != 5;
    planet.visibility_flags = 0;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_robotoid_ai_turn(&input);
    failures += trace.calls != 0 || input.production_queued != 0;
    printf("ROBOTOID_AI_PRODUCTION_CHECKS=12 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
