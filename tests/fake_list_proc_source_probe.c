#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { int cursor, calls, track, sends, lookup, popup; StarsU16 cursor_id; StarsU16 msg[4]; StarsU16 part0; StarsU16 part1; StarsU16 popup_mode; } Trace;
static void pos(void *v, StarsU16 h, StarsPoint *p){Trace*t=(Trace*)v;(void)h;p->x=10;p->y=20;++t->cursor;}
static void client(void *v, StarsU16 h, StarsPoint *p){(void)v;(void)h;p->x=(StarsI16)(p->x-2);p->y=(StarsI16)(p->y-3);}
static void cursor(void *v, StarsU16 c){Trace*t=(Trace*)v;t->cursor_id=c;}
static StarsI32 proc(void *v, StarsU32 p, StarsU16 h, StarsU16 m, StarsU32 l){Trace*t=(Trace*)v;(void)p;(void)h;(void)l;t->msg[t->calls++]=m;return 77;}
static void track(void *v, StarsU16 h, StarsU16 x, StarsI16 y, StarsU16 w, StarsI16 list, StarsI16 right){Trace*t=(Trace*)v;(void)h;(void)x;(void)y;(void)w;(void)list;(void)right;++t->track;}
static StarsU32 send(void *v, StarsU16 h, StarsU16 m, StarsU16 w, StarsU32 l){Trace*t=(Trace*)v;(void)h;(void)w;(void)l;++t->sends;return m==0x409U?3:0;}
static void lookup(void *v, StarsU8 *p, StarsU16 n){Trace*t=(Trace*)v;(void)n;t->part0=p[0];t->part1=p[1];++t->lookup;}
static void popup(void *v, StarsU16 h, StarsU16 x, StarsI16 y){Trace*t=(Trace*)v;(void)h;(void)x;(void)y;t->popup_mode=9;++t->popup;}
int main(void){StarsFakeListProcOps o;StarsFakeListProcInput i;Trace t;StarsI32 r;memset(&t,0,sizeof t);memset(&i,0,sizeof i);o.get_cursor_pos=pos;o.screen_to_client=client;o.set_cursor=cursor;o.call_window_proc=proc;o.track_slot=track;o.send_message=send;o.lookup_part=lookup;o.popup=popup;i.build_mode=4;i.hand_cursor=55;i.work[0]=1;i.work[1]=0x44;i.real_list_proc=99;i.ops=&o;i.context=&t;r=stars_fake_list_proc(&i,7,0x20,0,0);if(r!=1||t.cursor!=1||t.cursor_id!=55)return 1;r=stars_fake_list_proc(&i,7,0x201,4,0x00020010UL);if(r!=0||t.track!=1||t.calls!=2||t.msg[0]!=0x201||t.msg[1]!=0x202)return 2;r=stars_fake_list_proc(&i,7,0x204,4,0x00030020UL);if(r!=0||t.sends!=2||t.lookup!=1||t.popup!=1||i.global_popup!=9||t.part0!=1||t.part1!=3)return 3;printf("FAKE_LIST_PROC_CHECKS=3 FAILURES=0 RESULT=PASS\n");return 0;}
