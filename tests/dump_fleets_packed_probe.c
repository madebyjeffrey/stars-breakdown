#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 opened;
    StarsI16 headers;
    StarsI16 rows;
    StarsI16 closed;
    StarsDumpFleetRow row;
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
    (void)name;
    (void)detailed;
    ++((Trace *)context)->opened;
    return 1;
}

static StarsI16 header(void *context, StarsI16 number)
{
    (void)number;
    ++((Trace *)context)->headers;
    return 1;
}

static StarsI16 row(void *context, const StarsDumpFleetRow *value)
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
    static StarsU8 raw[124];
    static StarsDumpFleetsPackedInput input;
    static Trace trace;
    StarsI16 index;

    memset(raw, 0, sizeof raw);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    put16(raw, 0, 10);
    put16(raw, 2, 2);
    put16(raw, 4, 7);
    put16(raw, 6, 17);
    put16(raw, 8, 1100);
    put16(raw, 10, 1200);
    put16(raw, 12, 3);
    put16(raw, 20, 2);
    put16(raw, 44, 700);
    put16(raw, 98, 3);
    put16(raw, 118, 4);
    for (index = 0; index < 5; ++index)
        put32(raw, (StarsU16)(76 + index * 4), (StarsU32)(10 + index));
    input.fleet_record = raw;
    input.fleet_size = (StarsU16)sizeof raw;
    input.eta = 4;
    input.cloak_percent = 25;
    input.scanner_range = 12;
    input.scanner_detail = 6;
    input.destination_id = 17;
    input.destination_visible = 1;
    input.destination_requires_detail = 0;
    input.plan_id = 3;
    input.mines = 10;
    input.mine_sweep = 20;
    input.mine_laying = 30;
    input.terraforming = 40;
    input.report.file_name = "PACKED.FLE";
    input.report.game_active = 1;
    input.report.viewer_player = 2;
    input.report.detailed = 1;
    input.report.open_report = open_report;
    input.report.write_header = header;
    input.report.write_row = row;
    input.report.close_report = close_report;
    input.report.context = &trace;
    if (sizeof raw != STARS_DUMP_FLEET_RECORD_SIZE ||
            !stars_dump_fleets_packed(&input) || trace.opened != 1 ||
            trace.headers != 3 || trace.rows != 1 || trace.closed != 1 ||
            trace.row.id != 10 || trace.row.owner != 2 ||
            trace.row.x != 1100 || trace.row.y != 1200 ||
            trace.row.ship_total != 5 || trace.row.ship_class_total[0] != 3 ||
            trace.row.ship_class_total[2] != 2 || trace.row.cargo[0] != 10 ||
            trace.row.player_number != 3 || trace.row.eta != 4 ||
            trace.row.weight != 700 || trace.row.destination_visible != 1 ||
            trace.row.ship_class_total[1] != 0 ||
            trace.row.ship_class_total[3] != 0 ||
            trace.row.ship_other_total != 0 || trace.row.cargo[1] != 11 ||
            trace.row.cargo[2] != 12 || trace.row.cargo[3] != 13 ||
            trace.row.cargo[4] != 14 || trace.row.plan_id != 3 ||
            trace.row.warp != 4 || trace.row.cloak_percent != 25 ||
            trace.row.scanner_range != 12 || trace.row.scanner_detail != 6 ||
            trace.row.task_id != 3 || trace.row.mines != 10 ||
            trace.row.mine_sweep != 20 || trace.row.mine_laying != 30 ||
            trace.row.terraforming != 40)
        return 2;
    memset(&trace, 0, sizeof trace);
    input.report.detailed = 0;
    if (!stars_dump_fleets_packed(&input) || trace.row.player_number != 0 ||
            trace.row.eta != 0 || trace.row.warp != 0 ||
            trace.row.weight != 0 || trace.row.cloak_percent != 0 ||
            trace.row.scanner_range != 0 || trace.row.scanner_detail != 0 ||
            trace.row.task_id != 0 || trace.row.mines != 0 ||
            trace.row.mine_sweep != 0 || trace.row.mine_laying != 0 ||
            trace.row.terraforming != 0)
        return 3;
    input.destination_requires_detail = 1;
    if (!stars_dump_fleets_packed(&input) || trace.row.destination_visible != 0)
        return 4;
    puts("DUMP_FLEETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS");
    return 0;
}
