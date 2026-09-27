#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsU16 fleets; StarsU16 planets; StarsU16 bad; } Trace;

static void mark_fleet(void *context, StarsU16 id, StarsI16 detail)
{
    Trace *t = (Trace *)context;
    if (id != 9U || detail != 3) ++t->bad;
    ++t->fleets;
}
static void mark_planet(void *context, StarsU16 id, StarsI16 viewer,
    StarsI16 detail)
{
    Trace *t = (Trace *)context;
    if (id != 4U || viewer != 1 || detail != 3) ++t->bad;
    ++t->planets;
}
static StarsI16 cloak(void *context, const StarsVisThingFleet *fleet)
{ (void)context; return fleet->cloak_percent; }
static StarsI16 random_value(void *context, StarsI16 limit)
{ (void)context; (void)limit; return 99; }

int main(void)
{
    StarsU16 details[2];
    StarsVisThingTarget things[2];
    StarsVisThingFleet fleets[1];
    StarsVisThingPlanet planets[1];
    StarsSetVisPFThingsInput input;
    StarsSetVisPFThingsOps ops;
    Trace trace;
    memset(details, 0, sizeof details);
    memset(things, 0, sizeof things);
    memset(fleets, 0, sizeof fleets);
    memset(planets, 0, sizeof planets);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    things[0].kind = 1U; things[0].owner = 1; things[0].x = 0;
    things[0].y = 0; things[0].dest_x = 4U << 10;
    things[1].kind = 0U; things[1].owner = 1; things[1].x = 1; things[1].y = 1;
    fleets[0].id = 9U; fleets[0].x = 1; fleets[0].y = 1;
    planets[0].id = 4U; planets[0].x = 1; planets[0].y = 1; planets[0].owner = -1;
    ops.mark_fleet = mark_fleet; ops.mark_planet = mark_planet;
    ops.cloak_percent = cloak; ops.random_value = random_value;
    input.viewer = 1; input.race_stat = 6; input.player_detail_flags = details;
    input.player_count = 2U; input.things = things; input.thing_count = 2U;
    input.fleets = fleets; input.fleet_count = 1U;
    input.planets = planets; input.planet_count = 1U;
    stars_set_vis_pf_things(&input, &ops, &trace);
    if (trace.fleets != 1U || trace.planets != 1U || trace.bad != 0U ||
            (things[0].dest_x & 0x8000U) == 0U ||
            (things[1].bits_8 & 2UL) == 0UL) return 1;
    memset(&trace, 0, sizeof trace);
    input.race_stat = 5;
    things[0].kind = 0U; things[0].owner = 1; things[0].range_squared = 9UL;
    things[1].owner = 0;
    fleets[0].planet_id = -1; fleets[0].cloak_percent = 50;
    stars_set_vis_pf_things(&input, &ops, &trace);
    if (trace.fleets != 1U || trace.bad != 0U) return 2;
    puts("SET_VIS_PF_THINGS_SOURCE=PASS");
    return 0;
}
