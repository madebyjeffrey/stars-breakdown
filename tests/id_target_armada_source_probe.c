#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace { StarsI16 moves, transfers, clears; } Trace;
static StarsI16 lookup(void *context, StarsU16 id, StarsArmadaTargetPlanet *p)
{ (void)context; p->id=id;p->owner=1;p->starbase=0;p->turn_seen=0;p->point.x=10;p->point.y=20;p->minerals=0;p->population=100;return 1; }
static StarsI16 move(void *context, StarsArmadaTargetFleet *f, StarsU16 k,
    StarsU16 id, StarsPoint p)
{ Trace*t=(Trace*)context; (void)f;(void)k;(void)id;(void)p;++t->moves;return 1; }
static void transfer(void *context, StarsU16 id, StarsI16 p, StarsI16 n)
{ Trace*t=(Trace*)context;(void)id;(void)p;(void)n;++t->transfers; }
static void clear_task(void *context, StarsArmadaTargetFleet *f, StarsI16 m)
{ Trace*t=(Trace*)context;(void)f;(void)m;++t->clears; }
static StarsI16 random_value(void *context, StarsI16 n)
{ (void)context; return n > 1 ? 1 : 0; }
int main(void)
{
    StarsArmadaTargetFleet fleet; StarsArmadaTargetPlanet planets[1];
    StarsIdTargetArmadaOps ops; Trace trace; int failures=0;
    memset(&fleet,0,sizeof fleet);memset(planets,0,sizeof planets);
    memset(&ops,0,sizeof ops);memset(&trace,0,sizeof trace);
    fleet.id=7;fleet.planet_id=2;fleet.ship_counts[2]=8;
    planets[0].id=9;planets[0].owner=2;
    ops.lookup_planet=lookup;ops.move=move;ops.random=random_value;
    ops.transfer_supply=transfer;ops.transfer_troopers=transfer;
    ops.clear_task=clear_task;ops.context=&trace;
    if(stars_id_target_armada(&fleet,1,0,0,planets,1,&ops)!=0||trace.moves!=1)
        ++failures;
    memset(&trace,0,sizeof trace);fleet.planet_id=2;fleet.ship_counts[2]=0;
    if(stars_id_target_armada(&fleet,1,0,0,planets,1,&ops)!=0||trace.clears!=1)
        ++failures;
    printf("ID_TARGET_ARMADA_CHECKS=2 FAILURES=%d RESULT=%s\n",failures,
        failures?"FAIL":"PASS");return failures!=0;
}
