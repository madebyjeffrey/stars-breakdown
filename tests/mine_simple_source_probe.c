#include "core.h"
#include <stdio.h>
#include <string.h>
typedef struct Probe { int select,blt,del,pat; StarsI16 x,y; } Probe;
static StarsU16 create_dc(void *c,StarsU16 h){(void)c;(void)h;return 9;}
static StarsU16 select_object(void *opaque,StarsU16 dc,StarsU16 obj){Probe *p=(Probe *)opaque;(void)dc;++p->select;return (StarsU16)(obj+1);}
static void bit_blt(void *opaque,StarsU16 dc,StarsI16 x,StarsI16 y,StarsI16 w,StarsI16 h,StarsU16 src,StarsI16 sx,StarsI16 sy,StarsU32 rop){Probe *p=(Probe *)opaque;(void)dc;(void)w;(void)h;(void)src;(void)sx;(void)sy;(void)rop;++p->blt;p->x=x;p->y=y;}
static void delete_dc(void *opaque,StarsU16 dc){Probe *p=(Probe *)opaque;(void)dc;++p->del;}
static void pat_blt(void *opaque,StarsU16 dc,StarsI16 x,StarsI16 y,StarsI16 w,StarsI16 h,StarsU32 rop){Probe *p=(Probe *)opaque;(void)dc;(void)x;(void)y;(void)w;(void)h;(void)rop;++p->pat;}
int main(void)
{
    StarsMineThing things[4]; StarsI16 matching,total; StarsRect r={0,0,20,20};
    StarsSelectionArrowOps a; StarsDiamondOps d; Probe p; int failures=0;
    memset(&p,0,sizeof(p)); things[0].raw=0x0400;things[1].raw=0x0401;things[2].raw=0x2400;things[3].raw=0x0200;
    stars_get_mine_field_counts(things,4,2,0x0400,&matching,&total); if(matching!=1||total!=2)++failures;
    memset(&p,0,sizeof(p));memset(&a,0,sizeof(a));a.create_dc=create_dc;a.select_object=select_object;a.bit_blt=bit_blt;a.delete_dc=delete_dc;stars_draw_selection_arrow(3,&r,0,7,&a,&p);if(p.blt!=2||p.select!=2||p.del!=1||p.x!=5||p.y!=5)++failures;
    memset(&p,0,sizeof(p));memset(&d,0,sizeof(d));d.select_object=select_object;d.pat_blt=pat_blt;stars_draw_diamond(3,&r,7,8,9,&d,&p);if(p.select!=4||p.pat!=50)++failures;
    printf("MINE_SIMPLE_CHECKS=3 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");return failures!=0;
}
