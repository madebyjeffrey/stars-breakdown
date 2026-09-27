#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 regen; StarsI16 enables; } Trace;
static void regen(void *c, StarsVcrTokenState *t)
{Trace*x=(Trace*)c;++x->regen;t->shield=(StarsI16)(t->shield+2);}
static void enable(void *c, StarsI16 step, StarsI16 count)
{Trace*x=(Trace*)c;(void)step;(void)count;++x->enables;}
int main(void)
{
    StarsSetVcrBoardInput input; StarsVcrTokenState tokens[2],initial[2];
    StarsVcrStepRecord steps[2]; Trace trace;
    memset(&input,0,sizeof input); memset(tokens,0,sizeof tokens);
    memset(initial,0,sizeof initial); memset(steps,0,sizeof steps); memset(&trace,0,sizeof trace);
    initial[0].ship_count=4; initial[0].shield=5; initial[0].battle_square=2; initial[0].flags=8; initial[0].target=7;
    initial[1].ship_count=3; initial[1].shield=4; initial[1].battle_square=3;
    steps[0].token_index=0; steps[0].ship_loss=1; steps[0].shield_damage=2; steps[0].destination_square=4; steps[0].packed_flags=0x40;
    steps[1].token_index=1; steps[1].ship_loss=1; steps[1].round=1; steps[1].destination_square=5; steps[1].packed_flags=0x20;
    input.tokens=tokens; input.initial_tokens=initial; input.token_count=2; input.steps=steps; input.step_count=2;
    input.current_step=-1; input.regenerate_shield=regen; input.enable_buttons=enable; input.context=&trace;
    if(stars_set_vcr_board(0,&input)!=0||tokens[0].ship_count!=3||tokens[0].shield!=3||input.focus_square!=4)return 1;
    if(stars_set_vcr_board(1,&input)!=1||tokens[1].ship_count!=2||input.round!=1||trace.regen!=1)return 2;
    if(stars_set_vcr_board(-1,&input)!=-1||tokens[0].ship_count!=4||tokens[1].ship_count!=3||trace.enables!=3)return 3;
    puts("SET_VCR_BOARD_SOURCE=PASS");return 0;
}
