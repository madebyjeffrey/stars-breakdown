#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsMovePlanet planet;
    StarsI16 applied;
    StarsI16 removed;
    StarsI16 validated;
    StarsI16 messages;
} Trace;

static StarsI16 random_value(void *p, StarsI16 limit)
{ (void)p; (void)limit; return 0; }
static StarsI16 wormhole_chance(void *p, const StarsMoveThing *thing)
{ (void)p; (void)thing; return 100; }
static StarsI16 validate_wormhole(void *p, StarsMoveThing *thing)
{ (void)p; (void)thing; return 5; }
static StarsI16 distance_value(void *p, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{ (void)p; return (StarsI16)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)); }
static StarsI16 decay(void *p, StarsMoveThing *thing, StarsI16 amount)
{ (void)p; (void)thing; (void)amount; return 0; }
static StarsI16 warp(void *p, const StarsMovePlanet *planet, StarsI16 *range)
{ (void)p; (void)planet; *range = 4; return 1; }
static StarsI16 survival(void *p, const StarsMovePlanet *planet)
{ (void)p; (void)planet; return 100; }
static StarsMovePlanet *find_planet(void *p, StarsI16 id)
{ Trace *t = (Trace *)p; return id == t->planet.id ? &t->planet : (StarsMovePlanet *)0; }
static StarsI16 apply_packet(void *p, StarsMovePlanet *planet, StarsI16 a,
    StarsI16 b, StarsI16 c, StarsI16 warp_level)
{ Trace *t = (Trace *)p; (void)planet; (void)a; (void)b; (void)c; (void)warp_level; ++t->applied; return 1; }
static void remove_thing(void *p, StarsMoveThing *thing)
{ Trace *t = (Trace *)p; (void)thing; ++t->removed; }
static void validate_waypoints(void *p)
{ Trace *t = (Trace *)p; ++t->validated; }
static void message(void *p, StarsI16 player, StarsI16 code,
    const StarsMoveThing *thing, StarsI16 a, StarsI16 b)
{ Trace *t = (Trace *)p; (void)player; (void)code; (void)thing; (void)a; (void)b; ++t->messages; }

int main(void)
{
    StarsMoveThing things[3];
    StarsMoveThingsOps ops;
    StarsMoveThingsInput input;
    Trace trace;

    memset(things, 0, sizeof things); memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    trace.planet.id = 4; trace.planet.x = 0; trace.planet.y = 0;
    things[0].kind = 2; things[0].active = 1; things[0].x = 10;
    things[1].kind = 1; things[1].active = 1; things[1].target_planet = 4;
    things[1].strength[0] = 20; things[1].warp_level = 0;
    things[2].kind = 3; things[2].active = 1;
    ops.context = &trace; ops.random_value = random_value;
    ops.wormhole_chance = wormhole_chance; ops.validate_wormhole = validate_wormhole;
    ops.distance = distance_value; ops.packet_decay = decay;
    ops.warp_machine = warp; ops.planet_survival = survival;
    ops.find_planet = find_planet; ops.apply_packet = apply_packet;
    ops.remove_thing = remove_thing; ops.validate_waypoints = validate_waypoints;
    ops.send_message = message;
    input.things = things; input.thing_count = 3; input.map_size = 2;
    input.ops = &ops; input.post_production = 0;
    stars_move_things(&input);
    if (!things[0].active || !things[2].active || trace.applied != 1 ||
            trace.removed != 1 || trace.validated != 1) return 1;
    input.post_production = 1; stars_move_things(&input);
    if (things[0].x == 10 && things[0].y == 0) return 2;
    puts("MOVE_THINGS_SOURCE=PASS");
    return 0;
}
