#include "core.h"

enum {
    SHIP_ORDERS_TILE_RESET_BIT = 0x0800,
    SHIP_ORDERS_TILE_HIDE_BIT = 0x1000,
    SHIP_ORDERS_LABEL_TITLE = 0x0381,
    SHIP_ORDERS_LABEL_CURRENT = 0x0382,
    SHIP_ORDERS_LABEL_NONE = 899,
    SHIP_ORDERS_LABEL_DESTINATION = 0x0380,
    SHIP_ORDERS_LABEL_DISTANCE = 0x037e,
    SHIP_ORDERS_LABEL_WARP = 0x037f,
    SHIP_ORDERS_LABEL_ETA = 0x037b,
    SHIP_ORDERS_LABEL_FUEL = 0x037d,
    SHIP_ORDERS_LABEL_WARP_FULL = 0x0394,
    SHIP_ORDERS_WARP_FORMAT = 0x036a,
    SHIP_ORDERS_FUEL_RED = 0x0000ffUL,
    SHIP_ORDERS_REPORT_HEIGHT_ADJUST = 1,
    SHIP_ORDERS_POSITION_FLAGS = 0x15,
    SHIP_ORDERS_TILE_LEFT_INSET = 4,
    SHIP_ORDERS_NO_ORDER = -1,
    SHIP_ORDERS_NO_COORDINATE = -1
};

static StarsI16 get_orders_text(const StarsDrawShipOrdersInput *input,
    StarsU16 id, StarsU8 *text)
{
    if (!input->ops.get_text)
        return 0;
    return input->ops.get_text(input->context, id, text, 128U);
}

static const StarsShipOrdersWaypoint *order_at(
    const StarsDrawShipOrdersInput *input, StarsI16 index)
{
    if (!input->waypoints || index < 0 ||
            (StarsU16)index >= input->waypoint_count)
        return 0;
    return &input->waypoints[index];
}

static void hide_order_windows(const StarsDrawShipOrdersInput *input)
{
    if (!input->ops.show_window)
        return;
    input->ops.show_window(input->context, input->ship_list, 0);
    input->ops.show_window(input->context, input->report_combo, 0);
}

static void clear_reference(const StarsDrawShipOrdersInput *input,
    StarsU16 reference)
{
    StarsRect invalid;

    if (!input->ops.set_reference)
        return;
    invalid.left = 0;
    invalid.top = (StarsU16)-5;
    invalid.right = 0;
    invalid.bottom = (StarsU16)-6;
    input->ops.set_reference(input->context, reference, &invalid);
}

static void draw_order_text(const StarsDrawShipOrdersInput *input,
    StarsI16 x, StarsI16 y, StarsU8 *text, StarsI16 length)
{
    if ((input->tile_flags & SHIP_ORDERS_TILE_HIDE_BIT) == 0U &&
            input->ops.text_out)
        input->ops.text_out(input->context, input->hdc, x, y, text, length);
}

/* NE 0B:0000-0912. Draw the ship-order tile and expose the active route's
 * destination, distance, warp, ETA, fuel cost, report selector, and
 * navigation reference.  The waypoint/game callbacks are the original
 * services; the comments and geometry make the recovered UI contract clear. */
void stars_draw_ship_orders(StarsDrawShipOrdersInput *input)
{
    StarsU8 text[128];
    StarsRect tile;
    StarsRect gauge_rect;
    StarsRect report_rect;
    const StarsShipOrdersWaypoint *destination;
    const StarsShipOrdersWaypoint *current;
    StarsI16 left;
    StarsI16 right;
    StarsI16 y;
    StarsI16 list_height_before;
    StarsI16 list_height_after;
    StarsI16 desired_list_height;
    StarsI16 height_delta;
    StarsI16 position_flags;
    StarsI16 text_length;
    StarsI16 current_width;
    StarsI16 destination_width;
    StarsI16 label_width;
    StarsI16 distance_waypoint;
    StarsI16 warp;
    StarsI32 fuel_use;

    if (!input || !input->ops.draw_tile)
        return;
    if (input->tile_flags & SHIP_ORDERS_TILE_RESET_BIT) {
        clear_reference(input, 0U);
        clear_reference(input, 3U);
        hide_order_windows(input);
        input->tile_flags = (StarsU16)(input->tile_flags &
            (StarsU16)~SHIP_ORDERS_TILE_RESET_BIT);
    }
    text_length = get_orders_text(input, SHIP_ORDERS_LABEL_TITLE, text);
    tile = input->tile_rect;
    if (!input->ops.draw_tile(input->context, input->hdc, input->tile_flags,
            text, &tile)) {
        hide_order_windows(input);
        return;
    }
    left = (StarsI16)tile.left + SHIP_ORDERS_TILE_LEFT_INSET;
    right = (StarsI16)tile.right - SHIP_ORDERS_TILE_LEFT_INSET;
    y = (StarsI16)tile.top + (input->grid_bit == 0U ? 4 : 2);
    clear_reference(input, 3U);

    list_height_before = input->ops.get_client_height ?
        input->ops.get_client_height(input->context, input->ship_list) : 0;
    desired_list_height = (StarsI16)((input->arial8_height + 2) *
        (input->grid_bit == 0U ? 4 : 3));
    height_delta = (StarsI16)(desired_list_height - list_height_before);
    if (input->ship_list_width == right - left && height_delta >= 0 &&
            height_delta < input->arial8_height)
        position_flags = 0x15;
    else {
        position_flags = 0x14;
        input->ship_list_width = (StarsI16)(right - left);
    }
    if (input->ops.set_window_pos)
        input->ops.set_window_pos(input->context, input->ship_list, left, y,
            (StarsI16)(right - left), desired_list_height,
            (StarsU16)position_flags);
    if (input->ops.show_window)
        input->ops.show_window(input->context, input->ship_list, 5);
    list_height_after = input->ops.get_client_height ?
        input->ops.get_client_height(input->context, input->ship_list) :
        desired_list_height;
    y = (StarsI16)(y + (input->grid_bit == 0U ? 4 : 2) + list_height_after);

    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->bold_font);
    current_width = get_orders_text(input, SHIP_ORDERS_LABEL_CURRENT, text);
    if (input->ops.text_extent)
        current_width = (StarsI16)input->ops.text_extent(input->context,
            input->hdc, text, current_width);
    destination_width = get_orders_text(input, SHIP_ORDERS_LABEL_NONE, text);
    if (input->ops.text_extent)
        destination_width = (StarsI16)input->ops.text_extent(input->context,
            input->hdc, text, destination_width);
    label_width = current_width > destination_width ? current_width :
        destination_width;
    if (input->active_waypoint < 1)
        text_length = get_orders_text(input, SHIP_ORDERS_LABEL_NONE, text);
    else
        text_length = get_orders_text(input, SHIP_ORDERS_LABEL_CURRENT, text);
    draw_order_text(input, left, y, text, text_length);
    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->normal_font);

    if (input->active_waypoint < 1) {
        distance_waypoint = input->cord < 2 ? SHIP_ORDERS_NO_ORDER : 1;
        destination = order_at(input, distance_waypoint);
    } else {
        distance_waypoint = input->active_waypoint;
        destination = order_at(input, distance_waypoint);
    }
    current = order_at(input, 0);
    if (input->ops.get_location)
        text_length = input->ops.get_location(input->context,
            distance_waypoint, destination, text, (StarsU16)sizeof text);
    else {
        text[0] = '\0';
        text_length = 0;
    }
    if (input->ops.right_text)
        input->ops.right_text(input->context, input->hdc, right, y, text,
            text_length, (StarsI16)(right - left - label_width));
    y = (StarsI16)(y + input->arial8_height);

    clear_reference(input, 0U);
    if (input->cord < 2 || input->active_waypoint == 0) {
        if (input->cord < 2) {
            if ((input->tile_flags & SHIP_ORDERS_TILE_HIDE_BIT) == 0U &&
                    input->ops.fill_rect) {
                gauge_rect.left = (StarsU16)(left - 1);
                gauge_rect.top = (StarsU16)y;
                gauge_rect.right = (StarsU16)(right - 1);
                gauge_rect.bottom = (StarsU16)(y + input->arial8_height * 4);
                input->ops.fill_rect(input->context, input->hdc,
                    &gauge_rect, input->button_face_brush);
            }
            y = (StarsI16)(y + (input->arial8_height -
                (input->grid_bit != 0U)) * 4 +
                (input->grid_bit != 0U ? 2 : 0));
            goto report_control;
        }
    }
    if ((input->tile_flags & SHIP_ORDERS_TILE_HIDE_BIT) == 0U) {
        if (input->ops.select_font)
            input->ops.select_font(input->context, input->hdc,
                input->bold_font);
        text_length = get_orders_text(input, SHIP_ORDERS_LABEL_DESTINATION,
            text);
        draw_order_text(input, left, y, text, text_length);
    }
    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->normal_font);
    if (input->ops.get_distance)
        text_length = input->ops.get_distance(input->context, current,
            destination, text, (StarsU16)sizeof text);
    else
        text_length = 0;
    if (input->ops.right_text)
        input->ops.right_text(input->context, input->hdc, right, y, text,
            text_length, (StarsI16)(right - left - label_width));
    y = (StarsI16)(y + input->arial8_height - (input->grid_bit != 0U));
    if ((input->tile_flags & SHIP_ORDERS_TILE_HIDE_BIT) == 0U) {
        if (input->ops.select_font)
            input->ops.select_font(input->context, input->hdc,
                input->bold_font);
        text_length = get_orders_text(input, SHIP_ORDERS_LABEL_WARP, text);
        draw_order_text(input, left, y, text, text_length);
    }
    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->normal_font);
    warp = input->ops.get_warp ? input->ops.get_warp(input->context,
        input->fleet, input->active_waypoint) : 0;
    if (input->active_waypoint == 0 && input->ops.format_number && warp < 11)
        text_length = input->ops.format_number(input->context, text,
            (StarsU16)sizeof text, SHIP_ORDERS_WARP_FORMAT, warp);
    else
        text_length = get_orders_text(input, SHIP_ORDERS_LABEL_WARP_FULL, text);
    if (input->active_waypoint == 0 && input->ops.right_text)
        input->ops.right_text(input->context, input->hdc, right, y, text,
            text_length, (StarsI16)(right - left - label_width));
    else if (input->active_waypoint != 0) {
        gauge_rect.left = (StarsU16)(left + label_width);
        gauge_rect.top = (StarsU16)y;
        gauge_rect.right = (StarsU16)(right - 1);
        gauge_rect.bottom = (StarsU16)(y + input->arial8_height);
        if (input->ops.set_reference)
            input->ops.set_reference(input->context, 0U, &gauge_rect);
        if (input->ops.draw_gauge)
            input->ops.draw_gauge(input->context, input->hdc, &gauge_rect,
                input->fleet);
    }
    y = (StarsI16)(y + input->arial8_height);
    if ((input->tile_flags & SHIP_ORDERS_TILE_HIDE_BIT) == 0U) {
        if (input->ops.select_font)
            input->ops.select_font(input->context, input->hdc,
                input->bold_font);
        text_length = get_orders_text(input, SHIP_ORDERS_LABEL_ETA, text);
        draw_order_text(input, left, y, text, text_length);
    }
    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->normal_font);
    text_length = input->ops.get_eta ? input->ops.get_eta(input->context,
        input->fleet, input->active_waypoint, text, (StarsU16)sizeof text) : 0;
    if (input->ops.right_text)
        input->ops.right_text(input->context, input->hdc, right, y, text,
            text_length, (StarsI16)(right - left - label_width));
    if (input->ops.set_text_color)
        input->ops.set_text_color(input->context, input->hdc, 0UL);
    y = (StarsI16)(y + input->arial8_height - (input->grid_bit != 0U));
    if ((input->tile_flags & SHIP_ORDERS_TILE_HIDE_BIT) == 0U) {
        if (input->ops.select_font)
            input->ops.select_font(input->context, input->hdc,
                input->bold_font);
        text_length = get_orders_text(input, SHIP_ORDERS_LABEL_FUEL, text);
        draw_order_text(input, left, y, text, text_length);
    }
    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->normal_font);
    fuel_use = input->ops.fuel_use ? input->ops.fuel_use(input->context,
        input->fleet, input->active_waypoint) : 0L;
    if (input->ops.format_number)
        text_length = input->ops.format_number(input->context, text,
            (StarsU16)sizeof text, SHIP_ORDERS_LABEL_FUEL, fuel_use);
    else
        text_length = 0;
    if (input->fuel_available < fuel_use && input->ops.set_text_color)
        input->ops.set_text_color(input->context, input->hdc,
            SHIP_ORDERS_FUEL_RED);
    if (input->ops.right_text)
        input->ops.right_text(input->context, input->hdc, right, y, text,
            text_length, (StarsI16)(right - left - label_width - 0x14));
    if (input->fuel_available < fuel_use && input->ops.set_text_color)
        input->ops.set_text_color(input->context, input->hdc, 0UL);
    y = (StarsI16)(y + input->arial8_height);

report_control:
    if (input->ops.send_report)
        input->ops.send_report(input->context, input->report_combo,
            (StarsU16)input->report_selection);
    report_rect.left = (StarsU16)left;
    report_rect.top = (StarsU16)y;
    report_rect.right = (StarsU16)right;
    report_rect.bottom = (StarsU16)(y + (input->arial8_height |
        SHIP_ORDERS_REPORT_HEIGHT_ADJUST));
    if (input->ops.set_window_pos)
        input->ops.set_window_pos(input->context, input->report_combo, left,
            y, 0, 0, SHIP_ORDERS_POSITION_FLAGS);
    if (input->ops.show_window)
        input->ops.show_window(input->context, input->report_combo, 5);
    if (input->ops.set_reference)
        input->ops.set_reference(input->context, 3U, &report_rect);
    if (input->ops.draw_diamond)
        input->ops.draw_diamond(input->context, input->hdc, &report_rect,
            input->diamond_brush);
}
