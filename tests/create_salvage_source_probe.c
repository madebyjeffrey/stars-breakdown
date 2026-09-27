#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 race_calls;
    StarsU16 drops;
    StarsU16 planet_adds;
    StarsI16 owner;
    StarsU32 dropped[3];
    StarsI32 added[3];
} Trace;

static StarsI16 race(void *c, StarsI16 player, StarsI16 stat)
{ Trace *t = (Trace *)c; (void)stat; ++t->race_calls; t->owner = player; return 1; }
static StarsI32 cost(void *c, StarsI16 player, StarsI16 design,
    StarsI16 mineral, StarsI16 adjusted)
{ (void)c; (void)player; (void)design; (void)adjusted; return (StarsI32)(30L * (mineral + 1)); }
static void drop(void *c, const StarsU32 *amounts, StarsI16 player,
    StarsI16 x, StarsI16 y)
{ Trace *t = (Trace *)c; (void)player; (void)x; (void)y; ++t->drops;
  t->dropped[0] = amounts[0]; t->dropped[1] = amounts[1]; t->dropped[2] = amounts[2]; }
static void add_planet(void *c, void *planet, StarsI16 mineral, StarsI32 amount)
{ Trace *t = (Trace *)c; (void)planet; ++t->planet_adds; t->added[mineral] = amount; }

int main(void)
{
    StarsCreateSalvageInput input;
    StarsKillShipsFleet fleet;
    Trace trace;
    memset(&input, 0, sizeof input); memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    fleet.player = 4; fleet.x = 7; fleet.y = 8; fleet.ship_counts[0] = 2;
    fleet.cargo[0] = 9UL;
    input.fleet = &fleet; input.ops.race_grbit = race;
    input.ops.ore_cost = cost; input.ops.drop_salvage = drop;
    input.ops.add_planet_mineral = add_planet; input.context = &trace;
    stars_create_salvage(&input);
    if (trace.race_calls != 1U || trace.owner != 4 || trace.drops != 1U ||
            trace.planet_adds != 0U || trace.dropped[0] != 15UL ||
            trace.dropped[1] != 20UL || trace.dropped[2] != 30UL)
        return 1;
    memset(&trace, 0, sizeof trace);
    input.planet = &fleet; input.planet_depleted = 1U;
    stars_create_salvage(&input);
    if (trace.race_calls != 1U || trace.drops != 0U ||
            trace.planet_adds != 3U || trace.added[0] != 23L ||
            trace.added[1] != 32L || trace.added[2] != 48L)
        return 2;
    puts("CREATE_SALVAGE_SOURCE=PASS");
    return 0;
}
