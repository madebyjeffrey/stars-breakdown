#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsColonistPlanet planet;
    StarsI16 claimed;
    StarsI16 production;
    StarsI16 research;
    StarsI16 messages;
} Trace;

static StarsI16 lookup(void *p, StarsI16 id, StarsColonistPlanet *planet)
{ Trace *t = (Trace *)p; if (id != t->planet.id) return 0; *planet = t->planet; return 1; }
static StarsI16 survival(void *p, const StarsColonistPlanet *planet,
    StarsI16 owner, StarsI32 input, StarsI32 *output)
{ (void)p; (void)planet; (void)owner; *output = input / 2; return 1; }
static StarsI16 race_stat(void *p, StarsI16 owner, StarsI16 stat)
{ (void)p; (void)owner; (void)stat; return 8; }
static void claim(void *p, StarsColonistPlanet *planet, StarsI16 owner)
{ Trace *t = (Trace *)p; t->planet = *planet; t->planet.owner = owner; ++t->claimed; }
static void production(void *p, StarsColonistPlanet *planet, StarsI16 owner)
{ Trace *t = (Trace *)p; (void)planet; (void)owner; ++t->production; }
static void research(void *p, StarsI16 owner, StarsI16 planet)
{ Trace *t = (Trace *)p; (void)owner; (void)planet; ++t->research; }
static void message(void *p, StarsI16 owner, StarsI16 code, StarsI16 planet,
    StarsI16 value, StarsI16 b, StarsI16 c)
{ Trace *t = (Trace *)p; (void)owner; (void)code; (void)planet; (void)value; (void)b; (void)c; ++t->messages; }

int main(void)
{
    StarsColonistDrop drop;
    StarsDropColonistsOps ops;
    StarsDropColonistsInput input;
    Trace trace;

    memset(&drop, 0, sizeof drop); memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    trace.planet.id = 7; trace.planet.owner = -1;
    trace.planet.capacity = 100; trace.planet.climate_factor = 100;
    drop.source_owner = 2; drop.target_planet = 7; drop.colonists = 20;
    drop.active = 1;
    ops.context = &trace; ops.lookup_planet = lookup; ops.survival = survival;
    ops.race_stat = race_stat; ops.claim_planet = claim;
    ops.init_production = production; ops.learn_research = research;
    ops.send_message = message;
    input.drops = &drop; input.drop_count = 1; input.ops = &ops;
    stars_drop_colonists(&input);
    if (drop.active || drop.target_planet != -1 || trace.claimed != 1 ||
            trace.production != 1 || trace.research != 1 || trace.messages < 2)
        return 1;
    puts("DROP_COLONISTS_SOURCE=PASS");
    return 0;
}
