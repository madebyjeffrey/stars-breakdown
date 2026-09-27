#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 built;
    StarsI16 messages;
    StarsI32 cost[4];
} Trace;

static StarsI16 costs(void *p, const StarsProducePlanet *planet,
    const StarsProduceItem *item, StarsI32 out[4])
{ Trace *t = (Trace *)p; (void)planet; (void)item; memcpy(out, t->cost, sizeof t->cost); return 1; }
static StarsI16 capacity(void *p, const StarsProducePlanet *planet,
    StarsI16 type, StarsI16 operable)
{ (void)p; (void)planet; (void)type; (void)operable; return 3; }
static StarsI16 build_object(void *p, StarsProducePlanet *planet,
    StarsI16 object_kind, StarsI16 type, StarsI16 count, StarsI32 minerals[3])
{ Trace *t = (Trace *)p; (void)planet; (void)object_kind; (void)type; (void)count; (void)minerals; ++t->built; return 1; }
static void message(void *p, StarsI16 owner, StarsI16 code, StarsI16 id,
    StarsI16 value, StarsI16 status)
{ Trace *t = (Trace *)p; (void)owner; (void)code; (void)id; (void)value; (void)status; ++t->messages; }

int main(void)
{
    StarsProducePlanet planet;
    StarsProduceItem item;
    StarsProduceItem partial;
    StarsBuildProductionOps ops;
    StarsBuildProductionInput input;
    Trace trace;
    StarsI16 status;

    memset(&planet, 0, sizeof planet); memset(&item, 0, sizeof item);
    memset(&partial, 0, sizeof partial); memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    planet.id = 4; planet.owner = 1;
    item.type = 1; item.object_kind = 2; item.quantity = 2; item.active = 1;
    trace.cost[0] = 5; trace.cost[1] = 2;
    ops.context = &trace; ops.costs = costs; ops.capacity = capacity;
    ops.build_object = build_object; ops.send_message = message;
    input.planet = &planet; input.item = &item; input.partial = &partial;
    input.resources[0] = 20; input.resources[1] = 10;
    input.status = &status; input.ops = &ops;
    if (stars_build_production_item(&input) != 2 || item.quantity != 0 ||
            trace.built != 2 || trace.messages != 1 || status != 0) return 1;

    memset(&item, 0, sizeof item); memset(&partial, 0, sizeof partial);
    memset(&trace, 0, sizeof trace);
    item.type = 0; item.object_kind = 2; item.quantity = 1; item.active = 1;
    trace.cost[0] = 16; trace.cost[1] = 2;
    trace.cost[2] = 8; trace.cost[3] = 35;
    memset(input.resources, 0, sizeof input.resources);
    input.resources[0] = 604; input.resources[1] = 965;
    input.resources[2] = 526; input.resources[3] = 28;
    if (stars_build_production_item(&input) != 0 || item.quantity != 1 ||
            item.progress != 81 || partial.quantity != 0 ||
            status != 6 || trace.built != 0 ||
            input.resources[0] != 592 || input.resources[1] != 964 ||
            input.resources[2] != 520 || input.resources[3] != 0) {
        fprintf(stderr, "PARTIAL qty=%d pct=%d out=%d built=%d status=%d res=%d,%d,%d,%d\n",
            item.quantity, item.progress, partial.quantity, trace.built,
            status, input.resources[0],
            input.resources[1], input.resources[2], input.resources[3]);
        return 2;
    }

    memset(&partial, 0, sizeof partial);
    input.resources[0] = 4; input.resources[1] = 1;
    input.resources[2] = 2; input.resources[3] = 7;
    if (stars_build_production_item(&input) != 1 || item.quantity != 0 ||
            item.progress != 0 ||
            partial.quantity != 0 || trace.built != 1 ||
            trace.messages != 1 || status != 0 ||
            input.resources[0] != 0 || input.resources[1] != 0 ||
            input.resources[2] != 0 || input.resources[3] != 0)
        return 3;
    puts("C_BUILD_PROD_ITEM_SOURCE=PASS");
    return 0;
}
