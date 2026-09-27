#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsU16 font, brush, old_mode, mode_calls; StarsI32 maximum, gauge, displayed; StarsI16 current; char text[16]; } Trace;
static void so(void*c,StarsU16 h,StarsU16 f){Trace*t=(Trace*)c;(void)h;t->font=f;}
static StarsI32 dg(void*c,StarsU16 h,const StarsRect*r,StarsI16 n,StarsU16 b,StarsI32 m){Trace*t=(Trace*)c;(void)h;(void)r;(void)n;t->brush=b;t->maximum=m;return 12L;}
static StarsU16 bm(void*c,StarsU16 h,StarsU16 n){Trace*t=(Trace*)c;(void)h;t->mode_calls++;return n==1U?99U:88U;}
static StarsI16 fm(void*c,StarsI32 v,StarsU8*t,StarsU16 n){Trace*x=(Trace*)c;(void)n;x->displayed=v;strcpy((char*)t,"value");return 5;}
static void te(void*c,StarsU16 h,const StarsU8*t,StarsI16 n){(void)c;(void)h;(void)t;(void)n;}
static void ct(void*c,StarsU16 h,const StarsRect*r,const StarsU8*t,StarsI16 n){Trace*x=(Trace*)c;(void)h;(void)r;(void)t;x->current=n;}
int main(void){StarsDrawMassWarpGaugeInput i;Trace t;StarsRect r;r.left=0;r.top=0;r.right=10;r.bottom=10;t.font=0;t.brush=0;t.old_mode=0;t.mode_calls=0;t.maximum=0;t.gauge=0;t.displayed=0;t.current=0;i.hdc=4;i.rect=&r;i.best=10;i.current=12;i.font_arial8=7;i.brush_yellow=1;i.brush_red=2;i.brush_purple=3;i.select_object=so;i.draw_gauge=dg;i.set_bk_mode=bm;i.format_value=fm;i.get_text_extent=te;i.center_text=ct;i.context=&t;stars_draw_mass_warp_gauge(&i);if(t.font!=7U||t.brush!=1U||t.maximum!=9L||t.displayed!=16L||t.current!=5||t.mode_calls!=2U)return 1;i.best=-10;i.current=30;stars_draw_mass_warp_gauge(&i);if(t.brush!=2U)return 2;puts("DRAW_MASS_WARP_SOURCE=PASS");return 0;}
