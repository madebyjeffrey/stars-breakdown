#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 messages;
    StarsI16 ships;
    StarsI16 starbases;
    StarsI16 specials;
    StarsI16 random_seed;
} Trace;

static StarsI16 max_installation(void *p, const StarsBuildObjectPlanet *planet,
    StarsI16 kind)
{
    (void)p; (void)planet; (void)kind; return 10;
}
static void message(void *p, StarsI16 player, StarsI16 code, StarsI16 a,
    StarsI16 b, StarsI16 c, StarsI16 d, StarsI16 e, StarsI16 f,
    StarsI16 g, StarsI16 h)
{
    Trace *t = (Trace *)p; (void)player; (void)code; (void)a; (void)b;
    (void)c; (void)d; (void)e; (void)f; (void)g; (void)h; ++t->messages;
}
static StarsI16 remove_message(void *p, StarsI16 player, StarsI16 code,
    StarsI16 planet)
{ (void)p; (void)player; (void)code; (void)planet; return 0; }
static StarsI16 can_build(void *p, StarsI16 player, StarsI16 item,
    StarsI16 starbase)
{ (void)p; (void)player; (void)item; (void)starbase; return 1; }
static void build_ship(void *p, StarsI16 player, StarsI16 planet,
    StarsI16 item, StarsI16 quantity)
{ Trace *t = (Trace *)p; (void)player; (void)planet; (void)item; (void)quantity; ++t->ships; }
static void build_starbase(void *p, StarsI16 player, StarsI16 planet,
    StarsI16 item)
{ Trace *t = (Trace *)p; (void)player; (void)planet; (void)item; ++t->starbases; }
static StarsI16 best_terraform(void *p, const StarsBuildObjectPlanet *planet,
    StarsI16 mode)
{ (void)p; (void)planet; (void)mode; return 1; }
static StarsI16 warp_machine(void *p, const StarsBuildObjectPlanet *planet,
    StarsI16 *range)
{ (void)p; (void)planet; *range = 5; return 1; }
static StarsI16 random_value(void *p, StarsI16 limit)
{ Trace *t = (Trace *)p; t->random_seed = (StarsI16)(t->random_seed + 7); return (StarsI16)(t->random_seed % limit); }
static StarsI16 race_stat(void *p, StarsI16 player, StarsI16 stat)
{ (void)p; (void)player; (void)stat; return 6; }
static void special(void *p, StarsI16 player, StarsI16 item, StarsI16 quantity)
{ Trace *t = (Trace *)p; (void)player; (void)item; (void)quantity; ++t->specials; }

int main(void)
{
    StarsBuildObjectInput input;
    StarsBuildObjectOps ops;
    StarsBuildObjectPlanet planet;
    Trace trace;

    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    memset(&planet, 0, sizeof planet); memset(&trace, 0, sizeof trace);
    ops.context = &trace; ops.max_installation = max_installation;
    ops.send_message = message; ops.remove_message = remove_message;
    ops.can_build = can_build; ops.build_ship = build_ship;
    ops.build_starbase = build_starbase; ops.best_terraform = best_terraform;
    ops.warp_machine = warp_machine; ops.random_value = random_value;
    ops.race_stat = race_stat; ops.build_special = special;
    planet.id = 12; planet.player = 2; planet.flags = 0x0200;
    planet.starbase_packets = 4; input.planet = &planet;
    input.player_count = 4; input.ops = ops; input.quantity = 3;

    input.object_group = 1; input.item = 0;
    if (!stars_build_object(&input) || planet.installations[0] != 3) return 1;
    input.item = 4; input.quantity = 2;
    if (!stars_build_object(&input) || planet.environment[0] != 2) return 2;
    input.object_group = 2; input.item = 3; input.quantity = 2;
    if (!stars_build_object(&input) || trace.ships != 1) return 3;
    input.item = 0x10; input.quantity = 1;
    if (!stars_build_object(&input) || trace.starbases != 1 ||
            planet.starbase_hull != 0) return 4;
    input.object_group = 1; input.item = 0x12; input.quantity = 1;
    if (!stars_build_object(&input) || trace.specials != 1) return 5;
    puts("F_BUILD_OBJECT_SOURCE=PASS");
    return 0;
}
