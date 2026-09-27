#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI32 power(void *context, StarsI16 player, StarsU16 slot)
{
    (void)context;
    return (StarsI32)(player + slot + 1);
}

int main(void)
{
    StarsMineSweepFleetInput input;
    memset(&input, 0, sizeof input);
    input.player = 2; input.power = power; input.context = 0;
    input.mine_sweep_counts[0] = 3; input.mine_sweep_counts[1] = -1;
    input.mine_sweep_counts[4] = 2;
    if (stars_mine_sweep_from_fleet(&input) != 23) return 1;
    input.mine_sweep_counts[0] = 0; input.mine_sweep_counts[4] = 0;
    if (stars_mine_sweep_from_fleet(&input) != 0) return 2;
    puts("MINE_SWEEP_SOURCE=PASS");
    return 0;
}
