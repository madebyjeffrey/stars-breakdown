#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsAutomitronAiTurnInput *input;
    StarsI16 visits;
    StarsI16 first_saw_full_count;
} Trace;

static StarsI16 visit(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI16 budget)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->visits == 0)
        trace->first_saw_full_count =
            trace->input->unowned_viable_count == 2 &&
            planet->id == 0 && budget == 50;
    ++trace->visits;
    return 1;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnPlanet planets[4];
    Trace trace;
    StarsI16 failures;

    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(planets, 0, sizeof planets);
    memset(&trace, 0, sizeof trace);
    planets[0].id = 0;
    planets[0].owner = 2;
    planets[1].id = 1;
    planets[1].owner = -1;
    planets[1].planet_class = 3;
    planets[1].optimal_value = 1;
    planets[2].id = 2;
    planets[2].owner = -1;
    planets[2].planet_class = 2;
    planets[2].optimal_value = 1;
    planets[3].id = 3;
    planets[3].owner = -1;
    planets[3].planet_class = 4;
    planets[3].optimal_value = 2;
    trace.input = &input;
    ops.visit_planet = visit;
    ops.context = &trace;
    input.turn = 10;
    input.player_id = 2;
    input.planet_count = 4;
    input.planets = planets;
    input.ops = &ops;

    failures += !stars_do_automitron_ai_turn(&input);
    failures += !trace.first_saw_full_count;
    failures += trace.visits != 4;
    failures += input.unowned_viable_count != 2;
    trace.visits = 0;
    input.unowned_viable_count = 99;
    failures += !stars_do_automitron_ai_turn(&input) ||
        input.unowned_viable_count != 2;

    printf("AUTOMITRON_CENSUS_CHECKS=5 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
