#include "core.h"

enum {
    PLANET_CLICK_PLANET = 1,
    PLANET_CLICK_FLEET = 2,
    PLANET_CLICK_PLANET_TILES = 6,
    PLANET_CLICK_FLEET_TILES = 7,
    PLANET_CLICK_COLUMN_WIDTH = 0xc6,
    PLANET_CLICK_LEFT_INSET = 4,
    PLANET_CLICK_RIGHT_INSET = 0xc3,
    PLANET_CLICK_BUTTON_INSET = 0x11,
    PLANET_CLICK_TRACK_TYPE = 0x70,
    PLANET_CLICK_TRACK_EXPANDED = 0x01,
    PLANET_CLICK_TILE_EXPANDED = 0x80,
    PLANET_CLICK_TILE_COLUMN_MASK = 7
};

static StarsI16 tile_column(const StarsPlanetClickTile *tile)
{
    return (StarsI16)(tile->layout_flags & PLANET_CLICK_TILE_COLUMN_MASK);
}

static StarsI16 tile_height(const StarsPlanetClickTile *tile,
    StarsI16 arial8_height)
{
    return (tile->layout_flags & PLANET_CLICK_TILE_EXPANDED) != 0U ?
        tile->height : (StarsI16)(arial8_height + 3);
}

static StarsI16 tile_kind_is_ship(StarsI16 kind)
{
    return (StarsI16)(kind == 0x20 || kind == 0x100 || kind == 1 ||
        kind == 0x10 || kind == 4);
}

static StarsI16 tile_kind_is_planet_order(StarsI16 kind)
{
    return (StarsI16)(kind == 1 || kind == 0x100 || kind == 0x40 ||
        kind == 8);
}

static StarsPlanetClickTile *click_tiles(StarsPlanetClickInput *input,
    StarsI16 *count)
{
    if (input->selected_kind == PLANET_CLICK_PLANET) {
        *count = input->planet_tile_count;
        return input->planet_tiles;
    }
    if (input->selected_kind == PLANET_CLICK_FLEET) {
        *count = input->fleet_tile_count;
        return input->fleet_tiles;
    }
    *count = 0;
    return (StarsPlanetClickTile *)0;
}

static void finish_reflow(StarsPlanetClickInput *input, StarsI16 old_column,
    StarsI16 old_index, StarsI16 new_column, StarsI16 new_index)
{
    if (!input->reflow_column || old_column == new_column)
        return;
    input->reflow_column(input->context, new_column, new_index, 1);
    if (new_column < old_column)
        input->reflow_column(input->context, old_column, old_index + 1, 1);
    else
        input->reflow_column(input->context, old_column, old_index, 1);
}

/* Genuine semantic reconstruction of PlanetClick, NE 0A:489E-515B.
 * Tile layout, button tracking, pointer capture, and order dispatch are
 * explicit typed services.  Tile movement itself remains ordinary C record
 * manipulation, preserving the original column and row decisions without
 * replaying its far-pointer implementation. */
void stars_planet_click(StarsI16 x, StarsI16 y, StarsI16 sks,
    StarsI16 right_button, StarsPlanetClickInput *input)
{
    StarsPlanetClickTile *tiles;
    StarsPlanetClickTile saved;
    StarsPlanetClickButton button;
    StarsRect tile_rect;
    StarsRect button_rect;
    StarsPoint point;
    StarsI16 count;
    StarsI16 tile_index;
    StarsI16 old_index;
    StarsI16 target_index;
    StarsI16 column;
    StarsI16 old_column;
    StarsI16 target_column;
    StarsI16 index;
    StarsI16 dc;
    StarsI16 moving;

    if (!input)
        return;
    tiles = click_tiles(input, &count);
    if (!tiles)
        return;
    column = (StarsI16)((StarsU16)x / PLANET_CLICK_COLUMN_WIDTH);
    if ((StarsI16)((StarsU16)x % PLANET_CLICK_COLUMN_WIDTH) <= 3 ||
            (StarsI16)((StarsU16)x % PLANET_CLICK_COLUMN_WIDTH) >= 0xc2)
        return;
    tile_index = 0;
    while (tile_index < count) {
        if (column <= tile_column(&tiles[tile_index])) {
            if (column < tile_column(&tiles[tile_index]))
                return;
            if (tiles[tile_index].top <= y &&
                    y < tiles[tile_index].top +
                    tile_height(&tiles[tile_index], input->arial8_height))
                break;
        }
        ++tile_index;
    }
    if (tile_index == count)
        return;

    point.x = x;
    point.y = y;
    tile_rect.top = (StarsU16)tiles[tile_index].top;
    tile_rect.bottom = (StarsU16)(tiles[tile_index].top +
        input->arial8_height + 4);
    tile_rect.left = (StarsU16)(column * PLANET_CLICK_COLUMN_WIDTH +
        PLANET_CLICK_LEFT_INSET);
    tile_rect.right = (StarsU16)(column * PLANET_CLICK_COLUMN_WIDTH +
        PLANET_CLICK_RIGHT_INSET);
    if (!input->point_in_rect ||
            !input->point_in_rect(input->context, &tile_rect, point) ||
            right_button) {
        if ((input->selected_kind == PLANET_CLICK_FLEET &&
                tile_kind_is_ship(tiles[tile_index].kind)) ||
                (tiles[tile_index].kind == 4)) {
            if (input->click_ship_orders)
                input->click_ship_orders(input->context, point, sks, 0,
                    right_button);
        } else if (input->selected_kind == PLANET_CLICK_PLANET &&
                tile_kind_is_planet_order(tiles[tile_index].kind)) {
            if (input->click_planet_orders)
                input->click_planet_orders(input->context, point, sks, 0,
                    right_button);
        }
        return;
    }

    button_rect = tile_rect;
    button_rect.top = (StarsU16)(tile_rect.top + 1);
    button_rect.left = (StarsU16)(tile_rect.right -
        PLANET_CLICK_BUTTON_INSET);
    if (input->point_in_rect(input->context, &button_rect, point)) {
        button_rect.left = (StarsU16)(button_rect.left - 1);
        if ((tiles[tile_index].layout_flags & PLANET_CLICK_TILE_EXPANDED) ==
                0U)
            button.flags = PLANET_CLICK_TRACK_EXPANDED;
        else
            button.flags = 0;
        if (input->init_button)
            input->init_button(input->context, &button,
                input->planet_window, &button_rect,
                (StarsU16)(PLANET_CLICK_TRACK_TYPE |
                ((tiles[tile_index].layout_flags &
                PLANET_CLICK_TILE_EXPANDED) == 0U ? 1U : 0U)));
        moving = 1;
        while (moving && input->track_button)
            moving = input->track_button(input->context, &button);
        if ((button.flags & 2U) != 0U) {
            tiles[tile_index].layout_flags = (StarsU16)(
                tiles[tile_index].layout_flags & (StarsU16)~0x80U);
            if ((tiles[tile_index].layout_flags & PLANET_CLICK_TILE_EXPANDED)
                    == 0U)
                tiles[tile_index].layout_flags = (StarsU16)(
                    tiles[tile_index].layout_flags | 0x80U);
            if (input->reflow_column)
                input->reflow_column(input->context,
                    tile_column(&tiles[tile_index]), tile_index, 1);
        }
        return;
    }

    if (!input->get_dc || !input->draw_border || !input->capture ||
            !input->get_mouse_move || !input->release_capture ||
            !input->release_dc)
        return;
    if ((tiles[tile_index].layout_flags & PLANET_CLICK_TILE_EXPANDED) != 0U)
        tile_rect.bottom = (StarsU16)(tile_rect.top +
            tiles[tile_index].height + 1);
    dc = (StarsI16)input->get_dc(input->context, input->planet_window);
    input->draw_border(input->context, (StarsU16)dc, &tile_rect);
    input->capture(input->context, input->planet_window);
    point.x = x;
    point.y = y;
    while (input->get_mouse_move(input->context, &point)) {
        if (point.x != x || point.y != y) {
            input->draw_border(input->context, (StarsU16)dc, &tile_rect);
            if (input->offset_rect)
                input->offset_rect(input->context, &tile_rect,
                    (StarsI16)(point.x - x), (StarsI16)(point.y - y));
            x = point.x;
            y = point.y;
            input->draw_border(input->context, (StarsU16)dc, &tile_rect);
        }
    }
    input->draw_border(input->context, (StarsU16)dc, &tile_rect);
    input->release_capture(input->context);
    input->release_dc(input->context, input->planet_window, (StarsU16)dc);

    old_index = tile_index;
    old_column = tile_column(&tiles[old_index]);
    target_column = (StarsI16)((StarsU16)((tile_rect.left +
        (tile_rect.right - tile_rect.left) / 2)) /
        PLANET_CLICK_COLUMN_WIDTH);
    if (target_column >= 2)
        target_column = 1;
    if (target_column < 0)
        target_column = 0;
    target_index = 0;
    while (target_index < count &&
            tile_column(&tiles[target_index]) < target_column)
        ++target_index;
    while (target_index < count &&
            tile_column(&tiles[target_index]) == target_column &&
            (StarsI16)tile_rect.top +
            (StarsI16)((tile_rect.bottom - tile_rect.top) / 2) >=
            tiles[target_index].top + tile_height(&tiles[target_index],
                input->arial8_height) / 2)
        ++target_index;
    if (target_index == old_index || target_index == old_index + 1) {
        if (target_column != old_column) {
            tiles[old_index].layout_flags = (StarsU16)(
                (tiles[old_index].layout_flags & (StarsU16)~7U) |
                ((StarsU16)target_column & 7U));
            finish_reflow(input, old_column, old_index, target_column,
                target_index);
        }
        return;
    }

    saved = tiles[old_index];
    if (target_index < old_index) {
        for (index = old_index; index > target_index; --index)
            tiles[index] = tiles[index - 1];
    } else {
        for (index = old_index; index < target_index - 1; ++index)
            tiles[index] = tiles[index + 1];
        --target_index;
    }
    saved.layout_flags = (StarsU16)((saved.layout_flags & (StarsU16)~7U) |
        ((StarsU16)target_column & 7U));
    tiles[target_index] = saved;
    finish_reflow(input, old_column, old_index, target_column, target_index);
}
