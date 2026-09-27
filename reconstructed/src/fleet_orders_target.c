#include "core.h"

/* NE 0B:CAFE-CCFF. Retarget orders that reference a fleet being removed or
 * relocated.  The first matching order determines the replacement object;
 * later matches reuse that result exactly as the original did. */
void stars_fleet_orders_change_target(StarsFleetOrdersTargetInput *input,
    StarsFleetTargetRecord *old_fleet)
{
    StarsU16 i;
    StarsI16 j;
    StarsU16 target;
    StarsU16 target_kind;
    StarsNearestTarget nearest;
    StarsI16 found;

    if (!input || !old_fleet || !input->fleets || !input->find_nearest)
        return;
    found = 0;
    for (i = 0; i < input->fleet_count; ++i) {
        StarsFleetTargetRecord *fleet = &input->fleets[i];
        if (!fleet->orders)
            continue;
        for (j = (StarsI16)fleet->order_count - 1; j >= 0; --j) {
            StarsFleetTargetOrder *order = &fleet->orders[j];
            if (((order->flags >> 8) & 0x0fU) != 2U ||
                    order->target != old_fleet->token)
                continue;
            if (!found) {
                if (!input->find_nearest(input->context,
                        (StarsI16)(old_fleet->x + 1), old_fleet->y,
                        0x83U, &nearest)) {
                    target_kind = 4U;
                    target = (StarsU16)j;
                } else if ((nearest.grobj_full & 2U) == 0U) {
                    target_kind = 1U;
                    target = nearest.planet_id;
                } else {
                    target_kind = 2U;
                    if (nearest.fleet_index < input->fleet_count)
                        target = input->fleets[nearest.fleet_index].token;
                    else
                        target = 0;
                }
                found = 1;
            }
            order->target = target;
            order->flags = (StarsU16)((order->flags & 0xf0ffU) |
                ((target_kind & 0x0fU) << 8));
        }
    }
}
