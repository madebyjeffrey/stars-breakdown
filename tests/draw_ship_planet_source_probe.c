#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 draws;
    StarsU16 shows;
    StarsU16 hides;
    StarsU16 enables;
    StarsU16 last_enabled_window;
    StarsI16 last_enabled;
    StarsU16 texts;
    StarsU16 positions;
    StarsI16 last_x;
    StarsI16 last_y;
    StarsI16 last_width;
    StarsI16 last_height;
    StarsI16 name_id;
} Trace;

static StarsI16 get_text(void *context, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    (void)context;
    (void)capacity;
    strcpy((char *)out, id == 0x0377U ? "NONE" : "THING");
    return 4;
}

static StarsI16 get_planet_name(void *context, StarsI16 id, StarsU8 *out,
    StarsU16 capacity)
{
    Trace *t = (Trace *)context;
    (void)capacity;
    t->name_id = id;
    strcpy((char *)out, "PLANET");
    return 6;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsU16 flags,
    const StarsU8 *title, StarsRect *rect)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)flags;
    (void)title;
    ++t->draws;
    rect->left = 10U;
    rect->top = 20U;
    rect->right = 100U;
    rect->bottom = 50U;
    return 1;
}

static void show_window(void *context, StarsU16 window, StarsI16 show)
{
    Trace *t = (Trace *)context;
    (void)window;
    if (show == 0)
        ++t->hides;
    else
        ++t->shows;
}

static void enable_window(void *context, StarsU16 window, StarsI16 enable)
{
    Trace *t = (Trace *)context;
    ++t->enables;
    t->last_enabled_window = window;
    t->last_enabled = enable;
}

static void set_window_text(void *context, StarsU16 window,
    const StarsU8 *text)
{
    Trace *t = (Trace *)context;
    (void)window;
    (void)text;
    ++t->texts;
}

static void set_window_pos(void *context, StarsU16 window, StarsI16 x,
    StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    Trace *t = (Trace *)context;
    (void)window;
    (void)flags;
    ++t->positions;
    t->last_x = x;
    t->last_y = y;
    t->last_width = width;
    t->last_height = height;
}

int main(void)
{
    StarsDrawShipPlanetInput input;
    StarsDrawShipPlanetThing things[1];
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    things[0].flags = (StarsU16)(1U << 13);
    things[0].point.x = 5;
    things[0].point.y = 6;
    input.hdc = 1U;
    input.tile_flags = 0x0800U;
    input.planet_id = 5;
    input.selected_planet_player = 3;
    input.current_player = 3;
    input.grid_bit = 0;
    input.arial8_height = 10;
    input.button_planet = 20U;
    input.button_thing = 21U;
    input.things = things;
    input.thing_count = 1U;
    input.ops.get_text = get_text;
    input.ops.get_planet_name = get_planet_name;
    input.ops.draw_tile = draw_tile;
    input.ops.show_window = show_window;
    input.ops.enable_window = enable_window;
    input.ops.set_window_text = set_window_text;
    input.ops.set_window_pos = set_window_pos;
    input.context = &trace;
    stars_draw_ship_planet(&input);
    if (trace.draws != 1U || trace.hides != 2U || trace.enables != 2U ||
            trace.shows != 2U || trace.texts != 1U || trace.positions != 2U ||
            trace.last_x != 74 || trace.last_y != 24 || trace.last_width != 22 ||
            trace.last_height != 15 || trace.name_id != (StarsI16)0x8005)
        return 1;

    memset(&trace, 0, sizeof trace);
    input.tile_flags = 0U;
    input.planet_id = -1;
    input.object_x = 5;
    input.object_y = 6;
    stars_draw_ship_planet(&input);
    if (trace.enables != 2U || trace.last_enabled_window != 21U ||
            trace.last_enabled != 0 || trace.texts != 1U)
        return 2;
    puts("DRAW_SHIP_PLANET_SOURCE=PASS");
    return 0;
}
