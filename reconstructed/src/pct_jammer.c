#include "core.h"

static StarsI32 jammer_mul_percent(StarsI32 value, StarsI16 percent,
    StarsU16 count)
{
    while (count != 0U) {
        value = (value * (StarsI32)percent) / 100L;
        --count;
    }
    return value;
}

/* NE 1A:42E6-451D. Fold each hull component's jammer percentage into the
 * initial 10000 value, apply the large-hull adjustment, and clamp the final
 * result to the original 95-percent ceiling. */
StarsI16 stars_pct_jammer_from_hul(const StarsPctJammerFromHulInput *input)
{
    StarsI32 value = 10000L;
    StarsU16 i;
    StarsU16 type;
    StarsU16 count;
    StarsI16 percent;
    StarsI16 dp;
    StarsI16 result;

    if (!input || !input->components) return 0;
    for (i = 0; i < input->component_count; ++i) {
        const StarsJammerComponent *component = &input->components[i];
        type = (StarsU16)(component->packed_count_type & 0x00ffU);
        count = (StarsU16)(component->packed_count_type >> 8);
        if (component->group == 0x0800U) {
            if (type == 4U) percent = 90;
            else if (type < 8U || type > 11U) percent = 100;
            else if (!input->part_dp) percent = 100;
            else {
                dp = input->part_dp(input->context, component->group, type);
                percent = (StarsI16)(100 - dp);
            }
        } else if (component->group == 8U && type == 9U) percent = 80;
        else if (component->group == 0x0080U && type == 6U) percent = 70;
        else if (component->group == 4U && type == 6U) percent = 95;
        else percent = 100;
        if (percent < 100) value = jammer_mul_percent(value, percent, count);
    }
    if (value < 100L) value = 100L;
    result = (StarsI16)(100 - ((StarsI16)value + 50) / 100);
    if (input->hull_id > 0x20U)
        result = (StarsI16)(result - result / 4);
    if (result > 95) result = 95;
    if (result < 0) result = 0;
    return result;
}
