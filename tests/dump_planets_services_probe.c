#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct ServiceTrace {
    StarsI16 callback_calls;
    StarsI16 callback_before_row;
    StarsI16 saw_id;
    StarsU16 saw_size;
    StarsI16 row_calls;
    StarsDumpPlanetRow row;
} ServiceTrace;

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static void put32(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    put16(raw, offset, (StarsU16)(value & 0xffffUL));
    put16(raw, (StarsU16)(offset + 2), (StarsU16)(value >> 16));
}

static StarsI16 open_report(void *context, const char *name,
    StarsI16 detailed)
{
    (void)context;
    (void)name;
    (void)detailed;
    return 1;
}

static StarsI16 write_header(void *context, StarsI16 number)
{
    (void)context;
    (void)number;
    return 1;
}

static StarsI16 write_row(void *context, const StarsDumpPlanetRow *row)
{
    ServiceTrace *trace;
    trace = (ServiceTrace *)context;
    trace->callback_before_row = trace->callback_calls == 1;
    trace->row = *row;
    ++trace->row_calls;
    return 1;
}

static void close_report(void *context)
{
    (void)context;
}

static void resolve_services(void *context, const StarsU8 *record,
    StarsU16 size, StarsDumpPlanet *planet)
{
    ServiceTrace *trace;
    trace = (ServiceTrace *)context;
    ++trace->callback_calls;
    trace->saw_id = planet->id;
    trace->saw_size = size;
    if (record[0] != 4 || record[1] != 0)
        trace->saw_id = -1;
    planet->environment[0] = 61;
    planet->mineral_rate[2] = 303L;
    planet->resources = 701L;
    planet->survival_percent = 66L;
    planet->scanner_detail = 9;
    planet->starbase_id = 12;
    planet->starbase_level = 4;
    planet->wormhole_id = 13;
    planet->wormhole_distance = 8;
    planet->mine_count = 17;
}

int main(void)
{
    static StarsU8 raw[56];
    static StarsDumpPlanetsPackedInput input;
    static ServiceTrace trace;
    StarsU32 weights;
    int checks;
    int failures;

    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    memset(raw, 0, sizeof raw);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    put16(raw, 0, 4);
    put16(raw, 2, 2);
    put16(raw, 4, 0x0207U);
    put16(raw, 18, 1200);
    raw[9] = 70;
    raw[10] = 80;
    raw[11] = 90;
    raw[12] = 44;
    raw[13] = 45;
    raw[14] = 46;
    raw[15] = 51;
    raw[16] = 52;
    raw[17] = 53;
    put32(raw, 28, 11);
    put32(raw, 32, 22);
    put32(raw, 36, 33);
    weights = (22UL << 20) | (11UL << 8);
    put32(raw, 20, weights);
    put32(raw, 24, 12UL << 12);
    put16(raw, 46, (StarsU16)(8U | (3U << 10)));
    put16(raw, 48, (StarsU16)(5U | (7U << 10)));
    put16(raw, 50, 17);
    input.planet_record = raw;
    input.planet_size = (StarsU16)sizeof raw;
    input.resources = 99;
    input.survival_percent = 88;
    input.scanner_detail = 6;
    input.resolve_services = resolve_services;
    input.report.file_name = "SERVICES.PLA";
    input.report.game_active = 1;
    input.report.viewer_player = 2;
    input.report.game_turn = 20;
    input.report.detailed = 1;
    input.report.open_report = open_report;
    input.report.write_header = write_header;
    input.report.write_row = write_row;
    input.report.close_report = close_report;
    input.report.context = &trace;
    CHECK(stars_dump_planets_packed(&input) == 1);
    CHECK(trace.callback_calls == 1);
    CHECK(trace.saw_id == 4 && trace.saw_size == 56);
    CHECK(trace.callback_before_row == 1 && trace.row_calls == 1);
    CHECK(trace.row.environment[0] == 61);
    CHECK(trace.row.mineral_rate[2] == 303L);
    CHECK(trace.row.resources == 701L);
    CHECK(trace.row.survival_percent == 66L);
    CHECK(trace.row.scanner_detail == 9);
    CHECK(trace.row.starbase_id == 12 && trace.row.starbase_level == 4);
    CHECK(trace.row.wormhole_id == 13 && trace.row.wormhole_distance == 8);
    CHECK(trace.row.mine_count == 17);
    printf("DUMP_PLANETS_SERVICES_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
#undef CHECK
}
