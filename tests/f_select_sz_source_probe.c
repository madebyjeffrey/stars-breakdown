#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T{int near,change,ensure,update,send,lookup;StarsI16 kind,index;}T;
static StarsI16 nf(void*v,StarsPoint p,StarsI16 k,StarsScanSelection*s){T*t=(T*)v;(void)p;++t->near;t->kind=k;s->x=p.x;s->y=p.y;return 1;}
static void ch(void*v,const StarsScanSelection*s,StarsI16 k){T*t=(T*)v;(void)s;t->kind=k;++t->change;}
static void en(void*v,StarsPoint p,StarsI16 f){T*t=(T*)v;(void)p;(void)f;++t->ensure;}
static void up(void*v,StarsU16 w){T*t=(T*)v;(void)w;++t->update;}
static void sm(void*v,StarsU16 w,StarsU16 m,StarsU16 p,StarsU32 l){T*t=(T*)v;(void)w;(void)m;(void)p;(void)l;++t->send;}
static StarsI16 lf(void*v,StarsU16 id,StarsFindNameFleet*f){T*t=(T*)v;(void)id;++t->lookup;f->point.x=30;f->point.y=40;f->index=2;return 1;}
int main(void){StarsFindNamePlanet p[2];StarsFindNameFleet f[1];StarsFSelectSzInput i;T t;memset(&i,0,sizeof i);memset(&t,0,sizeof t);p[0].name=(const StarsU8*)"Alpha";p[0].point.x=1;p[0].point.y=2;f[0].valid=1;f[0].name=(const StarsU8*)"Fleet One";i.name=(const StarsU8*)"Alpha";i.planets=p;i.planet_count=1;i.fleets=f;i.fleet_count=1;i.scanner_window=3;i.find_nearest=nf;i.change_selection=ch;i.ensure_point=en;i.update_window=up;i.send_message=sm;i.lookup_fleet=lf;i.context=&t;if(stars_f_select_sz(&i)!=1||t.kind!=1||t.change!=1||t.send!=1)return 1;i.name=(const StarsU8*)"Fleet One";if(stars_f_select_sz(&i)!=1||t.kind!=2||t.change!=2)return 2;i.name=(const StarsU8*)"Unknown";if(stars_f_select_sz(&i)!=0)return 3;puts("F_SELECT_SZ_SOURCE_CHECKS=3 FAILURES=0 RESULT=PASS");return 0;}
