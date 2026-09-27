#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 freed;
    StarsI16 traveled;
} Trace;
static StarsI16 race(void *c, StarsI16 p, StarsI16 s)
{ (void)c; (void)p; (void)s; return 5; }
static StarsI16 packet(void *c, StarsThingDecayThing *t, StarsI16 rate)
{ (void)c; (void)t; (void)rate; return 0; }
static void travel(void *c, StarsThingDecayFleet *f, StarsThingDecayThing *t)
{ Trace *x = (Trace *)c; (void)f; (void)t; ++x->traveled; }
static StarsI16 planets(void *c, StarsI16 x, StarsI16 y, StarsI32 r)
{ (void)c; (void)x; (void)y; (void)r; return 3; }
static void free_thing(void *c, StarsThingDecayThing *t)
{ Trace *x = (Trace *)c; t->active = 0; ++x->freed; }

int main(void)
{
    StarsThingDecayThing things[2];
    StarsThingDecayFleet fleet;
    StarsThingDecayInput input;
    Trace trace;
    memset(things, 0, sizeof things); memset(&fleet, 0, sizeof fleet);
    memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    things[0].kind = 1; things[0].active = 1; things[0].packet_resources[0] = 100;
    things[1].kind = 0; things[1].active = 1; things[1].radius = 100;
    things[1].strength = 100; things[1].mine_type = 2;
    things[1].active = 1;
    fleet.x = 0; fleet.y = 0;
    input.things = things; input.thing_count = 2; input.fleets = &fleet;
    input.fleet_count = 1; input.ops.race_stat = race; input.ops.packet_decay = packet;
    input.ops.travel_through_mines = travel; input.ops.planets_in_circle = planets;
    input.ops.free_thing = free_thing; input.context = &trace;
    stars_thing_decay(&input);
    if (trace.freed != 0 || trace.traveled != 1 ||
            things[0].packet_resources[0] != 90 || things[0].strength != 9 ||
            things[1].strength != 70 ||
            (fleet.status_flags & 0x1000) == 0)
        return 1;
    puts("THING_DECAY_SOURCE=PASS");
    return 0;
}
