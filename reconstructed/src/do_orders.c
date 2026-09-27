#include "core.h"

/* Genuine C reconstruction of DoOrders, NE 23:179A-18F4. */
void stars_do_orders(StarsI16 post_movement, StarsDoOrdersInput *input)
{
    StarsI16 index;
    StarsU16 battle_id;

    if (!input || !input->planets || input->planet_count < 0)
        return;
    for (index = 0; index < input->planet_count; ++index) {
        input->planets[index].flags = (StarsU16)(
            (input->planets[index].flags & 0xdfffU) |
            (input->planets[index].player != -1 ? 0x2000U : 0U));
    }
    if (post_movement != 0) {
        battle_id = (StarsU16)(((input->game_turn & 0x000fU) << 8) | 1U);
        if (input->set_battle_id)
            input->set_battle_id(input->context, battle_id);
        if (input->do_battles)
            input->do_battles(input->context, post_movement);
    }
    if (input->do_thing_interactions)
        input->do_thing_interactions(input->context, post_movement);
    if (post_movement != 0) {
        for (index = 0; index < input->planet_count; ++index)
            input->planets[index].turn = 0;
    }
    if (input->satisfy_orders)
        input->satisfy_orders(input->context,
            (StarsI16)(post_movement == 0 ? 1 : 3));
    if (input->drop_colonists)
        input->drop_colonists(input->context, 0);
    if (input->update_research)
        input->update_research(input->context, 0);
    if (input->satisfy_orders)
        input->satisfy_orders(input->context,
            (StarsI16)(post_movement == 0 ? 2 : 4));
    if (post_movement == 0 && input->transfer_to_others)
        input->transfer_to_others(input->context, 0);
}
