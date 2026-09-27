#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T { int rect,fill,bk,sticky,msg,text,get,select,alert,focus,end,help;
    StarsU16 last_focus; StarsI16 end_code; } T;
static void gr(void*v,StarsU16 w,StarsRect*r){T*t=(T*)v;(void)w;++t->rect;r->right=20;r->bottom=30;}
static void fr(void*v,StarsU16 h,const StarsRect*r,StarsU16 b){T*t=(T*)v;(void)h;(void)r;(void)b;++t->fill;}
static void bk(void*v,StarsU16 h,StarsU32 c){T*t=(T*)v;(void)h;(void)c;++t->bk;}
static void sk(void*v,StarsU16 w,StarsPoint*p,StarsI16 f){T*t=(T*)v;(void)w;(void)p;(void)f;++t->sticky;}
static void sm(void*v,StarsU16 w,StarsU16 i,StarsU16 m,StarsU16 p,StarsU32 l){T*t=(T*)v;(void)w;(void)i;(void)m;(void)p;(void)l;++t->msg;}
static void gt(void*v,StarsU16 w,StarsU16 i,StarsU8*d,StarsU16 n){T*t=(T*)v;(void)w;(void)i;(void)n;++t->text;d[0]='x';d[1]=0;}
static StarsU16 gi(void*v,StarsU16 w,StarsU16 i){T*t=(T*)v;(void)w;(void)i;++t->get;return 9;}
static StarsI16 sel(void*v,const StarsU8*s){T*t=(T*)v;(void)s;++t->select;return 0;}
static const StarsU8 *ids(void*v,StarsU16 n){(void)v;(void)n;return (const StarsU8 *)"bad";}
static void al(void*v,const StarsU8*s,StarsI16 n){T*t=(T*)v;(void)s;(void)n;++t->alert;}
static void fo(void*v,StarsU16 w){T*t=(T*)v;t->last_focus=w;++t->focus;}
static void en(void*v,StarsU16 w,StarsI16 n){T*t=(T*)v;(void)w;t->end_code=n;++t->end;}
static void wh(void*v,StarsU16 w,const StarsU8*f,StarsU16 n,StarsU16 d){T*t=(T*)v;(void)w;(void)f;(void)n;(void)d;++t->help;}
int main(void){StarsFindDlgOps o;StarsFindDlgInput i;StarsPoint p;T t;memset(&o,0,sizeof o);memset(&i,0,sizeof i);memset(&t,0,sizeof t);o.get_client_rect=gr;o.fill_rect=fr;o.set_background_color=bk;o.sticky_position=sk;o.send_item_message=sm;o.get_item_text=gt;o.get_item=gi;o.select_name=sel;o.format_ids=ids;o.alert=al;o.set_focus=fo;o.end_dialog=en;o.win_help=wh;i.button_face_brush=6;i.button_face_color=7;i.sticky_point=&p;i.help_file=(const StarsU8 *)"h";i.ops=&o;i.context=&t;if(stars_find_dlg(1,0x14,2,0,&i)!=1||t.rect!=1||t.fill!=1)return 1;if(stars_find_dlg(1,0x19,2,6UL<<16,&i)!=6||t.bk!=1)return 2;if(stars_find_dlg(1,0x110,0,0,&i)!=1||t.sticky!=1||t.msg!=1)return 3;if(stars_find_dlg(1,0x111,1,0,&i)!=0||t.alert!=1||t.focus!=1||t.end!=0)return 4;if(stars_find_dlg(1,0x111,2,0,&i)!=1||t.end_code!=0)return 5;if(stars_find_dlg(1,0x111,0x76,0,&i)!=1||t.help!=1)return 6;puts("FIND_DLG_SOURCE_CHECKS=6 FAILURES=0 RESULT=PASS");return 0;}
