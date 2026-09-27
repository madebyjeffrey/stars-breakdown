#include <string.h>
#include "core.h"

enum {
    PRODUCTION_DLG_WM_PAINT = 15,
    PRODUCTION_DLG_WM_ERASE_BACKGROUND = 20,
    PRODUCTION_DLG_WM_SETCURSOR = 32,
    PRODUCTION_DLG_WM_DRAWITEM = 43,
    PRODUCTION_DLG_WM_MEASUREITEM = 44,
    PRODUCTION_DLG_WM_INIT_DIALOG = 272,
    PRODUCTION_DLG_WM_COMMAND = 273,
    PRODUCTION_DLG_WM_LBUTTONDOWN = 513,
    PRODUCTION_DLG_WM_RBUTTONDOWN = 516,
    PRODUCTION_DLG_WM_CONTROL_COLOR = 25,
    PRODUCTION_DLG_RESEARCH = 0x8b,
    PRODUCTION_DLG_DIAMOND = 0x0d3,
    PRODUCTION_DLG_LIST = 0x416,
    PRODUCTION_DLG_PLANET_LIST = 0x417,
    PRODUCTION_DLG_NEXT = 0x42e,
    PRODUCTION_DLG_PREVIOUS = 0x42f,
    PRODUCTION_DLG_COMPACT_WIDTH = 0x262,
    PRODUCTION_DLG_FULL_WIDTH = 0x2f8
};

static StarsI16 production_point_in_diamond(const StarsProductionDlgInput *input,
    StarsPoint point)
{
    return input->ops->point_in_rect ? input->ops->point_in_rect(
        input->ops->context, &input->state->diamond, point) : 0;
}

static StarsI16 production_in_diamond(const StarsProductionDlgInput *input,
    StarsU32 lparam)
{
    StarsPoint point;

    point.x = (StarsI16)(lparam & 0xffffU);
    point.y = (StarsI16)((lparam >> 16) & 0xffffU);
    return production_point_in_diamond(input, point);
}

static StarsI16 production_cursor_in_diamond(
    const StarsProductionDlgInput *input, StarsU16 hwnd)
{
    StarsPoint point;

    point.x = point.y = 0;
    if (input->ops->get_cursor_pos)
        input->ops->get_cursor_pos(input->ops->context, &point);
    if (input->ops->screen_to_client)
        input->ops->screen_to_client(input->ops->context, hwnd,
            (StarsRect *)&point);
    return production_point_in_diamond(input, point);
}

static void production_paint(StarsProductionDlgInput *input, StarsU16 hwnd)
{
    StarsRect paint;
    StarsRect client;
    StarsU16 dc;

    paint.left = paint.top = paint.right = paint.bottom = 0;
    client = paint;
    dc = input->ops->begin_paint ? input->ops->begin_paint(
        input->ops->context, hwnd, &paint) : 0;
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->ops->context, hwnd, &client);
    if (input->ops->draw_dialog)
        input->ops->draw_dialog(input->ops->context, hwnd, dc, &client, -1);
    if (input->ops->end_paint)
        input->ops->end_paint(input->ops->context, hwnd, &paint);
}

void stars_production_dlg_decode_zip_queues(StarsZipProductionQueue *queues,
    StarsU16 queue_capacity, const StarsU8 *raw, StarsU16 raw_count)
{
    StarsU16 index;
    StarsU16 word;
    StarsU16 count;
    const StarsU8 *record;

    if (!queues || !raw)
        return;
    count = queue_capacity;
    if (count > 5U)
        count = 5U;
    if (raw_count < count)
        count = raw_count;
    for (index = 0; index < count; ++index) {
        record = raw + (unsigned long)index * 40UL;
        memcpy(queues[index].name, record, 13U);
        queues[index].valid = record[13];
        queues[index].no_research = record[14];
        queues[index].item_count = record[15];
        for (word = 0; word < 12U; ++word)
            queues[index].items[word] = (StarsU16)(record[16U + word * 2U] |
                ((StarsU16)record[17U + word * 2U] << 8));
    }
}

static void production_initialize(StarsProductionDlgInput *input,
    StarsU16 hwnd)
{
    static const StarsU16 controls[9] = {
        0x42e, 0x42f, 1, 2, 0x418, 0x419, 0x439, 0x43a, 0x42d
    };
    StarsRect client;
    StarsU16 item;
    StarsU16 index;
    StarsI16 column_width;
    StarsI16 row_height;
    StarsI16 list_top;
    StarsI16 client_center;
    StarsI16 list_left;

    input->state->hwnd = hwnd;
    if (input->ops->get_dialog_item && input->state->selected_player_planets < 2) {
        item = input->ops->get_dialog_item(input->ops->context, hwnd,
            PRODUCTION_DLG_NEXT);
        if (input->ops->enable_window)
            input->ops->enable_window(input->ops->context, item, 0);
        item = input->ops->get_dialog_item(input->ops->context, hwnd,
            PRODUCTION_DLG_PREVIOUS);
        if (input->ops->enable_window)
            input->ops->enable_window(input->ops->context, item, 0);
    }
    if (input->ops->set_window_pos)
        input->ops->set_window_pos(input->ops->context, hwnd, 0, 0, 0,
            input->state->compact_mode ? PRODUCTION_DLG_COMPACT_WIDTH :
            PRODUCTION_DLG_FULL_WIDTH,
            input->state->compact_mode ? input->state->arial8_height * 24 + 24 :
            0x244, 6);
    client.left = client.top = client.right = client.bottom = 0;
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->ops->context, hwnd, &client);
    client_center = (StarsI16)(client.right / 2);
    list_left = (StarsI16)((client.right * 11) / 20 + 16);
    column_width = (StarsI16)((client.right - list_left) / 4);
    row_height = (StarsI16)((input->state->arial8_height * 3) / 2);
    if (input->ops->get_dialog_item && input->ops->set_window_pos) {
        item = input->ops->get_dialog_item(input->ops->context, hwnd,
            PRODUCTION_DLG_RESEARCH);
        input->ops->set_window_pos(input->ops->context, item, 0, 6,
            (StarsI16)(client.bottom - input->state->arial8_height * 3 / 2 - 6),
            (StarsI16)(list_left - 12), row_height, 4);
        for (index = 0; index < 4; ++index) {
            item = input->ops->get_dialog_item(input->ops->context, hwnd,
                controls[index]);
            input->ops->set_window_pos(input->ops->context, item, 0,
                (StarsI16)(list_left + column_width * index),
                (StarsI16)(client.bottom - row_height - 6),
                (StarsI16)(column_width - 6), row_height, 4);
        }
    }
    list_top = 8;
    for (index = 4; index < 9; ++index) {
        item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
            input->ops->context, hwnd, controls[index]) : controls[index];
        if (input->ops->set_window_pos)
            input->ops->set_window_pos(input->ops->context, item, 0,
                (StarsI16)(client_center - (column_width + 0x12) / 2),
                list_top, (StarsI16)(column_width + 0x12 - 6), row_height, 4);
        list_top = (StarsI16)(list_top + row_height + input->state->arial8_height);
    }
    if (input->ops->initialize_dialog)
        input->ops->initialize_dialog(input->ops->context, hwnd, 0, 0U);
    if (input->ops->sticky_position)
        input->ops->sticky_position(input->ops->context, hwnd, 1);
}

static void production_zip_popup(StarsProductionDlgInput *input,
    StarsU16 hwnd, StarsI16 x, StarsI16 y)
{
    const StarsU8 *names[4];
    StarsI16 queue_indices[4];
    StarsZipProductionQueue backup[4];
    StarsI16 selection;
    StarsI16 valid_count;
    StarsI16 queue_index;
    StarsU16 queue_capacity;
    StarsU16 index;
    const StarsU8 *editor_text;

    valid_count = 0;
    queue_capacity = input->state->zip_queue_capacity;
    if (queue_capacity > 4)
        queue_capacity = 4;
    if (input->state->zip_queues && queue_capacity > 0) {
        for (index = 0; index < (StarsU16)queue_capacity; ++index) {
            if (input->state->zip_queues[index].valid != 0U) {
                names[valid_count] = input->state->zip_queues[index].name;
                queue_indices[valid_count] = (StarsI16)index;
                ++valid_count;
            }
        }
    } else {
        valid_count = input->state->zip_valid_count;
        if (valid_count > 4)
            valid_count = 4;
        for (index = 0; index < (StarsU16)valid_count; ++index) {
            names[index] = (const StarsU8 *)0;
            queue_indices[index] = (StarsI16)index;
        }
    }
    editor_text = input->ops->get_compressed_string ?
        input->ops->get_compressed_string(input->ops->context, 0x04c0U) :
        (const StarsU8 *)0;
    if (input->ops->popup_menu_items)
        selection = input->ops->popup_menu_items(input->ops->context, hwnd,
            x, y, names, (StarsU16)valid_count, editor_text);
    else if (input->ops->popup_menu)
        selection = input->ops->popup_menu(input->ops->context, hwnd, x, y);
    else
        return;
    if (selection == (StarsI16)(valid_count + 1)) {
        if (input->ops->edit_zip_dialog && input->state->zip_queues &&
            queue_capacity > 0) {
            for (index = 0; index < (StarsU16)queue_capacity; ++index)
                backup[index] = input->state->zip_queues[index];
            if (!input->ops->edit_zip_dialog(input->ops->context, hwnd,
                    input->state->zip_queues, (StarsU16)queue_capacity)) {
                for (index = 0; index < (StarsU16)queue_capacity; ++index)
                    input->state->zip_queues[index] = backup[index];
            }
        } else if (input->ops->open_zip_dialog)
            input->ops->open_zip_dialog(input->ops->context, hwnd);
    } else if (selection >= 0 && selection < valid_count) {
        queue_index = queue_indices[selection];
        if (input->ops->select_zip)
            input->ops->select_zip(input->ops->context, hwnd, queue_index);
        else if (input->ops->command_handler)
            input->ops->command_handler(input->ops->context, hwnd, 0x0816,
                (StarsU32)queue_index);
    }
}

StarsI16 stars_production_dlg(StarsProductionDlgInput *input,
    StarsU16 hwnd, StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect rect;
    StarsU16 item;
    StarsU16 notify;

    if (!input || !input->state || !input->ops)
        return 0;
    if (message == PRODUCTION_DLG_WM_PAINT) {
        production_paint(input, hwnd);
        return 1;
    }
    if (message == PRODUCTION_DLG_WM_ERASE_BACKGROUND) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->ops->context, hwnd, &rect);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->ops->context, wparam, &rect,
                input->state->button_face_brush);
        return 1;
    }
    if (message == PRODUCTION_DLG_WM_CONTROL_COLOR) {
        item = (StarsU16)lparam;
        notify = (StarsU16)(lparam >> 16);
        if (input->ops->get_dialog_item &&
            input->ops->get_dialog_item(input->ops->context, hwnd,
                PRODUCTION_DLG_RESEARCH) != item && notify != 6)
            return 0;
        return (StarsI16)input->state->button_face_brush;
    }
    if (message == PRODUCTION_DLG_WM_SETCURSOR) {
        if (production_cursor_in_diamond(input, hwnd) &&
            input->ops->set_cursor) {
            input->ops->set_cursor(input->ops->context,
                input->state->help_cursor);
            return 1;
        }
        return 0;
    }
    if (message == PRODUCTION_DLG_WM_DRAWITEM) {
        if ((StarsI16)(lparam >> 16) == -1) {
            if (input->ops->draw_focus_item)
                input->ops->draw_focus_item(input->ops->context, lparam, -2);
        } else if ((StarsI16)(lparam >> 16) == 1 ||
                   (StarsI16)(lparam >> 16) == 2 ||
                   (StarsI16)(lparam >> 16) == 4) {
            if (input->ops->draw_list_item)
                input->ops->draw_list_item(input->ops->context, lparam, 4);
        }
        return 1;
    }
    if (message == PRODUCTION_DLG_WM_MEASUREITEM) {
        if (input->ops->set_measure_height)
            input->ops->set_measure_height(input->ops->context, lparam,
                (StarsI16)(input->state->arial8_height + 2));
        return 1;
    }
    if (message == PRODUCTION_DLG_WM_INIT_DIALOG) {
        production_initialize(input, hwnd);
        return 1;
    }
    if (message == PRODUCTION_DLG_WM_COMMAND) {
        if (input->ops->command_handler)
            input->ops->command_handler(input->ops->context, hwnd, wparam,
                lparam);
        return 0;
    }
    if (message == PRODUCTION_DLG_WM_LBUTTONDOWN ||
        message == PRODUCTION_DLG_WM_RBUTTONDOWN) {
        if (production_in_diamond(input, lparam)) {
            if (message == PRODUCTION_DLG_WM_LBUTTONDOWN) {
                if (input->ops->copy_popup_string)
                    (void)input->ops->copy_popup_string(input->ops->context,
                        0x00d3U, input->state->popup_text,
                        (StarsU16)sizeof input->state->popup_text);
                if (input->ops->popup)
                    input->ops->popup(input->ops->context, hwnd,
                        (StarsI16)(lparam & 0xffffU),
                        (StarsI16)(lparam >> 16));
            } else
                production_zip_popup(input, hwnd,
                    (StarsI16)(lparam & 0xffffU),
                    (StarsI16)(lparam >> 16));
            return 1;
        }
    }
    return 0;
}
