#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedShipBitmapContext {
    unsigned char selection[226];
    StarsI16 object_kind;
    StarsI16 object_id;
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
    StarsI16 tile_kind;
    StarsI16 tile_id;
    StarsI16 name_id;
    StarsI16 dib_source_x;
    StarsI16 dib_source_y;
} PackedShipBitmapContext;

static void put16(unsigned char *raw, int offset, unsigned short value)
{
    raw[offset] = (unsigned char)(value & 0xffU);
    raw[offset + 1] = (unsigned char)(value >> 8);
}

static unsigned short get16(const unsigned char *raw, int offset)
{
    return (unsigned short)(raw[offset] |
        ((unsigned short)raw[offset + 1] << 8));
}

static void load_selection(PackedShipBitmapContext *packed)
{
    packed->object_kind = (StarsI16)get16(packed->selection, 4);
    packed->object_id = (StarsI16)get16(packed->selection, 8);
}

static StarsI16 name(void *context, StarsI16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    PackedShipBitmapContext *packed = (PackedShipBitmapContext *)context;
    packed->name_id = id;
    if (capacity > 1U) {
        buffer[0] = (StarsU8)'N';
        buffer[1] = 0;
        return 1;
    }
    return 0;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 kind,
    StarsI16 id, StarsRect *rect, const StarsU8 *text)
{
    PackedShipBitmapContext *packed = (PackedShipBitmapContext *)context;
    (void)hdc;
    (void)text;
    packed->tile_kind = kind;
    packed->tile_id = id;
    rect->left = 0;
    rect->top = 0;
    rect->right = 200;
    rect->bottom = 100;
    return 1;
}

static void show(void *context, StarsU16 window, StarsI16 command)
{
    PackedShipBitmapContext *packed = (PackedShipBitmapContext *)context;
    (void)window;
    if (command == 0)
        ++packed->hidden;
    else
        ++packed->shown;
}

static void position(void *context, StarsU16 window, StarsU16 insert_after,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    PackedShipBitmapContext *packed = (PackedShipBitmapContext *)context;
    (void)window;
    (void)insert_after;
    (void)flags;
    ++packed->positioned;
    packed->last_x = x;
    packed->last_y = y;
    packed->last_width = width;
    packed->last_height = height;
}

static StarsU16 draw_fleet(void *context, StarsU16 hdc, StarsU16 object_id)
{
    PackedShipBitmapContext *packed = (PackedShipBitmapContext *)context;
    (void)hdc;
    ++packed->fleet_draws;
    packed->object_id = (StarsI16)object_id;
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
    PackedShipBitmapContext *packed = (PackedShipBitmapContext *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)raster;
    ++packed->patterns;
}

static void dib(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 bitmap, StarsI16 source_x,
    StarsI16 source_y, StarsI16 source_width, StarsI16 source_height,
    StarsU32 raster)
{
    PackedShipBitmapContext *packed = (PackedShipBitmapContext *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)bitmap;
    (void)source_width;
    (void)source_height;
    (void)raster;
    ++packed->dibs;
    packed->dib_source_x = source_x;
    packed->dib_source_y = source_y;
}

static void palette(void *context, StarsU16 hdc, StarsU16 palette_id)
{
    (void)context;
    (void)hdc;
    (void)palette_id;
}

static void install(StarsPlanShipBitmapOps *ops,
    PackedShipBitmapContext *packed)
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
    ops->context = packed;
}

static void reset_trace(PackedShipBitmapContext *packed)
{
    packed->hidden = 0;
    packed->shown = 0;
    packed->positioned = 0;
    packed->fleet_draws = 0;
    packed->patterns = 0;
    packed->dibs = 0;
    packed->last_x = 0;
    packed->last_y = 0;
    packed->last_width = 0;
    packed->last_height = 0;
    packed->tile_kind = 0;
    packed->tile_id = 0;
    packed->name_id = 0;
    packed->dib_source_x = 0;
    packed->dib_source_y = 0;
}

int main(void)
{
    PackedShipBitmapContext packed;
    StarsDrawPlanShipBitmapInput input;
    StarsPlanShipBitmapOps ops;
    int checks;
    int failures;

    memset(&packed, 0, sizeof packed);
    memset(&input, 0, sizeof input);
    put16(packed.selection, 4, 2);
    put16(packed.selection, 8, 4);
    put16(packed.selection, 28, 4);
    put16(packed.selection, 152, 7);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof packed.selection == 226);
    CHECK(get16(packed.selection, 4) == 2U);
    CHECK(get16(packed.selection, 8) == 4U);
    CHECK(get16(packed.selection, 28) == 4U);
    CHECK(get16(packed.selection, 152) == 7U);
    load_selection(&packed);
    CHECK(packed.object_kind == 2 && packed.object_id == 4);

    install(&ops, &packed);
    input.hdc = 3;
    input.tile_flags = 0x0800;
    input.arial8_height = 8;
    input.button_handles[4] = 4;
    input.button_handles[5] = 5;
    input.button_handles[6] = 6;
    input.object_kind = packed.object_kind;
    input.object_id = packed.object_id;
    input.ops = &ops;
    reset_trace(&packed);
    stars_draw_plan_ship_bitmap(&input);
    CHECK(packed.tile_kind == 2 && packed.tile_id == 4 && packed.name_id == 4);
    CHECK(packed.hidden == 3 && packed.shown == 3 && packed.positioned == 3);
    CHECK(packed.fleet_draws == 1 && packed.last_x == 107 &&
        packed.last_width == 93);
    CHECK((input.tile_flags & 0x0800) == 0);

    load_selection(&packed);
    CHECK(packed.object_kind == 2 && packed.object_id == 4);
    put16(packed.selection, 4, 1);
    put16(packed.selection, 8, 7);
    load_selection(&packed);
    input.object_kind = packed.object_kind;
    input.object_id = packed.object_id;
    input.tile_flags = 0x1000;
    input.global_display_flags = 0x0008;
    reset_trace(&packed);
    stars_draw_plan_ship_bitmap(&input);
    CHECK(packed.tile_kind == 1 && packed.tile_id == 7 && packed.name_id == 7);
    CHECK(packed.patterns == 10 && packed.dibs == 1);
    CHECK(packed.dib_source_x == 64 && packed.dib_source_y == 128);
    CHECK(packed.shown == 0 && packed.positioned == 0);

    input.tile_flags = 0;
    packed.object_kind = 1;
    packed.object_id = 7;
    reset_trace(&packed);
    input.ops = &ops;
    /* The typed adapter also covers the failed common-tile path. */
    input.global_display_flags = 0;
    stars_draw_plan_ship_bitmap(&input);
    CHECK(packed.tile_kind == 1 && packed.dibs == 1);
    CHECK(checks == 17);
    puts("DRAW_PLAN_SHIP_BITMAP_PACKED_CHECKS=17 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
