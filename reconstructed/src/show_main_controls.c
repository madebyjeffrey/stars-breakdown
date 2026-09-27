#include "core.h"

/* NE 26:0160-027C. Show or hide the main controls and switch the primary
 * action label between the two compressed strings used by the dialog. */
void stars_show_main_controls(StarsShowMainControlsInput *input)
{
    static const StarsU16 control_ids[9] = {
        0x0816U, 0x0818U, 0x0817U, 0x0810U, 0x0811U,
        0x0812U, 0x0813U, 0x0814U, 0x0815U
    };
    StarsU16 i;
    StarsU16 control;
    StarsI16 button_show;
    StarsI16 string_id;
    const StarsU8 *text;

    if (!input || !input->get_dialog_item || !input->show_window)
        return;
    for (i = 0; i < 9U; ++i) {
        control = input->get_dialog_item(input->context, input->hwnd,
            control_ids[i]);
        input->show_window(input->context, control, input->show);
    }
    control = input->get_dialog_item(input->context, input->hwnd, 1U);
    button_show = input->show == 5 ? 0 : 5;
    input->show_window(input->context, control, button_show);
    string_id = input->show == 5 ? 0x02e2 : 0x02e3;
    text = input->get_string ?
        input->get_string(input->context, string_id) : (const StarsU8 *)0;
    if (input->set_dialog_item_text)
        input->set_dialog_item_text(input->context, input->hwnd, 2U, text);
}
