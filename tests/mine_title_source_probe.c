#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct Probe { int invalidates,messages,positions; char title[128]; StarsI16 y,w,h; StarsU16 flags; } Probe;
static void get_string(void *c,StarsU16 id,StarsU8 *out,StarsU16 n){(void)c;if(n){strncpy((char *)out,id==0x554?"Type":"Nothing",n-1);out[n-1]=0;}}
static void get_name(void *c,StarsI16 kind,StarsI16 id,StarsU8 *out,StarsU16 n){(void)c;(void)kind;(void)id;if(n){strncpy((char *)out,"-Name",n-1);out[n-1]=0;}}
static void set_title(void *c,const StarsU8 *s){Probe *p=(Probe *)c;strncpy(p->title,(const char *)s,sizeof(p->title)-1);}
static void invalidate(void *c,StarsU16 h,StarsI16 e){Probe *p=(Probe *)c;(void)h;(void)e;++p->invalidates;}
static void message(void *c,StarsU16 h,StarsU16 m,StarsU16 w){Probe *p=(Probe *)c;(void)h;(void)m;(void)w;++p->messages;}
static void client(void *c,StarsU16 h,StarsRect *r){(void)c;(void)h;r->left=0;r->top=0;r->right=200;r->bottom=100;}
static void position(void *c,StarsU16 h,StarsI16 x,StarsI16 y,StarsI16 w,StarsI16 ht,StarsU16 f){Probe *p=(Probe *)c;(void)h;(void)x;p->y=y;p->w=w;p->h=ht;p->flags=f;++p->positions;}
int main(void)
{
    StarsMineralTitleState s; StarsMineralTitleOps o; Probe p; int failures=0;
    memset(&s,0,sizeof(s));memset(&o,0,sizeof(o));memset(&p,0,sizeof(p));s.grobj_full=8;s.grobj=8;s.planet=-1;s.fleet=-1;s.thing=3;s.player=2;s.thing_owner=2;s.race_stat=5;s.thing_cargo=1;
    o.get_string=get_string;o.get_name=get_name;o.set_title=set_title;o.invalidate=invalidate;o.send_message=message;o.get_client_rect=client;o.set_window_pos=position;
    stars_set_mineral_title_bar(1,2,10,&s,&o,&p);if(strcmp(p.title,"Type-Name")!=0||p.invalidates!=1||p.messages!=1||p.flags!=0x44||p.y!=80||p.w!=112||p.h!=14)++failures;
    s.thing_flags=0x2000;stars_set_mineral_title_bar(1,2,10,&s,&o,&p);if(p.messages!=1||p.flags!=0x84)++failures;
    s.grobj=1;s.planet=4;stars_set_mineral_title_bar(1,2,10,&s,&o,&p);if(strcmp(p.title,"Type-Name")!=0)++failures;
    printf("MINE_TITLE_CHECKS=3 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");return failures!=0;
}
