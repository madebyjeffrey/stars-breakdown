#include "core.h"

/* NE 17:25D2-275C. Sum the mine-producing components represented by each
 * design in the fleet, scale by the number of ships, and cap the result at
 * the original 4000-unit limit.  The callback supplies FLookupPart's
 * resolved design-point value. */
StarsI32 stars_mine_from_fleet(const StarsMineFleetInput *input)
{
    StarsI32 total = 0;
    StarsU16 slot;
    if (!input || !input->design_counts || !input->designs ||
        !input->part_dp)
        return 0;
    for (slot = 0; slot < 16U; ++slot) {
        const StarsMineFleetDesign *design;
        StarsI32 design_total = 0;
        StarsU16 part_index;
        if (input->design_counts[slot] <= 0)
            continue;
        design = &input->designs[slot];
        if (!design->parts)
            continue;
        for (part_index = 0; part_index < design->part_count; ++part_index) {
            const StarsMineFleetPart *part = &design->parts[part_index];
            StarsU16 subtype = (StarsU16)(part->subtype & 0x00ffU);
            StarsU16 quantity = (StarsU16)(part->subtype >> 8);
            if (part->group == 0x0080U && subtype < 7U)
                design_total += (StarsI32)quantity *
                    input->part_dp(input->context, part->group, subtype);
        }
        total += design_total * input->design_counts[slot];
    }
    if (total > 3999L)
        total = 4000L;
    return total;
}
