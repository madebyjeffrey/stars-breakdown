#include "core.h"

static StarsU16 print_map_high_word(StarsU32 value)
{ return (StarsU16)(value >> 16); }

static StarsI16 print_map_valid_page(const StarsU8 *text)
{ return text && text[0] >= '1' && text[0] <= '9' && text[1] == 0; }

/* NE 34:A1C2-A4A3. Handle print-map initialization, page validation, help,
 * and the numeric edit correction used by the original dialog procedure. */
StarsI16 stars_print_map_dlg(StarsPrintMapDlgInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect rect;
    StarsU16 i;
    StarsU16 control;
    StarsU8 text[10];
    const StarsU8 *error_text;
    if (!input) return 0;
    if (message == 0x0014U) {
        if (input->get_client_rect && input->fill_rect) {
            input->get_client_rect(input->context, hwnd, &rect);
            input->fill_rect(input->context, wparam, &rect,
                input->button_face_brush);
        }
        return 1;
    }
    if (message == 0x0019U && print_map_high_word(lparam) == 6U) {
        if (input->set_background_color)
            input->set_background_color(input->context, wparam,
                input->button_face_color);
        return (StarsI16)input->button_face_brush;
    }
    if (message == 0x0110U) {
        for (i = 0; i < 2U; ++i) {
            control = input->get_dialog_item ?
                input->get_dialog_item(input->context, hwnd,
                    (StarsU16)(0x010cU + i)) : 0U;
            if (input->send_message) {
                input->send_message(input->context, control, 0x0415U, 1U,
                    0UL);
                input->send_message(input->context, control, 0x0030U,
                    input->font_arial8, 0UL);
            }
            text[0] = (StarsU8)(input->page[i] + (StarsU16)'0');
            text[1] = 0;
            if (input->set_window_text)
                input->set_window_text(input->context, control, text);
        }
        if (input->sticky_dialog)
            input->sticky_dialog(input->context, hwnd, 1U);
        return 1;
    }
    if (message == 0x0111U) {
        if (wparam == 1U || wparam == 2U) {
            if (wparam == 1U && input->get_window_text) {
                for (i = 0; i < 2U; ++i) {
                    control = input->get_dialog_item ?
                        input->get_dialog_item(input->context, hwnd,
                            (StarsU16)(0x010cU + i)) : 0U;
                    text[0] = 0;
                    text[1] = 0;
                    input->get_window_text(input->context, control, text, 10U);
                    if (!print_map_valid_page(text)) {
                        error_text = input->format_ids ?
                            input->format_ids(input->context, 0x0527) :
                            (const StarsU8 *)0;
                        if (input->alert)
                            input->alert(input->context, error_text, 0x10);
                        if (input->set_focus)
                            input->set_focus(input->context, control);
                        break;
                    }
                    input->page[i] = (StarsU16)(text[0] - '0');
                }
            }
            if (input->sticky_dialog)
                input->sticky_dialog(input->context, hwnd, 0U);
            if (input->end_dialog)
                input->end_dialog(input->context, hwnd,
                    wparam == 1U ? 1 : 0);
            return 1;
        }
        if (wparam == 0x0076U) {
            if (input->win_help)
                input->win_help(input->context, hwnd, input->help_file,
                    1U, 0x0c3cU);
            return 1;
        }
        if ((wparam == 0x010cU || wparam == 0x010dU) &&
            print_map_high_word(lparam) == 0x0400U && input->get_window_text) {
            text[0] = 0;
            text[1] = 0;
            input->get_window_text(input->context, (StarsU16)lparam, text, 10U);
            if (text[0] && (text[0] < '1' || text[0] > '9')) {
                if (input->message_beep)
                    input->message_beep(input->context, 0U);
                if (input->set_window_text)
                    input->set_window_text(input->context, (StarsU16)lparam,
                        text + 1);
            }
        }
    }
    return 0;
}
