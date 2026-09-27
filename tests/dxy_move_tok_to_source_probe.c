#include <stdio.h>
#include <string.h>
#include "core.h"
static StarsI16 range(void *c, StarsU16 mask, StarsU8 *preferred){(void)c;(void)mask;*preferred=0xffU;return 2;}
static StarsI32 score(void *c, StarsU16 coordinate, StarsI16 primary, StarsU16 mask){(void)c;(void)primary;(void)mask;return (coordinate==0x22U)?0L:10L;}
static StarsI16 same(void *c, StarsU16 coordinate, StarsI16 kind){(void)c;(void)coordinate;(void)kind;return 0;}
int main(void)
{
    StarsDxyMoveToken token; StarsDxyMoveInput input;
    memset(&token,0,sizeof token); memset(&input,0,sizeof input); token.battle_coordinate=0x00U;
    input.token=&token; input.speed=4; input.attack_mask=3; input.move_range=range; input.score_damage=score; input.same_square=same;
    if (stars_dxy_move_tok_to(&input)!=1 || token.battle_coordinate!=0x01U ||
        !(token.movement_flags&0x10U)) return 1;
    token.object_kind=1; token.battle_coordinate=0x33U;
    if (stars_dxy_move_tok_to(&input)!=1 || token.battle_coordinate!=0x33U) return 2;
    puts("DXY_MOVE_TOK_TO_SOURCE=PASS"); return 0;
}
