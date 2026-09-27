#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 stats,labels,fleets,texts,attacks; } Trace;
static void stats(void *c,const StarsDrawVcrToken*t,StarsI32*a,StarsU16*d,StarsI16*s){Trace*x=(Trace*)c;(void)t;*a=9;*d=8;*s=7;++x->stats;}
static void label(void *c,const StarsDrawVcrToken*t,StarsU8*b,StarsU16 n){Trace*x=(Trace*)c;(void)t;(void)b;(void)n;++x->labels;}
static void fleet(void *c,StarsU16 h,const StarsDrawVcrToken*t){Trace*x=(Trace*)c;(void)h;(void)t;++x->fleets;}
static void text(void *c,StarsU16 h,StarsI16 x,StarsI16 y,const StarsU8*s){Trace*t=(Trace*)c;(void)h;(void)x;(void)y;(void)s;++t->texts;}
static void attack(void *c,StarsU16 h){Trace*t=(Trace*)c;(void)h;++t->attacks;}
int main(void)
{
    StarsDrawVcrInput input; StarsDrawVcrToken tokens[3]; Trace trace;
    memset(&input,0,sizeof input);memset(tokens,0,sizeof tokens);memset(&trace,0,sizeof trace);
    tokens[0].ship_count=2;tokens[0].battle_square=0x11;tokens[1].ship_count=0;tokens[2].ship_count=1;tokens[2].battle_square=0x22;
    input.tokens=tokens;input.token_count=3;input.board_square_size=32;input.ops.get_stats=stats;input.ops.format_label=label;input.ops.draw_fleet=fleet;input.ops.draw_text=text;input.ops.draw_attack=attack;input.context=&trace;
    stars_draw_vcr(4,-1,-1,&input);
    if(trace.stats!=2||trace.labels!=2||trace.fleets!=2||trace.texts!=8||trace.attacks!=1)return 1;
    puts("DRAW_VCR_SOURCE=PASS");return 0;
}
