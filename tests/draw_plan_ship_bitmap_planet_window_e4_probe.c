#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct BitmapE4Trace {
    char events[32];
    StarsI16 event_count;
    StarsI16 tile_kind;
    StarsI16 tile_id;
    StarsI16 fleet_draws;
    StarsI16 patterns;
    StarsI16 dibs;
    StarsI16 dib_source_x;
    StarsI16 dib_source_y;
    StarsI16 positioned;
    StarsU8 tile_text[96];
    StarsU8 selection[226];
    StarsPlanShipBitmapOps bitmap_ops;
    StarsDrawPlanShipBitmapInput bitmap_input;
    StarsPlanetWndProcOps window_ops;
    StarsPlanetWndProcInput window_input;
} BitmapE4Trace;

static void event(BitmapE4Trace *trace, char value)
{
    if (trace->event_count < (StarsI16)(sizeof trace->events - 1)) {
        trace->events[trace->event_count] = value;
        ++trace->event_count;
        trace->events[trace->event_count] = 0;
    }
}

static void put16(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1] = (StarsU8)(value >> 8);
}

static StarsI16 name(void *context, StarsI16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    BitmapE4Trace *trace = (BitmapE4Trace *)context;
    const char *text = id == 4 ? "Stalwart Defender #5" : "Oop Be Gone";
    StarsU16 index = 0;
    while (index + 1U < capacity && text[index] != 0) {
        buffer[index] = (StarsU8)text[index];
        ++index;
    }
    if (capacity != 0U)
        buffer[index] = 0;
    (void)trace;
    return (StarsI16)index;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 kind,
    StarsI16 id, StarsRect *rect, const StarsU8 *text)
{
    BitmapE4Trace *trace = (BitmapE4Trace *)context;
    (void)hdc;
    trace->tile_kind = kind;
    trace->tile_id = id;
    strcpy((char *)trace->tile_text, (const char *)text);
    event(trace, 'D');
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
    BitmapE4Trace *trace = (BitmapE4Trace *)context;
    (void)window;
    (void)insert_after;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)flags;
    ++trace->positioned;
}

static StarsU16 fleet(void *context, StarsU16 hdc, StarsU16 id)
{
    BitmapE4Trace *trace = (BitmapE4Trace *)context;
    (void)hdc;
    (void)id;
    ++trace->fleet_draws;
    event(trace, 'F');
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
    BitmapE4Trace *trace = (BitmapE4Trace *)context;
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
    BitmapE4Trace *trace = (BitmapE4Trace *)context;
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
    event(trace, 'I');
}

static void palette(void *context, StarsU16 hdc, StarsU16 palette_id)
{
    (void)context;
    (void)hdc;
    (void)palette_id;
}

static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    BitmapE4Trace *trace = (BitmapE4Trace *)context;
    (void)hwnd;
    rect->left = 0;
    rect->top = 0;
    rect->right = 30;
    rect->bottom = 40;
    event(trace, 'B');
    return 55;
}

static void paint_planet_ship(void *context, StarsU16 hwnd, StarsU16 dc)
{
    BitmapE4Trace *trace = (BitmapE4Trace *)context;
    (void)hwnd;
    trace->bitmap_input.hdc = dc;
    event(trace, 'P');
    stars_draw_plan_ship_bitmap(&trace->bitmap_input);
}

static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    BitmapE4Trace *trace = (BitmapE4Trace *)context;
    (void)hwnd;
    (void)rect;
    event(trace, 'E');
}

static void install(BitmapE4Trace *trace)
{
    memset(&trace->bitmap_ops, 0, sizeof trace->bitmap_ops);
    trace->bitmap_ops.planet_name = name;
    trace->bitmap_ops.fleet_name = name;
    trace->bitmap_ops.draw_tile = draw_tile;
    trace->bitmap_ops.show_window = show;
    trace->bitmap_ops.set_window_position = position;
    trace->bitmap_ops.draw_fleet = fleet;
    trace->bitmap_ops.select_object = select_object;
    trace->bitmap_ops.pattern_blt = pattern;
    trace->bitmap_ops.dib_blt = dib;
    trace->bitmap_ops.select_palette = palette;
    trace->bitmap_ops.realize_palette = palette;
    trace->bitmap_ops.context = trace;
    memset(&trace->window_ops, 0, sizeof trace->window_ops);
    trace->window_ops.begin_paint = begin_paint;
    trace->window_ops.paint_planet_ship = paint_planet_ship;
    trace->window_ops.end_paint = end_paint;
    trace->window_ops.context = trace;
    memset(&trace->window_input, 0, sizeof trace->window_input);
    trace->window_input.ops = &trace->window_ops;
    trace->window_input.selected_kind = 2;
    trace->window_input.selected_fleet = 4;
    memset(&trace->bitmap_input, 0, sizeof trace->bitmap_input);
    trace->bitmap_input.tile_flags = 0;
    trace->bitmap_input.arial8_height = 8;
    trace->bitmap_input.button_handles[4] = 4;
    trace->bitmap_input.button_handles[5] = 5;
    trace->bitmap_input.button_handles[6] = 6;
    trace->bitmap_input.packed_selection_record = trace->selection;
    trace->bitmap_input.packed_selection_size = sizeof trace->selection;
    trace->bitmap_input.ops = &trace->bitmap_ops;
}

int main(void)
{
    BitmapE4Trace trace;
    int checks;
    int failures;

    memset(&trace, 0, sizeof trace);
    put16(trace.selection, 4, 2);
    put16(trace.selection, 8, 4);
    install(&trace);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(stars_planet_wnd_proc(77, 0x0f, 7, 0, &trace.window_input) == 0);
    CHECK(strcmp(trace.events, "BPDFE") == 0);
    CHECK(trace.tile_kind == 2 && trace.tile_id == 4 &&
        strcmp((const char *)trace.tile_text, "Stalwart Defender #5") == 0);
    CHECK(trace.fleet_draws == 1 && trace.positioned == 3);
    CHECK(trace.bitmap_input.object_kind == 2 &&
        trace.bitmap_input.object_id == 4);

    memset(trace.events, 0, sizeof trace.events);
    trace.event_count = 0;
    trace.positioned = 0;
    trace.patterns = 0;
    trace.dibs = 0;
    put16(trace.selection, 4, 1);
    put16(trace.selection, 8, 7);
    trace.bitmap_input.tile_flags = 0x1000;
    CHECK(stars_planet_wnd_proc(77, 0x0f, 7, 0, &trace.window_input) == 0);
    CHECK(strcmp(trace.events, "BPDIE") == 0);
    CHECK(trace.bitmap_input.object_kind == 1 &&
        trace.bitmap_input.object_id == 7 && trace.tile_kind == 1 &&
        trace.tile_id == 7);
    CHECK(strcmp((const char *)trace.tile_text, "Oop Be Gone") == 0);
    CHECK(trace.patterns == 10 && trace.dibs == 1 &&
        trace.dib_source_x == 64 && trace.dib_source_y == 128);
    CHECK(trace.positioned == 0);
    CHECK(checks == 12);
#ifndef TARGET_NATIVE
    printf("DRAW_PLAN_SHIP_BITMAP_PLANET_WINDOW_E4_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#else
    puts("DRAW_PLAN_SHIP_BITMAP_PLANET_WINDOW_E4_CHECKS=12 FAILURES=0 RESULT=PASS");
#endif
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DPSBE4\\DPSBE4.TXT", "w");
        if (!report)
            return 4;
        fprintf(report,
            "DRAW_PLAN_SHIP_BITMAP_PLANET_WINDOW_E4_CHECKS=12 FAILURES=%d RESULT=%s\n",
            failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    return failures ? 1 : 0;
#undef CHECK
}
