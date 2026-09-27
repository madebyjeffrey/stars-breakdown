#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T{int vis,dc,rect,draw,upd,scroll,val,rel;}T;
static StarsI16 vi(void*v,StarsU16 w){T*t=(T*)v;(void)w;++t->vis;return 1;}
static StarsU16 gd(void*v,StarsU16 w){T*t=(T*)v;(void)w;++t->dc;return 7;}
static void gr(void*v,StarsU16 w,StarsRect*r){T*t=(T*)v;(void)w;++t->rect;r->right=100;r->bottom=80;}
static void dr(void*v,StarsU16 d,StarsU16 w,const StarsRect*r){T*t=(T*)v;(void)d;(void)w;(void)r;++t->draw;}
static void up(void*v,StarsU16 w){T*t=(T*)v;(void)w;++t->upd;}
static void sw(void*v,StarsU16 w,StarsI16 x,StarsI16 y,const StarsRect*a,const StarsRect*b){T*t=(T*)v;(void)w;(void)x;(void)y;(void)a;(void)b;++t->scroll;}
static void va(void*v,StarsU16 w,const StarsRect*r){T*t=(T*)v;(void)w;(void)r;++t->val;}
static void re(void*v,StarsU16 w,StarsU16 d){T*t=(T*)v;(void)w;(void)d;++t->rel;}
int main(void){StarsScrollScannerOps o;StarsScrollScannerInput i;T t;memset(&o,0,sizeof o);memset(&i,0,sizeof i);memset(&t,0,sizeof t);o.visible=vi;o.get_dc=gd;o.get_client_rect=gr;o.draw=dr;o.update=up;o.scroll_window=sw;o.validate_rect=va;o.release_dc=re;i.scanner_window=3;i.dy_sbar=10;i.ops=&o;i.context=&t;stars_scroll_scanner(10,5,&i);if(t.draw!=2||t.val!=2||t.scroll!=1||t.upd!=2||t.rel!=1)return 1;memset(&t,0,sizeof t);i.browser_window=4;stars_scroll_scanner(60,0,&i);if(t.draw!=1||t.scroll!=0)return 2;printf("SCROLL_SCANNER_SOURCE_CHECKS=2 FAILURES=0 RESULT=PASS\n");return 0;}
