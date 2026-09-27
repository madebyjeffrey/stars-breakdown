#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 selects;
    StarsU16 selected[3];
    StarsU16 selected_old;
    StarsI16 patterns;
    StarsI16 pattern_ok;
    StarsI16 dibs;
    StarsI16 dib_ok;
    StarsI16 palettes;
    StarsI16 realizes;
    StarsI16 fleet_draws;
    StarsI16 positions;
    StarsI16 shows;
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
    if (capacity < 2U) return 0;
    buffer[0] = (StarsU8)'N';
    buffer[1] = 0;
    return 1;
}

static StarsI16 tile(void *context, StarsU16 hdc, StarsI16 kind,
    StarsI16 id, StarsRect *rect, const StarsU8 *text)
{
    (void)context;
    (void)hdc;
    (void)kind;
    (void)id;
    (void)text;
    rect->left = 10;
    rect->top = 20;
    rect->right = 210;
    rect->bottom = 100;
    return 1;
}

static void show(void *context, StarsU16 hwnd, StarsI16 command)
{
    (void)hwnd;
    if (command == 0)
        ++((Trace *)context)->shows;
}

static void position(void *context, StarsU16 hwnd, StarsU16 insert_after,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    (void)insert_after;
    (void)flags;
    ++trace->positions;
    trace->last_x = x;
    trace->last_y = y;
    trace->last_width = width;
    trace->last_height = height;
}

static StarsU16 select_object(void *context, StarsU16 hdc, StarsU16 object_id)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    if (trace->selects < 3)
        trace->selected[trace->selects] = object_id;
    ++trace->selects;
    if (trace->selects == 1) {
        trace->selected_old = 77;
        return trace->selected_old;
    }
    return 0;
}

static void pattern(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU32 raster)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    if (trace->patterns == 0)
        trace->pattern_ok = x == 22 && y == 26 && width == 0x46 &&
            height == 2 && raster == 0xf00021UL;
    ++trace->patterns;
}

static void dib(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 bitmap, StarsI16 source_x,
    StarsI16 source_y, StarsI16 source_width, StarsI16 source_height,
    StarsU32 raster)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    trace->dib_ok = x == 25 && y == 29 && width == 0x40 && height == 0x40 &&
        bitmap == 44 && source_x == 320 && source_y == 64 &&
        source_width == 0x40 && source_height == 0x40 &&
        raster == 0xcc0020UL;
    ++trace->dibs;
}

static void palette(void *context, StarsU16 hdc, StarsU16 palette_id)
{
    (void)hdc;
    (void)palette_id;
    ++((Trace *)context)->palettes;
}

static void realize(void *context, StarsU16 hdc, StarsU16 ignored)
{
    (void)hdc;
    (void)ignored;
    ++((Trace *)context)->realizes;
}

static StarsU16 draw_fleet(void *context, StarsU16 hdc, StarsU16 fleet_id)
{
    (void)hdc;
    (void)fleet_id;
    ++((Trace *)context)->fleet_draws;
    return 1;
}

static void check(int condition, int *checks, int *failures)
{
    ++*checks;
    if (!condition) ++*failures;
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
    ops.draw_fleet = draw_fleet;
    ops.select_object = select_object;
    ops.pattern_blt = pattern;
    ops.dib_blt = dib;
    ops.select_palette = palette;
    ops.realize_palette = realize;
    ops.context = &trace;
    ops.context = &trace;
    input.hdc = 3;
    input.object_kind = 1;
    input.object_id = 4;
    input.tile_flags = 0x1000;
    input.arial8_height = 8;
    input.shadow_brush = 11;
    input.hilite_brush = 22;
    input.planet_dib = 44;
    input.palette = 55;
    input.global_display_flags = 0;
    input.ops = &ops;
    checks = 0;
    failures = 0;

    stars_draw_plan_ship_bitmap(&input);
    check(trace.selects == 3 && trace.selected[0] == 11 &&
        trace.selected[1] == 22 && trace.selected[2] == 77, &checks,
        &failures);
    check(trace.patterns == 10 && trace.pattern_ok, &checks, &failures);
    check(trace.dibs == 1 && trace.dib_ok, &checks, &failures);
    check(trace.palettes == 1 && trace.realizes == 1, &checks, &failures);
    check(trace.fleet_draws == 0 && trace.positions == 0, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    input.object_kind = 2;
    input.tile_flags = 0;
    stars_draw_plan_ship_bitmap(&input);
    check(trace.fleet_draws == 1 && trace.patterns == 0 && trace.dibs == 0,
        &checks, &failures);
    check(trace.positions == 3 && trace.last_x == 117 &&
        trace.last_width == 93 && trace.last_height == 12, &checks,
        &failures);
    check(trace.shows == 0, &checks, &failures);

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("DPSBGERES.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "DRAW_PLAN_SHIP_BITMAP_GDI_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    printf("DRAW_PLAN_SHIP_BITMAP_GDI_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
