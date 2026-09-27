#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 selection;
    StarsI16 kind;
    StarsI16 details;
    StarsI16 last_kind;
    StarsI16 last_index;
    StarsRect detail_rect;
    StarsI16 buttons;
    StarsI16 button_ids[3];
    StarsI16 button_x[3];
    StarsI16 button_top[3];
    StarsI16 button_width[3];
    StarsI16 button_height[3];
    StarsI16 enabled[3];
    StarsI16 event_count;
    StarsI16 events[8];
} Trace;

static void event(Trace *trace, StarsI16 value)
{
    if (trace->event_count < 8)
        trace->events[trace->event_count] = value;
    ++trace->event_count;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 title,
    StarsRect *rect)
{
    (void)context;
    (void)hdc;
    (void)title;
    rect->left = 0;
    rect->top = 0;
    rect->right = 120;
    rect->bottom = 80;
    return 1;
}

static StarsI32 combo_selection(void *context, StarsU16 hdc)
{
    (void)hdc;
    return ((Trace *)context)->selection;
}

static StarsI16 lookup_transfer(void *context, StarsI16 player,
    StarsI16 selection, StarsI16 transfer_index, StarsI16 *kind)
{
    Trace *trace = (Trace *)context;
    (void)player;
    (void)selection;
    (void)transfer_index;
    *kind = trace->kind;
    return 1;
}

static void draw_details(void *context, const StarsRect *rect,
    StarsI16 kind, StarsI16 transfer_index)
{
    Trace *trace = (Trace *)context;
    ++trace->details;
    trace->last_kind = kind;
    trace->last_index = transfer_index;
    trace->detail_rect = *rect;
    event(trace, 1);
}

static void show_control(void *context, StarsI16 control, StarsI16 show)
{
    (void)context;
    (void)control;
    (void)show;
}

static void enable_button(void *context, StarsI16 button, StarsI16 enabled)
{
    Trace *trace = (Trace *)context;
    if (button >= 0 && button < 3)
        trace->enabled[button] = enabled;
}

static void reset_references(void *context)
{
    (void)context;
}

static void set_combo(void *context, StarsI16 left, StarsI16 top,
    StarsI16 width, StarsI16 height)
{
    (void)context;
    (void)left;
    (void)top;
    (void)width;
    (void)height;
}

static void set_button(void *context, StarsI16 button, StarsI16 left,
    StarsI16 top, StarsI16 width, StarsI16 height)
{
    Trace *trace = (Trace *)context;
    if (trace->buttons < 3) {
        trace->button_ids[trace->buttons] = button;
        trace->button_x[trace->buttons] = left;
        trace->button_top[trace->buttons] = top;
        trace->button_width[trace->buttons] = width;
        trace->button_height[trace->buttons] = height;
    }
    ++trace->buttons;
    event(trace, 2);
}

static void install(StarsPlanetShipListOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->combo_selection = combo_selection;
    ops->lookup_transfer = lookup_transfer;
    ops->draw_details = draw_details;
    ops->show_control = show_control;
    ops->enable_button = enable_button;
    ops->reset_references = reset_references;
    ops->set_combo_position = set_combo;
    ops->set_button_position = set_button;
    ops->context = trace;
}

static void reset_trace(Trace *trace)
{
    trace->details = 0;
    trace->last_kind = 0;
    trace->last_index = 0;
    trace->buttons = 0;
    trace->event_count = 0;
    trace->enabled[0] = trace->enabled[1] = trace->enabled[2] = -1;
}

int main(void)
{
    StarsPlanetShipListInput input;
    StarsPlanetShipListOps ops;
    Trace trace;
    int checks;
    int failures;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.line_height = 8;
    input.combo_height = 40;
    input.player_id = 3;
    input.state.has_object = 1;
    input.state.fleet_owner = 3;
    input.state.transfer_index = 11;
    input.ops = &ops;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    trace.selection = 2;
    trace.kind = 2;
    reset_trace(&trace);
    stars_draw_planet_ship_list(&input);
    CHECK(trace.details == 1 && trace.last_kind == 2 &&
        trace.last_index == 11);
    CHECK(trace.detail_rect.left == 4 && trace.detail_rect.top == 2 &&
        trace.detail_rect.right == 116 && trace.detail_rect.bottom == 10);
    CHECK(trace.buttons == 3 && trace.button_ids[0] == 1 &&
        trace.button_ids[1] == 2 && trace.button_ids[2] == 0);
    CHECK(trace.button_x[0] == 4 && trace.button_x[1] == 44 &&
        trace.button_x[2] == 84 && trace.button_top[0] == 13);
    CHECK(trace.button_width[0] == 34 && trace.button_height[0] == 12 &&
        trace.events[0] == 1 && trace.events[1] == 2);

    trace.kind = 8;
    reset_trace(&trace);
    stars_draw_planet_ship_list(&input);
    CHECK(trace.details == 1 && trace.last_kind == 8 &&
        trace.enabled[1] == 0);

    trace.selection = -1;
    reset_trace(&trace);
    stars_draw_planet_ship_list(&input);
    CHECK(trace.details == 0 && input.state.transfer_found == 0);

    trace.selection = 2;
    input.state.tile_ship_mode = 1;
    reset_trace(&trace);
    stars_draw_planet_ship_list(&input);
    CHECK(trace.details == 0 && trace.enabled[2] == 0);

    CHECK(checks == 9);
    if (failures == 0)
        puts("DRAW_PLANET_SHIP_LIST_DETAIL_RENDER_CHECKS=9 FAILURES=0 RESULT=PASS");
    else
        puts("DRAW_PLANET_SHIP_LIST_DETAIL_RENDER_CHECKS=9 FAILURES=1 RESULT=FAIL");
    return failures ? 1 : 0;
#undef CHECK
}
