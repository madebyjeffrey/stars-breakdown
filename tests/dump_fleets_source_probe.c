#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 opened;
    StarsI16 headers;
    StarsI16 rows;
    StarsI16 closed;
    StarsI16 fail_row;
    StarsDumpFleetRow first;
    StarsDumpFleetRow last;
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

static StarsI16 row(void *context, const StarsDumpFleetRow *value)
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
    StarsDumpFleet fleets[2];
    StarsDumpFleetsInput input;
    Trace trace;

    memset(fleets, 0, sizeof fleets);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleets[0].id = 10;
    fleets[0].owner = 2;
    fleets[0].x = 1100;
    fleets[0].y = 1200;
    fleets[0].destination_id = 17;
    fleets[0].destination_visible = 1;
    fleets[0].ship_count[0] = 3;
    fleets[0].ship_count[4] = 2;
    fleets[0].ship_class[0] = 2;
    fleets[0].ship_class[4] = 4;
    fleets[0].eta = 4;
    fleets[0].player_number = 2;
    fleets[0].weight = 700;
    fleets[0].cloak_percent = 25;
    fleets[1].id = 11;
    fleets[1].owner = 4;
    fleets[1].destination_id = 19;
    fleets[1].destination_visible = 1;
    fleets[1].destination_requires_detail = 1;
    fleets[1].ship_count[2] = 1;
    fleets[1].ship_class[2] = 0;
    fleets[1].eta = 6;
    input.file_name = "REPORT.FLE";
    input.game_active = 1;
    input.viewer_player = 2;
    input.fleets = fleets;
    input.fleet_count = 2;
    input.open_report = open_report;
    input.write_header = header;
    input.write_row = row;
    input.close_report = close_report;
    input.context = &trace;
    if (!stars_dump_fleets(&input) || trace.opened != 1 ||
            trace.headers != 2 || trace.rows != 2 || trace.closed != 1 ||
            trace.last.id != 11 || trace.last.ship_total != 1 ||
            trace.last.ship_other_total != 1 ||
            trace.last.destination_visible || trace.first.player_number != 0)
        return 1;
    input.detailed = 1;
    trace.headers = 0;
    trace.rows = 0;
    if (!stars_dump_fleets(&input) || trace.headers != 3 || trace.rows != 2 ||
            trace.first.ship_class_total[0] != 3 ||
            trace.first.ship_class_total[2] != 2 ||
            !trace.last.destination_visible || trace.last.eta != 6 ||
            trace.first.player_number != 2)
        return 2;
    trace.fail_row = 11;
    if (stars_dump_fleets(&input) || trace.closed != 3)
        return 3;
    puts("DUMP_FLEETS_CHECKS=10 FAILURES=0 RESULT=PASS");
    return 0;
}
