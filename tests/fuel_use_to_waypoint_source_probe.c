#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 estimates;
    StarsI16 last_estimate_flag;
    StarsI16 planet_queries;
    StarsI32 ram_gain;
    double precise;
} Trace;

static StarsI32 distance(void *context, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    (void)context;
    (void)y1;
    (void)y2;
    return x2 - x1;
}

static double precise_distance(void *context, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    (void)x1; (void)y1; (void)x2; (void)y2;
    return ((Trace *)context)->precise;
}

static StarsI32 estimate(void *context, const StarsFuelWaypointFleet *fleet,
    StarsI16 segment, StarsI16 warp, StarsI32 fuel, StarsI16 max_cargo)
{
    Trace *trace;
    (void)fleet;
    (void)segment;
    (void)warp;
    trace = (Trace *)context;
    ++trace->estimates;
    trace->last_estimate_flag = max_cargo;
    return fuel == -1L ? 10 : fuel;
}

static StarsI32 ram_scoop(void *context, const StarsFuelWaypointFleet *fleet,
    StarsI16 warp, StarsI32 distance_value)
{
    Trace *trace = (Trace *)context;
    (void)fleet;
    (void)warp;
    (void)distance_value;
    return trace->ram_gain;
}

static StarsI16 cargo_hull(void *context, const StarsFuelWaypointFleet *fleet,
    StarsI16 slot)
{
    (void)context;
    (void)fleet;
    (void)slot;
    return 0x19;
}

static StarsI16 planet_info(void *context, StarsI16 id, StarsI16 *owner,
    StarsU16 *flags, StarsI16 *hull_cargo)
{
    Trace *trace;
    (void)id;
    trace = (Trace *)context;
    ++trace->planet_queries;
    *owner = 4;
    *flags = 0x0200;
    *hull_cargo = 1;
    return 1;
}

int main(void)
{
    StarsFuelWaypointOrder orders[3];
    StarsFuelWaypointFleet fleet;
    StarsFuelWaypointOps ops;
    StarsFuelUseToWaypointInput input;
    Trace trace;
    StarsI32 result;

    (void)memset(orders, 0, sizeof orders);
    (void)memset(&fleet, 0, sizeof fleet);
    (void)memset(&ops, 0, sizeof ops);
    (void)memset(&trace, 0, sizeof trace);
    fleet.owner = 4;
    fleet.cargo[0] = 1;
    fleet.orders = orders;
    orders[0].x = 0;
    orders[1].x = 10;
    orders[1].flags = (StarsU16)(2U << 4);
    orders[2].x = 20;
    orders[2].target_id = 7;
    orders[2].flags = (StarsU16)((2U << 4) | 0x0100U);
    ops.distance = distance;
    ops.estimate_fuel = estimate;
    ops.ram_scoop_gain = ram_scoop;
    ops.cargo_hull_type = cargo_hull;
    ops.planet_info = planet_info;
    ops.context = &trace;
    input.fleet = &fleet;
    input.waypoint_count = 2;
    input.max_cargo = 1;
    input.ops = &ops;
    result = stars_fuel_use_to_waypoint(&input);
    if (result != 8 || trace.estimates != 6 ||
        trace.last_estimate_flag != 0 || trace.planet_queries != 1)
        return 1;
    orders[1].flags = 0;
    result = stars_fuel_use_to_waypoint(&input);
    if (result != 4)
        return 2;
    trace.ram_gain = 2;
    fleet.cargo[0] = 0;
    orders[1].flags = (StarsU16)(2U << 4);
    result = stars_fuel_use_to_waypoint(&input);
    if (result != 16)
        return 3;
    trace.ram_gain = 3;
    result = stars_fuel_use_to_waypoint(&input);
    if (result != 12)
        return 4;
    input.waypoint_count = 1;
    ops.precise_distance = precise_distance;
    trace.precise = 7.0;
    if (stars_fuel_use_to_waypoint(&input) != 5) return 5;
    trace.precise = 7.001;
    if (stars_fuel_use_to_waypoint(&input) != 6) return 6;
    puts("FUEL_USE_TO_WAYPOINT_SOURCE=PASS");
    return 0;
}
