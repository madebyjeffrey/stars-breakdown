#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 values;
    StarsI16 rights;
    StarsI16 buttons;
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
    StarsI16 row;
    (void)context;
    row = (StarsI16)(control - 0x123U);
    rect->left = (StarsU16)(20 + row * 10);
    rect->top = (StarsU16)(100 + row * 10);
    rect->right = (StarsU16)(100 + row * 10);
    rect->bottom = (StarsU16)(112 + row * 10);
}

static void map_window_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context; (void)hwnd;
    rect->left = (StarsU16)(rect->left + 5);
    rect->right = (StarsU16)(rect->right + 5);
    rect->top = (StarsU16)(rect->top + 7);
    rect->bottom = (StarsU16)(rect->bottom + 7);
}

static void select_font(void *context, StarsU16 dc, StarsU16 font)
{
    (void)context; (void)dc; (void)font;
}

static StarsI16 text_extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsI16 count)
{
    (void)context; (void)dc; (void)text;
    return (StarsI16)(count * 3);
}

static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    const char *value;
    (void)context;
    value = id == 0x0af2U ? "BB" : "L";
    if (capacity > 2U) {
        buffer[0] = (StarsU8)value[0];
        buffer[1] = (StarsU8)value[1];
        buffer[2] = 0;
    }
    return id == 0x0af2U ? 2 : 1;
}

static StarsI16 get_value(void *context, StarsI16 index, StarsI16 mode)
{
    Trace *trace;
    trace = (Trace *)context;
    (void)mode;
    ++trace->values;
    return index == 0 ? 1 : 1;
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
    (void)dc; (void)x; (void)y; (void)text; (void)count; (void)width;
    ++trace->rights;
}

static void draw_button(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 style)
{
    Trace *trace;
    trace = (Trace *)context;
    (void)dc; (void)rect; (void)style;
    ++trace->buttons;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    static const signed char widths[18] = {
        1, 2, 1, -1, 1, 2, 1, 2, 1, 2, 1, 2,
        1, 2, 1, 2, 1, 2
    };
    static const StarsU16 expected_left[18] = {
        72, 110, 82, 123, 92, 130, 102, 140, 112, 150, 122, 160,
        132, 170, 98, 136, 98, 136
    };
    StarsDrawNewGame3Ops ops;
    StarsDrawNewGame3Input input;
    StarsRect refs[36];
    StarsI16 count;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;
    StarsI16 pair;
    Trace trace;
    FILE *report;

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
    failures += check(count == 36, &checks);
    failures += check(trace.values == 18, &checks);
    failures += check(trace.rights == 18 && trace.buttons == 36, &checks);
    for (index = 0; index < 18; ++index) {
        pair = (StarsI16)(index * 2);
        failures += check(refs[pair].left == expected_left[index], &checks);
        failures += check(refs[pair].right ==
            (StarsU16)(expected_left[index] + 15), &checks);
        failures += check(refs[pair].bottom ==
            (StarsU16)(refs[pair].top + 9), &checks);
        failures += check(refs[pair + 1].left == refs[pair].left &&
            refs[pair + 1].right == refs[pair].right, &checks);
        failures += check(refs[pair + 1].top ==
            (StarsU16)(refs[pair].top + 8) &&
            refs[pair + 1].bottom == (StarsU16)(refs[pair].bottom + 8),
            &checks);
    }
    failures += check(refs[24].top == 166 && refs[28].top == 182, &checks);
    failures += check(refs[28].top == 182 && refs[32].top == 200, &checks);

    report = fopen("C:\\DNG3R\\DNG3RE2.TXT", "w");
    if (report) {
        fprintf(report,
            "DRAW_NEW_GAME3_RECTANGLES_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("DRAW_NEW_GAME3_RECTANGLES_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
