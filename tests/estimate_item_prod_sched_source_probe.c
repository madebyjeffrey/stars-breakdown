#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace { StarsI16 mined; StarsI16 builds; StarsI16 population; StarsU16 last_quantity; } Trace;
static void mined(void *c, StarsEstimateProdPlanet *p, StarsI32 m[3]){(void)p;++((Trace*)c)->mined;m[0]=10;m[1]=20;m[2]=30;}
static StarsI32 resources(void *c, const StarsEstimateProdPlanet *p){(void)c;(void)p;return 40;}
static StarsI16 build(void *c, StarsEstimateProdPlanet *p, const StarsEstimateProdItem *i, StarsEstimateProdItem *partial, const StarsI32 m[3], StarsI16 f, StarsI16 *phase){Trace*t=(Trace*)c;(void)p;(void)partial;(void)m;(void)f;++t->builds;t->last_quantity=i->quantity;*phase=0;return 1;}
static void population(void *c, StarsEstimateProdPlanet *p, StarsI16 n){(void)p;(void)n;++((Trace*)c)->population;}
typedef struct Simulation { int calls, failures, years; } Simulation;
static StarsI16 mutable_build(void *opaque, StarsEstimateProdPlanet *planet,
    StarsEstimateProdItem *item, StarsEstimateProdItem *partial,
    StarsI32 budget[4], StarsI16 alchemy, StarsI16 *phase)
{
    Simulation *simulation = (Simulation *)opaque;
    ++simulation->calls;
    if (alchemy) ++simulation->failures;
    if (simulation->calls == 1) {
        if (budget[0] != 109 || budget[3] != 30 || !partial)
            ++simulation->failures;
        item->progress_kind = 17;
        partial->quantity = 1;
        partial->packed = (1UL << 17) | (8UL << 10) | 1UL;
        budget[0] -= 5; budget[3] -= 11;
        *phase = 5; return 0;
    }
    if (simulation->calls == 2) {
        if (partial || budget[0] != 114 || planet->population != 11)
            ++simulation->failures;
        item->quantity = 0; budget[3] -= 3;
        *phase = 0; return 1;
    }
    if (simulation->calls == 3) {
        if (item->progress_kind != 17 || budget[3] != 27 ||
                planet->mines != 3 || !partial || partial->quantity)
            ++simulation->failures;
        item->quantity = 0; *phase = 0; return 1;
    }
    ++simulation->failures; *phase = 0; return 0;
}
static void simulated_population(void *opaque, StarsEstimateProdPlanet *planet,
    StarsI16 years)
{
    Simulation *simulation = (Simulation *)opaque;
    simulation->years += years; planet->population += years;
}
static StarsI16 alchemy_build(void *opaque, StarsEstimateProdPlanet *planet,
    StarsEstimateProdItem *item, StarsEstimateProdItem *partial,
    StarsI32 budget[4], StarsI16 alchemy, StarsI16 *phase)
{
    Simulation *simulation = (Simulation *)opaque;
    (void)planet; (void)budget; (void)partial;
    ++simulation->calls;
    if (alchemy != 1 || ((item->packed >> 17) & 7UL) != 2UL)
        ++simulation->failures;
    *phase = 0; return 1;
}
static void simulation_mined(void *opaque, StarsEstimateProdPlanet *planet, StarsI32 out[3])
{ (void)opaque; (void)planet; out[0] = 10; out[1] = 20; out[2] = 30; }
static int mutable_simulation(void)
{
    StarsEstimateProdPlanet planet, before;
    StarsEstimateProdItem queue[2], saved[2];
    StarsEstimateProdInput input;
    StarsI16 first, last;
    Simulation simulation;
    memset(&planet, 0, sizeof planet); memset(queue, 0, sizeof queue);
    memset(&input, 0, sizeof input); memset(&simulation, 0, sizeof simulation);
    planet.minerals[0] = 99; planet.population = 10;
    planet.research_percent = 25; planet.mines = 2;
    queue[0].quantity = 1; queue[0].packed = (2UL << 17) | 1UL;
    before = planet; memcpy(saved, queue, sizeof queue);
    input.planet = &planet; input.queue = queue; input.queue_count = 1;
    input.first_turn = &first; input.last_turn = &last;
    input.estimate_mined = mined; input.resources_at_planet = resources;
    input.build_item_mutable = mutable_build;
    input.change_population = simulated_population; input.context = &simulation;
    input.estimate_mined = simulation_mined;
    stars_estimate_item_prod_schedule(&input);
    if (first != 2 || last != 2 || simulation.calls != 3 || simulation.years != 1 ||
            simulation.failures || memcmp(&planet, &before, sizeof planet) ||
            memcmp(queue, saved, sizeof queue)) return 1;
    memset(&simulation, 0, sizeof simulation);
    queue[0].packed = (1UL << 17) | (3UL << 10) | 1UL;
    queue[1].packed = (2UL << 17) | 1UL; queue[1].quantity = 1;
    input.queue_count = 2; input.item_index = 1;
    input.build_item_mutable = alchemy_build;
    stars_estimate_item_prod_schedule(&input);
    if (first != 1 || last != 1 || simulation.calls != 1 || simulation.failures)
        return 2;
    return 0;
}
int main(void)
{
    StarsEstimateProdPlanet planet; StarsEstimateProdItem queue[1]; StarsEstimateProdInput input; StarsI16 first,last; Trace trace;
    memset(&planet,0,sizeof planet);memset(queue,0,sizeof queue);memset(&input,0,sizeof input);memset(&trace,0,sizeof trace);
    planet.minerals[0]=99;
    queue[0].quantity=1;input.planet=&planet;input.queue=queue;input.queue_count=1;input.item_index=0;input.first_turn=&first;input.last_turn=&last;input.estimate_mined=mined;input.resources_at_planet=resources;input.build_item=build;input.change_population=population;input.context=&trace;
    stars_estimate_item_prod_schedule(&input);
    if(first!=1||last!=1||trace.mined!=1||trace.builds!=1||planet.minerals[0]!=99)return 1;
    input.item_index=-2;stars_estimate_item_prod_schedule(&input);if(first!=40||last!=0)return 2;
    planet.research_percent=25;input.item_index=-1;stars_estimate_item_prod_schedule(&input);if(first!=40||last!=0)return 3;
    memset(&trace,0,sizeof trace);memset(queue,0,sizeof queue);
    queue[0].quantity=1;queue[0].packed=(3UL<<10)|(1UL<<17);
    input.item_index=0;input.queue_count=1;input.context=&trace;
    stars_estimate_item_prod_schedule(&input);
    if(first!=1||last!=1||trace.last_quantity!=0x3fcU)return 4;
    if (mutable_simulation()) return 5;
    puts("ESTIMATE_ITEM_PROD_SCHED_SOURCE=PASS");return 0;
}
