#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct Trace { StarsI16 moves, transfers; } Trace;
static StarsI32 resources(void *c,const StarsMacFreighterPlanet *p)
{ (void)c;return p->minerals[0]+p->minerals[1]+p->minerals[2]; }
static void transfer(void *c,StarsU16 id,StarsI16 p,StarsI16 m,StarsI32 n)
{ Trace*t=(Trace*)c;(void)id;(void)p;(void)m;(void)n;++t->transfers; }
static StarsI16 move(void *c,StarsMacFreighterFleet *f,StarsU16 k,StarsU16 id,StarsPoint p)
{ Trace*t=(Trace*)c;(void)f;(void)k;(void)id;(void)p;++t->moves;return 1; }
int main(void)
{
    StarsMacFreighterFleet fleet;StarsMacFreighterPlanet planets[2];
    StarsIdTargetMacFreighterOps ops;Trace trace;int failures=0;
    memset(&fleet,0,sizeof fleet);memset(planets,0,sizeof planets);memset(&ops,0,sizeof ops);memset(&trace,0,sizeof trace);
    fleet.id=1;fleet.planet_id=1;fleet.fleet_capacity=500;fleet.point.x=0;fleet.point.y=0;
    planets[0].id=1;planets[0].owner=1;planets[0].capacity_percent=40;planets[0].minerals[2]=2000;
    planets[1].id=2;planets[1].owner=2;planets[1].point.x=10;planets[1].point.y=0;planets[1].minerals[0]=1;planets[1].minerals[1]=1;planets[1].minerals[2]=1;planets[1].capacity_percent=30;
    ops.resources=resources;ops.transfer_supply=transfer;ops.move=move;ops.context=&trace;
    if(stars_id_target_mac_freighter(&fleet,1,20,planets,2,&ops)!=2||trace.moves!=1||trace.transfers!=1)++failures;
    memset(&trace,0,sizeof trace);fleet.planet_id=-1;
    if(stars_id_target_mac_freighter(&fleet,1,20,planets,1,&ops)!=1||trace.moves!=1)++failures;
    printf("ID_TARGET_MAC_FREIGHTER_CHECKS=2 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");
    return failures!=0;
}
