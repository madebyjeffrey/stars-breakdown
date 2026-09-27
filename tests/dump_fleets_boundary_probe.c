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
    StarsDumpFleetRow last;
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

static StarsI16 write_row(void *context, const StarsDumpFleetRow *row)
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

static void setup(StarsDumpFleetsInput *input, StarsDumpFleet *fleet,
    Trace *trace)
{
    StarsI16 index;

    memset(input, 0, sizeof *input);
    memset(fleet, 0, sizeof *fleet);
    memset(trace, 0, sizeof *trace);
    fleet->id = 7;
    fleet->owner = 2;
    fleet->x = 1100;
    fleet->y = 1200;
    fleet->planet_id = 9;
    fleet->destination_id = 17;
    fleet->destination_visible = 1;
    fleet->destination_requires_detail = 1;
    fleet->plan_id = 3;
    fleet->ship_class[0] = 2;
    fleet->ship_class[1] = 3;
    fleet->ship_class[2] = 4;
    fleet->ship_class[3] = 5;
    fleet->ship_class[4] = 0;
    fleet->ship_class[5] = 6;
    fleet->ship_count[0] = 3;
    fleet->ship_count[1] = 4;
    fleet->ship_count[2] = 5;
    fleet->ship_count[3] = 6;
    fleet->ship_count[4] = 7;
    fleet->ship_count[5] = 8;
    for (index = 0; index < 5; ++index)
        fleet->cargo[index] = (StarsI32)(index + 1);
    fleet->player_number = 2;
    fleet->eta = 4;
    fleet->warp = 8;
    fleet->weight = 700L;
    fleet->cloak_percent = 25;
    fleet->scanner_range = 12;
    fleet->scanner_detail = 6;
    fleet->task_id = 11;
    fleet->mines = 13L;
    fleet->mine_sweep = 14L;
    fleet->mine_laying = 15L;
    fleet->terraforming = 16L;
    trace->open_result = 1;
    trace->header_fail = -1;
    trace->row_fail = -1;
    input->file_name = "BOUNDARY.FLE";
    input->game_active = 1;
    input->viewer_player = 2;
    input->fleets = fleet;
    input->fleet_count = 1;
    input->open_report = open_report;
    input->write_header = write_header;
    input->write_row = write_row;
    input->close_report = close_report;
    input->context = trace;
}

int main(void)
{
    StarsDumpFleetsInput input;
    StarsDumpFleet fleet;
    Trace trace;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    setup(&input, &fleet, &trace);
    input.game_active = 0;
    CHECK(stars_dump_fleets(&input) == 0 && trace.opened == 0);
    input.game_active = 1;
    input.viewer_player = -1;
    CHECK(stars_dump_fleets(&input) == 0 && trace.opened == 0);
    input.viewer_player = 2;
    input.file_name = 0;
    CHECK(stars_dump_fleets(&input) == 0 && trace.opened == 0);

    setup(&input, &fleet, &trace);
    trace.open_result = 0;
    CHECK(stars_dump_fleets(&input) == 0 && trace.opened == 1 &&
        trace.headers == 0 && trace.closed == 0);

    setup(&input, &fleet, &trace);
    trace.header_fail = 1;
    CHECK(stars_dump_fleets(&input) == 0 && trace.headers == 2 &&
        trace.rows == 0 && trace.closed == 1);

    setup(&input, &fleet, &trace);
    trace.row_fail = 7;
    CHECK(stars_dump_fleets(&input) == 0 && trace.headers == 2 &&
        trace.rows == 1 && trace.closed == 1);

    setup(&input, &fleet, &trace);
    CHECK(stars_dump_fleets(&input) == 1 && trace.headers == 2 &&
        trace.rows == 1 && trace.closed == 1);
    CHECK(trace.last.ship_total == 33L && trace.last.ship_other_total == 15L);
    CHECK(trace.last.ship_class_total[0] == 3L &&
        trace.last.ship_class_total[1] == 4L &&
        trace.last.ship_class_total[2] == 5L &&
        trace.last.ship_class_total[3] == 6L);
    CHECK(trace.last.cargo[0] == 1L && trace.last.cargo[4] == 5L);
    CHECK(trace.last.destination_visible == 0 && trace.last.player_number == 0);
    CHECK(trace.last.eta == 0 && trace.last.weight == 0L &&
        trace.last.scanner_range == 0 && trace.last.task_id == 0);
    CHECK(trace.last.mines == 0L && trace.last.terraforming == 0L);

    setup(&input, &fleet, &trace);
    input.detailed = 1;
    CHECK(stars_dump_fleets(&input) == 1 && trace.headers == 3);
    CHECK(trace.last.destination_visible == 1 && trace.last.player_number == 2);
    CHECK(trace.last.eta == 4 && trace.last.warp == 8 &&
        trace.last.weight == 700L && trace.last.cloak_percent == 25);
    CHECK(trace.last.scanner_range == 12 && trace.last.scanner_detail == 6 &&
        trace.last.task_id == 11);
    CHECK(trace.last.mines == 13L && trace.last.mine_sweep == 14L &&
        trace.last.mine_laying == 15L && trace.last.terraforming == 16L);
    CHECK(trace.last.id == 7 && trace.last.owner == 2 && trace.last.x == 1100 &&
        trace.last.y == 1200 && trace.last.planet_id == 9 &&
        trace.last.destination_id == 17 && trace.last.plan_id == 3);

    fleet.destination_visible = 0;
    memset(&trace, 0, sizeof trace);
    trace.open_result = 1;
    trace.header_fail = -1;
    trace.row_fail = -1;
    input.context = &trace;
    CHECK(stars_dump_fleets(&input) == 1 && trace.last.destination_visible == 0);

    printf("DUMP_FLEETS_BOUNDARY_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures != 0;
}
