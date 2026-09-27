#include <stdio.h>
#include "core.h"

static StarsI32 power(void *context, StarsU16 encoded, StarsU16 advanced,
    StarsU16 index)
{
    (void)context; (void)index;
    return (StarsI32)((encoded >> 8) + advanced);
}
int main(void)
{
    static const StarsMineSweepComponent c[] = {
        {0x10U, 0x0300U}, {0x20U, 0x0900U}, {0x10U, 0x0200U}
    };
    StarsMineSweepHullInput input;
    input.hull_definition = 32U; input.components = c; input.component_count = 3U;
    input.component_power = power; input.context = 0;
    if (stars_mine_sweep_from_hull(&input) != 7) return 1;
    input.hull_definition = 31U;
    if (stars_mine_sweep_from_hull(&input) != 5) return 2;
    puts("MINE_SWEEP_HULL_SOURCE=PASS");
    return 0;
}
