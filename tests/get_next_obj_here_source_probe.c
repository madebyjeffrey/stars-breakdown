#include <stdio.h>
#include <string.h>
#include "core.h"
int main(void)
{
    StarsNextObjectFleet fleets[3]; StarsNextObjectScan scan;
    StarsGetNextObjHereInput in;
    memset(fleets,0,sizeof fleets); memset(&scan,0,sizeof scan);
    memset(&in,0,sizeof in); fleets[0].valid=1;fleets[0].id=10;fleets[0].player=2;
    fleets[0].point.x=4;fleets[0].point.y=5;fleets[1].valid=1;fleets[1].id=11;
    fleets[1].player=1;fleets[1].point.x=4;fleets[1].point.y=5;
    in.scan=&scan;in.fleets=fleets;in.fleet_count=3;in.selected_kind=1;
    in.selected_point.x=4;in.selected_point.y=5;in.current_player=1;
    in.only_ours=1; if(stars_get_next_obj_here(&in)!=1||scan.object_kind!=2||
        scan.fleet_index!=1)return 1;
    memset(&scan,0,sizeof scan);in.selected_kind=2;in.selected_id=11;
    scan.point.x=4;scan.point.y=5;in.selected_full_flags=1;
    in.selected_planet_player=1;in.selected_planet_id=77;in.fleet_count=2;
    if(stars_get_next_obj_here(&in)!=1||scan.object_kind!=1||scan.planet_id!=77)
        return 2;
    memset(&scan,0,sizeof scan);in.selected_kind=2;in.selected_id=10;
    in.selected_point.x=99;in.selected_point.y=99;in.scan->point.x=88;
    in.selected_full_flags=0; if(stars_get_next_obj_here(&in)!=0)return 3;
    printf("GET_NEXT_OBJ_HERE_SOURCE_CHECKS=3 FAILURES=0 RESULT=PASS\n");
    return 0;
}
