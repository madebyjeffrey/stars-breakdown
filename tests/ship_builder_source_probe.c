#include <stdio.h>
#include "core.h"
typedef struct Trace { StarsU16 nth, make, dialog, resource, owner, freed, fill;
    StarsU16 production_list; const void *production; } Trace;
static void *nth(void *c, StarsU16 n){((Trace*)c)->nth=n;return (void*)0x1234;}
static void *make(void *c, StarsU16 i){Trace*t=(Trace*)c;t->make=i;return (void*)0x5678;}
static StarsI16 dialog(void *c, StarsU16 i, StarsU16 r, StarsU16 o, void *p){Trace*t=(Trace*)c;(void)i;(void)p;t->dialog=1;t->resource=r;t->owner=o;return 0;}
static void free_proc(void *c, void *p){(void)p;((Trace*)c)->freed=1;}
static void fill(void *c, StarsU16 list, const void *prod, const void *planet){Trace*t=(Trace*)c;(void)planet;t->fill=1;t->production_list=list;t->production=prod;}
static int run_case(StarsU16 flags, StarsU16 type, const void *prod){StarsShipBuilderInput i;Trace t;int ok; t.nth=0;t.make=0;t.dialog=0;t.resource=0;t.owner=0;t.freed=0;t.fill=0;t.production_list=0;t.production=(void*)0;i.dialog_size.x=11;i.dialog_size.y=20;i.dialog_font_height=4;i.display_flags=flags;i.instance=7;i.frame_window=8;i.production_list=9;i.selected_object_type=type;i.selected_production=prod;i.nth_valid_shdef=nth;i.make_proc_instance=make;i.dialog_box=dialog;i.free_proc_instance=free_proc;i.fill_production=fill;i.context=&t;ok=stars_ship_builder(&i)==0&&i.dialog_size.y==(StarsI16)(20+((flags>>14)&1U)*12)&&t.nth==0&&t.make==7&&t.dialog&&t.resource==0x5cU&&t.owner==8&&t.freed;if(type==1U&&prod)ok=ok&&t.fill&&t.production_list==9U&&t.production==prod;else ok=ok&&!t.fill;return ok;}
int main(void){int marker;marker=1;if(!run_case(0x4000U,1,(const void*)0x2222))return 1;if(!run_case(0,0,(const void*)0))return 2;puts("SHIP_BUILDER_SOURCE=PASS");return marker-1;}
