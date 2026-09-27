#include "core.h"

/* Genuine C reconstruction of DoBattles, NE 1F:3A26-3C25. */
void stars_do_battles(StarsI16 post_movement, StarsDoBattlesInput *input)
{
    void *battle_tokens;
    void *cargo_weights;
    StarsU16 battle_token_ids[16];
    StarsU16 token_count;
    StarsU16 battle_count;
    StarsI16 index;
    StarsI16 battle_result;
    StarsDoBattlesFleet *fleet;

    if (!input)
        return;
    if (input->link_fleets)
        input->link_fleets(input->context, post_movement);
    battle_tokens = input->allocate ?
        input->allocate(input->context, 0x1d00U, 6U) : (void *)0;
    cargo_weights = input->allocate ?
        input->allocate(input->context, 0x0200U, 6U) : (void *)0;
    for (index = 0; index < (StarsI16)input->fleet_count; ++index) {
        fleet = &input->fleets[index];
        fleet->flags = (StarsU16)(fleet->flags & 0xefffU);
        if ((fleet->flags & 0x0800U) == 0U &&
            (fleet->flags & 0x0400U) == 0U && fleet->has_next != 0U &&
            input->cplr_battle && input->cool_battle) {
            token_count = 0;
            battle_count = 0;
            battle_result = input->cplr_battle(input->context, fleet,
                battle_token_ids, &token_count, &battle_count);
            if (battle_result != -1 && battle_result != 0)
                input->cool_battle(input->context, fleet, battle_result,
                    battle_token_ids, token_count, battle_count);
        }
    }
    if (input->free_memory) {
        input->free_memory(input->context, cargo_weights, 6U);
        input->free_memory(input->context, battle_tokens, 6U);
    }
    battle_tokens = (void *)0;
    cargo_weights = (void *)0;
    if (input->battle_table) {
        ((StarsU8 *)input->battle_table)[0] = 0xffU;
        ((StarsU8 *)input->battle_table)[1] = 0xffU;
        if (input->free_memory)
            input->free_memory(input->context, input->battle_table, 0x0bU);
        input->battle_table = (void *)0;
    }
    if (input->battle_current) {
        input->battle_current[0] = 0xffU;
        input->battle_current[1] = 0xffU;
    }
    if (input->do_bombing)
        input->do_bombing(input->context);
}
