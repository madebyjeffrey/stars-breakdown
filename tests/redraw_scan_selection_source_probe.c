#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T{int vis,dc,path,draw,convert,rel;}T;
static StarsI16 vi(void*v,StarsU16 w){T*t=(T*)v;(void)w;++t->vis;return 1;}
static StarsU16 gd(void*v,StarsU16 w){T*t=(T*)v;(void)w;++t->dc;return 9;}
static void re(void*v,StarsU16 w,StarsU16 d){T*t=(T*)v;(void)w;(void)d;++t->rel;}
static void pa(void*v,StarsU16 d,StarsI16 f){T*t=(T*)v;(void)d;(void)f;++t->path;}
static void dr(void*v,StarsU16 d,const StarsRect*r){T*t=(T*)v;(void)d;(void)r;++t->draw;}
static void ls(void*v,StarsPoint*p){T*t=(T*)v;++t->convert;p->x=(StarsI16)(p->x+100);p->y=(StarsI16)(p->y+200);}
int main(void){StarsRedrawScanInput i;StarsScanSelection s;StarsRedrawScanInput z;T t;memset(&i,0,sizeof i);memset(&z,0,sizeof z);memset(&s,0,sizeof s);memset(&t,0,sizeof t);i.scanner_window=3;i.scan_zoom=2;i.scan_flags=0x1000;i.selection=&s;i.selected_point.x=30;i.selected_point.y=40;i.selected_id=12;i.selected_kind=2;i.selected_full=3;i.is_visible=vi;i.get_dc=gd;i.release_dc=re;i.draw_ship_path=pa;i.draw_scanner=dr;i.logical_to_scan=ls;i.context=&t;s.x=10;s.y=20;s.grobj=2;s.grobj_full=3;s.waypoint=4;s.fleet=5;s.planet=6;stars_redraw_scan_selection(0,0,&i);if(t.path!=1||t.draw!=2||t.convert!=2||t.rel!=1||s.x!=10||s.grobj!=2)return 1;memset(&t,0,sizeof t);s.grobj=2;i.selected_point.x=10;i.selected_point.y=20;stars_redraw_scan_selection(0,-1,&i);if(t.draw!=0||t.rel!=1)return 2;printf("REDRAW_SCAN_SELECTION_SOURCE_CHECKS=2 FAILURES=0 RESULT=PASS\n");return 0;}
