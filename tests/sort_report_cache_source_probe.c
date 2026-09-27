#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI16 compare(void *context, StarsI16 left, StarsI16 right,
    StarsI16 column)
{
    const StarsSortReportCacheInput *input =
        (const StarsSortReportCacheInput *)context;
    (void)column;
    if (input->fleets[left].player < input->fleets[right].player) return -1;
    if (input->fleets[left].player > input->fleets[right].player) return 1;
    return 0;
}

static StarsI16 battles(void *context)
{
    (void)context;
    return 3;
}

int main(void)
{
    StarsReportPlanetRow planets[4];
    StarsReportFleetRow fleets[4];
    StarsSortReportCacheInput input;
    StarsSortReportCacheState state;
    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    planets[0].player = 1; planets[0].status = 7U;
    planets[1].player = 1; planets[1].status = 6U;
    planets[2].player = 2; planets[2].status = 7U;
    planets[3].player = 1; planets[3].status = 7U;
    input.report = 0; input.sort_column = 2; input.player = 1;
    input.planets = planets; input.planet_count = 4;
    stars_sort_report_cache(&input, &state);
    if (state.rows != 2 || state.indices[0] != 0 || state.indices[1] != 3)
        return 1;
    memset(&state, 0, sizeof state);
    fleets[0].player = 2; fleets[0].present = 1U;
    fleets[1].player = 1; fleets[1].present = 1U;
    fleets[2].player = 3; fleets[2].present = 1U;
    fleets[3].present = 0U;
    input.report = 2; input.fleets = fleets; input.fleet_count = 4;
    input.compare = compare;
    stars_sort_report_cache(&input, &state);
    if (state.rows != 2 || state.indices[0] != 0 || state.indices[1] != 2 ||
        state.battle_cached != 0U) return 2;
    memset(&state, 0, sizeof state);
    input.report = 3; input.battle_count = battles;
    stars_sort_report_cache(&input, &state);
    if (state.rows != 3 || state.indices[2] != 2) return 3;
    puts("SORT_REPORT_CACHE_SOURCE=PASS");
    return 0;
}
