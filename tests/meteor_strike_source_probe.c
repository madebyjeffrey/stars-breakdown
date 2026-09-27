#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 messages; StarsI16 tossed; StarsI16 draws; StarsI16 limits[32]; StarsI16 gate; } Trace;
static StarsI16 random_value(void *c, StarsI16 limit)
{ Trace *t = (Trace *)c; t->limits[t->draws++] = limit;
  return t->draws == 1 ? t->gate : 0; }
static StarsI16 race(void *c, StarsI16 p, StarsI16 s)
{ (void)c; (void)p; (void)s; return 0; }
static void message(void *c, StarsI16 p, StarsI16 m, StarsI16 id,
    const StarsU32 impact[3])
{ Trace *t = (Trace *)c; (void)p; (void)m; (void)id; (void)impact; ++t->messages; }
static void toss(void *c, StarsMeteorPlanet *p)
{ Trace *t = (Trace *)c; (void)p; ++t->tossed; }
int main(void)
{
    StarsMeteorInput input;
    StarsMeteorPlanet planet;
    Trace trace;
    memset(&input, 0, sizeof input); memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    planet.id = 4; planet.owner = -1; planet.population = 50UL;
    planet.concentration[0] = 50; planet.environment[0] = 50;
    planet.original_environment[0] = 50;
    input.turn = 10; input.player_count = 2; input.planets = &planet;
    input.planet_count = 1; input.random = random_value; input.race_stat = race;
    input.send_message = message; input.toss_non_auto_build_items = toss;
    input.context = &trace;
    stars_meteor_strike(&input);
    if (trace.messages != 2 || trace.tossed != 1 ||
            planet.concentration[0] != 100 || planet.environment[0] != 53 ||
            planet.minerals[0] == 0UL)
        return 1;
    memset(&trace, 0, sizeof trace);
    input.turn = 0; trace.gate = 7;
    stars_meteor_strike(&input);
    if (trace.draws != 1 || trace.limits[0] != 20 || trace.messages || trace.tossed)
        return 2;
    memset(&trace, 0, sizeof trace);
    stars_meteor_strike(&input);
    if (trace.draws != 2 || trace.limits[0] != 20 || trace.limits[1] != 1 ||
            trace.messages || trace.tossed) return 3;
    puts("METEOR_STRIKE_SOURCE=PASS");
    return 0;
}
