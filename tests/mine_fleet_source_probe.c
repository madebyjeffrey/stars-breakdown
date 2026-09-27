#include <stdio.h>
#include "core.h"

static StarsI32 dp(void *context, StarsU16 group, StarsU16 subtype)
{
    (void)context;
    if (group != 0x80U) return -1000L;
    return (StarsI32)(10L + subtype);
}

int main(void)
{
    static const StarsMineFleetPart parts0[] = {
        {0x80U, (StarsU16)((2U << 8) | 1U)},
        {0x80U, (StarsU16)((3U << 8) | 6U)},
        {0x80U, (StarsU16)((9U << 8) | 7U)},
        {0x10U, (StarsU16)((4U << 8) | 2U)}
    };
    static const StarsMineFleetPart parts1[] = {
        {0x80U, (StarsU16)((1U << 8) | 0U)}
    };
    StarsMineFleetDesign designs[16];
    StarsI16 counts[16];
    StarsMineFleetInput input;
    StarsU16 i;
    for (i = 0; i < 16U; ++i) {
        designs[i].parts = 0;
        designs[i].part_count = 0U;
        counts[i] = 0;
    }
    designs[0].parts = parts0; designs[0].part_count = 4U; counts[0] = 2;
    designs[1].parts = parts1; designs[1].part_count = 1U; counts[1] = 3;
    input.player = 4; input.design_counts = counts; input.designs = designs;
    input.part_dp = dp; input.context = 0;
    if (stars_mine_from_fleet(&input) != 170L) return 1;
    counts[0] = 100; counts[1] = 100;
    if (stars_mine_from_fleet(&input) != 4000L) return 2;
    counts[0] = -1; counts[1] = 0;
    if (stars_mine_from_fleet(&input) != 0L) return 3;
    input.part_dp = 0;
    if (stars_mine_from_fleet(&input) != 0L) return 4;
    puts("MINE_FLEET_SOURCE=PASS");
    return 0;
}
