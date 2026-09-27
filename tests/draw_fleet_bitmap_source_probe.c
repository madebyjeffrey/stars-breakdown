#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 patterns;
    StarsU16 dibs;
    StarsU16 selections;
    StarsU16 palette_selects;
    StarsU16 palette_realizes;
    StarsU16 formats;
    StarsU16 text_outs;
    StarsU16 last_dib;
    StarsI16 last_dib_width;
    StarsI16 last_dib_x;
    StarsI16 last_dib_y;
    StarsI16 last_text_x;
    StarsU16 lookup_calls;
} Trace;

static StarsI16 lookup(void *context, const void *fleet, StarsI16 *differences)
{
    Trace *t = (Trace *)context;
    (void)fleet;
    ++t->lookup_calls;
    *differences = 5;
    return 100;
}

static StarsU16 select_object(void *context, StarsU16 hdc, StarsU16 object)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)object;
    ++t->selections;
    return 99U;
}

static void pattern(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU32 rop)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)rop;
    ++t->patterns;
}

static void select_palette(void *context, StarsU16 hdc, StarsU16 palette,
    StarsI16 background)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)palette;
    (void)background;
    ++t->palette_selects;
}

static void realize_palette(void *context, StarsU16 hdc)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    ++t->palette_realizes;
}

static void draw_dib(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 dib, StarsI16 source_x,
    StarsI16 source_y, StarsI16 source_width, StarsI16 source_height,
    StarsU32 rop)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)height;
    (void)source_x;
    (void)source_y;
    (void)source_width;
    (void)source_height;
    (void)rop;
    ++t->dibs;
    t->last_dib = dib;
    t->last_dib_width = width;
    t->last_dib_x = x;
    t->last_dib_y = y;
}

static StarsI16 format_count(void *context, StarsU8 *out, StarsU16 capacity,
    StarsI16 value)
{
    Trace *t = (Trace *)context;
    (void)capacity;
    (void)value;
    ++t->formats;
    strcpy((char *)out, "12");
    return 2;
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
    (void)y;
    (void)text;
    (void)length;
    ++t->text_outs;
    t->last_text_x = x;
}

static void text_color(void *context, StarsU16 hdc, StarsU32 color)
{
    (void)context;
    (void)hdc;
    (void)color;
}

int main(void)
{
    StarsDrawFleetBitmapInput input;
    Trace trace;
    StarsU16 i;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    input.hdc = 1U;
    input.x = 10;
    input.y = 20;
    input.draw_frame = 1;
    input.bitmap_index = 36;
    input.differences = 3;
    input.shrink = 0;
    input.race_bitmap = 3;
    input.ship_count = 2;
    input.brush_shadow = 1U;
    input.brush_highlight = 2U;
    input.palette = 3U;
    for (i = 0; i < 5U; ++i) {
        input.ship_dibs[i] = (StarsU16)(10U + i);
        input.ship_dibs_small[i] = (StarsU16)(20U + i);
    }
    input.race_dib_full = 30U;
    input.race_dib_small = 31U;
    input.font_arial7 = 4U;
    input.ops.select_object = select_object;
    input.ops.pattern_blt = pattern;
    input.ops.select_palette = select_palette;
    input.ops.realize_palette = realize_palette;
    input.ops.draw_dib = draw_dib;
    input.ops.format_count = format_count;
    input.ops.text_extent = extent;
    input.ops.text_out = text_out;
    input.ops.set_text_color = text_color;
    input.context = &trace;
    stars_draw_fleet_bitmap(&input);
    if (trace.patterns != 6U || trace.dibs != 2U || trace.selections != 4U ||
            trace.palette_selects != 1U || trace.palette_realizes != 1U ||
            trace.formats != 2U || trace.text_outs != 2U ||
            trace.last_dib_width != 16 || trace.last_dib_x != 10 ||
            trace.last_dib_y != 68 || trace.last_text_x != 69)
        return 1;

    memset(&trace, 0, sizeof trace);
    input.draw_frame = 0;
    input.bitmap_index = -1;
    input.differences = 1;
    input.shrink = 1;
    input.race_bitmap = -1;
    input.ship_count = 0;
    input.ops.lookup_primary = lookup;
    stars_draw_fleet_bitmap(&input);
    if (trace.lookup_calls != 1U || trace.dibs != 1U ||
            trace.last_dib != 23U || trace.last_dib_width != 32 ||
            trace.patterns != 6U || trace.text_outs != 0U)
        return 2;
    puts("DRAW_FLEET_BITMAP_SOURCE=PASS");
    return 0;
}
