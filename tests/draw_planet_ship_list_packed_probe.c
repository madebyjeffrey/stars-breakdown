#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedShipListContext {
    unsigned char selection[226];
    unsigned char xfer[128];
    StarsI16 combo_value;
    StarsI16 decoded_planet_id;
    StarsI16 decoded_transfer_id;
    StarsI16 decoded_transfer_kind;
    StarsI16 decoded_fleet_owner;
    StarsI16 decoded_thing_owner;
    StarsI16 decoded_thing_kind;
    StarsI16 shown;
    StarsI16 hidden;
    StarsI16 resets;
    StarsI16 positions;
    StarsI16 enables;
    StarsI16 fills;
    StarsI16 draw_calls;
    StarsI16 last_title;
    StarsI16 last_button;
    StarsI16 last_enabled;
} PackedShipListContext;

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

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 title,
    StarsRect *rect)
{
    PackedShipListContext *packed = (PackedShipListContext *)context;
    (void)hdc;
    ++packed->draw_calls;
    packed->last_title = title;
    rect->left = 0;
    rect->top = 0;
    rect->right = 120;
    rect->bottom = 80;
    return 1;
}

static void show_control(void *context, StarsI16 control, StarsI16 show)
{
    PackedShipListContext *packed = (PackedShipListContext *)context;
    (void)control;
    if (show)
        ++packed->shown;
    else
        ++packed->hidden;
}

static void reset_refs(void *context)
{
    ++((PackedShipListContext *)context)->resets;
}

static void position(void *context, StarsI16 left, StarsI16 top,
    StarsI16 width, StarsI16 height)
{
    (void)left;
    (void)top;
    (void)width;
    (void)height;
    ++((PackedShipListContext *)context)->positions;
}

static StarsI32 selection(void *context, StarsU16 hdc)
{
    (void)hdc;
    return ((PackedShipListContext *)context)->combo_value;
}

static void enable(void *context, StarsI16 button, StarsI16 enabled)
{
    PackedShipListContext *packed = (PackedShipListContext *)context;
    ++packed->enables;
    packed->last_button = button;
    packed->last_enabled = enabled;
}

static StarsI16 lookup(void *context, StarsI16 player, StarsI16 selection_id,
    StarsI16 transfer_index, StarsI16 *kind)
{
    PackedShipListContext *packed = (PackedShipListContext *)context;
    (void)player;
    (void)selection_id;
    if (transfer_index != (StarsI16)get16(packed->xfer, 0))
        return 0;
    *kind = (StarsI16)get16(packed->xfer, 2);
    return 1;
}

static void fill(void *context, const StarsRect *rect)
{
    (void)rect;
    ++((PackedShipListContext *)context)->fills;
}

static void install(StarsPlanetShipListOps *ops,
    PackedShipListContext *packed)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->show_control = show_control;
    ops->reset_references = reset_refs;
    ops->set_combo_position = position;
    ops->combo_selection = selection;
    ops->enable_button = enable;
    ops->lookup_transfer = lookup;
    ops->fill_details = fill;
    ops->context = packed;
}

static void reset_trace(PackedShipListContext *packed)
{
    packed->shown = 0;
    packed->hidden = 0;
    packed->resets = 0;
    packed->positions = 0;
    packed->enables = 0;
    packed->fills = 0;
    packed->draw_calls = 0;
    packed->last_title = 0;
    packed->last_button = 0;
    packed->last_enabled = 0;
}

int main(void)
{
    PackedShipListContext packed;
    StarsPlanetShipListInput input;
    StarsPlanetShipListOps ops;
    int checks;
    int failures;

    memset(&packed, 0, sizeof packed);
    memset(&input, 0, sizeof input);
    put16(packed.selection, 152, 7);
    put16(packed.xfer, 0, 11);
    put16(packed.xfer, 2, 2);
    put16(packed.xfer, 4, 0);
    put16(packed.xfer, 6, 3);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof packed.selection == 226);
    CHECK(sizeof packed.xfer == 128);
    CHECK(get16(packed.selection, 152) == 7U);
    CHECK(get16(packed.xfer, 0) == 11U);
    CHECK(get16(packed.xfer, 2) == 2U);
    CHECK(get16(packed.xfer, 6) == 3U);

    install(&ops, &packed);
    input.hdc = 1;
    input.line_height = 8;
    input.combo_height = 40;
    input.player_id = 3;
    input.packed_selection_record = packed.selection;
    input.packed_selection_size = (StarsU16)sizeof packed.selection;
    input.packed_transfer_record = packed.xfer;
    input.packed_transfer_size = (StarsU16)sizeof packed.xfer;
    input.state.tile_changed = 1;
    input.ops = &ops;
    packed.combo_value = 2;
    reset_trace(&packed);
    stars_draw_planet_ship_list(&input);
    CHECK(input.state.has_object == 1 && input.state.transfer_index == 11);
    CHECK(input.state.transfer_kind == 2 && input.state.fleet_owner == 3);
    CHECK(input.packed_selection_size == 226U &&
        input.packed_transfer_size == 128U);
    CHECK(packed.last_title == 0x339 && packed.draw_calls == 1);
    CHECK(packed.hidden == 4 && packed.resets == 2 &&
        packed.positions == 4 && packed.shown == 4);
    CHECK(packed.fills == 1 && packed.enables == 3 &&
        input.state.transfer_found == 1 && input.state.mode_flag == 0);

    put16(packed.xfer, 2, 8);
    put16(packed.xfer, 4, (5U << 9) | (6U << 13));
    input.state.tile_changed = 1;
    input.state.tile_ship_mode = 1;
    reset_trace(&packed);
    stars_draw_planet_ship_list(&input);
    CHECK(input.state.transfer_kind == 8 &&
        ((get16(packed.xfer, 4) >> 9) & 0xfU) == 5 &&
        ((get16(packed.xfer, 4) >> 13) & 7U) == 6);
    CHECK(packed.fills == 0 && input.state.mode_flag == 1 &&
        packed.last_button == 2 && packed.last_enabled == 0);
    CHECK(packed.hidden == 4 && packed.shown == 4);

    put16(packed.selection, 152, 0);
    input.state.tile_changed = 1;
    input.state.tile_ship_mode = 1;
    packed.combo_value = -1;
    reset_trace(&packed);
    stars_draw_planet_ship_list(&input);
    CHECK(packed.last_title == 0x338 && input.state.has_object == 0);
    CHECK(packed.hidden == 4 && packed.shown == 3 && packed.fills == 0);
    CHECK(checks == 18);
    puts("DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
