#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 best_calls;
    StarsI16 messages;
    StarsI16 last_message;
    StarsI16 last_after;
} Trace;

static StarsI32 amount(void *context, const StarsRemoteTurnFleet *fleet)
{ (void)context; (void)fleet; return 2L; }
static StarsI16 friendly(void *context, StarsI16 source, StarsI16 owner)
{ (void)context; return (StarsI16)(source == owner); }
static StarsI16 best(void *context, StarsRemoteTurnPlanet *planet,
    StarsI16 source, StarsI16 help)
{
    Trace *trace = (Trace *)context;
    (void)planet; (void)source; (void)help;
    ++trace->best_calls;
    return trace->best_calls == 1 ? -1 : 0;
}
static StarsI16 desirability(void *context,
    const StarsRemoteTurnPlanet *planet, StarsI16 player)
{ (void)context; (void)player; return planet->mineral_concentration[0]; }
static void message(void *context, StarsI16 player, StarsI16 message_id,
    StarsU16 a, StarsU16 b, StarsU16 c, StarsI16 before, StarsI16 after)
{
    Trace *trace = (Trace *)context;
    (void)player; (void)a; (void)b; (void)c; (void)before;
    ++trace->messages; trace->last_message = message_id;
    trace->last_after = after;
}

int main(void)
{
    StarsRemoteTurnFleet fleet;
    StarsRemoteTurnPlanet planet;
    StarsRemoteTurnInput input;
    Trace trace;

    memset(&fleet, 0, sizeof fleet); memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    fleet.player = 0; fleet.planet_id = 0; fleet.object_token = 5;
    planet.player = 1; planet.id = 9; planet.mineral_concentration[0] = 50;
    input.fleets = &fleet; input.fleet_count = 1;
    input.planets = &planet; input.planet_count = 1;
    input.ops.terraforming_amount = amount; input.ops.friendly = friendly;
    input.ops.best_terraform = best; input.ops.desirability = desirability;
    input.ops.send_message = message; input.context = &trace;
    stars_remote_terraform(&input);
    if (trace.best_calls != 2 || trace.messages != 2 ||
            planet.mineral_concentration[0] != 49 ||
            trace.last_message != 0x15a || trace.last_after != 49)
        return 1;
    puts("REMOTE_TERRAFORMING_SOURCE=PASS");
    return 0;
}
