#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { int frees, lookups, fills, draws, tutor; StarsU8 *freed[3]; StarsI16 players[2]; StarsU16 draw_flag; } Trace;
static void free_p(void*v,StarsU8*p){Trace*t=(Trace*)v;if(t->frees<3)t->freed[t->frees]=p;++t->frees;}
static void lookup(void*v,StarsI16 p,StarsU8*x){Trace*t=(Trace*)v;(void)x;if(t->lookups<2)t->players[t->lookups]=p;++t->lookups;}
static void fill(void*v){++((Trace*)v)->fills;}
static void draw(void*v,StarsU16 h,StarsU16 f){Trace*t=(Trace*)v;(void)h;t->draw_flag=f;++t->draws;}
static void tutor(void*v){++((Trace*)v)->tutor;}
int main(void){StarsFinishProductionOps o;StarsFinishProductionInput i;Trace t;StarsU8 a,b;memset(&t,0,sizeof t);memset(&i,0,sizeof i);o.free_product=free_p;o.lookup_planet=lookup;o.fill_list=fill;o.draw_ship=draw;o.advance_tutor=tutor;i.write=0;i.planet_bits=0x20UL;i.game_flags=0x0820;i.player=0;i.selected_product=&a;i.global_product=&b;i.global_count=1;i.ops=&o;i.context=&t;stars_finish_production(&i);if((i.planet_bits&0x00800000UL)==0||t.frees!=1||t.freed[0]!=&b||t.tutor!=1||i.game_flags!=0x0820||i.tutor_flags!=0x0400)return 1;memset(&t,0,sizeof t);i.write=1;i.player=1;i.planet_id=24;i.game_flags=0;i.selected_product=&a;i.global_product=&b;i.global_count=1;stars_finish_production(&i);if(t.frees!=1||t.freed[0]!=&a||t.lookups!=2||t.players[0]!=-1||t.players[1]!=24||t.fills!=1||t.draws!=1||t.draw_flag!=0x40||i.selected_product!=&b||i.global_product!=0)return 2;memset(&t,0,sizeof t);i.ai_active=1;i.global_product=&b;stars_finish_production(&i);if(t.fills||t.draws||t.lookups!=2||t.players[1]!=24)return 3;
i.write=0;i.game_flags=0;i.planet_bits=0x00800001UL;stars_finish_production(&i);if(i.planet_bits!=1UL)return 4;
printf("FINISH_PRODUCTION_CHECKS=4 FAILURES=0 RESULT=PASS\n");return 0;}
