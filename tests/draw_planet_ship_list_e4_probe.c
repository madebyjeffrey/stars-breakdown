#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    unsigned char selection[226];
    unsigned char transfer[128];
    StarsI16 selection_value;
    StarsI16 lookup_ok;
    StarsI16 lookup_kind;
    StarsI16 draw_ok;
    StarsI16 draw_calls;
    StarsI16 last_title;
    StarsI16 shown[4];
    StarsI16 hidden[4];
    StarsI16 resets;
    StarsI16 combo_calls;
    StarsI16 combo_flags;
    StarsI16 combo_left;
    StarsI16 combo_top;
    StarsI16 combo_width;
    StarsI16 combo_height;
    StarsI16 button_calls;
    StarsI16 button_ids[3];
    StarsI16 button_x[3];
    StarsI16 button_top[3];
    StarsI16 button_width[3];
    StarsI16 button_height[3];
    StarsI16 enable_calls;
    StarsI16 enabled[3];
    StarsI16 details;
    StarsI16 detail_kind;
    StarsI16 detail_index;
    StarsRect detail_rect;
    StarsI16 event_count;
    StarsI16 events[8];
} Trace;

static void put16(unsigned char *raw, int offset, unsigned short value)
{
    raw[offset] = (unsigned char)(value & 0xffU);
    raw[offset + 1] = (unsigned char)(value >> 8);
}

static void event(Trace *trace, StarsI16 value)
{
    if (trace->event_count < 8)
        trace->events[trace->event_count] = value;
    ++trace->event_count;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 title,
    StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    ++trace->draw_calls;
    trace->last_title = title;
    if (!trace->draw_ok)
        return 0;
    rect->left = 0;
    rect->top = 0;
    rect->right = 120;
    rect->bottom = 80;
    return 1;
}

static void show_control(void *context, StarsI16 control, StarsI16 show)
{
    Trace *trace = (Trace *)context;
    if (control < 0 || control >= 4)
        return;
    if (show)
        ++trace->shown[control];
    else
        ++trace->hidden[control];
}

static void reset_references(void *context)
{
    ++((Trace *)context)->resets;
}

static void set_combo_position_ex(void *context, StarsI16 left,
    StarsI16 top, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    Trace *trace = (Trace *)context;
    ++trace->combo_calls;
    trace->combo_left = left;
    trace->combo_top = top;
    trace->combo_width = width;
    trace->combo_height = height;
    trace->combo_flags = (StarsI16)flags;
    event(trace, 1);
}

static void set_button_position(void *context, StarsI16 button,
    StarsI16 left, StarsI16 top, StarsI16 width, StarsI16 height)
{
    Trace *trace = (Trace *)context;
    if (trace->button_calls < 3) {
        trace->button_ids[trace->button_calls] = button;
        trace->button_x[trace->button_calls] = left;
        trace->button_top[trace->button_calls] = top;
        trace->button_width[trace->button_calls] = width;
        trace->button_height[trace->button_calls] = height;
    }
    ++trace->button_calls;
    event(trace, 2);
}

static StarsI32 combo_selection(void *context, StarsU16 hdc)
{
    (void)hdc;
    return ((Trace *)context)->selection_value;
}

static void enable_button(void *context, StarsI16 button, StarsI16 enabled)
{
    Trace *trace = (Trace *)context;
    ++trace->enable_calls;
    if (button >= 0 && button < 3)
        trace->enabled[button] = enabled;
}

static StarsI16 lookup_transfer(void *context, StarsI16 player,
    StarsI16 selection, StarsI16 transfer_index, StarsI16 *kind)
{
    Trace *trace = (Trace *)context;
    (void)player;
    (void)selection;
    (void)transfer_index;
    *kind = trace->lookup_kind;
    return trace->lookup_ok;
}

static void draw_details(void *context, const StarsRect *rect,
    StarsI16 kind, StarsI16 transfer_index)
{
    Trace *trace = (Trace *)context;
    ++trace->details;
    trace->detail_kind = kind;
    trace->detail_index = transfer_index;
    trace->detail_rect = *rect;
    event(trace, 3);
}

static void install(StarsPlanetShipListOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->show_control = show_control;
    ops->reset_references = reset_references;
    ops->set_combo_position_ex = set_combo_position_ex;
    ops->combo_selection = combo_selection;
    ops->enable_button = enable_button;
    ops->lookup_transfer = lookup_transfer;
    ops->draw_details = draw_details;
    ops->set_button_position = set_button_position;
    ops->context = trace;
}

static void clear_trace(Trace *trace)
{
    trace->draw_calls = 0;
    trace->last_title = 0;
    trace->shown[0] = trace->shown[1] = trace->shown[2] = trace->shown[3] = 0;
    trace->hidden[0] = trace->hidden[1] = trace->hidden[2] = trace->hidden[3] = 0;
    trace->resets = 0;
    trace->combo_calls = 0;
    trace->combo_flags = 0;
    trace->button_calls = 0;
    trace->enable_calls = 0;
    trace->enabled[0] = trace->enabled[1] = trace->enabled[2] = -1;
    trace->details = 0;
    trace->event_count = 0;
}

int main(void)
{
    Trace trace;
    StarsPlanetShipListInput input;
    StarsPlanetShipListOps ops;
    int checks;
    int failures;

    memset(&trace, 0, sizeof trace);
    memset(&input, 0, sizeof input);
    put16(trace.selection, 152, 1);
    put16(trace.transfer, 0, 11);
    put16(trace.transfer, 2, 2);
    put16(trace.transfer, 6, 3);
    trace.draw_ok = 1;
    trace.selection_value = 2;
    trace.lookup_ok = 1;
    trace.lookup_kind = 2;
    install(&ops, &trace);
    input.hdc = 1;
    input.line_height = 8;
    input.combo_height = 40;
    input.player_id = 3;
    input.state.tile_changed = 1;
    input.state.tile_width = 0;
    input.packed_selection_record = trace.selection;
    input.packed_selection_size = (StarsU16)sizeof trace.selection;
    input.packed_transfer_record = trace.transfer;
    input.packed_transfer_size = (StarsU16)sizeof trace.transfer;
    input.ops = &ops;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    clear_trace(&trace);
    stars_draw_planet_ship_list(&input);
    CHECK(trace.draw_calls == 1 && trace.last_title == 0x339);
    CHECK(input.state.has_object == 1 && input.state.transfer_index == 11 &&
        input.state.transfer_kind == 2 && input.state.fleet_owner == 3);
    CHECK(trace.hidden[0] == 1 && trace.hidden[1] == 1 &&
        trace.hidden[2] == 1 && trace.hidden[3] == 1 && trace.resets == 2);
    CHECK(trace.combo_calls == 1 && trace.combo_left == 4 &&
        trace.combo_top == 2 && trace.combo_width == 112 &&
        trace.combo_height == 40 && trace.combo_flags == 0x14);
    CHECK(trace.details == 1 && trace.detail_kind == 2 &&
        trace.detail_index == 11 && trace.detail_rect.left == 4 &&
        trace.detail_rect.top == 2 && trace.detail_rect.right == 116 &&
        trace.detail_rect.bottom == 10);
    CHECK(trace.button_calls == 3 && trace.button_ids[0] == 1 &&
        trace.button_ids[1] == 2 && trace.button_ids[2] == 0 &&
        trace.button_x[0] == 4 && trace.button_x[1] == 44 &&
        trace.button_x[2] == 84 && trace.button_top[0] == 13 &&
        trace.button_width[0] == 34 && trace.button_height[0] == 12);
    CHECK(trace.events[0] == 1 && trace.events[1] == 3 &&
        trace.events[2] == 2 && input.state.transfer_found == 1 &&
        input.state.mode_flag == 0);

    clear_trace(&trace);
    input.state.tile_changed = 0;
    stars_draw_planet_ship_list(&input);
    CHECK(trace.combo_flags == 0x15 && trace.details == 1 &&
        trace.enabled[1] == 1 && trace.enabled[2] == 1);

    put16(trace.transfer, 2, 8);
    input.state.tile_changed = 1;
    input.state.tile_ship_mode = 1;
    clear_trace(&trace);
    stars_draw_planet_ship_list(&input);
    CHECK(input.state.transfer_kind == 8 && trace.details == 0 &&
        trace.enabled[1] == 0 && trace.enabled[2] == 0);

    memset(&input.packed_selection_record, 0,
        sizeof input.packed_selection_record);
    input.packed_selection_size = 0;
    input.packed_transfer_record = 0;
    input.packed_transfer_size = 0;
    input.state.tile_ship_mode = 0;
    input.state.tile_changed = 1;
    trace.lookup_ok = 0;
    clear_trace(&trace);
    stars_draw_planet_ship_list(&input);
    CHECK(trace.details == 0 && input.state.transfer_found == 0 &&
        trace.enabled[0] == 0 && trace.enabled[1] == 0 &&
        trace.shown[3] == 1 && trace.button_calls == 3);

    trace.draw_ok = 0;
    input.state.tile_changed = 1;
    clear_trace(&trace);
    stars_draw_planet_ship_list(&input);
    CHECK(trace.draw_calls == 1 && trace.hidden[0] == 2 &&
        trace.hidden[1] == 2 && trace.hidden[2] == 2 &&
        trace.hidden[3] == 2 && trace.combo_calls == 0 &&
        trace.details == 0);

    CHECK(checks == 12);
    if (failures == 0)
        puts("DRAW_PLANET_SHIP_LIST_E4_CHECKS=12 FAILURES=0 RESULT=PASS");
    else
        puts("DRAW_PLANET_SHIP_LIST_E4_CHECKS=12 FAILURES=1 RESULT=FAIL");
    return failures ? 1 : 0;
#undef CHECK
}
