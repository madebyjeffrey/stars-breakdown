#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct FleetProbeLog {
    StarsU16 fleet_id[16];
    StarsI16 fleet_detail[16];
    StarsU16 planet_id[16];
    StarsI16 planet_viewer[16];
    StarsI16 planet_detail[16];
    StarsU16 fleet_marks;
    StarsU16 planet_marks;
} FleetProbeLog;

static void probe_mark_fleet(void *opaque, StarsU16 id, StarsI16 detail)
{
    FleetProbeLog *log = (FleetProbeLog *)opaque;
    if (log->fleet_marks < 16U) {
        log->fleet_id[log->fleet_marks] = id;
        log->fleet_detail[log->fleet_marks] = detail;
    }
    ++log->fleet_marks;
}

static void probe_mark_planet(void *opaque, StarsU16 id,
    StarsI16 viewer, StarsI16 detail)
{
    FleetProbeLog *log = (FleetProbeLog *)opaque;
    if (log->planet_marks < 16U) {
        log->planet_id[log->planet_marks] = id;
        log->planet_viewer[log->planet_marks] = viewer;
        log->planet_detail[log->planet_marks] = detail;
    }
    ++log->planet_marks;
}

static int has_fleet_mark(const FleetProbeLog *log, StarsU16 id,
    StarsI16 detail)
{
    StarsU16 i;
    for (i = 0; i < log->fleet_marks && i < 16U; ++i)
        if (log->fleet_id[i] == id && log->fleet_detail[i] == detail)
            return 1;
    return 0;
}

static int has_planet_mark(const FleetProbeLog *log, StarsU16 id,
    StarsI16 detail)
{
    StarsU16 i;
    for (i = 0; i < log->planet_marks && i < 16U; ++i)
        if (log->planet_id[i] == id && log->planet_detail[i] == detail)
            return 1;
    return 0;
}

int main(void)
{
    StarsU16 details[4] = { 0U, 0U, 0U, 0U };
    StarsVisFleetScanFleet fleets[6];
    StarsVisThingTarget things[4];
    StarsVisThingPlanet planets[4];
    StarsSetVisPFFleetsInput input;
    StarsSetVisPFFleetsOps ops;
    FleetProbeLog log;
    int failures = 0;

    (void)memset(&log, 0, sizeof(log));
    (void)memset(fleets, 0, sizeof(fleets));
    (void)memset(things, 0, sizeof(things));
    (void)memset(planets, 0, sizeof(planets));

    fleets[0].id = 10U;
    fleets[0].player = 0;
    fleets[0].x = 0;
    fleets[0].y = 0;
    fleets[0].planet_id = 1;
    fleets[0].flags = 0x1000U;
    fleets[0].scanner_range = 10U;
    fleets[0].planet_range = 8;
    fleets[0].detection_percent = 100;
    fleets[0].scanner_bits = 3U;

    fleets[1].id = 11U;
    fleets[1].player = 1;
    fleets[1].x = 0;
    fleets[1].y = 0;
    fleets[1].planet_id = -1;

    fleets[2].id = 12U;
    fleets[2].player = 1;
    fleets[2].x = 3;
    fleets[2].y = 4;
    fleets[2].planet_id = -1;

    fleets[3].id = 13U;
    fleets[3].player = 1;
    fleets[3].x = 6;
    fleets[3].y = 8;
    fleets[3].planet_id = 2;
    fleets[3].cloak_percent = 50;

    fleets[4].id = 14U;
    fleets[4].player = 1;
    fleets[4].x = 100;
    fleets[4].y = 100;
    fleets[4].planet_id = 1;

    fleets[5].id = 15U;
    fleets[5].player = 1;
    fleets[5].x = 1;
    fleets[5].y = 1;
    fleets[5].planet_id = -1;
    fleets[5].flags = 0x0400U;

    planets[0].id = 1U;
    planets[0].x = 0;
    planets[0].y = 0;
    planets[0].owner = 0;
    planets[0].flags = 0U;
    planets[1].id = 2U;
    planets[1].x = 6;
    planets[1].y = 0;
    planets[1].owner = 1;
    planets[1].flags = 0x0200U;
    planets[1].starbase_count = 5000UL;
    planets[1].starbase_delta = 0;
    planets[2].id = 3U;
    planets[2].x = 2;
    planets[2].y = 0;
    planets[2].owner = -1;
    planets[2].flags = 0x0100U | 2U;
    planets[3].id = 4U;
    planets[3].x = 40;
    planets[3].y = 40;

    things[0].kind = 1U;
    things[0].owner = 1;
    things[0].x = 2;
    things[0].y = 0;
    things[1].kind = 3U;
    things[1].x = 4;
    things[1].y = 0;
    things[2].kind = 0U;
    things[2].owner = 2;
    things[2].x = 20;
    things[2].y = 0;
    things[2].concealment_threshold = 500UL;
    things[3].kind = 2U;
    things[3].x = 4;
    things[3].y = 0;

    (void)memset(&ops, 0, sizeof(ops));
    ops.mark_fleet = probe_mark_fleet;
    ops.mark_planet = probe_mark_planet;
    (void)memset(&input, 0, sizeof(input));
    input.viewer = 0;
    input.player_detail_flags = details;
    input.player_count = 4U;
    input.fleets = fleets;
    input.fleet_count = 6U;
    input.things = things;
    input.thing_count = 4U;
    input.planets = planets;
    input.planet_count = 4U;

    stars_set_vis_pf_fleets(&input, &ops, &log);
    if (!has_fleet_mark(&log, 11U, 4)) ++failures;
    if (!has_fleet_mark(&log, 11U, 3)) ++failures;
    if (!has_fleet_mark(&log, 12U, 3)) ++failures;
    if (!has_fleet_mark(&log, 14U, 3)) ++failures;
    if (has_fleet_mark(&log, 13U, 3)) ++failures;
    if (!has_planet_mark(&log, 1U, 4)) ++failures;
    if (!has_planet_mark(&log, 2U, 2)) ++failures;
    if (!has_planet_mark(&log, 3U, 3)) ++failures;
    if ((things[0].dest_x & 0x8000U) == 0U) ++failures;
    if ((things[1].bits_4 & 0x0010UL) == 0UL) ++failures;
    if ((things[2].bits_4 & 1UL) == 0UL ||
            (things[2].bits_8 & 1UL) == 0UL) ++failures;
    if ((things[3].bits_2 & 1UL) == 0UL ||
            (things[3].dest_x & 0x2000U) == 0U) ++failures;
    if ((details[1] & 0x0100U) == 0U || (details[1] & 7U) != 3U)
        ++failures;

    if (failures == 0)
        puts("SET_VIS_PF_FLEETS_SOURCE=PASS");
    else
        puts("SET_VIS_PF_FLEETS_SOURCE=FAIL");
    return failures;
}
