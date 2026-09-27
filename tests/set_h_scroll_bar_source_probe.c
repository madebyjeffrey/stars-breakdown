#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T{int rect,metric,pos,range,window;StarsI16 metrics[4];StarsI16 pos_value,max_value;StarsU16 flags,x,y,cx,cy;}T;
static void rect(void*v,StarsU16 h,StarsRect*r){T*t=(T*)v;(void)h;r->left=0;r->top=0;r->right=100;r->bottom=50;++t->rect;}
static StarsI16 metric(void*v,StarsU16 n){T*t=(T*)v;++t->metric;return t->metrics[n];}
static void pos(void*v,StarsU16 h,StarsU16 b,StarsI16 p,StarsI16 r){T*t=(T*)v;(void)h;(void)b;(void)r;t->pos_value=p;++t->pos;}
static void range(void*v,StarsU16 h,StarsU16 b,StarsI16 n,StarsI16 m,StarsI16 r){T*t=(T*)v;(void)h;(void)b;(void)n;(void)r;t->max_value=m;++t->range;}
static void window(void*v,StarsU16 h,StarsU16 a,StarsI16 x,StarsI16 y,StarsI16 w,StarsI16 c,StarsU16 f){T*t=(T*)v;(void)h;(void)a;t->x=(StarsU16)x;t->y=(StarsU16)y;t->cx=(StarsU16)w;t->cy=(StarsU16)c;t->flags=f;++t->window;}
int main(void){StarsSetHScrollBarOps o;StarsSetHScrollBarInput i;StarsReportViewState v;T t;memset(&o,0,sizeof o);o.get_client_rect=rect;o.get_system_metrics=metric;o.set_scroll_pos=pos;o.set_scroll_range=range;o.set_window_pos=window;memset(&t,0,sizeof t);t.metrics[2]=10;t.metrics[3]=6;memset(&v,0,sizeof v);v.report_window=3;v.hscroll_window=4;v.fields=4;v.field_first=2;v.rows_visible=3;v.visible_fields=0x1e;v.column_width[0]=4;v.column_width[1]=25;v.column_width[2]=25;v.column_width[3]=25;v.column_width[4]=25;i.view=&v;i.report_window=3;i.arial8_height=8;i.ops=&o;i.context=&t;stars_set_h_scroll_bar(&i);if(v.column_scroll!=2||t.pos_value!=1||t.max_value!=2||t.flags!=0x44||t.x!=10||t.y!=51||t.cx!=80||t.cy!=6)return 1;memset(&t,0,sizeof t);v.visible_fields=0;v.field_first=5;stars_set_h_scroll_bar(&i);if(v.column_scroll!=0||v.field_first!=1||t.pos!=1||t.range!=0||t.flags!=0x84)return 2;printf("SET_H_SCROLL_BAR_CHECKS=2 FAILURES=0 RESULT=PASS\n");return 0;}
