#include "core.h"

#define RENAME_WM_PAINT 0x0014U
#define RENAME_WM_CONTROL_COLOR 0x0019U
#define RENAME_WM_INIT_DIALOG 0x0110U
#define RENAME_WM_COMMAND 0x0111U
#define RENAME_EDIT_CONTROL 0x010cU
#define RENAME_EDIT_NOTIFY 0x0400U
#define RENAME_EDIT_SET_SELECTION 0x0401U
#define RENAME_ID_OK 1U
#define RENAME_ID_CANCEL 2U
#define RENAME_ID_HELP 0x0076U
#define RENAME_HELP_CONTEXT 0x0447U
#define RENAME_TITLE_STRING 0x02dcU
#define RENAME_EDIT_LIMIT_MESSAGE 0x0415U
#define RENAME_EDIT_LIMIT 0x001fU
#define RENAME_MAX_TEXT 0x0020U
#define RENAME_EDIT_TEXT_CAPACITY 0x00faU
#define RENAME_EDIT_WIDTH 0x00a0U
#define RENAME_STICKY_OFFSET 0x0046
#define RENAME_SET_POSITION_FLAGS 0x0015U
#define RENAME_BUTTON_FACE_DEFAULT 0x00ffffffUL
#define RENAME_BUTTON_FACE_BACKGROUND 0UL

static StarsI16 rename_edit_notification(StarsU32 lparam,
    StarsRenameInput *input)
{
    StarsU16 edit;
    StarsU32 selection;
    StarsI16 fits;

    if ((StarsU16)(lparam >> 16) != RENAME_EDIT_NOTIFY ||
        input->edit_update != 0U || !input->get_window_text ||
        !input->send_message || !input->fits_screen)
        return 0;
    input->edit_update = 1U;
    edit = (StarsU16)lparam;
    input->get_window_text(input->context, edit, input->work,
        RENAME_EDIT_TEXT_CAPACITY);
    selection = input->send_message(input->context, edit,
        RENAME_EDIT_NOTIFY, 0U, 0UL);
    fits = input->fits_screen(input->context, input->work,
        RENAME_EDIT_WIDTH);
    if (fits == 0) {
        if (input->set_window_text)
            input->set_window_text(input->context, edit, input->work);
        input->send_message(input->context, edit,
            RENAME_EDIT_SET_SELECTION, 0U, selection);
    }
    input->edit_update = 0U;
    return 1;
}

/* Genuine C reconstruction of RenameDlg, NE 11:0A68-0CFD. */
StarsI16 stars_rename_dlg(StarsU16 dialog, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam, StarsRenameInput *input)
{
    StarsRect rect;
    StarsU16 edit;
    StarsU16 command;
    const StarsU8 *title;

    if (!input)
        return 0;
    if (message == RENAME_WM_PAINT) {
        if (input->get_client_rect && input->fill_rect) {
            input->get_client_rect(input->context, dialog, &rect);
            input->fill_rect(input->context, wparam, &rect,
                input->button_face_brush);
        }
        return 1;
    }
    if (message == RENAME_WM_CONTROL_COLOR) {
        if ((lparam >> 16) == 6UL && input->set_background_color) {
            input->set_background_color(input->context, wparam,
                input->button_face_color);
            return (StarsI16)input->button_face_brush;
        }
        return 0;
    }
    if (message == RENAME_WM_INIT_DIALOG) {
        title = input->get_string ? input->get_string(input->context,
            RENAME_TITLE_STRING) : (const StarsU8 *)0;
        if (title && input->set_window_text)
            input->set_window_text(input->context, dialog, title);
        if (input->set_window_pos)
            input->set_window_pos(input->context, dialog,
                (StarsI16)(input->sticky_position.x + RENAME_STICKY_OFFSET),
                (StarsI16)(input->sticky_position.y + RENAME_STICKY_OFFSET),
                RENAME_SET_POSITION_FLAGS);
        if (input->send_dialog_item_message)
            input->send_dialog_item_message(input->context, dialog,
                RENAME_EDIT_CONTROL, RENAME_EDIT_LIMIT_MESSAGE,
                RENAME_EDIT_LIMIT, 0UL);
        if (input->get_dialog_item && input->set_window_text) {
            edit = input->get_dialog_item(input->context, dialog,
                RENAME_EDIT_CONTROL);
            input->set_window_text(input->context, edit, input->work);
        }
        if (input->sticky_position_call)
            input->sticky_position_call(input->context, dialog,
                &input->sticky_position, 1);
        return 1;
    }
    if (message != RENAME_WM_COMMAND)
        return 0;
    command = wparam;
    if (command == RENAME_ID_OK || command == RENAME_ID_CANCEL) {
        if (command == RENAME_ID_OK && input->get_dialog_text)
            input->get_dialog_text(input->context, dialog,
                RENAME_EDIT_CONTROL, input->work, RENAME_MAX_TEXT);
        if (input->fits_screen && command == RENAME_ID_OK)
            input->fits_screen(input->context, input->work,
                RENAME_EDIT_WIDTH);
        if (input->sticky_position_call)
            input->sticky_position_call(input->context, dialog,
                &input->sticky_position, 0);
        if (input->end_dialog)
            input->end_dialog(input->context, dialog,
                (StarsU16)(command == RENAME_ID_OK));
        return 1;
    }
    if (command == RENAME_EDIT_CONTROL)
        return rename_edit_notification(lparam, input);
    if (command == RENAME_ID_HELP) {
        if (input->win_help)
            input->win_help(input->context, dialog, input->help_file,
                RENAME_HELP_CONTEXT);
        return 1;
    }
    return 0;
}
