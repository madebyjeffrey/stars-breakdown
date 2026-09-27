#include <stdio.h>
#include "core.h"

static StarsI16 lookup(void *context, StarsI16 id, StarsStargateArmor *armor)
{
    (void)context;
    armor->fuel_max = id == 1 ? 100 : 80;
    armor->defense = id == 1 ? 40 : 30;
    return 1;
}
int main(void)
{
    StarsStargateDamageInput input;
    StarsI16 damage;
    input.lookup = lookup; input.context = 0;
    if (stars_calc_stargate_damage(&input, 1, 2, 100, 20, &damage) != 1 ||
            damage != 0)
        return 1;
    if (stars_calc_stargate_damage(&input, 1, 2, 501, 20, &damage) != -1)
        return 1;
    if (stars_calc_stargate_damage(&input, 1, 2, 100, 201, &damage) != -2)
        return 1;
    if (stars_calc_stargate_damage(&input, 1, 2, 400, 20, &damage) != 1 ||
            damage <= 0)
        return 1;
    puts("STARGATE_DAMAGE_SOURCE=PASS");
    return 0;
}
