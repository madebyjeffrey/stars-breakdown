#include <stdio.h>
#include <string.h>
#include "core.h"

void stars_stream_open(StarsU8 *data, StarsU16 name, StarsI16 mode,
    const StarsStreamApi *api, void *context)
{ (void)data; (void)name; (void)mode; (void)api; (void)context; }
void stars_stream_write(StarsU8 *data, const StarsU8 *input, StarsU16 count,
    const StarsStreamApi *api, void *context)
{ (void)data; (void)input; (void)count; (void)api; (void)context; }
void stars_write_bof(StarsU8 *data, StarsI16 player, StarsI16 type,
    StarsI16 multi, StarsTickCount ticks, StarsWriteTransfer write,
    void *context)
{
    (void)data; (void)player; (void)type; (void)multi; (void)ticks;
    (void)write; (void)context;
}

static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{ (void)context; (void)player; (void)stat; return 2; }

int main(void)
{
    static const char colonists[6] = { 1, 2, 3, 4, 5, 6 };
    static const StarsU32 levels[3] = { 100UL, 200UL, 300UL };
    StarsTechLevelCostInput input;
    StarsTechLevelCostOps ops;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    input.colonist_counts = colonists;
    input.level_costs = levels;
    input.level_count = 3U;
    input.level = 1;
    input.tech = 2;
    input.player = 0;
    input.game_flags = 2U;
    ops.get_race_stat = race_stat;
    if (stars_get_tech_level_cost(&input, &ops, (void *)0) != 410)
        return 1;
    puts("GET_TECH_LEVEL_COST_SOURCE=PASS");
    return 0;
}
