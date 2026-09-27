#include "core.h"

#define VCR_STAT_DISPLACEMENT_MASK 0x007fU
#define VCR_STAT_DISPLACEMENT_SPECIAL 499U
#define VCR_STAT_SPECIAL_VALUE 0xf980U
#define VCR_STAT_DAMAGE_MASK 0x1fffU
#define VCR_STAT_DAMAGE_SHIFT 13
#define VCR_STAT_PERCENT 100L
#define VCR_STAT_SHIELD_SCALE 50L

/* Genuine semantic reconstruction of GetVCRStats, NE 30:193E-1C61.
 * It folds every current battle-record entry for one token into the number
 * of lost ships and accumulated shield damage, derives remaining armor from
 * the hull displacement and current displacement, and returns the token's
 * remaining shields and ships.  The special displacement encoding is kept as
 * a named compatibility rule rather than exposed as an unexplained constant. */
void stars_get_vcr_stats(const StarsGetVcrStatsInput *input, StarsI16 token_id,
    StarsI32 *armor, StarsU16 *displacement, StarsI32 *shields,
    StarsI16 *ships)
{
    const StarsVcrStatToken *token;
    const StarsVcrStatRecord *record;
    StarsI32 design_value;
    StarsI32 shield_damage = 0;
    StarsI32 armor_value;
    StarsI16 lost_ships = 0;
    StarsI16 remaining_ships;
    StarsU16 current_displacement = 0xffffU;
    StarsU16 i;
    StarsI32 scaled;
    StarsI32 factor;

    if (armor) *armor = 0;
    if (displacement) *displacement = 0;
    if (shields) *shields = 0;
    if (ships) *ships = 0;
    if (!input || !input->tokens || token_id < 0 ||
            (StarsU16)token_id >= input->token_count) return;
    token = &input->tokens[token_id];
    design_value = input->design_values ? input->design_values[token_id] :
        token->hull_dp;
    for (i = 0; i < input->record_count; ++i) {
        record = &input->records[i];
        if (record->token_index != (StarsU16)token_id) continue;
        lost_ships = (StarsI16)(lost_ships + record->ship_loss);
        factor = (StarsI32)(record->packed_damage & VCR_STAT_DAMAGE_MASK);
        factor <<= ((record->packed_damage >> VCR_STAT_DAMAGE_SHIFT) << 1);
        shield_damage += factor;
        current_displacement = record->displacement;
    }
    if (current_displacement == 0xffffU) {
        current_displacement = token->displacement;
        if ((StarsU16)(current_displacement >> 7) > VCR_STAT_DISPLACEMENT_SPECIAL)
            current_displacement = (StarsU16)((current_displacement &
                VCR_STAT_DISPLACEMENT_MASK) | VCR_STAT_SPECIAL_VALUE);
    } else {
        remaining_ships = (StarsI16)(token->ship_count - lost_ships);
        scaled = (StarsI32)token->hull_dp * remaining_ships;
        factor = ((StarsI32)(current_displacement & 0xffff007fU) *
            remaining_ships) / VCR_STAT_PERCENT;
        if (factor < 1) factor = 1;
        armor_value = scaled - (((StarsI32)token->hull_dp *
            (StarsI32)(current_displacement >> 7) / 10L) * factor /
            VCR_STAT_SHIELD_SCALE);
        design_value = armor_value;
    }
    remaining_ships = (StarsI16)(token->ship_count - lost_ships);
    if (remaining_ships < 1) {
        remaining_ships = 0;
        design_value = 0;
        shield_damage = (StarsI32)token->shield * token->ship_count;
    }
    if (displacement) *displacement = current_displacement;
    if (ships) *ships = remaining_ships;
    if (armor) *armor = design_value;
    if (shields) *shields = shield_damage;
}
