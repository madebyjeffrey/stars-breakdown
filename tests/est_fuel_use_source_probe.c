#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsI16 distances; } Trace;
static StarsI16 warp(void *c, StarsI16 order, StarsI16 player){(void)c;(void)order;(void)player;return 2;}
static StarsI32 distance(void *c, StarsI16 order, StarsI16 warp_value, StarsI16 player, StarsI16 destination){(void)order;(void)warp_value;(void)player;(void)destination;++((Trace*)c)->distances;return 100;}
int main(void)
{
    StarsEstFuelFleet fleet; StarsEstFuelInput input; StarsU16 flags; Trace trace;
    memset(&fleet,0,sizeof fleet);memset(&input,0,sizeof input);memset(&trace,0,sizeof trace);
    fleet.player=2;fleet.destination=9;fleet.mineral_weight[0]=1000;fleet.mineral_weight[1]=500;fleet.cargo_weight=1500;
    fleet.slots[0].cargo_count=2;fleet.slots[0].ship_stat=50;fleet.slots[0].hull_fuel_weight=4;fleet.slots[0].fuel_used[2]=20;fleet.slots[0].engine_available=1;fleet.slots[0].engine_id=10;
    fleet.slots[1].cargo_count=1;fleet.slots[1].ship_stat=30;fleet.slots[1].hull_fuel_weight=2;fleet.slots[1].fuel_used[2]=40;fleet.slots[1].engine_available=1;fleet.slots[1].engine_id=3;
    flags=0;input.fleet=&fleet;input.order=1;input.warp=-1;input.travel=-1;input.race_grbit=1;input.game_flags=&flags;input.warp_from_order=warp;input.distance=distance;input.context=&trace;
    if(stars_est_fuel_use(&input)<=0 || trace.distances!=1 || (flags&0x40U)==0U)return 1;
    input.range_only=1;input.travel=100;input.warp=2;fleet.cargo_weight=2500;
    if(stars_est_fuel_use(&input)!=178571)return 2;
    memset(&fleet,0,sizeof fleet);
    fleet.mineral_weight[0]=100;fleet.slots[0].cargo_count=1;
    fleet.slots[0].ship_stat=100;fleet.slots[0].hull_fuel_weight=2;
    fleet.slots[0].fuel_used[10]=20;fleet.slots[0].engine_available=1;
    fleet.slots[0].engine_id=1;input.range_only=0;input.warp=10;
    input.travel=100;input.race_grbit=0;
    if(stars_est_fuel_use(&input)!=11)return 5;
    flags=0x40U;input.warp=12;
    if(stars_est_fuel_use(&input)!=0 || (flags&0x40U)!=0U)return 6;
    input.range_only=1;input.warp=2;
    fleet.slots[0].cargo_count=0;fleet.slots[1].cargo_count=0;
    if(stars_est_fuel_use(&input)!=1000000000L)return 3;
    memset(&fleet,0,sizeof fleet);input.range_only=0;input.travel=1000;input.warp=2;input.race_grbit=1;
    fleet.mineral_weight[0]=1000000;fleet.slots[0].cargo_count=1000;
    fleet.slots[0].ship_stat=1000;fleet.slots[0].hull_fuel_weight=0;
    fleet.slots[0].fuel_used[2]=1000;fleet.slots[0].engine_available=1;
    if(stars_est_fuel_use(&input)!=42500000)return 4;
    puts("EST_FUEL_USE_SOURCE=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\EFU\\EFUR.TXT", "w");
        if (!report)
            return 7;
        fputs("EST_FUEL_USE_CHECKS=6 FAILURES=0 RESULT=PASS\n", report);
        fclose(report);
    }
#endif
    return 0;
}
