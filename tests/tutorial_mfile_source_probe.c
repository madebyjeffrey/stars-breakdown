#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T { StarsU16 id, type, writes, bytes; } T;
static const StarsU8 blob[] = { 3, 0, 0, 0, 32, 1, 0, 85, 0, 32, 2, 0, 86, 87, 0, 32 };
static StarsU16 fr(void*c,StarsU16 id,StarsU16 type){T*t=(T*)c;t->id=id;t->type=type;return 7;}
static StarsU16 lr(void*c,StarsU16 h){(void)c;return h;}
static const StarsTutorialResource *lk(void*c,StarsU16 h){static StarsTutorialResource r={(const StarsU8*)blob,(StarsU16)sizeof(blob)};(void)c;(void)h;return &r;}
static void ur(void*c,StarsU16 h){(void)c;(void)h;}
static void xr(void*c,StarsU16 h){(void)c;(void)h;}
static StarsI16 fn(void*c,StarsU8*n,StarsU16 z){(void)c;(void)z;strcpy((char*)n,"TUTOR.M");return 1;}
static StarsI16 op(void*c,const StarsU8*n,StarsU16 m){(void)c;(void)n;(void)m;return 1;}
static void wr(void*c,const StarsU8*d,StarsU16 n){T*t=(T*)c;t->writes++;t->bytes=(StarsU16)(t->bytes+n);(void)d;}
static void cl(void*c){(void)c;}
static int test(void){StarsWriteTutorialMFileInput i;T t;int f=0;memset(&t,0,sizeof(t));i.turn=1;i.find_resource=fr;i.load_resource=lr;i.lock_resource=lk;i.unlock_resource=ur;i.free_resource=xr;i.make_filename=fn;i.open_stream=op;i.write_stream=wr;i.close_stream=cl;i.context=&t;if(stars_write_tutorial_m_file(&i)!=1||t.id!=0x2713U||t.type!=0x2712U||t.writes!=2U||t.bytes!=5U)++f;i.turn=3;if(stars_write_tutorial_m_file(&i)!=2)++f;i.turn=32;if(stars_write_tutorial_m_file(&i)!=1||t.id!=0x2715U||t.type!=0x2714U)++f;return f;}
int main(void){int f=test();if(f)return f;puts("TUTORIAL_MFILE_SOURCE=PASS");return 0;}
