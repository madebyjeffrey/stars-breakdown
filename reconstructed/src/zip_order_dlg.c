#include "core.h"
#include <string.h>

enum {
    ZIP_ORDER_WM_PAINT = 15,
    ZIP_ORDER_WM_ERASE_BACKGROUND = 20,
    ZIP_ORDER_WM_CONTROL_COLOR = 25,
    ZIP_ORDER_WM_INIT_DIALOG = 272,
    ZIP_ORDER_WM_COMMAND = 273,
    ZIP_ORDER_FIRST_RADIO = 0x431,
    ZIP_ORDER_LAST_RADIO = 0x434,
    ZIP_ORDER_LIST = 0x417,
    ZIP_ORDER_OK = 1,
    ZIP_ORDER_CANCEL = 2,
    ZIP_ORDER_CREATE = 0x816,
    ZIP_ORDER_RENAME = 0x41b,
    ZIP_ORDER_DELETE = 0x817,
    ZIP_ORDER_HELP = 0x76,
    ZIP_ORDER_HELP_CONTEXT = 0x44a
};

static void zip_order_escape(const StarsU8 *source, StarsU8 *destination,
    StarsU16 capacity)
{
    StarsU16 in;
    StarsU16 out;

    if (capacity == 0U)
        return;
    in = 0;
    out = 0;
    while (source && source[in] != 0U && out + 1U < capacity) {
        destination[out++] = source[in++];
        if (destination[out - 1U] == (StarsU8)'&' && out + 1U < capacity)
            destination[out++] = (StarsU8)'&';
    }
    destination[out] = 0;
}

static void zip_order_default_name(StarsZipOrderDlgState *state,
    const StarsZipOrderDlgOps *ops, StarsI16 slot)
{
    if (ops->format_default)
        ops->format_default(ops->context, state->names[slot],
            (StarsI16)(slot + 1));
    else {
        state->names[slot][0] = (StarsU8)'Z';
        state->names[slot][1] = (StarsU8)'i';
        state->names[slot][2] = (StarsU8)'p';
        state->names[slot][3] = (StarsU8)' ';
        state->names[slot][4] = (StarsU8)('1' + slot);
        state->names[slot][5] = 0;
    }
}

static void zip_order_set_text(StarsZipOrderDlgState *state,
    StarsU16 hwnd, StarsI16 slot, const StarsZipOrderDlgOps *ops)
{
    StarsU16 item;
    StarsU8 escaped[64];

    item = ops->get_dialog_item ? ops->get_dialog_item(ops->context, hwnd,
        (StarsU16)(ZIP_ORDER_FIRST_RADIO + slot)) : 0;
    if (state->valid[slot])
        zip_order_escape(state->names[slot], escaped, (StarsU16)sizeof escaped);
    else {
        zip_order_default_name(state, ops, slot);
        zip_order_escape(state->names[slot], escaped, (StarsU16)sizeof escaped);
    }
    if (ops->set_window_text)
        ops->set_window_text(ops->context, item, escaped);
}

static void zip_order_paint(StarsZipOrderDlgState *state, StarsU16 hwnd,
    const StarsZipOrderDlgOps *ops)
{
    StarsRect client;
    StarsRect frame;
    StarsU16 dc;
    StarsU16 item;
    StarsU16 old_font;
    StarsI16 length;
    StarsI16 y;
    StarsI16 index;
    StarsI32 caption_width;
    StarsI16 word_length;
    StarsU8 text[96];
    const StarsU8 *compressed;

    client.left = client.top = client.right = client.bottom = 0;
    frame.left = frame.top = frame.right = frame.bottom = 0;
    dc = ops->begin_paint ? ops->begin_paint(ops->context, hwnd, &client) : 0;
    if (ops->get_client_rect)
        ops->get_client_rect(ops->context, hwnd, &client);
    item = ops->get_dialog_item ? ops->get_dialog_item(ops->context, hwnd,
        ZIP_ORDER_FIRST_RADIO) : 0;
    if (ops->get_window_rect)
        ops->get_window_rect(ops->context, item, &frame);
    if (ops->screen_to_client)
        ops->screen_to_client(ops->context, hwnd, &frame);
    item = ops->get_dialog_item ? ops->get_dialog_item(ops->context, hwnd,
        ZIP_ORDER_LAST_RADIO) : 0;
    if (ops->get_window_rect)
        ops->get_window_rect(ops->context, item, &client);
    if (ops->screen_to_client)
        ops->screen_to_client(ops->context, hwnd, &client);
    frame.right = client.right;
    frame.bottom = client.bottom;
    if (ops->expand_rect)
        ops->expand_rect(ops->context, &frame, state->arial8_height,
            state->arial8_height >> 1);
    if (ops->draw_frame)
        ops->draw_frame(ops->context, dc, &frame, -1);
    old_font = 0;
    if (ops->select_object)
        old_font = ops->select_object(ops->context, dc, state->font_arial8);
    if (ops->set_background_color)
        ops->set_background_color(ops->context, dc, state->button_face_color);
    length = ops->get_string ? ops->get_string(ops->context, 0x4c1, text,
        (StarsU16)sizeof text) : 0;
    y = (StarsI16)(frame.top - state->arial8_height / 2);
    if (ops->text_out)
        ops->text_out(ops->context, dc, (StarsI16)(frame.left + 8), y,
            text, length);
    y = (StarsI16)(frame.bottom + 8);
    if (!state->valid[state->selected]) {
        length = ops->get_string ? ops->get_string(ops->context, 0x4c8,
            text, (StarsU16)sizeof text) : 0;
        if (ops->text_out)
            ops->text_out(ops->context, dc, 12, y, text, length);
    } else {
        caption_width = 0;
        if (ops->text_extent && state->resource_caption)
            caption_width = ops->text_extent(ops->context, dc,
                state->resource_caption, 9);
        for (index = 0; index < 5; ++index) {
            if (state->resource_labels[index] && ops->right_text) {
                if (ops->set_text_color)
                    ops->set_text_color(ops->context, dc,
                        state->resource_colors[index]);
                ops->right_text(ops->context, dc,
                    (StarsI16)(caption_width + 8), y,
                    state->resource_labels[index], 0);
                if (ops->set_text_color)
                    ops->set_text_color(ops->context, dc, 0U);
            }
            compressed = ops->compressed_string ?
                ops->compressed_string(ops->context,
                    (StarsU16)(0x6d +
                        ((state->words[state->selected][index] >> 12) & 0xf))) :
                (const StarsU8 *)0;
            word_length = 0;
            if (ops->get_string)
                word_length = ops->get_string(ops->context,
                    (StarsU16)(0x6d +
                    ((state->words[state->selected][index] >> 12) & 0xf)),
                    text, (StarsU16)sizeof text);
            if (ops->text_out && (word_length != 0 || !compressed))
                ops->text_out(ops->context, dc, 14, y, text, word_length);
            else if (ops->text_out && compressed)
                ops->text_out(ops->context, dc, 14, y, compressed,
                    (StarsI16)strlen((const char *)compressed));
            y = (StarsI16)(y + state->arial8_height);
        }
    }
    if (ops->select_object)
        (void)ops->select_object(ops->context, dc, old_font);
    if (ops->end_paint)
        ops->end_paint(ops->context, hwnd, &client);
}

static void zip_order_initialize(StarsZipOrderDlgState *state, StarsU16 hwnd,
    const StarsZipOrderDlgOps *ops)
{
    StarsU16 item;
    StarsI16 slot;
    const StarsU8 *title;

    title = ops->compressed_string ? ops->compressed_string(ops->context,
        0x231) : (const StarsU8 *)0;
    if (ops->set_window_text && title)
        ops->set_window_text(ops->context, hwnd, title);
    state->frame_window = hwnd;
    item = ops->get_dialog_item ? ops->get_dialog_item(ops->context, hwnd,
        ZIP_ORDER_LIST) : 0;
    if (ops->show_window)
        ops->show_window(ops->context, item, 0);
    state->selected = 0;
    if (ops->check_radio_button)
        ops->check_radio_button(ops->context, hwnd, ZIP_ORDER_FIRST_RADIO,
            ZIP_ORDER_LAST_RADIO, ZIP_ORDER_FIRST_RADIO);
    if (ops->enable_buttons)
        ops->enable_buttons(ops->context, hwnd, 0);
    for (slot = 0; slot < 4; ++slot)
        zip_order_set_text(state, hwnd, slot, ops);
    if (ops->sticky_position)
        ops->sticky_position(ops->context, hwnd, &state->sticky_position, 1);
    if (state->tutor_active && ops->advance_tutor)
        ops->advance_tutor(ops->context);
}

static StarsI16 zip_order_edit(StarsZipOrderDlgState *state, StarsU16 hwnd,
    StarsU16 command, const StarsZipOrderDlgOps *ops)
{
    StarsU8 work[64];
    StarsI16 accepted;
    StarsU16 item;

    if (!state->valid[state->selected])
        zip_order_default_name(state, ops, state->selected);
    (void)strncpy((char *)work, (const char *)state->names[state->selected],
        sizeof work - 1U);
    work[sizeof work - 1U] = 0;
    accepted = ops->rename_dialog ? ops->rename_dialog(ops->context,
        state->selected, work, (StarsU16)(command == ZIP_ORDER_RENAME)) : 0;
    if (!accepted)
        return 0;
    if (work[0] == 0U)
        zip_order_default_name(state, ops, state->selected);
    else {
        (void)strncpy((char *)state->names[state->selected],
            (const char *)work, sizeof state->names[state->selected] - 1U);
        state->names[state->selected][sizeof state->names[state->selected] - 1U] = 0;
    }
    if (command == ZIP_ORDER_CREATE) {
        state->valid[state->selected] = 1;
        if (ops->copy_order)
            ops->copy_order(ops->context, state->selected);
    }
    item = ops->get_dialog_item ? ops->get_dialog_item(ops->context, hwnd,
        (StarsU16)(ZIP_ORDER_FIRST_RADIO + state->selected)) : 0;
    if (ops->set_window_text)
        ops->set_window_text(ops->context, item, state->names[state->selected]);
    if (ops->invalidate)
        ops->invalidate(ops->context, hwnd);
    if (ops->enable_buttons)
        ops->enable_buttons(ops->context, hwnd, state->selected);
    state->dirty = 1;
    return 1;
}

StarsI16 stars_zip_order_dlg(StarsZipOrderDlgState *state, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam,
    const StarsZipOrderDlgOps *ops)
{
    StarsRect rect;
    StarsU16 id;
    StarsI16 notify;

    if (!state || !ops)
        return 0;
    if (message == ZIP_ORDER_WM_PAINT) {
        zip_order_paint(state, hwnd, ops);
        return 1;
    }
    if (message == ZIP_ORDER_WM_ERASE_BACKGROUND) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (ops->get_client_rect)
            ops->get_client_rect(ops->context, hwnd, &rect);
        if (ops->fill_rect)
            ops->fill_rect(ops->context, wparam, &rect,
                state->button_face_brush);
        return 1;
    }
    if (message == ZIP_ORDER_WM_CONTROL_COLOR) {
        for (id = ZIP_ORDER_FIRST_RADIO; id <= ZIP_ORDER_LAST_RADIO; ++id)
            if ((StarsU16)lparam == (ops->get_dialog_item ?
                    ops->get_dialog_item(ops->context, hwnd, id) : 0))
                break;
        if (id <= ZIP_ORDER_LAST_RADIO) {
            if (ops->set_background_color)
                ops->set_background_color(ops->context, wparam,
                    state->button_face_color);
            return (StarsI16)state->button_face_brush;
        }
        return 0;
    }
    if (message == ZIP_ORDER_WM_INIT_DIALOG) {
        zip_order_initialize(state, hwnd, ops);
        return 1;
    }
    if (message != ZIP_ORDER_WM_COMMAND)
        return 0;
    notify = (StarsI16)(StarsU16)(lparam >> 16);
    if (wparam > 0x430U && wparam < 0x435U) {
        state->selected = (StarsI16)(wparam - ZIP_ORDER_FIRST_RADIO);
        if (ops->enable_buttons)
            ops->enable_buttons(ops->context, hwnd, state->selected);
        if (ops->invalidate)
            ops->invalidate(ops->context, hwnd);
        (void)notify;
        return 0;
    }
    if (wparam == ZIP_ORDER_OK || wparam == ZIP_ORDER_CANCEL) {
        state->frame_window = 0;
        if (ops->sticky_position)
            ops->sticky_position(ops->context, hwnd, &state->sticky_position, 0);
        if (ops->end_dialog)
            ops->end_dialog(ops->context, hwnd,
                (StarsI16)(wparam == ZIP_ORDER_OK));
        if (state->tutor_active && ops->advance_tutor)
            ops->advance_tutor(ops->context);
        return 1;
    }
    if (wparam == ZIP_ORDER_CREATE || wparam == ZIP_ORDER_RENAME)
        return zip_order_edit(state, hwnd, wparam, ops);
    if (wparam == ZIP_ORDER_DELETE) {
        state->valid[state->selected] = 0;
        if (ops->delete_order)
            ops->delete_order(ops->context, state->selected);
        zip_order_set_text(state, hwnd, state->selected, ops);
        if (ops->invalidate)
            ops->invalidate(ops->context, hwnd);
        state->dirty = 1;
        return 0;
    }
    if (wparam == ZIP_ORDER_HELP) {
        if (ops->win_help)
            ops->win_help(ops->context, hwnd, state->help_file,
                ZIP_ORDER_HELP_CONTEXT);
        return 1;
    }
    if (ops->set_focus)
        ops->set_focus(ops->context, hwnd);
    return 0;
}
