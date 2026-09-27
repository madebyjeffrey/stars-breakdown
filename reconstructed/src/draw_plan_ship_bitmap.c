#include "core.h"

#define PLAN_SHIP_BITMAP_PLANET 1
#define PLAN_SHIP_BITMAP_FLEET 2
#define PLAN_SHIP_BITMAP_HIDE 0x0800
#define PLAN_SHIP_BITMAP_NO_BUTTONS 0x1000
#define PLAN_SHIP_BITMAP_GRID 0x0008
#define PLAN_SHIP_BITMAP_HIDE_MASK 0xf7ff
#define PLAN_SHIP_BITMAP_BUTTON_FIRST 4
#define PLAN_SHIP_BITMAP_BUTTON_FLEET_LAST 6
#define PLAN_SHIP_BITMAP_BUTTON_PLANET_LAST 5
#define PLAN_SHIP_BITMAP_BITMAP_SIZE 0x40
#define PLAN_SHIP_BITMAP_BUTTON_WIDTH 0x5f
#define PLAN_SHIP_BITMAP_RASTER_COPY 0xcc0020UL
#define PLAN_SHIP_BITMAP_SELECTION_SIZE 226U
#define PLAN_SHIP_BITMAP_SELECTION_KIND_OFFSET 4U
#define PLAN_SHIP_BITMAP_SELECTION_ID_OFFSET 8U

static StarsU16 draw_plan_ship_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static void draw_plan_ship_hide(const StarsDrawPlanShipBitmapInput *input)
{
    StarsI16 index;

    if (!input->ops->show_window)
        return;
    for (index = PLAN_SHIP_BITMAP_BUTTON_FIRST; index <=
            PLAN_SHIP_BITMAP_BUTTON_FLEET_LAST; ++index)
        input->ops->show_window(input->ops->context,
            input->button_handles[index], 0);
}

static StarsI16 draw_plan_ship_name(const StarsDrawPlanShipBitmapInput *input,
    StarsU8 *buffer, StarsU16 capacity)
{
    if (input->object_kind == PLAN_SHIP_BITMAP_PLANET &&
        input->ops->planet_name)
        return input->ops->planet_name(input->ops->context, input->object_id,
            buffer, capacity);
    if (input->object_kind == PLAN_SHIP_BITMAP_FLEET &&
        input->ops->fleet_name)
        return input->ops->fleet_name(input->ops->context, input->object_id,
            buffer, capacity);
    return 0;
}

static void draw_plan_ship_planet(const StarsDrawPlanShipBitmapInput *input,
    StarsI16 x, StarsI16 y, StarsI16 tile_index)
{
    StarsU16 old_brush;

    old_brush = 0;
    if (input->ops->select_object)
        old_brush = input->ops->select_object(input->ops->context,
            input->hdc, input->shadow_brush);
    if (input->ops->pattern_blt) {
        input->ops->pattern_blt(input->ops->context, input->hdc, x, y,
            0x46, 2, 0xf00021UL);
        input->ops->pattern_blt(input->ops->context, input->hdc, x, y + 2,
            2, 0x44, 0xf00021UL);
    }
    if (input->ops->select_object)
        input->ops->select_object(input->ops->context, input->hdc,
            input->hilite_brush);
    if (input->ops->pattern_blt) {
        input->ops->pattern_blt(input->ops->context, input->hdc, x + 2,
            y + 0x44, 0x44, 2, 0xf00021UL);
        input->ops->pattern_blt(input->ops->context, input->hdc, x + 0x44,
            y + 2, 2, 0x42, 0xf00021UL);
        input->ops->pattern_blt(input->ops->context, input->hdc, x + 1,
            y + 0x45, 1, 1, 0xf00021UL);
        input->ops->pattern_blt(input->ops->context, input->hdc, x + 0x45,
            y + 1, 1, 1, 0xf00021UL);
        input->ops->pattern_blt(input->ops->context, input->hdc, x + 2,
            y + 2, 0x42, 1, 0x42UL);
        input->ops->pattern_blt(input->ops->context, input->hdc, x + 2,
            y + 3, 1, 0x41, 0x42UL);
        input->ops->pattern_blt(input->ops->context, input->hdc, x + 3,
            y + 0x43, 0x41, 1, 0x42UL);
        input->ops->pattern_blt(input->ops->context, input->hdc, x + 0x43,
            y + 3, 1, 0x40, 0x42UL);
    }
    if (input->ops->select_object)
        input->ops->select_object(input->ops->context, input->hdc, old_brush);
    if (input->ops->select_palette)
        input->ops->select_palette(input->ops->context, input->hdc,
            input->palette);
    if (input->ops->realize_palette)
        input->ops->realize_palette(input->ops->context, input->hdc, 0);
    if (input->ops->dib_blt)
        input->ops->dib_blt(input->ops->context, input->hdc, x + 3, y + 3,
            0x40, 0x40, input->planet_dib,
            (StarsI16)((tile_index % 7) * 0x40),
            (StarsI16)((tile_index / 7) * 0x40), 0x40, 0x40,
            PLAN_SHIP_BITMAP_RASTER_COPY);
}

/* Genuine source reconstruction of DrawPlanShipBitmap, NE 0A:3336-377D. */
void stars_draw_plan_ship_bitmap(StarsDrawPlanShipBitmapInput *input)
{
    StarsU8 name[96];
    StarsRect tile;
    StarsI16 length;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
    StarsI16 button;
    StarsI16 last_button;
    StarsI16 tile_index;
    StarsI16 step;

    if (!input || !input->ops || !input->ops->draw_tile)
        return;
    if (input->packed_selection_record) {
        if (input->packed_selection_size < PLAN_SHIP_BITMAP_SELECTION_SIZE)
            return;
        input->object_kind = (StarsI16)draw_plan_ship_packed_get16(
            input->packed_selection_record,
            PLAN_SHIP_BITMAP_SELECTION_KIND_OFFSET);
        input->object_id = (StarsI16)draw_plan_ship_packed_get16(
            input->packed_selection_record,
            PLAN_SHIP_BITMAP_SELECTION_ID_OFFSET);
    }
    length = draw_plan_ship_name(input, name, (StarsU16)sizeof name);
    if ((input->tile_flags & PLAN_SHIP_BITMAP_HIDE) != 0) {
        draw_plan_ship_hide(input);
        input->tile_flags = (StarsI16)(input->tile_flags &
            PLAN_SHIP_BITMAP_HIDE_MASK);
    }
    if (!input->ops->draw_tile(input->ops->context, input->hdc,
            input->object_kind, input->object_id, &tile, name)) {
        draw_plan_ship_hide(input);
        return;
    }
    x = (StarsI16)(tile.left + 12);
    width = (StarsI16)(tile.right - x - PLAN_SHIP_BITMAP_BUTTON_WIDTH);
    y = (StarsI16)(tile.top +
        (((input->global_display_flags & PLAN_SHIP_BITMAP_GRID) == 0) ? 6 : 2));
    if (input->object_kind == PLAN_SHIP_BITMAP_FLEET) {
        if (input->ops->draw_fleet_ex)
            (void)input->ops->draw_fleet_ex(input->ops->context, input->hdc,
                input->object_id, x, y, 1, -1, 0, 0, -1, 0);
        else if (input->ops->draw_fleet)
            (void)input->ops->draw_fleet(input->ops->context, input->hdc,
                input->object_id);
    }
    else if (input->object_kind == PLAN_SHIP_BITMAP_PLANET) {
        tile_index = (StarsI16)((input->object_id + 8) % 0x1c);
        draw_plan_ship_planet(input, x, y, tile_index);
    }
    height = (StarsI16)(input->arial8_height * 3 / 2);
    x = (StarsI16)(tile.right - width);
    if ((input->tile_flags & PLAN_SHIP_BITMAP_NO_BUTTONS) == 0) {
        if ((input->global_display_flags & PLAN_SHIP_BITMAP_GRID) == 0)
            y = (StarsI16)(y - 4);
        else {
            y = (StarsI16)(y - 2);
            height = (StarsI16)(height - 2);
        }
        last_button = input->object_kind == PLAN_SHIP_BITMAP_FLEET ?
            PLAN_SHIP_BITMAP_BUTTON_FLEET_LAST :
            PLAN_SHIP_BITMAP_BUTTON_PLANET_LAST;
        for (button = PLAN_SHIP_BITMAP_BUTTON_FIRST; button <= last_button;
                ++button) {
            if (input->ops->set_window_position)
                input->ops->set_window_position(input->ops->context,
                    input->button_handles[button], 0, x, y, width, height,
                    0x14);
            if (input->ops->show_window)
                input->ops->show_window(input->ops->context,
                    input->button_handles[button], 5);
            step = (input->global_display_flags & PLAN_SHIP_BITMAP_GRID) == 0
                ? 3 : 2;
            y = (StarsI16)(y + step + height);
        }
    }
    (void)length;
}
