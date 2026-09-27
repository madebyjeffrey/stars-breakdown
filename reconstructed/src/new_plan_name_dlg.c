#include "core.h"

/* Genuine C reconstruction of NewPlanNameDlg, NE 31:04CE-0652. */
StarsI16 stars_new_plan_name_dlg(StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam, const StarsNewPlanNameDlgInput *input)
{
    StarsRect rect;

    if (!input || !input->ops)
        return 0;
    if (message == 0x14U) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &rect);
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
        if (input->ops->set_window_pos && input->sticky_point)
            input->ops->set_window_pos(input->context, hwnd, 0,
                (StarsI16)(input->sticky_point->x + 0x46),
                (StarsI16)(input->sticky_point->y + 0x46), 0, 0, 0x15U);
        if (input->ops->send_item_message)
            input->ops->send_item_message(input->context, hwnd, 0x010cU,
                0x0415U, 0x001fU, 0);
        if (input->ops->set_item_text && input->plan_name)
            input->ops->set_item_text(input->context, hwnd, 0x010cU,
                input->plan_name);
        return 1;
    } else if (message == 0x0111U) {
        if (wparam == 1U || wparam == 2U) {
            if (wparam == 1U && input->ops->get_item_text &&
                input->plan_name) {
                input->ops->get_item_text(input->context, hwnd, 0x010cU,
                    input->plan_name, 0x20U);
                if (input->dirty_plan)
                    *input->dirty_plan = 1;
            }
            if (input->ops->end_dialog)
                input->ops->end_dialog(input->context, hwnd,
                    (StarsI16)(wparam == 1U));
            return 1;
        }
        if (wparam == 0x76U) {
            if (input->ops->win_help)
                input->ops->win_help(input->context, hwnd, input->help_file,
                    1, 0x0439U);
            return 1;
        }
    }
    return 0;
}
