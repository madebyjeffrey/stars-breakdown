#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI32 level_cost(void *context, StarsI16 tech, StarsI16 level,
        StarsI16 player)
{
    StarsU16 *flags;
    (void)context;
    (void)player;
    flags = (StarsU16 *)context;
    return (StarsI32)((tech + level) * 10 * ((*flags & 2U) ? 2 : 1));
}

int main(void)
{
    StarsCostDevelopingInput input;
    StarsU8 target[6] = {2, 1, 2, 4, 4, 7};
    StarsU32 base[6] = {1, 2, 3, 4, 5, 6};
    signed char current[6] = {0, 1, 2, 3, 4, 5};
    StarsU16 flags;
    input.target = target;
    input.base_costs = base;
    input.game_flags = 0;
    input.player = 3;
    input.level_cost = level_cost;
    flags = 0;
    input.context = &flags;
    memcpy(input.current, current, sizeof(current));
    if (stars_cost_of_developing_item(&input) != 319L ||
            memcmp(input.current, current, sizeof(current)) != 0)
        return 1;
    input.game_flags = 2U;
    flags = 2U;
    if (stars_cost_of_developing_item(&input) != 638L)
        return 1;
    target[0] = 0x1bU;
    if (stars_cost_of_developing_item(&input) != -1L ||
            memcmp(input.current, current, sizeof(current)) != 0)
        return 1;
    puts("COST_DEVELOPING_SOURCE=PASS");
    return 0;
}
