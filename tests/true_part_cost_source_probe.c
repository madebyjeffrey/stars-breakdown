#include <stdio.h>
#include "core.h"
static StarsI16 stat(void *c, StarsI16 p, StarsI16 id)
{ (void)p; return id==0x0e ? *(StarsI16 *)c : 0; }
static StarsI16 grbit(void *c, StarsI16 p, StarsI16 id)
{ (void)c; (void)p; return id==0x0c ? 1 : 0; }
int main(void)
{
    StarsTruePart part={0};
    StarsTruePartCostInput input;
    StarsU16 costs[4], flags=0;
    StarsI16 race=0;
    static const signed char technology[6]={4,4,4,4,4,4};
    static const signed char no_technology[6]={0,0,0,0,0,0};
    part.group=0x10; part.subtype=1;
    part.base_costs[0]=100;part.base_costs[1]=200;
    part.base_costs[2]=0;part.base_costs[3]=400;
    part.technology[0]=2;
    input.player_technology=technology;
    input.game_flags=&flags;input.race_stat=stat;input.race_grbit=grbit;
    input.context=&race;
    stars_get_true_part_cost(&input,3,&part,costs);
    if(costs[0]!=90||costs[1]!=180||costs[2]!=0||costs[3]!=360||
            (flags&0x4000U)!=0)return 1;
    part.group=0x2000;part.technology[0]=0;race=3;flags=0;
    stars_get_true_part_cost(&input,3,&part,costs);
    if(costs[3]!=200||(flags&0x4000U)!=0)return 1;
    input.player_technology=no_technology; part.group=1; part.technology[0]=1;
    flags=0; stars_get_true_part_cost(&input,3,&part,costs);
    if(costs[0]!=200||costs[1]!=400||costs[3]!=800||
            (flags&0x4000U)==0)return 1;
    flags=0x0008U; stars_get_true_part_cost(&input,3,&part,costs);
    if(costs[0]!=100||costs[1]!=200||costs[3]!=400||
            (flags&0x4000U)!=0||(flags&0x0008U)==0)return 1;
    puts("TRUE_PART_COST_SOURCE=PASS");return 0;
}
