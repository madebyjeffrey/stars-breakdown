#include "core.h"

/* NE 32:0E1C-0F36. Restore the tutorial's scanner and production-queue
 * snapshots, reconcile the menu state, and restore report sorting. */
void stars_restore_game_state(StarsRestoreGameStateInput *input)
{
    StarsU16 i;
    StarsU16 submenu;
    StarsU16 menu_flags;
    if (!input) return;
    input->grbit_scan = input->tutor_grbit_scan;
    input->scan_zoom = input->tutor_scan_zoom;
    for (i = 0; i < 13U; ++i) {
        input->zip_prod_saved[i][0] = input->zip_prod_current[i][0];
        input->zip_prod_saved[i][1] = input->zip_prod_current[i][1];
        input->zip_prod_current[i][0] = (StarsU8)input->tutor_zpq[i];
        input->zip_prod_current[i][1] =
            (StarsU8)(input->tutor_zpq[i] >> 8);
    }
    input->zip_saved_valid = input->zip_current_valid;
    input->zip_current_valid = (StarsU16)((input->tutor_zip_valid >> 4) & 1U);
    if (((input->game_grbits >> 15) & 1U) !=
        ((input->tutor_zip_valid >> 11) & 1U)) {
        submenu = input->get_sub_menu ?
            input->get_sub_menu(input->context, input->frame_window, 1U) : 0U;
        menu_flags = ((input->game_grbits >> 15) & 1U) != 0U ? 8U : 0U;
        input->game_grbits = (StarsU16)((input->game_grbits & 0x7fffU) |
            (((input->game_grbits >> 15) & 1U) << 15));
        if (input->check_menu)
            input->check_menu(input->context, submenu, 0x00b3U, menu_flags);
        if (input->refit_children) input->refit_children(input->context);
    }
    if (input->report_sort != input->tutor_report_sort) {
        input->report_sort = input->tutor_report_sort;
        if (input->invalidate_report)
            input->invalidate_report(input->context, 1U, 1U);
    }
}
