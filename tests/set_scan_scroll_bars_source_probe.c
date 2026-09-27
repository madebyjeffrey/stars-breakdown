#include <stdio.h>
#include "core.h"
typedef struct Trace { StarsU16 axis, range[2], calls; } Trace;
static StarsI16 scan(void *c, StarsI16 value){(void)c;return value;}
static void range(void *c, StarsU16 hwnd, StarsU16 axis, StarsU16 max,
    StarsU16 page, StarsU16 redraw){Trace*t=(Trace*)c;(void)hwnd;(void)page;(void)redraw;t->axis=axis;t->range[axis]=max;++t->calls;}
int main(void){StarsSetScanScrollBarsInput i;Trace t; t.axis=0;t.range[0]=0;t.range[1]=0;t.calls=0;i.in_scroll_set=0;i.d_gal_inv=4000;i.dy_sbar=100;i.client_right=1500;i.client_bottom=1600;i.d_scan_page=0;i.d_scan_inc=0;i.scan_to_point=scan;i.set_scroll_range=range;i.context=&t;stars_set_scan_scroll_bars(9,&i);if(i.in_scroll_set!=0||t.calls!=2||t.range[0]!=1500||t.range[1]!=1500||i.d_scan_page!=500||i.d_scan_inc!=64)return 1;i.in_scroll_set=0;i.d_gal_inv=1800;i.client_right=100;i.client_bottom=100;i.dy_sbar=0;i.d_scan_page=77;i.d_scan_inc=88;stars_set_scan_scroll_bars(9,&i);if(t.range[0]!=1000||t.range[1]!=1000||i.d_scan_page!=32||i.d_scan_inc!=4)return 2;puts("SET_SCAN_SCROLL_BARS_SOURCE=PASS");return 0;}
