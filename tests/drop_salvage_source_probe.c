#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsSalvageThing things[4]; StarsI16 count; StarsI16 randoms[6]; StarsI16 random_count; } Trace;
static StarsI16 random_value(void *c, StarsI16 limit){Trace*t=(Trace*)c;(void)limit;return t->randoms[t->random_count++];}
static StarsSalvageThing *new_thing(void *c, StarsI16 player, StarsI16 kind){Trace*t=(Trace*)c;StarsSalvageThing*p;(void)player;(void)kind;p=&t->things[t->count++];memset(p,0,sizeof *p);return p;}
int main(void)
{
    StarsDropSalvageInput input; StarsSalvageThing *head; StarsSalvageThing existing; StarsPoint plans[1]; Trace trace;
    memset(&input,0,sizeof input); memset(&trace,0,sizeof trace); memset(&existing,0,sizeof existing); plans[0].x=20; plans[0].y=20;
    head=(StarsSalvageThing*)0; input.thing=&head; input.player=2; input.point.x=4; input.point.y=5; input.plan_points=plans; input.plan_count=1; input.random=random_value; input.new_thing=new_thing; input.context=&trace;
    input.minerals[0]=30000UL; input.minerals[1]=1UL; stars_drop_salvage(&input);
    if (trace.count!=2 || head!=&trace.things[1] || trace.things[0].value!=3000U ||
        trace.things[0].minerals[0]!=30000UL || trace.things[1].minerals[0]!=0UL ||
        trace.things[1].minerals[1]!=1UL || trace.things[1].value!=1U ||
        trace.things[1].x!=4 || trace.things[1].y!=5) return 1;
    memset(&trace,0,sizeof trace); head=&existing; memset(&existing,0,sizeof existing); existing.value=100U; existing.minerals[0]=4UL;
    input.minerals[0]=6UL; input.minerals[1]=0UL; input.minerals[2]=0UL; stars_drop_salvage(&input);
    if (trace.count!=0 || existing.minerals[0]!=10UL || existing.value!=1U) return 2;
    memset(&trace,0,sizeof trace); head=(StarsSalvageThing*)0; input.point=plans[0]; input.minerals[0]=1UL; stars_drop_salvage(&input);
    if (trace.count!=0) return 3;
    puts("DROP_SALVAGE_SOURCE=PASS"); return 0;
}
