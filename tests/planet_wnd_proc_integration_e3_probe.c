#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    char events[32];
    StarsI16 event_count;
    StarsI16 paint_count;
    StarsI16 paint_dc;
    StarsI16 end_count;
    StarsI16 rect_count;
    StarsRect rect;
    StarsI16 fill_count;
    StarsU16 fill_brush;
    StarsI16 min_count;
    StarsI32 min_lparam;
    StarsI16 min_width;
    StarsI16 min_height;
    StarsI16 cursor_pos_count;
    StarsI16 map_count;
    StarsI16 point_count;
    StarsI16 ship_cursor_count;
    StarsI16 planet_cursor_count;
    StarsU16 ship_cursor;
    StarsU16 planet_cursor;
    StarsI16 set_cursor_count;
    StarsU16 cursor_value;
    StarsI16 draw_action;
    StarsI16 draw_focus_count;
    StarsI16 draw_entire_count;
    StarsI32 draw_lparam;
    StarsI16 draw_mode;
    StarsI16 measure_count;
    StarsI32 measure_lparam;
    StarsI16 measure_height;
    StarsPoint cursor_point;
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
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    ++trace->paint_count;
    trace->paint_dc = 55;
    rect->left = 1;
    rect->top = 2;
    rect->right = 30;
    rect->bottom = 40;
    event(trace, 'B');
    return 55;
}

static void paint(void *context, StarsU16 hwnd, StarsU16 dc)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    (void)dc;
    event(trace, 'P');
}

static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    (void)rect;
    ++trace->end_count;
    event(trace, 'E');
}

static void get_client_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    ++trace->rect_count;
    trace->rect.left = 0;
    trace->rect.top = 0;
    trace->rect.right = 20;
    trace->rect.bottom = 20;
    *rect = trace->rect;
    event(trace, 'R');
}

static void fill_rect(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)dc;
    (void)rect;
    ++trace->fill_count;
    trace->fill_brush = brush;
    event(trace, 'F');
}

static void get_cursor_pos(void *context, StarsPoint *point)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->cursor_pos_count;
    point->x = 100;
    point->y = 200;
    event(trace, 'G');
}

static void screen_to_client(void *context, StarsU16 hwnd,
    StarsPoint *point)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    ++trace->map_count;
    point->x = (StarsI16)(point->x - 90);
    point->y = (StarsI16)(point->y - 190);
    trace->cursor_point = *point;
    event(trace, 'M');
}

static StarsI16 point_in_rect(void *context, const StarsRect *rect,
    StarsPoint point)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)rect;
    ++trace->point_count;
    return point.x == 10 && point.y == 10;
}

static StarsU16 click_ship_orders(void *context, StarsPoint point,
    StarsI16 sks, StarsI16 cursor_query, StarsI16 right_button)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)point;
    (void)sks;
    (void)cursor_query;
    (void)right_button;
    ++trace->ship_cursor_count;
    return trace->ship_cursor;
}

static StarsU16 click_planet_orders(void *context, StarsPoint point,
    StarsI16 sks, StarsI16 cursor_query, StarsI16 right_button)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)point;
    (void)sks;
    (void)cursor_query;
    (void)right_button;
    ++trace->planet_cursor_count;
    return trace->planet_cursor;
}

static void set_cursor_value(void *context, StarsU16 cursor)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->set_cursor_count;
    trace->cursor_value = cursor;
}

static void set_minimum_size(void *context, StarsI32 l_param,
    StarsI16 width, StarsI16 height)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->min_count;
    trace->min_lparam = l_param;
    trace->min_width = width;
    trace->min_height = height;
}

static StarsI16 get_draw_action(void *context, StarsI32 l_param)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)l_param;
    return trace->draw_action;
}

static void draw_focus(void *context, StarsI32 l_param, StarsI16 mode)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->draw_focus_count;
    trace->draw_lparam = l_param;
    trace->draw_mode = mode;
}

static void draw_entire(void *context, StarsI32 l_param, StarsI16 mode)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->draw_entire_count;
    trace->draw_lparam = l_param;
    trace->draw_mode = mode;
}

static void measure(void *context, StarsI32 l_param, StarsI16 height)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->measure_count;
    trace->measure_lparam = l_param;
    trace->measure_height = height;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsPlanetWndProcOps ops;
    StarsPlanetWndProcInput input;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    ops.paint_planet_ship = paint;
    ops.begin_paint = begin_paint;
    ops.end_paint = end_paint;
    ops.get_client_rect = get_client_rect;
    ops.fill_rect = fill_rect;
    ops.get_cursor_pos = get_cursor_pos;
    ops.screen_to_client = screen_to_client;
    ops.point_in_rect = point_in_rect;
    ops.click_ship_orders = click_ship_orders;
    ops.click_planet_orders = click_planet_orders;
    ops.set_cursor_value = set_cursor_value;
    ops.set_minimum_size_exact = set_minimum_size;
    ops.get_draw_item_action = get_draw_action;
    ops.draw_item_focus = draw_focus;
    ops.draw_item_entire = draw_entire;
    ops.measure_combo_item = measure;
    ops.context = &trace;
    input.ariel8_height = 8;
    input.frame_dx = 10;
    input.frame_dy = 12;
    input.title_bar_height = 3;
    input.button_face_brush = 99;
    input.ops = &ops;
    checks = 0;
    failures = 0;
    failures += check(stars_planet_wnd_proc(77, 0x0f, 7, 0, &input) == 0,
        &checks);
    failures += check(trace.paint_count == 1 && trace.paint_dc == 55 &&
        trace.end_count == 1 && strcmp(trace.events, "BPE") == 0,
        &checks);
    failures += check(stars_planet_wnd_proc(77, 0x14, 44, 0, &input) == 1 &&
        trace.rect_count == 1 && trace.fill_count == 1 &&
        trace.fill_brush == 99 && trace.events[3] == 'R' &&
        trace.events[4] == 'F', &checks);
    failures += check(stars_planet_wnd_proc(77, 0x24, 0, 0x1234L, &input) == 0 &&
        trace.min_count == 1 && trace.min_lparam == 0x1234L &&
        trace.min_width == 218 && trace.min_height == 225, &checks);
    trace.draw_action = -1;
    failures += check(stars_planet_wnd_proc(77, 0x2b, 0, 0x1111L,
        &input) == 1 && trace.draw_focus_count == 1 &&
        trace.draw_mode == -2 && trace.draw_lparam == 0x1111L, &checks);
    trace.draw_action = 1;
    failures += check(stars_planet_wnd_proc(77, 0x2b, 0, 0x2222L,
        &input) == 1 && trace.draw_entire_count == 1 &&
        trace.draw_mode == -4 && trace.draw_lparam == 0x2222L, &checks);
    trace.draw_action = 3;
    failures += check(stars_planet_wnd_proc(77, 0x2b, 0, 0x3333L,
        &input) == 1 && trace.draw_focus_count == 1 &&
        trace.draw_entire_count == 1, &checks);
    failures += check(stars_planet_wnd_proc(77, 0x2c, 0, 0x4444L,
        &input) == 1 && trace.measure_count == 1 &&
        trace.measure_lparam == 0x4444L && trace.measure_height == 10,
        &checks);
    trace.ship_cursor = 0;
    trace.planet_cursor = 77;
    failures += check(stars_planet_wnd_proc(77, 0x20, 0, 0, &input) == 1 &&
        trace.cursor_pos_count == 1 && trace.map_count == 1 &&
        trace.point_count == 1 && trace.ship_cursor_count == 1 &&
        trace.planet_cursor_count == 1 && trace.set_cursor_count == 1 &&
        trace.cursor_value == 77 && trace.cursor_point.x == 10 &&
        trace.cursor_point.y == 10, &checks);
    trace.ship_cursor = 66;
    failures += check(stars_planet_wnd_proc(77, 0x20, 0, 0, &input) == 1 &&
        trace.ship_cursor_count == 2 && trace.planet_cursor_count == 1 &&
        trace.set_cursor_count == 2 && trace.cursor_value == 66, &checks);
#ifndef TARGET_NATIVE
    printf("PLANET_WND_PROC_INTEGRATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
#ifdef TARGET_NATIVE
    {
        FILE *report;

        report = fopen("C:\\PWTE3\\PWTE3.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "PLANET_WND_PROC_INTEGRATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#endif
    return failures == 0 ? 0 : 1;
}
