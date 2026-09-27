#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 begin_count;
    StarsI16 paint_count;
    StarsI16 end_count;
    StarsI16 tile_count;
    StarsI16 load_count;
    StarsI16 label_count;
    StarsI16 value_count;
    StarsI16 separator_count;
    StarsI16 reference_count;
    StarsU16 paint_dc;
    StarsI16 saw_mineral_rate;
    StarsI16 saw_scanner_range;
    StarsI16 saw_defense;
    char events[8];
    StarsI16 event_count;
} Trace;

static void event(Trace *trace, char value)
{
    if (trace->event_count < (StarsI16)(sizeof trace->events - 1)) {
        trace->events[trace->event_count] = value;
        ++trace->event_count;
        trace->events[trace->event_count] = 0;
    }
}

static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    ++trace->begin_count;
    rect->left = 1;
    rect->top = 2;
    rect->right = 100;
    rect->bottom = 80;
    trace->paint_dc = 55;
    event(trace, 'B');
    return trace->paint_dc;
}

static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    (void)rect;
    ++trace->end_count;
    event(trace, 'E');
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, const void *tile,
    StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)tile;
    ++trace->tile_count;
    rect->left = 10;
    rect->top = 10;
    rect->right = 210;
    rect->bottom = 100;
    return 1;
}

static StarsI16 load_data(void *context, const void *raw,
    StarsPlanetStatsData *data)
{
    Trace *trace = (Trace *)context;
    (void)raw;
    ++trace->load_count;
    memset(data, 0, sizeof *data);
    data->mineral_rate_x100 = 1300L;
    data->resources_available = 55;
    data->resources_total = 70;
    data->scanner_available = 1;
    data->scanner_range = 12;
    data->defense_capacity = 77;
    data->best_defense_available = 1;
    data->survival_percent = 88;
    return 1;
}

static StarsI16 measure_label(void *context, StarsU16 hdc, StarsI16 label)
{
    (void)context;
    (void)hdc;
    (void)label;
    return 15;
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
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)label;
    (void)x;
    (void)y;
    ++trace->label_count;
}

static void draw_value(void *context, StarsU16 hdc, StarsI16 field,
    StarsI16 right, StarsI16 value_offset, StarsI16 y,
    const StarsPlanetStatsData *data)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)right;
    (void)value_offset;
    (void)y;
    ++trace->value_count;
    if (field == 0)
        trace->saw_mineral_rate = data->mineral_rate_x100 == 1300L;
    if (field == 2)
        trace->saw_scanner_range = data->scanner_range == 12;
    if (field == 4)
        trace->saw_defense = data->defense_capacity == 77;
}

static void set_reference(void *context, StarsI16 id, const StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)id;
    (void)rect;
    ++trace->reference_count;
}

static void draw_separator(void *context, StarsU16 hdc, StarsI16 y,
    StarsI16 left, StarsI16 right)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)y;
    (void)left;
    (void)right;
    ++trace->separator_count;
}

static void paint_planet_ship(void *context, StarsU16 hwnd, StarsU16 dc)
{
    Trace *trace = (Trace *)context;
    StarsPlanetStatsOps stats_ops;
    StarsPlanetStatsInput stats_input;

    (void)hwnd;
    ++trace->paint_count;
    event(trace, 'P');
    memset(&stats_ops, 0, sizeof stats_ops);
    stats_ops.draw_tile = draw_tile;
    stats_ops.select_font = select_font;
    stats_ops.measure_label = measure_label;
    stats_ops.draw_label = draw_label;
    stats_ops.draw_value = draw_value;
    stats_ops.set_reference = set_reference;
    stats_ops.draw_separator = draw_separator;
    stats_ops.load_data = load_data;
    stats_ops.context = trace;
    memset(&stats_input, 0, sizeof stats_input);
    stats_input.hdc = dc;
    stats_input.tile = trace;
    stats_input.line_height = 8;
    stats_input.font_bold = 1;
    stats_input.font_normal = 0;
    stats_input.ops = &stats_ops;
    stars_draw_planet_stats(&stats_input);
}

int main(void)
{
    Trace trace;
    StarsPlanetWndProcOps proc_ops;
    StarsPlanetWndProcInput proc_input;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    memset(&trace, 0, sizeof trace);
    memset(&proc_ops, 0, sizeof proc_ops);
    memset(&proc_input, 0, sizeof proc_input);
    proc_ops.begin_paint = begin_paint;
    proc_ops.paint_planet_ship = paint_planet_ship;
    proc_ops.end_paint = end_paint;
    proc_ops.context = &trace;
    proc_input.ops = &proc_ops;
    CHECK(stars_planet_wnd_proc(77, 0x0f, 0, 0, &proc_input) == 0);
    CHECK(trace.begin_count == 1 && trace.paint_count == 1 &&
        trace.end_count == 1 && trace.paint_dc == 55);
    CHECK(strcmp(trace.events, "BPE") == 0);
    CHECK(trace.load_count == 1 && trace.tile_count == 1);
    CHECK(trace.label_count == 7 && trace.value_count == 7);
    CHECK(trace.reference_count == 4 && trace.separator_count == 2);
    CHECK(trace.saw_mineral_rate && trace.saw_scanner_range &&
        trace.saw_defense);
    CHECK(checks == 8);
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("PSTINTRES.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "PLANET_STATS_WNDPROC_INTEGRATION_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    if (failures)
        puts("PLANET_STATS_WNDPROC_INTEGRATION_CHECKS=8 FAILURES=1 RESULT=FAIL");
    else
        puts("PLANET_STATS_WNDPROC_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
