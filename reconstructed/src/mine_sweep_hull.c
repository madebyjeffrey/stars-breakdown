#include "core.h"

/* NE 17:2BFA-2D5E. Resolve each mine-sweeper component and accumulate its
 * power.  The callback represents FLookupPart plus the original fixed-point
 * hull/part calculation, while the traversal and ihuldef adjustment remain
 * explicit here. */
StarsI32 stars_mine_sweep_from_hull(const StarsMineSweepHullInput *input)
{
    StarsI32 total = 0;
    StarsU16 i;
    StarsU16 advanced;
    if (!input || !input->components || !input->component_power) return 0;
    advanced = input->hull_definition > 31U ? 1U : 0U;
    for (i = 0; i < input->component_count; ++i)
        if (input->components[i].type == 0x10U)
            total += input->component_power(input->context,
                input->components[i].encoded_value, advanced, i);
    return total < 1 ? 0 : total;
}
