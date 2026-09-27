#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct Trace { StarsI16 creates, changes, failures; } Trace;
static StarsI16 random_design(void *context, StarsI16 limit)
{ (void)context; return limit > 1 ? 2 : 0; }
static StarsI16 create_shdef(void *context, StarsI16 slot, StarsU16 hull,
    StarsI16 name_id)
{ Trace*t=(Trace*)context;(void)slot;(void)hull;(void)name_id;++t->creates;if(t->failures<2){++t->failures;return 0;}return 1; }
static void change_shdef(void *context, StarsI16 slot,
    const StarsTurinDroneShdef *design)
{ Trace*t=(Trace*)context;(void)slot;(void)design;++t->changes; }
int main(void)
{
    StarsEnsureTurinDroneShdefsInput input;StarsTurinDroneShdef designs[16];
    Trace trace;int failures=0;
    memset(&input,0,sizeof input);memset(designs,0,sizeof designs);memset(&trace,0,sizeof trace);
    input.designs=designs;input.design_count=16;input.create_shdef=create_shdef;
    input.change_shdef=change_shdef;input.random_value=random_design;input.context=&trace;
    input.tech[1]=12;input.tech[2]=9;input.tech[3]=15;input.tech[4]=12;input.tech[5]=4;
    designs[0].existing=1;designs[1].existing=0;designs[2].existing=1;
    designs[4].active=0x0200;designs[8].active=0x0200;
    designs[15].active=0x0200;
    stars_ensure_turin_drone_shdefs(&input);
    if(trace.creates<5||trace.changes!=1)++failures;
    memset(&trace,0,sizeof trace);memset(input.tech,0,sizeof input.tech);
    designs[0].existing=1;designs[1].existing=1;designs[2].existing=1;
    stars_ensure_turin_drone_shdefs(&input);
    if(trace.creates!=0||trace.changes!=0)++failures;
    printf("ENSURE_TURIN_DRONE_SHDEFS_CHECKS=2 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");
    return failures!=0;
}
