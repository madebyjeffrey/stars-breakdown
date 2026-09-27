#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 draws;
    StarsU16 text_outs;
    StarsU16 right_texts;
    StarsU16 gauges;
    StarsU16 references;
    StarsU16 formats;
    StarsU16 labels[8];
    StarsU16 font_selects;
    StarsI16 last_gauge_mode;
    StarsI16 first_gauge_left;
    StarsI16 first_gauge_top;
    StarsI16 first_text_length;
} Trace;

static StarsI16 get_text(void *context, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    Trace *t = (Trace *)context;
    StarsI16 length = id == 0x02eaU ? 4 : 1;
    (void)capacity;
    if (t->draws < 8U)
        t->labels[t->draws++] = id;
    strcpy((char *)out, id == 0x02eaU ? "LONG" : "T");
    return length;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsU16 flags,
    const StarsU8 *title, StarsRect *rect)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)flags;
    (void)title;
    t->draws = 0;
    rect->left = 10U;
    rect->top = 20U;
    rect->right = 100U;
    rect->bottom = 100U;
    return 1;
}

static void select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)font;
    ++t->font_selects;
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
    ++t->text_outs;
    if (t->text_outs == 1U)
        t->first_text_length = length;
}

static void set_color(void *context, StarsU16 hdc, StarsU32 color)
{
    (void)context;
    (void)hdc;
    (void)color;
}

static void reference(void *context, StarsU16 index, const StarsRect *rect)
{
    Trace *t = (Trace *)context;
    (void)index;
    (void)rect;
    ++t->references;
}

static void gauge(void *context, StarsU16 hdc, const StarsRect *rect,
    const void *planet, StarsI16 mode)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)planet;
    ++t->gauges;
    t->last_gauge_mode = mode;
    if (t->gauges == 1U) {
        t->first_gauge_left = (StarsI16)rect->left;
        t->first_gauge_top = (StarsI16)rect->top;
    }
}

static StarsI16 format_value(void *context, StarsU8 *out, StarsU16 capacity,
    StarsI32 value)
{
    Trace *t = (Trace *)context;
    (void)capacity;
    (void)value;
    ++t->formats;
    strcpy((char *)out, "1");
    return 1;
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

int main(void)
{
    StarsDrawShipCargoInput input;
    StarsI32 values[5] = { 1L, 2L, 3L, 4L, 5L };
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    input.hdc = 1U;
    input.tile_flags = 0x0800U;
    input.grid_bit = 0U;
    input.arial8_height = 10;
    input.max_mineral_quantity_width = 8;
    input.normal_font = 1U;
    input.bold_font = 2U;
    input.mineral_colors[0] = 10UL;
    input.mineral_colors[1] = 11UL;
    input.mineral_colors[2] = 12UL;
    input.button_text_color = 13UL;
    input.planet = &input;
    input.resource_values = values;
    input.mineral_names[0] = (const StarsU8 *)"A";
    input.mineral_names[1] = (const StarsU8 *)"B";
    input.mineral_names[2] = (const StarsU8 *)"C";
    input.tile_rect.left = 0U;
    input.tile_rect.top = 0U;
    input.tile_rect.right = 120U;
    input.tile_rect.bottom = 120U;
    input.ops.get_text = get_text;
    input.ops.draw_tile = draw_tile;
    input.ops.select_font = select_font;
    input.ops.text_extent = extent;
    input.ops.text_out = text_out;
    input.ops.set_text_color = set_color;
    input.ops.set_reference = reference;
    input.ops.draw_gauge = gauge;
    input.ops.format_value = format_value;
    input.ops.right_text = right_text;
    input.context = &trace;
    stars_draw_ship_cargo(&input);
    if (trace.gauges != 2U || trace.last_gauge_mode != 5 ||
            trace.first_gauge_left != 22 || trace.first_gauge_top != 21 ||
            trace.text_outs != 6U || trace.right_texts != 4U ||
            trace.formats != 4U || trace.references != 4U ||
            trace.first_text_length != 4)
        return 1;

    memset(&trace, 0, sizeof trace);
    input.tile_flags = 0U;
    input.grid_bit = 1U;
    stars_draw_ship_cargo(&input);
    if (trace.gauges != 2U || trace.text_outs != 2U ||
            trace.right_texts != 0U)
        return 2;
    puts("DRAW_SHIP_CARGO_SOURCE=PASS");
    return 0;
}
