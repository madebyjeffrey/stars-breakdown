#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 messages;
    StarsI16 last_message;
    StarsU32 last_object;
    StarsU16 removed;
    StarsU16 researched;
} Trace;

static StarsI16 fixed_random(void *context, StarsI16 limit)
{
    (void)context;
    return (StarsI16)(limit > 1 ? 1 : 0);
}
static void message2(void *context, StarsI16 player, StarsI16 message,
    StarsU32 object, StarsU16 word, StarsI16 value)
{
    Trace *trace = (Trace *)context;
    (void)player; (void)word; (void)value;
    trace->messages = (StarsU16)(trace->messages + 1U);
    trace->last_message = message;
    trace->last_object = object;
}
static void remove_message(void *context, StarsI16 player, StarsI16 message,
    StarsU32 object)
{
    Trace *trace = (Trace *)context;
    (void)player; (void)message; (void)object;
    trace->removed = (StarsU16)(trace->removed + 1U);
}
static void update_research(void *context, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    (void)player;
    trace->researched = (StarsU16)(trace->researched + 1U);
}
static StarsU32 develop_cost(void *context, StarsI16 player, const StarsU8 *technology)
{
    (void)context; (void)player; (void)technology;
    return 1U;
}

int main(void)
{
    StarsThingInteractionThing thing;
    StarsThingInteractionFleet fleet;
    StarsThingInteractionPlanet planet;
    StarsThingInteractionPlayer players[16];
    StarsThingInteractionOps ops;
    Trace trace;

    memset(&thing, 0, sizeof thing);
    memset(&fleet, 0, sizeof fleet);
    memset(&planet, 0, sizeof planet);
    memset(players, 0, sizeof players);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.random_value = fixed_random;
    ops.send_message2 = message2;
    ops.remove_message = remove_message;
    ops.develop_cost = develop_cost;
    ops.update_research = update_research;
    thing.overlap = 0x6000U;
    thing.x = 40; thing.y = 40;
    fleet.x = 40; fleet.y = 40; fleet.player = 0; fleet.id = 7;
    fleet.minerals[0] = 1000U;
    fleet.minerals[1] = 1000U;
    fleet.minerals[2] = 1000U;
    stars_do_thing_interactions(0, &thing, 1, &fleet, 1, &planet, 1,
        players, &ops, &trace, 50);
    if (trace.messages != 0U || thing.visited_players != 0U)
        return 1;
    fleet.minerals[2] = 1000U;
    stars_do_thing_interactions(1, &thing, 1, &fleet, 1, &planet, 1,
        players, &ops, &trace, 50);
    if (trace.messages != 1U || trace.last_message != 0x108 ||
            thing.visited_players != 0U)
        return 2;
    fleet.minerals[0] = 2000U;
    fleet.minerals[1] = 2000U;
    fleet.minerals[2] = 2000U;
    stars_do_thing_interactions(1, &thing, 1, &fleet, 1, &planet, 1,
        players, &ops, &trace, 50);
    if (trace.removed != 1U || trace.researched != 1U ||
            (thing.visited_players & 1U) == 0U)
        return 3;
    thing.visited_players = 0U;
    fleet.x = 0; fleet.y = 0;
    planet.x = 40; planet.y = 40; planet.owner = 0; planet.overlap = 0x0200U;
    planet.minerals[0] = 2000U;
    planet.minerals[1] = 2000U;
    planet.minerals[2] = 2000U;
    players[0].detail_flags = 0x0200U;
    players[0].technology_stage = 2U;
    stars_do_thing_interactions(1, &thing, 1, &fleet, 1, &planet, 1,
        players, &ops, &trace, 50);
    if ((thing.visited_players & 1U) == 0U ||
            planet.minerals[0] != 2000U || planet.minerals[1] != 500U ||
            planet.minerals[2] != 0U)
        return 4;
    puts("DO_THING_INTERACTIONS_SOURCE=PASS");
    return 0;
}
