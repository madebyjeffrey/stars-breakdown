#include "core.h"

static StarsU16 check_target_word(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] | ((StarsU16)data[offset + 1U] << 8));
}

/* Genuine C reconstruction of CheckTarget, NE 31:212A-2349. */
void stars_check_target(StarsToken *token, const StarsCheckTargetFleet *fleet,
    const StarsCheckTargetInput *input)
{
    StarsI16 owner;
    StarsU8 *ship_definition;
    const StarsU8 *battle_plan;
    StarsU16 value;
    StarsU16 kind;

    if (!token || !fleet || !input || !input->ship_definition ||
        !input->battle_plan)
        return;
    owner = (StarsI16)((fleet->flags >> 9) & 0x0fU);
    ship_definition = input->ship_definition(input->context, owner,
        fleet->ship_design);
    if (!ship_definition)
        return;
    if (input->has_teeth &&
        input->has_teeth(input->context, ship_definition) != 0)
        kind = 3U;
    else if (input->has_bombs &&
        input->has_bombs(input->context, ship_definition) != 0)
        kind = 4U;
    else if (input->fuel_tanker &&
        input->fuel_tanker(input->context, ship_definition) != 0)
        kind = 6U;
    else if (input->max_stat &&
        input->max_stat(input->context, ship_definition, 2) != 0)
        kind = 7U;
    else
        kind = 5U;
    value = check_target_word(token->raw, 0x17U);
    value = (StarsU16)((value & 0x0fffU) | ((StarsU16)kind << 12));
    token->raw[0x17U] = (StarsU8)value;
    token->raw[0x18U] = (StarsU8)(value >> 8);

    battle_plan = input->battle_plan(input->context, owner, fleet->plan);
    if (!battle_plan)
        return;
    value = check_target_word(token->raw, 0x17U);
    value = (StarsU16)((value & 0xfff0U) |
        (check_target_word(battle_plan, 4U) & 0x000fU));
    token->raw[0x17U] = (StarsU8)value;
    token->raw[0x18U] = (StarsU8)(value >> 8);
    value = check_target_word(token->raw, 0x17U);
    value = (StarsU16)((value & 0xff0fU) |
        (((check_target_word(battle_plan, 4U) >> 4) & 0x000fU) << 4));
    token->raw[0x17U] = (StarsU8)value;
    token->raw[0x18U] = (StarsU8)(value >> 8);
    kind = (StarsU16)(value >> 12);
    if (kind == 3U) {
        value = check_target_word(token->raw, 0x17U);
        value = (StarsU16)((value & 0xf0ffU) |
            (((check_target_word(battle_plan, 0) >> 8) & 0x000fU) << 8));
    } else {
        value = (StarsU16)(check_target_word(token->raw, 0x17U) & 0xf0ffU);
    }
    token->raw[0x17U] = (StarsU8)value;
    token->raw[0x18U] = (StarsU8)(value >> 8);
    if (((value >> 8) & 0x0fU) == 0U) {
        value = check_target_word(token->raw, 0x1bU);
        value = (StarsU16)((value & 0xfc1fU) | 0x00e0U);
        token->raw[0x1bU] = (StarsU8)value;
        token->raw[0x1cU] = (StarsU8)(value >> 8);
    }
}
