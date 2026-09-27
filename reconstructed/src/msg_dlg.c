#include "core.h"

static StarsU16 msg_dlg_edit_control(const StarsMsgDlgInput *input)
{
    return input->edit_control ? input->edit_control :
        STARS_MSG_DLG_EDIT_CONTROL;
}

/* Genuine semantic reconstruction of MsgDlg, NE 7:8F68-9220.  This is the
 * dialog procedure for the serial-number/message input box: painting draws
 * the explanatory string, erase-background fills the client area, the edit
 * control color is made button-face colored, initialization selects the
 * default entry, and OK validates before ending the dialog. */
StarsI16 stars_msg_dlg(const StarsMsgDlgInput *input)
{
    const StarsMsgDlgOps *ops;
    StarsU16 edit;
    StarsU16 dc;
    StarsRect client;
    StarsRect text_rect;
    StarsPoint position;
    StarsU8 text[256];
    StarsI16 text_length;
    StarsI16 valid;
    StarsU16 error_string;

    if (!input || !input->ops) return 0;
    ops = input->ops;
    edit = msg_dlg_edit_control(input);

    if (input->message == STARS_MSG_DLG_PAINT) {
        dc = ops->begin_paint ? ops->begin_paint(input->context,
            input->hwnd) : 0;
        if (ops->get_client_rect)
            ops->get_client_rect(input->context, input->hwnd, &client);
        if (ops->get_text_rect)
            ops->get_text_rect(input->context, input->hwnd, edit,
                &text_rect);
        if (ops->select_font) ops->select_font(input->context, dc,
            input->font);
        if (ops->set_background_color) ops->set_background_color(
            input->context, dc, input->button_face_color);
        if (ops->set_text_color) ops->set_text_color(input->context, dc, 0UL);
        text_length = ops->get_string ? ops->get_string(input->context,
            0U, text, (StarsU16)sizeof text) : 0;
        if (ops->draw_text) ops->draw_text(input->context, dc, text,
            text_length, &text_rect, STARS_MSG_DLG_DRAW_TEXT_FLAGS);
        if (ops->end_paint) ops->end_paint(input->context, input->hwnd, dc);
        return 1;
    }
    if (input->message == STARS_MSG_DLG_ERASE_BACKGROUND) {
        if (ops->get_client_rect)
            ops->get_client_rect(input->context, input->hwnd, &client);
        if (ops->fill_rect) ops->fill_rect(input->context, input->wparam,
            &client, input->button_brush);
        return 1;
    }
    if (input->message == STARS_MSG_DLG_CONTROL_COLOR) {
        if ((StarsU16)(input->lparam >> 16) ==
                STARS_MSG_DLG_EDIT_CONTROL_TYPE) {
            if (ops->set_background_color) ops->set_background_color(
                input->context, input->wparam, input->button_face_color);
            return (StarsI16)input->button_brush;
        }
        return 0;
    }
    if (input->message == STARS_MSG_DLG_INIT_DIALOG) {
        position.x = -1; position.y = -1;
        text[0] = 0;
        if (ops->send_item_message) ops->send_item_message(input->context,
            input->hwnd, edit, STARS_MSG_DLG_COMBO_SELECT, 8U, 0UL);
        if (ops->set_window_text) ops->set_window_text(input->context,
            ops->get_dialog_item ? ops->get_dialog_item(input->context,
            input->hwnd, edit) : edit, text);
        if (ops->sticky_position) ops->sticky_position(input->context,
            input->hwnd, &position, 1);
        return 1;
    }
    if (input->message != STARS_MSG_DLG_COMMAND)
        return 0;

    if (input->wparam == STARS_MSG_DLG_ACCEPT ||
            input->wparam == STARS_MSG_DLG_CANCEL) {
        if (input->wparam == STARS_MSG_DLG_ACCEPT) {
            text_length = ops->get_item_text ? ops->get_item_text(
                input->context, input->hwnd, edit, text, 9U) : 0;
            (void)text_length;
            valid = ops->validate_serial ? ops->validate_serial(
                input->context, text) : 0;
            if (!valid) {
                error_string = input->invalid_serial_string ?
                    input->invalid_serial_string :
                    STARS_MSG_DLG_INVALID_SERIAL_STRING;
                text[0] = 0;
                if (ops->format_error) ops->format_error(input->context,
                    error_string, text, (StarsU16)sizeof text);
                if (ops->alert) ops->alert(input->context, text,
                    STARS_MSG_DLG_ALERT_ERROR);
                if (ops->set_focus) ops->set_focus(input->context,
                    ops->get_dialog_item ? ops->get_dialog_item(
                    input->context, input->hwnd, edit) : edit);
                return 0;
            }
        }
        if (ops->end_dialog) ops->end_dialog(input->context, input->hwnd,
            (StarsI16)(input->wparam == STARS_MSG_DLG_ACCEPT));
        return 1;
    }
    if (input->wparam == STARS_MSG_DLG_HELP) {
        if (ops->win_help) ops->win_help(input->context, input->hwnd,
            input->help_file, STARS_MSG_DLG_HELP_COMMAND);
        return 1;
    }
    return 0;
}
