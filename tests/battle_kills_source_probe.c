#include <stdio.h>
#include "core.h"
int main(void)
{
    StarsBattleKillToken tokens[3]; StarsBattleKillEntry a[3],b[2];
    StarsBattleKillGroup groups[2];
    tokens[0].player=2;tokens[1].player=4;tokens[2].player=2;
    a[0].token_index=0U;a[0].kills=5U;a[1].token_index=1U;a[1].kills=7U;a[2].token_index=2U;a[2].kills=0U;
    b[0].token_index=1U;b[0].kills=9U;b[1].token_index=2U;b[1].kills=11U;
    groups[0].entries=a;groups[0].entry_count=3U;groups[1].entries=b;groups[1].entry_count=2U;
    if(stars_battle_kills(tokens,3U,groups,2U,2,1)!=16UL)return 1;
    if(stars_battle_kills(tokens,3U,groups,2U,2,0)!=16UL)return 2;
    a[0].kills=0xffffU;b[1].kills=0xffffU;
    if(stars_battle_kills(tokens,3U,groups,2U,2,1)!=0x0001fffeUL)return 3;
    if(stars_battle_kills(tokens,0U,groups,2U,2,1)!=0UL)return 4;
    puts("BATTLE_KILLS_SOURCE=PASS");return 0;
}
