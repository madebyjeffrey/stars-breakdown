#include <stdio.h>
#include <string.h>
#include "core.h"

int main(void)
{
    StarsPotentMacFleetInput input;
    StarsI16 equivalent;
    memset(&input, 0, sizeof input);
    input.potency_threshold = 9;
    input.ship_counts[2] = 2; input.ship_counts[5] = 2;
    input.ship_counts[8] = 2; input.designs[8].hull_type = 9U;
    if (!stars_potent_mac_war_fleet(&input, &equivalent) || equivalent != 10)
        return 1;
    input.ship_counts[8] = 1;
    if (stars_potent_mac_war_fleet(&input, &equivalent))
        return 2;
    input.ship_counts[8] = 2; input.designs[8].ai_controlled = 1U;
    if (stars_potent_mac_war_fleet(&input, &equivalent)) return 3;
    puts("POTENT_MAC_FLEET_SOURCE=PASS");
    return 0;
}
