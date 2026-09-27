#include <stdio.h>
#include "core.h"
int main(void)
{
    StarsBattleTargetToken t[4];
    t[0].player=0;t[0].grobj=2U;t[0].primary_target_type=3U;t[0].active_flags=1U;
    t[1].player=1;t[1].grobj=1U;t[1].primary_target_type=7U;t[1].active_flags=1U;
    t[2].player=3;t[2].grobj=2U;t[2].primary_target_type=5U;t[2].active_flags=0U;
    t[3].player=4;t[3].grobj=2U;t[3].primary_target_type=4U;t[3].active_flags=1U;
    if(stars_does_primary_target_type_exist(0,1U,0x02U,t,4U)!=1)return 1;
    if(stars_does_primary_target_type_exist(0,2U,0x02U,t,4U)!=1)return 2;
    if(stars_does_primary_target_type_exist(0,3U,0x01U,t,4U)!=0)return 3;
    if(stars_does_primary_target_type_exist(0,4U,0x12U,t,4U)!=1)return 4;
    if(stars_does_primary_target_type_exist(0,5U,0x12U,t,4U)!=1)return 5;
    if(stars_does_primary_target_type_exist(0,6U,0x10U,t,4U)!=0)return 6;
    if(stars_does_primary_target_type_exist(0,0U,0xffffU,t,4U)!=0)return 7;
    puts("PRIMARY_TARGET_SOURCE=PASS");return 0;
}
