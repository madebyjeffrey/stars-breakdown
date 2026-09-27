#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 calls, destroyed, created, invalidated, positioned, procedures;
    StarsU16 show, width, height, flags;
    StarsU32 tick;
    StarsRect rectangle;
} Trace;
static StarsI16 get_string(void *v,StarsI16 id,StarsU8*out){(void)v;(void)id;strcpy((char*)out,"Tip");return 3;}
static StarsU16 get_dc(void *v,StarsU16 w){(void)v;(void)w;return 5;}
static StarsU16 select_object(void *v,StarsU16 d,StarsU16 o){(void)v;(void)d;(void)o;return 9;}
static StarsU32 extent(void *v,StarsU16 d,const StarsU8*s,StarsI16 n){(void)v;(void)d;(void)s;(void)n;return 12UL;}
static StarsU32 get_tick(void *v){Trace*t=(Trace*)v;return t->tick;}
static void pos(void *v,StarsU16 w,StarsI16 x,StarsI16 y,StarsI16 width,StarsI16 h,StarsU16 f){Trace*t=(Trace*)v;t->positioned++;t->width=width;t->height=h;t->flags=f;(void)w;(void)x;(void)y;}
static void proc(void *v,StarsU16 w,StarsU16 m,StarsU16 show,StarsU32 l){Trace*t=(Trace*)v;t->procedures++;t->show=show;(void)w;(void)m;(void)l;}
static StarsU16 visible(void *v,StarsU16 w){(void)v;(void)w;return 0;}
static void destroy(void *v,StarsU16 w){Trace*t=(Trace*)v;t->destroyed++;(void)w;}
static void release_dc(void *v,StarsU16 w,StarsU16 d){(void)v;(void)w;(void)d;}
static StarsU16 create(void *v,StarsI16 width,StarsI16 height){Trace*t=(Trace*)v;t->created++;t->width=(StarsU16)width;t->height=(StarsU16)height;return 44;}
static void invalidate(void *v,StarsU16 w,StarsU16 erase){Trace*t=(Trace*)v;t->invalidated++;(void)w;(void)erase;}
int main(void){StarsTooltipState s;StarsTooltipOps o;Trace t;StarsRect r;memset(&s,0,sizeof s);memset(&o,0,sizeof o);memset(&t,0,sizeof t);r.left=1;r.top=2;r.right=3;r.bottom=4;o.font=8;o.font_height=8;o.get_string=get_string;o.get_dc=get_dc;o.select_object=select_object;o.text_extent=extent;o.get_tick=get_tick;o.set_window_pos=pos;o.window_proc=proc;o.is_visible=visible;o.destroy_window=destroy;o.release_dc=release_dc;o.create_window=create;o.invalidate_window=invalidate;o.context=&t;s.last_tick=100;s.window=0;stars_show_tooltip(4,&r,&s,&o);if(s.window!=44||t.created!=1||t.positioned!=1||t.procedures!=1||t.show!=1||t.width!=18||t.height!=14||t.flags!=0x216)return 1;stars_show_tooltip(4,&r,&s,&o);if(t.created!=1||t.procedures!=1)return 2;s.window=44;stars_show_tooltip(-1,(const StarsRect*)0,&s,&o);if(t.destroyed!=1)return 3;puts("SHOW_TOOLTIP_SOURCE=PASS");return 0;}
