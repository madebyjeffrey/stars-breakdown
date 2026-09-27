/* Genuine C reconstruction of CBattleKills (NE 30:062e-07a8). */
#include "core.h"

StarsU32 stars_battle_kills(const StarsBattleKillToken *tokens,
    StarsU16 token_count, const StarsBattleKillGroup *groups,
    StarsU16 group_count, StarsI16 local_player, StarsI16 our_dead)
{
    StarsU16 group;
    StarsU16 entry;
    StarsU32 total;

    if (!tokens || !groups) return 0UL;
    total = 0UL;
    for (group = 0U; group < group_count; ++group) {
        const StarsBattleKillGroup *g = &groups[group];
        if (!g->entries) continue;
        for (entry = 0U; entry < g->entry_count; ++entry) {
            const StarsBattleKillEntry *k = &g->entries[entry];
            if (k->kills != 0U && k->token_index < token_count &&
                ((our_dead && tokens[k->token_index].player == local_player) ||
                 (!our_dead && tokens[k->token_index].player != local_player)))
                total += k->kills;
        }
    }
    return total;
}
