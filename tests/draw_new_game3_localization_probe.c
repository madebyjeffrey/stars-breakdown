#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 formats;
    StarsI16 values;
    StarsI16 rights;
    StarsI16 suffixes;
    StarsI16 label_count;
    StarsU16 labels[27];
    StarsU8 first_right[32];
} Trace;

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)context; (void)hwnd; return 3U;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)context; (void)hwnd; (void)dc;
}

static void get_window_rect(void *context, StarsU16 control, StarsRect *rect)
{
    (void)context; (void)control;
    rect->left = 20; rect->top = 100; rect->right = 100; rect->bottom = 112;
}

static void map_window_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context; (void)hwnd; (void)rect;
}

static void select_font(void *context, StarsU16 dc, StarsU16 font)
{
    (void)context; (void)dc; (void)font;
}

static StarsI16 text_extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsI16 count)
{
    (void)context; (void)dc; (void)text;
    return (StarsI16)(count * 2);
}

static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    Trace *trace;
    const char *value;
    trace = (Trace *)context;
    if (id == 0x0af2U)
        value = "VAL";
    else if (id == 0x0af4U) {
        value = "pct";
        ++trace->suffixes;
    } else {
        value = "L";
        if (trace->label_count < 27)
            trace->labels[trace->label_count++] = id;
    }
    if (capacity > 0U) {
        strncpy((char *)buffer, value, (size_t)capacity - 1U);
        buffer[capacity - 1U] = 0;
    }
    return (StarsI16)strlen(value);
}

static StarsI16 get_value(void *context, StarsI16 index, StarsI16 mode)
{
    Trace *trace;
    trace = (Trace *)context;
    (void)index; (void)mode;
    ++trace->values;
    return 7;
}

static StarsI16 format_value(void *context, StarsU8 *buffer, StarsU16 capacity,
    StarsI16 value)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->formats;
    if (capacity < 3U)
        return 0;
    buffer[0] = 'N';
    buffer[1] = (StarsU8)('0' + value);
    buffer[2] = 0;
    return 2;
}

static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 count)
{
    (void)context; (void)dc; (void)x; (void)y; (void)text; (void)count;
}

static void right_text(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 count, StarsI16 width)
{
    Trace *trace;
    trace = (Trace *)context;
    (void)dc; (void)x; (void)y; (void)width;
    if (trace->rights == 0) {
        strncpy((char *)trace->first_right, (const char *)text,
            sizeof trace->first_right - 1U);
        trace->first_right[sizeof trace->first_right - 1U] = 0;
    }
    (void)count;
    ++trace->rights;
}

static void draw_button(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 style)
{
    (void)context; (void)dc; (void)rect; (void)style;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    signed char widths[18];
    StarsDrawNewGame3Ops ops;
    StarsDrawNewGame3Input input;
    StarsRect refs[36];
    StarsI16 count;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;
    Trace trace;
    FILE *report;

    for (index = 0; index < 18; ++index)
        widths[index] = 1;
    widths[0] = -1;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(refs, 0, sizeof refs);
    memset(&trace, 0, sizeof trace);
    ops.get_dc = get_dc;
    ops.release_dc = release_dc;
    ops.get_window_rect = get_window_rect;
    ops.map_window_rect = map_window_rect;
    ops.select_font = select_font;
    ops.text_extent = text_extent;
    ops.get_string = get_string;
    ops.get_value = get_value;
    ops.format_value = format_value;
    ops.text_out = text_out;
    ops.right_text = right_text;
    ops.draw_button = draw_button;
    ops.context = &trace;
    input.hwnd = 4U;
    input.line_height = 12;
    input.reference_rects = refs;
    input.reference_count = &count;
    input.packed_column_widths = widths;
    input.packed_column_width_count = 18U;
    input.ops = &ops;
    checks = 0;
    failures = 0;
    stars_draw_new_game3(&input, -1);
    failures += check(trace.formats == 18, &checks);
    failures += check(trace.values == 18, &checks);
    failures += check(trace.rights == 18, &checks);
    failures += check(trace.suffixes == 1, &checks);
    failures += check(strcmp((const char *)trace.first_right, "N7pct") == 0,
        &checks);
    failures += check(trace.label_count == 27, &checks);
    for (index = 0; index < 27; ++index)
        failures += check(trace.labels[index] == (StarsU16)(0x3aaU + index),
            &checks);
    failures += check(count == 36, &checks);
    report = fopen("C:\\DNG3L\\DNG3LE3.TXT", "w");
    if (report) {
        fprintf(report,
            "DRAW_NEW_GAME3_LOCALIZATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("DRAW_NEW_GAME3_LOCALIZATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
