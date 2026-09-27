#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsI16 values[4]; StarsI16 at; StarsI16 player; StarsI16 mineral; } Trace;
static StarsI16 random_value(void *context, StarsI16 limit)
{
    Trace *t = (Trace *)context; (void)limit; return t->values[t->at++];
}
static void message(void *context, StarsI16 player, StarsU16 a, StarsU16 b,
    StarsI16 mineral)
{ Trace *t = (Trace *)context; (void)a; (void)b; t->player = player;
  t->mineral = mineral; }
int main(void)
{
    StarsDiscoverMineralPlanet planets[1];
    StarsDiscoverMineralsInput input; Trace trace;
    memset(planets, 0, sizeof planets); memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    trace.values[0] = 0; trace.values[1] = 0; trace.values[2] = 1;
    trace.values[3] = 4;
    planets[0].player = 2; planets[0].id = 17U; planets[0].concentration[1] = 10U;
    input.turn = 10; input.planets = planets; input.planet_count = 1U;
    input.random = random_value; input.message = message; input.context = &trace;
    if (stars_discover_new_minerals(&input), planets[0].concentration[1] != 19U ||
        trace.player != 2 || trace.mineral != 1) return 1;
    input.mineral_size = 15; trace.at = 0;
    trace.values[0] = 1;
    if (stars_discover_new_minerals(&input), trace.at != 1) return 2;
    puts("DISCOVER_MINERALS_SOURCE=PASS");
    return 0;
}
