#include "core.h"

static StarsU32 fleet_visibility_distance(StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    StarsI32 dx = (StarsI32)x1 - (StarsI32)x2;
    StarsI32 dy = (StarsI32)y1 - (StarsI32)y2;
    return (StarsU32)(dx * dx + dy * dy);
}

static void fleet_detail(StarsU16 *details, StarsU16 count,
    StarsI16 owner)
{
    StarsU16 flags;

    if (!details || owner < 0 || owner >= (StarsI16)count) return;
    flags = details[(StarsU16)owner];
    if ((flags & 0x0100U) == 0U)
        details[(StarsU16)owner] = (StarsU16)((flags & 0xfff8U) |
            0x0100U | 3U);
}

static void vis_fleet_mark_fleet(const StarsSetVisPFFleetsOps *ops,
    void *context, StarsU16 id, StarsI16 detail)
{
    if (ops && ops->mark_fleet)
        ops->mark_fleet(context, id, detail);
}

static void vis_fleet_mark_planet(const StarsSetVisPFFleetsOps *ops,
    void *context, StarsU16 id, StarsI16 viewer, StarsI16 detail)
{
    if (ops && ops->mark_planet)
        ops->mark_planet(context, id, viewer, detail);
}

static StarsI16 vis_fleet_cloak(const StarsSetVisPFFleetsOps *ops,
    void *context, const StarsVisFleetScanFleet *fleet)
{
    if (ops && ops->cloak_percent)
        return ops->cloak_percent(context, fleet);
    return fleet->cloak_percent;
}

static StarsU32 attenuated_radius(StarsU32 radius_squared,
    StarsI16 cloak_percent)
{
    StarsI32 visible_percent = 100 - (StarsI32)cloak_percent;
    StarsU32 result;

    if (visible_percent <= 0) return 0UL;
    result = (radius_squared * (StarsU32)visible_percent) / 100UL;
    result = (result * (StarsU32)visible_percent) / 100UL;
    return result;
}

static StarsVisThingPlanet *find_visibility_planet(
    StarsSetVisPFFleetsInput *input, StarsI16 id)
{
    StarsU16 i;

    if (!input || id < 0) return (StarsVisThingPlanet *)0;
    for (i = 0; i < input->planet_count; ++i)
        if (input->planets[i].id == (StarsU16)id)
            return &input->planets[i];
    return (StarsVisThingPlanet *)0;
}

static void reveal_fleet_things(StarsSetVisPFFleetsInput *input,
    const StarsSetVisPFFleetsOps *ops, void *context,
    const StarsVisFleetScanFleet *source, StarsU32 scanner_squared,
    StarsU32 planet_squared, StarsU32 viewer_mask)
{
    StarsU16 i;
    StarsU16 kind;
    StarsU32 distance;
    StarsU32 partial_radius;
    StarsVisThingTarget *thing;

    (void)ops; (void)context;
    if (input->viewer == (StarsI16)-1) return;
    for (i = 0; i < input->thing_count; ++i) {
        thing = &input->things[i];
        kind = thing->kind;
        if (kind > 3U) continue;

        /* Kind 0 is the only object allowed to begin outside the fleet's
         * ordinary scanner circle; its masks and concealment gate decide
         * whether the long-range reveal is sufficient. */
        if (kind == 0U && (thing->bits_8 & viewer_mask) != 0UL)
            continue;
        if (kind == 3U && (thing->bits_4 & 0x0010UL) != 0UL)
            continue;
        if (kind == 1U && (thing->dest_x & 0x8000U) != 0U)
            continue;
        if (kind == 2U && (thing->dest_x & 0x2000U) != 0U)
            continue;

        distance = fleet_visibility_distance(source->x, source->y,
            thing->x, thing->y);
        if (distance > scanner_squared && kind != 0U)
            continue;

        if (kind == 1U) {
            thing->dest_x = (StarsU16)((thing->dest_x & 0x7fffU) |
                0x8000U);
            fleet_detail(input->player_detail_flags, input->player_count,
                thing->owner);
        } else if (kind == 3U) {
            thing->bits_4 |= 0x0010UL;
        } else if (kind == 2U) {
            if ((thing->bits_2 & viewer_mask) == 0UL) {
                partial_radius = scanner_squared / 16UL;
                if (partial_radius < distance &&
                        planet_squared < distance)
                    continue;
            }
            thing->bits_2 |= viewer_mask;
            thing->dest_x = (StarsU16)((thing->dest_x & 0xdfffU) |
                0x2000U);
        } else {
            if (((thing->bits_4 & viewer_mask) == 0UL ||
                    scanner_squared < distance) &&
                    planet_squared < distance) {
                partial_radius = scanner_squared / 16UL;
                if (partial_radius < distance &&
                        thing->concealment_threshold < distance)
                    continue;
            }
            thing->bits_4 |= viewer_mask;
            thing->bits_8 |= viewer_mask;
            fleet_detail(input->player_detail_flags, input->player_count,
                thing->owner);
        }
    }
}

static void reveal_fleet_planets(StarsSetVisPFFleetsInput *input,
    const StarsSetVisPFFleetsOps *ops, void *context,
    const StarsVisFleetScanFleet *source, StarsI16 planet_range)
{
    StarsU16 i;
    StarsU32 radius_squared;
    StarsU32 distance;
    StarsU32 starbase_radius;
    StarsVisThingPlanet *planet;

    if (planet_range <= 0) return;
    radius_squared = (StarsU32)((StarsI32)planet_range *
        (StarsI32)planet_range);
    for (i = 0; i < input->planet_count; ++i) {
        planet = &input->planets[i];
        if ((planet->flags & 0x0100U) != 0U &&
                (planet->flags & 0x00ffU) >= 3U)
            continue;
        distance = fleet_visibility_distance(source->x, source->y,
            planet->x, planet->y);
        if (distance > radius_squared) continue;
        if ((planet->flags & 0x0200U) != 0U && planet->owner != -1 &&
                planet->starbase_delta <= 0 &&
                (planet->starbase_delta < 0 ||
                 planet->starbase_count < 10000UL)) {
            starbase_radius = (radius_squared * planet->starbase_count) /
                10000UL;
            if (starbase_radius < distance) {
                vis_fleet_mark_planet(ops, context, planet->id,
                    input->viewer, 2);
                continue;
            }
        }
        vis_fleet_mark_planet(ops, context, planet->id,
            input->viewer, 3);
    }
}

/* NE 15:a100.  SetVisPFFleets applies the scanner geometry of each owned
 * fleet to fleets, special things, and planets, while also recognizing an
 * enemy fleet sitting on one of the viewer's known planets. */
void stars_set_vis_pf_fleets(StarsSetVisPFFleetsInput *input,
    const StarsSetVisPFFleetsOps *ops, void *context)
{
    StarsU16 i;
    StarsU16 j;
    StarsU32 viewer_mask;
    StarsU16 scanner_bits;
    StarsU16 scanner_range;
    StarsI16 planet_range;
    StarsI16 detection_percent;
    StarsU32 scanner_squared;
    StarsU32 planet_squared;
    StarsU32 distance;
    StarsI16 cloak;
    StarsU32 cloak_radius;
    StarsVisFleetScanFleet *source;
    StarsVisFleetScanFleet *target;
    StarsVisThingPlanet *planet;

    if (!input || !input->fleets) return;
    viewer_mask = input->viewer == (StarsI16)-1 ? 0UL :
        (StarsU32)1UL << ((StarsU16)input->viewer & 31U);

    for (i = 0; i < input->fleet_count; ++i) {
        source = &input->fleets[i];
        if ((source->flags & 0x0400U) != 0U) continue;

        if (source->player != input->viewer) {
            if ((source->flags & 0x0100U) == 0U && source->planet_id != -1) {
                planet = find_visibility_planet(input, source->planet_id);
                if (planet && planet->owner == input->viewer)
                    vis_fleet_mark_fleet(ops, context, source->id, 3);
            }
            continue;
        }

        if ((source->flags & 0x1000U) != 0U && source->planet_id != -1) {
            planet = find_visibility_planet(input, source->planet_id);
            if (planet)
                vis_fleet_mark_planet(ops, context, planet->id,
                    input->viewer, 3);
        }

        scanner_range = source->scanner_range;
        planet_range = source->planet_range;
        detection_percent = source->detection_percent;
        scanner_bits = source->scanner_bits;
        if (ops && ops->scanner)
            scanner_range = ops->scanner(context, source, &planet_range,
                &detection_percent, &scanner_bits);
        if (scanner_range > 0x7fffU) scanner_range = 0U;
        scanner_squared = (StarsU32)scanner_range *
            (StarsU32)scanner_range;
        planet_squared = (StarsU32)((StarsI32)planet_range *
            (StarsI32)planet_range);

        for (j = 0; j < input->fleet_count; ++j) {
            target = &input->fleets[j];
            if ((target->flags & 0x0400U) != 0U) continue;
            if ((scanner_bits & 1U) != 0U && source->x == target->x &&
                    source->y == target->y &&
                    ((target->flags & 0x0100U) == 0U ||
                     (target->flags & 0x00ffU) < 4U))
                vis_fleet_mark_fleet(ops, context, target->id, 4);
            if ((target->flags & 0x0100U) != 0U) continue;

            distance = fleet_visibility_distance(source->x, source->y,
                target->x, target->y);
            if (distance > scanner_squared) continue;
            if (target->planet_id != -1 && distance > planet_squared)
                continue;
            cloak = vis_fleet_cloak(ops, context, target);
            if (detection_percent != 100)
                cloak = (StarsI16)(((StarsI32)cloak * detection_percent) /
                    100);
            if (cloak == 0) {
                vis_fleet_mark_fleet(ops, context, target->id, 3);
                continue;
            }
            cloak_radius = attenuated_radius(scanner_squared, cloak);
            if (distance > cloak_radius) continue;
            if (target->planet_id != -1) {
                cloak_radius = attenuated_radius(planet_squared, cloak);
                if (cloak_radius < distance) continue;
            }
            vis_fleet_mark_fleet(ops, context, target->id, 3);
        }

        reveal_fleet_things(input, ops, context, source, scanner_squared,
            planet_squared, viewer_mask);
        if ((scanner_bits & 2U) != 0U && source->planet_id != -1) {
            planet = find_visibility_planet(input, source->planet_id);
            if (planet)
                vis_fleet_mark_planet(ops, context, planet->id,
                    input->viewer, 4);
        }
        reveal_fleet_planets(input, ops, context, source, planet_range);
    }
}
