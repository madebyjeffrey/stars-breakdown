#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 formatted; StarsI16 count; StarsI16 selected; } Trace;
static void format(void *c,const StarsVcrMenuToken *t,StarsI16 loss,StarsU8 *s,StarsU16 n)
{Trace*x=(Trace*)c;(void)t;(void)loss;(void)s;(void)n;++x->formatted;}
static StarsI16 popup(void *c,StarsU16 h,StarsI16 x,StarsI16 y,StarsU16 n,const StarsU8 *s,StarsI16 sel)
{Trace*t=(Trace*)c;(void)h;(void)x;(void)y;(void)s;t->count=n;t->selected=sel;return n>1?1:0;}
int main(void)
{
    StarsPopupVcrMenuInput input; StarsVcrMenuToken tokens[3]; StarsVcrStepRecord steps[1]; Trace trace;
    memset(&input,0,sizeof input);memset(tokens,0,sizeof tokens);memset(steps,0,sizeof steps);memset(&trace,0,sizeof trace);
    tokens[0].battle_square=0x21;tokens[0].ship_count=2;tokens[1].battle_square=0x21;tokens[1].ship_count=1;tokens[2].battle_square=0x22;tokens[2].ship_count=1;
    steps[0].token_index=0;steps[0].ship_loss=1;input.tokens=tokens;input.steps=steps;input.token_count=3;input.step_count=1;input.current_step=0;input.focused_token=0;input.format=format;input.popup=popup;input.context=&trace;
    if(stars_popup_vcr_menu(4,10,20,0x21,&input)!=1||trace.formatted!=2||trace.count!=2||trace.selected!=0)return 1;
    if(stars_popup_vcr_menu(4,10,20,0x22,&input)!=2||trace.formatted!=3)return 2;
    puts("POPUP_VCR_MENU_SOURCE=PASS");return 0;
}
