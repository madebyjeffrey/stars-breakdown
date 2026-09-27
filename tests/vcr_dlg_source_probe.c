#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 paints,draws,timers,kills,ends,helps,popups,focus,resizes; } Trace;
static void one(void *c){Trace*t=(Trace*)c;++t->paints;}
static void draw(void *c,StarsI16 a,StarsI16 b){Trace*t=(Trace*)c;(void)a;(void)b;++t->draws;}
static void resize(void *c,StarsU16 w){Trace*t=(Trace*)c;(void)w;++t->resizes;}
static void timer(void *c,StarsI16 a,StarsI16 b){Trace*t=(Trace*)c;(void)a;(void)b;++t->timers;}
static void kill(void *c){Trace*t=(Trace*)c;++t->kills;}
static void end(void *c){Trace*t=(Trace*)c;++t->ends;}
static StarsI16 popup(void *c,StarsI16 x,StarsI16 y,StarsU16 s){Trace*t=(Trace*)c;(void)x;(void)y;(void)s;++t->popups;return 3;}
static void focus(void *c,StarsU16 s){Trace*t=(Trace*)c;(void)s;++t->focus;}
int main(void)
{
    StarsVcrDlgInput i; Trace t; memset(&i,0,sizeof i); memset(&t,0,sizeof t);
    i.final_step=5;i.current_step=-1;i.speed=2;i.board_square=32;i.context=&t;
    i.ops.paint=one;i.ops.draw=draw;i.ops.resize=resize;i.ops.set_timer=timer;
    i.ops.kill_timer=kill;i.ops.end_dialog=end;i.ops.popup=popup;i.ops.focus_square=focus;
    if(stars_vcr_dlg(4,0x110,0,0,&i)!=1||t.resizes!=1)return 1;
    if(stars_vcr_dlg(4,0x111,0xa4,0,&i)!=1||i.current_step!=0)return 2;
    if(stars_vcr_dlg(4,0x111,0xa4,0,&i)!=1||i.current_step!=1)return 3;
    if(stars_vcr_dlg(4,0x111,0xa3,0,&i)!=1||t.timers!=1)return 4;
    if(stars_vcr_dlg(4,0x113,0,0,&i)!=0||i.current_step!=2)return 5;
    if(stars_vcr_dlg(4,0x204,0,((StarsU32)50<<16)|50,&i)!=0||t.popups!=1)return 6;
    if(stars_vcr_dlg(4,2,0,0,&i)!=0||t.ends!=1)return 7;
    puts("VCR_DLG_SOURCE=PASS");return 0;
}
