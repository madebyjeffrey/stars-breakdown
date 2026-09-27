#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsU8 plan[4],planet[12],salvage[12]; StarsI16 owner,planet_id; StarsPoint point; StarsI16 salvage_calls,add_calls; } Trace;
static const StarsU8 *plan(void *v,StarsI16 owner,StarsI16 index){Trace*t=(Trace*)v;t->owner=owner;if(index!=2)return (const StarsU8*)0;return t->plan;}
static StarsU8 *find_planet(void *v,StarsI16 id){Trace*t=(Trace*)v;t->planet_id=id;return t->planet;}
static void salvage(void *v,const StarsU8 *m,StarsI16 owner,const StarsPoint *p){Trace*t=(Trace*)v;t->salvage_calls++;t->owner=owner;t->point=*p;memcpy(t->salvage,m,12);}
static void add(void *v,StarsU8 *p,const StarsU8 *m){Trace*t=(Trace*)v;StarsI16 n;t->add_calls++;for(n=0;n<12;n++)p[n]=(StarsU8)(p[n]+m[n]);}
static void setup(StarsDumpCargoInput *i,Trace *t){memset(i,0,sizeof *i);i->fleet_flags=(StarsU16)(3<<9);i->plan_index=2;i->battle_plan=plan;i->find_planet=find_planet;i->drop_salvage=salvage;i->add_minerals=add;i->context=t;t->plan[3]=0x80;}
int main(void){StarsDumpCargoInput i;Trace t;StarsU8 expected[12];memset(expected,0,sizeof expected);expected[0]=1;expected[4]=2;expected[8]=3;memset(&t,0,sizeof t);setup(&i,&t);i.planet=-1;i.point.x=4;i.point.y=5;i.minerals[0]=1;i.minerals[4]=2;i.minerals[8]=3;if(stars_dump_cargo(&i)!=1||t.salvage_calls!=1||t.owner!=3||t.point.x!=4||t.point.y!=5||memcmp(t.salvage,expected,12)!=0||i.minerals[0]!=0||i.minerals[4]!=0||i.minerals[8]!=0)return 1;memset(&t,0,sizeof t);setup(&i,&t);i.planet=7;t.planet[0]=0xff;t.planet[1]=0xff;i.minerals[0]=1;i.minerals[1]=1;if(stars_dump_cargo(&i)!=1||t.add_calls!=1||t.planet_id!=7||t.planet[0]!=0||i.minerals[0]!=0||i.minerals[1]!=0)return 2;memset(&t,0,sizeof t);setup(&i,&t);i.planet=-1;if(stars_dump_cargo(&i)!=0||t.salvage_calls!=0)return 3;memset(&t,0,sizeof t);setup(&i,&t);i.planet=-1;i.plan_index=1;i.minerals[0]=1;if(stars_dump_cargo(&i)!=0||i.minerals[0]!=1)return 4;puts("DUMP_CARGO_SOURCE=PASS");return 0;}
