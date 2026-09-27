#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 opened;
    StarsI16 headers;
    StarsI16 rows;
    StarsI16 closed;
    StarsDumpPlanetRow row;
} Trace;

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
    Trace *trace;
    (void)name;
    (void)detailed;
    trace = (Trace *)context;
    ++trace->opened;
    return 1;
}

static StarsI16 header(void *context, StarsI16 number)
{
    (void)number;
    ++((Trace *)context)->headers;
    return 1;
}

static StarsI16 row(void *context, const StarsDumpPlanetRow *value)
{
    Trace *trace;
    trace = (Trace *)context;
    trace->row = *value;
    ++trace->rows;
    return 1;
}

static void close_report(void *context)
{
    ++((Trace *)context)->closed;
}

int main(void)
{
    static StarsU8 raw[56];
    static StarsDumpPlanetsPackedInput input;
    static Trace trace;
    StarsU32 weights;

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
    input.report.file_name = "PACKED.PLA";
    input.report.game_active = 1;
    input.report.viewer_player = 2;
    input.report.game_turn = 20;
    input.report.detailed = 1;
    input.report.open_report = open_report;
    input.report.write_header = header;
    input.report.write_row = row;
    input.report.close_report = close_report;
    input.report.context = &trace;
    if (sizeof raw != STARS_DUMP_PLANET_RECORD_SIZE ||
            !stars_dump_planets_packed(&input) || trace.opened != 1 ||
            trace.headers != 3 || trace.rows != 1 || trace.closed != 1 ||
            trace.row.id != 4 || !trace.row.owner_visible ||
            trace.row.turns_since_visit != 3 ||
            trace.row.environment[0] != 44 ||
            trace.row.environment_original[0] != 51 ||
            trace.row.mineral_concentration[0] != 70 ||
            trace.row.mineral_rate[0] != 11 ||
            trace.row.production != 22 || trace.row.resources != 99 ||
            trace.row.starbase_id != 8 || trace.row.wormhole_id != 5 ||
            trace.row.owner != 2 || trace.row.flags != 0x0207U ||
            trace.row.environment[1] != 45 || trace.row.environment[2] != 46 ||
            trace.row.mineral_concentration[1] != 80 ||
            trace.row.mineral_concentration[2] != 90 ||
            trace.row.mineral_rate[1] != 22 || trace.row.mineral_rate[2] != 33 ||
            trace.row.scanner_detail != 6 || trace.row.starbase_level != 3 ||
            trace.row.wormhole_distance != 7 || trace.row.mine_count != 11)
        return 2;
    puts("DUMP_PLANETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS");
    return 0;
}
