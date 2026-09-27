#include <stdio.h>
#include <string.h>
#include "core.h"

int main(void)
{
    static const StarsU32 items[] = {
        (2UL << 17) | (3UL << 10) | 7UL,
        (2UL << 17) | (3UL << 10) | (1UL << 20) | 9UL,
        (1UL << 17) | (3UL << 10) | 4UL
    };
    StarsQueuedProductionPlanet planets[2];
    StarsQueuedProductionInput input;
    StarsI16 progress;
    memset(planets, 0, sizeof planets); memset(&input, 0, sizeof input);
    planets[0].player = 2; planets[0].build_enabled = 1U;
    planets[0].has_production = 1U; planets[0].space_dock = 1U;
    planets[0].items = items; planets[0].item_count = 3U;
    input.design = 3; input.current_player = 2; input.planets = planets;
    input.planet_count = 2U;
    if (stars_count_queued_production(&input, &progress) != 16 || !progress)
        return 1;
    input.space_docks_only = 1; planets[0].space_dock = 0U;
    if (stars_count_queued_production(&input, &progress) != 0 || progress)
        return 2;
    puts("QUEUED_PRODUCTION_SOURCE=PASS");
    return 0;
}
