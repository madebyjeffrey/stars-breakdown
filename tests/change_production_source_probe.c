#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { int init, dialog, release, finish, draw, free, end, format, alert; StarsI16 result, alert_code; StarsU16 draw_flag, end_window; StarsU8 *freed; } Trace;
static void init(void*v,StarsU8*b,StarsU16 n){Trace*t=(Trace*)v;(void)b;(void)n;++t->init;}
static StarsI16 dialog(void*v,StarsU8*b,StarsU16 n){Trace*t=(Trace*)v;(void)b;(void)n;++t->dialog;return 1;}
static void release(void*v){++((Trace*)v)->release;}
static void finish(void*v,StarsI16 r){Trace*t=(Trace*)v;t->result=r;++t->finish;}
static void draw(void*v,StarsU16 h,StarsU16 f){Trace*t=(Trace*)v;(void)h;t->draw_flag=f;++t->draw;}
static void free_p(void*v,StarsU8*p){Trace*t=(Trace*)v;t->freed=p;++t->free;}
static void end(void*v,StarsU16 h,StarsI16 r){Trace*t=(Trace*)v;(void)r;t->end_window=h;++t->end;}
static const StarsU8 *format(void*v,StarsU16 id){static StarsU8 s[]="ERR";Trace*t=(Trace*)v;(void)id;++t->format;return s;}
static void alert(void*v,const StarsU8*s,StarsI16 c){Trace*t=(Trace*)v;(void)s;t->alert_code=c;++t->alert;}
int main(void){StarsChangeProductionOps o;StarsChangeProductionInput i;Trace t;StarsU8 b[256],p[4];memset(&o,0,sizeof o);o.initialize=init;o.dialog=dialog;o.release_dialog_proc=release;o.finish=finish;o.draw_ship=draw;o.free_product=free_p;o.end_dialog=end;o.format_ids=format;o.alert=alert;memset(&t,0,sizeof t);memset(&i,0,sizeof i);i.product_buffer=b;i.product_buffer_size=256;i.selected_object=1;i.saved_environment=p;i.ops=&o;i.context=&t;if(stars_change_production(&i)!=1||t.init!=1||t.dialog!=1||t.release!=1||t.finish!=1||t.result!=1||t.draw!=1||t.draw_flag!=8||i.dialog_up!=0||i.current_environment!=p)return 1;memset(&t,0,sizeof t);memset(&i,0,sizeof i);i.exception=1;i.global_product=p;i.dialog_window=77;i.dialog_up=1;i.saved_environment=b;i.ops=&o;i.context=&t;if(stars_change_production(&i)!=0||t.free!=1||t.freed!=p||t.end!=1||t.end_window!=77||t.format!=1||t.alert!=1||t.alert_code!=0x10||i.global_product!=0||i.dialog_window!=0||i.dialog_up!=0||i.current_environment!=b)return 2;printf("CHANGE_PRODUCTION_CHECKS=2 FAILURES=0 RESULT=PASS\n");return 0;}
