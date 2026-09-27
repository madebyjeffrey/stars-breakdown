#include <stdio.h>
#include "core.h"

typedef struct Trace {
    StarsI16 values[8];
    StarsI16 count;
    StarsI16 player;
    StarsU16 message;
    StarsU16 id1;
    StarsU16 id2;
    StarsI16 climate;
    StarsI16 tossed;
} Trace;

static StarsI16 next_random(void *value, StarsI16 bound)
{
    Trace *trace = (Trace *)value;
    (void)bound;
    return trace->values[trace->count++];
}

static void send_message(void *value, StarsI16 player, StarsU16 message,
    StarsU16 id1, StarsU16 id2, StarsI16 climate)
{
    Trace *trace = (Trace *)value;
    trace->player = player;
    trace->message = message;
    trace->id1 = id1;
    trace->id2 = id2;
    trace->climate = climate;
}

static void toss(void *value, StarsClimatePlanet *planet)
{
    Trace *trace = (Trace *)value;
    trace->tossed = planet->id;
}

int main(void)
{
    StarsClimatePlanet planet;
    StarsPlanetaryClimateInput input;
    Trace trace;

    planet.id = 17;
    planet.player = 2;
    planet.climate_metric = 20UL;
    planet.environment[0] = 98;
    planet.environment[1] = 50;
    planet.environment[2] = 1;
    planet.original_environment[0] = 2;
    planet.original_environment[1] = 50;
    planet.original_environment[2] = 98;
    input.planets = &planet;
    input.planet_count = 1;
    input.turn = 10;
    input.random = next_random;
    input.send_message = send_message;
    input.toss_build_items = toss;
    input.context = &trace;
    trace.values[0] = 0;
    trace.values[1] = 0;
    trace.values[2] = 1;
    trace.values[3] = 0;
    trace.values[4] = 1;
    trace.values[5] = 1;
    trace.count = 0;
    trace.tossed = 0;
    stars_planetary_climate_change(&input);
    if (trace.count != 6 || planet.environment[1] != 43 ||
        planet.original_environment[1] != 43 || trace.player != 2 ||
        trace.message != 0x00fdu || trace.id1 != 17U || trace.id2 != 17U ||
        trace.climate != 1 || trace.tossed != 17)
        return 1;
    input.turn = 20;
    trace.values[0] = 0;
    trace.count = 0;
    planet.environment[0] = 98;
    planet.climate_metric = 250UL;
    stars_planetary_climate_change(&input);
    if (trace.count != 6 || planet.environment[1] != 36)
        return 2;
    input.turn = 19; trace.count = 0; trace.tossed = 0;
    stars_planetary_climate_change(&input);
    if (trace.count != 2 || trace.tossed != 0 || planet.environment[1] != 36)
        return 3;
    puts("PLANETARY_CLIMATE_CHANGE_SOURCE=PASS");
    return 0;
}
