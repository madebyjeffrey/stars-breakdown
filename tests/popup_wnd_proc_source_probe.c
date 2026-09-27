#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T{int rect,fill,begin,draw,end,destroy,release,tutor,def;StarsU16 dc,brush,window;StarsI32 result;}T;
static void gr(void*v,StarsU16 h,StarsRect*r){T*t=(T*)v;(void)h;r->left=1;r->top=2;r->right=3;r->bottom=4;++t->rect;}
static void fi(void*v,StarsU16 h,const StarsRect*r,StarsU16 b){T*t=(T*)v;(void)h;(void)r;t->brush=b;++t->fill;}
static StarsU16 bp(void*v,StarsU16 h,StarsRect*r){T*t=(T*)v;(void)h;(void)r;t->dc=44;++t->begin;return t->dc;}
static void dr(void*v,StarsU16 h,StarsU16 d){T*t=(T*)v;(void)h;if(d==44)++t->draw;}
static void ep(void*v,StarsU16 h,const StarsRect*r){T*t=(T*)v;(void)h;(void)r;++t->end;}
static void de(void*v,StarsU16 h){T*t=(T*)v;t->window=h;++t->destroy;}
static void rc(void*v){++((T*)v)->release;}
static void tu(void*v){++((T*)v)->tutor;}
static StarsI32 df(void*v,StarsU16 h,StarsU16 m,StarsU16 w,StarsU32 l){T*t=(T*)v;(void)h;(void)m;(void)w;(void)l;t->result=99;++t->def;return t->result;}
int main(void){StarsPopupWndProcOps o;StarsPopupWndProcInput i;T t;memset(&o,0,sizeof o);o.get_client_rect=gr;o.fill_rect=fi;o.begin_paint=bp;o.draw_popup=dr;o.end_paint=ep;o.destroy_window=de;o.release_capture=rc;o.advance_tutor=tu;o.default_proc=df;memset(&t,0,sizeof t);memset(&i,0,sizeof i);i.popup_window=77;i.popup_kind=9;i.button_face_brush=5;i.game_flags=0x0800;i.tutor_flags=0x8000;i.ops=&o;i.context=&t;if(stars_popup_wnd_proc(&i,8,0x14,6,0)!=1||t.rect!=1||t.fill!=1||t.brush!=5)return 1;if(stars_popup_wnd_proc(&i,8,0x0f,0,0)!=0||t.begin!=1||t.draw!=1||t.end!=1)return 2;if(stars_popup_wnd_proc(&i,8,0x202,0,0)!=0||i.popup_window!=0||i.popup_kind!=0||t.destroy!=1||t.window!=77||t.release!=1||t.tutor!=1||i.tutor_flags!=0x8400)return 3;if(stars_popup_wnd_proc(&i,8,0x123,2,3)!=99||t.def!=1)return 4;printf("POPUP_WND_PROC_CHECKS=4 FAILURES=0 RESULT=PASS\n");return 0;}
