#include "core.h"

enum {
    WAYPT_RESET_BIT = 0x0800,
    WAYPT_HIDE_BIT = 0x1000,
    WAYPT_TILE_TITLE = 0x037a,
    WAYPT_DISTANCE_LABEL = 0x0379,
    WAYPT_LABEL_FLEET = 0x037e,
    WAYPT_LABEL_SCAN = 0x04fa,
    WAYPT_MESSAGE_QUERY_SELECTION = 0x0407,
    WAYPT_MESSAGE_CLEAR = 0x0405,
    WAYPT_SHOW = 5,
    WAYPT_HIDE = 0,
    WAYPT_POSITION_SAME = 0x15,
    WAYPT_POSITION_MOVE = 0x14,
    WAYPT_PRIMARY_REPORT = 1,
    WAYPT_PRIMARY_MINE_LAYING = 6,
    WAYPT_PRIMARY_FLEET = 7,
    WAYPT_PRIMARY_COLONIZE = 9,
    WAYPT_PRIMARY_MINEFIELD = 3,
    WAYPT_PRIMARY_STARBASE = 2,
    WAYPT_PRIMARY_TRANSFER = 4,
    WAYPT_PRIMARY_WAYPOINT = 5,
    WAYPT_TEXT_WAYPOINT = 0x25,
    WAYPT_TEXT_TRANSFER = 0x04d2,
    WAYPT_TEXT_MINE_LAYING = 0x00e9,
    WAYPT_TEXT_NO_LAYING = 0x00e6,
    WAYPT_TEXT_NO_MINES = 0x00e2,
    WAYPT_TEXT_NO_DESTINATION = 0x00e5,
    WAYPT_TEXT_CANNOT_SCAN = 0x00f3,
    WAYPT_TEXT_NO_STARBASE = 0x23,
    WAYPT_TEXT_STARBASE_TECH = 0xde,
    WAYPT_TEXT_STARBASE_READY = 0x24,
    WAYPT_KIND_PLANET = 1
};

static void show_all(const StarsWayPtOrdersInput *input, StarsI16 show)
{
    StarsU16 i;

    if (!input->ops.show_window)
        return;
    for (i = 0; i < 3U; ++i)
        input->ops.show_window(input->context, input->dropdown[i], show);
    input->ops.show_window(input->context, input->order_edit, show);
}

static void clear_reference(const StarsWayPtOrdersInput *input,
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

static void fill_gap(const StarsWayPtOrdersInput *input, StarsI16 left,
    StarsI16 top, StarsI16 right, StarsI16 bottom)
{
    StarsRect rect;

    if ((input->tile_flags & WAYPT_HIDE_BIT) != 0U ||
            !input->ops.fill_rect)
        return;
    rect.left = (StarsU16)(left - 1);
    rect.top = (StarsU16)top;
    rect.right = (StarsU16)(right + 1);
    rect.bottom = (StarsU16)(bottom + 2);
    input->ops.fill_rect(input->context, input->hdc, &rect,
        input->button_face_brush);
}

static StarsI16 text_width(const StarsWayPtOrdersInput *input, StarsU16 id,
    StarsU8 *text)
{
    StarsI16 length;

    if (!input->ops.get_text)
        return 0;
    length = input->ops.get_text(input->context, id, text, 128U);
    if (input->ops.text_extent)
        length = (StarsI16)input->ops.text_extent(input->context, input->hdc,
            text, length);
    return length;
}

static void draw_message(const StarsWayPtOrdersInput *input, StarsI16 left,
    StarsI16 top, StarsI16 right, StarsI16 bottom, StarsU16 id,
    StarsU32 color)
{
    StarsU8 text[128];
    StarsRect rect;
    StarsI16 length;

    if (!input->ops.get_text || !input->ops.draw_text)
        return;
    length = input->ops.get_text(input->context, id, text, sizeof text);
    if (input->ops.set_text_color)
        input->ops.set_text_color(input->context, input->hdc, color);
    if (input->ops.select_font)
        input->ops.select_font(input->context, input->hdc, input->small_font);
    rect.left = (StarsU16)left;
    rect.top = (StarsU16)top;
    rect.right = (StarsU16)right;
    rect.bottom = (StarsU16)(bottom + 2);
    input->ops.draw_text(input->context, input->hdc, text, length, &rect,
        0x0810);
    if (input->ops.set_text_color)
        input->ops.set_text_color(input->context, input->hdc,
            input->button_text_color);
}

static void position_dropdown(const StarsWayPtOrdersInput *input, StarsU16 n,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height,
    StarsU16 flags)
{
    if (input->ops.set_window_pos)
        input->ops.set_window_pos(input->context, input->dropdown[n], x, y,
            width, height, flags);
    if (input->ops.show_window)
        input->ops.show_window(input->context, input->dropdown[n], WAYPT_SHOW);
}

/* NE 000b:0912-17b5. Render the waypoint-order controls and the detail
 * message for the selected order.  Window handles, localization, and game
 * state are callbacks/typed fields; the recovered dispatch and geometry stay
 * visible as ordinary C. */
void stars_draw_ship_waypoint_orders(StarsWayPtOrdersInput *input)
{
    StarsU8 text[128];
    StarsRect tile;
    StarsRect reference;
    StarsI16 left;
    StarsI16 right;
    StarsI16 top;
    StarsI16 bottom;
    StarsI16 width;
    StarsI16 y;
    StarsI16 primary;
    StarsI16 secondary;
    StarsI16 edit_width;
    StarsI16 label_width;
    StarsI16 message_id;
    StarsI16 good_scan;
    StarsU16 flags;

    if (!input || !input->ops.get_text || !input->ops.draw_tile)
        return;
    if ((input->tile_flags & WAYPT_RESET_BIT) != 0U) {
        show_all(input, WAYPT_HIDE);
        clear_reference(input, 5U);
        clear_reference(input, 18U);
        input->tile_flags = (StarsU16)(input->tile_flags &
            (StarsU16)~WAYPT_RESET_BIT);
    }
    input->ops.get_text(input->context, WAYPT_TILE_TITLE, text, sizeof text);
    tile = input->tile_rect;
    if (!input->ops.draw_tile(input->context, input->hdc, input->tile_flags,
            text, &tile)) {
        show_all(input, WAYPT_HIDE);
        return;
    }
    left = (StarsI16)tile.left + 4;
    right = (StarsI16)tile.right - 4;
    top = (StarsI16)tile.top + (input->grid_bit == 0U ? 4 : 2);
    bottom = (StarsI16)tile.bottom - 4;
    width = (StarsI16)(right - left);
    clear_reference(input, 5U);
    flags = input->dropdown_width[0] == width ? WAYPT_POSITION_SAME :
        WAYPT_POSITION_MOVE;
    input->dropdown_width[0] = width;
    position_dropdown(input, 0U, left, top, width,
        (StarsI16)(input->arial8_height * 10 + input->ship_dropdown_height),
        flags);
    y = (StarsI16)(top + input->ship_dropdown_height + 3);
    primary = input->ops.send_message ? (StarsI16)input->ops.send_message(
        input->context, input->dropdown[0], WAYPT_MESSAGE_QUERY_SELECTION,
        0U, 0L) : 0;

    if (input->ops.is_visible && !input->ops.is_visible(input->context,
            input->dropdown[1]))
        fill_gap(input, left, y, right, bottom);
    if (primary == WAYPT_PRIMARY_REPORT || primary == WAYPT_PRIMARY_MINE_LAYING ||
            primary == WAYPT_PRIMARY_FLEET || primary == WAYPT_PRIMARY_COLONIZE) {
        label_width = 0;
        if (primary == WAYPT_PRIMARY_FLEET)
            label_width = text_width(input, WAYPT_LABEL_FLEET, text) + 2;
        else if (primary == WAYPT_PRIMARY_COLONIZE)
            label_width = text_width(input, WAYPT_LABEL_SCAN, text) + 2;
        if (label_width != 0)
            left = (StarsI16)(left + label_width + 2);
        width = (StarsI16)(right - left);
        flags = input->dropdown_width[1] == width ? WAYPT_POSITION_SAME :
            WAYPT_POSITION_MOVE;
        input->dropdown_width[1] = width;
        position_dropdown(input, 1U, left, y, width,
            (StarsI16)(input->ship_dropdown_height * 6), flags);
        if (primary == WAYPT_PRIMARY_REPORT) {
            reference.left = (StarsU16)(right - (input->ship_dropdown_height | 1U) + 8);
            reference.top = (StarsU16)(y + 3);
            reference.right = (StarsU16)(right + 1);
            reference.bottom = (StarsU16)(y + (input->ship_dropdown_height | 1U) - 5);
            if (input->ops.set_reference)
                input->ops.set_reference(input->context, 5U, &reference);
            if (input->ops.draw_diamond)
                input->ops.draw_diamond(input->context, input->hdc, &reference,
                    input->diamond_brush);
        } else if (primary == WAYPT_PRIMARY_FLEET && input->ops.draw_gauge) {
            reference.left = (StarsU16)left;
            reference.top = (StarsU16)(y + input->ship_dropdown_height + 4);
            reference.right = (StarsU16)(right - 1);
            reference.bottom = (StarsU16)(reference.top + input->arial8_height);
            if (input->ops.set_reference)
                input->ops.set_reference(input->context, 18U, &reference);
            input->ops.draw_gauge(input->context, input->hdc, &reference,
                input->fleet, 7);
            y = (StarsI16)(reference.bottom + 1);
        }
    } else {
        if (input->ops.is_visible && input->ops.is_visible(input->context,
                input->dropdown[1]) && input->ops.show_window)
            input->ops.show_window(input->context, input->dropdown[1], WAYPT_HIDE);
        fill_gap(input, left, y, right, bottom);
    }
    y = (StarsI16)(y + input->ship_dropdown_height + 3);
    if (primary == WAYPT_PRIMARY_REPORT) {
        label_width = text_width(input, WAYPT_DISTANCE_LABEL, text);
        flags = input->dropdown_width[2] == width ? WAYPT_POSITION_SAME :
            WAYPT_POSITION_MOVE;
        input->dropdown_width[2] = (StarsI16)(width - label_width - 8);
        position_dropdown(input, 2U, left, y, input->dropdown_width[2],
            (StarsI16)(input->ship_dropdown_height * 9), flags);
        secondary = input->ops.send_message ? (StarsI16)input->ops.send_message(
            input->context, input->dropdown[2], WAYPT_MESSAGE_QUERY_SELECTION,
            0U, 0L) : 0;
        edit_width = (StarsI16)(input->order_edit_width - label_width);
        if (edit_width < 0)
            edit_width = 0;
        if (input->ops.set_window_pos)
            input->ops.set_window_pos(input->context, input->order_edit,
                (StarsI16)(right - input->order_edit_width - 4), y, edit_width,
                input->ship_dropdown_height, WAYPT_POSITION_MOVE);
        if (input->ops.enable_window)
            input->ops.enable_window(input->context, input->order_edit,
                secondary != 0 && secondary != 7);
        if (input->ops.show_window)
            input->ops.show_window(input->context, input->order_edit,
                WAYPT_SHOW);
        reference.left = (StarsU16)(right - label_width - 1);
        reference.top = (StarsU16)(y + 2);
        reference.right = (StarsU16)(right - 1);
        reference.bottom = (StarsU16)bottom;
        if (input->ops.set_reference)
            input->ops.set_reference(input->context, 18U, &reference);
        if (secondary != 0 && secondary != 7 && input->ops.draw_text)
            input->ops.draw_text(input->context, input->hdc, text, 2,
                &reference, 0x0800);
    } else {
        if (input->ops.show_window) {
            input->ops.show_window(input->context, input->dropdown[2], WAYPT_HIDE);
            input->ops.show_window(input->context, input->order_edit, WAYPT_HIDE);
        }
    }
    if (primary == WAYPT_PRIMARY_WAYPOINT)
        message_id = WAYPT_TEXT_WAYPOINT;
    else if (primary == WAYPT_PRIMARY_TRANSFER) {
        if (input->active_order_kind == 2)
            return;
        message_id = WAYPT_TEXT_TRANSFER;
    } else if (primary == WAYPT_PRIMARY_MINE_LAYING) {
        if (input->layer_mines > 0 && input->ops.format_number) {
            StarsI16 length;
            length = input->ops.format_number(input->context, text, sizeof text,
                WAYPT_TEXT_MINE_LAYING, input->layer_mines);
            if (input->ops.draw_text) {
                reference.left = (StarsU16)left;
                reference.top = (StarsU16)y;
                reference.right = (StarsU16)right;
                reference.bottom = (StarsU16)(bottom + 2);
                input->ops.draw_text(input->context, input->hdc, text, length,
                    &reference, 0x0810);
            }
            return;
        }
        message_id = WAYPT_TEXT_NO_LAYING;
    } else if (primary == WAYPT_PRIMARY_MINEFIELD) {
        if (input->mines < 1)
            message_id = WAYPT_TEXT_NO_MINES;
        else if (!input->destination_planet ||
                (input->destination_player != -1 &&
                 (input->destination_player != input->selected_player ||
                  !input->race_can_scan)))
            message_id = WAYPT_TEXT_NO_DESTINATION;
        else if (input->mineral_density > 1 && input->ops.draw_minerals) {
            input->ops.draw_minerals(input->context, input->hdc, left, y,
                (StarsI16)input->mines, input->mineral_density);
            return;
        } else
            message_id = WAYPT_TEXT_CANNOT_SCAN;
    } else if (primary == WAYPT_PRIMARY_STARBASE) {
        if (!input->has_starbase_design)
            message_id = WAYPT_TEXT_NO_STARBASE;
        else if (!input->starbase_tech_complete)
            message_id = WAYPT_TEXT_STARBASE_TECH;
        else
            message_id = WAYPT_TEXT_STARBASE_READY;
    } else {
        return;
    }
    good_scan = message_id == WAYPT_TEXT_STARBASE_READY;
    draw_message(input, left, y, right, bottom,
        (StarsU16)message_id, good_scan ? input->button_text_color :
        input->disabled_text_color);
}
