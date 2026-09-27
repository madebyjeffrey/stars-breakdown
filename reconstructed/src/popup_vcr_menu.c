#include "core.h"

#define VCR_MENU_MAX_ITEMS 40
#define VCR_MENU_LABEL_BYTES 64
#define VCR_MENU_NO_SELECTION (-1)

static StarsI16 menu_losses(const StarsPopupVcrMenuInput *input,
    StarsI16 token_id)
{
    StarsI16 total = 0;
    StarsI16 step;
    if (input->current_step < 0) return 0;
    for (step = 0; (StarsU16)step < input->step_count && step <= input->current_step; ++step)
        if (input->steps[step].token_index == (StarsU16)token_id)
            total = (StarsI16)(total + input->steps[step].ship_loss);
    return total;
}

/* Genuine semantic reconstruction of PopupVCRMenu, NE 30:4518-48F5.
 * It gathers living tokens on the requested board square, formats a readable
 * player/design/ship-count label, appends current-step losses when the square
 * is the active VCR location, preserves the focused token as the initial menu
 * selection, and maps the popup's chosen row back to the original token id. */
StarsI16 stars_popup_vcr_menu(StarsU16 hwnd, StarsI16 x, StarsI16 y,
    StarsU16 battle_square, StarsPopupVcrMenuInput *input)
{
    StarsU8 labels[VCR_MENU_MAX_ITEMS][VCR_MENU_LABEL_BYTES];
    StarsI16 ids[VCR_MENU_MAX_ITEMS];
    StarsI16 selected = VCR_MENU_NO_SELECTION;
    StarsI16 selected_row = VCR_MENU_NO_SELECTION;
    StarsI16 count = 0;
    StarsI16 token_id;
    StarsI16 losses;
    if (!input || !input->tokens || !input->format || !input->popup) return -1;
    for (token_id = 0; (StarsU16)token_id < input->token_count &&
            count < VCR_MENU_MAX_ITEMS; ++token_id) {
        if (input->tokens[token_id].battle_square != battle_square ||
                input->tokens[token_id].ship_count <= 0) continue;
        losses = menu_losses(input, token_id);
        input->format(input->context, &input->tokens[token_id], losses,
            labels[count], VCR_MENU_LABEL_BYTES);
        ids[count] = token_id;
        if (token_id == input->focused_token) selected = count;
        ++count;
    }
    if (!count) return -1;
    selected_row = input->popup(input->context, hwnd, x, y, (StarsU16)count,
        &labels[0][0], selected);
    if (selected_row < 0 || selected_row >= count) return -1;
    return ids[selected_row];
}
