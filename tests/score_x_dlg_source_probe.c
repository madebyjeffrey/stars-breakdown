#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T { int begin,end,history,score,victory,rect,fill,cursor,init,sticky,advance,ended,help,invalidated,strings,popup; StarsU16 brush; StarsI16 result; } T;
static StarsU16 bp(void*v,StarsU16 h,StarsRect*r){T*t=(T*)v;(void)h;(void)r;++t->begin;return 9;}
static void ep(void*v,StarsU16 h,StarsRect*r){T*t=(T*)v;(void)h;(void)r;++t->end;}
static void drh(void*v,StarsU16 d,StarsU16 h){T*t=(T*)v;(void)d;(void)h;++t->history;}
static void drs(void*v,StarsU16 d,StarsU16 h){T*t=(T*)v;(void)d;(void)h;++t->score;}
static void drv(void*v,StarsU16 d,StarsU16 h){T*t=(T*)v;(void)d;(void)h;++t->victory;}
static void gr(void*v,StarsU16 h,StarsRect*r){T*t=(T*)v;(void)h;r->left=0;r->top=0;r->right=100;r->bottom=80;++t->rect;}
static void fi(void*v,StarsU16 d,const StarsRect*r,StarsU16 b){T*t=(T*)v;(void)d;(void)r;t->brush=b;++t->fill;}
static void gc(void*v,StarsPoint*p){(void)v;p->x=4;p->y=8;}
static void sm(void*v,StarsU16 h,StarsPoint*p){(void)v;(void)h;(void)p;}
static void sc(void*v,StarsU16 c){T*t=(T*)v;if(c==33)++t->cursor;}
static void in(void*v,StarsU16 h,StarsU16 m){T*t=(T*)v;(void)h;if(m==0)++t->init;}
static void st(void*v,StarsU16 h,StarsI16 on){T*t=(T*)v;(void)h;(void)on;++t->sticky;}
static void ad(void*v){++((T*)v)->advance;}
static void ed(void*v,StarsU16 h,StarsI16 r){T*t=(T*)v;(void)h;t->result=r;++t->ended;}
static void hl(void*v,StarsU16 h,const StarsU8*f,StarsU16 c,StarsU16 id){T*t=(T*)v;(void)h;(void)f;(void)c;if(id==0x455)++t->help;}
static void iv(void*v,StarsU16 h){T*t=(T*)v;(void)h;++t->invalidated;}
static StarsI16 gs(void*v,StarsU16 id,StarsU8*out,StarsU16 n){T*t=(T*)v;(void)id;++t->strings;if(n>1){out[0]='X';out[1]=0;}return 1;}
static StarsI16 pm(void*v,StarsU16 h,StarsI16 x,StarsI16 y,StarsU16 n,const StarsU8*names,const StarsU16*ids,StarsI16 cur,StarsI16 right){T*t=(T*)v;(void)h;(void)x;(void)y;(void)names;(void)ids;(void)cur;(void)right;if(n==8)++t->popup;return 3;}
int main(void){StarsScoreXOps o;StarsScoreXInput i;T t;int f=0;memset(&o,0,sizeof o);memset(&i,0,sizeof i);memset(&t,0,sizeof t);o.begin_paint=bp;o.end_paint=ep;o.draw_history=drh;o.draw_score=drs;o.draw_victory=drv;o.get_client_rect=gr;o.fill_rect=fi;o.get_cursor_pos=gc;o.screen_to_client=sm;o.set_cursor=sc;o.init_dialog=in;o.sticky_position=st;o.advance_tutor=ad;o.end_dialog=ed;o.help=hl;o.invalidate=iv;o.get_string=gs;o.popup_menu=pm;i.mode=0;i.tutor_active=1;i.font_height=8;i.font_height_large=10;i.hand_cursor=33;i.button_face_brush=7;i.help_file=(const StarsU8*)"H";i.ops=&o;i.context=&t;if(stars_score_x_dlg(&i,4,0x110,0,0)!=1||i.dialog_window!=4||i.in_dialog!=1||t.init!=1||t.sticky!=1||t.advance!=1)++f;if(stars_score_x_dlg(&i,4,0x0f,0,0)!=1||t.score!=1||t.begin!=1||t.end!=1)++f;if(stars_score_x_dlg(&i,4,0x111,0xc6,0)!=1||i.mode!=1||t.victory!=0||t.invalidated!=1)++f;if(stars_score_x_dlg(&i,4,0x111,0x76,0)!=1||t.help!=1)++f;if(stars_score_x_dlg(&i,4,0x111,2,0)!=1||i.in_dialog!=0||i.dialog_window!=0||t.ended!=1||t.result!=0)++f;i.mode=2;i.history_selection=1;i.history_flags=0;if(stars_score_x_dlg(&i,4,0x20,0,0)!=1||t.cursor!=1)++f;if(stars_score_x_dlg(&i,4,0x201,0,0x00080004UL)!=0||t.strings!=8||t.popup!=1||i.history_selection!=3||i.history_flags!=0x80)++f;
#ifndef TARGET_NATIVE
    printf("SCORE_X_DLG_CHECKS=7 FAILURES=%d RESULT=%s\n",f,f?"FAIL":"PASS");
#endif
    return f!=0;}
