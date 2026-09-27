#include <stdio.h>
#include <string.h>
#include "core.h"

int main(void)
{
    StarsCalcPlanetMaxPopInput input;
    memset(&input, 0, sizeof input);
    input.planet_owner = 2;
    input.player = 2;
    input.race_stat = 8;
    input.has_starbase = 1U;
    input.starbase_population = 12345L;
    if (stars_calc_planet_max_pop(&input) != 12345L) return 1;
    input.player = 1;
    if (stars_calc_planet_max_pop(&input) != 0L) return 2;
    input.player = 2;
    input.race_stat = 0;
    input.desirability = 4L;
    input.race_grbit = 0;
    if (stars_calc_planet_max_pop(&input) != 250L) return 3;
    input.race_stat = 9;
    input.desirability = 10L;
    input.race_grbit = 1;
    if (stars_calc_planet_max_pop(&input) != 1320L) return 4;
    puts("CALC_PLANET_MAX_POP_SOURCE=PASS");
    return 0;
}
