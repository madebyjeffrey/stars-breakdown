#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { unsigned fills, blts, selects; StarsRect fill; StarsU16 brush; } Trace;
static void fill(void *c, StarsU16 dc, const StarsRect *r, StarsU16 b)
{Trace*t=(Trace*)c;(void)dc;++t->fills;t->fill=*r;t->brush=b;}
static void select_object(void *c, StarsU16 dc, StarsU16 o)
{Trace*t=(Trace*)c;(void)dc;(void)o;++t->selects;}
static void pat(void *c, StarsU16 dc, StarsI16 x, StarsI16 y, StarsI16 w, StarsI16 h, StarsU32 rop)
{Trace*t=(Trace*)c;(void)dc;(void)x;(void)y;(void)w;(void)h;(void)rop;++t->blts;}
int main(void)
{StarsDrawLockLightInput i;Trace t;memset(&t,0,sizeof(t));i.hdc=3U;i.rect.left=10U;i.rect.top=20U;i.rect.right=30U;i.rect.bottom=40U;i.full_redraw=0;i.button_face=7U;i.button_shadow=8U;i.button_hilite=9U;i.fill_rect=fill;i.select_object=select_object;i.pat_blt=pat;i.context=&t;
 stars_draw_lock_light(&i);if(t.fills!=1U||t.fill.left!=12U||t.fill.top!=22U||t.fill.right!=28U||t.fill.bottom!=38U||t.brush!=7U)return 1;
 memset(&t,0,sizeof(t));i.full_redraw=1;stars_draw_lock_light(&i);if(t.fills!=0U||t.selects!=2U||t.blts!=4U)return 2;puts("LOCK_LIGHT_SOURCE=PASS");return 0;}
