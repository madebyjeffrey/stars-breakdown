#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 live_mode;
    StarsI16 gets;
    StarsI16 releases;
    StarsI16 fleet_id;
    StarsI16 planet_id;
    StarsI16 popup_calls;
    StarsI16 popup_kind;
    StarsI16 popup_planet;
    StarsI16 popup_variable;
    StarsI16 popup_value;
    StarsI16 popup_minimum;
    StarsI16 popup_maximum;
    StarsI16 popup_index;
    StarsI16 popup_measure_calls;
    StarsI16 measured_kind;
    StarsI16 measured_value1;
    StarsI16 popup_creates;
    StarsI16 create_kind;
    StarsI16 create_width;
    StarsI16 create_height;
    StarsI16 create_x;
    StarsI16 create_y;
    StarsU16 create_parent;
    StarsU32 create_style;
    StarsI16 popup_messages;
    StarsI16 captures;
    StarsI16 paint_calls;
    StarsI16 painted_kind;
    StarsI16 painted_value1;
    StarsI16 painted_value2;
    StarsI16 paint_state_calls;
} Trace;

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)hwnd;
    ++((Trace *)context)->gets;
    return 4;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)hwnd;
    (void)dc;
    ++((Trace *)context)->releases;
}

static StarsI16 get_fleet(void *context, StarsI16 row, StarsI16 kind,
    StarsExecuteReportClickFleet *fleet)
{
    (void)context;
    (void)row;
    (void)kind;
    fleet->id = 42;
    fleet->planet_id = 7;
    fleet->point.x = 10;
    fleet->point.y = 20;
    fleet->flags = 0;
    return 1;
}

static StarsI16 get_planet(void *context, StarsI16 row,
    StarsExecuteReportClickPlanet *planet)
{
    Trace *trace = (Trace *)context;
    (void)row;
    memset(planet, 0, sizeof *planet);
    planet->id = trace->live_mode ? 10 : 9;
    planet->mineral_weight = trace->live_mode ?
        (StarsU32)((10UL << 20) | (10UL << 8)) :
        (StarsU32)(5UL << 20);
    planet->resources = trace->live_mode ? 35 : 60;
    planet->research_percent = trace->live_mode ? 15 : 25;
    return 1;
}

static void invalidate(void *context, StarsI16 report, StarsI16 full)
{
    (void)context;
    (void)report;
    (void)full;
}

static StarsI16 max_value(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 which)
{
    Trace *trace = (Trace *)context;
    (void)planet;
    (void)which;
    return trace->live_mode ? 1200 : 7;
}

static StarsI16 max_operable(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 which)
{
    Trace *trace = (Trace *)context;
    (void)planet;
    (void)which;
    return trace->live_mode ? 25 : 4;
}

static StarsI16 popup_client_to_screen(void *context, StarsU16 hwnd,
    StarsPoint *point)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    point->x = (StarsI16)(point->x + (trace->live_mode ? 344 : 5));
    point->y = (StarsI16)(point->y + (trace->live_mode ? 335 : 7));
    return 1;
}

static StarsU16 popup_get_dc(void *context, StarsU16 hwnd)
{
    (void)context;
    (void)hwnd;
    return 8;
}

static StarsU16 popup_select_font(void *context, StarsU16 hdc,
    StarsU16 font)
{
    (void)context;
    (void)hdc;
    return (StarsU16)(font + 1);
}

static StarsPoint popup_measure(void *context,
    const StarsPopupMeasureCase *measure)
{
    Trace *trace = (Trace *)context;
    StarsPoint size;
    ++trace->popup_measure_calls;
    trace->measured_kind = measure->popup_kind;
    trace->measured_value1 = measure->value1;
    size.x = trace->live_mode ?
        (measure->popup_kind == 7 ? 211 :
        (measure->popup_kind == 12 ? 195 :
        (trace->popup_variable ? 191 : 197))) :
        (StarsI16)(30 + measure->popup_kind);
    size.y = trace->live_mode ? 78 : 20;
    return size;
}

static StarsI16 popup_metric(void *context, StarsI16 index)
{
    Trace *trace = (Trace *)context;
    if (trace->live_mode)
        return index == 0 ? 1280 : 1024;
    return 200;
}

static StarsI16 draw_get_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context;
    (void)hwnd;
    rect->left = 0;
    rect->top = 0;
    rect->right = 120;
    rect->bottom = 50;
    return 1;
}

static StarsU32 draw_color(void *context, StarsU16 hdc, StarsU32 color)
{
    (void)hdc;
    ++((Trace *)context)->paint_state_calls;
    return color + 1;
}

static StarsU16 draw_mode(void *context, StarsU16 hdc, StarsU16 mode)
{
    (void)hdc;
    ++((Trace *)context)->paint_state_calls;
    return (StarsU16)(mode + 1);
}

static StarsU16 draw_font(void *context, StarsU16 hdc, StarsU16 font)
{
    (void)hdc;
    ++((Trace *)context)->paint_state_calls;
    return (StarsU16)(font + 1);
}

static void draw_case(void *context, const StarsDrawPopupCase *popup)
{
    Trace *trace = (Trace *)context;
    ++trace->paint_calls;
    trace->painted_kind = popup->popup_kind;
    trace->painted_value1 = popup->value1;
    trace->painted_value2 = popup->value2;
}

static StarsU16 create_window(void *context, StarsU16 parent, StarsU32 style,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height)
{
    Trace *trace = (Trace *)context;
    StarsDrawPopupInput draw_input;
    StarsDrawPopupOps draw_ops;

    trace->create_parent = parent;
    trace->create_style = style;
    ++trace->popup_creates;
    trace->create_kind = trace->popup_kind;
    trace->create_x = x;
    trace->create_y = y;
    trace->create_width = width;
    trace->create_height = height;
    memset(&draw_input, 0, sizeof draw_input);
    memset(&draw_ops, 0, sizeof draw_ops);
    draw_ops.get_client_rect = draw_get_rect;
    draw_ops.set_background_color = draw_color;
    draw_ops.set_text_color = draw_color;
    draw_ops.set_background_mode = draw_mode;
    draw_ops.select_font = draw_font;
    draw_ops.draw_mineral = draw_case;
    draw_ops.draw_fleet = draw_case;
    draw_ops.draw_planet_population = draw_case;
    draw_ops.draw_factory_mine = draw_case;
    draw_ops.draw_resource = draw_case;
    draw_ops.draw_default = draw_case;
    draw_ops.context = trace;
    draw_input.hwnd = 77;
    draw_input.hdc = 9;
    draw_input.popup_kind = trace->popup_kind;
    draw_input.font_normal = 4;
    draw_input.font_bold = 5;
    draw_input.arial8_height = 8;
    draw_input.ops = &draw_ops;
    stars_draw_popup(&draw_input);
    return 77;
}

static void popup_release_dc(void *context, StarsU16 hwnd, StarsU16 hdc)
{
    (void)context;
    (void)hwnd;
    (void)hdc;
}

static void popup_send_message(void *context, StarsU16 hwnd,
    StarsU16 message, StarsU16 font)
{
    (void)hwnd;
    (void)message;
    (void)font;
    ++((Trace *)context)->popup_messages;
}

static void popup_capture(void *context, StarsU16 hwnd)
{
    (void)hwnd;
    ++((Trace *)context)->captures;
}

static void show_popup(void *context, StarsPoint point,
    const StarsExecuteReportClickPopup *popup)
{
    Trace *trace = (Trace *)context;
    StarsPopupInput input;
    StarsPopupOps ops;

    ++trace->popup_calls;
    trace->popup_kind = popup->kind;
    trace->popup_planet = popup->planet_id;
    trace->popup_variable = popup->planet_variable;
    trace->popup_value = popup->planet_value;
    trace->popup_minimum = popup->planet_minimum;
    trace->popup_maximum = popup->planet_maximum;
    trace->popup_index = popup->mineral_index;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    ops.client_to_screen = popup_client_to_screen;
    ops.get_dc = popup_get_dc;
    ops.select_font = popup_select_font;
    ops.measure = popup_measure;
    ops.get_system_metric = popup_metric;
    ops.create_window = create_window;
    ops.release_dc = popup_release_dc;
    ops.send_message = popup_send_message;
    ops.set_capture = popup_capture;
    ops.context = trace;
    input.hwnd = trace->live_mode ? 0x086cU : 3U;
    input.x = point.x;
    input.y = point.y;
    input.popup_kind = popup->kind;
    input.planet_id = popup->planet_id;
    input.font_normal = 4;
    input.line_height = 8;
    input.line_height_large = 10;
    input.ops = &ops;
    stars_popup(&input);
}

static void install(StarsExecuteReportClickOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->get_dc = get_dc;
    ops->release_dc = release_dc;
    ops->get_fleet = get_fleet;
    ops->get_planet = get_planet;
    ops->invalidate_report = invalidate;
    ops->popup = show_popup;
    ops->max_factories = max_value;
    ops->max_operable_factories = max_operable;
    ops->max_mines = max_value;
    ops->max_operable_mines = max_operable;
    ops->context = trace;
}

int main(void)
{
    StarsExecuteReportClickState state;
    StarsExecuteReportClickOps ops;
    Trace trace;
    StarsPoint point;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    point.x = 14;
    point.y = 18;
    state.report_window = 3;
    state.transfer_dialog = -1;
    state.field_width[0] = 30;
    install(&ops, &trace);

    stars_execute_report_click(&state, point, 1, 8, 0, &ops);
    if (trace.popup_calls != 1 || trace.popup_kind != 3 ||
            trace.popup_planet != 42 || trace.popup_variable != 1 ||
            trace.popup_maximum != 255 || trace.popup_measure_calls != 1 ||
            trace.measured_kind != 3 || trace.popup_creates != 1 ||
            trace.create_kind != 3 || trace.create_width != 33 ||
            trace.create_height != 20 || trace.paint_calls != 1 ||
            trace.painted_kind != 3 || trace.popup_messages != 1 ||
            trace.captures != 1)
        return 1;

    stars_execute_report_click(&state, point, 0, 7, 0, &ops);
    if (trace.popup_calls != 2 || trace.popup_kind != 8 ||
            trace.popup_planet != 9 || trace.popup_variable != 1 ||
            trace.popup_value != 5 || trace.popup_minimum != 4 ||
            trace.popup_maximum != 7 || trace.measured_kind != 8 ||
            trace.measured_value1 != 200 || trace.create_width != 40 ||
            trace.create_height != 22 || trace.painted_kind != 8 ||
            trace.painted_value1 != 120 || trace.painted_value2 != 1)
        return 2;

    stars_execute_report_click(&state, point, 0, 12, 0, &ops);
    if (trace.popup_calls != 3 || trace.popup_kind != 12 ||
            trace.popup_planet != 9 || trace.popup_variable != 60 ||
            trace.popup_value != 45 || trace.measured_kind != 12 ||
            trace.measured_value1 != 200 || trace.create_width != 44 ||
            trace.create_height != 22 || trace.paint_calls != 3 ||
            trace.painted_kind != 12 || trace.painted_value1 != 120 ||
            trace.painted_value2 != 1 || trace.gets != 3 ||
            trace.releases != 3)
        return 3;

    /* Live original Planet Summary Report: GetWindowRect was 340,312 to
     * 940,712; client 265,26 became screen 609,361.  Its kind-7 popup was
     * 396,281 to 609,361 (213x80) with report-window parent/capture. */
    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.live_mode = 1;
    state.report_window = 0x086cU;
    state.transfer_dialog = -1;
    point.x = 265;
    point.y = 26;
    stars_execute_report_click(&state, point, 0, 2, 0, &ops);
    if (trace.popup_calls != 1 || trace.popup_kind != 7 ||
            trace.popup_planet != 10 || trace.create_x != 396 ||
            trace.create_y != 281 || trace.create_width != 213 ||
            trace.create_height != 80 || trace.create_parent != 0x086cU ||
            trace.create_style != 0x90800000UL ||
            trace.popup_messages != 1 || trace.captures != 1 ||
            trace.gets != 1 || trace.releases != 1 ||
            trace.paint_calls != 1 || trace.painted_kind != 7) {
#ifndef INTEGRATION_NATIVE
        fprintf(stderr, "live popup kind=%d id=%d rect=%d,%d %dx%d parent=%u style=%08lx msg=%d cap=%d get=%d rel=%d paint=%d/%d\n",
            trace.popup_kind, trace.popup_planet, trace.create_x,
            trace.create_y, trace.create_width, trace.create_height,
            trace.create_parent, (unsigned long)trace.create_style,
            trace.popup_messages, trace.captures, trace.gets,
            trace.releases, trace.paint_calls, trace.painted_kind);
#endif
        return 4;
    }

    point.x = 406;
    stars_execute_report_click(&state, point, 0, 12, 0, &ops);
    if (trace.popup_calls != 2 || trace.popup_kind != 12 ||
            trace.popup_planet != 10 || trace.popup_variable != 35 ||
            trace.popup_value != 30 || trace.create_x != 553 ||
            trace.create_y != 281 || trace.create_width != 197 ||
            trace.create_height != 80 || trace.create_parent != 0x086cU ||
            trace.popup_messages != 2 || trace.captures != 2 ||
            trace.paint_calls != 2 || trace.painted_kind != 12)
        return 5;

    point.x = 160;
    stars_execute_report_click(&state, point, 0, 6, 0, &ops);
    if (trace.popup_calls != 3 || trace.popup_kind != 8 ||
            trace.popup_planet != 10 || trace.popup_variable != 0 ||
            trace.popup_value != 10 || trace.popup_minimum != 25 ||
            trace.popup_maximum != 1200 || trace.create_x != 305 ||
            trace.create_y != 281 || trace.create_width != 199 ||
            trace.create_height != 80 || trace.captures != 3 ||
            trace.paint_calls != 3 || trace.painted_kind != 8)
        return 6;

    point.x = 205;
    stars_execute_report_click(&state, point, 0, 7, 0, &ops);
    if (trace.popup_calls != 4 || trace.popup_kind != 8 ||
            trace.popup_planet != 10 || trace.popup_variable != 1 ||
            trace.popup_value != 10 || trace.popup_minimum != 25 ||
            trace.popup_maximum != 1200 || trace.create_x != 356 ||
            trace.create_y != 281 || trace.create_width != 193 ||
            trace.create_height != 80 || trace.captures != 4 ||
            trace.paint_calls != 4 || trace.painted_kind != 8 ||
            trace.gets != 4 || trace.releases != 4)
        return 7;

    puts("EXECUTE_REPORT_CLICK_POPUP_INTEGRATION_CHECKS=86 FAILURES=0 RESULT=PASS");
    return 0;
}
