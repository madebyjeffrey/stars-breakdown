#include "core.h"

/* Genuine C reconstruction of AutoFleetOrder, NE 11:23C8-2544. */
void stars_auto_fleet_order(StarsAutoFleetOrderInput *input)
{
    StarsI16 index;
    StarsI16 stat;
    StarsI32 mine;
    StarsI32 other_mine;
    StarsU16 join;
    StarsAutoFleet *other;

    if (!input || !input->fleet || !input->planet || !input->fleet->order)
        return;
    mine = input->mine_value ?
        input->mine_value(input->context, input->fleet) : 0L;
    stat = input->race_stat ?
        input->race_stat(input->context, input->fleet->player, 0x000e) : 0;
    if (!((input->planet->player == -1) ||
        (stat == 8 && input->planet->player == input->fleet->player)) ||
        mine == 0L) {
        input->fleet->plan_index = 0;
        return;
    }
    join = 0;
    other = (StarsAutoFleet *)0;
    if (input->planet->player == -1 && input->other_fleets) {
        for (index = 0; index < (StarsI16)input->other_fleet_count; ++index) {
            other = &input->other_fleets[index];
            if (input->fleet->player <= other->player &&
                (other->flags & 0x0400U) == 0U && other != input->fleet) {
                if (input->fleet->player < other->player)
                    break;
                if (other->point.x == input->fleet->point.x &&
                    other->point.y == input->fleet->point.y) {
                    other_mine = input->mine_value ?
                        input->mine_value(input->context, other) : 0L;
                    if (other_mine > 0L && other_mine < 4000L) {
                        join = 1;
                        break;
                    }
                }
            }
        }
    }
    if (join) {
        input->fleet->order->flags = (StarsU16)(
            (input->fleet->order->flags & 0xfff0U) | 4U);
        input->fleet->order->flags = (StarsU16)(
            (input->fleet->order->flags & 0xf0ffU) | 0x0200U);
        input->fleet->order->target_token = other->token;
    } else {
        input->fleet->order->flags = (StarsU16)(
            (input->fleet->order->flags & 0xfff0U) | 3U);
    }
    input->fleet->plan_index = 0;
}
