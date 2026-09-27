#include "core.h"

/* NE 1C:66E0-68C4. Sum the cost of advancing each technology to a target,
 * while leaving the player's current technology levels unchanged. */
StarsI32 stars_cost_of_developing_item(StarsCostDevelopingInput *input)
{
    signed char saved[6];
    StarsI32 total;
    StarsI32 cost;
    StarsU16 i;
    StarsI16 level;

    if (!input || !input->target || !input->base_costs ||
            !input->level_cost)
        return 0;
    for (i = 0; i < 6U; ++i)
        if (input->target[i] > 0x1aU)
            return -1;
    for (i = 0; i < 6U; ++i)
        saved[i] = input->current[i];
    total = 0;
    for (i = 0; i < 6U; ++i) {
        if (input->current[i] >= (signed char)input->target[i])
            continue;
        cost = -(StarsI32)input->base_costs[i];
        if ((input->game_flags & 2U) != 0U)
            cost *= 2;
        level = input->current[i];
        while (level < (StarsI16)input->target[i]) {
            cost += input->level_cost(input->context, (StarsI16)i,
                (StarsI16)(level + 1), input->player);
            ++level;
            input->current[i] = (signed char)level;
        }
        if (cost < 0)
            cost = 0;
        total += cost;
    }
    for (i = 0; i < 6U; ++i)
        input->current[i] = saved[i];
    return total;
}
