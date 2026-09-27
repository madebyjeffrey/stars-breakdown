#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct Trace { StarsI16 creates,changes,failures; } Trace;
static StarsI16 random_value(void *c,StarsI16 n){(void)c;return n>1?1:0;}
static StarsI16 part_available(void *c,StarsU16 p,StarsU16 i){(void)c;(void)p;(void)i;return 1;}
static StarsI16 create_shdef(void *c,StarsI16 s,StarsU16 h,StarsI16 n)
{Trace*t=(Trace*)c;(void)s;(void)h;(void)n;++t->creates;if(t->failures<2){++t->failures;return 0;}return 1;}
static void change_shdef(void *c,StarsI16 s,const StarsMacintiShdef*d)
{Trace*t=(Trace*)c;(void)s;(void)d;++t->changes;}
int main(void)
{
    StarsEnsureMacintiShdefsInput input;StarsMacintiShdef designs[16];Trace trace;int failures=0;
    memset(&input,0,sizeof input);memset(designs,0,sizeof designs);memset(&trace,0,sizeof trace);
    input.designs=designs;input.design_count=16;input.turn=40;input.advanced_player=1;
    input.tech[0]=6;input.tech[1]=12;input.tech[2]=9;input.tech[3]=15;input.tech[4]=12;input.tech[5]=4;
    input.random_value=random_value;input.part_available=part_available;input.create_shdef=create_shdef;input.change_shdef=change_shdef;input.context=&trace;
    designs[1].existing=0;designs[5].active=0x0200;designs[12].active=0x0200;designs[14].active=0x0200;designs[0].hull_id=3;
    stars_ensure_macinti_shdefs(&input);
    if(trace.creates<5||trace.changes<2)++failures;
    memset(&trace,0,sizeof trace);memset(input.tech,0,sizeof input.tech);designs[1].existing=1;designs[0].existing=1;designs[5].active=0;designs[14].active=0;
    stars_ensure_macinti_shdefs(&input);
    if(trace.creates!=0||trace.changes!=0)++failures;
    printf("ENSURE_MACINTI_SHDEFS_CHECKS=2 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");return failures!=0;
}
