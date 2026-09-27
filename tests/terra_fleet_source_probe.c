#include <stdio.h>
#include <string.h>
#include "core.h"

int main(void)
{
    static const StarsTerraComponent parts[] = {
        {0x80U, 0x0507U}, {0x80U, 0x0306U}, {0x10U, 0x0707U},
        {0x80U, 0x0207U}
    };
    StarsTerraFleetInput input;
    memset(&input, 0, sizeof input);
    input.component_counts[0] = 3;
    input.designs[0].components = parts; input.designs[0].component_count = 4U;
    input.component_counts[1] = 2;
    input.designs[1].components = parts; input.designs[1].component_count = 1U;
    if (stars_terra_from_fleet(&input) != 31) return 1;
    input.component_counts[0] = -1;
    input.component_counts[1] = 0;
    if (stars_terra_from_fleet(&input) != 0) return 2;
    puts("TERRA_FLEET_SOURCE=PASS");
    return 0;
}
