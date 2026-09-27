#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsAutoRoutePlanet target;
    StarsI16 gate_calls;
    StarsI16 fuel_calls;
    StarsI16 allow_gate;
} Trace;

static StarsI16 ideal_warp(void *context, const StarsAutoRouteFleet *fleet,
    StarsI16 order)
{
    (void)context;
    (void)fleet;
    (void)order;
    return 5;
}

static StarsI32 distance(void *context, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    (void)context;
    (void)y1;
    (void)y2;
    return x2 - x1;
}

static StarsAutoRoutePlanet *find_planet(void *context, StarsI16 id)
{
    Trace *trace;
    trace = (Trace *)context;
    return id == trace->target.id ? &trace->target :
        (StarsAutoRoutePlanet *)0;
}

static StarsI16 gate_damage(void *context, StarsI16 source_gate,
    StarsI16 target_gate, StarsI32 distance_value, StarsI32 cargo,
    StarsI16 *damage)
{
    Trace *trace;
    (void)source_gate;
    (void)target_gate;
    (void)distance_value;
    (void)cargo;
    trace = (Trace *)context;
    ++trace->gate_calls;
    *damage = 0;
    return trace->allow_gate;
}

static StarsI32 estimate_fuel(void *context, const StarsAutoRouteFleet *fleet,
    StarsI16 segment, StarsI16 warp, StarsI32 distance_value,
    StarsI16 max_cargo)
{
    Trace *trace;
    (void)fleet;
    (void)segment;
    (void)max_cargo;
    trace = (Trace *)context;
    ++trace->fuel_calls;
    return distance_value < 0 ? 1000 / warp : distance_value / warp;
}

int main(void)
{
    StarsAutoRouteOrder orders[2];
    StarsAutoRouteFleet fleet;
    StarsAutoRoutePlanet source;
    StarsAutoRouteOps ops;
    StarsAutoRouteInput input;
    Trace trace;

    (void)memset(orders, 0, sizeof orders);
    (void)memset(&fleet, 0, sizeof fleet);
    (void)memset(&source, 0, sizeof source);
    (void)memset(&ops, 0, sizeof ops);
    (void)memset(&trace, 0, sizeof trace);
    trace.target.id = 3;
    trace.target.owner = 2;
    trace.target.x = 100;
    trace.target.status_flags = 0x0200;
    trace.target.stargate = 7;
    trace.target.hull_cargo_max = 1;
    source.owner = 2;
    source.route_target_id = 3;
    source.status_flags = 0x0200;
    source.stargate = 6;
    fleet.owner = 2;
    fleet.x = 0;
    fleet.fuel_capacity = 1000;
    fleet.orders = orders;
    trace.allow_gate = 1;
    ops.ideal_warp = ideal_warp;
    ops.distance = distance;
    ops.find_planet = find_planet;
    ops.gate_damage = gate_damage;
    ops.estimate_fuel = estimate_fuel;
    ops.context = &trace;
    input.fleet = &fleet;
    input.source = &source;
    input.ops = &ops;
    stars_auto_route_fleet(&input);
    if (fleet.cord != 2 || fleet.orders[1].target_id != 3 || fleet.orders[1].x != 100 ||
        ((fleet.orders[1].flags >> 4) & 0x0fU) != 11U ||
        trace.gate_calls != 1)
        return 1;
    trace.allow_gate = 0;
    fleet.fuel_capacity = 10;
    stars_auto_route_fleet(&input);
    if (((fleet.orders[1].flags >> 4) & 0x0fU) != 1U ||
        trace.fuel_calls == 0)
        return 2;
    trace.gate_calls = 0;
    fleet.mineral_weights[0] = 1;
    fleet.fuel_capacity = 1000;
    stars_auto_route_fleet(&input);
    if (trace.gate_calls != 0)
        return 3;
    puts("AUTO_ROUTE_FLEET_SOURCE=PASS");
    return 0;
}
