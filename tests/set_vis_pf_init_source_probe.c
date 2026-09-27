#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsU16 marks; StarsU16 detail; StarsU16 mine_calls; } Trace;

static void mark_planet(void *context, StarsU16 id, StarsI16 viewer,
    StarsI16 detail)
{
    Trace *trace = (Trace *)context;
    if (id != 4U || viewer != 1 || detail != 4) trace->detail = 0xffffU;
    ++trace->marks;
}

static StarsI16 scanner(void *context, const StarsVisPFFleet *fleet,
    StarsU16 *steal)
{
    (void)context;
    *steal = fleet->scanner_steal;
    return fleet->scanner_result;
}

static StarsI32 mine(void *context, const StarsVisPFFleet *fleet)
{ Trace *trace = (Trace *)context; ++trace->mine_calls; return fleet->mine_strength; }

int main(void)
{
    StarsVisPFPlayer players[2];
    StarsVisPFPlanet planets[1];
    StarsVisPFFleet fleets[1];
    StarsVisPFThing things[4];
    StarsVisPFInitInput input;
    StarsVisPFInitOps ops;
    Trace trace;
    memset(players, 0, sizeof players);
    memset(planets, 0, sizeof planets);
    memset(fleets, 0, sizeof fleets);
    memset(things, 0, sizeof things);
    memset(&trace, 0, sizeof trace);
    memset(&ops, 0, sizeof ops);
    players[1].detail_flags = 0x0091U;
    players[1].fleet_count = 0x7009U; players[1].planet_count = 42U;
    players[1].design_flags[2] = 0x0200U;
    planets[0].id = 4U; planets[0].player = 1; planets[0].flags = 0x0200U;
    planets[0].starbase_slot = 2U;
    fleets[0].player = 1; fleets[0].planet_id = 0;
    fleets[0].scanner_result = -1; fleets[0].scanner_steal = 2U;
    fleets[0].ship_counts[3] = 5U;
    things[0].kind = 1U; things[0].owner = 0;
    things[1].kind = 2U; things[1].dest_x = 0xffffU;
    things[2].kind = 3U;
    things[3].kind = 0U; things[3].owner = 0; things[3].target_mask = 2UL;
    ops.mark_planet = mark_planet; ops.scanner = scanner;
    ops.mine_from_fleet = mine;
    input.viewer = 1; input.race_stat = 6; input.players = players;
    input.player_count = 2U; input.planets = planets; input.planet_count = 1U;
    input.fleets = fleets; input.fleet_count = 1U;
    input.things = things; input.thing_count = 4U;
    stars_set_vis_pf_init(&input, &ops, &trace);
    if (players[1].detail_flags != 0x0197U || players[1].fleet_count != 0xa001U || players[1].planet_count != 1U ||
            trace.mine_calls != 0U || trace.marks != 1U || trace.detail == 0xffffU ||
            (players[1].detail_flags & 0x0100U) == 0U ||
            players[1].design_built[3] != 5UL ||
            (fleets[0].flags & 0x0100U) == 0U ||
            (things[0].dest_x & 0x8000U) == 0U ||
            (things[1].dest_x & 0x2000U) != 0U ||
            (things[2].flags & 0x0010U) == 0U) return 1;
    memset(&trace, 0, sizeof trace);
    planets[0].player = -1; fleets[0].mine_special = 1U;
    fleets[0].mine_strength = 1L; fleets[0].scanner_steal = 0U;
    stars_set_vis_pf_init(&input, &ops, &trace);
    if (trace.mine_calls != 1U || trace.marks != 1U || trace.detail == 0xffffU) return 3;
    memset(&trace, 0, sizeof trace);
    input.viewer = -1;
    stars_set_vis_pf_init(&input, &ops, &trace);
    if (things[1].dest_x != 0xffffU || trace.marks != 0U) return 2;
    puts("SET_VIS_PF_INIT_SOURCE=PASS");
    return 0;
}
