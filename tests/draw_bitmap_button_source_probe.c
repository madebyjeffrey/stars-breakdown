#include <stdio.h>
#include "core.h"

typedef struct Trace { StarsI16 selects; StarsI16 patblts; StarsI16 dibs;
    StarsI16 last_x; StarsI16 last_y; StarsI16 last_width; } Trace;
static StarsU16 select_object(void *c, StarsU16 dc, StarsU16 object)
{ (void)dc;(void)object;++((Trace *)c)->selects;return 9; }
static void pat_blt(void *c, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsI16 w, StarsI16 h, StarsU32 rop)
{ Trace *t=(Trace *)c;(void)dc;(void)h;(void)rop;++t->patblts;t->last_x=x;t->last_y=y;t->last_width=w; }
static void dib_blt(void *c, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsI16 w, StarsI16 h, StarsU16 dib, StarsI16 sx, StarsI16 sy,
    StarsI16 sw, StarsI16 sh, StarsU32 rop)
{ (void)dc;(void)x;(void)y;(void)h;(void)dib;(void)sx;(void)sy;(void)sw;
  (void)sh;(void)rop;++((Trace *)c)->dibs;((Trace *)c)->last_width=w; }

static void initialize(StarsDrawBitmapButtonInput *i,
    StarsDrawBitmapButtonOps *o, Trace *t)
{
    i->hdc=1;i->point.x=10;i->point.y=20;i->button=7;i->pressed=0;
    i->arial8_height=8;i->button_face_brush=2;i->button_shadow_brush=3;
    i->button_highlight_brush=4;i->toolbar_bitmap=5;i->ops=*o;i->context=t;
}
int main(void)
{
    StarsDrawBitmapButtonInput i;StarsDrawBitmapButtonOps o;Trace t;int failures=0;
    o.select_object=select_object;o.pat_blt=pat_blt;o.dib_blt=dib_blt;
    t.selects=t.patblts=t.dibs=0;t.last_x=t.last_y=t.last_width=0;
    initialize(&i,&o,&t);stars_draw_bitmap_button(&i);
    if(t.selects!=4||t.patblts!=12||t.dibs!=1||t.last_width!=24)++failures;
    t.selects=t.patblts=t.dibs=0;i.pressed=1;
    stars_draw_bitmap_button(&i);
    if(t.selects!=4||t.patblts!=14||t.dibs!=1)++failures;
    t.selects=t.patblts=t.dibs=0;i.pressed=2;
    stars_draw_bitmap_button(&i);
    if(t.selects!=5||t.patblts!=13||t.dibs!=1)++failures;
#ifndef TARGET_NATIVE
    printf("DRAW_BITMAP_BUTTON_CHECKS=3 FAILURES=%d RESULT=%s\n",failures,
        failures?"FAIL":"PASS");
#endif
    return failures!=0;
}
