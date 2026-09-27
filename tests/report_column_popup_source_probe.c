#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 choice, sort_calls, scroll_calls, invalidates; StarsU16 menu_count, first_id, dc; } Trace;
static StarsU16 get_dc(void *v, StarsU16 w){Trace*t=(Trace*)v;t->dc=9;(void)w;return t->dc;}
static void release_dc(void *v, StarsU16 w, StarsU16 d){(void)v;(void)w;(void)d;}
static StarsI16 header(void *v, StarsI16 r, StarsI16 c, StarsU16 d, StarsU8 *out, StarsU16 n){(void)v;(void)r;(void)c;(void)d;if(n>2){out[0]=(StarsU8)'H';out[1]=(StarsU8)'\0';}return 1;}
static StarsI16 string(void *v, StarsI16 id, StarsU8 *out, StarsU16 n){Trace*t=(Trace*)v;if(t->first_id==0)t->first_id=(StarsU16)id;if(n>2){out[0]=(StarsU8)'S';out[1]=0;}return 1;}
static StarsI16 menu(void *v, StarsU16 w, StarsI16 x, StarsI16 y, StarsU16 n, const StarsU8 *const *items, StarsI16 right){Trace*t=(Trace*)v;t->menu_count=n;(void)w;(void)x;(void)y;(void)items;(void)right;return t->choice;}
static void sort(void *v, StarsI16 r, StarsI16 c){Trace*t=(Trace*)v;t->sort_calls++;(void)r;(void)c;}
static void scroll(void *v){Trace*t=(Trace*)v;t->scroll_calls++;}
static void invalidate(void *v, StarsU16 w){Trace*t=(Trace*)v;t->invalidates++;(void)w;}
int main(void){StarsReportPopupState s;StarsReportColumnPopupInput i;Trace t;memset(&s,0,sizeof s);memset(&t,0,sizeof t);s.report=0;s.field_count=4;s.sort_column=-1;s.visible_mask=3;memset(&i,0,sizeof i);i.report_window=20;i.state=&s;i.column=1;i.hdc=9;i.get_header=header;i.get_string=string;i.popup_menu=menu;i.sort_cache=sort;i.set_scroll_bar=scroll;i.invalidate=invalidate;i.get_dc=get_dc;i.release_dc=release_dc;i.context=&t;t.choice=0;stars_report_column_popup(10,11,&i);if(s.sort_column!=1||s.ascending!=1||t.sort_calls!=1||t.menu_count!=7)return 1;t.choice=3;stars_report_column_popup(10,11,&i);if((s.visible_mask&2U)!=0U||t.scroll_calls!=1)return 2;t.choice=6;stars_report_column_popup(10,11,&i);if((s.visible_mask&4U)==0U||t.scroll_calls!=2||t.invalidates!=3)return 3;puts("REPORT_COLUMN_POPUP_SOURCE=PASS");return 0;}
