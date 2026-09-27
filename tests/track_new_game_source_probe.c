#include <stdio.h>
#include "core.h"
typedef struct Trace { StarsI16 hit, value, step, gets, sets, draws, track_calls; StarsU16 msg, hdc; } Trace;
static StarsI16 hit(void *c, StarsPoint p){(void)p;return ((Trace *)c)->hit;}
static void init(void *c, StarsU16 w, StarsU16 h, StarsU16 m, StarsU16 f, StarsU16 z){Trace*t=(Trace*)c;(void)w;(void)h;(void)f;(void)z;t->msg=m;}
static StarsI16 track(void *c, StarsU16 w, StarsU16 *hdc){Trace*t=(Trace*)c;(void)w;*hdc=t->hdc;return t->track_calls++<2?1:0;}
static StarsI16 get(void *c, StarsI16 v, StarsI16 raw){Trace*t=(Trace*)c;(void)v;(void)raw;++t->gets;return t->value;}
static StarsI16 set(void *c, StarsI16 v, StarsI16 value){Trace*t=(Trace*)c;(void)v;t->value=value;++t->sets;return value;}
static void draw(void *c, StarsU16 w, StarsU16 h, StarsI16 v){Trace*t=(Trace*)c;(void)w;(void)h;(void)v;++t->draws;}
int main(void){StarsTrackNewGameDlg3Ops o;Trace t;StarsPoint p={0,0};t.hit=4;t.value=10;t.step=1;t.gets=0;t.sets=0;t.draws=0;t.track_calls=0;t.msg=0;t.hdc=33;o.hit_test=hit;o.init_track=init;o.track_button=track;o.get_value=get;o.set_value=set;o.draw=draw;if(stars_track_new_game_dlg3(9,p,0,&o,&t)!=1||t.value!=12||t.msg!=0xa0U||t.sets!=2||t.draws!=2)return 1;t.hit=5;t.value=10;t.gets=0;t.sets=0;t.draws=0;t.track_calls=0;if(stars_track_new_game_dlg3(9,p,4,&o,&t)!=1||t.value!=0||t.msg!=0xa1U)return 2;t.hit=-1;if(stars_track_new_game_dlg3(9,p,0,&o,&t)!=0)return 3;puts("TRACK_NEW_GAME_SOURCE=PASS");return 0;}
