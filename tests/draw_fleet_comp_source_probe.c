#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 draw_calls;
    StarsU16 hides;
    StarsU16 shows;
    StarsU16 positions;
    StarsU16 texts;
    StarsU16 right_texts;
    StarsU16 fonts;
    StarsU16 enables;
    StarsU16 client_rects;
    StarsI32 last_total;
    StarsI16 last_enable;
    StarsI16 last_position_flags;
    StarsI16 last_width;
    StarsI16 last_height;
    StarsU16 last_text_id;
    StarsI16 cloak_calls;
} Trace;

static StarsI16 get_text(void *context, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    Trace *t = (Trace *)context;
    (void)capacity;
    t->last_text_id = id;
    strcpy((char *)out, "TEXT");
    return 4;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsU16 flags,
    const StarsU8 *title, StarsRect *rect)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)flags;
    (void)title;
    ++t->draw_calls;
    rect->left = 10U;
    rect->top = 20U;
    rect->right = 110U;
    rect->bottom = 100U;
    return 1;
}

static StarsI32 extent(void *context, StarsU16 hdc, const StarsU8 *text,
    StarsI16 length)
{
    (void)context;
    (void)hdc;
    (void)text;
    return (StarsI32)(length * 2);
}

static void text_out(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)text;
    (void)length;
    ++t->texts;
}

static void right_text(void *context, StarsU16 hdc, StarsI16 right,
    StarsI16 y, const StarsU8 *text, StarsI16 length, StarsI16 offset)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)right;
    (void)y;
    (void)text;
    (void)length;
    (void)offset;
    ++t->right_texts;
}

static StarsI16 get_client_height(void *context, StarsU16 window)
{
    Trace *t = (Trace *)context;
    (void)window;
    ++t->client_rects;
    return t->client_rects == 1U ? 20 : 35;
}

static void set_window_pos(void *context, StarsU16 window, StarsI16 x,
    StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    Trace *t = (Trace *)context;
    (void)window;
    (void)x;
    (void)y;
    ++t->positions;
    t->last_width = width;
    t->last_height = height;
    t->last_position_flags = (StarsI16)flags;
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

static void select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)font;
    ++t->fonts;
}

static void enable_window(void *context, StarsU16 window, StarsI16 enable)
{
    Trace *t = (Trace *)context;
    (void)window;
    ++t->enables;
    t->last_enable = enable;
}

static StarsI16 find_ideal_warp(void *context, const void *fleet,
    StarsI16 ignore_scoops)
{
    (void)context;
    (void)fleet;
    (void)ignore_scoops;
    return 6;
}

static StarsI32 estimate_fuel(void *context, const void *fleet,
    StarsI16 ideal_warp)
{
    (void)context;
    (void)fleet;
    (void)ideal_warp;
    return 123L;
}

static StarsI16 format_fuel(void *context, StarsU8 *out, StarsU16 capacity,
    StarsI32 value)
{
    (void)context;
    (void)capacity;
    (void)value;
    strcpy((char *)out, "F");
    return 1;
}

static StarsI16 format_percent(void *context, StarsU8 *out,
    StarsU16 capacity, StarsI16 value)
{
    (void)context;
    (void)capacity;
    (void)value;
    strcpy((char *)out, "P");
    return 1;
}

static StarsI16 cloak_percent(void *context, const void *fleet)
{
    Trace *t = (Trace *)context;
    (void)fleet;
    ++t->cloak_calls;
    return 25;
}

static StarsI16 can_split(void *context, StarsI32 total)
{
    Trace *t = (Trace *)context;
    t->last_total = total;
    return (StarsI16)(total >= 2L);
}

static StarsI16 can_split_all(void *context, StarsI32 total)
{
    (void)context;
    return (StarsI16)(total == 5L);
}

static StarsI16 can_merge(void *context, const void *fleet)
{
    (void)context;
    (void)fleet;
    return 1;
}

static void install_ops(StarsDrawFleetCompInput *input)
{
    input->ops.get_text = get_text;
    input->ops.draw_tile = draw_tile;
    input->ops.text_extent = extent;
    input->ops.text_out = text_out;
    input->ops.right_text = right_text;
    input->ops.get_client_height = get_client_height;
    input->ops.set_window_pos = set_window_pos;
    input->ops.show_window = show_window;
    input->ops.enable_window = enable_window;
    input->ops.select_font = select_font;
    input->ops.find_ideal_warp = find_ideal_warp;
    input->ops.estimate_fuel = estimate_fuel;
    input->ops.format_fuel = format_fuel;
    input->ops.format_percent = format_percent;
    input->ops.cloak_percent = cloak_percent;
    input->ops.can_split = can_split;
    input->ops.can_split_all = can_split_all;
    input->ops.can_merge = can_merge;
}

int main(void)
{
    StarsDrawFleetCompInput input;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    input.tile_flags = 0x0800U;
    input.grid_bit = 0U;
    input.arial8_height = 10;
    input.ship_dropdown_height = 12;
    input.fleet_component_list = 11U;
    input.battle_dropdown = 12U;
    input.buttons[0] = 13U;
    input.buttons[1] = 14U;
    input.buttons[2] = 15U;
    input.ship_counts[0] = 2U;
    input.ship_counts[1] = 3U;
    install_ops(&input);
    input.context = &trace;
    stars_draw_fleet_comp(&input);
    if (trace.draw_calls != 1U || trace.hides != 5U || trace.shows != 5U ||
            trace.positions != 5U || trace.right_texts != 2U ||
            trace.texts != 3U || trace.enables != 3U ||
            trace.client_rects != 2U || trace.last_total != 5L ||
            trace.last_enable != 1 || trace.cloak_calls != 1 ||
            trace.last_width != 27 || trace.last_height != 15)
        return 1;

    memset(&trace, 0, sizeof trace);
    input.tile_flags = 0x1000U;
    input.grid_bit = 1U;
    input.fleet_component_width = 92;
    input.battle_dropdown_width = 92;
    stars_draw_fleet_comp(&input);
    if (trace.draw_calls != 1U || trace.texts != 0U ||
            trace.right_texts != 1U || trace.cloak_calls != 0 ||
            trace.last_position_flags != 0x14)
        return 2;
    puts("DRAW_FLEET_COMP_SOURCE=PASS");
    return 0;
}
