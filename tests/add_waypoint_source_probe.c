#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct State {
    StarsU16 beeps, alerts, gets, draws, releases, warps, redraws, looks;
    StarsU16 changes, logical, bounds, invalidates;
    StarsI16 alert_id, alert_style;
    StarsU16 dc;
} State;
static StarsI16 dist(void *c, StarsI16 n){(void)c;return n==0x14?20:0;}
static StarsI16 grow(void *c,StarsMoveOrder **p,StarsU16 n){(void)c;(void)p;(void)n;return 0;}
static void beep(void*c,StarsU16 n){State*s=(State*)c;s->beeps+=(n==0x40);}
static void alert(void*c,StarsU16 n,StarsI16 x){State*s=(State*)c;s->alerts++;s->alert_id=(StarsI16)n;s->alert_style=x;}
static StarsU16 getdc(void*c,StarsU16 w){State*s=(State*)c;s->gets++;s->dc=(StarsU16)(w+1);return s->dc;}
static void draw(void*c,StarsU16 d,StarsI16 e){State*s=(State*)c;s->draws+=(d==s->dc&&e==0);}
static void rel(void*c,StarsU16 w,StarsU16 d){State*s=(State*)c;s->releases+=(w+1==d);}
static StarsU16 warp(void*c,StarsU16 w,const StarsMoveOrder*o){State*s=(State*)c;s->warps+=(w==9&&o->id==7);return 3;}
static void redraw(void*c,StarsU16 a,StarsI16 b){State*s=(State*)c;s->redraws+=(a==0&&b==0);}
static void look(void*c){++((State*)c)->looks;}
static void change(void*c,const StarsWayPointScan*s,StarsI16 n){State*x=(State*)c;x->changes+=(s->grobj==4&&n==1);}
static void logical(void*c,StarsPoint*p){State*s=(State*)c;s->logical++;p->x=(StarsI16)(p->x+1);}
static void bound(void*c,StarsRect*r,const StarsPoint*p,StarsI16 n){State*s=(State*)c;s->bounds+=(n==3&&p[0].x==101);r->left=1;r->top=2;r->right=3;r->bottom=4;}
static void inval(void*c,StarsU16 w,const StarsRect*r,StarsI16 f){State*s=(State*)c;s->invalidates+=(w==9&&r->right==3&&f==0);}
static void zero_ops(StarsAddWayPointInput*i,State*s){memset(i,0,sizeof(*i));i->scan_to_point=dist;i->message_beep=beep;i->alert_limit=alert;i->get_dc=getdc;i->draw_ship_path=draw;i->release_dc=rel;i->warp_best=warp;i->redraw_selection=redraw;i->lookup_fleet=look;i->change_selection=change;i->logical_to_scan=logical;i->bound_points=bound;i->invalidate_rect=inval;i->scanner_window=9;i->context=s;}
int main(void){StarsAddWayPointInput i;StarsWayPointScan s;StarsMoveOrder o[4];State t;StarsPoint p;StarsU16 fleets[1];memset(&t,0,sizeof t);memset(o,0,sizeof o);zero_ops(&i,&t);i.orders=o;i.order_count=0x57;i.order_capacity=4;p.x=2;p.y=3;s.point=p;if(stars_add_waypoint(&i,p,&s)!=0||t.beeps!=1||t.alert_id!=0x516||t.alert_style!=0x10)return 1;memset(&t,0,sizeof t);memset(o,0,sizeof o);zero_ops(&i,&t);i.orders=o;i.order_count=2;i.order_capacity=4;i.active_waypoint=0;o[0].point.x=10;o[0].point.y=10;o[1].point.x=20;o[1].point.y=20;p.x=15;p.y=16;s.point=p;s.grobj=1;s.grobj_full=1;s.idpl=7;o[0].flags=6;o[0].reserved[0]=5;fleets[0]=99;i.fleet_ids=fleets;i.fleet_count=1;if(stars_add_waypoint(&i,p,&s)!=1||i.order_count!=3||o[1].id!=7||((o[1].flags>>8)&15)!=1||((o[1].flags>>4)&15)!=3||o[0].flags!=0||t.gets!=1||t.draws!=1||t.releases!=1||t.warps!=1||t.redraws!=1||t.looks!=2||t.changes!=1)return 2;memset(&t,0,sizeof t);memset(o,0,sizeof o);zero_ops(&i,&t);i.orders=o;i.order_count=1;i.order_capacity=1;i.active_waypoint=0;o[0].point.x=0;o[0].point.y=0;p.x=100;p.y=100;s.point.x=0;s.point.y=0;s.grobj=1;s.idpl=4;i.reallocate_orders=grow;if(stars_add_waypoint(&i,p,&s)!=0||t.releases!=1)return 3;memset(&t,0,sizeof t);memset(o,0,sizeof o);zero_ops(&i,&t);i.orders=o;i.order_count=2;i.order_capacity=4;i.active_waypoint=0;o[0].point.x=0;o[0].point.y=0;o[1].point.x=50;o[1].point.y=50;p.x=100;p.y=100;s.point.x=0;s.point.y=0;s.grobj=1;s.idpl=4;i.scan_flags=0x80;if(stars_add_waypoint(&i,p,&s)!=1||s.grobj!=4||s.iwp!=1||t.logical!=3||t.bounds!=1||t.invalidates!=1)return 4;puts("ADD_WAYPOINT_SOURCE=PASS");return 0;}
