#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsU16 random_calls; } Trace;
static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{ (void)context; (void)player; (void)stat; return 2; }
static StarsU32 hull_stat(void *context, const StarsSpeedDesign *design,
    StarsI16 stat)
{ (void)context; (void)design; (void)stat; return 100UL; }
static StarsU32 fleet_stat(void *context, const StarsSpeedFleet *fleet,
    StarsI16 stat)
{ (void)context; (void)fleet; (void)stat; return 100UL; }
static StarsI16 random_value(void *context, StarsI16 limit)
{ Trace *trace=(Trace *)context; ++trace->random_calls; return limit == 15 ? 3 : 0; }
static StarsI16 engine_lookup(void *context, StarsU16 id,
    StarsSpeedEngine *engine)
{ StarsU16 i; (void)context; engine->engine_id=id; for(i=0;i<10U;++i) engine->fuel_used[i]=0U; return 1; }

int main(void)
{
    StarsSpeedComponent components[4];
    StarsSpeedDesign design;
    StarsSpeedFleet fleet;
    StarsSpeedToken token;
    StarsSpeedInput input;
    Trace trace;
    memset(components, 0, sizeof components);
    memset(&design, 0, sizeof design);
    memset(&fleet, 0, sizeof fleet);
    memset(&token, 0, sizeof token);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    components[0].group=1U; components[0].type=7U; components[0].count=1U;
    components[1].group=0x80U; components[1].type=6U; components[1].count=2U;
    components[2].group=0x800U; components[2].type=4U; components[2].count=1U;
    components[3].group=0x1000U; components[3].type=8U; components[3].count=1U;
    design.empty_weight=100U; design.components=components; design.component_count=4U;
    fleet.player=0; fleet.cargo_weight=200UL;
    input.fleet=&fleet; input.design=&design; input.token=&token;
    input.race_stat=race_stat; input.hull_stat=hull_stat;
    input.fleet_stat=fleet_stat; input.random=random_value;
    input.engine_lookup=engine_lookup; input.context=&trace;
    if (stars_speed_of_ship(&input) != 8 || token.weight != 300U ||
            trace.random_calls != 1U || (token.random_speed_bits >> 10) != 3U)
        return 1;
    input.dump_cargo=1;
    if (stars_speed_of_ship(&input) != 7)
        return 1;
    input.fleet=0; input.token=0; input.dump_cargo=0;
    design.component_count=1U; design.empty_weight=140U;
    components[0].type=1U; components[0].count=2U;
    if (stars_speed_of_ship(&input) != 4) return 2;
    components[0].count=1U;
    if (stars_speed_of_ship(&input) != 3) return 3;
    components[0].count=0U;
    if (stars_speed_of_ship(&input) != 0) return 4;
    components[0].count=2U; components[0].type=8U;
    if (stars_speed_of_ship(&input) != 6) return 5;
    design.components=0; design.component_count=0U;
    if (stars_speed_of_ship(&input) != 0)
        return 1;
    puts("SPD_OF_SHIP_SOURCE=PASS");
    return 0;
}
