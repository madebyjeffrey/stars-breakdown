#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 hidden;
    StarsI16 shown;
    StarsI16 positioned;
    StarsI16 fleet_draws;
    StarsI16 patterns;
    StarsI16 dibs;
    StarsI16 last_x;
    StarsI16 last_y;
    StarsI16 last_width;
    StarsI16 last_height;
} Trace;

static StarsI16 name(void *context, StarsI16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    (void)context;
    (void)id;
    if (capacity > 1U) {
        buffer[0] = 'N';
        buffer[1] = 0;
        return 1;
    }
    return 0;
}

static StarsI16 tile(void *context, StarsU16 hdc, StarsI16 kind,
    StarsI16 id, StarsRect *rect, const StarsU8 *text)
{
    (void)context;
    (void)hdc;
    (void)kind;
    (void)id;
    (void)text;
    rect->left = 0;
    rect->top = 0;
    rect->right = 200;
    rect->bottom = 100;
    return 1;
}

static void show(void *context, StarsU16 hwnd, StarsI16 command)
{
    Trace *trace;
    (void)hwnd;
    trace = (Trace *)context;
    if (command == 0)
        ++trace->hidden;
    else
        ++trace->shown;
}

static void position(void *context, StarsU16 hwnd, StarsU16 insert_after,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    Trace *trace;
    (void)hwnd;
    (void)insert_after;
    (void)flags;
    trace = (Trace *)context;
    ++trace->positioned;
    trace->last_x = x;
    trace->last_y = y;
    trace->last_width = width;
    trace->last_height = height;
}

static StarsU16 object(void *context, StarsU16 hdc, StarsU16 object_id)
{
    Trace *trace;
    (void)hdc;
    (void)object_id;
    trace = (Trace *)context;
    ++trace->fleet_draws;
    return 0;
}

static StarsU16 select_object(void *context, StarsU16 hdc, StarsU16 object_id)
{
    (void)context;
    (void)hdc;
    (void)object_id;
    return 0;
}

static void pattern(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU32 raster)
{
    Trace *trace;
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)raster;
    trace = (Trace *)context;
    ++trace->patterns;
}

static void dib(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 bitmap, StarsI16 source_x,
    StarsI16 source_y, StarsI16 source_width, StarsI16 source_height,
    StarsU32 raster)
{
    Trace *trace;
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)bitmap;
    (void)source_x;
    (void)source_y;
    (void)source_width;
    (void)source_height;
    (void)raster;
    trace = (Trace *)context;
    ++trace->dibs;
}

static void palette(void *context, StarsU16 hdc, StarsU16 palette_id)
{
    (void)context;
    (void)hdc;
    (void)palette_id;
}

int main(void)
{
    StarsDrawPlanShipBitmapInput input;
    StarsPlanShipBitmapOps ops;
    Trace trace;

    (void)memset(&input, 0, sizeof input);
    (void)memset(&ops, 0, sizeof ops);
    (void)memset(&trace, 0, sizeof trace);
    ops.planet_name = name;
    ops.fleet_name = name;
    ops.draw_tile = tile;
    ops.show_window = show;
    ops.set_window_position = position;
    ops.draw_fleet = object;
    ops.select_object = select_object;
    ops.pattern_blt = pattern;
    ops.dib_blt = dib;
    ops.select_palette = palette;
    ops.realize_palette = palette;
    ops.context = &trace;
    input.hdc = 3;
    input.object_kind = 2;
    input.object_id = 4;
    input.tile_flags = 0x0800;
    input.arial8_height = 8;
    input.ops = &ops;
    input.button_handles[4] = 4;
    input.button_handles[5] = 5;
    input.button_handles[6] = 6;
    input.tile_flags = 0x0800;
    input.ops = &ops;
    input.global_display_flags = 0;
    stars_draw_plan_ship_bitmap(&input);
    if (trace.hidden != 3 || trace.shown != 3 || trace.positioned != 3 ||
        trace.fleet_draws != 1 || trace.last_x != 107 ||
        trace.last_width != 93)
        return 1;
    input.object_kind = 1;
    input.tile_flags = 0x1000;
    stars_draw_plan_ship_bitmap(&input);
    if (trace.patterns == 0 || trace.dibs != 1 || trace.shown != 3)
        return 2;
    puts("DRAW_PLAN_SHIP_BITMAP_SOURCE=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DPSBM2\\DPSMR.TXT", "w");
        if (!report)
            return 4;
        fputs("DRAW_PLAN_SHIP_BITMAP_CHECKS=9 FAILURES=0 RESULT=PASS\n",
            report);
        fclose(report);
    }
#endif
    return 0;
}
