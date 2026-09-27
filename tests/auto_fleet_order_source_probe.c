#include <stdio.h>
#include "core.h"

typedef struct Trace { StarsI16 mine_calls, stat_calls; } Trace;
static StarsI32 mine(void *value, const StarsAutoFleet *fleet)
{ Trace *t=(Trace *)value; t->mine_calls++; return fleet->token==20UL?100L:10L; }
static StarsI16 stat(void *value, StarsI16 player, StarsI16 index)
{ Trace *t=(Trace *)value; t->stat_calls++; (void)player; (void)index; return 8; }

int main(void)
{
    StarsAutoFleetOrder order;
    StarsAutoFleet fleet;
    StarsAutoFleet other;
    StarsAutoFleetPlanet planet;
    StarsAutoFleetOrderInput input;
    Trace trace;

    order.flags=0xfff0U; order.target_token=0;
    fleet.player=2; fleet.point.x=4; fleet.point.y=5; fleet.flags=0;
    fleet.token=20UL; fleet.plan_index=9; fleet.order=&order;
    other.player=2; other.point=fleet.point; other.flags=0;
    other.token=30UL; other.plan_index=0; other.order=(StarsAutoFleetOrder *)0;
    planet.player=-1; trace.mine_calls=0; trace.stat_calls=0;
    input.fleet=&fleet; input.planet=&planet; input.other_fleets=&other;
    input.other_fleet_count=1; input.mine_value=mine; input.race_stat=stat;
    input.context=&trace;
    stars_auto_fleet_order(&input);
    if (fleet.order->flags!=0xf2f4U || fleet.order->target_token!=30UL ||
        fleet.plan_index!=0 || trace.stat_calls!=1 || trace.mine_calls!=2)
        return 1;
    planet.player=2; order.flags=0xffffU; order.target_token=0;
    fleet.plan_index=9; trace.mine_calls=0;
    stars_auto_fleet_order(&input);
    if (fleet.order->flags!=0xfff3U || fleet.plan_index!=0 ||
        trace.mine_calls!=1)
        return 2;
    puts("AUTO_FLEET_ORDER_SOURCE=PASS"); return 0;
}
