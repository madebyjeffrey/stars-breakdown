#include "core.h"

/* NE 17:2B26-2BFA. Sum each positive mine-sweeper count times the resolved
 * hull's sweep power, then retain the original non-negative result rule. */
StarsI32 stars_mine_sweep_from_fleet(const StarsMineSweepFleetInput *input)
{
    StarsI32 total = 0;
    StarsU16 i;
    if (!input || !input->power) return 0;
    for (i = 0; i < 16U; ++i)
        if (input->mine_sweep_counts[i] > 0)
            total += input->power(input->context, input->player, i) *
                input->mine_sweep_counts[i];
    return total < 1 ? 0 : total;
}
