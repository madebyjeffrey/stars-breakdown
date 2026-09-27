#include <stdio.h>
#include <string.h>
#include "core.h"
static StarsI16 part_dp(void *c, StarsU16 group, StarsU16 type){(void)c;(void)group;(void)type;return 10;}
static StarsU32 shield(void *c, StarsU16 n){(void)c;(void)n;return 0x00001234UL;}
int main(void)
{
    StarsWeaponsComponent components[6]; StarsCheckWeaponsToken token; StarsCheckWeaponsInput input;
    StarsI16 dampening; StarsU8 initiative[64];
    memset(components,0,sizeof components); memset(&token,0,sizeof token); memset(&input,0,sizeof input); memset(initiative,0,sizeof initiative);
    token.init_base=20; components[0].group=4; components[0].packed_count_type=0x0106;
    components[1].group=0x10; components[1].packed_count_type=0x0101;
    components[2].group=0x800; components[2].packed_count_type=0x020c;
    components[3].group=0x1000; components[3].packed_count_type=0x010a;
    components[4].group=0x800; components[4].packed_count_type=0x010e;
    components[5].group=0x800; components[5].packed_count_type=0x010f;
    input.token=&token; input.component_count=6; input.components=components; input.dampening_field=&dampening;
    input.initiative=initiative; input.part_dp=part_dp; input.shield_power=shield;
    stars_check_weapons(&input);
    if (token.pct_jam!=5U || token.pct_cap!=121U || token.pct_beam_def!=90U ||
        token.init_min!=30U || token.init_max!=30U || initiative[30]!=1U ||
        dampening!=1 || (token.token_flags&2U)==0U || token.dp_shield!=0x1234U) return 1;
    puts("CHECK_WEAPONS_SOURCE=PASS"); return 0;
}
