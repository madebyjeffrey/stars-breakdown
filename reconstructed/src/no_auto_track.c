#include "core.h"

/* NE 17:1D32-1E52. Redirect enemy fleets' matching rendezvous orders to the
 * target fleet's current coordinates and mark those orders as non-tracking. */
void stars_no_auto_track_fleet(StarsNoAutoTrackInput *input)
{
    StarsU16 fleet;
    StarsU16 order;
    StarsAutoTrackFleet *f;
    StarsAutoTrackOrder *o;
    if (!input || !input->fleets) return;
    for (fleet = 0; fleet < input->fleet_count; ++fleet) {
        f = &input->fleets[fleet];
        if (!f->present) break;
        if (f->player != input->target_player && f->cord > 1U && f->orders) {
            for (order = 1U; order < f->order_count; ++order) {
                o = &f->orders[order];
                if (((o->flags >> 8) & 0x000fU) == 2U &&
                    o->target_fleet == input->target_fleet) {
                    o->flags = (StarsU16)(o->flags | 0x2000U);
                    o->x = input->target_x;
                    o->y = input->target_y;
                }
            }
        }
    }
}
