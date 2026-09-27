#include <stdio.h>
#include <string.h>
#include "core.h"
static StarsI16 relation(void *c, StarsI16 a, StarsI16 b){(void)c;return (a==0&&b==1)||(a==1&&b==0)?2:0;}
static StarsI16 counted(void *c, StarsI16 p, StarsI16 d){(void)c;(void)p;(void)d;return 1;}
int main(void)
{
    StarsCplrBattleFleet fleets[2]; StarsCplrBattleInput input; StarsU16 attacks[3];
    StarsU16 players,spectators;
    memset(fleets,0,sizeof fleets); memset(&input,0,sizeof input); fleets[0].player=0; fleets[0].has_teeth=1;
    fleets[0].plan_attacker=1; fleets[0].plan_target=5; fleets[0].ships[0]=3;
    fleets[1].player=1; fleets[1].has_teeth=1; fleets[1].plan_attacker=1; fleets[1].plan_target=4; fleets[1].ships[0]=2;
    input.fleets=fleets; input.fleet_count=2; input.player_count=3; input.planet_owner=-1; input.attack_masks=attacks;
    input.player_mask=&players; input.spectator_mask=&spectators; input.relation=relation; input.counted_ship=counted;
    if (stars_cplr_battle(&input)!=2 || players!=3U || spectators!=0U ||
        attacks[0]!=2U || attacks[1]!=1U || !(fleets[0].flags&0x100U)) return 1;
    fleets[0].flags=0x400U; fleets[1].flags=0; if (stars_cplr_battle(&input)!=0 || players!=0U ||
        (fleets[1].flags&0x900U)!=0x900U || spectators!=0U) return 2;
    puts("CPLR_BATTLE_SOURCE=PASS"); return 0;
}
