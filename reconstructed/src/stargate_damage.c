#include "core.h"

/* NE 11:152E-1899. Check a stargate jump and calculate the percentage of
 * damage caused by excess distance and ship weight.  The original uses
 * signed 32-bit fixed-point products; keeping each intermediate as long
 * preserves the same truncating divisions on the target compiler. */
StarsI16 stars_calc_stargate_damage(const StarsStargateDamageInput *input,
    StarsI16 source, StarsI16 destination, StarsI16 distance,
    StarsI16 weight, StarsI16 *damage)
{
    StarsStargateArmor source_armor;
    StarsStargateArmor destination_armor;
    StarsI32 range;
    StarsI32 scale;
    StarsI32 factor;

    if (!input || !input->lookup || !damage)
        return 0;
    if (!input->lookup(input->context, source, &source_armor) ||
            !input->lookup(input->context, destination, &destination_armor))
        return 0;
    range = source_armor.fuel_max;
    if (range == -1L)
        range = 8000L;
    if (range * 5L < (StarsI32)distance)
        return -1;
    if (source_armor.defense > 0 &&
            (StarsI32)source_armor.defense * 5L < (StarsI32)weight)
        return -2;
    if (destination_armor.defense > 0 &&
            (StarsI32)destination_armor.defense * 5L < (StarsI32)weight)
        return -2;
    scale = 10000L;
    if ((StarsI32)distance > range) {
        factor = (range * 5L - (StarsI32)distance) * 2500L;
        factor /= range;
        if (factor < 1L) {
            *damage = 100;
            return 0;
        }
        scale = scale * factor / 10000L;
    }
    if (source_armor.defense > 0 &&
            (StarsI32)source_armor.defense < (StarsI32)weight) {
        factor = ((StarsI32)source_armor.defense * 5L -
            (StarsI32)weight) * 2500L;
        factor /= source_armor.defense;
        if (factor < 1L) {
            *damage = 100;
            return 0;
        }
        scale = scale * factor / 10000L;
    }
    if (destination_armor.defense > 0 &&
            (StarsI32)destination_armor.defense < (StarsI32)weight) {
        factor = ((StarsI32)destination_armor.defense * 5L -
            (StarsI32)weight) * 2500L;
        factor /= destination_armor.defense;
        if (factor < 1L) {
            *damage = 100;
            return 0;
        }
        scale = scale * factor / 10000L;
    }
    *damage = (StarsI16)((10000L - scale) / 100L);
    return 1;
}
