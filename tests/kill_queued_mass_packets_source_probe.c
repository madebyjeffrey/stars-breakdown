#include <stdio.h>
#include "core.h"
typedef struct T { StarsU16 freed, refreshed, count; } T;
static void fr(void*c,StarsU32*r){T*t=(T*)c;++t->freed;*r=0;}
static void rf(void*c,StarsI16 p,const StarsU32*r,StarsU16 n){T*t=(T*)c;(void)p;(void)r;t->refreshed=1;t->count=n;}
int main(void){StarsKillQueuedMassPacketsInput i;StarsU32 q[3],keep1,keep2;StarsU16 n=3;T t;keep1=(1UL<<17)|(18UL<<10)|2UL;keep2=(2UL<<17)|(15UL<<10)|3UL;q[0]=(1UL<<17)|(14UL<<10)|1UL;q[1]=keep1;q[2]=keep2;t.freed=0;t.refreshed=0;t.count=0;i.planet_id=4;i.production_records=q;i.production_count=&n;i.selected_group=1;i.selected_planet_id=4;i.free_production=fr;i.refresh_production=rf;i.context=&t;stars_kill_queued_mass_packets(&i);if(n!=2U||q[0]!=keep1||q[1]!=keep2||t.freed!=0U||!t.refreshed||t.count!=2U)return 1;q[0]=(1UL<<17)|(14UL<<10);n=1;stars_kill_queued_mass_packets(&i);if(n!=0U||q[0]!=0U||t.freed!=1U)return 2;puts("KILL_QUEUED_MASS_PACKETS_SOURCE=PASS");return 0;}
