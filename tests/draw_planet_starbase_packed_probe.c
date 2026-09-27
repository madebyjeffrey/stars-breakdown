#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedStarbaseContext {
    unsigned char planet[56];
    StarsPlanetStarbaseData data;
    StarsI16 tiles;
    StarsI16 text;
    StarsI16 right;
    StarsI16 references;
    StarsI16 buttons;
    StarsI16 gauges;
    StarsI16 bars;
    StarsI16 colors;
    StarsI16 gauge_best;
    StarsI16 no_warp;
    StarsU16 title_id;
    StarsI16 data_seen;
} PackedStarbaseContext;

static void put16(unsigned char *raw, int offset, unsigned short value)
{
    raw[offset] = (unsigned char)(value & 0xffU);
    raw[offset + 1] = (unsigned char)(value >> 8);
}

static void put32(unsigned char *raw, int offset, unsigned long value)
{
    put16(raw, offset, (unsigned short)(value & 0xffffUL));
    put16(raw, offset + 2, (unsigned short)(value >> 16));
}

static unsigned short get16(const unsigned char *raw, int offset)
{
    return (unsigned short)(raw[offset] |
        ((unsigned short)raw[offset + 1] << 8));
}

static unsigned long get32(const unsigned char *raw, int offset)
{
    return (unsigned long)get16(raw, offset) |
        ((unsigned long)get16(raw, offset + 2) << 16);
}

static void load_data(PackedStarbaseContext *packed)
{
    unsigned short flags;

    memset(&packed->data, 0, sizeof packed->data);
    flags = get16(packed->planet, 4);
    packed->data.has_starbase = (StarsI16)((flags >> 9) & 1U);
    packed->data.hull_id = 2;
    packed->data.cargo_capacity = 20;
    packed->data.defense = 15;
    packed->data.mass_warp_packed = get16(packed->planet, 44);
    packed->data.planet_id = (StarsI16)get16(packed->planet, 0);
}

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    PackedStarbaseContext *packed = (PackedStarbaseContext *)context;
    packed->title_id = id;
    return id == 0x343 ? (const StarsU8 *)"No base" :
        (const StarsU8 *)"Base";
}

static StarsI16 get_text(void *context, StarsU16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    const char *text = id == 0x560 ? "-" : "Label";
    StarsI16 index;
    (void)context;
    for (index = 0; index + 1 < (StarsI16)capacity && text[index] != 0;
         ++index)
        buffer[index] = (StarsU8)text[index];
    if (capacity != 0U)
        buffer[index] = 0;
    return index;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc,
    const StarsPlanetStarbaseTile *tile, const StarsU8 *title, StarsRect *rect)
{
    PackedStarbaseContext *packed = (PackedStarbaseContext *)context;
    (void)hdc;
    (void)tile;
    (void)title;
    ++packed->tiles;
    rect->left = 10;
    rect->top = 4;
    rect->right = 170;
    rect->bottom = 130;
    return 1;
}

static void font(void *context, StarsU16 hdc, StarsU16 value)
{
    (void)context;
    (void)hdc;
    (void)value;
}

static void text(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *buffer, StarsI16 length)
{
    (void)hdc;
    (void)x;
    (void)y;
    (void)buffer;
    (void)length;
    ++((PackedStarbaseContext *)context)->text;
}

static void right(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *buffer, StarsI16 length, StarsI16 offset)
{
    (void)hdc;
    (void)x;
    (void)y;
    (void)buffer;
    (void)length;
    (void)offset;
    ++((PackedStarbaseContext *)context)->right;
}

static StarsI32 extent(void *context, StarsU16 hdc, const StarsU8 *buffer,
    StarsI16 length)
{
    (void)context;
    (void)hdc;
    (void)buffer;
    return (StarsI32)(length * 2);
}

static void color(void *context, StarsU16 hdc, StarsU32 value)
{
    (void)hdc;
    (void)value;
    ++((PackedStarbaseContext *)context)->colors;
}

static void bar(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU32 rop)
{
    (void)hdc;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)rop;
    ++((PackedStarbaseContext *)context)->bars;
}

static void reference(void *context, StarsU16 id, const StarsRect *rect)
{
    (void)id;
    (void)rect;
    ++((PackedStarbaseContext *)context)->references;
}

static void button(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsI16 kind, StarsI16 enabled)
{
    (void)hdc;
    (void)rect;
    (void)kind;
    (void)enabled;
    ++((PackedStarbaseContext *)context)->buttons;
}

static void gauge(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsI16 value, StarsI16 best)
{
    PackedStarbaseContext *packed = (PackedStarbaseContext *)context;
    (void)hdc;
    (void)rect;
    (void)value;
    ++packed->gauges;
    packed->gauge_best = best;
}

static StarsI32 shield(void *context, StarsI16 hull, StarsI16 planet)
{
    (void)context;
    (void)hull;
    (void)planet;
    return 25;
}

static StarsI16 warp(void *context, StarsI16 planet, StarsI16 *sign)
{
    PackedStarbaseContext *packed = (PackedStarbaseContext *)context;
    (void)planet;
    *sign = 0;
    return packed->no_warp ? 0 : 5;
}

static StarsI16 format_value(void *context, StarsU8 *buffer, StarsU16 capacity,
    StarsI32 value, StarsI16 mode)
{
    (void)context;
    (void)capacity;
    (void)mode;
    buffer[0] = (StarsU8)('0' + (value % 10));
    buffer[1] = 0;
    return 1;
}

static void install(StarsPlanetStarbaseOps *ops,
    PackedStarbaseContext *packed)
{
    memset(ops, 0, sizeof *ops);
    ops->compressed_string = compressed;
    ops->get_text = get_text;
    ops->draw_tile = draw_tile;
    ops->select_font = font;
    ops->text_out = text;
    ops->right_text = right;
    ops->text_extent = extent;
    ops->set_text_color = color;
    ops->pat_blt = bar;
    ops->set_reference = reference;
    ops->draw_button = button;
    ops->draw_gauge = gauge;
    ops->shield_value = shield;
    ops->warp_value = warp;
    ops->format_value = format_value;
    ops->context = packed;
}

static void reset_trace(PackedStarbaseContext *packed)
{
    packed->tiles = 0;
    packed->text = 0;
    packed->right = 0;
    packed->references = 0;
    packed->buttons = 0;
    packed->gauges = 0;
    packed->bars = 0;
    packed->colors = 0;
    packed->gauge_best = 0;
    packed->title_id = 0;
    packed->data_seen = 0;
}

int main(void)
{
    PackedStarbaseContext packed;
    StarsPlanetStarbaseTile tile;
    StarsPlanetStarbaseOps ops;
    StarsPlanetStarbaseInput input;
    StarsRect references[17];
    int checks;
    int failures;

    memset(&packed, 0, sizeof packed);
    memset(&tile, 0, sizeof tile);
    memset(&input, 0, sizeof input);
    memset(references, 0, sizeof references);
    put16(packed.planet, 0, 4);
    put16(packed.planet, 4, 0x0200U);
    put32(packed.planet, 44, 80UL);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof packed.planet == 56);
    CHECK(get16(packed.planet, 0) == 4);
    CHECK(get16(packed.planet, 4) == 0x0200U);
    CHECK(((get16(packed.planet, 4) >> 9) & 1U) == 1U);
    CHECK((get16(packed.planet, 44) >> 4) == 5U);
    CHECK(get32(packed.planet, 44) == 80UL);

    load_data(&packed);
    CHECK(packed.data.has_starbase == 1 && packed.data.planet_id == 4);
    CHECK(packed.data.mass_warp_packed == 80 &&
        packed.data.cargo_capacity == 20 && packed.data.defense == 15);
    tile.flags = 0x0800U;
    install(&ops, &packed);
    input.hdc = 3;
    input.line_height = 10;
    input.normal_font = 1;
    input.bold_font = 2;
    input.highlight_color = 0x7f;
    input.tile = &tile;
    input.data = &packed.data;
    input.reference_rects = references;
    input.ops = &ops;
    reset_trace(&packed);
    stars_draw_planet_starbase(&input);
    CHECK(packed.title_id == 0x302 && packed.tiles == 1);
    CHECK(packed.text >= 6 && packed.right >= 4 && packed.buttons == 1);
    CHECK(packed.gauges == 1 && packed.gauge_best == 4);
    CHECK(references[13].left == 14 && references[14].left == 14);
    CHECK((tile.flags & 0x0800U) == 0U);
    packed.no_warp = 1;
    stars_draw_planet_starbase(&input);
    CHECK(references[15].right == (StarsU16)-6 &&
        references[16].right == (StarsU16)-6);
    put16(packed.planet, 4, 0U);
    load_data(&packed);
    tile.flags = 0x0800U;
    reset_trace(&packed);
    stars_draw_planet_starbase(&input);
    CHECK(packed.title_id == 0x343 && packed.tiles == 1);
    CHECK(references[14].left == (StarsU16)-5 &&
        references[16].right == (StarsU16)-6);
    CHECK(checks == 17);
    puts("DRAW_PLANET_STARBASE_PACKED_CHECKS=17 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
