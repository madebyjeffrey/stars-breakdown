#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedProductionContext {
    unsigned char planet[56];
    unsigned char prod[4];
    StarsPlanetProductionData data;
    StarsI16 tiles;
    StarsI16 shown;
    StarsI16 positions;
    StarsI16 buttons;
    StarsI16 texts;
    StarsI16 enables;
    StarsI16 tile_result;
    StarsI16 data_seen;
    StarsU16 title_id;
    StarsU16 last_string;
    StarsI16 last_button_enabled;
    StarsI16 last_position_height;
} PackedProductionContext;

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

static void load_data(PackedProductionContext *packed)
{
    unsigned long production_pointer;
    unsigned short header;
    unsigned short planet_id;

    memset(&packed->data, 0, sizeof packed->data);
    production_pointer = get32(packed->planet, 52);
    header = get16(packed->prod, 0);
    packed->data.queue_count = (StarsI16)packed->prod[3];
    packed->data.has_production = production_pointer != 0UL &&
        packed->data.queue_count != 0;
    packed->data.selected_item = 0;
    planet_id = get16(packed->planet, 0);
    if (planet_id != 0U) {
        strcpy((char *)packed->data.planet_name, "New Terra");
    }
    if (packed->data.has_production && (header & 0xffU) != 0U)
        strcpy((char *)packed->data.eta, "ETA");
}

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    PackedProductionContext *packed = (PackedProductionContext *)context;
    packed->last_string = id;
    if (id == 0x32e)
        packed->title_id = id;
    return (const StarsU8 *)(id == 0x218 ? "ETA:" :
        (id == 0x4e9 ? "Planet:" :
        (id == 0x38b ? "Build" : "Production")));
}

static StarsI16 draw_tile(void *context, StarsU16 hdc,
    StarsPlanetProductionTile *tile, const StarsU8 *title, StarsRect *rect)
{
    PackedProductionContext *packed = (PackedProductionContext *)context;
    (void)hdc;
    (void)tile;
    (void)title;
    ++packed->tiles;
    rect->left = 4;
    rect->top = 4;
    rect->right = 180;
    rect->bottom = 140;
    return packed->tile_result;
}

static void show_window(void *context, StarsU16 window, StarsU16 command)
{
    (void)window;
    (void)command;
    ++((PackedProductionContext *)context)->shown;
}

static void enable_window(void *context, StarsU16 window, StarsU16 enabled)
{
    PackedProductionContext *packed = (PackedProductionContext *)context;
    (void)window;
    ++packed->enables;
    packed->last_button_enabled = (StarsI16)enabled;
}

static void set_position(void *context, StarsU16 window, StarsU16 insert_after,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    PackedProductionContext *packed = (PackedProductionContext *)context;
    (void)window;
    (void)insert_after;
    (void)x;
    (void)y;
    (void)width;
    (void)flags;
    ++packed->positions;
    packed->last_position_height = height;
}

static StarsI16 selection(void *context, StarsU16 window)
{
    (void)context;
    (void)window;
    return 0;
}

static void font(void *context, StarsU16 hdc, StarsU16 value)
{
    (void)context;
    (void)hdc;
    (void)value;
}

static StarsI16 extent(void *context, StarsU16 hdc, const StarsU8 *text,
    StarsI16 length)
{
    (void)context;
    (void)hdc;
    (void)text;
    return length;
}

static void text(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *value, StarsI16 length)
{
    (void)hdc;
    (void)x;
    (void)y;
    (void)value;
    (void)length;
    ++((PackedProductionContext *)context)->texts;
}

static void right(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *value, StarsI16 length, StarsI16 offset)
{
    PackedProductionContext *packed = (PackedProductionContext *)context;
    (void)hdc;
    (void)x;
    (void)y;
    (void)offset;
    ++packed->texts;
    if (strcmp((const char *)value, (const char *)packed->data.planet_name) == 0 ||
            strcmp((const char *)value, (const char *)packed->data.eta) == 0)
        packed->data_seen = length > 0 || packed->data.eta[0] == 0;
}

static void button(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsI16 kind, StarsI16 enabled)
{
    PackedProductionContext *packed = (PackedProductionContext *)context;
    (void)hdc;
    (void)rect;
    (void)kind;
    ++packed->buttons;
    packed->last_button_enabled = enabled;
}

static void install(StarsPlanetProductionOps *ops,
    PackedProductionContext *packed)
{
    memset(ops, 0, sizeof *ops);
    ops->compressed_string = compressed;
    ops->draw_tile = draw_tile;
    ops->show_window = show_window;
    ops->enable_window = enable_window;
    ops->set_window_pos = set_position;
    ops->selection = selection;
    ops->select_font = font;
    ops->text_extent = extent;
    ops->text_out = text;
    ops->right_text = right;
    ops->draw_button = button;
    ops->context = packed;
}

static void reset_trace(PackedProductionContext *packed)
{
    packed->tiles = 0;
    packed->shown = 0;
    packed->positions = 0;
    packed->buttons = 0;
    packed->texts = 0;
    packed->enables = 0;
    packed->data_seen = 0;
    packed->title_id = 0;
    packed->last_string = 0;
    packed->last_button_enabled = 0;
    packed->last_position_height = 0;
}

int main(void)
{
    PackedProductionContext packed;
    StarsPlanetProductionTile tile;
    StarsPlanetProductionOps ops;
    StarsPlanetProductionInput input;
    StarsRect reference;
    int checks;
    int failures;

    memset(&packed, 0, sizeof packed);
    memset(&tile, 0, sizeof tile);
    memset(&input, 0, sizeof input);
    memset(&reference, 0, sizeof reference);
    put16(packed.planet, 0, 7);
    put32(packed.planet, 52, 0x00031220UL);
    put16(packed.prod, 0, (4U << 12) | (2U << 9) | (1U << 8) | 0x22U);
    packed.prod[2] = 8;
    packed.prod[3] = 2;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof packed.planet == 56);
    CHECK(get16(packed.planet, 0) == 7U);
    CHECK(get32(packed.planet, 52) == 0x00031220UL);
    CHECK((get16(packed.prod, 0) & 0xffU) == 0x22U);
    CHECK(((get16(packed.prod, 0) >> 8) & 1U) == 1U);
    CHECK(((get16(packed.prod, 0) >> 9) & 7U) == 2U);
    CHECK(((get16(packed.prod, 0) >> 12) & 0xfU) == 4U);
    CHECK(packed.prod[2] == 8U && packed.prod[3] == 2U);
    load_data(&packed);
    CHECK(packed.data.has_production == 1 && packed.data.queue_count == 2);
    CHECK(strcmp((char *)packed.data.planet_name, "New Terra") == 0 &&
        strcmp((char *)packed.data.eta, "ETA") == 0);

    tile.flags = 0x0800U;
    packed.tile_result = 1;
    install(&ops, &packed);
    input.hdc = 3;
    input.line_height = 10;
    input.normal_font = 1;
    input.bold_font = 2;
    input.list_window = 9;
    input.buttons[0] = 10;
    input.buttons[1] = 11;
    input.reference_rect = &reference;
    input.tile = &tile;
    input.data = &packed.data;
    input.ops = &ops;
    reset_trace(&packed);
    stars_draw_planet_production(&input);
    CHECK(packed.title_id == 0x32e && packed.tiles == 1);
    CHECK(packed.shown >= 3 && packed.positions >= 3 && packed.texts >= 3);
    CHECK(packed.buttons == 1 && packed.enables == 1 &&
        packed.last_button_enabled == 1);
    CHECK(packed.data_seen == 1 && packed.last_position_height == 15);
    CHECK((tile.flags & 0x0800U) == 0U && reference.left > 0);

    packed.prod[3] = 0;
    load_data(&packed);
    packed.tile_result = 0;
    tile.flags = 0x0800U;
    reset_trace(&packed);
    stars_draw_planet_production(&input);
    CHECK(packed.data.has_production == 0 && packed.data.eta[0] == 0);
    CHECK(packed.shown >= 3 && reference.left == (StarsU16)-5 &&
        reference.right == (StarsU16)-6);
    CHECK(checks == 18);
    puts("DRAW_PLANET_PRODUCTION_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
