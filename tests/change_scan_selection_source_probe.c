#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct State{StarsU16 find,redraw,things,fill,row,update,ensure,sbar,title,getdc,draw,release,ship,tutor,logical;StarsI16 active;StarsRect last;}State;
static StarsI16 find(void*c,StarsPoint p,StarsU16 g,StarsWayPointScan*s){State*x=(State*)c;x->find+=(p.x==4&&g==8);s->point.x=6;s->point.y=7;s->grobj=8;s->grobj_full=0;s->idpl=-1;s->ifl=-1;s->iwp=2;s->ith=5;return 1;}
static void redraw(void*c,StarsU16 h,StarsI16 v){State*x=(State*)c;x->redraw+=(h==0&&(v==-1||v==1));}
static StarsI16 thing(void*c,StarsI16 i,StarsPoint*p){State*x=(State*)c;x->things+=(i==5);p->x=100;p->y=200;return i==5;}
static void fill(void*c){++((State*)c)->fill;}
static void row(void*c,StarsI16 n){State*x=(State*)c;x->row+=(n==2);x->active=n;}
static void upd(void*c,StarsI16 n){State*x=(State*)c;x->update+=(n==0);}
static void ens(void*c,StarsPoint p,StarsI16 n){State*x=(State*)c;x->ensure+=(p.x==6&&p.y==7&&n==1);}
static void sbar(void*c,StarsU16 n){State*x=(State*)c;x->sbar+=(n==0);}
static void title(void*c,StarsU16 n){State*x=(State*)c;x->title+=(n==12);}
static StarsU16 getdc(void*c,StarsU16 n){State*x=(State*)c;x->getdc+=(n==11);return 22;}
static void draw(void*c,StarsU16 d,const StarsRect*r){State*x=(State*)c;x->draw+=(d==22);x->last=*r;}
static void rel(void*c,StarsU16 w,StarsU16 d){State*x=(State*)c;x->release+=(w==11&&d==22);}
static void logical(void*c,StarsPoint*p){State*x=(State*)c;x->logical++;p->x=(StarsI16)(p->x+1);}
static void ship(void*c,StarsU16 a,StarsU16 b){State*x=(State*)c;x->ship+=(a==0&&(b==0x122||b==0x4002));}
static void tutor(void*c){++((State*)c)->tutor;}
static void init(StarsChangeScanSelInput*i,StarsScanSelection*s,State*x){memset(i,0,sizeof *i);i->selection=s;i->active_waypoint=0;i->scanner_window=11;i->mine_window=12;i->player=0;i->tutorial_flags=0x0800;i->special_radius=8;i->find_nearest=find;i->redraw_selection=redraw;i->get_thing=thing;i->fill_orders=fill;i->set_order_row=row;i->update_orders=upd;i->ensure_point=ens;i->draw_scroll_bar=sbar;i->set_title=title;i->get_dc=getdc;i->draw_scanner=draw;i->release_dc=rel;i->logical_to_scan=logical;i->draw_ship=ship;i->advance_tutor=tutor;i->context=x;}
int main(void){StarsChangeScanSelInput i;StarsScanSelection s;StarsWayPointScan n;State x;memset(&x,0,sizeof x);memset(&s,0,sizeof s);memset(&n,0,sizeof n);s.x=4;s.y=5;s.grobj=8;s.grobj_full=0;s.thing=5;s.planet=0;s.fleet=0;s.waypoint=0;init(&i,&s,&x);n.point.x=4;n.point.y=5;n.grobj=8;n.grobj_full=0;n.idpl=0;n.ifl=0;n.iwp=0;n.ith=5;stars_change_scan_selection(&i,&n,1);if(x.redraw!=0)return 1;n.point.x=4;n.point.y=5;n.grobj=8;n.grobj_full=0;n.iwp=-1;n.ith=5;stars_change_scan_selection(&i,&n,0);if(x.find!=1||s.x!=6||s.y!=7||s.grobj!=8||s.planet!=-1||x.fill!=1||x.row!=1||x.update!=1||x.ensure!=1||x.sbar!=1||x.title!=1||x.getdc!=2||x.draw!=2||x.release!=2||x.logical!=4||x.ship!=1||x.tutor!=1)return 2;printf("CHANGE_SCAN_SELECTION_SOURCE=PASS\n");return 0;}
