#include "core.h"

#define FUEL_WP_MAX_WARP 10
#define FUEL_WP_SEGMENT_TARGET_SHIFT 8
#define FUEL_WP_PLANET_TARGET 1U
#define FUEL_WP_CARGO_HULL_A 0x19
#define FUEL_WP_CARGO_HULL_B 0x1a
#define FUEL_WP_CARGO_GAIN 200L
#define FUEL_WP_PLANET_SPECIAL 0x0200U
#define FUEL_WP_HULL_CARGO 0

static StarsI32 fuel_wp_max(StarsI32 a, StarsI32 b)
{
    return a > b ? a : b;
}

static double fuel_wp_distance(const StarsFuelUseToWaypointInput *input,
    StarsI16 index)
{
    const StarsFuelWaypointOrder *from;
    const StarsFuelWaypointOrder *to;

    from = &input->fleet->orders[index];
    to = &input->fleet->orders[index + 1];
    if (input->ops->precise_distance)
        return input->ops->precise_distance(input->ops->context, from->x, from->y,
            to->x, to->y);
    if (input->ops->distance)
        return input->ops->distance(input->ops->context, from->x, from->y,
            to->x, to->y);
    return (StarsI32)(to->x > from->x ? to->x - from->x : from->x - to->x) +
        (StarsI32)(to->y > from->y ? to->y - from->y : from->y - to->y);
}

static StarsI32 fuel_wp_estimate(const StarsFuelUseToWaypointInput *input,
    StarsI16 segment, StarsI16 warp, StarsI32 fuel_parameter)
{
    if (input->ops->estimate_fuel)
        return input->ops->estimate_fuel(input->ops->context, input->fleet,
            segment, warp, fuel_parameter, 0);
    return fuel_parameter < 0 ? fuel_wp_distance(input, segment) :
        fuel_parameter;
}

static StarsI16 fuel_wp_is_cargo_hull(const StarsFuelUseToWaypointInput *input,
    StarsI16 slot)
{
    StarsI16 type;

    if (!input->ops->cargo_hull_type)
        return 0;
    type = input->ops->cargo_hull_type(input->ops->context, input->fleet,
        slot);
    return type == FUEL_WP_CARGO_HULL_A || type == FUEL_WP_CARGO_HULL_B;
}

/* Genuine source reconstruction of LFuelUseToWaypoint, NE 0B:A9F4. */
StarsI32 stars_fuel_use_to_waypoint(StarsFuelUseToWaypointInput *input)
{
    StarsI16 segment;
    StarsI16 warp;
    StarsI16 turns;
    StarsI16 slot;
    StarsI16 planet_owner;
    StarsI16 hull_cargo;
    StarsU16 planet_flags;
    StarsI32 distance;
    double rounded_distance;
    StarsI32 base_fuel;
    StarsI32 per_warp_fuel;
    StarsI32 remainder_fuel;
    StarsI32 ram_gain;
    StarsI32 segment_fuel;
    StarsI32 total_fuel;
    StarsI32 greatest_total;
    StarsI32 adjusted_fuel;
    const StarsFuelWaypointOrder *next;

    if (!input || !input->fleet || !input->fleet->orders || !input->ops ||
        input->waypoint_count <= 0)
        return 0;
    total_fuel = 0;
    greatest_total = 0;
    for (segment = 0; segment < input->waypoint_count; ++segment) {
        next = &input->fleet->orders[segment + 1];
        warp = (StarsI16)((next->flags >> 4) & 0x000fU);
        distance = 0;
        rounded_distance = 0.0;
        if (warp > 0 && warp <= FUEL_WP_MAX_WARP) {
            /* NE 0B:AACB-AB29: retain both original floating constants and
             * two divisions, rather than approximating years as distance/warp. */
            rounded_distance = fuel_wp_distance(input, segment) + 0.99999;
            distance = (StarsI16)rounded_distance;
        }
        if (warp <= 0 || warp > FUEL_WP_MAX_WARP) {
            turns = 1;
            base_fuel = 0;
        } else {
            turns = (StarsI16)(rounded_distance / warp / warp + 0.9999);
            base_fuel = fuel_wp_estimate(input, segment, warp, -1L);
        }
        segment_fuel = base_fuel;
        if (turns > 1) {
            per_warp_fuel = fuel_wp_estimate(input, segment, warp,
                (StarsI32)warp * warp);
            segment_fuel = per_warp_fuel * (turns - 1);
            remainder_fuel = fuel_wp_estimate(input, segment, warp,
                distance - (StarsI32)warp * warp * (turns - 1));
            segment_fuel = fuel_wp_max(segment_fuel + remainder_fuel,
                base_fuel);
            if (input->ops->ram_scoop_gain)
                ram_gain = input->ops->ram_scoop_gain(input->ops->context,
                    input->fleet, warp, (StarsI32)warp * warp);
            else
                ram_gain = 0;
            for (slot = 0; slot < 16; ++slot)
                if (input->fleet->cargo[slot] != 0 &&
                    fuel_wp_is_cargo_hull(input, slot))
                    ram_gain += input->fleet->cargo[slot] * FUEL_WP_CARGO_GAIN;
            if (ram_gain > 0 && ram_gain >= per_warp_fuel) {
                segment_fuel = per_warp_fuel;
            } else if (ram_gain > 0) {
                remainder_fuel = (per_warp_fuel - ram_gain) * (turns - 1);
                adjusted_fuel = per_warp_fuel + remainder_fuel;
                if (adjusted_fuel < segment_fuel)
                    segment_fuel = adjusted_fuel;
            }
        }
        total_fuel += segment_fuel;
        greatest_total = fuel_wp_max(greatest_total, total_fuel);
        if (((next->flags >> FUEL_WP_SEGMENT_TARGET_SHIFT) & 0x000fU) ==
            FUEL_WP_PLANET_TARGET && input->ops->planet_info) {
            planet_flags = 0;
            planet_owner = 0;
            hull_cargo = 0;
            if (input->ops->planet_info(input->ops->context, next->target_id,
                    &planet_owner, &planet_flags, &hull_cargo) &&
                planet_owner == input->fleet->owner &&
                (planet_flags & FUEL_WP_PLANET_SPECIAL) != 0U &&
                hull_cargo != FUEL_WP_HULL_CARGO)
                total_fuel = 0;
        }
    }
    return greatest_total;
}
