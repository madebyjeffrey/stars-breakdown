#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 lines,icons,animations; } Trace;
static void line(void *c,StarsU16 h,StarsI16 a,StarsI16 b,StarsI16 d,StarsI16 e,StarsI16 f,StarsI16 g)
{Trace*t=(Trace*)c;(void)h;(void)a;(void)b;(void)d;(void)e;(void)f;(void)g;++t->lines;}
static void icon(void *c,StarsU16 h,StarsI16 x,StarsI16 y,StarsI16 i)
{Trace*t=(Trace*)c;(void)h;(void)x;(void)y;(void)i;++t->icons;}
static void animate(void *c,StarsU16 h,StarsI16 a,StarsI16 b,StarsI16 d,StarsI16 e,StarsI16 f,StarsI16 g)
{Trace*t=(Trace*)c;(void)h;(void)a;(void)b;(void)d;(void)e;(void)f;(void)g;++t->animations;}
int main(void)
{
    StarsAnimateAttackInput input; StarsAnimateAttackToken tokens[2]; StarsAnimateAttackRecord records[2]; Trace trace;
    memset(&input,0,sizeof input);memset(tokens,0,sizeof tokens);memset(records,0,sizeof records);memset(&trace,0,sizeof trace);
    tokens[0].battle_square=0x11;tokens[1].battle_square=0x22;records[0].attacker_token=0;records[0].target_token=1;records[0].flags=5;records[1].attacker_token=0;records[1].target_token=1;records[1].flags=2;
    input.tokens=tokens;input.records=records;input.token_count=2;input.record_count=2;input.battle_square_size=32;input.animate=1;input.ops.draw_line=line;input.ops.draw_icon=icon;input.ops.animate_icon=animate;input.context=&trace;
    stars_animate_attack(7,&input);
    if(trace.lines!=2||trace.animations!=1||trace.icons!=2)return 1;
    input.animate=0;records[0].flags=0;trace.lines=trace.icons=trace.animations=0;stars_animate_attack(7,&input);
    if(trace.lines!=1||trace.animations!=0||trace.icons!=2)return 2;
    puts("ANIMATE_ATTACK_SOURCE=PASS");return 0;
}
