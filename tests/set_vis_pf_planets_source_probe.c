#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct PlanetProbeLog {
    StarsU16 fleet_id[16];
    StarsI16 fleet_detail[16];
    StarsU16 planet_id[32];
    StarsI16 planet_detail[32];
    StarsI16 progress[8];
    StarsU16 fleet_marks;
    StarsU16 planet_marks;
    StarsU16 progress_calls;
} PlanetProbeLog;

static void probe_mark_fleet(void *opaque, StarsU16 id, StarsI16 detail)
{
    PlanetProbeLog *log = (PlanetProbeLog *)opaque;
    if (log->fleet_marks < 16U) {
        log->fleet_id[log->fleet_marks] = id;
        log->fleet_detail[log->fleet_marks] = detail;
    }
    ++log->fleet_marks;
}

static void probe_mark_planet(void *opaque, StarsU16 id,
    StarsI16 viewer, StarsI16 detail)
{
    PlanetProbeLog *log = (PlanetProbeLog *)opaque;
    (void)viewer;
    if (log->planet_marks < 32U) {
        log->planet_id[log->planet_marks] = id;
        log->planet_detail[log->planet_marks] = detail;
    }
    ++log->planet_marks;
}

static StarsI16 probe_stargate(void *opaque,
    const StarsVisPlanetScanPlanet *planet, StarsI16 viewer,
    StarsI16 design)
{
    (void)opaque;
    (void)viewer;
    if (!planet) return design == 0 ? 12 : 0;
    return planet->id == 3U ? 1 : 0;
}

static void probe_progress(void *opaque, StarsI16 amount)
{
    PlanetProbeLog *log = (PlanetProbeLog *)opaque;
    if (log->progress_calls < 8U)
        log->progress[log->progress_calls] = amount;
    ++log->progress_calls;
}

static int has_fleet_mark(const PlanetProbeLog *log, StarsU16 id,
    StarsI16 detail)
{
    StarsU16 i;
    for (i = 0; i < log->fleet_marks && i < 16U; ++i)
        if (log->fleet_id[i] == id && log->fleet_detail[i] == detail)
            return 1;
    return 0;
}

static int has_planet_mark(const PlanetProbeLog *log, StarsU16 id,
    StarsI16 detail)
{
    StarsU16 i;
    for (i = 0; i < log->planet_marks && i < 32U; ++i)
        if (log->planet_id[i] == id && log->planet_detail[i] == detail)
            return 1;
    return 0;
}

int main(void)
{
    StarsU16 design_flags[10];
    StarsI16 gate_ranges[10];
    StarsU16 details[4];
    StarsVisPlanetScanPlanet planets[4];
    StarsVisFleetScanFleet fleets[4];
    StarsVisThingTarget things[4];
    StarsSetVisPFPlanetsInput input;
    StarsSetVisPFPlanetsOps ops;
    PlanetProbeLog log;
    int failures = 0;

    (void)memset(design_flags, 0, sizeof(design_flags));
    (void)memset(gate_ranges, 0, sizeof(gate_ranges));
    (void)memset(details, 0, sizeof(details));
    (void)memset(planets, 0, sizeof(planets));
    (void)memset(fleets, 0, sizeof(fleets));
    (void)memset(things, 0, sizeof(things));
    (void)memset(&input, 0, sizeof(input));
    (void)memset(&ops, 0, sizeof(ops));
    (void)memset(&log, 0, sizeof(log));

    planets[0].id = 1U; planets[0].x = 0; planets[0].y = 0;
    planets[0].owner = 0; planets[0].flags = 0x0200U;
    planets[0].starbase_slot = 0U; planets[0].scanner_range = 5;
    planets[0].deep_range = 8;
    planets[1].id = 2U; planets[1].x = 6; planets[1].y = 0;
    planets[1].owner = 1; planets[1].flags = 0x0200U;
    planets[1].starbase_count = 5000UL;
    planets[2].id = 3U; planets[2].x = 10; planets[2].y = 0;
    planets[2].owner = 1; planets[2].flags = 0x0200U;
    planets[2].starbase_count = 10000UL;
    planets[3].id = 4U; planets[3].x = 2; planets[3].y = 0;

    fleets[0].id = 10U; fleets[0].x = 3; fleets[0].y = 4;
    fleets[0].planet_id = -1;
    fleets[1].id = 11U; fleets[1].x = 2; fleets[1].y = 1;
    fleets[1].planet_id = 2; fleets[1].cloak_percent = 0;
    fleets[2].id = 12U; fleets[2].x = 4; fleets[2].y = 4;
    fleets[2].planet_id = 2; fleets[2].cloak_percent = 50;
    fleets[3].id = 13U; fleets[3].x = 1; fleets[3].y = 1;
    fleets[3].flags = 0x0100U;

    things[0].kind = 1U; things[0].owner = 1; things[0].x = 2;
    things[1].kind = 3U; things[1].x = 3;
    things[2].kind = 0U; things[2].owner = 2; things[2].x = 4;
    things[3].kind = 2U; things[3].x = 4;

    ops.mark_fleet = probe_mark_fleet;
    ops.mark_planet = probe_mark_planet;
    ops.stargate_range = probe_stargate;
    ops.progress = probe_progress;
    input.viewer = 0;
    input.race_stat = 7;
    input.starbase_design_flags = design_flags;
    input.stargate_ranges = gate_ranges;
    input.player_detail_flags = details;
    input.player_count = 4U;
    input.planets = planets;
    input.planet_count = 4U;
    input.fleets = fleets;
    input.fleet_count = 4U;
    input.things = things;
    input.thing_count = 4U;

    stars_set_vis_pf_planets(&input, &ops, &log);
    if (!has_planet_mark(&log, 2U, 2)) ++failures;
    if (!has_planet_mark(&log, 3U, 3)) ++failures;
    if (!has_planet_mark(&log, 4U, 3)) ++failures;
    if (!has_fleet_mark(&log, 10U, 3)) ++failures;
    if (!has_fleet_mark(&log, 11U, 3)) ++failures;
    if (has_fleet_mark(&log, 12U, 3)) ++failures;
    if ((things[0].dest_x & 0x8000U) == 0U ||
            (things[1].bits_4 & 0x0010UL) == 0UL ||
            (things[2].bits_8 & 1UL) == 0UL ||
            (things[3].bits_2 & 1UL) == 0UL ||
            (things[3].dest_x & 0x2000U) == 0U) ++failures;
    if ((details[1] & 0x0100U) == 0U ||
            (details[1] & 7U) != 3U) ++failures;
    if (log.progress_calls != 4U || log.progress[0] != -0x39f ||
            log.progress[3] != -0x39f) ++failures;

    if (failures == 0)
        puts("SET_VIS_PF_PLANETS_SOURCE=PASS");
    else
        puts("SET_VIS_PF_PLANETS_SOURCE=FAIL");
    return failures;
}
