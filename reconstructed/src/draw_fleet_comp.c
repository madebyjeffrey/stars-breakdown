#include "core.h"

enum {
    FLEET_COMP_TILE_RESET_BIT = 0x0800,
    FLEET_COMP_TILE_HIDE_BIT = 0x1000,
    FLEET_COMP_BUTTON_COUNT = 3,
    FLEET_COMP_BUTTON_FIRST = 8,
    FLEET_COMP_LEFT_INSET = 4,
    FLEET_COMP_TOP_INSET = 3,
    FLEET_COMP_LABEL_BATTLE = 0x04f9,
    FLEET_COMP_LABEL_FUEL = 0x036f,
    FLEET_COMP_LABEL_FUEL_VALUE = 0x0373,
    FLEET_COMP_LABEL_FUEL_UNAVAILABLE = 0x036e,
    FLEET_COMP_LABEL_CLOAK = 0x036d,
    FLEET_COMP_LABEL_NO_CLOAK = 0x0263,
    FLEET_COMP_BUTTON_GAP = 6,
    FLEET_COMP_BUTTON_POSITION_FLAGS = 0x14,
    FLEET_COMP_POSITION_NO_REDRAW = 0x15
};

#define FLEET_COMP_FUEL_LIMIT 1000000000L

static void hide_controls(const StarsDrawFleetCompInput *input)
{
    StarsI16 index;

    if (!input->ops.show_window)
        return;
    input->ops.show_window(input->context, input->fleet_component_list, 0);
    for (index = 0; index < FLEET_COMP_BUTTON_COUNT; ++index)
        input->ops.show_window(input->context,
            input->buttons[index], 0);
    input->ops.show_window(input->context, input->battle_dropdown, 0);
}

static StarsI16 get_comp_text(const StarsDrawFleetCompInput *input,
    StarsU16 id, StarsU8 *text)
{
    if (!input->ops.get_text)
        return 0;
    return input->ops.get_text(input->context, id, text, 128U);
}

static StarsI16 text_width(const StarsDrawFleetCompInput *input,
    StarsU8 *text, StarsI16 length)
{
    if (!input->ops.text_extent)
        return 0;
    return (StarsI16)input->ops.text_extent(input->context, input->hdc,
        text, length);
}

static void draw_text(const StarsDrawFleetCompInput *input, StarsI16 x,
    StarsI16 y, StarsU8 *text, StarsI16 length)
{
    if ((input->tile_flags & FLEET_COMP_TILE_HIDE_BIT) == 0U &&
            input->ops.text_out)
        input->ops.text_out(input->context, input->hdc, x, y, text, length);
}

/* NE 0B:1E72-245B. Lay out the fleet-composition tile, the fuel/cloak
 * summaries, and the split/merge controls.  The callbacks expose the
 * original window and game services while the geometry and state rules stay
 * ordinary, readable C. */
void stars_draw_fleet_comp(StarsDrawFleetCompInput *input)
{
    StarsU8 text[128];
    StarsRect tile;
    StarsI16 left;
    StarsI16 right;
    StarsI16 y;
    StarsI16 list_height_before;
    StarsI16 list_height_after;
    StarsI16 row_count;
    StarsI16 desired_list_height;
    StarsI16 height_delta;
    StarsI16 position_flags;
    StarsI16 text_length;
    StarsI16 label_width;
    StarsI16 fuel_width;
    StarsI16 button_width;
    StarsI16 button_x;
    StarsI16 button_y;
    StarsI16 index;
    StarsI16 ideal_warp;
    StarsI16 cloak_percent;
    StarsI32 fuel_use;
    StarsU32 ship_total;
    StarsU32 value;

    if (!input || !input->ops.draw_tile)
        return;
    if (input->tile_flags & FLEET_COMP_TILE_RESET_BIT) {
        hide_controls(input);
        input->tile_flags = (StarsU16)(input->tile_flags &
            (StarsU16)~FLEET_COMP_TILE_RESET_BIT);
    }
    text_length = get_comp_text(input, 0x0372U, text);
    tile = input->tile_rect;
    if (!input->ops.draw_tile(input->context, input->hdc, input->tile_flags,
            text, &tile)) {
        hide_controls(input);
        return;
    }

    left = (StarsI16)tile.left + FLEET_COMP_LEFT_INSET;
    right = (StarsI16)tile.right - FLEET_COMP_LEFT_INSET;
    y = (StarsI16)tile.top + FLEET_COMP_TOP_INSET;
    list_height_before = input->ops.get_client_height ?
        input->ops.get_client_height(input->context,
            input->fleet_component_list) : input->list_client_height;
    row_count = input->grid_bit == 0U ? 5 : 3;
    desired_list_height = (StarsI16)((input->arial8_height + 2) * row_count);
    height_delta = (StarsI16)(desired_list_height - list_height_before);
    if (input->fleet_component_width == right - left &&
            height_delta >= 0 && height_delta < input->arial8_height)
        position_flags = FLEET_COMP_POSITION_NO_REDRAW;
    else {
        position_flags = FLEET_COMP_BUTTON_POSITION_FLAGS;
        input->fleet_component_width = (StarsI16)(right - left);
    }
    if (input->ops.set_window_pos)
        input->ops.set_window_pos(input->context, input->fleet_component_list,
            left, y, (StarsI16)(right - left), desired_list_height,
            (StarsU16)position_flags);
    if (input->ops.show_window)
        input->ops.show_window(input->context, input->fleet_component_list, 5);
    list_height_after = input->ops.get_client_height ?
        input->ops.get_client_height(input->context,
            input->fleet_component_list) : desired_list_height;
    y = (StarsI16)(y + (input->grid_bit == 0U ?
        4 : 2) + list_height_after);

    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->font_bold);
    text_length = get_comp_text(input, FLEET_COMP_LABEL_BATTLE, text);
    fuel_width = text_width(input, text, text_length);
    draw_text(input, left, (StarsI16)(y + 4), text, text_length);
    if (input->battle_dropdown_width == right - left - fuel_width)
        position_flags = FLEET_COMP_POSITION_NO_REDRAW;
    else {
        position_flags = FLEET_COMP_BUTTON_POSITION_FLAGS;
        input->battle_dropdown_width = (StarsI16)(right - left -
            fuel_width);
    }
    if (input->ops.set_window_pos)
        input->ops.set_window_pos(input->context, input->battle_dropdown,
            (StarsI16)(left + fuel_width), y,
            input->battle_dropdown_width,
            (StarsI16)(input->ship_dropdown_height * 5),
            (StarsU16)position_flags);
    if (input->ops.show_window)
        input->ops.show_window(input->context, input->battle_dropdown, 5);

    y = (StarsI16)(y + input->ship_dropdown_height + 3);
    text_length = get_comp_text(input, FLEET_COMP_LABEL_FUEL, text);
    text_width(input, text, text_length);
    draw_text(input, left, y, text, text_length);
    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->font_normal);
    text_length = get_comp_text(input, FLEET_COMP_LABEL_FUEL_VALUE, text);
    label_width = (StarsI16)(text_width(input, text, text_length) + 6);
    ideal_warp = input->ops.find_ideal_warp ?
        input->ops.find_ideal_warp(input->context, input->fleet, 0) : 0;
    fuel_use = input->ops.estimate_fuel ?
        input->ops.estimate_fuel(input->context, input->fleet, ideal_warp) :
        FLEET_COMP_FUEL_LIMIT;
    if (fuel_use < FLEET_COMP_FUEL_LIMIT && input->ops.format_fuel)
        text_length = input->ops.format_fuel(input->context, text,
            (StarsU16)sizeof text, fuel_use);
    else
        text_length = get_comp_text(input, FLEET_COMP_LABEL_FUEL_UNAVAILABLE, text);
    if (input->ops.right_text)
        input->ops.right_text(input->context, input->hdc, right, y, text,
            text_length, label_width);

    if (input->grid_bit == 0U) {
        y = (StarsI16)(y + input->arial8_height);
        if ((input->tile_flags & FLEET_COMP_TILE_HIDE_BIT) == 0U) {
            if (input->ops.select_font)
                input->ops.select_font(input->context, input->hdc,
                    input->font_bold);
            text_length = get_comp_text(input, FLEET_COMP_LABEL_CLOAK, text);
            if (input->ops.text_out)
                input->ops.text_out(input->context, input->hdc, left, y,
                    text, text_length);
        }
        if (input->ops.select_font)
            input->ops.select_font(input->context, input->hdc,
                input->font_normal);
        cloak_percent = input->ops.cloak_percent ?
            input->ops.cloak_percent(input->context, input->fleet) : 0;
        if (cloak_percent == 0)
            text_length = get_comp_text(input, FLEET_COMP_LABEL_NO_CLOAK, text);
        else if (input->ops.format_percent)
            text_length = input->ops.format_percent(input->context, text,
                (StarsU16)sizeof text, cloak_percent);
        else
            text_length = 0;
        if (input->ops.right_text)
            input->ops.right_text(input->context, input->hdc, right, y, text,
                text_length, label_width);
    }

    y = (StarsI16)(y + input->arial8_height + 2 -
        (input->grid_bit != 0U));
    button_width = (StarsI16)((right - left - 10) /
        FLEET_COMP_BUTTON_COUNT);
    button_x = left;
    button_y = y;
    for (index = 0; index < FLEET_COMP_BUTTON_COUNT; ++index) {
        if (input->ops.set_window_pos)
            input->ops.set_window_pos(input->context, input->buttons[index],
                button_x, button_y, button_width,
                (StarsI16)(input->arial8_height + input->arial8_height / 2),
                FLEET_COMP_BUTTON_POSITION_FLAGS);
        if (input->ops.show_window)
            input->ops.show_window(input->context, input->buttons[index], 5);
        button_x = (StarsI16)(button_x + button_width +
            FLEET_COMP_BUTTON_GAP);
    }

    ship_total = 0UL;
    for (index = 0; index < 16; ++index) {
        value = (StarsU32)input->ship_counts[index];
        ship_total += value;
    }
    if (input->ops.enable_window) {
        input->ops.enable_window(input->context, input->buttons[0],
            input->ops.can_split ?
            input->ops.can_split(input->context, (StarsI32)ship_total) : 0);
        input->ops.enable_window(input->context, input->buttons[1],
            input->ops.can_split_all ?
            input->ops.can_split_all(input->context,
                (StarsI32)ship_total) : 0);
        input->ops.enable_window(input->context, input->buttons[2],
            input->ops.can_merge ?
            input->ops.can_merge(input->context, input->fleet) : 0);
    }
}
