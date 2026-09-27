#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 draws;
    StarsU16 hides;
    StarsU16 shows;
    StarsU16 positions;
    StarsU16 references;
    StarsU16 diamonds;
    StarsU16 gauges;
    StarsU16 messages;
    StarsU16 formatted;
    StarsU16 minerals;
    StarsU16 draw_succeeds;
    StarsU16 primary;
    StarsU16 secondary;
    StarsU16 last_message;
} Trace;

static StarsI16 get_text(void *context, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    (void)context;
    (void)id;
    if (capacity > 1U) {
        out[0] = 'T';
        out[1] = 0;
    }
    return 1;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsU16 flags,
    const StarsU8 *title, StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)flags;
    (void)title;
    ++trace->draws;
    if (!trace->draw_succeeds)
        return 0;
    rect->left = 10U;
    rect->top = 20U;
    rect->right = 110U;
    rect->bottom = 120U;
    return 1;
}

static StarsI32 send_message(void *context, StarsU16 window, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    Trace *trace = (Trace *)context;
    (void)wparam;
    (void)lparam;
    if (message == 0x0407U)
        return window == 10U ? trace->primary : trace->secondary;
    ++trace->messages;
    return 0;
}

static StarsI16 visible(void *context, StarsU16 window)
{
    (void)context;
    (void)window;
    return 0;
}

static void show_window(void *context, StarsU16 window, StarsI16 value)
{
    Trace *trace = (Trace *)context;
    (void)window;
    if (value == 0)
        ++trace->hides;
    else
        ++trace->shows;
}

static void set_position(void *context, StarsU16 window, StarsI16 x,
    StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    Trace *trace = (Trace *)context;
    (void)window;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)flags;
    ++trace->positions;
}

static void enable_window(void *context, StarsU16 window, StarsI16 enabled)
{
    (void)context;
    (void)window;
    (void)enabled;
}

static StarsI32 text_extent(void *context, StarsU16 hdc, const StarsU8 *text,
    StarsI16 length)
{
    (void)context;
    (void)hdc;
    (void)text;
    return length;
}

static void select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    (void)context;
    (void)hdc;
    (void)font;
}

static void fill_rect(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsU16 brush)
{
    (void)context;
    (void)hdc;
    (void)rect;
    (void)brush;
}

static void draw_text(void *context, StarsU16 hdc, const StarsU8 *text,
    StarsI16 length, const StarsRect *rect, StarsU16 format)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)text;
    (void)length;
    (void)rect;
    (void)format;
    ++trace->last_message;
}

static void set_color(void *context, StarsU16 hdc, StarsU32 color)
{
    (void)context;
    (void)hdc;
    (void)color;
}

static void set_reference(void *context, StarsU16 reference,
    const StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)reference;
    (void)rect;
    ++trace->references;
}

static void draw_diamond(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsU16 brush)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)rect;
    (void)brush;
    ++trace->diamonds;
}

static void draw_gauge(void *context, StarsU16 hdc, const StarsRect *rect,
    const void *fleet, StarsI16 kind)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)rect;
    (void)fleet;
    (void)kind;
    ++trace->gauges;
}

static StarsI16 format_number(void *context, StarsU8 *out, StarsU16 capacity,
    StarsU16 id, StarsI32 value)
{
    Trace *trace = (Trace *)context;
    (void)id;
    (void)value;
    ++trace->formatted;
    if (capacity > 1U) {
        out[0] = '1';
        out[1] = 0;
    }
    return 1;
}

static void draw_minerals(void *context, StarsU16 hdc, StarsI16 x,
    StarsI16 y, StarsI16 mines, StarsI16 density)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)mines;
    (void)density;
    ++trace->minerals;
}

static void install_ops(StarsWayPtOrdersInput *input)
{
    input->ops.get_text = get_text;
    input->ops.draw_tile = draw_tile;
    input->ops.send_message = send_message;
    input->ops.is_visible = visible;
    input->ops.show_window = show_window;
    input->ops.set_window_pos = set_position;
    input->ops.enable_window = enable_window;
    input->ops.text_extent = text_extent;
    input->ops.select_font = select_font;
    input->ops.fill_rect = fill_rect;
    input->ops.draw_text = draw_text;
    input->ops.set_text_color = set_color;
    input->ops.set_reference = set_reference;
    input->ops.draw_diamond = draw_diamond;
    input->ops.draw_gauge = draw_gauge;
    input->ops.format_number = format_number;
    input->ops.draw_minerals = draw_minerals;
}

int main(void)
{
    StarsWayPtOrdersInput input;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    input.tile_flags = 0x0800U;
    input.tile_rect.right = 120U;
    input.tile_rect.bottom = 130U;
    input.arial8_height = 10;
    input.arial7_height = 9;
    input.arial6_height = 8;
    input.ship_dropdown_height = 12;
    input.order_edit_width = 20;
    input.dropdown[0] = 10U;
    input.dropdown[1] = 11U;
    input.dropdown[2] = 12U;
    input.order_edit = 13U;
    input.button_text_color = 0UL;
    input.disabled_text_color = 0x7fUL;
    input.ops.send_message = send_message;
    install_ops(&input);
    input.context = &trace;
    trace.draw_succeeds = 1U;

    trace.primary = 1U;
    trace.secondary = 2U;
    stars_draw_ship_waypoint_orders(&input);
    if (trace.draws != 1U || trace.hides != 4U || trace.shows < 4U ||
            trace.positions < 3U || trace.diamonds != 1U ||
            trace.references < 3U)
        return 1;

    memset(&trace, 0, sizeof trace);
    trace.draw_succeeds = 1U;
    input.tile_flags = 0U;
    trace.primary = 3U;
    input.mines = 0;
    stars_draw_ship_waypoint_orders(&input);
    if (trace.draws != 1U || trace.last_message != 1U)
        return 2;

    memset(&trace, 0, sizeof trace);
    trace.draw_succeeds = 1U;
    trace.primary = 6U;
    input.layer_mines = 4;
    stars_draw_ship_waypoint_orders(&input);
    if (trace.formatted != 1U || trace.last_message != 1U)
        return 3;

    memset(&trace, 0, sizeof trace);
    input.tile_flags = 0U;
    input.tile_rect.right = 0U;
    trace.draw_succeeds = 0U;
    stars_draw_ship_waypoint_orders(&input);
    if (trace.draws != 1U || trace.hides != 4U)
        return 4;

    puts("DRAW_SHIP_WAYPOINT_ORDERS_CHECKS=4 FAILURES=0 RESULT=PASS");
    return 0;
}
