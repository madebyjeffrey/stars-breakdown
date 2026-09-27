#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct FormatTrace {
    StarsI16 formatted;
    StarsI16 drawn;
    StarsI16 fields[14];
    char text[14][32];
    StarsI16 lengths[14];
} FormatTrace;

static StarsI16 draw_tile(void *context, StarsU16 hdc, const void *tile,
    StarsRect *rect)
{
    (void)context;
    (void)hdc;
    (void)tile;
    rect->left = 10;
    rect->top = 10;
    rect->right = 210;
    rect->bottom = 100;
    return 1;
}

static StarsI16 measure_label(void *context, StarsU16 hdc, StarsI16 label)
{
    (void)context;
    (void)hdc;
    (void)label;
    return 15;
}

static StarsI16 format_value(void *context, StarsI16 field,
    const StarsPlanetStatsData *data, StarsU8 *out, StarsU16 capacity)
{
    (void)context;
    (void)capacity;
    switch (field) {
    case 0:
        return (StarsI16)sprintf((char *)out, "%ld", (long)data->mineral_rate_x100);
    case 1:
        return (StarsI16)sprintf((char *)out, "%d/%d",
            data->resources_available, data->resources_total);
    case 2:
        strcpy((char *)out, data->scanner_available ? "Delta" : "None");
        return (StarsI16)strlen((char *)out);
    case 3:
        return (StarsI16)sprintf((char *)out, "%d", data->scanner_range);
    case 4:
        return (StarsI16)sprintf((char *)out, "%d/%d",
            data->defense_capacity, data->best_defense_available);
    case 5:
        strcpy((char *)out, data->best_defense_available ? "BestDef" : "None");
        return (StarsI16)strlen((char *)out);
    case 6:
        return (StarsI16)sprintf((char *)out, "%d%%", data->survival_percent);
    default:
        out[0] = 0;
        return 0;
    }
}

static void draw_formatted_value(void *context, StarsU16 hdc, StarsI16 field,
    StarsI16 right, StarsI16 value_offset, StarsI16 y, const StarsU8 *text,
    StarsI16 length)
{
    FormatTrace *trace = (FormatTrace *)context;
    (void)hdc;
    (void)right;
    (void)value_offset;
    (void)y;
    if (trace->drawn < 14) {
        trace->fields[trace->drawn] = field;
        trace->lengths[trace->drawn] = length;
        if (length < 0)
            length = 0;
        if (length > 31)
            length = 31;
        memcpy(trace->text[trace->drawn], text, (size_t)length);
        trace->text[trace->drawn][length] = 0;
    }
    ++trace->drawn;
}

static void select_font(void *context, StarsU16 hdc, StarsI16 font)
{
    (void)context;
    (void)hdc;
    (void)font;
}

static void draw_label(void *context, StarsU16 hdc, StarsI16 label,
    StarsI16 x, StarsI16 y)
{
    (void)context;
    (void)hdc;
    (void)label;
    (void)x;
    (void)y;
}

static void set_reference(void *context, StarsI16 id, const StarsRect *rect)
{
    (void)context;
    (void)id;
    (void)rect;
}

static void draw_separator(void *context, StarsU16 hdc, StarsI16 y,
    StarsI16 left, StarsI16 right)
{
    (void)context;
    (void)hdc;
    (void)y;
    (void)left;
    (void)right;
}

static void install(StarsPlanetStatsOps *ops, FormatTrace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->select_font = select_font;
    ops->measure_label = measure_label;
    ops->draw_label = draw_label;
    ops->format_value = format_value;
    ops->draw_formatted_value = draw_formatted_value;
    ops->set_reference = set_reference;
    ops->draw_separator = draw_separator;
    ops->context = trace;
}

int main(void)
{
    StarsPlanetStatsInput input;
    StarsPlanetStatsOps ops;
    FormatTrace trace;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.hdc = 5;
    input.line_height = 8;
    input.font_bold = 1;
    input.font_normal = 0;
    input.data.mineral_rate_x100 = 1300L;
    input.data.resources_available = 55;
    input.data.resources_total = 70;
    input.data.scanner_available = 1;
    input.data.scanner_range = 12;
    input.data.defense_capacity = 77;
    input.data.best_defense_available = 88;
    input.data.survival_percent = 92;
    input.ops = &ops;
    stars_draw_planet_stats(&input);
    CHECK(trace.formatted == 0);
    CHECK(trace.drawn == 7);
    CHECK(trace.fields[0] == 0 && trace.fields[6] == 6);
    CHECK(strcmp(trace.text[0], "1300") == 0);
    CHECK(strcmp(trace.text[1], "55/70") == 0);
    CHECK(strcmp(trace.text[2], "Delta") == 0);
    CHECK(strcmp(trace.text[3], "12") == 0);
    CHECK(strcmp(trace.text[4], "77/88") == 0);
    CHECK(strcmp(trace.text[5], "BestDef") == 0);
    CHECK(strcmp(trace.text[6], "92%") == 0);

    trace.drawn = 0;
    input.data.mineral_rate_x100 = 0L;
    input.data.scanner_available = 0;
    input.data.best_defense_available = 0;
    stars_draw_planet_stats(&input);
    CHECK(trace.drawn == 7);
    CHECK(strcmp(trace.text[0], "0") == 0);
    CHECK(strcmp(trace.text[2], "None") == 0);
    CHECK(strcmp(trace.text[5], "None") == 0);
    CHECK(strcmp(trace.text[4], "77/0") == 0);
    CHECK(checks == 16);
    puts("DRAW_PLANET_STATS_FORMAT_CHECKS=16 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
