#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 teeth,bombs,tanker,maxstat,lookups,plans; StarsI16 classification; } Trace;
static StarsU8 ship[1];
static StarsU8 *lookup_ship(void *v,StarsI16 owner,StarsI16 design){Trace*t=(Trace*)v;t->lookups++;if(owner!=3||design!=5)return (StarsU8*)0;return ship;}
static const StarsU8 *lookup_plan(void *v,StarsI16 owner,StarsI16 plan){Trace*t=(Trace*)v;t->plans++;if(owner!=3||plan!=2)return (const StarsU8*)0;return (const StarsU8*)v+sizeof(Trace);}
static StarsI16 teeth(void *v,const StarsU8 *s){Trace*t=(Trace*)v;t->teeth++;(void)s;return t->classification==3;}
static StarsI16 bombs(void *v,const StarsU8 *s){Trace*t=(Trace*)v;t->bombs++;(void)s;return t->classification==4;}
static StarsI16 tanker(void *v,const StarsU8 *s){Trace*t=(Trace*)v;t->tanker++;(void)s;return t->classification==6;}
static StarsI16 maxstat(void *v,const StarsU8 *s,StarsI16 stat){Trace*t=(Trace*)v;t->maxstat++;(void)s;(void)stat;return t->classification==7;}
static void run_case(StarsI16 cls,StarsU16 plan_kind,StarsU16 *out){StarsU8 storage[sizeof(Trace)+6],plan[6];StarsToken token;StarsCheckTargetFleet fleet;StarsCheckTargetInput input;Trace*t=(Trace*)storage;memset(storage,0,sizeof storage);memset(plan,0,sizeof plan);memset(&token,0,sizeof token);t->classification=cls;plan[1]=(StarsU8)plan_kind;plan[4]=0x32;memcpy(storage+sizeof(Trace),plan,sizeof plan);fleet.flags=(StarsU16)(3<<9);fleet.plan=2;fleet.ship_design=5;memset(&input,0,sizeof input);input.ship_definition=lookup_ship;input.battle_plan=lookup_plan;input.has_teeth=teeth;input.has_bombs=bombs;input.fuel_tanker=tanker;input.max_stat=maxstat;input.context=t;stars_check_target(&token,&fleet,&input);*out=(StarsU16)(token.raw[0x17]|((StarsU16)token.raw[0x18]<<8));}
int main(void){StarsU16 value;run_case(3,3,&value);if(value!=0x3332)return 1;run_case(4,0,&value);if(value!=0x4032)return 2;run_case(6,0,&value);if(value!=0x6032)return 3;run_case(7,0,&value);if(value!=0x7032)return 4;run_case(0,0,&value);if(value!=0x5032)return 5;puts("CHECK_TARGET_SOURCE=PASS");return 0;}
