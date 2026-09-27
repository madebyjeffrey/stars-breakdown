#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct ServiceTrace {
    StarsI16 callback_calls;
    StarsI16 callback_before_row;
    StarsI16 saw_id;
    StarsU16 saw_size;
    StarsI16 row_calls;
    StarsDumpFleetRow row;
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

static StarsI16 write_row(void *context, const StarsDumpFleetRow *row)
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
    StarsU16 size, StarsDumpFleet *fleet)
{
    ServiceTrace *trace;
    trace = (ServiceTrace *)context;
    ++trace->callback_calls;
    trace->saw_id = fleet->id;
    trace->saw_size = size;
    if (record[8] != 0x4c || record[9] != 0x04)
        trace->saw_id = -1;
    fleet->eta = 9;
    fleet->cloak_percent = 33;
    fleet->scanner_range = 18;
    fleet->scanner_detail = 7;
    fleet->task_id = 12;
    fleet->destination_id = 44;
    fleet->destination_visible = 1;
    fleet->destination_requires_detail = 1;
    fleet->plan_id = 5;
    fleet->mines = 101L;
    fleet->mine_sweep = 102L;
    fleet->mine_laying = 103L;
    fleet->terraforming = 104L;
}

int main(void)
{
    static StarsU8 raw[124];
    static StarsDumpFleetsPackedInput input;
    static ServiceTrace trace;
    StarsI16 index;
    int checks;
    int failures;

    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
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
    input.resolve_services = resolve_services;
    input.report.file_name = "SERVICES.FLE";
    input.report.game_active = 1;
    input.report.viewer_player = 2;
    input.report.detailed = 1;
    input.report.open_report = open_report;
    input.report.write_header = write_header;
    input.report.write_row = write_row;
    input.report.close_report = close_report;
    input.report.context = &trace;
    CHECK(stars_dump_fleets_packed(&input) == 1);
    CHECK(trace.callback_calls == 1);
    CHECK(trace.saw_id == 10 && trace.saw_size == 124);
    CHECK(trace.callback_before_row == 1 && trace.row_calls == 1);
    CHECK(trace.row.eta == 9 && trace.row.cloak_percent == 33);
    CHECK(trace.row.scanner_range == 18 && trace.row.scanner_detail == 7);
    CHECK(trace.row.task_id == 12 && trace.row.destination_id == 44);
    CHECK(trace.row.destination_visible == 1 && trace.row.plan_id == 5);
    CHECK(trace.row.mines == 101L && trace.row.mine_sweep == 102L);
    CHECK(trace.row.mine_laying == 103L && trace.row.terraforming == 104L);
    CHECK(trace.row.ship_total == 5 && trace.row.cargo[4] == 14L);
    CHECK(trace.row.x == 1100 && trace.row.y == 1200);
    printf("DUMP_FLEETS_SERVICES_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
#undef CHECK
}
