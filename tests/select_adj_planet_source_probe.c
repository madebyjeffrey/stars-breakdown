#include <stdio.h>
#include "core.h"

typedef struct Trace { StarsI16 invalid,changes,redraws,mains,centers,bars,mines,titles; StarsI16 last_id; } Trace;
static void ir(void*v,StarsI16 a,StarsI16 b){Trace*t=(Trace*)v;t->invalid++;(void)a;(void)b;}
static void cs(void*v,const StarsScanSelection*s,StarsI16 m){Trace*t=(Trace*)v;t->changes++;t->last_id=s->x;(void)m;}
static void rs(void*v,StarsI16 a,StarsI16 b){Trace*t=(Trace*)v;t->redraws++;(void)a;(void)b;}
static void cm(void*v,StarsI16 a,StarsI16 b){Trace*t=(Trace*)v;t->mains++;t->last_id=b;(void)a;}
static void ce(void*v,StarsPoint p,StarsI16 m){Trace*t=(Trace*)v;t->centers++;t->last_id=p.x;(void)m;}
static void db(void*v){Trace*t=(Trace*)v;t->bars++;}
static void im(void*v,StarsU16 w,StarsU16 e){Trace*t=(Trace*)v;t->mines++;(void)w;(void)e;}
static void mt(void*v){Trace*t=(Trace*)v;t->titles++;}
int main(void){StarsAdjacentPlanet p[3];StarsPoint pt[4];StarsI16 owned[2];StarsSelectAdjacentInput i;StarsSelectAdjacentOps o;Trace t;p[0].id=1;p[0].owner=2;p[0].visibility_flags=7;p[1].id=2;p[1].owner=2;p[1].visibility_flags=7;p[2].id=3;p[2].owner=1;p[2].visibility_flags=1;pt[1].x=11;pt[1].y=0;pt[2].x=22;pt[2].y=0;pt[3].x=33;pt[3].y=0;owned[0]=1;owned[1]=2;i.planets=p;i.planet_count=3;i.planet_points=pt;i.player_planet_ids=owned;i.player_planet_count=2;i.current_player=2;i.selected_planet=1;i.mine_window=88;i.report_cached=0;o.invalidate_report=ir;o.change_scan=cs;o.redraw_scan=rs;o.change_main=cm;o.center_scan=ce;o.draw_scroll_bar=db;o.invalidate_mine=im;o.set_mineral_title=mt;o.context=&t;t.invalid=0;t.changes=0;t.redraws=0;t.mains=0;t.centers=0;t.bars=0;t.mines=0;t.titles=0;stars_select_adj_planet(1,2,&i,&o);if(t.invalid!=1||t.changes!=1||t.redraws!=2||t.mains!=1||t.centers!=1||t.last_id!=22||t.bars!=1||t.mines!=1||t.titles!=1)return 1;puts("SELECT_ADJ_PLANET_SOURCE=PASS");return 0;}
