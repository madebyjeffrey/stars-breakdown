#include "core.h"

/* Genuine C reconstruction of PopupWndProc, NE 19:0000-0137. */
StarsI32 stars_popup_wnd_proc(StarsPopupWndProcInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect rect;
    StarsU16 dc;

    if (!input)
        return 0;
    if (message == 1U)
        return 0;
    if (message == 0x0fU) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        dc = input->ops && input->ops->begin_paint ?
            input->ops->begin_paint(input->context, hwnd, &rect) : 0;
        if (input->ops && input->ops->draw_popup)
            input->ops->draw_popup(input->context, hwnd, dc);
        if (input->ops && input->ops->end_paint)
            input->ops->end_paint(input->context, hwnd, &rect);
        return 0;
    }
    if (message == 0x14U && (input->popup_kind == 9U ||
        input->popup_kind == 0x0bU || input->popup_kind == 0x0eU)) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops && input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &rect);
        if (input->ops && input->ops->fill_rect)
            input->ops->fill_rect(input->context, wparam, &rect,
                input->button_face_brush);
        return 1;
    }
    if (message == 0x0202U || message == 0x0205U) {
        if (input->popup_window == 0)
            return 0;
        if (input->ops && input->ops->destroy_window)
            input->ops->destroy_window(input->context, input->popup_window);
        input->popup_window = 0;
        input->popup_kind = 0;
        if (input->ops && input->ops->release_capture)
            input->ops->release_capture(input->context);
        if ((input->game_flags & 0x0800U) != 0U) {
            input->tutor_flags = (StarsU16)((input->tutor_flags & 0xfbffU) |
                0x0400U);
            if (input->ops && input->ops->advance_tutor)
                input->ops->advance_tutor(input->context);
        }
        return 0;
    }
    if (input->ops && input->ops->default_proc)
        return input->ops->default_proc(input->context, hwnd, message, wparam,
            lparam);
    return 0;
}
