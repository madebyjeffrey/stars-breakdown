/* Genuine C reconstruction of the hull weapon-component predicate. */
#include "core.h"
#include <string.h>

StarsI16 stars_hull_has_teeth(const StarsHullComponent *components,
    StarsU16 component_count)
{
    StarsU16 index;
    for (index = 0; index < component_count; ++index) {
        if ((components[index].attributes & 0x0030U) != 0U &&
            (components[index].packed_damage >> 8) != 0U)
            return 1;
    }
    return 0;
}

/* NE 31:4CA8.  Battle-range bytes hold two four-bit coordinates; the result
 * is their Chebyshev distance. */
StarsI16 stars_dz_from_brc_brc(StarsU8 first, StarsU8 second)
{
    StarsI16 dx;
    StarsI16 dy;
    dx = (StarsI16)(first & 0x0fU) - (StarsI16)(second & 0x0fU);
    dy = (StarsI16)(first >> 4) - (StarsI16)(second >> 4);
    if (dx < 0) dx = (StarsI16)-dx;
    if (dy < 0) dy = (StarsI16)-dy;
    return dx > dy ? dx : dy;
}

/* NE 31:44D4. Fisher-Yates shuffle of the 29-byte token records; the random
 * callback receives the number of remaining positions at each iteration. */
void stars_randomize_tok_order(StarsToken *tokens, StarsU16 count,
    StarsBattleRandom random_value, void *context)
{
    StarsU16 index;
    StarsI16 offset;
    StarsI16 chosen;
    StarsToken temporary;

    if (!tokens || !random_value)
        return;
    for (index = 0; index < count; ++index) {
        offset = random_value(context, (StarsI16)(count - index));
        chosen = (StarsI16)(index + (StarsU16)offset);
        if (chosen != (StarsI16)index) {
            memcpy(&temporary, &tokens[chosen], sizeof(temporary));
            memcpy(&tokens[chosen], &tokens[index], sizeof(temporary));
            memcpy(&tokens[index], &temporary, sizeof(temporary));
        }
    }
}

/* NE 31:5E34. Apply the tactic-specific give/take score transform, retaining
 * the original negative-give scaling and the lower bound of -1. */
StarsI32 stars_score_from_give_take_tactic(StarsI32 give, StarsI32 take,
    StarsI16 tactic)
{
    StarsI32 denominator;
    StarsI32 result;

    switch (tactic) {
    case 0:
    case 2:
        break;
    case 1:
    case 5:
        take = -give;
        break;
    case 3:
    case 4:
        if (give != 0) {
            denominator = take + 1L;
            if (denominator != 0L) {
                result = (-give * 100L) / denominator;
                take = result < 0L ? result : -1L;
            }
        }
        break;
    default:
        take = 0L;
        break;
    }
    return take;
}

/* NE 31:8B28.  Convert a speed into one rounded coordinate component.  The
 * remainder-specific tests are intentionally written out: the original
 * routine rounds ties according to the turn's phase rather than using a
 * generic floating-point round. */
StarsI16 stars_dxy_from_spd_round(StarsU16 speed, StarsI16 round)
{
    StarsU16 value;
    StarsU16 remainder;

    value = (StarsU16)((speed + 2U) / 4U);
    remainder = (StarsU16)(speed & 3U);
    if (remainder == 0U) {
        value = (StarsU16)(value + ((round & 1) == 0));
    } else if (remainder == 1U) {
        value = (StarsU16)(value + ((round & 3) != 2));
    } else if (remainder == 3U) {
        value = (StarsU16)(value + ((round & 3) == 0));
    }
    return (StarsI16)value;
}

/* NE 31:388E.  The original routine chooses one of two per-player far
 * design tables.  The callback keeps that table selection explicit while
 * returning the ordinary host pointer for the selected 0x93-byte record. */
const StarsU8 *stars_lpshdef_from_tok(const StarsToken *token,
    StarsLookupTokenHull lookup, void *context)
{
    StarsU16 hull_index;
    StarsU16 alternate;
    StarsU16 selector;

    if (!token || !lookup)
        return 0;
    if (token->raw[4] < 0x10U) {
        hull_index = token->raw[4];
        alternate = 0U;
    } else {
        hull_index = (StarsU16)(token->raw[4] - 0x10U);
        alternate = 1U;
    }
    selector = 0U;
    return lookup(context, token->raw[2], hull_index, alternate, &selector);
}

/* NE 31:388E's native DGROUP globals are rglpshdef at 0025:00FE and
 * rglpshdefSB at 0025:014C. Each contains sixteen four-byte far pointers;
 * a pointer is stored as offset word then segment word. This entry point
 * follows those recovered globals and delegates only segmented-address
 * mapping to the host/native resolver. */
const StarsU8 *stars_lpshdef_from_tok_globals(const StarsToken *token,
    const StarsU8 *dgroup, StarsResolveFarHull resolve, void *context)
{
    StarsU16 design;
    StarsU16 design_index;
    StarsU16 entry;
    StarsU16 table_offset;
    StarsU16 table_segment;
    StarsU16 hull_offset;
    StarsU16 table_base;
    StarsU16 player;

    if (!token || !dgroup || !resolve)
        return (const StarsU8 *)0;
    player = token->raw[2];
    design = token->raw[4];
    if (design < 0x10U) {
        table_base = 0x00feU;
        design_index = design;
    } else {
        table_base = 0x014cU;
        design_index = (StarsU16)(design - 0x10U);
    }
    entry = (StarsU16)(table_base + player * 4U);
    table_offset = (StarsU16)(dgroup[entry] |
        ((StarsU16)dgroup[(StarsU16)(entry + 1U)] << 8));
    table_segment = (StarsU16)(dgroup[(StarsU16)(entry + 2U)] |
        ((StarsU16)dgroup[(StarsU16)(entry + 3U)] << 8));
    hull_offset = (StarsU16)(table_offset + design_index * 0x0093U);
    return resolve(context, table_segment, hull_offset);
}

/* NE 31:3C16.  Battle tokens store current shield points at offset 0x11.
 * Regeneration is one tenth of the resolved maximum, but never crosses the
 * maximum; the callbacks keep the far-table resolution outside this module. */
void stars_regen_shield(StarsToken *token, StarsLookupTokenHull lookup,
    StarsTokenShieldPower shield_power, void *context)
{
    const StarsU8 *hull;
    StarsU32 maximum;
    StarsU32 candidate;

    hull = stars_lpshdef_from_tok(token, lookup, context);
    maximum = shield_power(context, hull, token->raw[2]);
    if ((token->raw[0x11] | token->raw[0x12]) != 0U) {
        candidate = maximum / 10U + (StarsU32)token->raw[0x11] +
            ((StarsU32)token->raw[0x12] << 8);
        if (candidate > maximum)
            candidate = maximum;
        token->raw[0x11] = (StarsU8)candidate;
        token->raw[0x12] = (StarsU8)(candidate >> 8);
    }
}

static StarsU16 battle_word(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU16)(bytes[offset] | ((StarsU16)bytes[offset + 1U] << 8));
}

/* NE 31:391E.  The original compares the packed beam/torpedo statistics in
 * three ordered stages.  The hull records are supplied by the same lookup
 * boundary as LpshdefFromTok; only the original byte offsets are interpreted
 * here. */
StarsI16 stars_f_can_kill_tok(const StarsToken *first, const StarsToken *second,
    StarsLookupTokenHull lookup, void *context)
{
    const StarsU8 *hull_first;
    const StarsU8 *hull_second;
    StarsU16 first_low;
    StarsU16 first_high;
    StarsU16 second_low;
    StarsU16 second_high;
    StarsU16 first_power;
    StarsU16 second_power;
    StarsU16 first_band;
    StarsU16 second_band;

    hull_first = stars_lpshdef_from_tok(first, lookup, context);
    hull_second = stars_lpshdef_from_tok(second, lookup, context);
    first_low = battle_word(hull_first, 0x87U);
    first_high = battle_word(hull_first, 0x89U);
    second_low = battle_word(hull_second, 0x87U);
    second_high = battle_word(hull_second, 0x89U);
    if ((StarsI16)second_high > (StarsI16)first_high ||
        ((StarsI16)second_high == (StarsI16)first_high &&
         second_low > first_low))
        return 0;

    first_power = (StarsU16)(first_high & 0x7fffU);
    second_power = (StarsU16)(second_high & 0x7fffU);
    if (first_power < second_power ||
        (first_power == second_power &&
         (first_low & 0xf000U) <= (second_low & 0xf000U)))
        return 1;

    first_band = (StarsU16)(first_low & 0xff00U);
    second_band = (StarsU16)(second_low & 0xff00U);
    if (first_band != second_band || first_power != second_power)
        return 0;
    return (StarsI16)(((second->raw[0x19] >> 8) & 0x0fU) <=
        ((first->raw[0x19] >> 8) & 0x0fU));
}

/* NE 31:1EC2.  Bomb-capable hull components are identified by exact group
 * words and a nonzero quantity in the packed component word. */
StarsI16 stars_hull_has_bombs(const StarsHullPart *parts, StarsU16 count)
{
    StarsU16 index;
    StarsU16 kind;
    StarsU16 packed;

    if (!parts)
        return 0;
    for (index = 0; index < count; ++index) {
        kind = parts[index].kind;
        packed = parts[index].packed_count_type;
        if ((kind == 0x0040U && (packed >> 8) != 0U) ||
            (kind == 0x0010U && (packed & 0xffU) == 0x12U &&
             (packed >> 8) != 0U) ||
            (kind == 0x1000U && (packed & 0xffU) == 1U &&
             (packed >> 8) != 0U))
            return 1;
    }
    return 0;
}

/* NE 31:1E16.  The fleet owner is encoded in overlap bits 9..12; only
 * nonempty designs are resolved through the player's hull table. */
StarsI16 stars_fleet_has_bombs(const StarsI16 *ship_counts, StarsU16 owner,
    StarsLookupBombHull lookup, void *context)
{
    StarsU16 index;
    StarsU16 count;
    const StarsHullPart *parts;

    if (!ship_counts || !lookup)
        return 0;
    for (index = 0; index < 16U; ++index) {
        if (ship_counts[index] != 0) {
            parts = lookup(context, owner, index, &count);
            if (parts && stars_hull_has_bombs(parts, count) != 0)
                return 1;
        }
    }
    return 0;
}

/* NE 31:1FBE.  A fleet qualifies only when a selected hull has teeth and its
 * associated hull-group byte is seven. */
StarsI16 stars_fleet_has_teeth(const StarsI16 *ship_counts, StarsU16 owner,
    StarsLookupFleetHull lookup, StarsGetHullType get_hull_group,
    void *context)
{
    StarsU16 index;
    StarsU16 count;
    const StarsHullComponent *components;

    if (!ship_counts || !lookup || !get_hull_group)
        return 0;
    for (index = 0; index < 16U; ++index) {
        if (ship_counts[index] != 0) {
            components = lookup(context, owner, index, &count);
            if (components && stars_hull_has_teeth(components, count) != 0 &&
                get_hull_group(context, owner, index) == 7U)
                return 1;
        }
    }
    return 0;
}

/* NE 30:48F6.  Button ranges are enabled independently, then focus is moved
 * to the first/last navigation button at the two sentinel positions. */
void stars_enable_vcr_buttons(StarsU16 dialog, StarsI16 step,
    StarsI16 last_step, const StarsEnableVcrButtonOps *ops, void *context)
{
    StarsU16 id;
    StarsU16 control;

    if (!ops || !ops->get_item || !ops->enable)
        return;
    for (id = 0xa1U; id < 0xa3U; ++id) {
        control = ops->get_item(context, dialog, id);
        ops->enable(context, control, (StarsU16)((StarsU16)step < 0x8000U));
    }
    for (id = 0xa3U; id < 0xa6U; ++id) {
        control = ops->get_item(context, dialog, id);
        ops->enable(context, control, (StarsU16)(step < last_step));
    }
    if (ops->set_focus) {
        if (step == -1)
            ops->set_focus(context, ops->get_item(context, dialog, 0xa3U));
        else if (step == last_step)
            ops->set_focus(context, ops->get_item(context, dialog, 1U));
    }
}

/* NE 34:4EBE. Select the mineral color, choose the dash or scaled-long text,
 * and pass the resulting string to the report's right-aligned text helper. */
void stars_draw_mineral_item(StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 mineral, StarsI32 value, const StarsDrawMineralItemOps *ops,
    void *context)
{
    const StarsU8 *text;
    StarsI16 length;

    if (!ops || !ops->right_text)
        return;
    if (ops->set_text_color) {
        if (mineral < 0)
            ops->set_text_color(context, hdc, 0UL);
        else if (ops->get_color)
            ops->set_text_color(context, hdc,
                ops->get_color(context, mineral));
    }
    if (value < 0) {
        text = ops->double_dash;
        length = text ? (StarsI16)strlen((const char *)text) : 0;
    } else if (ops->format_long) {
        text = ops->format_long(context, value, &length);
    } else {
        text = 0;
        length = 0;
    }
    ops->right_text(context, hdc, x, y, text, length, 0);
}
