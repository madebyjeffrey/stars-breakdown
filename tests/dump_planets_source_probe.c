#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 opened;
    StarsI16 headers;
    StarsI16 rows;
    StarsI16 closed;
    StarsI16 fail_row;
    StarsDumpPlanetRow first;
    StarsDumpPlanetRow last;
} Trace;

static StarsI16 open_report(void *context, const char *name,
    StarsI16 detailed)
{
    Trace *trace;
    (void)name;
    (void)detailed;
    trace = (Trace *)context;
    ++trace->opened;
    return 1;
}

static StarsI16 header(void *context, StarsI16 number)
{
    Trace *trace;
    (void)number;
    trace = (Trace *)context;
    ++trace->headers;
    return 1;
}

static StarsI16 row(void *context, const StarsDumpPlanetRow *value)
{
    Trace *trace;
    trace = (Trace *)context;
    if (trace->fail_row && value->id == trace->fail_row)
        return 0;
    if (trace->rows == 0)
        trace->first = *value;
    trace->last = *value;
    ++trace->rows;
    return 1;
}

static void close_report(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->closed;
}

int main(void)
{
    StarsDumpPlanet planets[2];
    StarsDumpPlanetsInput input;
    Trace trace;

    memset(planets, 0, sizeof planets);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    planets[0].id = 4;
    planets[0].owner = 2;
    planets[0].flags = STARS_DUMP_PLANET_FLAG_INHABITED;
    planets[0].turn = 17;
    planets[0].population = 1200;
    planets[0].environment[0] = 44;
    planets[0].environment_original[0] = 51;
    planets[0].mineral_concentration[0] = 700;
    planets[0].mineral_rate[0] = 11;
    planets[0].production = 22;
    planets[0].survival_percent = 88;
    planets[0].resources = 99;
    planets[0].scanner_range = 12;
    planets[0].starbase_id = 8;
    planets[1].id = 5;
    planets[1].owner = -1;
    planets[1].flags = STARS_DUMP_PLANET_FLAG_SCANNED;
    planets[1].turn = 19;
    input.file_name = "REPORT.PLA";
    input.game_active = 1;
    input.viewer_player = 2;
    input.game_turn = 20;
    input.planets = planets;
    input.planet_count = 2;
    input.open_report = open_report;
    input.write_header = header;
    input.write_row = row;
    input.close_report = close_report;
    input.context = &trace;
    if (!stars_dump_planets(&input) || trace.opened != 1 ||
            trace.headers != 2 || trace.rows != 2 || trace.closed != 1 ||
            trace.last.id != 5 || !trace.last.owner_visible ||
            trace.first.owner_visible ||
            trace.last.turns_since_visit != 1 ||
            trace.first.environment[0] != 0 ||
            trace.first.mineral_concentration[0] != 0 ||
            trace.first.starbase_id != -1)
        return 1;
    input.detailed = 1;
    planets[0].flags = (StarsU16)(STARS_DUMP_PLANET_FLAG_INHABITED |
        STARS_DUMP_PLANET_FLAG_SCANNED);
    trace.headers = 0;
    trace.rows = 0;
    if (!stars_dump_planets(&input) || trace.headers != 3 || trace.rows != 2 ||
            trace.first.environment[0] != 44 ||
            trace.first.mineral_concentration[0] != 700 ||
            trace.first.mineral_rate[0] != 11 ||
            trace.first.production != 22 ||
            trace.first.survival_percent != 88 ||
            trace.first.resources != 99 || trace.first.starbase_id != 8)
        return 2;
    planets[0].flags = 3;
    planets[0].production = 33;
    planets[0].resources = 44;
    planets[0].mineral_concentration[0] = 55;
    planets[0].mineral_rate[0] = 66;
    trace.rows = 0;
    trace.fail_row = 0;
    if (!stars_dump_planets(&input) || trace.first.production != 0 ||
            trace.first.resources != 0 ||
            trace.first.mineral_concentration[0] != 55 ||
            trace.first.mineral_rate[0] != 0)
        return 4;
    trace.fail_row = 5;
    if (stars_dump_planets(&input) || trace.closed != 4)
        return 5;
    puts("DUMP_PLANETS_CHECKS=16 FAILURES=0 RESULT=PASS");
    return 0;
}
