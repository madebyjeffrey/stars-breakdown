#include <stdio.h>
#include "core.h"
typedef struct T { StarsU16 frames, selected, button_flags; StarsI16 bx, by, bh, pat_x, pat_h; } T;
static void fr(void*c,StarsU16 h,const StarsRect*r,StarsU16 b){T*t=(T*)c;(void)h;(void)r;(void)b;++t->frames;}
static void se(void*c,StarsU16 h,StarsU16 o){T*t=(T*)c;(void)h;t->selected=o;}
static void co(void*c,StarsU16 h,StarsU32 n){(void)c;(void)h;(void)n;}
static void ct(void*c,StarsU16 h,const StarsRect*r,const StarsU8*s){(void)c;(void)h;(void)r;(void)s;}
static void bu(void*c,StarsU16 h,const StarsRect*r,StarsU16 f){T*t=(T*)c;(void)h;t->bx=r->left;t->by=r->top;t->bh=r->bottom;t->button_flags=f;}
static void pb(void*c,StarsU16 h,StarsI16 x,StarsI16 y,StarsI16 w,StarsI16 z,StarsU32 r){T*t=(T*)c;(void)h;(void)y;(void)w;(void)r;t->pat_x=x;t->pat_h=z;}
int main(void){StarsDrawTileNCInput i;StarsRect r;T t;i.hdc=2;i.tile_flags=1U;i.y_top=10;i.dy_full=40;i.arial8_height=8;i.font_arial8=7;i.text_color=1;i.face_color=2;i.button_shadow_brush=3;i.title=(const StarsU8*)"T";i.rect=&r;i.draw_frame=fr;i.select_object=se;i.set_text_color=co;i.set_background_color=co;i.center_title=ct;i.draw_button=bu;i.pat_blt=pb;i.context=&t;t.frames=0;t.selected=0;t.button_flags=0;t.bx=0;t.by=0;t.bh=0;t.pat_x=0;t.pat_h=0;if(stars_draw_tile_nc(&i)!=0||r.left!=202||r.right!=392||r.top!=22||r.bottom!=21||t.frames!=2U||t.selected!=3U||t.button_flags!=0x71U||t.bx!=375||t.by!=11||t.bh!=22||t.pat_x!=374||t.pat_h!=10)return 1;puts("DRAW_TILE_NC_SOURCE=PASS");return 0;}
