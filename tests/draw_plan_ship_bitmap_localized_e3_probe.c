#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct LocalizedE3Trace {
    StarsI16 fleet_name_calls;
    StarsI16 planet_name_calls;
    StarsI16 fleet_name_id;
    StarsI16 planet_name_id;
    StarsI16 tile_kind;
    StarsI16 tile_id;
    StarsI16 name_order;
    StarsI16 tile_order;
    StarsU8 tile_text[96];
} LocalizedE3Trace;

static void copy_text(StarsU8 *destination, StarsU16 capacity,
    const char *source)
{
    StarsU16 index;
    if (capacity == 0U)
        return;
    index = 0;
    while (index + 1U < capacity && source[index] != 0) {
        destination[index] = (StarsU8)source[index];
        ++index;
    }
    destination[index] = 0;
}

static StarsI16 fleet_name(void *context, StarsI16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    LocalizedE3Trace *trace = (LocalizedE3Trace *)context;
    trace->fleet_name_calls = (StarsI16)(trace->fleet_name_calls + 1);
    trace->fleet_name_id = id;
    trace->name_order = 1;
    copy_text(buffer, capacity, "Stalwart Defender #5");
    return (StarsI16)strlen("Stalwart Defender #5");
}

static StarsI16 planet_name(void *context, StarsI16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    LocalizedE3Trace *trace = (LocalizedE3Trace *)context;
    trace->planet_name_calls = (StarsI16)(trace->planet_name_calls + 1);
    trace->planet_name_id = id;
    trace->name_order = 1;
    copy_text(buffer, capacity, "Oop Be Gone");
    return (StarsI16)strlen("Oop Be Gone");
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 kind,
    StarsI16 id, StarsRect *rect, const StarsU8 *text)
{
    LocalizedE3Trace *trace = (LocalizedE3Trace *)context;
    (void)hdc;
    trace->tile_kind = kind;
    trace->tile_id = id;
    trace->tile_order = 2;
    strcpy((char *)trace->tile_text, (const char *)text);
    rect->left = 0;
    rect->top = 0;
    rect->right = 200;
    rect->bottom = 100;
    return 1;
}

static void show(void *context, StarsU16 window, StarsI16 command)
{
    (void)context;
    (void)window;
    (void)command;
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
    (void)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)raster;
}

static void dib(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 bitmap, StarsI16 source_x,
    StarsI16 source_y, StarsI16 source_width, StarsI16 source_height,
    StarsU32 raster)
{
    (void)context;
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
}

static void palette(void *context, StarsU16 hdc, StarsU16 palette_id)
{
    (void)context;
    (void)hdc;
    (void)palette_id;
}

static void install(StarsPlanShipBitmapOps *ops, LocalizedE3Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->planet_name = planet_name;
    ops->fleet_name = fleet_name;
    ops->draw_tile = draw_tile;
    ops->show_window = show;
    ops->set_window_position = position;
    ops->select_object = select_object;
    ops->pattern_blt = pattern;
    ops->dib_blt = dib;
    ops->select_palette = palette;
    ops->realize_palette = palette;
    ops->context = trace;
}

static void put16(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1] = (StarsU8)(value >> 8);
}

static void init_input(StarsDrawPlanShipBitmapInput *input,
    StarsPlanShipBitmapOps *ops, const StarsU8 *selection)
{
    memset(input, 0, sizeof *input);
    input->hdc = 3;
    input->tile_flags = 0x1000;
    input->arial8_height = 8;
    input->object_kind = -1;
    input->object_id = -1;
    input->packed_selection_record = selection;
    input->packed_selection_size = 226;
    input->ops = ops;
}

int main(void)
{
    StarsU8 selection[226];
    StarsDrawPlanShipBitmapInput input;
    StarsPlanShipBitmapOps ops;
    LocalizedE3Trace trace;
    int checks;
    int failures;

    memset(selection, 0, sizeof selection);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    put16(selection, 4, 2);
    put16(selection, 8, 4);
    init_input(&input, &ops, selection);
    stars_draw_plan_ship_bitmap(&input);
    CHECK(trace.fleet_name_calls == 1 && trace.planet_name_calls == 0);
    CHECK(trace.fleet_name_id == 4 && trace.tile_kind == 2 &&
        trace.tile_id == 4);
    CHECK(strcmp((const char *)trace.tile_text,
        "Stalwart Defender #5") == 0);
    CHECK(trace.name_order == 1 && trace.tile_order == 2);
    CHECK(input.object_kind == 2 && input.object_id == 4);

    memset(&trace, 0, sizeof trace);
    put16(selection, 4, 1);
    put16(selection, 8, 7);
    init_input(&input, &ops, selection);
    stars_draw_plan_ship_bitmap(&input);
    CHECK(trace.fleet_name_calls == 0 && trace.planet_name_calls == 1);
    CHECK(trace.planet_name_id == 7 && trace.tile_kind == 1 &&
        trace.tile_id == 7);
    CHECK(strcmp((const char *)trace.tile_text, "Oop Be Gone") == 0);
    CHECK(trace.name_order == 1 && trace.tile_order == 2);
    CHECK(input.object_kind == 1 && input.object_id == 7);

    CHECK(checks == 11);
#ifndef TARGET_NATIVE
    printf("DRAW_PLAN_SHIP_BITMAP_LOCALIZED_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#else
    puts("DRAW_PLAN_SHIP_BITMAP_LOCALIZED_E3_CHECKS=11 FAILURES=0 RESULT=PASS");
#endif
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DPSBE3\\DPSBE3.TXT", "w");
        if (!report)
            return 4;
        fprintf(report,
            "DRAW_PLAN_SHIP_BITMAP_LOCALIZED_E3_CHECKS=11 FAILURES=%d RESULT=%s\n",
            failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    return failures ? 1 : 0;
#undef CHECK
}
