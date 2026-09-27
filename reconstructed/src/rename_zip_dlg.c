#include "core.h"

/* Genuine C reconstruction of RenameZipDlg, NE 17:0880-0A6A. */
StarsI16 stars_rename_zip_dlg(StarsU16 dialog, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam, StarsRenameZipInput *input)
{
    StarsRect rect;
    StarsU16 edit;
    StarsU16 title_id;
    StarsU16 help_id;
    const StarsU8 *title;

    if (!input)
        return 0;
    if (message == 0x0014U) {
        if (input->get_client_rect && input->fill_rect) {
            input->get_client_rect(input->context, dialog, &rect);
            input->fill_rect(input->context, wparam, &rect,
                input->button_face_brush);
        }
        return 1;
    }
    if (message == 0x0019U) {
        if ((lparam >> 16) == 6UL && input->set_background_color) {
            input->set_background_color(input->context, wparam,
                input->button_face_color);
            return (StarsI16)input->button_face_brush;
        }
        return 0;
    }
    if (message == 0x0110U) {
        title_id = input->existing_order_dialog ? 0x04c2U : 0x04c3U;
        title = input->get_string ?
            input->get_string(input->context, title_id) :
            (const StarsU8 *)0;
        if (input->set_window_text && title)
            input->set_window_text(input->context, dialog, title);
        if (input->set_window_text && input->get_dialog_item) {
            edit = input->get_dialog_item(input->context, dialog, 0x010cU);
            input->set_window_text(input->context, edit, input->work);
        }
        if (input->set_window_pos)
            input->set_window_pos(input->context, dialog,
                (StarsI16)(input->sticky_position.x + 0x46),
                (StarsI16)(input->sticky_position.y + 0x46), 0x0015U);
        if (input->send_dialog_item_message)
            input->send_dialog_item_message(input->context, dialog, 0x010cU,
                0x0415U, 0x000cU, 0UL);
        if (input->sticky_position_call)
            input->sticky_position_call(input->context, dialog,
                &input->sticky_position, 1);
        return 1;
    }
    if (message == 0x0111U) {
        if (wparam == 1U || wparam == 2U) {
            if (wparam == 1U && input->get_dialog_text)
                input->get_dialog_text(input->context, dialog, 0x010cU,
                    input->work, 0x000eU);
            if (input->sticky_position_call)
                input->sticky_position_call(input->context, dialog,
                    &input->sticky_position, 0);
            if (input->end_dialog)
                input->end_dialog(input->context, dialog,
                    (StarsU16)(wparam == 1U));
            return 1;
        }
        if (wparam == 0x0076U) {
            help_id = input->existing_order_dialog ? 0x0c1fU : 0x0452U;
            if (input->win_help)
                input->win_help(input->context, dialog, input->help_file,
                    help_id);
            return 1;
        }
    }
    return 0;
}
