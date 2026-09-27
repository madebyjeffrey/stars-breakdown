#include <string.h>
#include "core.h"

/* Genuine C reconstruction of FindDlg, NE 0C:9286-945A. */
StarsI16 stars_find_dlg(StarsU16 window, StarsU16 message, StarsU16 wparam,
    StarsU32 lparam, const StarsFindDlgInput *input)
{
    StarsRect rect;
    StarsU8 text[42];
    StarsU16 edit;
    StarsI16 selected;
    const StarsU8 *formatted;

    if (!input || !input->ops)
        return 0;
    if (message == 0x14U) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, window, &rect);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->context, wparam, &rect,
                input->button_face_brush);
        return 1;
    }
    if (message == 0x19U) {
        if ((lparam >> 16) == 6U) {
            if (input->ops->set_background_color)
                input->ops->set_background_color(input->context, wparam,
                    input->button_face_color);
            return (StarsI16)input->button_face_brush;
        }
    } else if (message == 0x0110U) {
        if (input->ops->sticky_position)
            input->ops->sticky_position(input->context, window,
                input->sticky_point, 1);
        if (input->ops->send_item_message)
            input->ops->send_item_message(input->context, window, 0x010cU,
                0x0415U, 0x0027U, 0);
        return 1;
    } else if (message == 0x0111U) {
        if (wparam == 1U || wparam == 2U) {
            if (wparam == 1U) {
                memset(text, 0, sizeof text);
                if (input->ops->get_item_text)
                    input->ops->get_item_text(input->context, window,
                        0x010cU, text, 0x28U);
                selected = input->ops->select_name ?
                    input->ops->select_name(input->context, text) : 0;
                if (selected == 0) {
                    formatted = input->ops->format_ids ?
                        input->ops->format_ids(input->context, 0x020eU) :
                        (const StarsU8 *)0;
                    if (input->ops->alert)
                        input->ops->alert(input->context, formatted, 0x10);
                    edit = input->ops->get_item ?
                        input->ops->get_item(input->context, window, 0x010cU) : 0;
                    if (input->ops->set_focus)
                        input->ops->set_focus(input->context, edit);
                    if (input->ops->send_item_message)
                        input->ops->send_item_message(input->context, window,
                            0x010cU, 0x0401U, 0, (StarsU32)0xffff0000UL);
                    return 0;
                }
            }
            if (input->ops->sticky_position)
                input->ops->sticky_position(input->context, window,
                    input->sticky_point, 0);
            if (input->ops->end_dialog)
                input->ops->end_dialog(input->context, window,
                    (StarsI16)(wparam == 1U));
            return 1;
        }
        if (wparam == 0x76U) {
            if (input->ops->win_help)
                input->ops->win_help(input->context, window,
                    input->help_file, 1, 0x043dU);
            return 1;
        }
    }
    return 0;
}
