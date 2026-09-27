#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 details;
    StarsI16 last_kind;
    StarsI16 last_index;
    StarsI16 fills;
    StarsI16 draws;
    StarsI16 selection;
} Trace;

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
    Trace *trace = (Trace *)context;
    (void)hdc;
    return trace->selection;
}

static StarsI16 lookup_transfer(void *context, StarsI16 player,
    StarsI16 selection, StarsI16 transfer_index, StarsI16 *kind)
{
    (void)context;
    (void)player;
    (void)selection;
    (void)transfer_index;
    *kind = 2;
    return 1;
}

static void draw_details(void *context, const StarsRect *rect,
    StarsI16 kind, StarsI16 transfer_index)
{
    Trace *trace = (Trace *)context;
    (void)rect;
    ++trace->details;
    trace->last_kind = kind;
    trace->last_index = transfer_index;
}

static void fill_details(void *context, const StarsRect *rect)
{
    (void)rect;
    ++((Trace *)context)->fills;
}

static void show_control(void *context, StarsI16 control, StarsI16 show)
{
    (void)context;
    (void)control;
    (void)show;
}

static void enable_button(void *context, StarsI16 button, StarsI16 enabled)
{
    (void)context;
    (void)button;
    (void)enabled;
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

static void install(StarsPlanetShipListOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->combo_selection = combo_selection;
    ops->lookup_transfer = lookup_transfer;
    ops->draw_details = draw_details;
    ops->fill_details = fill_details;
    ops->show_control = show_control;
    ops->enable_button = enable_button;
    ops->reset_references = reset_references;
    ops->set_combo_position = set_combo;
    ops->context = trace;
}

int main(void)
{
    StarsPlanetShipListInput input;
    StarsPlanetShipListOps ops;
    Trace trace;
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
    trace.selection = 2;
    stars_draw_planet_ship_list(&input);
    failures = trace.details != 1 || trace.last_kind != 2 ||
        trace.last_index != 11 || trace.fills != 0;

    memset(&trace, 0, sizeof trace);
    trace.selection = -1;
    stars_draw_planet_ship_list(&input);
    if (trace.details != 0 || trace.fills != 0)
        failures = 1;

    memset(&trace, 0, sizeof trace);
    input.state.tile_ship_mode = 1;
    trace.selection = 2;
    stars_draw_planet_ship_list(&input);
    if (trace.details != 0 || trace.fills != 0)
        failures = 1;

    printf("DRAW_PLANET_SHIP_LIST_DETAILS_CHECKS=9 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
