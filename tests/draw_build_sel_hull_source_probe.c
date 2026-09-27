#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace {
    StarsI16 dc, released, fills, restores, lines;
    StarsU16 first_label;
    StarsI32 first_value;
} ProbeTrace;

static StarsU16 probe_dc(void *opaque, StarsU16 window)
{ ProbeTrace *t=(ProbeTrace *)opaque; (void)window; ++t->dc; return 7; }
static void probe_release(void *opaque, StarsU16 window, StarsU16 dc)
{ ProbeTrace *t=(ProbeTrace *)opaque; (void)window; (void)dc; ++t->released; }
static void probe_fill(void *opaque, StarsU16 dc, const StarsRect *rect)
{ ProbeTrace *t=(ProbeTrace *)opaque; (void)dc; (void)rect; ++t->fills; }
static void probe_line(void *opaque, StarsU16 dc, StarsI16 row, StarsU16 label,
    StarsI32 first, StarsI32 second, StarsI16 color)
{ ProbeTrace *t=(ProbeTrace *)opaque; (void)dc; (void)row; (void)second; (void)color;
  if (t->lines==1) { t->first_label=label; t->first_value=first; } ++t->lines; }
static void probe_restore(void *opaque, StarsU16 window, StarsU16 dc)
{ ProbeTrace *t=(ProbeTrace *)opaque; (void)window; (void)dc; ++t->restores; }

int main(void)
{
    StarsDrawHullOps ops; StarsDrawHullInput in; ProbeTrace t; StarsI16 f;
    memset(&ops,0,sizeof ops); memset(&in,0,sizeof in); memset(&t,0,sizeof t);
    ops.get_dc=probe_dc; ops.release_dc=probe_release; ops.fill_background=probe_fill;
    ops.draw_line=probe_line; ops.restore=probe_restore;
    in.window=10; in.mode=0; in.has_hull=1; in.hull_id=7; in.tech_flags=0x4000;
    in.client_rect.right=300; in.client_rect.bottom=200; in.mineral_cost[0]=10;
    in.mineral_cost[1]=20; in.mineral_cost[2]=30; in.mineral_cost[3]=40;
    in.empty_weight=5; in.armor=40; in.shield=20; in.power=12; in.cloak=30;
    in.jammer=25; in.speed=3; in.scanner_range=100; in.scanner_width=4;
    in.ops=&ops; in.context=&t;
    stars_draw_build_sel_hull(&in);
    f=t.dc!=1 || t.released!=1 || t.fills!=1 || t.restores!=1 || t.lines!=11;
    f+=t.first_label!=STARS_HULL_ROW_COST || t.first_value!=10;
    memset(&t,0,sizeof t); in.starbase_mode=1; in.race_can_discount=0;
    in.race_special=0; stars_draw_build_sel_hull(&in);
    f+=t.first_value!=4;
    memset(&t,0,sizeof t); in.mode=3; stars_draw_build_sel_hull(&in);
    f+=t.lines!=0 || t.dc!=0;
    memset(&t,0,sizeof t); in.mode=0; in.starbase_mode=0; in.popup_window=1;
    in.popup_kind=0x0b; in.popup_max=1; in.popup_distance=40; in.popup_units=2;
    stars_draw_build_sel_hull(&in);
    f+=t.lines!=12;
    { FILE *out=fopen("C:\\HULL\\HUR.TXT","w"); if(!out)return 3;
      fprintf(out,"DRAW_HULL_CHECKS=5 FAILURES=%d RESULT=%s\n",(int)f,f==0?"PASS":"FAIL"); fclose(out); }
#ifndef TARGET_NATIVE
    printf("DRAW_HULL_CHECKS=5 FAILURES=%d RESULT=%s\n",(int)f,f==0?"PASS":"FAIL");
#endif
    return f==0?0:1;
}
