#include "core.h"

#define NEW_GAME3_WM_PAINT 0x000fU
#define NEW_GAME3_WM_ERASE_BACKGROUND 0x0014U
#define NEW_GAME3_WM_CONTROL_COLOR 0x0019U
#define NEW_GAME3_WM_SETCURSOR 0x0020U
#define NEW_GAME3_WM_INIT_DIALOG 0x0110U
#define NEW_GAME3_WM_COMMAND 0x0111U
#define NEW_GAME3_WM_LBUTTONDOWN 0x0201U
#define NEW_GAME3_WM_LBUTTONDBLCLK 0x0203U
#define NEW_GAME3_ID_HELP 0x0076U
#define NEW_GAME3_ID_VC_FIRST 0x0123U
#define NEW_GAME3_ID_VC_LAST 0x0129U
#define NEW_GAME3_VC_MESSAGE_SET_CHECK 0x0401U
#define NEW_GAME3_VC_MESSAGE_GET_CHECK 0x0400U
#define NEW_GAME3_RACE_MESSAGE 0x0400U
#define NEW_GAME3_VC_BUTTON_COUNT 7U
#define NEW_GAME3_RACE_BUTTON_COUNT 4U
#define NEW_GAME3_CONTROL_COLOR_EDIT 6U
#define NEW_GAME3_HELP_CONTEXT 0x03fdU

static StarsI16 new_game3_vc_index(StarsU16 control)
{
    StarsU16 offset = (StarsU16)(control - NEW_GAME3_ID_VC_FIRST);
    return (StarsI16)(offset + (offset > 1U ? 1U : 0U));
}

/* NE 16:99F4-9D4C.  The wizard's third page coordinates its controls and
 * race-selection buttons; drawing, dialog state, and the inherited game
 * record are kept behind explicit callbacks. */
StarsI16 stars_new_game_dlg3(StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam, StarsNewGameDlg3Input *input)
{
    const StarsNewGameDlg3Ops *ops;
    StarsRect client;
    StarsPoint point;
    StarsU16 control;
    StarsU16 index;
    StarsI16 hit;
    StarsI16 value;
    StarsU16 dc;

    if (!input || !input->ops)
        return 0;
    ops = input->ops;
    if (message == NEW_GAME3_WM_PAINT) {
        dc = ops->begin_paint ? ops->begin_paint(input->context, hwnd) : 0U;
        if (ops->draw)
            ops->draw(input->context, hwnd, dc, -1);
        if (ops->end_paint)
            ops->end_paint(input->context, hwnd, dc);
        return 1;
    }
    if (message == NEW_GAME3_WM_ERASE_BACKGROUND) {
        if (ops->get_client_rect)
            ops->get_client_rect(input->context, hwnd, &client);
        if (ops->fill_rect)
            ops->fill_rect(input->context, wparam, &client,
                input->button_face_brush);
        return 1;
    }
    if (message == NEW_GAME3_WM_CONTROL_COLOR) {
        control = (StarsU16)lparam;
        for (index = NEW_GAME3_ID_VC_FIRST;
                index <= NEW_GAME3_ID_VC_LAST; ++index) {
            if (ops->get_dialog_item &&
                    ops->get_dialog_item(input->context, hwnd, index) ==
                    control)
                break;
        }
        if (index <= NEW_GAME3_ID_VC_LAST ||
                (StarsU16)(lparam >> 16) == NEW_GAME3_CONTROL_COLOR_EDIT) {
            if (ops->set_background_color)
                ops->set_background_color(input->context, wparam,
                    input->button_face_color);
            return (StarsI16)input->button_face_brush;
        }
        return 0;
    }
    if (message == NEW_GAME3_WM_SETCURSOR) {
        if (ops->get_cursor_pos)
            ops->get_cursor_pos(input->context, &point);
        if (ops->screen_to_client)
            ops->screen_to_client(input->context, hwnd, &point);
        hit = ops->hit_test ? ops->hit_test(input->context, point) : -1;
        if (hit >= 0) {
            if (ops->set_cursor)
                ops->set_cursor(input->context, input->hand_cursor);
            return 1;
        }
        return 0;
    }
    if (message == NEW_GAME3_WM_INIT_DIALOG) {
        if (ops->set_title)
            ops->set_title(input->context, hwnd, 3);
        for (index = 0; index < NEW_GAME3_VC_BUTTON_COUNT; ++index) {
            control = (StarsU16)(NEW_GAME3_ID_VC_FIRST + index);
            value = ops->get_vc_check ? ops->get_vc_check(input->context,
                new_game3_vc_index(control)) : 0;
            if (ops->get_dialog_item)
                control = ops->get_dialog_item(input->context, hwnd, control);
            if (ops->send_message)
                ops->send_message(input->context, control,
                    NEW_GAME3_VC_MESSAGE_SET_CHECK, (StarsU16)value, 0UL);
            if (input->read_only && ops->enable_window)
                ops->enable_window(input->context, control, 0U);
        }
        if (ops->sticky_position)
            ops->sticky_position(input->context, hwnd, input->sticky_point, 1);
        return 1;
    }
    if (message == NEW_GAME3_WM_COMMAND) {
        if (wparam == NEW_GAME3_ID_HELP) {
            if (ops->win_help)
                ops->win_help(input->context, hwnd, input->help_file, 1,
                    NEW_GAME3_HELP_CONTEXT);
            return 1;
        }
        for (index = 0; index < input->race_button_count; ++index)
            if (input->race_button_ids && wparam == input->race_button_ids[index])
                break;
        if (index < input->race_button_count) {
            if (ops->sticky_position)
                ops->sticky_position(input->context, hwnd,
                    input->sticky_point, 0);
            if (ops->end_dialog)
                ops->end_dialog(input->context, hwnd, (StarsI16)index);
            return 1;
        }
        if (wparam > NEW_GAME3_ID_VC_FIRST - 1U &&
                wparam < NEW_GAME3_ID_VC_LAST + 1U) {
            control = ops->get_dialog_item ?
                ops->get_dialog_item(input->context, hwnd, wparam) : wparam;
            value = ops->send_message ? (StarsI16)ops->send_message(
                input->context, control, NEW_GAME3_VC_MESSAGE_GET_CHECK,
                0U, 0UL) : 0;
            if (ops->set_vc_check)
                ops->set_vc_check(input->context, new_game3_vc_index(wparam),
                    value);
            if (ops->draw)
                ops->draw(input->context, hwnd, 0U, 8);
        }
        return 0;
    }
    if (message == NEW_GAME3_WM_LBUTTONDOWN ||
            message == NEW_GAME3_WM_LBUTTONDBLCLK) {
        point.x = (StarsU16)lparam;
        point.y = (StarsU16)(lparam >> 16);
        return ops->track ? ops->track(input->context, hwnd, point,
            (StarsI16)wparam) : 0;
    }
    return 0;
}
