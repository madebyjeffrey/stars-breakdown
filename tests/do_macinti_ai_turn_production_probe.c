#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 plans;
    StarsI16 queued;
    StarsI16 finished;
    StarsI16 queued_design;
    StarsI16 queued_quantity;
    StarsI16 queued_reason;
    StarsI16 finish_mode;
    StarsI16 raw_seen;
} Trace;

static StarsI16 visit_planet(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn)
{
    (void)opaque;
    (void)planet;
    (void)policy;
    (void)turn;
    return 1;
}

static StarsI16 plan_production(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn,
    StarsMacintiProductionPlan *plan)
{
    Trace *trace;
    (void)policy;
    (void)turn;
    trace = (Trace *)opaque;
    ++trace->plans;
    trace->raw_seen += planet->raw_record != 0 && planet->point.x == 12;
    if (planet->id == 4) {
        plan->action = STARS_MACINTI_PRODUCTION_QUEUE;
        plan->design = 7;
        plan->quantity = 2;
        plan->object_group = 2;
        plan->queue_mode = 1;
        plan->reason = 4;
    } else {
        plan->action = STARS_MACINTI_PRODUCTION_FINISH;
        plan->finish_mode = 0;
    }
    return 1;
}

static StarsI16 queue_production(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiProductionPlan *plan)
{
    Trace *trace = (Trace *)opaque;
    (void)planet;
    ++trace->queued;
    trace->queued_design = plan->design;
    trace->queued_quantity = plan->quantity;
    trace->queued_reason = plan->reason;
    return 1;
}

static StarsI16 finish_production(void *opaque,
    const StarsMacintiTurnPlanet *planet, StarsI16 mode)
{
    Trace *trace = (Trace *)opaque;
    (void)planet;
    ++trace->finished;
    trace->finish_mode = mode;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnOps ops;
    StarsMacintiAiTurnInput input;
    StarsMacintiTurnPlanet planets[2];
    StarsU8 raw_planet[2];
    Trace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(planets, 0, sizeof planets);
    memset(raw_planet, 0, sizeof raw_planet);
    memset(&trace, 0, sizeof trace);
    planets[0].id = 4;
    planets[0].raw_record = raw_planet;
    planets[0].point.x = 12;
    planets[0].owner = 3;
    planets[0].production_queue_count = 1;
    planets[0].production_queue_limit = 24;
    planets[0].resources_available[0] = 10;
    planets[0].resources_available[1] = 10;
    planets[0].resources_available[2] = 10;
    planets[0].resources_available[3] = 10;
    planets[0].production_queue_cost[0] = 2;
    planets[0].production_queue_cost[1] = 2;
    planets[0].production_queue_cost[2] = 2;
    planets[0].production_queue_cost[3] = 2;
    planets[0].hull_cost[0] = 4;
    planets[0].hull_cost[1] = 4;
    planets[0].hull_cost[2] = 4;
    planets[0].hull_cost[3] = 4;
    planets[1] = planets[0];
    planets[1].id = 5;
    planets[1].raw_record = 0;
    planets[1].point.x = 0;
    ops.plan_production = plan_production;
    ops.queue_production = queue_production;
    ops.finish_production = finish_production;
    ops.visit_planet_typed = visit_planet;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 3;
    input.planet_count = 2;
    input.planets = planets;
    input.ops = &ops;
    failures = !stars_do_macinti_ai_turn(&input);
    failures += trace.plans != 2 || trace.queued != 1 ||
        trace.finished != 1;
    failures += trace.raw_seen != 1;
    failures += trace.queued_design != 7 || trace.queued_quantity != 2 ||
        trace.queued_reason != 4 || trace.finish_mode != 0;
    failures += input.typed_production_candidates != 2 ||
        input.typed_production_queued != 1 ||
        input.typed_production_finished != 1 ||
        input.typed_production_rejected != 0;
    printf("MACINTI_AI_PRODUCTION_CHECKS=3 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
