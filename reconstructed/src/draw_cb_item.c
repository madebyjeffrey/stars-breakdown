#include "core.h"

/* NE 10:6128-6207. Prepare the owner-draw item state, request its display
 * text, draw the production row, and finish with the focus-state handler. */
void stars_draw_cb_entire_item(StarsDrawCbItemInput *input, StarsI16 inflate)
{
    StarsI16 selected;
    StarsI16 listbox;
    StarsU16 message;
    if (!input) return;
    selected = (StarsI16)(input->item_state & 1U);
    listbox = (StarsI16)(input->hwnd_item == input->fleet_comp_list ||
        input->hwnd_item == input->planet_prod_list || inflate > 0);
    if (inflate > 0) inflate = (StarsI16)-inflate;
    message = listbox ? 0x040aU : 0x0408U;
    if (input->send_message)
        input->send_message(input->context, input->hwnd_item, message,
            input->item_id, 0x112057a4UL);
    if (input->draw_production)
        input->draw_production(input->context, input->hdc,
            &input->item_rect, input->work_text, inflate, selected, listbox);
    if (input->handle_focus)
        input->handle_focus(input->context, input, (StarsI16)(inflate + 2));
}
