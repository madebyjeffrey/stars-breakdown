#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct Probe { int invalidates, releases; StarsRect r[2]; StarsI16 erase[2]; } Probe;
static void client(void *c,StarsU16 h,StarsRect *r){(void)c;(void)h;r->left=0;r->top=0;r->right=200;r->bottom=100;}
static StarsU16 dc(void *c,StarsU16 h){(void)c;(void)h;return 4;}
static StarsU16 font(void *c,StarsU16 d,StarsU16 f){(void)c;(void)d;return (StarsU16)(f+10);}
static StarsI16 width(void *c,StarsU16 d,StarsU16 id,StarsU16 n){(void)c;(void)d;(void)id;return (StarsI16)n;}
static void release(void *c,StarsU16 h,StarsU16 d){Probe *p=(Probe *)c;(void)h;(void)d;++p->releases;}
static void invalidate(void *c,StarsU16 h,const StarsRect *r,StarsI16 erase){Probe *p=(Probe *)c;(void)h;p->r[p->invalidates]=*r;p->erase[p->invalidates]=erase;++p->invalidates;}
int main(void)
{
    StarsMineralBarsOps o; Probe p; int failures=0; memset(&o,0,sizeof(o));memset(&p,0,sizeof(p));
    o.get_client_rect=client;o.get_dc=dc;o.select_font=font;o.text_width=width;o.release_dc=release;o.invalidate_rect=invalidate;
    stars_invalidate_mineral_bars(8,10,4,&o,&p);
    if(p.releases!=1||p.invalidates!=2||p.erase[0]!=1||p.erase[1]!=0)++failures;
    if(p.r[0].left!=172||p.r[0].top!=2||p.r[0].right!=199||p.r[0].bottom!=12)++failures;
    if(p.r[1].left!=0||p.r[1].top!=72||p.r[1].right!=199||p.r[1].bottom!=102)++failures;
    printf("MINE_BARS_CHECKS=3 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");return failures!=0;
}
