#include <stdio.h>
#include "core.h"

static StarsI16 can_terraform(void *context, const StarsTerraformPlanet *planet,
        StarsI16 help, StarsI16 min[3], StarsI16 max[3], StarsI16 cost[3])
{
    (void)context; (void)planet; (void)help;
    min[0] = -1; max[0] = 60; cost[0] = 1;
    min[1] = 40; max[1] = -1; cost[1] = 1;
    min[2] = -1; max[2] = -1; cost[2] = 0;
    return 1;
}
static StarsI16 desirability(void *context, const StarsTerraformPlanet *planet,
        StarsI16 player)
{
    (void)context; (void)player;
    return (StarsI16)(planet->env[0] + planet->env[1]);
}
int main(void)
{
    StarsTerraformPlanet planet = {3, {50, 50, 50}};
    StarsBestTerraformInput input;
    StarsI16 current_player = 9;
    input.planet = &planet; input.current_player = &current_player;
    input.can_terraform = can_terraform; input.desirability = desirability;
    input.context = 0;
    if (stars_best_terraform(&input, 0) != 1 || current_player != 9 ||
            planet.env[0] != 50 || planet.env[1] != 50)
        return 1;
    planet.player = -1;
    if (stars_best_terraform(&input, 0) != 0 || current_player != 9)
        return 1;
    puts("BEST_TERRAFORM_SOURCE=PASS");
    return 0;
}
