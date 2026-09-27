#include <stdio.h>
#include "core.h"

static StarsI16 desirability(void *context, const StarsI16 *environment,
    StarsI16 player)
{
    (void)context;
    return (StarsI16)(environment[0] + environment[1] + environment[2] + player);
}
int main(void)
{
    StarsPlanetOptValueInput input;
    input.player = 2; input.environment[0] = 10; input.environment[1] = 20; input.environment[2] = 30;
    input.target[0] = 15; input.target[1] = 18; input.target[2] = -1;
    input.terraform_min[0] = 0; input.terraform_min[1] = 19; input.terraform_min[2] = -1;
    input.terraform_max[0] = 14; input.terraform_max[1] = 40; input.terraform_max[2] = 40;
    input.can_terraform = 1U;
    if (stars_pct_planet_opt_value(&input, 0, desirability, 0) != 65) return 1;
    input.can_terraform = 0U;
    if (stars_pct_planet_opt_value(&input, 55, desirability, 0) != 55) return 2;
    puts("PLANET_OPT_VALUE_SOURCE=PASS");
    return 0;
}
