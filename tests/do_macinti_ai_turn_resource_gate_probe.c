#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 affordable;
    StarsI16 batch_affordable;
} Trace;

static StarsI16 visit_planet(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn)
{
    Trace *trace;
    (void)planet;
    (void)turn;
    trace = (Trace *)opaque;
    ++trace->calls;
    trace->affordable += policy->production_affordable;
    trace->batch_affordable += policy->production_batch_affordable;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnOps ops;
    StarsMacintiAiTurnInput input;
    StarsMacintiTurnPlanet planet;
    Trace trace;
    StarsI16 failures;
    StarsI16 i;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    ops.visit_planet_typed = visit_planet;
    ops.context = &trace;
    planet.owner = 3;
    for (i = 0; i < 4; ++i) {
        planet.resources_available[i] = 100;
        planet.production_queue_cost[i] = 10;
        planet.hull_cost[i] = 30;
    }
    planet.production_batch = 3;
    input.turn = 150;
    input.player_id = 3;
    input.planet_count = 1;
    input.planets = &planet;
    input.ops = &ops;
    failures = !stars_do_macinti_ai_turn(&input);
    failures += trace.calls != 1 || trace.affordable != 1 ||
        trace.batch_affordable != 1;
    for (i = 0; i < 4; ++i)
        planet.resources_available[i] = 10;
    trace.calls = 0;
    trace.affordable = 0;
    trace.batch_affordable = 0;
    failures += !stars_do_macinti_ai_turn(&input);
    failures += trace.calls != 1 || trace.affordable != 1 ||
        trace.batch_affordable != 0;
    planet.resources_available[2] = 9;
    trace.calls = 0;
    trace.affordable = 0;
    trace.batch_affordable = 0;
    failures += !stars_do_macinti_ai_turn(&input);
    failures += trace.calls != 1 || trace.affordable != 0 ||
        trace.batch_affordable != 0;
    printf("MACINTI_AI_RESOURCE_GATE_CHECKS=9 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
