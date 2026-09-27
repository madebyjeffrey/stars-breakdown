#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 mined;
    StarsI16 built;
    StarsI16 messages;
    StarsI16 codes[4];
    StarsI16 owners[4];
    StarsI16 ids[4];
    StarsI16 populations;
    StarsI16 research;
    StarsI16 events;
} Trace;

typedef struct FullBuildTrace {
    StarsI16 build_calls;
    StarsI16 object_calls;
} FullBuildTrace;

static void mine(void *p)
{ Trace *t = (Trace *)p; ++t->mined; }
static StarsI32 resources(void *p, const StarsProducePlanet *planet, StarsI16 owner)
{ (void)p; (void)owner; return planet->resources; }
static StarsI16 capacity(void *p, const StarsProducePlanet *planet,
    StarsI16 type, StarsI16 operable)
{ (void)p; (void)planet; (void)type; (void)operable; return 100; }
static StarsI16 build_item(void *p, StarsProducePlanet *planet,
    StarsProduceItem *item, StarsI32 *resources_left, StarsI16 partial)
{ Trace *t = (Trace *)p; (void)planet; (void)item; (void)partial; if (*resources_left < 10) return 0; *resources_left -= 10; ++t->built; return 1; }
static StarsI16 build_object(void *p, StarsProducePlanet *planet,
    StarsI16 object_kind, StarsI16 type, StarsI16 count, StarsI32 *minerals)
{ (void)p; (void)planet; (void)object_kind; (void)type; (void)count; (void)minerals; return 1; }
static void message(void *p, StarsI16 owner, StarsI16 code, StarsI16 id,
    StarsI16 a, StarsI16 b)
{ Trace *t = (Trace *)p; (void)a; (void)b;
  if (t->messages < 4) { t->codes[t->messages] = code;
      t->owners[t->messages] = owner; t->ids[t->messages] = id; }
  ++t->messages; }
static void populations(void *p, StarsProducePlanet *planet)
{ Trace *t = (Trace *)p; (void)planet; ++t->populations; }
static void research(void *p, StarsI16 mode)
{ Trace *t = (Trace *)p; (void)mode; ++t->research; }
static void events(void *p)
{ Trace *t = (Trace *)p; ++t->events; }

static StarsI16 full_costs(void *context, const StarsProducePlanet *planet,
    const StarsProduceItem *item, StarsI32 costs[4])
{
    (void)context; (void)planet; (void)item;
    costs[0] = 16; costs[1] = 2; costs[2] = 8; costs[3] = 35;
    return 1;
}

static StarsI16 full_capacity(void *context,
    const StarsProducePlanet *planet, StarsI16 type, StarsI16 operable)
{
    (void)context; (void)planet; (void)type; (void)operable;
    return 3;
}

static StarsI16 full_object(void *context, StarsProducePlanet *planet,
    StarsI16 object_kind, StarsI16 type, StarsI16 count, StarsI32 minerals[3])
{
    FullBuildTrace *trace = (FullBuildTrace *)context;
    (void)planet; (void)object_kind; (void)type; (void)count; (void)minerals;
    ++trace->object_calls;
    return 1;
}

static void full_message(void *context, StarsI16 owner, StarsI16 code,
    StarsI16 planet, StarsI16 value, StarsI16 status)
{
    (void)context; (void)owner; (void)code; (void)planet; (void)value;
    (void)status;
}

static StarsI16 build_item_with_resources(void *context,
    StarsProducePlanet *planet, StarsProduceItem *item,
    StarsProduceItem *partial, StarsI32 resources[4], StarsI16 *status)
{
    FullBuildTrace *trace = (FullBuildTrace *)context;
    StarsBuildProductionOps build_ops;
    StarsBuildProductionInput build_input;
    StarsI16 built;
    StarsI16 index;

    memset(&build_ops, 0, sizeof build_ops);
    memset(&build_input, 0, sizeof build_input);
    build_ops.costs = full_costs;
    build_ops.capacity = full_capacity;
    build_ops.build_object = full_object;
    build_ops.send_message = full_message;
    build_ops.context = context;
    build_input.planet = planet;
    build_input.item = item;
    build_input.partial = partial;
    build_input.status = status;
    build_input.ops = &build_ops;
    for (index = 0; index < 4; ++index)
        build_input.resources[index] = resources[index];
    built = stars_build_production_item(&build_input);
    for (index = 0; index < 4; ++index)
        resources[index] = build_input.resources[index];
    ++trace->build_calls;
    return built;
}

static int run_partial_production_case(void)
{
    StarsProduceItem queue[2];
    StarsProducePlanet planet;
    StarsProduceOps ops;
    StarsProduceInput input;
    FullBuildTrace trace;

    memset(queue, 0, sizeof queue);
    memset(&planet, 0, sizeof planet);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    queue[0].type = 0;
    queue[0].object_kind = 2;
    queue[0].quantity = 1;
    queue[0].active = 1;
    queue[1] = queue[0];
    planet.id = 24;
    planet.owner = 1;
    planet.resources = 32;
    planet.research_percent = 15;
    planet.mineral[0] = 604;
    planet.mineral[1] = 965;
    planet.mineral[2] = 526;
    planet.items = queue;
    planet.item_count = 2;
    planet.item_capacity = 2;
    ops.context = &trace;
    ops.resources_at_planet = resources;
    ops.max_capacity = capacity;
    ops.build_item_with_resources = build_item_with_resources;
    input.planets = &planet;
    input.planet_count = 1;
    input.ops = &ops;
    stars_produce(&input);
    if (trace.build_calls != 1 || trace.object_calls != 0 ||
            planet.item_count != 2 || planet.items[0].partial != 0 ||
            planet.items[0].progress != 81 ||
            planet.items[0].quantity != 1 ||
            planet.items[1].partial != 0 ||
            planet.items[1].progress != 0 ||
            planet.items[1].quantity != 1 || planet.mineral[0] != 592 ||
            planet.mineral[1] != 964 || planet.mineral[2] != 520 ||
            planet.last_year_resources != 4)
        return 0;
    return 1;
}

static int run_cheater_resource_case(void)
{
    StarsProduceItem queue[2];
    StarsProducePlanet planet;
    StarsProduceOps ops;
    StarsProduceInput input;
    FullBuildTrace trace;

    memset(queue, 0, sizeof queue);
    memset(&planet, 0, sizeof planet);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    queue[0].type = 0;
    queue[0].object_kind = 2;
    queue[0].quantity = 1;
    queue[0].active = 1;
    queue[1] = queue[0];
    planet.id = 24;
    planet.owner = 1;
    planet.resources = 35;
    planet.cheater = 1;
    planet.mineral[0] = 604;
    planet.mineral[1] = 965;
    planet.mineral[2] = 526;
    planet.items = queue;
    planet.item_count = 2;
    planet.item_capacity = 2;
    ops.context = &trace;
    ops.resources_at_planet = resources;
    ops.max_capacity = capacity;
    ops.build_item_with_resources = build_item_with_resources;
    input.planets = &planet;
    input.planet_count = 1;
    input.ops = &ops;
    stars_produce(&input);
    if (trace.build_calls != 1 || planet.items[0].progress != 81 ||
            planet.items[0].quantity != 1 || planet.item_count != 2)
        return 0;
    return 1;
}

int main(void)
{
    StarsProduceItem item;
    StarsProducePlanet planets[3];
    StarsProduceOps ops;
    StarsProduceInput input;
    Trace trace;

    memset(&item, 0, sizeof item); memset(planets, 0, sizeof planets);
    memset(&ops, 0, sizeof ops); memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    item.type = 3; item.object_kind = 1; item.quantity = 2; item.active = 1;
    planets[0].id = 5; planets[0].owner = 1; planets[0].resources = 30;
    planets[0].items = &item; planets[0].item_count = 1;
    planets[1].id = 6; planets[1].owner = -1;
    planets[2].id = 10; planets[2].owner = 0;
    planets[2].resources = 21;
    ops.context = &trace; ops.mine_minerals = mine;
    ops.resources_at_planet = resources; ops.max_capacity = capacity;
    ops.build_item = build_item; ops.build_object = build_object;
    ops.send_message = message; ops.update_populations = populations;
    ops.update_research = research; ops.random_events = events;
    input.planets = planets; input.planet_count = 3; input.ops = &ops;
    stars_produce(&input);
    if (trace.mined != 1 || trace.built != 2 || item.quantity != 0 ||
            planets[0].item_count != 0 || trace.populations != 3 ||
            trace.research != 1 || trace.events != 1) return 1;
    if (trace.messages != 2 || trace.codes[0] != 0x3e ||
            trace.codes[1] != 0x3f || trace.owners[1] != 0 ||
            trace.ids[1] != 10 || planets[2].last_year_resources != 21)
        return 1;
    if (!run_partial_production_case() || !run_cheater_resource_case())
        return 2;
    puts("PRODUCE_SOURCE=PASS");
    return 0;
}
