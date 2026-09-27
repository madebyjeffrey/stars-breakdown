#include "core.h"

static StarsI16 fleet_cloak_component_points(
    const StarsFleetCloakComponent *component,
    StarsFleetCloakPartPoints part_points, void *context)
{
    StarsU16 subtype;
    StarsU16 count;
    StarsI16 points;

    subtype = (StarsU16)(component->packed_count_type & 0x00ffU);
    count = (StarsU16)(component->packed_count_type >> 8);
    points = 0;
    if (count != 0U) {
        if (component->group == 1U && subtype == 8U) points = 20;
        else if (component->group == 2U && subtype == 6U) points = 40;
        else if (component->group == 4U && subtype == 4U) points = 70;
        else if (component->group == 4U && subtype == 6U) points = 20;
        else if (component->group == 8U && subtype == 7U) points = 50;
        else if (component->group == 8U && subtype == 9U) points = 40;
        else if (component->group == 0x10U && subtype == 0x12U)
            points = 20;
        else if (component->group == 0x80U && subtype == 6U)
            points = 60;
        else if (component->group == 0x80U && subtype == 7U)
            points = 50;
        else if (component->group == 0x800U && subtype < 5U &&
                part_points)
            points = part_points(context, component->group, subtype);
        else if (component->group == 0x1000U && subtype == 4U)
            points = 20;
        if (count > 1U)
            points = (StarsI16)(points * (StarsI16)count);
    }
    return points;
}

static StarsI16 cloak_percentage(StarsI32 ratio)
{
    StarsI32 value;

    if (ratio < 0) return 0;
    if (ratio <= 100L) return (StarsI16)(ratio >> 1);
    if (ratio <= 300L) return (StarsI16)((ratio - 100L) / 8L + 50L);
    if (ratio <= 612L) return (StarsI16)((ratio - 300L) / 24L + 75L);
    value = ratio - 612L;
    if (value <= 512L) return (StarsI16)((value >> 6) + 88L);
    if (value < 1000L) return (StarsI16)((value > 767L) + 96L);
    return 98;
}

/* NE 11:2D5E-3170.  Compute the fleet cloak percentage from the sixteen
 * normal ship-design slots.  The original uses 32-bit accumulators until a
 * conservative overflow guard trips, then promotes those accumulated values
 * to doubles; retaining that transition keeps the old x87 fallback visible
 * without replaying machine bytes. */
StarsI16 stars_pct_cloak_from_lpfl(const StarsPctCloakFromLpflInput *input)
{
    StarsU16 slot;
    StarsU16 component_index;
    StarsI16 stat;
    StarsI32 ratio;
    StarsU32 weight;
    StarsU32 weighted_int;
    StarsU32 total_int;
    double weighted_fp;
    double total_fp;
    int high_precision;

    if (!input || !input->ship_counts || !input->designs ||
            !input->race_stat)
        return 0;
    weighted_int = 0UL;
    total_int = 0UL;
    weighted_fp = 0.0;
    total_fp = 0.0;
    high_precision = 0;
    for (slot = 0; slot < 16U; ++slot) {
        const StarsFleetCloakDesign *design;
        StarsI32 design_points;
        StarsI32 count;

        count = input->ship_counts[slot];
        if (count <= 0) continue;
        design = &input->designs[slot];
        weight = (StarsU32)((StarsU32)count * design->hull_weight);
        design_points = 0;
        stat = input->race_stat(input->context, input->player, 0x0e);
        if (stat == 1) design_points = 300;
        for (component_index = 0;
                component_index < design->component_count;
                ++component_index)
            design_points += fleet_cloak_component_points(
                &design->components[component_index], input->part_points,
                input->context);
        if (design_points > 0L) {
            if (!high_precision &&
                    (design_points > 4000L || weight > 500000UL ||
                     weighted_int > 100000000UL ||
                     total_int > 50000000UL)) {
                high_precision = 1;
                weighted_fp = (double)(StarsI32)weighted_int;
                total_fp = (double)(StarsI32)total_int;
            }
            if (high_precision)
                weighted_fp += (double)design_points * (double)weight;
            else
                weighted_int += (StarsU32)((StarsU32)design_points * weight);
        }
        if (high_precision) total_fp += (double)weight;
        else total_int += weight;
    }
    if (!high_precision && weighted_int == 0UL) return 0;
    if (input->race_stat(input->context, input->player, 0x0e) != 1 &&
            !high_precision && input->mineral_weights) {
        for (slot = 0; slot < 4U; ++slot)
            total_int += input->mineral_weights[slot];
    }
    if (high_precision)
        ratio = (StarsI32)(weighted_fp / total_fp);
    else
        ratio = (StarsI32)weighted_int / (StarsI32)total_int;
    return cloak_percentage(ratio);
}
