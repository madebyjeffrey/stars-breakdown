#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsDamageApplyPlanet planet; StarsDamageApplyFleet fleet; StarsI16 killed; StarsI16 queues; StarsI16 records; } Trace;
static StarsDamageApplyPlanet *find_planet(void *c, StarsI16 id){Trace*t=(Trace*)c;(void)id;return &t->planet;}
static StarsDamageApplyFleet *find_fleet(void *c, StarsI16 id){Trace*t=(Trace*)c;(void)id;return &t->fleet;}
static void kill_ships(void *c, StarsDamageApplyToken *t, StarsI16 n, StarsDamageApplyFleet *f){Trace*x=(Trace*)c;(void)t;(void)f;x->killed=n;}
static void kill_queue(void *c, StarsDamageApplyPlanet *p){Trace*t=(Trace*)c;(void)p;++t->queues;}
static void record(void *c, StarsI16 id, StarsU16 w, StarsU32 d, StarsU16 f){Trace*t=(Trace*)c;(void)id;(void)w;(void)d;(void)f;++t->records;}
int main(void)
{
    StarsDamageApplyToken token; StarsFDamageTokInput input; Trace trace; StarsU32 beam,packets;
    memset(&token,0,sizeof token); memset(&input,0,sizeof input); memset(&trace,0,sizeof trace);
    token.id=7; token.ship_count=2; token.dp_shield=3; token.hull_damage=10; beam=4;
    input.token=&token; input.beam_damage=&beam; input.shields_only=1; input.weapon_flags=4; input.record=record; input.context=&trace;
    if (stars_f_damage_tok(&input)!=0 || beam!=0UL || token.dp_shield!=1U || trace.records!=1) return 1;
    memset(&trace,0,sizeof trace); token.dp_shield=0; token.ship_count=2; beam=25; packets=3;
    input.shields_only=0; input.torpedo_damage=0; input.torpedo_packets=&packets; input.find_fleet=find_fleet; input.kill_ships=kill_ships; input.record=record;
    if (stars_f_damage_tok(&input)!=1 || token.ship_count!=0U || trace.killed!=2 || packets!=1UL || trace.records!=1) return 2;
    memset(&trace,0,sizeof trace); token.object_kind=1; token.id=9; token.ship_count=1; token.hull_damage=10; beam=5;
    trace.planet.starbase_damage=4; trace.planet.starbase_limit=99; input.torpedo_damage=0; input.torpedo_packets=(StarsU32*)0;
    input.find_planet=find_planet; input.kill_queued_ships=kill_queue; input.kill_queued_packets=kill_queue;
    if (stars_f_damage_tok(&input)!=1 || token.ship_count!=0U || trace.queues!=2 || trace.records!=1) return 3;
    puts("F_DAMAGE_TOK_SOURCE=PASS"); return 0;
}
