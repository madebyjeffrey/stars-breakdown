#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct Trace { StarsI16 moves, clears, weak; } Trace;
static StarsI16 lookup(void *c, StarsU16 id, StarsArmadaTargetPlanet *p)
{ Trace*t=(Trace*)c;p->id=id;p->owner=t->weak?2:-1;p->starbase=0;p->turn_seen=0;p->point.x=4;p->point.y=8;p->minerals=0;p->population=0;return 1; }
static StarsI16 move(void *c, StarsArmadaTargetFleet *f, StarsU16 k, StarsU16 id, StarsPoint p)
{ Trace*t=(Trace*)c;(void)f;(void)k;(void)id;(void)p;++t->moves;return 1; }
static void clear_task(void *c, StarsArmadaTargetFleet *f, StarsI16 m)
{ Trace*t=(Trace*)c;(void)f;(void)m;++t->clears; }
static StarsI16 potency(void *c, const StarsArmadaTargetFleet *f, StarsI16 m, StarsI16 *e)
{ Trace*t=(Trace*)c;(void)f;(void)m;*e=t->weak?1:10;return 1; }
int main(void)
{
    StarsArmadaTargetFleet fleet;StarsArmadaTargetPlanet planets[1];
    StarsTargetMacArmadaOps ops;StarsU8 potency_limits[4]={4,2,6,3};
    Trace trace;int failures=0;
    memset(&fleet,0,sizeof fleet);memset(planets,0,sizeof planets);memset(&ops,0,sizeof ops);memset(&trace,0,sizeof trace);
    fleet.id=1;fleet.planet_id=2;fleet.ship_counts[8]=4;planets[0].id=9;planets[0].owner=2;
    ops.lookup_planet=lookup;ops.move=move;ops.clear_task=clear_task;ops.potency=potency;ops.context=&trace;
    stars_target_mac_armada(&fleet,1,20,0,potency_limits,planets,1,&ops);
    if(trace.moves!=1)++failures;
    memset(&trace,0,sizeof trace);trace.weak=1;fleet.ship_counts[8]=0;
    stars_target_mac_armada(&fleet,1,20,0,potency_limits,planets,1,&ops);
    if(trace.clears!=1)++failures;
    printf("TARGET_MAC_ARMADA_CHECKS=2 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");
    return failures!=0;
}
