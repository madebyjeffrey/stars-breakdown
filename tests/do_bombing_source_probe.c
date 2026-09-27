#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 messages;
    StarsI16 uninhabited;
} Trace;
static StarsI16 attack(void *context, const StarsDoBombingFleet *fleet,
    StarsI16 owner)
{
    (void)context; (void)fleet; (void)owner; return 1;
}
static StarsI16 calculate(void *context, const StarsDoBombingFleet *fleet,
    StarsDoBombingDamage *damage)
{
    (void)context; (void)fleet;
    damage->population = 20; damage->minimum_population = 5;
    damage->smart_population = 10; damage->buildings = 4;
    damage->terraforming = 2; damage->multiple_fleets = 0;
    return 1;
}
static void survival(void *context, const StarsDoBombingPlanet *planet,
    StarsU16 *ordinary, StarsU16 *smart)
{
    (void)context; (void)planet; *ordinary = 500; *smart = 500;
}
static void send_message(void *context, StarsI16 player, StarsI16 event,
    StarsI16 object, StarsI16 a, StarsI16 b, StarsI16 c, StarsI16 d,
    StarsI16 e, StarsI16 f)
{
    Trace *trace = (Trace *)context;
    (void)player; (void)event; (void)object; (void)a; (void)b;
    (void)c; (void)d; (void)e; (void)f; ++trace->messages;
}
static void uninhabit(void *context, StarsDoBombingPlanet *planet)
{
    Trace *trace = (Trace *)context; (void)planet; ++trace->uninhabited;
}

int main(void)
{
    StarsDoBombingInput input;
    StarsDoBombingFleet fleets[2];
    StarsDoBombingPlanet planets[2];
    Trace trace;
    memset(&input, 0, sizeof input); memset(fleets, 0, sizeof fleets);
    memset(planets, 0, sizeof planets); memset(&trace, 0, sizeof trace);
    fleets[0].id = 4; fleets[0].player = 0; fleets[0].planet_id = 3;
    fleets[1].id = 5; fleets[1].player = 0; fleets[1].planet_id = 4;
    planets[0].id = 3; planets[0].owner = 1; planets[0].population = 100;
    planets[0].mineral_stock = 50; planets[0].buildings = 10;
    planets[0].environment[0] = 3; planets[0].original_environment[0] = 0;
    planets[1].id = 4; planets[1].owner = 1; planets[1].flags = 0x0200;
    planets[1].population = 100; planets[1].mineral_stock = 50;
    input.fleets = fleets; input.fleet_count = 2; input.planets = planets;
    input.planet_count = 2; input.attack = attack; input.calculate = calculate;
    input.survival = survival; input.send_message = send_message;
    input.uninhabit = uninhabit; input.context = &trace;
    stars_do_bombing(&input);
    if (!(fleets[0].flags & 0x1000U) || fleets[1].flags ||
            planets[0].population != 90 || planets[0].buildings != 8 ||
            planets[0].mineral_stock != 48 || planets[0].environment[0] != 2 ||
            trace.messages != 2) return 1;
    fleets[0].planet_id = 4; fleets[0].flags = 0; fleets[1].planet_id = -1;
    planets[1].flags = 0;
    planets[1].population = 5; planets[1].mineral_stock = 0;
    planets[1].buildings = 0; planets[1].environment[0] = 0;
    planets[1].original_environment[0] = 0;
    trace.messages = 0; trace.uninhabited = 0;
    stars_do_bombing(&input);
    if (planets[1].population != 0 || trace.uninhabited != 1 ||
            trace.messages != 2) return 2;
    puts("DO_BOMBING_SOURCE=PASS");
    return 0;
}
