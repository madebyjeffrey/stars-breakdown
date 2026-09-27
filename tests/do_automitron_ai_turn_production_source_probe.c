#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProductionTrace {
    StarsI16 calls;
    StarsI16 design;
    StarsI16 quantity;
} ProductionTrace;

static StarsI16 status(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected)
{
    (void)opaque;
    (void)last;
    (void)budget;
    if (first == 11)
        *selected = 4;
    else if (first == 4)
        *selected = 6;
    else if (first == 2)
        *selected = 7;
    else
        *selected = 8;
    return 1;
}

static StarsI16 queue_production(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI16 design,
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
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronAiTurnInput input;
    StarsAutomitronTurnPlanet planets[2];
    ProductionTrace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(planets, 0, sizeof planets);
    memset(&trace, 0, sizeof trace);
    ops.check_design_status = status;
    ops.queue_production = queue_production;
    ops.context = &trace;
    planets[0].owner = 2;
    planets[0].visibility_flags = 0x0200U;
    planets[0].mineral_rate = 1;
    planets[0].mineral_concentration = 0;
    planets[0].production_slot_open = 1;
    planets[0].resources_available[0] = 100;
    planets[0].resources_available[1] = 100;
    planets[0].resources_available[2] = 100;
    planets[0].resources_available[3] = 100;
    planets[0].production_queue_cost[0] = 10;
    planets[0].production_queue_cost[1] = 10;
    planets[0].production_queue_cost[2] = 10;
    planets[0].production_queue_cost[3] = 10;
    planets[0].hull_cost[0] = 10;
    planets[0].hull_cost[1] = 10;
    planets[0].hull_cost[2] = 10;
    planets[0].hull_cost[3] = 10;
    planets[1].owner = -1;
    input.turn = 150;
    input.player_id = 2;
    input.planet_count = 2;
    input.planets = planets;
    input.ops = &ops;
    failures = !stars_do_automitron_ai_turn(&input);
    failures += trace.calls != 1 || trace.design != 4 || trace.quantity != 5;
    failures += input.production_queued != 1 ||
        input.production_hulls_queued != 5;
    planets[0].hull_cost[0] = 30;
    planets[0].hull_cost[1] = 30;
    planets[0].hull_cost[2] = 30;
    planets[0].hull_cost[3] = 30;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_automitron_ai_turn(&input);
    failures += trace.calls != 0 || input.production_queued != 0 ||
        input.production_hulls_queued != 0;
    planets[0].hull_cost[0] = 10;
    planets[0].hull_cost[1] = 10;
    planets[0].hull_cost[2] = 10;
    planets[0].hull_cost[3] = 10;
    planets[0].mineral_rate = 0;
    planets[0].mineral_concentration = 0x5db;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_automitron_ai_turn(&input);
    failures += trace.calls != 0 || input.production_queued != 0;
    planets[0].mineral_concentration = 0x5dc;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_automitron_ai_turn(&input);
    failures += trace.calls != 1 || input.production_hulls_queued != 5;
    planets[0].desirability = -1;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_automitron_ai_turn(&input);
    failures += trace.calls != 0 || input.production_queued != 0;
    planets[0].desirability = 0;
    planets[0].visibility_flags = 0;
    memset(&trace, 0, sizeof trace);
    failures += !stars_do_automitron_ai_turn(&input);
    failures += trace.calls != 0 || input.production_queued != 0;
    printf("AUTOMITRON_AI_PRODUCTION_CHECKS=12 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
