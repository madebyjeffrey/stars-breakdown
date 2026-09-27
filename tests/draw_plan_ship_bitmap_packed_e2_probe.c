#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedE2Trace {
    StarsI16 shown;
    StarsI16 positioned;
    StarsI16 fleet_draws;
    StarsI16 patterns;
    StarsI16 dibs;
    StarsI16 tile_kind;
    StarsI16 tile_id;
    StarsI16 name_id;
    StarsI16 dib_source_x;
    StarsI16 dib_source_y;
} PackedE2Trace;

static void put16(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1] = (StarsU8)(value >> 8);
}

static StarsI16 name(void *context, StarsI16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    PackedE2Trace *trace = (PackedE2Trace *)context;
    trace->name_id = id;
    if (capacity > 1U) {
        buffer[0] = (StarsU8)'N';
        buffer[1] = 0;
    }
    return capacity > 1U ? 1 : 0;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 kind,
    StarsI16 id, StarsRect *rect, const StarsU8 *text)
{
    PackedE2Trace *trace = (PackedE2Trace *)context;
    (void)hdc;
    (void)text;
    trace->tile_kind = kind;
    trace->tile_id = id;
    rect->left = 0;
    rect->top = 0;
    rect->right = 200;
    rect->bottom = 100;
    return 1;
}

static void show(void *context, StarsU16 window, StarsI16 command)
{
    PackedE2Trace *trace = (PackedE2Trace *)context;
    (void)window;
    if (command != 0)
        ++trace->shown;
}

static void position(void *context, StarsU16 window, StarsU16 insert_after,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    PackedE2Trace *trace = (PackedE2Trace *)context;
    (void)window;
    (void)insert_after;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)flags;
    ++trace->positioned;
}

static StarsU16 draw_fleet(void *context, StarsU16 hdc, StarsU16 id)
{
    PackedE2Trace *trace = (PackedE2Trace *)context;
    (void)hdc;
    (void)id;
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
    PackedE2Trace *trace = (PackedE2Trace *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)raster;
    ++trace->patterns;
}

static void dib(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 bitmap, StarsI16 source_x,
    StarsI16 source_y, StarsI16 source_width, StarsI16 source_height,
    StarsU32 raster)
{
    PackedE2Trace *trace = (PackedE2Trace *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)bitmap;
    (void)source_width;
    (void)source_height;
    (void)raster;
    ++trace->dibs;
    trace->dib_source_x = source_x;
    trace->dib_source_y = source_y;
}

static void palette(void *context, StarsU16 hdc, StarsU16 palette_id)
{
    (void)context;
    (void)hdc;
    (void)palette_id;
}

static void install(StarsPlanShipBitmapOps *ops, PackedE2Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->planet_name = name;
    ops->fleet_name = name;
    ops->draw_tile = draw_tile;
    ops->show_window = show;
    ops->set_window_position = position;
    ops->draw_fleet = draw_fleet;
    ops->select_object = select_object;
    ops->pattern_blt = pattern;
    ops->dib_blt = dib;
    ops->select_palette = palette;
    ops->realize_palette = palette;
    ops->context = trace;
}

static void init_input(StarsDrawPlanShipBitmapInput *input,
    StarsPlanShipBitmapOps *ops, const StarsU8 *selection,
    StarsU16 selection_size)
{
    memset(input, 0, sizeof *input);
    input->hdc = 3;
    input->tile_flags = 0;
    input->arial8_height = 8;
    input->button_handles[4] = 4;
    input->button_handles[5] = 5;
    input->button_handles[6] = 6;
    input->object_kind = -1;
    input->object_id = -1;
    input->packed_selection_record = selection;
    input->packed_selection_size = selection_size;
    input->ops = ops;
}

int main(void)
{
    StarsU8 selection[226];
    StarsDrawPlanShipBitmapInput input;
    StarsPlanShipBitmapOps ops;
    PackedE2Trace trace;
    int checks;
    int failures;

    memset(selection, 0, sizeof selection);
    memset(&trace, 0, sizeof trace);
    put16(selection, 4, 2);
    put16(selection, 6, 2);
    put16(selection, 8, 4);
    put16(selection, 28, 4);
    put16(selection, 30, 1);
    put16(selection, 34, 9);
    put16(selection, 152, 7);
    put16(selection, 154, 1);
    install(&ops, &trace);
    init_input(&input, &ops, selection, sizeof selection);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    stars_draw_plan_ship_bitmap(&input);
    CHECK(input.object_kind == 2 && input.object_id == 4);
    CHECK(trace.tile_kind == 2 && trace.tile_id == 4 && trace.name_id == 4);
    CHECK(trace.fleet_draws == 1 && trace.positioned == 3);

    memset(&trace, 0, sizeof trace);
    put16(selection, 4, 1);
    put16(selection, 8, 7);
    input.object_kind = -1;
    input.object_id = -1;
    input.tile_flags = 0x1000;
    input.global_display_flags = 0x0008;
    stars_draw_plan_ship_bitmap(&input);
    CHECK(input.object_kind == 1 && input.object_id == 7);
    CHECK(trace.tile_kind == 1 && trace.tile_id == 7 && trace.name_id == 7);
    CHECK(trace.patterns == 10 && trace.dibs == 1);
    CHECK(trace.dib_source_x == 64 && trace.dib_source_y == 128);
    CHECK(trace.positioned == 0 && trace.shown == 0);

    memset(&trace, 0, sizeof trace);
    input.object_kind = 9;
    input.object_id = 9;
    input.packed_selection_size = 10;
    stars_draw_plan_ship_bitmap(&input);
    CHECK(input.object_kind == 9 && input.object_id == 9);
    CHECK(trace.tile_kind == 0 && trace.fleet_draws == 0 && trace.dibs == 0);
    CHECK(checks == 11);
#ifndef TARGET_NATIVE
    printf("DRAW_PLAN_SHIP_BITMAP_PACKED_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#else
    puts("DRAW_PLAN_SHIP_BITMAP_PACKED_E2_CHECKS=11 FAILURES=0 RESULT=PASS");
#endif
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DPSBE2\\DPSBE2.TXT", "w");
        if (!report)
            return 4;
        fprintf(report,
            "DRAW_PLAN_SHIP_BITMAP_PACKED_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    return failures ? 1 : 0;
#undef CHECK
}
