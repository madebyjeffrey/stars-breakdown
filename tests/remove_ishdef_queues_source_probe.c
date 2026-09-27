#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct ProbeState { int freed; int looked; int filled; } ProbeState;
static StarsI16 dock(void *c, StarsI16 p, StarsU16 d)
{ (void)c; (void)p; return d == 3U ? 0x21 : 0x20; }
static void free_prod(void *c, StarsIshdefProductionPlanet *p)
{ ProbeState *s=(ProbeState *)c; (void)p; ++s->freed; }
static void lookup(void *c, StarsU16 id, StarsIshdefProductionPlanet *p)
{ ProbeState *s=(ProbeState *)c; (void)id; (void)p; ++s->looked; }
static void fill(void *c, StarsIshdefProductionPlanet *p)
{ ProbeState *s=(ProbeState *)c; (void)p; ++s->filled; }
static StarsU32 rec(StarsU16 kind, StarsU16 id)
{ return ((StarsU32)kind << 17) | ((StarsU32)id << 10) | 0x55UL; }

int main(void)
{
    StarsRemoveIshdefQueuesInput in;
    StarsIshdefProductionPlanet planets[3], selected;
    StarsU32 records[4], empty[2], kept[2];
    StarsU16 counts[3], selected_count;
    ProbeState state;

    memset(&in, 0, sizeof(in)); memset(&state, 0, sizeof(state));
    records[0]=rec(2,7); records[1]=rec(2,4); records[2]=rec(1,7);
    counts[0]=3; planets[0].player=1; planets[0].flags=0x0200U;
    planets[0].starbase_design=3; planets[0].production_records=records;
    planets[0].production_count=&counts[0];
    empty[0]=rec(2,7); counts[1]=1; planets[1].player=1;
    planets[1].flags=0x0200U; planets[1].starbase_design=3;
    planets[1].production_records=empty; planets[1].production_count=&counts[1];
    kept[0]=rec(2,7); kept[1]=rec(2,4); selected_count=2;
    selected.player=1; selected.flags=0x0200U;
    selected.production_records=kept; selected.production_count=&selected_count;
    in.planets=planets; in.planet_count=2; in.player=1; in.is_space_dock=dock;
    in.free_production=free_prod; in.context=&state; in.selected_planet=&selected;
    in.selected_id=9; in.selected_is_planet=1; in.lookup_selected=lookup;
    in.fill_selected=fill;
    stars_remove_ishdef_from_all_queues(&in, 7);
    if (counts[0] != 2 || records[0] != rec(2,4) ||
            records[1] != rec(1,7) || counts[1] != 0 ||
            planets[1].production_records != 0 || state.freed != 1 ||
            state.looked != 1 || state.filled != 1) return 1;
    in.space_docks_only=1; planets[0].starbase_design=4; counts[0]=1;
    records[0]=rec(2,7); planets[0].production_records=records;
    stars_remove_ishdef_from_all_queues(&in, 7);
    if (counts[0] != 1 || records[0] != rec(2,7)) return 2;
    puts("REMOVE_ISHDEF_QUEUES_SOURCE=PASS"); return 0;
}
