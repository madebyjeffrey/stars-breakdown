#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 dcs;
    StarsI16 releases;
    StarsI16 clients;
    StarsI16 modes;
    StarsI16 colors;
    StarsI16 fonts;
    StarsI16 texts;
    StarsI16 rights;
    StarsI16 buttons;
    StarsU16 dc;
    StarsU16 last_mode;
    StarsU16 current_mode;
    StarsU16 last_font;
    StarsU32 last_color;
    StarsU32 current_color;
    StarsU16 styles[36];
} Trace;

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    Trace *trace;
    trace = (Trace *)context;
    (void)hwnd;
    ++trace->dcs;
    trace->dc = 3U;
    return trace->dc;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    Trace *trace;
    trace = (Trace *)context;
    (void)hwnd;
    ++trace->releases;
    (void)dc;
}

static StarsU16 set_bk_mode(void *context, StarsU16 dc, StarsU16 mode)
{
    Trace *trace;
    StarsU16 old;
    trace = (Trace *)context;
    (void)dc;
    old = trace->current_mode;
    trace->current_mode = mode;
    trace->last_mode = mode;
    ++trace->modes;
    return old;
}

static StarsU32 set_bk_color(void *context, StarsU16 dc, StarsU32 color)
{
    Trace *trace;
    StarsU32 old;
    trace = (Trace *)context;
    (void)dc;
    old = trace->current_color;
    trace->current_color = color;
    trace->last_color = color;
    ++trace->colors;
    return old;
}

static void get_client_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace;
    trace = (Trace *)context;
    (void)hwnd;
    ++trace->clients;
    rect->left = 0; rect->top = 0; rect->right = 640; rect->bottom = 480;
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
    Trace *trace;
    trace = (Trace *)context;
    (void)dc;
    ++trace->fonts;
    trace->last_font = font;
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
    const char *value;
    (void)context;
    value = id == 0x0af2U ? "V" : "%";
    if (capacity > 1U) {
        buffer[0] = (StarsU8)value[0];
        buffer[1] = 0;
    }
    return 1;
}

static StarsI16 get_value(void *context, StarsI16 index, StarsI16 mode)
{
    (void)context; (void)index; (void)mode;
    return 7;
}

static StarsI16 format_value(void *context, StarsU8 *buffer, StarsU16 capacity,
    StarsI16 value)
{
    (void)context;
    if (capacity < 2U)
        return 0;
    buffer[0] = (StarsU8)('0' + value);
    buffer[1] = 0;
    return 1;
}

static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 count)
{
    Trace *trace;
    trace = (Trace *)context;
    (void)dc; (void)x; (void)y; (void)text; (void)count;
    ++trace->texts;
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
    (void)dc; (void)rect;
    if (trace->buttons < 36)
        trace->styles[trace->buttons] = style;
    ++trace->buttons;
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
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(refs, 0, sizeof refs);
    memset(&trace, 0, sizeof trace);
    trace.current_mode = 1U;
    trace.current_color = 0x55667788UL;
    ops.get_dc = get_dc;
    ops.release_dc = release_dc;
    ops.set_bk_mode = set_bk_mode;
    ops.set_bk_color = set_bk_color;
    ops.get_client_rect = get_client_rect;
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
    input.bold_font = 9U;
    input.read_only = 1;
    input.button_face_color = 0x11223344UL;
    input.reference_rects = refs;
    input.reference_count = &count;
    input.packed_column_widths = widths;
    input.packed_column_width_count = 18U;
    input.ops = &ops;

    checks = 0;
    failures = 0;
    stars_draw_new_game3(&input, -1);
    failures += check(trace.dcs == 1 && trace.releases == 1, &checks);
    failures += check(trace.clients == 1 && trace.dc == 3U, &checks);
    failures += check(trace.modes == 2 && trace.colors == 2, &checks);
    failures += check(trace.fonts == 1 && trace.last_font == 9U, &checks);
    failures += check(trace.texts == 27 && trace.rights == 18, &checks);
    failures += check(trace.buttons == 36 && count == 36, &checks);
    failures += check(trace.styles[0] == 0xa4U &&
        trace.styles[1] == 0xa5U && trace.styles[35] == 0xa5U, &checks);
    failures += check(trace.current_mode == 1U &&
        trace.current_color == 0x55667788UL, &checks);

    memset(&trace, 0, sizeof trace);
    trace.current_mode = 1U;
    trace.current_color = 0x55667788UL;
    input.hdc = 3U;
    stars_draw_new_game3(&input, 3);
    failures += check(trace.dcs == 0 && trace.releases == 0, &checks);
    failures += check(trace.buttons == 0 && trace.texts == 0, &checks);
    failures += check(trace.rights == 3, &checks);
    failures += check(trace.modes == 2 && trace.colors == 2, &checks);

    report = fopen("C:\\DNG3I\\DNG3IE4.TXT", "w");
    if (report) {
        fprintf(report,
            "DRAW_NEW_GAME3_INTEGRATION_E4_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("DRAW_NEW_GAME3_INTEGRATION_E4_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
