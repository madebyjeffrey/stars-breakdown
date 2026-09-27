#include "core.h"

enum {
    SELECT_ADJ_NO_FLEET = -1,
    SELECT_ADJ_SCANNER_ENEMY = 0x0083,
    SELECT_ADJ_SCANNER_LOCAL = 0x0082,
    SELECT_ADJ_FLEET_KIND = 2,
    SELECT_ADJ_INVALIDATE_REPORT = 1,
    SELECT_ADJ_INVALIDATE_WINDOW = 1
};

/* The original routine works on the global fleet pointer table and the
 * current player's compact fleet-order table.  The input and callback
 * boundary below makes those global dependencies explicit without changing
 * the 16-bit decisions made by the original code. */
static StarsI16 find_fleet(const StarsSelectAdjFleetInput *input,
    StarsI16 fleet_id)
{
    StarsU16 index;
    for (index = 0; index < input->fleet_count; ++index)
        if (input->fleets[index].id == fleet_id)
            return (StarsI16)index;
    return SELECT_ADJ_NO_FLEET;
}

/* NE 0B:3D32-400E. Select an adjacent fleet, wrapping through the local
 * player's fleet order, and keep the scanner, report, and mine controls in
 * step with the new selection. */
void stars_select_adj_fleet(StarsI16 increment, StarsI16 fleet_id,
    StarsSelectAdjFleetInput *input)
{
    StarsI16 fleet_index;
    StarsI16 old_dropdown = SELECT_ADJ_NO_FLEET;
    StarsI16 order_index;
    StarsI16 next_index;
    const StarsSelectAdjFleetRecord *fleet;

    if (!input || !input->fleets || !input->ops ||
            input->fleet_count == 0U)
        return;
    /* Keyboard navigation starts from the currently selected fleet; an
     * explicit id is used only for a direct selection. */
    if (increment != 0)
        fleet_id = input->selected_fleet_id;
    /* The report is invalidated before the requested fleet is resolved, just
     * as in the original, so a failed lookup still clears a stale report. */
    if (input->report_cached == 0U && input->ops->invalidate_report)
        input->ops->invalidate_report(input->context,
            SELECT_ADJ_INVALIDATE_REPORT, SELECT_ADJ_INVALIDATE_REPORT);
    fleet_index = find_fleet(input, fleet_id);
    if (fleet_index < 0)
        return;
    fleet = &input->fleets[fleet_index];
    if (fleet->player != input->player) {
        /* Enemy fleets can be shown in the scanner, but they are not copied
         * into the local main-object selection. */
        if (input->ops->change_scan_selection)
            input->ops->change_scan_selection(input->context, fleet->point,
                SELECT_ADJ_SCANNER_ENEMY);
    } else {
        if (increment == 0) {
            /* A direct selection preserves the dropdown id only when the
             * scanner was already on this fleet's point. */
            if (input->selection_kind == SELECT_ADJ_FLEET_KIND &&
                    input->selected_point.x == fleet->point.x &&
                    input->selected_point.y == fleet->point.y)
                old_dropdown = fleet->id;
        } else if (input->player_fleet_order &&
                input->player_fleet_count != 0U) {
            /* The executable uses one-step wraparound rather than modulo;
             * retain that behavior for large or unusual increments. */
            order_index = 0;
            while (order_index < (StarsI16)input->player_fleet_count &&
                    input->fleets[input->player_fleet_order[order_index]].id !=
                    fleet_id)
                ++order_index;
            next_index = (StarsI16)(order_index + increment);
            if (next_index < 0)
                next_index = (StarsI16)input->player_fleet_count - 1;
            else if (next_index >= (StarsI16)input->player_fleet_count)
                next_index = 0;
            fleet_index = (StarsI16)input->player_fleet_order[next_index];
            fleet = &input->fleets[fleet_index];
        }
        if (input->ops->change_scan_selection)
            input->ops->change_scan_selection(input->context, fleet->point,
                SELECT_ADJ_SCANNER_LOCAL);
        /* Local selection updates both the scanner highlight and the main
         * object, with the two redraws deliberately bracketing the latter. */
        if (input->ops->redraw_scan_selection)
            input->ops->redraw_scan_selection(input->context, 0, 0);
        if (input->ops->change_main_object)
            input->ops->change_main_object(input->context,
                SELECT_ADJ_FLEET_KIND, fleet->id);
        if (input->ops->redraw_scan_selection)
            input->ops->redraw_scan_selection(input->context, 0, 1);
    }
    if (input->ops->center_scanner)
        input->ops->center_scanner(input->context, fleet->point, 1);
    if (input->ops->draw_scroll_bar)
        input->ops->draw_scroll_bar(input->context);
    if (input->ops->invalidate_mine_window)
        input->ops->invalidate_mine_window(input->context, input->mine_window,
            SELECT_ADJ_INVALIDATE_WINDOW);
    if (input->ops->set_mineral_title)
        input->ops->set_mineral_title(input->context, input->mine_window);
    if (old_dropdown != SELECT_ADJ_NO_FLEET && input->ops->set_fleet_dropdown)
        input->ops->set_fleet_dropdown(input->context, old_dropdown);
}
