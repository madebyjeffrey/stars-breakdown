#include <stdio.h>
#include "core.h"

static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{
    (void)player;
    (void)stat;
    return *(StarsI16 *)context;
}

static StarsI16 part_points(void *context, StarsU16 group, StarsU16 subtype)
{
    (void)context;
    if (group == 0x800U && subtype == 3U) return 17;
    return 0;
}

int main(void)
{
    StarsPctCloakFromLpflInput input;
    StarsFleetCloakDesign designs[16];
    StarsFleetCloakComponent components[2];
    StarsI16 counts[16];
    StarsU32 minerals[4];
    StarsI16 stat;
    StarsU16 i;

    for (i = 0; i < 16U; ++i) {
        counts[i] = 0;
        designs[i].hull_weight = 0UL;
        designs[i].components = 0;
        designs[i].component_count = 0U;
    }
    for (i = 0; i < 4U; ++i) minerals[i] = 0UL;
    components[0].group = 1U;
    components[0].packed_count_type = 0x0108U;
    components[1].group = 0x800U;
    components[1].packed_count_type = 0x0203U;
    designs[0].hull_weight = 1000UL;
    designs[0].components = components;
    designs[0].component_count = 2U;
    counts[0] = 1;
    stat = 0;
    input.player = 0;
    input.ship_counts = counts;
    input.designs = designs;
    input.mineral_weights = minerals;
    input.part_points = part_points;
    input.race_stat = race_stat;
    input.context = &stat;
    if (stars_pct_cloak_from_lpfl(&input) != 27) return 1;

    stat = 1;
    if (stars_pct_cloak_from_lpfl(&input) != 77) return 2;

    stat = 0;
    designs[0].hull_weight = 1000000UL;
    counts[0] = 100;
    if (stars_pct_cloak_from_lpfl(&input) != 27) return 3;

    designs[0].hull_weight = 1000UL;
    counts[0] = 1;
    minerals[0] = 1000UL;
    if (stars_pct_cloak_from_lpfl(&input) != 13) return 4;
    puts("PCT_CLOAK_SOURCE=PASS");
    return 0;
}
