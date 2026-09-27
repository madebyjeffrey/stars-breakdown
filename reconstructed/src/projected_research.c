#include "core.h"

static void add_research_fixed(StarsU16 *low, StarsI32 *high,
    StarsU32 value)
{
    StarsU16 old_low;
    StarsU16 carry;
    old_low = *low;
    *low = (StarsU16)(*low + (StarsU16)value);
    carry = *low < old_low ? 1U : 0U;
    *high += (StarsI32)((StarsI32)value >> 15) + carry;
}

/* NE 28:65AE-66DF. Temporarily project research at percentage, accumulating
 * owned-planet resource or production-schedule spending in the original
 * split low/high representation before restoring the player record. */
StarsI32 stars_projected_research_spending(StarsProjectedResearchInput *input)
{
    StarsU8 saved_percentage;
    StarsU16 low;
    StarsI32 high;
    StarsU16 i;
    StarsU32 value;

    if (!input || !input->player_percentage || !input->planets ||
        !input->resources_at_planet || !input->production_schedule)
        return 0;
    saved_percentage = *input->player_percentage;
    *input->player_percentage = (StarsU8)input->percentage;
    low = 0U;
    high = 0L;
    for (i = 0; i < input->planet_count; ++i) {
        if (input->planets[i].owner != input->player) continue;
        if (!input->planets[i].production_active)
            value = input->resources_at_planet(input->context, i,
                input->player);
        else value = input->production_schedule(input->context, i, -1);
        add_research_fixed(&low, &high, value);
    }
    *input->player_percentage = saved_percentage;
    return (StarsI32)(((StarsU32)high << 16) | low);
}
