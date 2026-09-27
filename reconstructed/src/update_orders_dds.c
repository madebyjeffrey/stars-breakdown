#include <string.h>
#include "core.h"

enum {
    ORDERS_RESET = 0x0407,
    ORDERS_SELECT = 0x040e,
    ORDERS_ADD = 0x0403,
    ORDERS_MODE_WARP = 6,
    ORDERS_MODE_ENGINE = 7,
    ORDERS_MODE_PLAYER = 9,
    ORDERS_MODE_MINERAL = 1,
    ORDERS_MINERAL_COUNT = 5,
    ORDERS_WARP_COUNT = 11,
    ORDERS_PLAYER_NAME_BUFFER = 32,
    ORDERS_MINERAL_MARKER = '*',
    ORDERS_MINERAL_BLANK = ' ',
    ORDERS_WARP_STEP = 50,
    ORDERS_WARP_FIRST = 50,
    ORDERS_WARP_LABEL_ID = 0x04b5,
    ORDERS_WARP_FINAL_LABEL_ID = 0x04b6,
    ORDERS_MINERAL_FORMAT_ID = 0x0385,
    ORDERS_MINERAL_FINAL_LABEL_ID = 0x0386,
    ORDERS_TARGET_FIRST_STRING = 0x006d,
    ORDERS_TARGET_SPECIAL_STRING = 0x0074,
    ORDERS_TARGET_SPECIAL_REPLACEMENT = 0x0077,
    ORDERS_TARGET_LAST_STRING = 0x0076
};

static StarsI32 orders_send_message(const StarsUpdateOrdersInput *input,
    StarsU16 window, StarsU16 message, StarsU16 selection,
    const StarsU8 *text)
{
    if (!input->ops.send_message)
        return 0;
    return input->ops.send_message(input->context, window, message,
        selection, text);
}

static void add_string(const StarsUpdateOrdersInput *input, StarsU16 window,
    StarsU16 string_id, StarsU8 *text)
{
    if (input->ops.get_string) {
        input->ops.get_string(input->context, string_id, text,
            ORDERS_PLAYER_NAME_BUFFER);
        orders_send_message(input, window, ORDERS_ADD, 0, text);
    }
}

static StarsI32 select_mineral_order(const StarsUpdateOrdersInput *input,
    StarsI16 selected_order, StarsI16 row_count, StarsU8 *text)
{
    StarsI16 first_marked = -1;
    StarsI16 index;
    StarsU16 mineral_index;

    for (index = 0; index < row_count; ++index) {
        mineral_index = index == 0 ? ORDERS_MINERAL_COUNT - 1 :
            (StarsU16)(index - 1);
        if (input->mineral_names[mineral_index]) {
            text[0] = (StarsU8)(input->order_values[mineral_index] >> 12 ?
                ORDERS_MINERAL_MARKER : ORDERS_MINERAL_BLANK);
            strcpy((char *)text + 1,
                (const char *)input->mineral_names[mineral_index]);
            orders_send_message(input, input->order_dropdowns[1], ORDERS_ADD, 0,
                text);
            if (text[0] == ORDERS_MINERAL_MARKER && first_marked < 0)
                first_marked = mineral_index;
        }
    }
    if (selected_order < 0 && first_marked >= 0)
        selected_order = first_marked;
    if (selected_order < 0 || selected_order == ORDERS_MINERAL_COUNT - 1)
        selected_order = 0;
    else
        ++selected_order;
    return (StarsI16)orders_send_message(input, input->order_dropdowns[1],
        ORDERS_SELECT, (StarsU16)selected_order, 0);
}

/* NE 0B:93EE-9A36. Rebuild the order-type, target, and mineral dropdowns for
 * the current fleet order, preserving the executable's selection conventions. */
void stars_update_orders_dds(StarsI16 level, StarsUpdateOrdersInput *input)
{
    StarsI32 order_mode;
    StarsI32 second_selection = 0;
    StarsI32 selection_result;
    StarsI16 index;
    StarsU16 string_id;
    StarsU8 text[96];
    StarsU8 player_name[ORDERS_PLAYER_NAME_BUFFER];
    StarsI16 selected_order;
    StarsI32 fleet_stat;
    StarsI16 row_count;

    if (!input)
        return;
    text[0] = '\0';
    if (level == 0)
        order_mode = orders_send_message(input, input->order_dropdowns[0],
            ORDERS_SELECT, input->active_warp_nibble, 0);
    else
        order_mode = orders_send_message(input, input->order_dropdowns[0],
            ORDERS_RESET, 0, 0);
    if (level > 1) {
        second_selection = orders_send_message(input, input->order_dropdowns[1],
            ORDERS_RESET, 0, 0);
        if (order_mode != ORDERS_MODE_MINERAL || level > 3)
            goto update_target_dropdown;
    }
    orders_send_message(input, input->order_dropdowns[1], ORDERS_RESET, 0, 0);
    if (order_mode == ORDERS_MODE_MINERAL) {
        fleet_stat = input->ops.get_fleet_stat ?
            input->ops.get_fleet_stat(input->context, input->fleet, 2) : 0;
        row_count = fleet_stat == 0 ? 1 : ORDERS_MINERAL_COUNT;
        selected_order = -1;
        if (level > 1)
            selected_order = second_selection == 0 ?
                ORDERS_MINERAL_COUNT - 1 : (StarsI16)second_selection - 1;
        selection_result = select_mineral_order(input, selected_order,
            row_count, text);
    } else if (order_mode == ORDERS_MODE_ENGINE) {
        for (index = 0; index < ORDERS_WARP_COUNT; ++index) {
            if (input->ops.format_text)
                input->ops.format_text(input->context, text, sizeof text,
                    ORDERS_WARP_LABEL_ID,
                    (StarsI32)((index + 1) * ORDERS_WARP_STEP), 0, 1);
            orders_send_message(input, input->order_dropdowns[1], ORDERS_ADD, 0,
                text);
        }
        add_string(input, input->order_dropdowns[1],
            ORDERS_WARP_FINAL_LABEL_ID, text);
        selection_result = orders_send_message(input, input->order_dropdowns[1],
            ORDERS_SELECT, input->active_mode7_selection, 0);
    } else if (order_mode == ORDERS_MODE_PLAYER) {
        text[0] = ORDERS_MINERAL_BLANK;
        text[1] = '\0';
        for (index = 0; index < input->player_count; ++index) {
            if (index != input->player_id && input->ops.get_player_name) {
                input->ops.get_player_name(input->context, index,
                    player_name, sizeof player_name);
                strcpy((char *)text + 1, (const char *)player_name);
                orders_send_message(input, input->order_dropdowns[1], ORDERS_ADD,
                    0, text);
            }
        }
        selection_result = orders_send_message(input, input->order_dropdowns[1],
            ORDERS_SELECT, input->active_mode9_selection, 0);
    } else if (order_mode == ORDERS_MODE_WARP) {
        for (index = 0; index < ORDERS_MINERAL_COUNT; ++index) {
            if (input->ops.format_text)
                input->ops.format_text(input->context, text, sizeof text,
                    ORDERS_MINERAL_FORMAT_ID, (StarsI32)(index + 1),
                    index == 0 ? ' ' : 's', 2);
            orders_send_message(input, input->order_dropdowns[1], ORDERS_ADD, 0,
                text);
        }
        add_string(input, input->order_dropdowns[1],
            ORDERS_MINERAL_FINAL_LABEL_ID, text);
        selection_result = orders_send_message(input, input->order_dropdowns[1],
            ORDERS_SELECT, input->active_mode6_selection, 0);
    } else {
        selection_result = 0;
    }

update_target_dropdown:
    if (level < 3) {
        orders_send_message(input, input->order_dropdowns[2], ORDERS_RESET, 0, 0);
        if (order_mode == ORDERS_MODE_MINERAL) {
            for (string_id = ORDERS_TARGET_FIRST_STRING;
                    string_id <= ORDERS_TARGET_LAST_STRING; ++string_id) {
                if (string_id == ORDERS_TARGET_SPECIAL_STRING &&
                        selection_result == 0)
                    add_string(input, input->order_dropdowns[2],
                        ORDERS_TARGET_SPECIAL_REPLACEMENT, text);
                else
                    add_string(input, input->order_dropdowns[2], string_id,
                        text);
            }
            selected_order = selection_result == 0 ?
                ORDERS_MINERAL_COUNT - 1 : (StarsI16)selection_result - 1;
            orders_send_message(input, input->order_dropdowns[2], ORDERS_SELECT,
                (StarsU16)(input->order_values[selected_order] >> 12), 0);
        }
    } else {
        orders_send_message(input, input->order_dropdowns[2], ORDERS_RESET, 0, 0);
    }
    if (level < 4 && order_mode == ORDERS_MODE_MINERAL) {
        selected_order = selection_result == 0 ?
            ORDERS_MINERAL_COUNT - 1 : (StarsI16)selection_result - 1;
        if (input->ops.format_text)
            input->ops.format_text(input->context, text, sizeof text,
                0x0994, input->order_values[selected_order] & 0x0fffL,
                0, 1);
        if (input->ops.set_window_text)
            input->ops.set_window_text(input->context, input->order_edit,
                text);
    }
}
