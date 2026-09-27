#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI16 lookup(void *context, StarsU16 group, StarsU16 subtype,
        StarsI16 *dp, StarsI16 *cost)
{
    (void)context;
    if (group != 0x2000U) return 0;
    if (subtype == 5U) { *dp=10; *cost=4; return 1; }
    if (subtype == 9U) { *dp=20; *cost=7; return 1; }
    if (subtype == 14U) { *dp=15; *cost=8; return 1; }
    if (subtype == 17U) { *dp=25; *cost=9; return 1; }
    return 0;
}
int main(void)
{
    StarsCanTerraformInput in;
    StarsCanTerraformPlanet planet;
    StarsI16 current_player=8, minimum[3], maximum[3], cost[3];
    memset(&in,0,sizeof(in)); memset(&planet,0,sizeof(planet));
    planet.player=2; planet.environment[0]=50; planet.environment[1]=50;
    planet.environment[2]=50; planet.original_environment[0]=50;
    planet.original_environment[1]=50; planet.original_environment[2]=50;
    in.planet=&planet; in.current_player=&current_player;
    in.race_center[0]=40; in.race_center[1]=50; in.race_center[2]=50;
    in.race_environment_enabled[0]=0; in.race_environment_enabled[1]=0;
    in.race_environment_enabled[2]=0; in.lookup_part=lookup;
    if (!stars_can_terraform_lppl(&in,minimum,maximum,cost,1) ||
            current_player!=8 || minimum[0]!=40 || maximum[0]!=-1 ||
            minimum[1]!=-1 || maximum[1]!=-1 || cost[0]!=7 ||
            cost[1]!=8 || cost[2]!=9) return 1;
    planet.environment[0]=35;
    if (!stars_can_terraform_lppl(&in,minimum,maximum,cost,0) ||
            minimum[0]!=-1 || maximum[0]!=70) return 2;
    in.race_environment_enabled[1]=-1;
    if (stars_can_terraform_lppl(&in,minimum,maximum,cost,1) != 1 ||
            minimum[1]!=-1 || maximum[1]!=-1) return 3;
    puts("CAN_TERRAFORM_SOURCE=PASS"); return 0;
}
