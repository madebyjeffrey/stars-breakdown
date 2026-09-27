#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsBoardPlanet planet; StarsI16 dumped; StarsI16 checked; StarsI16 targeted; StarsI16 randomized; StarsI16 recorded; } Trace;
static StarsBoardPlanet *find_planet(void *c, StarsI16 id){Trace*t=(Trace*)c;(void)id;return &t->planet;}
static StarsI16 dump_cargo(void *c, StarsBoardFleet *f){Trace*t=(Trace*)c;(void)f;++t->dumped;return 1;}
static void check_initiative(void *c, StarsBoardToken *t){Trace*x=(Trace*)c;(void)x;t->init_base=10;t->init_min=10;t->init_max=12;}
static void check_weapons(void *c, StarsBoardToken *t, StarsI16 *d, StarsU8 *p){Trace*x=(Trace*)c;(void)x;(void)p;*d=1;t->dp_shield=4;}
static void check_target(void *c, StarsBoardToken *t, StarsBoardFleet *f, StarsI16 d){Trace*x=(Trace*)c;(void)t;(void)f;(void)d;++x->targeted;}
static StarsU8 speed(void *c, StarsBoardFleet *f, StarsI16 d, StarsBoardToken *t, StarsI16 dump){(void)c;(void)f;(void)d;(void)t;(void)dump;return 8;}
static StarsI16 race_flag(void *c, StarsI16 p, StarsU16 code){(void)c;(void)p;(void)code;return 1;}
static void randomize(void *c, StarsBoardToken *t, StarsI16 n){Trace*x=(Trace*)c;(void)t;x->randomized=n;}
static void record_token(void *c, const StarsBoardToken *t){Trace*x=(Trace*)c;(void)t;++x->recorded;}
int main(void)
{
    StarsBoardFleet fleets[2]; StarsBoardPlanet planets[1]; StarsBoardToken tokens[4];
    StarsInitializeBoardInput input; Trace trace; StarsU8 initiative[64]; StarsI16 min_init,max_init;
    memset(fleets,0,sizeof fleets); memset(planets,0,sizeof planets); memset(tokens,0,sizeof tokens);
    memset(&input,0,sizeof input); memset(&trace,0,sizeof trace); memset(initiative,0,sizeof initiative);
    planets[0].id=42; planets[0].owner=2; planets[0].is_starbase=1; planets[0].design_nibble=3; trace.planet=planets[0];
    fleets[0].id=7; fleets[0].player=0; fleets[0].planet_id=42; fleets[0].flags=0x100U; fleets[0].design_count[0]=2;
    fleets[1].id=8; fleets[1].player=1; fleets[1].planet_id=-1; fleets[1].flags=0x80U;
    input.fleets=fleets; input.fleet_count=2; input.planets=planets; input.planet_count=1; input.player_count=3;
    input.battle_coordinate=5; input.active_players=0x0005U; input.tokens=tokens; input.token_capacity=4;
    input.initiative=initiative; input.initiative_min=&min_init; input.initiative_max=&max_init;
    input.find_planet=find_planet; input.dump_cargo=dump_cargo; input.check_initiative=check_initiative;
    input.check_weapons=check_weapons; input.check_target=check_target; input.speed=speed;
    input.race_flag=race_flag; input.randomize=randomize; input.record_token=record_token; input.context=&trace;
    stars_initialize_board(&input);
    if (trace.randomized!=2 || trace.recorded!=2 || trace.targeted!=1 ||
        tokens[0].object_kind!=1U || tokens[0].player!=2 || tokens[0].design!=19 ||
        tokens[1].object_kind!=2U || tokens[1].warp!=4U ||
        !(input.missed_players&2U) || !(trace.planet.flags&0x4000U) ||
        !(fleets[0].flags&0x4000U) || min_init!=10 || max_init!=12) return 1;
    puts("INITIALIZE_BOARD_SOURCE=PASS"); return 0;
}
