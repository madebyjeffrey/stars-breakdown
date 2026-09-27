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

int main(void)
{
    StarsVisibleFinishPlayer players[2];
    StarsVisibleFinishInput input;
    memset(players, 0, sizeof players);
    players[1].fleet_count = 0x7005U;
    players[0].fleet_count = 0x3002U;
    players[1].fleet_flags[0] = 0x0200U;
    players[1].fleet_flags[1] = 0x0100U;
    players[1].fleet_flags[2] = 0x0400U;
    players[1].fleet_known[0] = 1UL;
    players[1].starbase_flags[0] = 0x0100U;
    players[1].starbase_known[0] = 1UL;
    memset(&input, 0, sizeof input);
    input.viewer = 0;
    input.race_stat = 2;
    input.player_count = 2U;
    input.players = players;
    stars_set_visible_pf_finish(&input);
    if (players[1].fleet_count != 0x1005U || players[0].fleet_count != 0x3002U ||
            players[1].fleet_flags[0] != 0x0307U ||
            players[1].fleet_flags[1] != 0x0107U ||
            players[1].fleet_flags[2] != 0x0400U ||
            players[1].starbase_flags[0] != 0x0107U ||
            players[1].shdef_count != 2U ||
            players[1].detail_flags != 0x0103U)
        return 1;
    puts("SET_VISIBLE_PF_FINISH_SOURCE=PASS");
    return 0;
}
