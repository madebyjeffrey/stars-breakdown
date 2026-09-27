#include <stdio.h>
#include "core.h"
static StarsI16 nearest(void *c, StarsI16 x, StarsI16 y, StarsU16 mask,
        StarsNearestTarget *out)
{ (void)c;(void)x;(void)y;(void)mask;out->grobj_full=0;out->planet_id=22;return 1; }
int main(void)
{
    StarsFleetTargetOrder orders1[2]={{9,0x0200},{9,0x0201}};
    StarsFleetTargetOrder orders2[1]={{9,0x0202}};
    StarsFleetTargetRecord fleets[3];
    StarsFleetOrdersTargetInput input;
    fleets[0].token=9;fleets[0].x=10;fleets[0].y=20;
    fleets[0].orders=orders1;fleets[0].order_count=2;
    fleets[1].token=77;fleets[1].x=0;fleets[1].y=0;
    fleets[1].orders=orders2;fleets[1].order_count=1;
    fleets[2].token=88;fleets[2].orders=0;fleets[2].order_count=0;
    input.fleets=fleets;input.fleet_count=3;input.find_nearest=nearest;
    input.context=0;
    stars_fleet_orders_change_target(&input,&fleets[0]);
    if(orders1[0].target!=22||orders1[1].target!=22||orders2[0].target!=22||
            (orders1[0].flags&0x0f00U)!=0x0100U)
        return 1;
    puts("FLEET_ORDERS_TARGET_SOURCE=PASS");return 0;
}
