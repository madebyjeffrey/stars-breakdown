#include "core.h"

#define AUTO_ROUTE_ORDER_ROUTE 8U
#define AUTO_ROUTE_TARGET_PLANET 1U
#define AUTO_ROUTE_WARP_MASK 0x00f0U
#define AUTO_ROUTE_TARGET_MASK 0x0f00U
#define AUTO_ROUTE_PLANET_GATE 0x0200U
#define AUTO_ROUTE_PLANET_TARGET 0x03ffU
#define AUTO_ROUTE_MIN_WARP 3
#define AUTO_ROUTE_MAX_WARP 10
#define AUTO_ROUTE_STARGATE_WARP 11

static StarsI32 auto_route_distance(const StarsAutoRouteInput *input,
    StarsI16 x1, StarsI16 y1, StarsI16 x2, StarsI16 y2)
{
    if (input->ops->distance)
        return input->ops->distance(input->ops->context, x1, y1, x2, y2);
    return (StarsI32)(x2 > x1 ? x2 - x1 : x1 - x2) +
        (StarsI32)(y2 > y1 ? y2 - y1 : y1 - y2);
}

static StarsI32 auto_route_fuel(const StarsAutoRouteInput *input,
    StarsI16 warp, StarsI32 fuel_distance, StarsI16 max_cargo)
{
    if (input->ops->estimate_fuel)
        return input->ops->estimate_fuel(input->ops->context, input->fleet,
            0, warp, fuel_distance, max_cargo);
    return fuel_distance;
}

static StarsI16 auto_route_best_cargo(const StarsAutoRouteFleet *fleet)
{
    StarsI16 slot;
    StarsI32 best;

    best = 0;
    for (slot = 0; slot < 16; ++slot)
        if (fleet->cargo[slot] != 0 &&
            fleet->cargo_hull_capacity[slot] > best)
            best = fleet->cargo_hull_capacity[slot];
    return (StarsI16)best;
}

static StarsI16 auto_route_has_minerals(const StarsAutoRouteFleet *fleet)
{
    StarsI16 index;

    for (index = 0; index < 4; ++index)
        if (fleet->mineral_weights[index] != 0)
            return 1;
    return 0;
}

/* Genuine source reconstruction of AutoRouteFleet, NE 11:1E52-227C. */
void stars_auto_route_fleet(StarsAutoRouteInput *input)
{
    StarsAutoRouteOrder *order;
    StarsAutoRoutePlanet *target;
    StarsI16 warp;
    StarsI16 lower_warp;
    StarsI16 damage;
    StarsI16 max_cargo;
    StarsI16 slot;
    StarsI32 distance;
    StarsI32 fuel_at_warp;
    StarsI32 fuel_at_lower;

    if (!input || !input->fleet || !input->source || !input->ops ||
        !input->fleet->orders)
        return;
    order = &input->fleet->orders[1];
    input->fleet->cord = 2;
    input->fleet->orders[0].flags = 0;
    order->flags = (StarsU16)(AUTO_ROUTE_ORDER_ROUTE << 0);
    order->flags = (StarsU16)(order->flags | AUTO_ROUTE_TARGET_PLANET << 8);
    if (!input->ops->find_planet)
        return;
    target = input->ops->find_planet(input->ops->context,
        input->source->route_target_id);
    if (!target)
        return;
    order->target_id = target->id;
    order->x = target->x;
    order->y = target->y;
    order->flags = (StarsU16)(order->flags | 0x1000U);
    warp = input->ops->ideal_warp ? input->ops->ideal_warp(
        input->ops->context, input->fleet, 0) : 0;
    distance = auto_route_distance(input, input->fleet->x, input->fleet->y,
        order->x, order->y);
    if (!auto_route_has_minerals(input->fleet) &&
        input->source->owner == target->owner &&
        (input->source->status_flags & AUTO_ROUTE_PLANET_GATE) != 0U &&
        (target->status_flags & AUTO_ROUTE_PLANET_GATE) != 0U &&
        input->source->stargate != -1 && target->stargate != -1) {
        max_cargo = auto_route_best_cargo(input->fleet);
        damage = 0;
        if (input->ops->gate_damage && input->ops->gate_damage(
                input->ops->context, input->source->stargate,
                target->stargate, distance, max_cargo, &damage) && damage == 0)
            warp = AUTO_ROUTE_STARGATE_WARP;
        if (warp < 9 && target->hull_cargo_max != 0) {
            for (slot = 9; slot > 0; --slot) {
                fuel_at_warp = auto_route_fuel(input, slot, -1, 1);
                if (distance <= fuel_at_warp)
                    break;
            }
            warp = slot;
        }
    }
    if (warp > 0 && warp < AUTO_ROUTE_STARGATE_WARP) {
        fuel_at_warp = distance / ((StarsI32)warp * warp);
        lower_warp = warp;
        while (lower_warp >= AUTO_ROUTE_MIN_WARP) {
            --lower_warp;
            if (lower_warp <= 0)
                break;
            fuel_at_lower = distance / ((StarsI32)lower_warp * lower_warp);
            if (fuel_at_lower > fuel_at_warp) {
                ++lower_warp;
                break;
            }
            fuel_at_warp = fuel_at_lower;
        }
        warp = lower_warp;
        while (warp > 0) {
            fuel_at_warp = auto_route_fuel(input, warp, distance, 0);
            if (fuel_at_warp <= input->fleet->fuel_capacity || warp == 1)
                break;
            --warp;
        }
    }
    order->flags = (StarsU16)(order->flags & (StarsU16)~AUTO_ROUTE_WARP_MASK);
    order->flags = (StarsU16)(order->flags |
        ((StarsU16)warp << 4));
}
