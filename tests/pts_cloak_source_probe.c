#include <stdio.h>
#include "core.h"

static StarsI16 part_points(void *context, StarsU16 group, StarsU16 subtype)
{
    (void)context;
    if (group != 0x800U || subtype != 3U)
        return -1;
    return 17;
}

int main(void)
{
    StarsCptsCloakInput input;
    input.part_points = part_points;
    input.context = 0;
    input.group = 1U;
    input.packed_count_type = 0x0108U;
    if (stars_cpts_cloak_from_lphs(&input) != 20)
        return 1;
    input.group = 4U;
    input.packed_count_type = 0x0206U;
    if (stars_cpts_cloak_from_lphs(&input) != 40)
        return 1;
    input.group = 0x800U;
    input.packed_count_type = 0x0203U;
    if (stars_cpts_cloak_from_lphs(&input) != 34)
        return 1;
    input.group = 0x1000U;
    input.packed_count_type = 0x0104U;
    if (stars_cpts_cloak_from_lphs(&input) != 20)
        return 1;
    input.group = 2U;
    input.packed_count_type = 0x0105U;
    if (stars_cpts_cloak_from_lphs(&input) != 0)
        return 1;
    input.group = 1U;
    input.packed_count_type = 0x0008U;
    if (stars_cpts_cloak_from_lphs(&input) != 0)
        return 1;
    puts("CPTS_CLOAK_SOURCE=PASS");
    return 0;
}
