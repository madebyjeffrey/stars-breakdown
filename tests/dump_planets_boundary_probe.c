#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 opened;
    StarsI16 headers;
    StarsI16 rows;
    StarsI16 closed;
    StarsI16 open_result;
    StarsI16 header_fail;
    StarsI16 row_fail;
    StarsDumpPlanetRow last;
} Trace;

static StarsI16 open_report(void *context, const char *name,
    StarsI16 detailed)
{
    Trace *trace = (Trace *)context;
    (void)name;
    (void)detailed;
    ++trace->opened;
    return trace->open_result;
}

static StarsI16 write_header(void *context, StarsI16 number)
{
    Trace *trace = (Trace *)context;
    ++trace->headers;
    return number != trace->header_fail;
}

static StarsI16 write_row(void *context, const StarsDumpPlanetRow *row)
{
    Trace *trace = (Trace *)context;
    trace->last = *row;
    ++trace->rows;
    return row->id != trace->row_fail;
}

static void close_report(void *context)
{
    ++((Trace *)context)->closed;
}

static void setup(StarsDumpPlanetsInput *input, StarsDumpPlanet *planet,
    Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(planet, 0, sizeof *planet);
    memset(trace, 0, sizeof *trace);
    planet->id = 7;
    planet->owner = 2;
    planet->flags = (StarsU16)(STARS_DUMP_PLANET_FLAG_INHABITED |
        STARS_DUMP_PLANET_FLAG_SCANNED);
    planet->turn = 17;
    planet->population = 1200L;
    planet->production = 22L;
    planet->survival_percent = 88L;
    planet->resources = 99L;
    planet->environment[0] = 44;
    planet->environment_original[0] = 51;
    planet->mineral_concentration[0] = 700L;
    planet->mineral_rate[0] = 11L;
    planet->scanner_range = 12;
    planet->scanner_detail = 6;
    planet->starbase_id = 8;
    planet->starbase_level = 3;
    planet->wormhole_id = 5;
    planet->wormhole_distance = 7;
    planet->mine_count = 11;
    trace->open_result = 1;
    trace->header_fail = -1;
    trace->row_fail = -1;
    input->file_name = "BOUNDARY.PLA";
    input->game_active = 1;
    input->viewer_player = 2;
    input->game_turn = 20;
    input->planets = planet;
    input->planet_count = 1;
    input->open_report = open_report;
    input->write_header = write_header;
    input->write_row = write_row;
    input->close_report = close_report;
    input->context = trace;
}

int main(void)
{
    StarsDumpPlanetsInput input;
    StarsDumpPlanet planet;
    Trace trace;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    setup(&input, &planet, &trace);
    input.game_active = 0;
    CHECK(stars_dump_planets(&input) == 0 && trace.opened == 0);
    input.game_active = 1;
    input.viewer_player = -1;
    CHECK(stars_dump_planets(&input) == 0 && trace.opened == 0);
    input.viewer_player = 2;
    input.file_name = 0;
    CHECK(stars_dump_planets(&input) == 0 && trace.opened == 0);

    setup(&input, &planet, &trace);
    trace.open_result = 0;
    CHECK(stars_dump_planets(&input) == 0 && trace.opened == 1 &&
        trace.headers == 0 && trace.closed == 0);

    setup(&input, &planet, &trace);
    trace.header_fail = 1;
    CHECK(stars_dump_planets(&input) == 0 && trace.headers == 2 &&
        trace.rows == 0 && trace.closed == 1);

    setup(&input, &planet, &trace);
    trace.row_fail = 7;
    CHECK(stars_dump_planets(&input) == 0 && trace.headers == 2 &&
        trace.rows == 1 && trace.closed == 1);

    setup(&input, &planet, &trace);
    CHECK(stars_dump_planets(&input) == 1 && trace.headers == 2 &&
        trace.rows == 1 && trace.closed == 1);
    CHECK(trace.last.turns_since_visit == 3 && trace.last.owner_visible == 1);
    CHECK(trace.last.production == 22L && trace.last.resources == 99L &&
        trace.last.scanner_range == 0 && trace.last.starbase_id == -1);
    CHECK(trace.last.environment[0] == 0 &&
        trace.last.mineral_concentration[0] == 0 &&
        trace.last.mineral_rate[0] == 0);

    setup(&input, &planet, &trace);
    input.detailed = 1;
    CHECK(stars_dump_planets(&input) == 1 && trace.headers == 3);
    CHECK(trace.last.environment[0] == 44 &&
        trace.last.environment_original[0] == 51);
    CHECK(trace.last.mineral_concentration[0] == 700L &&
        trace.last.mineral_rate[0] == 11L);
    CHECK(trace.last.production == 22L && trace.last.survival_percent == 88L &&
        trace.last.resources == 99L);
    CHECK(trace.last.scanner_range == 12 && trace.last.scanner_detail == 6 &&
        trace.last.starbase_id == 8 && trace.last.wormhole_id == 5);

    planet.flags = (StarsU16)(3U | STARS_DUMP_PLANET_FLAG_SCANNED);
    memset(&trace, 0, sizeof trace);
    trace.open_result = 1;
    trace.header_fail = -1;
    trace.row_fail = -1;
    input.context = &trace;
    CHECK(stars_dump_planets(&input) == 1);
    CHECK(trace.last.mineral_concentration[0] == 700L &&
        trace.last.environment[0] == 44 && trace.last.mineral_rate[0] == 0);
    CHECK(trace.last.production == 0 && trace.last.resources == 0);

    planet.flags = (StarsU16)(2U | STARS_DUMP_PLANET_FLAG_SCANNED);
    memset(&trace, 0, sizeof trace);
    trace.open_result = 1;
    trace.header_fail = -1;
    trace.row_fail = -1;
    input.context = &trace;
    CHECK(stars_dump_planets(&input) == 1);
    CHECK(trace.last.mineral_concentration[0] == 0 &&
        trace.last.environment[0] == 0 && trace.last.mineral_rate[0] == 0);
    CHECK(trace.last.production == 0 && trace.last.resources == 0);

    planet.flags = STARS_DUMP_PLANET_FLAG_INHABITED;
    memset(&trace, 0, sizeof trace);
    trace.open_result = 1;
    trace.header_fail = -1;
    trace.row_fail = -1;
    input.context = &trace;
    CHECK(stars_dump_planets(&input) == 1 && trace.last.owner_visible == 0);
    CHECK(trace.last.scanner_range == 0 && trace.last.starbase_id == -1 &&
        trace.last.wormhole_id == -1);
    planet.owner = -1;
    memset(&trace, 0, sizeof trace);
    trace.open_result = 1;
    trace.header_fail = -1;
    trace.row_fail = -1;
    input.context = &trace;
    CHECK(stars_dump_planets(&input) == 1 && trace.last.owner_visible == 1);

    printf("DUMP_PLANETS_BOUNDARY_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures != 0;
}
