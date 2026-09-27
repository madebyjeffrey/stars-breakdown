#include "core.h"

/* NE 11:C82C-C9F5. Count matching ship-design production entries across the
 * eligible owned planets and report whether any matching queue item has
 * progress. */
StarsI16 stars_count_queued_production(const StarsQueuedProductionInput *input,
    StarsI16 *progress)
{
    StarsI16 total = 0;
    StarsU16 planet;
    StarsU16 item;
    StarsU32 raw;
    if (progress) *progress = 0;
    if (!input || !input->planets) return 0;
    for (planet = 0; planet < input->planet_count; ++planet) {
        const StarsQueuedProductionPlanet *p = &input->planets[planet];
        if (!p->has_production || !p->item_count ||
            p->player != input->current_player || !p->build_enabled ||
            (input->space_docks_only && !p->space_dock)) continue;
        for (item = 0; item < p->item_count; ++item) {
            raw = p->items[item];
            if (((raw >> 17) & 7UL) == 2UL &&
                ((raw >> 10) & 0x7fUL) == (StarsU32)(input->design & 0x7f)) {
                total = (StarsI16)(total + (StarsI16)(raw & 0x3ffUL));
                if (progress && ((raw >> 20) & 0x7fUL) != 0UL) *progress = 1;
            }
        }
    }
    return total;
}
