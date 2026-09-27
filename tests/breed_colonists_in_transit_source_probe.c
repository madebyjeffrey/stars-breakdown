#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 cargo_messages;
    StarsI16 planet_messages;
    StarsTransitBreedPlanet *planet;
} Trace;
static StarsI16 race(void *c, StarsI16 player, StarsI16 stat)
{ (void)c; (void)stat; return player == 0 ? 4 : 0; }
static StarsI16 random_value(void *c, StarsI16 limit)
{ (void)c; (void)limit; return 0; }
static StarsI32 transfer(void *c, StarsTransitBreedFleet *fleet, StarsI32 amount)
{ (void)c; fleet->colonists -= (StarsU32)(amount / 2L); return amount / 2L; }
static StarsTransitBreedPlanet *lookup(void *c, StarsI16 id)
{ Trace *t = (Trace *)c; (void)id; return t->planet; }
static void message(void *c, StarsI16 player, StarsI16 id, StarsU16 a,
    StarsU16 b, StarsU16 d, StarsI16 e, StarsI16 f, StarsI16 g)
{ Trace *t = (Trace *)c; (void)player; (void)a; (void)b; (void)d; (void)e;
  (void)f; (void)g; if (id == 0xfb) ++t->cargo_messages;
  if (id == 0x158) ++t->planet_messages; }

int main(void)
{
    StarsTransitBreedFleet fleet;
    StarsTransitBreedPlanet planet;
    StarsTransitBreedInput input;
    Trace trace;
    memset(&fleet, 0, sizeof fleet); memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleet.player = 0; fleet.id = 4; fleet.destination = -1;
    fleet.growth_rate = 1; fleet.colonists = 400UL;
    planet.player = 0; planet.id = 8; planet.population = 0UL;
    fleet.destination = planet.id;
    input.fleets = &fleet; input.fleet_count = 1; input.player_count = 1;
    input.ops.race_stat = race; input.ops.random = random_value;
    input.ops.transfer_colonists = transfer; input.ops.planet_from_id = lookup;
    input.ops.send_message = message;
    trace.planet = &planet;
    input.context = &trace;
    stars_breed_colonists_in_transit(&input);
    if (trace.cargo_messages != 1 || trace.planet_messages != 1 ||
            fleet.colonists != 399UL || planet.population != 1UL)
        return 1;
    puts("BREED_COLONISTS_IN_TRANSIT_SOURCE=PASS");
    return 0;
}
