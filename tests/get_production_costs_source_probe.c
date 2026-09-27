#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 stat;
    StarsI16 grbit;
    StarsI16 part_lookup;
    StarsI16 hull_lookup;
} Trace;

static StarsI16 stat(void *p, StarsI16 player, StarsI16 which)
{
    Trace *t = (Trace *)p; (void)player; ++t->stat;
    return which == 5 ? 31 : which == 0x0e ? 4 : 0;
}
static StarsI16 grbit(void *p, StarsI16 player, StarsI16 which)
{ Trace *t = (Trace *)p; (void)player; (void)which; ++t->grbit; return 0; }
static StarsI16 part(void *p, StarsU16 group, StarsU16 subtype,
    StarsTruePart *out)
{
    Trace *t = (Trace *)p; (void)group; (void)subtype; ++t->part_lookup;
    memset(out, 0, sizeof(*out)); out->base_costs[0] = 10;
    out->base_costs[1] = 20; out->base_costs[2] = 30; out->base_costs[3] = 40;
    return 1;
}
static void part_cost(void *p, StarsI16 player, const StarsTruePart *part,
    StarsU16 costs[4])
{ (void)p; (void)player; costs[0]=part->base_costs[0]; costs[1]=part->base_costs[1]; costs[2]=part->base_costs[2]; costs[3]=part->base_costs[3]; }
static StarsI16 hull(void *p, StarsI16 player, StarsU16 item, StarsI16 special,
    StarsProductionHull *out)
{ Trace *t = (Trace *)p; (void)player; (void)item; (void)special; ++t->hull_lookup; memset(out,0,sizeof(*out)); out->base_costs[0]=10; out->base_costs[1]=20; out->base_costs[2]=30; out->base_costs[3]=40; return 1; }

int main(void)
{
    StarsProductionCostOps ops;
    StarsProductionCostInput in;
    StarsU32 costs[4];
    Trace t;
    memset(&ops, 0, sizeof ops); memset(&in, 0, sizeof in); memset(&t, 0, sizeof t);
    ops.race_stat=stat; ops.race_grbit=grbit; ops.lookup_part=part;
    ops.part_cost=part_cost; ops.lookup_hull=hull; in.ops=&ops; in.context=&t;
    in.player=0; in.planet_player=0; in.item_kind=0; in.quantity=3;
    stars_get_production_costs(&in, costs);
    if (costs[0] != 0 || costs[1] != 0 || costs[2] != 0 || costs[3] != 93) return 1;
    in.item_kind=2; in.quantity=1; stars_get_production_costs(&in, costs);
    if (costs[0] != 6 || costs[1] != 12 || costs[2] != 18 || costs[3] != 24) return 2;
    in.item_variant=2; in.item_kind=1; in.quantity=2;
    stars_get_production_costs(&in, costs);
    if (costs[0] != 20 || costs[1] != 40 || costs[2] != 60 || costs[3] != 80) return 3;
    puts("GET_PRODUCTION_COSTS_SOURCE=PASS"); return 0;
}
