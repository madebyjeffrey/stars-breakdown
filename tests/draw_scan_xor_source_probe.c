#include <stdio.h>
#include "core.h"
typedef struct Trace { StarsU16 stock, selected[2], rops[2], moves, lines, clip;
    StarsI16 ex[20]; } Trace;
static void rect(void*c,StarsU16 w,StarsRect*r){Trace*t=(Trace*)c;(void)w;r->right=100;r->bottom=80;t->moves=0;}
static void ex(void*c,StarsU16 h,StarsI16 a,StarsI16 b,StarsI16 d,StarsI16 e){Trace*t=(Trace*)c;(void)h;if(t->ex[0]<20){t->ex[0]++;t->ex[t->ex[0]]=a;t->ex[t->ex[0]+1]=b;t->ex[t->ex[0]+2]=d;t->ex[t->ex[0]+3]=e;}}
static StarsU16 go(void*c,StarsU16 n){Trace*t=(Trace*)c;(void)n;t->stock=66;return 66;}
static StarsU16 so(void*c,StarsU16 h,StarsU16 p){Trace*t=(Trace*)c;(void)h;t->selected[0]=p;return 77;}
static StarsU16 rop(void*c,StarsU16 h,StarsU16 n){Trace*t=(Trace*)c;(void)h;t->rops[0]=n;return 88;}
static void mv(void*c,StarsU16 h,StarsI16 x,StarsI16 y){Trace*t=(Trace*)c;(void)h;(void)x;(void)y;t->moves=1;}
static void ln(void*c,StarsU16 h,StarsI16 x,StarsI16 y){Trace*t=(Trace*)c;(void)h;(void)x;(void)y;t->lines++;}
static void sc(void*c,StarsU16 h,StarsU16 r){((Trace*)c)->clip=r;(void)h;}
int main(void){StarsDrawScanXorLinesInput i;Trace t;StarsU16 n;for(n=0;n<sizeof t.ex/sizeof t.ex[0];++n)t.ex[n]=0;t.stock=0;t.selected[0]=0;t.rops[0]=0;t.moves=0;t.lines=0;t.clip=0;i.hdc=4;i.scanner_window=5;i.points[0].x=1;i.points[0].y=2;i.points[2].x=10;i.points[2].y=20;i.points[3]=i.points[2];i.point_count=4;i.scrollbar_height=8;i.ship_pen=33;i.huge_region=99;i.get_client_rect=rect;i.exclude_clip_rect=ex;i.get_stock_object=go;i.select_object=so;i.set_rop2=rop;i.move_to=mv;i.line_to=ln;i.select_clip_region=sc;i.context=&t;stars_draw_scan_xor_lines(&i);if(t.stock!=66||t.selected[0]!=77||t.rops[0]!=88||t.moves!=1||t.lines!=1||t.clip!=99)return 1;puts("DRAW_SCAN_XOR_SOURCE=PASS");return 0;}
