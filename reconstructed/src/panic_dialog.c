/* Genuine C reconstruction of PanicDlg (NE 32:026a-0374). */
#include "core.h"

StarsI16 stars_panic_dialog(const StarsPanicDialogInput *input)
{
    StarsRect rect;
    StarsI16 high_word;

    if (!input) return 0;
    if (input->message == 0x0014U) {
        if (input->get_client_rect && input->fill_rect) {
            input->get_client_rect(input->context, input->hwnd, &rect);
            input->fill_rect(input->context, input->wparam, &rect,
                input->button_face);
        }
        return 1;
    }
    if (input->message == 0x0019U) {
        high_word = (StarsI16)(input->lparam >> 16);
        if (high_word == 6 && input->set_background_color) {
            input->set_background_color(input->context, input->wparam,
                input->color_button_face);
            return (StarsI16)input->button_face;
        }
    } else if (input->message == 0x0111U) {
        if (input->wparam == 2U) {
            if (input->end_dialog)
                input->end_dialog(input->context, input->hwnd, 0);
            return 1;
        }
        if (input->wparam == 0x76U) {
            if (input->win_help)
                input->win_help(input->context, input->hwnd,
                    input->help_file, 1U, input->tutor_help_id);
            return 1;
        }
        if (input->wparam == 0x9c9U || input->wparam == 0x9caU) {
            if (input->end_dialog)
                input->end_dialog(input->context, input->hwnd,
                    (StarsI16)input->wparam);
            return 1;
        }
    }
    return 0;
}
