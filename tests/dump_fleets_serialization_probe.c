#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 fields;
    StarsI16 ends;
    StarsI16 sequence_bad;
    StarsI16 detail_values_bad;
    StarsI16 formatted_fields;
    StarsI16 formatted_index;
    StarsI16 formatted_ends;
    StarsI16 format_bad;
    StarsI16 original_oracle;
    StarsI16 line_length;
    char line[1024];
    StarsDumpFleetRow row;
} Trace;

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
    Trace *trace;

    trace = (Trace *)context;
    if (trace->original_oracle && number == 0) {
        strcpy(trace->line,
            "Fleet Name\tX\tY\tPlanet\tDestination\tBattle Plan\t"
            "Ship Cnt\tIron\tBora\tGerm\tCol\tFuel\r\n");
        trace->line_length = (StarsI16)strlen(trace->line);
    }
    (void)context;
    return 1;
}

static StarsI16 write_field(void *context, StarsI16 field,
    const StarsDumpFleetRow *row)
{
    Trace *trace;
    trace = (Trace *)context;
    if (field != trace->fields)
        trace->sequence_bad = 1;
    if (field == 6 && row->ship_total != 8L)
        trace->detail_values_bad = 1;
    if (field == 12 && row->player_number != 4)
        trace->detail_values_bad = 1;
    if (field == 19 && row->task_id != 12)
        trace->detail_values_bad = 1;
    if (field == 24 && row->ship_other_total != 1L)
        trace->detail_values_bad = 1;
    if (field == 28 && row->ship_class_total[3] != 4L)
        trace->detail_values_bad = 1;
    trace->row = *row;
    ++trace->fields;
    return 1;
}

static StarsI16 write_end(void *context)
{
    ++((Trace *)context)->ends;
    return 1;
}

static StarsI16 copy_text(StarsU8 *output, StarsU16 capacity,
    const char *text)
{
    StarsU16 length;

    length = (StarsU16)strlen(text);
    if (length >= capacity)
        return -1;
    strcpy((char *)output, text);
    return (StarsI16)length;
}

static StarsI16 format_text(void *context, StarsI16 field,
    const StarsDumpFleetRow *row, StarsU8 *output, StarsU16 capacity)
{
    Trace *trace;
    char number[32];

    trace = (Trace *)context;
    if (field == trace->formatted_index)
        ;
    else
        trace->format_bad = 1;
    if (field == 0) {
        if (row->id == 8)
            return copy_text(output, capacity,
                "Humanoid Long Range Scout #2");
        if (row->id == 9)
            return copy_text(output, capacity,
                "Humanoid Santa Maria #3");
        if (row->id == 10)
            return copy_text(output, capacity,
                "Humanoid Teamster #4");
        if (row->id == 11)
            return copy_text(output, capacity,
                "Humanoid Stalwart Defender #5");
        if (row->id == 12)
            return copy_text(output, capacity,
                "Humanoid Cotton Picker #6");
        return copy_text(output, capacity, "Humanoid Armed Probe #1");
    }
    if (field == 1)
        return copy_text(output, capacity, "1464");
    if (field == 2)
        return copy_text(output, capacity, "1442");
    if (field == 3)
        return copy_text(output, capacity, "Oop Be Gone");
    if (field == 4)
        return copy_text(output, capacity, "-- ");
    if (field == 5)
        return copy_text(output, capacity, "Default");
    if (field == 19)
        return copy_text(output, capacity, "Task 12");
    if (field == 16)
        return copy_text(output, capacity, "33%");
    if (field == 17)
        return copy_text(output, capacity, "18");
    if (field == 18)
        return copy_text(output, capacity, "7");
    if (field == 6)
        sprintf(number, "%ld", (long)row->ship_total);
    else if (field >= 7 && field <= 11)
        sprintf(number, "%ld", (long)row->cargo[field - 7]);
    else if (field == 12)
        sprintf(number, "%d", row->player_number);
    else if (field == 13)
        sprintf(number, "%d", row->eta);
    else if (field == 14)
        sprintf(number, "%d", row->warp);
    else if (field == 15)
        sprintf(number, "%ld", (long)row->weight);
    else if (field == 20)
        sprintf(number, "%ld", (long)row->mines);
    else if (field == 21)
        sprintf(number, "%ld", (long)row->mine_sweep);
    else if (field == 22)
        sprintf(number, "%ld", (long)row->mine_laying);
    else if (field == 23)
        sprintf(number, "%ld", (long)row->terraforming);
    else if (field == 24)
        sprintf(number, "%ld", (long)row->ship_other_total);
    else if (field >= 25 && field <= 28)
        sprintf(number, "%ld", (long)row->ship_class_total[field - 25]);
    else
        return -1;
    return copy_text(output, capacity, number);
}

static StarsI16 write_text_value(void *context, StarsI16 field,
    const StarsU8 *text, StarsU16 length)
{
    Trace *trace;

    trace = (Trace *)context;
    if (field != trace->formatted_index)
        trace->format_bad = 1;
    if ((StarsI16)(trace->line_length + (field != 0 ? 1 : 0) + length) >=
            (StarsI16)sizeof trace->line - 1)
        return 0;
    if (field != 0)
        trace->line[trace->line_length++] = '\t';
    memcpy(trace->line + trace->line_length, text, length);
    trace->line_length = (StarsI16)(trace->line_length + length);
    trace->line[trace->line_length] = 0;
    ++trace->formatted_fields;
    ++trace->formatted_index;
    return 1;
}

static StarsI16 write_text_end(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    trace->line[trace->line_length++] = '\r';
    trace->line[trace->line_length++] = '\n';
    trace->line[trace->line_length] = 0;
    ++trace->formatted_ends;
    trace->formatted_index = 0;
    return 1;
}

static void close_report(void *context)
{
    (void)context;
}

static void setup(StarsDumpFleetsInput *input, StarsDumpFleet *fleet,
    Trace *trace)
{
    StarsI16 index;

    memset(input, 0, sizeof *input);
    memset(fleet, 0, sizeof *fleet);
    memset(trace, 0, sizeof *trace);
    fleet->id = 7;
    fleet->x = 1464;
    fleet->y = 1442;
    fleet->planet_id = 1;
    fleet->destination_id = 2;
    fleet->destination_visible = 1;
    fleet->plan_id = 3;
    fleet->player_number = 4;
    fleet->eta = 5;
    fleet->warp = 6;
    fleet->weight = 700L;
    fleet->cloak_percent = 33;
    fleet->scanner_range = 18;
    fleet->scanner_detail = 7;
    fleet->task_id = 12;
    fleet->mines = 101L;
    fleet->mine_sweep = 102L;
    fleet->mine_laying = 103L;
    fleet->terraforming = 104L;
    fleet->ship_count[0] = 3;
    fleet->ship_class[0] = 2;
    fleet->ship_count[4] = 4;
    fleet->ship_class[4] = 5;
    fleet->ship_count[5] = 1;
    for (index = 0; index < 5; ++index)
        fleet->cargo[index] = (StarsI32)(10 + index);
    input->file_name = "SERIAL.FLE";
    input->game_active = 1;
    input->viewer_player = 2;
    input->fleets = fleet;
    input->fleet_count = 1;
    input->open_report = open_report;
    input->write_header = write_header;
    input->write_text_field = write_field;
    input->write_text_end = write_end;
    input->close_report = close_report;
    input->context = trace;
}

int main(void)
{
    StarsDumpFleetsInput input;
    StarsDumpFleet fleet;
    Trace trace;
    int checks;
    int failures;

    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    setup(&input, &fleet, &trace);
    input.detailed = 0;
    CHECK(stars_dump_fleets(&input) == 1);
    CHECK(trace.fields == 12 && trace.ends == 1);
    CHECK(trace.sequence_bad == 0 && trace.detail_values_bad == 0);
    CHECK(trace.row.x == 1464 && trace.row.y == 1442);
    CHECK(trace.row.planet_id == 1 && trace.row.destination_id == 2);
    CHECK(trace.row.ship_total == 8L && trace.row.cargo[4] == 14L);

    setup(&input, &fleet, &trace);
    input.detailed = 1;
    CHECK(stars_dump_fleets(&input) == 1);
    CHECK(trace.fields == 29 && trace.ends == 1);
    CHECK(trace.sequence_bad == 0 && trace.detail_values_bad == 0);
    CHECK(trace.row.player_number == 4 && trace.row.eta == 5);
    CHECK(trace.row.scanner_range == 18 && trace.row.scanner_detail == 7);
    CHECK(trace.row.mines == 101L && trace.row.terraforming == 104L);
    CHECK(trace.row.ship_other_total == 1L);
    CHECK(trace.row.ship_class_total[3] == 4L);

    setup(&input, &fleet, &trace);
    fleet.ship_count[0] = 1;
    fleet.ship_count[4] = 0;
    fleet.ship_count[5] = 0;
    fleet.cargo[0] = 0L;
    fleet.cargo[1] = 0L;
    fleet.cargo[2] = 0L;
    fleet.cargo[3] = 0L;
    fleet.cargo[4] = 50L;
    input.detailed = 0;
    input.write_text_field = 0;
    input.format_text_field = format_text;
    input.write_text_value = write_text_value;
    input.write_text_end = write_text_end;
    CHECK(stars_dump_fleets(&input) == 1);
    CHECK(trace.formatted_fields == 12 && trace.formatted_ends == 1);
    CHECK(trace.format_bad == 0);
    CHECK(strcmp(trace.line,
        "Humanoid Armed Probe #1\t1464\t1442\tOop Be Gone\t-- \tDefault\t1\t0\t0\t0\t0\t50\r\n") == 0);

    setup(&input, &fleet, &trace);
    input.detailed = 1;
    input.write_text_field = 0;
    input.format_text_field = format_text;
    input.write_text_value = write_text_value;
    input.write_text_end = write_text_end;
    CHECK(stars_dump_fleets(&input) == 1);
    CHECK(trace.formatted_fields == 29 && trace.formatted_ends == 1);
    CHECK(trace.format_bad == 0);
    CHECK(strstr(trace.line, "Task 12") != 0);
    CHECK(strstr(trace.line, "104") != 0);

    {
        StarsDumpFleet fleets[6];
        const StarsI32 fuel[6] = { 50L, 300L, 200L, 450L, 530L, 210L };
        char expected[1024];
        const char *expected_lines[7];
        StarsI16 index;

        setup(&input, &fleets[0], &trace);
        for (index = 1; index < 6; ++index)
            fleets[index] = fleets[0];
        for (index = 0; index < 6; ++index) {
            fleets[index].id = (StarsI16)(7 + index);
            fleets[index].ship_count[0] = 1;
            fleets[index].ship_count[4] = 0;
            fleets[index].ship_count[5] = 0;
            fleets[index].cargo[0] = 0L;
            fleets[index].cargo[1] = 0L;
            fleets[index].cargo[2] = 0L;
            fleets[index].cargo[3] = 0L;
            fleets[index].cargo[4] = fuel[index];
        }
        input.fleets = fleets;
        input.fleet_count = 6;
        input.detailed = 0;
        input.write_text_field = 0;
        input.format_text_field = format_text;
        input.write_text_value = write_text_value;
        input.write_text_end = write_text_end;
        trace.original_oracle = 1;
        CHECK(stars_dump_fleets(&input) == 1);
        expected_lines[0] =
            "Fleet Name\tX\tY\tPlanet\tDestination\tBattle Plan\t"
            "Ship Cnt\tIron\tBora\tGerm\tCol\tFuel\r\n";
        expected_lines[1] =
            "Humanoid Armed Probe #1\t1464\t1442\tOop Be Gone\t-- \t"
            "Default\t1\t0\t0\t0\t0\t50\r\n";
        expected_lines[2] =
            "Humanoid Long Range Scout #2\t1464\t1442\tOop Be Gone\t"
            "-- \tDefault\t1\t0\t0\t0\t0\t300\r\n";
        expected_lines[3] =
            "Humanoid Santa Maria #3\t1464\t1442\tOop Be Gone\t-- \t"
            "Default\t1\t0\t0\t0\t0\t200\r\n";
        expected_lines[4] =
            "Humanoid Teamster #4\t1464\t1442\tOop Be Gone\t-- \t"
            "Default\t1\t0\t0\t0\t0\t450\r\n";
        expected_lines[5] =
            "Humanoid Stalwart Defender #5\t1464\t1442\tOop Be Gone\t"
            "-- \tDefault\t1\t0\t0\t0\t0\t530\r\n";
        expected_lines[6] =
            "Humanoid Cotton Picker #6\t1464\t1442\tOop Be Gone\t-- \t"
            "Default\t1\t0\t0\t0\t0\t210\r\n";
        expected[0] = 0;
        for (index = 0; index < 7; ++index)
            strcat(expected, expected_lines[index]);
        CHECK(trace.formatted_fields == 72 && trace.formatted_ends == 6);
        CHECK(trace.format_bad == 0 && strcmp(trace.line, expected) == 0);
    }
    printf("DUMP_FLEETS_SERIALIZATION_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
#undef CHECK
}
