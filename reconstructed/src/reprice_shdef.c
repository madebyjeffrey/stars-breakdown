#include "common.h"
#include <string.h>

static StarsU16 shdef_word(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU16)(bytes[offset] |
        ((StarsU16)bytes[offset + 1U] << 8));
}

static void shdef_put_word(StarsU8 *bytes, StarsU16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsI16 shdef_add_part(const StarsShdefRepriceInput *input,
    StarsU16 group, StarsU16 item, StarsU16 quantity,
    StarsU32 totals[4], StarsU16 *flags)
{
    StarsLookupPartInput lookup;
    StarsTruePart part;
    StarsTruePartCostInput cost_input;
    const StarsU8 *record;
    StarsU16 costs[4];
    StarsU16 index;

    memset(&lookup, 0, sizeof lookup);
    lookup.group = group;
    lookup.item = item;
    lookup.player = -1;
    if (stars_lookup_part(&lookup) < 0)
        return 0;
    record = stars_lookup_part_record(&lookup, input->part_segment,
        input->part_segment_size);
    if (!record)
        return 0;
    memset(&part, 0, sizeof part);
    part.group = group;
    part.subtype = item;
    for (index = 0; index < 6U; ++index)
        part.technology[index] = (signed char)record[2U + index];
    for (index = 0; index < 4U; ++index)
        part.base_costs[index] = shdef_word(record,
            (StarsU16)(0x2aU + index * 2U));
    memset(&cost_input, 0, sizeof cost_input);
    cost_input.player_technology = input->player_technology;
    cost_input.game_flags = flags;
    cost_input.race_stat = input->race_stat;
    cost_input.race_grbit = input->race_grbit;
    cost_input.context = input->context;
    stars_get_true_part_cost(&cost_input, input->player, &part, costs);
    for (index = 0; index < 4U; ++index)
        totals[index] += (StarsU32)quantity * costs[index];
    return 1;
}

StarsI16 stars_reprice_shdef_cost(const StarsShdefRepriceInput *input,
    const StarsU8 *source, StarsU16 bytes, StarsU8 *destination)
{
    StarsU8 copy[147];
    StarsU32 totals[4];
    StarsU16 flags;
    StarsU16 count;
    StarsU16 index;
    StarsU16 group;
    StarsU16 packed;
    StarsU16 offset;
    StarsU16 hull;

    if (!input || !source || !destination || bytes < 147U ||
            !input->part_segment || !input->player_technology ||
            !input->race_stat || !input->race_grbit)
        return 0;
    count = source[0x7aU];
    if (count > 16U)
        return 0;
    for (index = 0; index < 4U; ++index)
        totals[index] = 0;
    flags = input->game_flags ? *input->game_flags : 0;
    /* LphuldefFromId selects the separate starbase table at hull 32.
     * FLookupPart's normal hull group only accepts IDs 0..31. */
    hull = shdef_word(source, 0U);
    if (!shdef_add_part(input, hull < 32U ? 0x4000U : 0x0400U,
            hull < 32U ? hull : (StarsU16)(hull - 32U), 1U,
            totals, &flags))
        return 0;
    for (index = 0; index < count; ++index) {
        offset = (StarsU16)(0x3aU + index * 4U);
        group = shdef_word(source, offset);
        packed = shdef_word(source, (StarsU16)(offset + 2U));
        if (packed >> 8 && !shdef_add_part(input, group,
                (StarsU16)(packed & 0xffU), (StarsU16)(packed >> 8),
                totals, &flags))
            return 0;
    }
    memcpy(copy, source, sizeof copy);
    for (index = 0; index < 4U; ++index)
        shdef_put_word(copy, (StarsU16)(0x2aU + index * 2U),
            (StarsU16)totals[index]);
    memcpy(destination, copy, sizeof copy);
    if (input->game_flags)
        *input->game_flags = flags;
    return 1;
}

/* UpdateShdefCost (NE 8:47B0): the loader also needs physical fields and
 * the invalidated power cache, beyond the cost-only task-5 adapter. */
StarsI16 stars_initialize_shdef_cost(const StarsShdefRepriceInput *input,
    const StarsU8 *source, StarsU16 bytes, StarsU8 *destination)
{
    StarsU8 copy[147];
    StarsLookupPartInput lookup;
    const StarsU8 *part;
    StarsU16 hull, count, i, at, quantity, item, group, damage, weight;
    StarsU16 increment;
    StarsI16 weak_armor;
    if (!destination) return 0;
    if (!stars_reprice_shdef_cost(input, source, bytes, copy)) return 0;
    hull = shdef_word(copy, 0U);
    memset(&lookup, 0, sizeof lookup);
    lookup.player = -1;
    lookup.group = hull < 32U ? 0x4000U : 0x0400U;
    lookup.item = hull < 32U ? hull : (StarsU16)(hull - 32U);
    if (stars_lookup_part(&lookup) < 0) return 0;
    part = stars_lookup_part_record(&lookup, input->part_segment,
        input->part_segment_size);
    if (!part) return 0;
    damage = shdef_word(part, 56U);
    weight = shdef_word(part, 40U);
    weak_armor = (StarsI16)((copy[123] == 7U) &&
        input->race_grbit(input->context, input->player, 13) != 0);
    count = copy[122];
    for (i = 0U; i < count; ++i) {
        at = (StarsU16)(58U + i * 4U);
        quantity = copy[at + 3U];
        if (quantity == 0U) continue;
        group = shdef_word(copy, at);
        item = copy[at + 2U];
        lookup.group = group;
        lookup.item = item;
        if (stars_lookup_part(&lookup) < 0) return 0;
        part = stars_lookup_part_record(&lookup, input->part_segment,
            input->part_segment_size);
        if (!part) return 0;
        weight = (StarsU16)(weight + quantity * shdef_word(part, 40U));
        if (group == 4U && (item == 3U || item == 6U))
            damage = (StarsU16)(damage + quantity * 65U);
        else if (group == 8U) {
            increment = (StarsU16)(quantity * shdef_word(part, 52U));
            if (weak_armor)
                increment = (StarsU16)((increment >> 1) | (increment & 0x8000U));
            damage = (StarsU16)(damage + increment);
        } else if (group == 0x1000U && item == 4U)
            damage = (StarsU16)(damage + quantity * 50U);
    }
    shdef_put_word(copy, 40U, weight);
    shdef_put_word(copy, 56U, damage);
    memset(copy + 135U, 255, 4U);
    memcpy(destination, copy, sizeof copy);
    return 1;
}
