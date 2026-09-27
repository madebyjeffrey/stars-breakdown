#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 merges;
    StarsI16 productions;
} Trace;

static StarsI16 merge(void *opaque, StarsI16 mask)
{
    Trace *trace;
    (void)mask;
    trace = (Trace *)opaque;
    ++trace->merges;
    return 1;
}

static StarsI16 production(void *opaque, const StarsCyberTurnPlanet *planet,
    StarsI16 budget)
{
    Trace *trace;
    (void)planet;
    (void)budget;
    trace = (Trace *)opaque;
    ++trace->productions;
    return 1;
}

int main(void)
{
    StarsCyberAiTurnOps ops;
    StarsCyberAiTurnInput input;
    StarsCyberTurnPlanet planet;
    Trace trace;
    StarsI16 failures;
    StarsI16 i;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    ops.merge_shdefs = merge;
    ops.visit_production = production;
    ops.context = &trace;
    planet.owner = 3;
    for (i = 0; i < 4; ++i) {
        planet.resources_available[i] = 20;
        planet.production_queue_cost[i] = 20;
    }
    input.turn = 150;
    input.player_id = 3;
    input.planet_count = 1;
    input.planets = &planet;
    input.ops = &ops;
    failures = !stars_do_cyber_ai_turn(&input);
    failures += trace.merges != 5 || trace.productions != 1;
    failures += input.production_affordable_count != 1 ||
        input.production_blocked_count != 0;
    planet.resources_available[2] = 19;
    trace.productions = 0;
    failures += !stars_do_cyber_ai_turn(&input);
    failures += trace.productions != 0 ||
        input.production_affordable_count != 0 ||
        input.production_blocked_count != 1;
    printf("CYBER_AI_RESOURCE_GATE_CHECKS=8 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
