#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T { StarsU16 strings, names, unlinks; StarsU32 seed; StarsI16 generated; } T;
static StarsI16 gs(void*c,StarsU16 id,StarsU8*d,StarsU16 n){T*t=(T*)c;(void)n;if(id==0x567U||id==0x56eU)++t->strings;if(id==0x1e7U)++t->names;d[0]='X';d[1]=0;return 1;}
static void fn(void*c,StarsU8*d,StarsU16 id){T*t=(T*)c;(void)d;if(id==0x5a22U)++t->names;}
static void ul(void*c,StarsU16 id,StarsI16 p){T*t=(T*)c;(void)id;(void)p;++t->unlinks;}
static void rn(void*c,StarsU32 s){T*t=(T*)c;t->seed=s;}
static void gw(void*c,StarsI16 b){T*t=(T*)c;t->generated=b;}
int main(void){StarsCreateTutorWorldInput i;StarsU16 pc=9,de=9,sz=9,sd=9,gf=0x0017;StarsU32 lid=0;StarsU8 vc[16],p0[192],p1[192],d[192],cp[192],gn[64],n[64],ns[64],n1[64];T t;memset(&t,0,sizeof(t));memset(vc,0,sizeof(vc));memset(d,3,sizeof(d));memset(cp,4,sizeof(cp));cp[6]=0xff;cp[7]=0xff;i.player_count=&pc;i.density=&de;i.size=&sz;i.start_distance=&sd;i.game_flags=&gf;i.game_lid=&lid;i.victory_conditions=vc;i.player0=p0;i.player1=p1;i.default_player=d;i.competitor_player=cp;i.game_name=gn;i.player0_name=n;i.player0_names=ns;i.player1_name=n1;i.get_string=gs;i.format_names=fn;i.unlink_file=ul;i.randomize=rn;i.generate_world=gw;i.context=&t;stars_create_tutor_world(&i);if(pc!=2U||de!=0U||sz!=0U||sd!=1U||gf!=0xffU||lid!=0x008cef49UL||vc[7]!=0x80U||vc[8]!=0x81U||p0[0]!=3U||p1[0]!=4U||p1[6]!=0xffU||p1[7]!=0x23U||t.strings!=2U||t.names!=2U||t.unlinks!=4U||t.seed!=0x499602d2UL||t.generated!=0)return 1;puts("CREATE_TUTOR_WORLD_SOURCE=PASS");return 0;}
