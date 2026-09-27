#include "core.h"

/* NE 0B:C336-C5E5. Remove a normal ship design from every matching fleet;
 * split cargo through the existing balance operation when other designs
 * remain, otherwise delete the fleet. Starbase designs use the dedicated
 * cleanup path. */
void stars_destroy_all_ishdef(StarsDestroyIshdefInput *input,
    StarsI16 ishdef)
{
    StarsU16 fleet_index;

    if (!input || !input->fleets) return;
    if (ishdef >= 16) {
        if (input->destroy_starbase_design)
            input->destroy_starbase_design(input->context,
                (StarsI16)(ishdef - 16), input->player);
        if (input->invalidate_report)
            input->invalidate_report(input->context, 0, 1);
    } else {
        for (fleet_index = 0; fleet_index < input->fleet_count;
                ++fleet_index) {
            StarsDestroyIshdefFleet *fleet = &input->fleets[fleet_index];
            StarsU16 other;
            StarsI16 removed;
            StarsDestroyIshdefFleet source_copy;
            StarsDestroyIshdefFleet split;

            if (fleet->player != input->player ||
                    fleet->ship_counts[(StarsU16)ishdef] <= 0)
                continue;
            removed = fleet->ship_counts[(StarsU16)ishdef];
            other = 0;
            while (other < 16U &&
                    (other == (StarsU16)ishdef ||
                     fleet->ship_counts[other] == 0))
                ++other;
            if (other < 16U) {
                source_copy = *fleet;
                split.id = 0;
                split.player = input->player;
                split.flags = 0x0400U;
                for (other = 0; other < 16U; ++other)
                    split.ship_counts[other] = 0;
                split.ship_counts[(StarsU16)ishdef] = removed;
                if (input->transfer_balance)
                    input->transfer_balance(input->context, &source_copy,
                        &split);
                *fleet = source_copy;
                if (input->selected_is_fleet &&
                        input->selected_fleet_id == source_copy.id) {
                    if (input->refresh_selection)
                        input->refresh_selection(input->context,
                            source_copy.id);
                    if (input->invalidate_window)
                        input->invalidate_window(input->context, 0);
                }
            } else {
                fleet->ship_counts[(StarsU16)ishdef] = 0;
                if (input->delete_fleet)
                    input->delete_fleet(input->context, fleet->id, 0, -1);
            }
        }
        if (input->invalidate_report)
            input->invalidate_report(input->context, 1, 1);
    }
    if (input->remove_from_queues)
        input->remove_from_queues(input->context, ishdef, 0);
}
