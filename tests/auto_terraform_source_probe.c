#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 random_calls;
    StarsI16 climate_messages;
    StarsI16 desirability_messages;
    StarsI16 last_value;
} Trace;

static StarsI16 random_value(void *context, StarsI16 limit)
{
    Trace *trace = (Trace *)context;
    ++trace->random_calls;
    if (limit == 3) return 1;
    return 0;
}
static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{
    (void)context; (void)stat;
    return player == 0 ? 3 : 0;
}
static StarsI16 race_target(void *context, StarsI16 player, StarsI16 axis)
{
    (void)context; (void)player; (void)axis;
    return 40;
}
static StarsI16 can_terraform(void *context,
    const StarsAutoTerraformPlanet *planet, StarsI16 min[3],
    StarsI16 max[3], StarsI16 cost[3], StarsI16 mode)
{
    (void)context; (void)planet; (void)mode;
    min[0] = -1; max[0] = 60; cost[0] = 1;
    min[1] = 45; max[1] = -1; cost[1] = 1;
    min[2] = -1; max[2] = -1; cost[2] = 0;
    return 1;
}
static StarsI16 desirability(void *context,
    const StarsAutoTerraformPlanet *planet, StarsI16 player)
{
    (void)context; (void)player;
    return (StarsI16)(planet->environment[0] + planet->environment[1]);
}
static void message(void *context, StarsI16 player, StarsI16 message_id,
    StarsU16 id, StarsI16 value)
{
    Trace *trace = (Trace *)context;
    (void)player; (void)id;
    trace->last_value = value;
    if (message_id == 0x15c) ++trace->climate_messages;
    if (message_id == 0x156) ++trace->desirability_messages;
}

int main(void)
{
    StarsAutoTerraformPlanet planet;
    StarsAutoTerraformInput input;
    Trace trace;

    memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    planet.player = 0; planet.id = 7; planet.population = 1000L;
    planet.reserve = 0L;
    planet.environment[0] = 50; planet.environment[1] = 50;
    planet.original_environment[0] = 50; planet.original_environment[1] = 50;
    input.planets = &planet; input.planet_count = 1; input.player_count = 1;
    input.ops.random = random_value; input.ops.race_stat = race_stat;
    input.ops.race_target = race_target; input.ops.can_terraform = can_terraform;
    input.ops.desirability = desirability; input.ops.send_message = message;
    input.context = &trace;
    stars_auto_terraform(&input);
    if (trace.climate_messages != 1 || trace.desirability_messages != 1 ||
            planet.original_environment[0] != 50 ||
            planet.original_environment[1] != 49 ||
            planet.environment[0] != 60 || planet.environment[1] != 45 ||
            trace.last_value != 105)
        return 1;
    puts("AUTO_TERRAFORM_SOURCE=PASS");
    return 0;
}
