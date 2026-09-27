#include "core.h"

static StarsU16 scale_cost(StarsU16 cost, StarsI16 percent)
{
    StarsU32 product;
    product = (StarsU32)cost * (StarsU16)percent;
    return (StarsU16)(product / 100UL);
}

/* NE 0B:CD00-D291. Apply the player's technology and race modifiers to a
 * component's base mineral costs. */
void stars_get_true_part_cost(const StarsTruePartCostInput *input,
    StarsI16 player, const StarsTruePart *part, StarsU16 costs[4])
{
    StarsI16 progress;
    StarsI16 difference;
    StarsI16 i;
    StarsI16 stat;
    StarsI16 has_requirement;

    progress = 0;
    if (!input || !part || !costs || !input->player_technology ||
            !input->race_stat || !input->race_grbit)
        return;
    for (i = 0; i < 4; ++i)
        costs[i] = part->base_costs[i];
    if (player == -1)
        return;
    if ((part->group & 0x2000U) == 0U &&
            (((part->group & 0x8000U) == 0U || part->subtype < 9U) ||
             part->subtype > 0x0dU) &&
            ((part->group & 0x8000U) == 0U || part->subtype > 8U)) {
        progress = 100;
        for (i = 0; i < 6; ++i) {
            difference = (StarsI16)(input->player_technology[i] -
                part->technology[i]);
            if (part->technology[i] > 0 && progress > difference)
                progress = difference;
        }
        if (progress == 100) {
            progress = input->player_technology[0];
            for (i = 1; i < 6; ++i)
                if (input->player_technology[i] < progress)
                    progress = input->player_technology[i];
        }
        if (progress > 0) {
            if (progress > 19) progress = 19;
            if (input->race_grbit(input->context, player, 0x0c) == 0) {
                progress = (StarsI16)(progress * 4);
                if (progress > 75) progress = 75;
            } else {
                progress = (StarsI16)(progress * 5);
                if (progress > 80) progress = 80;
            }
            for (i = 0; i < 4; ++i) {
                if (costs[i] != 0) {
                    costs[i] = (StarsU16)(costs[i] -
                        scale_cost(costs[i], progress));
                    if (costs[i] == 0) costs[i] = 1;
                }
            }
        }
    }
    stat = input->race_stat(input->context, player, 0x0e);
    if (part->group == 0x200U && stat == 7) {
        for (i = 0; i < 4; ++i)
            costs[i] = (StarsU16)(costs[i] - (costs[i] >> 2));
    } else if ((part->group == 0x10U || part->group == 0x20U ||
            part->group == 0x40U) && stat == 2) {
        for (i = 0; i < 4; ++i)
            costs[i] = (StarsU16)(costs[i] - (costs[i] >> 2));
    } else if ((part->group == 0x10U || part->group == 0x20U ||
            part->group == 0x40U) && stat == 4) {
        for (i = 0; i < 4; ++i)
            costs[i] = (StarsU16)(costs[i] + (costs[i] >> 2));
    } else if (part->group == 0x2000U && stat == 3) {
        costs[3] = (StarsU16)(costs[3] / 2U);
    } else if (part->group == 1U &&
            input->race_grbit(input->context, player, 8) != 0) {
        for (i = 0; i < 4; ++i)
            costs[i] = (StarsU16)(costs[i] - (costs[i] >> 1));
    }
    if (progress < 1 && input->race_grbit(input->context, player, 0x0c) != 0 &&
            input->game_flags &&
            (*input->game_flags & 0x0008U) == 0U) {
        has_requirement = 0;
        for (i = 0; i < 6; ++i)
            if (part->technology[i] >= 1) has_requirement = 1;
        if (has_requirement) {
            *input->game_flags = (StarsU16)(*input->game_flags | 0x4000U);
            for (i = 0; i < 4; ++i)
                costs[i] = (StarsU16)(costs[i] << 1);
        } else {
            *input->game_flags = (StarsU16)(*input->game_flags &
                (StarsU16)~0x4000U);
        }
    } else if (input->game_flags) {
        *input->game_flags = (StarsU16)(*input->game_flags &
            (StarsU16)~0x4000U);
    }
}
