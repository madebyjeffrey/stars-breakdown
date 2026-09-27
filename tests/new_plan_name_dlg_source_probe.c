#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace {
    StarsI16 rects, fills, backgrounds, positions, messages, sets, gets;
    StarsI16 ends, helps, end_result;
    StarsU16 fill_dc, brush, position_window, position_insert;
    StarsI16 x, y, message, message_wparam;
    StarsU32 background, message_lparam;
    StarsU8 *set_text;
    StarsU8 *get_text;
    StarsU16 get_count;
    const StarsU8 *help;
} Trace;
static void get_rect(void *v, StarsU16 w, StarsRect *r){Trace*t=(Trace*)v;t->rects++;r->left=w;r->top=2;r->right=3;r->bottom=4;}
static void fill(void *v, StarsU16 dc, const StarsRect *r, StarsU16 b){Trace*t=(Trace*)v;t->fills++;t->fill_dc=dc;t->brush=b;(void)r;}
static void bgcolor(void *v, StarsU16 dc, StarsU32 c){Trace*t=(Trace*)v;t->backgrounds++;t->fill_dc=dc;t->background=c;}
static void pos(void *v, StarsU16 w, StarsU16 ins, StarsI16 x, StarsI16 y, StarsI16 cx, StarsI16 cy, StarsU16 f){Trace*t=(Trace*)v;t->positions++;t->position_window=w;t->position_insert=ins;t->x=x;t->y=y;t->message=(StarsI16)f;(void)cx;(void)cy;}
static void msg(void *v, StarsU16 w, StarsU16 id, StarsU16 m, StarsU16 wp, StarsU32 lp){Trace*t=(Trace*)v;t->messages++;t->position_window=w;t->message=(StarsI16)m;t->message_wparam=(StarsI16)wp;t->message_lparam=lp;(void)id;}
static void settext(void *v, StarsU16 w, StarsU16 id, const StarsU8 *s){Trace*t=(Trace*)v;t->sets++;t->set_text=(StarsU8*)s;(void)w;(void)id;}
static void gettext(void *v, StarsU16 w, StarsU16 id, StarsU8 *s, StarsU16 n){Trace*t=(Trace*)v;t->gets++;t->get_text=s;t->get_count=n;if(n>1){s[0]=(StarsU8)'N';s[1]=0;}(void)w;(void)id;}
static void enddlg(void *v, StarsU16 w, StarsI16 r){Trace*t=(Trace*)v;t->ends++;t->end_result=r;(void)w;}
static void help(void *v, StarsU16 w, const StarsU8 *s, StarsU16 c, StarsU16 d){Trace*t=(Trace*)v;t->helps++;t->help=s;t->message=(StarsI16)c;t->message_wparam=(StarsI16)d;(void)w;}
int main(void){StarsNewPlanNameDlgOps o;StarsNewPlanNameDlgInput i;StarsPoint p;Trace t;StarsU8 name[32];memset(&o,0,sizeof o);memset(&i,0,sizeof i);memset(&t,0,sizeof t);memset(name,0,sizeof name);p.x=10;p.y=20;i.button_face_brush=8;i.button_face_color=0x12345678UL;i.sticky_point=&p;i.plan_name=name;i.dirty_plan=&t.end_result;i.help_file=(const StarsU8*)"HELP";i.ops=&o;i.context=&t;o.get_client_rect=get_rect;o.fill_rect=fill;o.set_background_color=bgcolor;o.set_window_pos=pos;o.send_item_message=msg;o.set_item_text=settext;o.get_item_text=gettext;o.end_dialog=enddlg;o.win_help=help;if(stars_new_plan_name_dlg(9,0x14,7,0,&i)!=1||t.rects!=1||t.fills!=1||t.fill_dc!=7||t.brush!=8)return 1;if(stars_new_plan_name_dlg(9,0x19,7,0x00060000UL,&i)!=8||t.backgrounds!=1||t.background!=0x12345678UL)return 2;if(stars_new_plan_name_dlg(9,0x110,0,0,&i)!=1||t.positions!=1||t.x!=0x50||t.y!=0x5a||t.message!=0x415||t.messages!=1||t.message_wparam!=0x1f||t.set_text!=name||t.sets!=1)return 3;t.end_result=0;if(stars_new_plan_name_dlg(9,0x111,1,0,&i)!=1||t.gets!=1||t.get_count!=0x20||t.ends!=1||t.end_result!=1||name[0]!='N')return 4;t.end_result=0;if(stars_new_plan_name_dlg(9,0x111,2,0,&i)!=1||t.ends!=2||t.end_result!=0)return 5;if(stars_new_plan_name_dlg(9,0x111,0x76,0,&i)!=1||t.helps!=1||strcmp((const char*)t.help,"HELP")!=0||t.message!=1||t.message_wparam!=0x439)return 6;if(stars_new_plan_name_dlg(9,0x111,3,0,&i)!=0||stars_new_plan_name_dlg(9,0x20,0,0,&i)!=0){return 7;}puts("NEW_PLAN_NAME_DLG_SOURCE=PASS");return 0;}
