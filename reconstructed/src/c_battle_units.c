#include "core.h"

static StarsU16 battle_category_mask(StarsU16 category)
{
    if (category < 2U || category > 5U)
        return 0x0008U;
    if (category == 2U)
        return 0x0010U;
    if (category == 3U)
        return 0x0020U;
    if (category == 5U)
        return 0x0040U;
    return 0x0080U;
}

/* Genuine C reconstruction of CBattleUnits, NE 1E:0450-060F. */
StarsU32 stars_c_battle_units(const StarsBattleUnitsInput *input,
    StarsU16 battle_filter, StarsBattleUnitHullCategory hull_category,
    void *context)
{
    StarsU32 total;
    StarsU16 index;
    StarsU16 category;
    const StarsBattleUnitToken *token;

    if (!input || (!input->tokens && input->token_count != 0U))
        return 0UL;
    total = 0UL;
    for (index = 0; index < input->token_count; ++index) {
        token = &input->tokens[index];
        if ((token->owner == input->local_player &&
            (battle_filter & 1U) == 0U) ||
            (token->owner != input->local_player &&
            (battle_filter & 2U) == 0U))
            continue;
        if ((battle_filter & 4U) == 0U && token->design >= 16U)
            continue;
        if ((battle_filter & 0x00f8U) != 0x00f8U && token->design < 16U) {
            category = hull_category ?
                hull_category(context, token->owner, token->design) :
                token->hull_category;
            if ((battle_filter & battle_category_mask(category)) == 0U)
                continue;
        }
        total += token->battle_units;
    }
    return total;
}
