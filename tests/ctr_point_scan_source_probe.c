#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T{int rect,scan,pos,inv,scroll;StarsI16 p[2],v[2],dx,dy;}T;
static void gr(void*v,StarsU16 w,StarsRect*r){T*t=(T*)v;(void)w;++t->rect;r->right=100;r->bottom=80;}
static StarsI16 st(void*v,StarsI16 n){T*t=(T*)v;(void)t;return n;}
static StarsI16 ps(void*v,StarsI16 n){T*t=(T*)v;(void)t;return n;}
static void sp(void*v,StarsU16 w,StarsU16 b,StarsI16 n,StarsI16 e){T*t=(T*)v;(void)w;(void)e;t->p[b]=n;++t->pos;}
static void iv(void*v,StarsU16 w,StarsI16 e){T*t=(T*)v;(void)w;(void)e;++t->inv;}
static void sc(void*v,StarsI16 x,StarsI16 y){T*t=(T*)v;t->dx=x;t->dy=y;++t->scroll;}
int main(void){StarsCtrPointScanInput i;T t;memset(&i,0,sizeof i);memset(&t,0,sizeof t);i.scanner_window=3;i.point.x=1500;i.point.y=1500;i.gal_size=5000;i.dy_sbar=10;i.scan_to_point=st;i.point_to_scan=ps;i.set_scroll_pos=sp;i.scroll_scanner=sc;i.invalidate=iv;i.get_client_rect=gr;i.context=&t;stars_ctr_point_scan(&i);if(t.rect!=1||t.pos!=2||t.p[0]!=1452||t.p[1]!=3464||t.inv!=1)return 1;memset(&t,0,sizeof t);i.x_scan_top=1400;i.y_scan_top=1400;i.scroll=1;stars_ctr_point_scan(&i);if(t.scroll!=1||t.dx!=-52||t.dy!=-2064)return 2;printf("CTR_POINT_SCAN_SOURCE_CHECKS=2 FAILURES=0 RESULT=PASS\n");return 0;}
