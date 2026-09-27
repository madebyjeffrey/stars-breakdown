#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsI16 messages; StarsI16 freed; } Trace;
static StarsI32 fleet_power(void *c, const StarsSweepMinesFleet *f)
{ (void)c; (void)f; return 10L; }
static StarsI16 attack(void *c, StarsI16 a, StarsI16 b)
{ (void)c; return (StarsI16)(a != b); }
static void message(void *c, StarsI16 p, StarsI16 id, StarsI16 source,
    StarsI32 amount, StarsI16 type)
{ Trace *t = (Trace *)c; (void)p; (void)id; (void)source; (void)amount;
  (void)type; ++t->messages; }
static void free_thing(void *c, StarsSweepMinesThing *thing)
{ Trace *t = (Trace *)c; thing->active = 0; ++t->freed; }

int main(void)
{
    StarsSweepMinesFleet fleet;
    StarsSweepMinesThing thing;
    StarsSweepMinesInput input;
    Trace trace;
    memset(&fleet, 0, sizeof fleet); memset(&thing, 0, sizeof thing);
    memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    fleet.player = 0; fleet.id = 4; fleet.x = 0; fleet.y = 0;
    thing.kind = 0; thing.owner = 1; thing.x = 0; thing.y = 0;
    thing.detection_radius = 100L; thing.remaining_strength = 20L;
    thing.active = 1;
    input.fleets = &fleet; input.fleet_count = 1;
    input.things = &thing; input.thing_count = 1; input.context = &trace;
    input.ops.fleet_power = fleet_power; input.ops.can_attack = attack;
    input.ops.send_message = message; input.ops.free_thing = free_thing;
    stars_sweep_for_mines(&input);
    if (trace.messages != 2 || trace.freed != 0 ||
            thing.remaining_strength != 10L || (thing.owner_mask & 1UL) == 0)
        return 1;
    puts("SWEEP_FOR_MINES_SOURCE=PASS");
    return 0;
}
