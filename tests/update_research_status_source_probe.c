#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 level_messages;
    StarsU16 unlock_messages;
    StarsU16 pool_messages;
    StarsI16 last_message;
    StarsI16 last_pool_player;
} Trace;

static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{
    (void)context;
    (void)stat;
    return player == 1 ? 1 : 2;
}

static StarsI16 race_grbit(void *context, StarsI16 player, StarsI16 stat)
{
    (void)context;
    (void)player;
    (void)stat;
    return 0;
}

static StarsU32 level_cost(void *context, StarsI16 tech, StarsI16 level,
    StarsI16 player)
{
    (void)context;
    (void)tech;
    (void)level;
    (void)player;
    return 100UL;
}

static StarsI16 lookup_part(void *context, StarsU16 group, StarsU16 part,
    StarsU8 requirements[6])
{
    StarsU16 i;
    (void)context;
    if (group != 1U || part > 0U)
        return 0;
    for (i = 0; i < 6U; ++i)
        requirements[i] = 9U;
    requirements[0] = 1U;
    return 1;
}

static void send_message(void *context, StarsI16 player, StarsI16 message,
    StarsI16 arg1, StarsI16 arg2, StarsI16 arg3, StarsI16 arg4)
{
    Trace *trace = (Trace *)context;
    (void)player;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    if (message == 0x50)
        ++trace->level_messages;
    else
        ++trace->unlock_messages;
    trace->last_message = message;
}

static void send_pool_message(void *context, StarsI16 player,
    StarsI16 message, StarsI16 tech, StarsI16 amount)
{
    Trace *trace = (Trace *)context;
    (void)message;
    (void)tech;
    (void)amount;
    ++trace->pool_messages;
    trace->last_pool_player = player;
}

int main(void)
{
    StarsResearchPlayer players[2];
    StarsResearchOps ops;
    StarsResearchInput input;
    Trace trace;

    memset(players, 0, sizeof players);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    players[0].id = 0;
    players[0].current_mode = 6U;
    players[0].research[0] = 300UL;
    players[1].id = 1;
    input.players = players;
    input.player_count = 2U;
    input.ops = &ops;
    input.context = &trace;
    ops.race_stat = race_stat;
    ops.race_grbit = race_grbit;
    ops.level_cost = level_cost;
    ops.lookup_part = lookup_part;
    ops.send_message = send_message;
    ops.send_pool_message = send_pool_message;
    stars_update_research_status(&input);
    if (players[0].technology_level[0] != 3U ||
            players[0].research[0] != 0UL || trace.level_messages != 3U ||
            trace.unlock_messages != 1U || trace.pool_messages != 0U)
        return 1;

    memset(&trace, 0, sizeof trace);
    memset(players, 0, sizeof players);
    players[0].id = 0;
    players[0].current_mode = 6U;
    players[0].research_last_year = 100UL;
    players[1].id = 1;
    input.use_pool = 1;
    stars_update_research_status(&input);
    if (trace.pool_messages != 1U || trace.last_pool_player != 1 ||
            players[0].technology_level[0] != 1U ||
            players[1].research[0] != 25UL)
        return 1;
    puts("UPDATE_RESEARCH_STATUS_SOURCE=PASS");
    return 0;
}
