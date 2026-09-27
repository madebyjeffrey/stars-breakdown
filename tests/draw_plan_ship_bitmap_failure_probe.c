#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 tile_result;
    StarsI16 tile_calls;
    StarsI16 tile_kind;
    StarsI16 tile_id;
    StarsI16 name_id;
    StarsI16 hidden;
    StarsI16 shown;
    StarsI16 positioned;
    StarsI16 fleet_draws;
    StarsI16 patterns;
    StarsI16 dibs;
} Trace;

static StarsI16 name(void *context, StarsI16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->name_id = id;
    if (capacity < 2U)
        return 0;
    buffer[0] = (StarsU8)'N';
    buffer[1] = 0;
    return 1;
}

static StarsI16 tile(void *context, StarsU16 hdc, StarsI16 kind,
    StarsI16 id, StarsRect *rect, const StarsU8 *text)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hdc;
    (void)text;
    ++trace->tile_calls;
    trace->tile_kind = kind;
    trace->tile_id = id;
    rect->left = 10;
    rect->top = 20;
    rect->right = 210;
    rect->bottom = 100;
    return trace->tile_result;
}

static void show(void *context, StarsU16 window, StarsI16 command)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)window;
    if (command == 0)
        ++trace->hidden;
    else
        ++trace->shown;
}

static void position(void *context, StarsU16 window, StarsU16 insert_after,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    (void)context;
    (void)window;
    (void)insert_after;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)flags;
}

static StarsU16 fleet(void *context, StarsU16 hdc, StarsU16 fleet_id)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hdc;
    (void)fleet_id;
    ++trace->fleet_draws;
    return 1;
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
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)raster;
    ++((Trace *)context)->patterns;
}

static void dib(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 bitmap, StarsI16 source_x,
    StarsI16 source_y, StarsI16 source_width, StarsI16 source_height,
    StarsU32 raster)
{
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
    ++((Trace *)context)->dibs;
}

static void palette(void *context, StarsU16 hdc, StarsU16 palette_id)
{
    (void)context;
    (void)hdc;
    (void)palette_id;
}

static void reset_trace(Trace *trace)
{
    StarsI16 result;

    result = trace->tile_result;
    memset(trace, 0, sizeof *trace);
    trace->tile_result = result;
}

int main(void)
{
    StarsDrawPlanShipBitmapInput input;
    StarsPlanShipBitmapOps ops;
    Trace trace;
    int checks;
    int failures;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.planet_name = name;
    ops.fleet_name = name;
    ops.draw_tile = tile;
    ops.show_window = show;
    ops.set_window_position = position;
    ops.draw_fleet = fleet;
    ops.select_object = select_object;
    ops.pattern_blt = pattern;
    ops.dib_blt = dib;
    ops.select_palette = palette;
    ops.realize_palette = palette;
    ops.context = &trace;
    input.hdc = 3;
    input.object_kind = 2;
    input.object_id = 17;
    input.tile_flags = 0;
    input.arial8_height = 8;
    input.button_handles[4] = 4;
    input.button_handles[5] = 5;
    input.button_handles[6] = 6;
    input.shadow_brush = 11;
    input.hilite_brush = 22;
    input.planet_dib = 44;
    input.palette = 55;
    input.ops = &ops;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    trace.tile_result = 0;
    stars_draw_plan_ship_bitmap(&input);
    CHECK(trace.tile_calls == 1 && trace.tile_kind == 2 &&
        trace.tile_id == 17 && trace.name_id == 17);
    CHECK(trace.hidden == 3 && trace.shown == 0 && trace.positioned == 0);
    CHECK(trace.fleet_draws == 0 && trace.patterns == 0 && trace.dibs == 0);

    reset_trace(&trace);
    trace.tile_result = 1;
    input.tile_flags = 0x1800;
    stars_draw_plan_ship_bitmap(&input);
    CHECK(trace.hidden == 3 && trace.shown == 0 && trace.positioned == 0);
    CHECK(trace.fleet_draws == 1 && trace.patterns == 0 && trace.dibs == 0);
    CHECK((input.tile_flags & 0x0800) == 0);

    printf("DRAW_PLAN_SHIP_BITMAP_FAILURE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
#undef CHECK
}
