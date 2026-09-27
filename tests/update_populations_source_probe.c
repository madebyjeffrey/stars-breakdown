#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 changes;
    StarsU16 messages;
    StarsU16 uninhabited;
    StarsI16 last_message;
    StarsI16 last_player;
} Trace;

static StarsI32 change(void *c, StarsUpdatePopulationPlanet *p, StarsI16 mode)
{ Trace *t = (Trace *)c; (void)p; (void)mode; ++t->changes; return -100L; }
static StarsI16 desirable(void *c, const StarsUpdatePopulationPlanet *p)
{ (void)c; (void)p; return -1; }
static void message(void *c, StarsI16 player, StarsU16 id, StarsI16 a,
    StarsI16 b)
{ Trace *t = (Trace *)c; (void)a; (void)b; ++t->messages;
  t->last_message = (StarsI16)id; t->last_player = player; }
static StarsI16 race(void *c, StarsI16 player, StarsI16 stat)
{ (void)c; (void)player; (void)stat; return 8; }
static void uninhabit(void *c, StarsUpdatePopulationPlanet *p)
{ Trace *t = (Trace *)c; (void)p; ++t->uninhabited; }

int main(void)
{
    StarsUpdatePopulationInput input;
    StarsUpdatePopulationPlanet planets[3];
    Trace trace;
    memset(&input, 0, sizeof input); memset(&planets, 0, sizeof planets);
    memset(&trace, 0, sizeof trace);
    planets[0].player = 2; planets[0].id = 10; planets[0].population_word = 10;
    planets[0].reserve = 1;
    planets[1].player = -1; planets[1].id = 11;
    planets[2].player = 3; planets[2].id = 12;
    input.planets = planets; input.planet_count = 3;
    input.ops.change_population = change; input.ops.desirability = desirable;
    input.ops.send_message = message; input.ops.race_stat = race;
    input.ops.uninhabit = uninhabit; input.context = &trace;
    stars_update_populations(&input);
    if (trace.changes != 1U || trace.messages != 2U ||
            trace.uninhabited != 2U || trace.last_message != 0x41 ||
            trace.last_player != 3)
        return 1;
    puts("UPDATE_POPULATIONS_SOURCE=PASS");
    return 0;
}
