#include "core.h"
#include <string.h>

enum {
    ZIP_PROD_WM_PAINT = 15,
    ZIP_PROD_WM_ERASE_BACKGROUND = 20,
    ZIP_PROD_WM_CONTROL_COLOR = 25,
    ZIP_PROD_WM_INIT_DIALOG = 272,
    ZIP_PROD_WM_COMMAND = 273,
    ZIP_PROD_FIRST_RADIO = 0x431,
    ZIP_PROD_LAST_RADIO = 0x434,
    ZIP_PROD_LIST = 0x417,
    ZIP_PROD_CREATE = 0x816,
    ZIP_PROD_RENAME = 0x41b,
    ZIP_PROD_DELETE = 0x817,
    ZIP_PROD_HELP = 0x76,
    ZIP_PROD_HELP_ID = 0x452,
    ZIP_PROD_DIRTY_BIT = 0x0020,
    ZIP_PROD_MAX_ITEMS = 12
};

static void zip_prod_copy(char *destination, const StarsU8 *source,
    StarsU16 capacity)
{
    StarsU16 i;

    if (capacity == 0U)
        return;
    for (i = 0; i + 1U < capacity && source && source[i] != 0U; ++i)
        destination[i] = (char)source[i];
    destination[i] = 0;
}

static void zip_prod_escape(const StarsU8 *source, StarsU8 *destination,
    StarsU16 capacity)
{
    StarsU16 i;
    StarsU16 out;

    if (capacity == 0U)
        return;
    out = 0;
    for (i = 0; source && source[i] != 0U && out + 1U < capacity; ++i) {
        destination[out++] = source[i];
        if (source[i] == (StarsU8)'&' && out + 1U < capacity)
            destination[out++] = (StarsU8)'&';
    }
    destination[out] = 0;
}

static void zip_prod_default_name(StarsZipProdDlgInput *input,
    StarsI16 queue_index)
{
    const StarsU8 *format;

    format = input->ops->compressed_string ?
        input->ops->compressed_string(input->ops->context, 0x04beU) :
        (const StarsU8 *)0;
    if (input->ops->format_default && format)
        input->ops->format_default(input->ops->context, input->state->work,
            format, (StarsI16)(queue_index + 1));
    else {
        input->state->work[0] = (StarsU8)'Q';
        input->state->work[1] = (StarsU8)'u';
        input->state->work[2] = (StarsU8)'e';
        input->state->work[3] = (StarsU8)'u';
        input->state->work[4] = (StarsU8)'e';
        input->state->work[5] = (StarsU8)' ';
        input->state->work[6] = (StarsU8)('1' + queue_index);
        input->state->work[7] = 0;
    }
}

static void zip_prod_set_radio_text(StarsZipProdDlgInput *input,
    StarsU16 hwnd, StarsI16 queue_index)
{
    if (input->state->queues[queue_index].valid == 0U)
        zip_prod_default_name(input, queue_index);
    else
        zip_prod_escape(input->state->queues[queue_index].name,
            input->state->work, (StarsU16)sizeof(input->state->work));
    if (input->ops->set_window_text && input->ops->get_dialog_item)
        input->ops->set_window_text(input->ops->context,
            input->ops->get_dialog_item(input->ops->context, hwnd,
                (StarsU16)(ZIP_PROD_FIRST_RADIO + queue_index)),
            input->state->work);
}

static void zip_prod_paint(StarsZipProdDlgInput *input, StarsU16 hwnd)
{
    StarsRect paint;
    StarsRect frame;
    StarsU16 dc;
    StarsU16 item;
    StarsI16 length;
    const StarsU8 *text;

    paint.left = paint.top = paint.right = paint.bottom = 0;
    frame.left = frame.top = frame.right = frame.bottom = 0;
    dc = input->ops->begin_paint ? input->ops->begin_paint(
        input->ops->context, hwnd, &paint) : 0;
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->ops->context, hwnd, &paint);
    item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
        input->ops->context, hwnd, 0x434U) : 0;
    if (input->ops->get_window_rect)
        input->ops->get_window_rect(input->ops->context, item, &frame);
    if (input->ops->map_window_points)
        input->ops->map_window_points(input->ops->context, 0, hwnd, &frame, 2);
    if (input->ops->expand_rect)
        input->ops->expand_rect(input->ops->context, &frame,
            input->state->arial8_height, input->state->arial8_height >> 1);
    if (input->ops->draw_frame)
        input->ops->draw_frame(input->ops->context, dc, &frame, -1);
    if (input->ops->select_object)
        input->ops->select_object(input->ops->context, dc, 0);
    if (input->ops->set_background_color)
        input->ops->set_background_color(input->ops->context, dc,
            input->state->button_face_color);
    text = input->ops->get_string ? input->ops->compressed_string ?
        input->ops->compressed_string(input->ops->context, 0x04c1U) :
        (const StarsU8 *)0 : (const StarsU8 *)0;
    length = text && input->ops->get_string ? input->ops->get_string(
        input->ops->context, 0x04c1U, input->state->work,
        (StarsU16)sizeof(input->state->work)) : 0;
    if (input->ops->text_out && length >= 0)
        input->ops->text_out(input->ops->context, dc,
            (StarsI16)(frame.left + 8),
            (StarsI16)(frame.top - (input->state->arial8_height >> 1)),
            input->state->work, length);
    if (input->state->queues[input->state->selected_queue].valid != 0U) {
        input->state->resource_tech = (StarsI16)
            (input->state->queues[input->state->selected_queue].no_research & 1U);
        length = input->ops->get_string ? input->ops->get_string(
            input->ops->context,
            input->state->tech_label_id[input->state->resource_tech],
            input->state->work, (StarsU16)sizeof(input->state->work)) : 0;
        if (input->ops->text_out)
            input->ops->text_out(input->ops->context, dc,
                (StarsI16)(frame.left + 8), input->state->static_y,
                input->state->work, length);
    }
    if (input->ops->end_paint)
        input->ops->end_paint(input->ops->context, hwnd, &paint);
}

static void zip_prod_fill(StarsZipProdDlgInput *input, StarsU16 hwnd,
    StarsI16 queue_index)
{
    if (input->ops->fill_list)
        input->ops->fill_list(input->ops->context, hwnd,
            &input->state->queues[queue_index]);
}

static void zip_prod_build_queue(StarsZipProdDlgInput *input,
    StarsI16 queue_index)
{
    StarsI16 index;
    StarsI16 result;
    StarsU16 kind;
    StarsU16 code;
    StarsU16 word;

    input->state->queues[queue_index].item_count = 0;
    if (!input->ops->get_production)
        return;
    for (index = 0; index < 256 &&
            input->state->queues[queue_index].item_count < ZIP_PROD_MAX_ITEMS;
            ++index) {
        kind = 0;
        code = 0;
        result = input->ops->get_production(input->ops->context, index,
            &kind, &code);
        if (result == 0)
            break;
        if (result < 0 || code >= 7U)
            continue;
        word = (StarsU16)((code & 0x003fU) |
            ((kind & 0x03ffU) << 6));
        input->state->queues[queue_index].items[
            input->state->queues[queue_index].item_count] = word;
        ++input->state->queues[queue_index].item_count;
    }
}

static void zip_prod_init(StarsZipProdDlgInput *input, StarsU16 hwnd)
{
    StarsRect window_rect;
    StarsRect client_rect;
    StarsRect list_rect;
    StarsU16 list;
    StarsU16 index;
    const StarsU8 *title;

    input->state->dialog_window = hwnd;
    title = input->ops->compressed_string ?
        input->ops->compressed_string(input->ops->context, 0x0232U) :
        (const StarsU8 *)0;
    if (input->ops->set_window_text && title)
        input->ops->set_window_text(input->ops->context, hwnd, title);
    window_rect.left = window_rect.top = window_rect.right =
        window_rect.bottom = 0;
    client_rect = window_rect;
    list_rect = window_rect;
    if (input->ops->get_window_rect)
        input->ops->get_window_rect(input->ops->context, hwnd, &window_rect);
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->ops->context, hwnd, &client_rect);
    list = input->ops->get_dialog_item ? input->ops->get_dialog_item(
        input->ops->context, hwnd, ZIP_PROD_LIST) : 0;
    if (input->ops->get_window_rect)
        input->ops->get_window_rect(input->ops->context, list, &list_rect);
    if (input->ops->map_window_points)
        input->ops->map_window_points(input->ops->context, 0, hwnd,
            &list_rect, 2);
    input->state->static_y = (StarsI16)(list_rect.top + 2);
    if (input->ops->set_window_pos)
        input->ops->set_window_pos(input->ops->context, hwnd, 0, 0,
            (StarsI16)(window_rect.right - window_rect.left),
            (StarsI16)(client_rect.bottom - list_rect.top +
                input->state->static_y), 6);
    if (input->ops->check_radio_button)
        input->ops->check_radio_button(input->ops->context, hwnd,
            ZIP_PROD_FIRST_RADIO, ZIP_PROD_LAST_RADIO, ZIP_PROD_FIRST_RADIO);
    if (input->ops->enable_buttons)
        input->ops->enable_buttons(input->ops->context, hwnd, 0);
    input->state->resource_tech = (StarsI16)
        (input->state->queues[0].no_research & 1U);
    input->state->selected_queue = 0;
    zip_prod_fill(input, hwnd, 0);
    for (index = 0; index < 4; ++index)
        zip_prod_set_radio_text(input, hwnd, (StarsI16)index);
    if (input->ops->sticky_position)
        input->ops->sticky_position(input->ops->context, hwnd,
            &input->state->sticky_position, 1);
    if ((input->state->game_bits & 0x0800) != 0 &&
            input->ops->advance_tutor)
        input->ops->advance_tutor(input->ops->context);
}

static void zip_prod_rename(StarsZipProdDlgInput *input, StarsU16 hwnd,
    StarsI16 queue_index, StarsU16 command)
{
    StarsI16 result;

    if (input->state->queues[queue_index].valid == 0U)
        zip_prod_default_name(input, queue_index);
    else
        zip_prod_copy((char *)input->state->work,
            input->state->queues[queue_index].name,
            (StarsU16)sizeof(input->state->work));
    if (queue_index != 0 && input->ops->rename_dialog) {
        result = input->ops->rename_dialog(input->ops->context, queue_index,
            input->state->work, (StarsU16)sizeof(input->state->work));
        if (result == 0)
            return;
        if (input->state->work[0] == 0U)
            zip_prod_default_name(input, queue_index);
        zip_prod_copy((char *)input->state->queues[queue_index].name,
            input->state->work,
            (StarsU16)sizeof(input->state->queues[queue_index].name));
        zip_prod_escape(input->state->queues[queue_index].name,
            input->state->work, (StarsU16)sizeof(input->state->work));
        if (input->ops->set_window_text && input->ops->get_dialog_item)
            input->ops->set_window_text(input->ops->context,
                input->ops->get_dialog_item(input->ops->context, hwnd,
                    (StarsU16)(ZIP_PROD_FIRST_RADIO + queue_index)),
                input->state->work);
    }
    if (command == ZIP_PROD_CREATE) {
        input->state->queues[queue_index].valid = 1;
        input->state->resource_tech = input->ops->resource_tech ?
            (StarsI16)(input->ops->resource_tech(input->ops->context) & 1U) : 0;
        input->state->queues[queue_index].no_research =
            (StarsU8)input->state->resource_tech;
        zip_prod_build_queue(input, queue_index);
        zip_prod_fill(input, hwnd, queue_index);
    }
}

StarsI16 stars_zip_prod_dlg(StarsZipProdDlgInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect rect;
    StarsU16 item;
    StarsI16 selected;
    StarsI16 notify;

    if (!input || !input->state || !input->ops)
        return 0;
    if (message == ZIP_PROD_WM_PAINT) {
        zip_prod_paint(input, hwnd);
        return 1;
    }
    if (message == ZIP_PROD_WM_ERASE_BACKGROUND) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->ops->context, hwnd, &rect);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->ops->context, wparam, &rect,
                input->state->button_face_brush);
        return 1;
    }
    if (message == ZIP_PROD_WM_CONTROL_COLOR) {
        for (item = ZIP_PROD_FIRST_RADIO; item <= ZIP_PROD_LAST_RADIO;
                ++item) {
            if ((StarsU32)input->ops->get_dialog_item(input->ops->context,
                    hwnd, item) == lparam) {
                if (input->ops->set_background_color)
                    input->ops->set_background_color(input->ops->context,
                        wparam, input->state->button_face_color);
                return (StarsI16)input->state->button_face_brush;
            }
        }
        return 0;
    }
    if (message == ZIP_PROD_WM_INIT_DIALOG) {
        zip_prod_init(input, hwnd);
        return 1;
    }
    if (message != ZIP_PROD_WM_COMMAND)
        return 0;
    notify = (StarsI16)(StarsU16)(lparam >> 16);
    if (notify == 0 && wparam >= ZIP_PROD_FIRST_RADIO &&
            wparam < ZIP_PROD_LAST_RADIO + 1U) {
        selected = (StarsI16)(wparam - ZIP_PROD_FIRST_RADIO);
        input->state->selected_queue = selected;
        input->state->resource_tech = (StarsI16)
            (input->state->queues[selected].no_research & 1U);
        if (input->ops->enable_buttons)
            input->ops->enable_buttons(input->ops->context, hwnd, selected);
        zip_prod_fill(input, hwnd, selected);
        return 0;
    }
    if (wparam == 1U || wparam == 2U) {
        if (input->ops->sticky_position)
            input->ops->sticky_position(input->ops->context, hwnd,
                &input->state->sticky_position, 0);
        if (input->ops->end_dialog)
            input->ops->end_dialog(input->ops->context, hwnd,
                (StarsU16)(wparam == 1U));
        input->state->static_y = -1;
        if ((input->state->game_bits & 0x0800) != 0 &&
                input->ops->advance_tutor)
            input->ops->advance_tutor(input->ops->context);
        return 1;
    }
    if (wparam == ZIP_PROD_DELETE) {
        selected = input->state->selected_queue;
        input->state->queues[selected].valid = 0;
        zip_prod_set_radio_text(input, hwnd, selected);
        zip_prod_fill(input, hwnd, selected);
        input->state->dirty = (StarsI16)(input->state->dirty |
            ZIP_PROD_DIRTY_BIT);
        return 0;
    }
    if (wparam == ZIP_PROD_HELP) {
        if (input->ops->win_help)
            input->ops->win_help(input->ops->context, hwnd, (const StarsU8 *)0,
                input->state->help_id ? (StarsU16)input->state->help_id :
                ZIP_PROD_HELP_ID);
        return 1;
    }
    if (wparam == ZIP_PROD_CREATE || wparam == ZIP_PROD_RENAME) {
        zip_prod_rename(input, hwnd, input->state->selected_queue, wparam);
        if (input->ops->enable_buttons)
            input->ops->enable_buttons(input->ops->context, hwnd,
                input->state->selected_queue);
        if (input->ops->set_focus)
            input->ops->set_focus(input->ops->context, hwnd);
        input->state->dirty = (StarsI16)(input->state->dirty |
            ZIP_PROD_DIRTY_BIT);
        return 0;
    }
    return 0;
}
