#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 fields;
    StarsI16 current_field;
    StarsI16 ends;
    StarsI16 sequence_bad;
    StarsI16 format_bad;
    StarsI16 write_bad;
    StarsI16 formatted_fields;
    StarsI16 formatted_ends;
    StarsI16 formatted_index;
    StarsI16 line_length;
    char line[2048];
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
    (void)context;
    (void)number;
    return 1;
}

static StarsI16 write_field(void *context, StarsI16 field,
    const StarsDumpPlanetRow *row)
{
    Trace *trace;

    trace = (Trace *)context;
    if (field != trace->current_field) {
        trace->sequence_bad = 1;
    }
    if (field == 0 && trace->fields == 0 && row->id != 4) {
        trace->sequence_bad = 1;
    }
    if (field == 3 && trace->fields < STARS_DUMP_PLANET_TEXT_FIELD_COUNT &&
            row->turns_since_visit != 3) {
        trace->sequence_bad = 1;
    }
    if (field == 19 && trace->fields < STARS_DUMP_PLANET_TEXT_FIELD_COUNT &&
            row->resources != 35L) {
        trace->sequence_bad = 1;
    }
    ++trace->fields;
    ++trace->current_field;
    if (field == STARS_DUMP_PLANET_TEXT_FIELD_COUNT - 1)
        trace->current_field = 0;
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
    const StarsDumpPlanetRow *row, StarsU8 *output, StarsU16 capacity)
{
    Trace *trace;
    char number[64];

    trace = (Trace *)context;
    if (field != trace->formatted_index)
        trace->format_bad = 1;
    if (field == 0)
        return copy_text(output, capacity,
            row->id == 4 ? "Oop Be Gone" : "Lazy B");
    if (field == 1)
        return copy_text(output, capacity, row->id == 4 ? "Humanoid" : "");
    if (field == 2)
        return copy_text(output, capacity, row->id == 4 ? "Starbase" : "");
    if (field == 5)
        return copy_text(output, capacity, row->id == 4 ? "100%" : "20%");
    if (field == 6)
        return copy_text(output, capacity,
            row->id == 4 ? "      --- Queue is Empty ---" : "");
    if (field == 9)
        return copy_text(output, capacity, row->id == 4 ? "9.56%" : "");
    if (field == 3)
        sprintf(number, "%d", row->turns_since_visit);
    else if (field == 4)
        if (row->id == 4)
            sprintf(number, "%ld", (long)row->population);
        else
            number[0] = 0;
    else if (field == 7)
        if (row->id == 4)
            sprintf(number, "%d", row->mine_count);
        else
            number[0] = 0;
    else if (field == 8)
        if (row->id == 4)
            sprintf(number, "%d", 10);
        else
            number[0] = 0;
    else if (field >= 10 && field <= 12)
        sprintf(number, "%ld", (long)(row->mineral_concentration[field - 10]));
    else if (field >= 13 && field <= 15)
        if (row->id == 4)
            sprintf(number, "%ld", (long)(row->mineral_rate[field - 13]));
        else
            number[0] = 0;
    else if (field >= 16 && field <= 18)
        sprintf(number, "%d", row->environment[field - 16]);
    else if (field == 19)
        if (row->id == 4)
            sprintf(number, "%ld", (long)row->resources);
        else
            number[0] = 0;
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
            (StarsI16)sizeof trace->line - 1) {
        trace->write_bad = 1;
        return 0;
    }
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

static void setup(StarsDumpPlanetsInput *input, StarsDumpPlanet *planets,
    Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(planets, 0, sizeof *planets * 2);
    memset(trace, 0, sizeof *trace);
    planets[0].id = 4;
    planets[0].owner = 2;
    planets[0].flags = STARS_DUMP_PLANET_FLAG_INHABITED;
    planets[0].turn = 17;
    planets[0].population = 25000L;
    planets[0].production = 100L;
    planets[0].resources = 35L;
    planets[0].mineral_concentration[0] = 463L;
    planets[0].mineral_concentration[1] = 762L;
    planets[0].mineral_concentration[2] = 730L;
    planets[0].mineral_rate[0] = 7L;
    planets[0].mineral_rate[1] = 7L;
    planets[0].mineral_rate[2] = 7L;
    planets[0].environment[0] = 74;
    planets[0].environment[1] = 70;
    planets[0].environment[2] = 72;
    planets[0].mine_count = 10;
    planets[1].id = 5;
    planets[1].owner = -1;
    planets[1].flags = (StarsU16)(STARS_DUMP_PLANET_FLAG_INHABITED |
        STARS_DUMP_PLANET_FLAG_SCANNED);
    planets[1].turn = 20;
    planets[1].environment[0] = 60;
    planets[1].environment[1] = 76;
    planets[1].environment[2] = 48;
    input->file_name = "SERIAL.PLA";
    input->game_active = 1;
    input->viewer_player = 2;
    input->game_turn = 20;
    input->planets = planets;
    input->planet_count = 2;
    input->open_report = open_report;
    input->write_header = write_header;
    input->close_report = close_report;
    input->context = trace;
}

int main(void)
{
    StarsDumpPlanetsInput input;
    StarsDumpPlanet planets[2];
    Trace trace;
    int checks;
    int failures;

    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    setup(&input, planets, &trace);
    input.write_text_field = write_field;
    input.write_text_end = write_end;
    input.detailed = 0;
    CHECK(stars_dump_planets(&input) == 1);
    CHECK(trace.fields == 40 && trace.ends == 2);
    CHECK(trace.sequence_bad == 0);

    setup(&input, planets, &trace);
    input.format_text_field = format_text;
    input.write_text_value = write_text_value;
    input.write_text_end = write_text_end;
    input.detailed = 1;
    planets[0].turn = 20;
    planets[1].turn = 20;
    CHECK(stars_dump_planets(&input) == 1);
    CHECK(trace.formatted_fields == 40 && trace.formatted_ends == 2);
    CHECK(trace.format_bad == 0 && trace.write_bad == 0);
    CHECK(strcmp(trace.line,
        "Oop Be Gone\tHumanoid\tStarbase\t0\t25000\t100%\t"
        "      --- Queue is Empty ---\t10\t10\t9.56%\t463\t762\t730\t"
        "7\t7\t7\t74\t70\t72\t35\r\n"
        "Lazy B\t\t\t0\t\t20%\t\t\t\t\t0\t0\t0\t\t\t\t"
        "60\t76\t48\t\r\n") == 0);

    setup(&input, planets, &trace);
    input.format_text_field = format_text;
    input.write_text_value = write_text_value;
    input.write_text_end = write_text_end;
    input.detailed = 1;
    planets[0].turn = 20;
    planets[1].turn = 20;
    planets[0].id = 4;
    planets[1].id = 4;
    CHECK(stars_dump_planets(&input) == 1);
    CHECK(strstr(trace.line, "Oop Be Gone\tHumanoid\tStarbase") != 0);
    CHECK(strstr(trace.line, "9.56%\t463\t762\t730") != 0);
    CHECK(strstr(trace.line, "35\r\n") != 0);

    input.format_text_field = 0;
    input.write_text_value = 0;
    input.write_text_end = 0;
    input.write_row = 0;
    CHECK(stars_dump_planets(&input) == 0);
    printf("DUMP_PLANETS_SERIALIZATION_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures != 0;
#undef CHECK
}
