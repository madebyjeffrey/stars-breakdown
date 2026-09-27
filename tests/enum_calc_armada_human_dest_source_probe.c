#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI32 distance; StarsI16 calls,flags_calls; } Trace;
static StarsI32 dist(void *v,StarsPoint a,StarsPoint b){Trace*t=(Trace*)v;t->calls++;(void)a;(void)b;return t->distance;}
static StarsI16 random4(void *v,StarsI16 n){(void)v;(void)n;return 0;}
static StarsU16 flags(void *v,StarsI16 player){Trace*t=(Trace*)v;t->flags_calls++;return player==2?0:0x0200;}
int main(void){StarsU8 ai[0x30];StarsPoint points[3];StarsArmadaPlanet source,test;StarsArmadaDestinationInput i;Trace t;memset(ai,0,sizeof ai);memset(points,0,sizeof points);memset(&i,0,sizeof i);memset(&t,0,sizeof t);ai[0x1a]=10;source.id=0;source.player=0;source.segment=1;test.id=1;test.player=2;test.segment=2;i.ai_planets=ai;i.plan_points=points;i.distance=dist;i.random=random4;i.player_flags=flags;i.context=&t;t.distance=100;if(stars_enum_calc_armada_human_dest(&source,&test,&i)!=17||t.calls!=1||t.flags_calls!=1)return 1;test.player=1;if(stars_enum_calc_armada_human_dest(&source,&test,&i)!=0||t.calls!=1||t.flags_calls!=2)return 2;test.player=2;test.segment=source.segment;if(stars_enum_calc_armada_human_dest(&source,&source,&i)!=0)return 3;ai[0x1a]=0;if(stars_enum_calc_armada_human_dest(&source,&test,&i)!=0||t.calls!=1)return 4;puts("ENUM_CALC_ARMADA_HUMAN_DEST_SOURCE=PASS");return 0;}
