#include "core.h"

/* NE 32:0C90-0E1B. Capture tutorial scanner and production snapshots,
 * reconcile the scanner menu, reset report sorting, and discard a live game
 * when the saved-game state requires it. */
void stars_save_game_state(StarsSaveGameStateInput *input)
{
    StarsU16 i;
    StarsU16 submenu;

    if (!input) return;
    input->tutor_grbit_scan = input->current_grbit_scan;
    input->tutor_scan_zoom = input->current_scan_zoom;
    input->tutor_flags = (StarsU16)((input->tutor_flags & 0xf7fdU) |
        (((input->display_flags >> 15) & 1U) << 11));
    for (i = 0; i < 13U; ++i)
        input->tutor_zpq[i] = (StarsU16)(
            input->current_queue[i].count |
            ((StarsU16)input->current_queue[i].no_research << 8));
    input->tutor_zip_valid = (StarsU16)((input->tutor_zip_valid & 0xefffU) |
        ((input->current_valid & 1U) << 12));
    for (i = 0; i < 13U; ++i) {
        input->current_queue[i].no_research =
            input->saved_queue[i].no_research;
        input->current_queue[i].count = input->saved_queue[i].count;
    }
    if ((input->display_flags & 0x8000U) == 0U) {
        submenu = input->get_sub_menu ?
            input->get_sub_menu(input->context, input->frame_window, 1U) : 0U;
        input->display_flags = (StarsU16)(input->display_flags | 0x8000U);
        if (input->check_menu)
            input->check_menu(input->context, submenu, 0x00b3U, 8U);
        if (input->refit_children) input->refit_children(input->context);
    }
    input->tutor_report_sort = input->report_sort;
    if (input->report_sort != 1 || !input->report_ascending) {
        input->report_sort = 1;
        input->report_ascending = 1;
        if (input->invalidate_report)
            input->invalidate_report(input->context, 1U, 0U);
    }
    if (input->game_lid != 0UL && !(input->game_flags & 8U) &&
        input->destroy_game)
        input->destroy_game(input->context);
}
