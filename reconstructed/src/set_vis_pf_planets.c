#include "core.h"

static StarsU32 planet_visibility_distance(StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    StarsI32 dx = (StarsI32)x1 - (StarsI32)x2;
    StarsI32 dy = (StarsI32)y1 - (StarsI32)y2;
    return (StarsU32)(dx * dx + dy * dy);
}

static StarsU32 planet_range_squared(StarsI16 range)
{
    StarsI32 signed_range = (StarsI32)range;
    return (StarsU32)(signed_range * signed_range);
}

static int planet_is_visible_enough(const StarsVisPlanetScanPlanet *planet)
{
    return (planet->flags & 0x0100U) == 0U ||
        (planet->flags & 0x00ffU) < 3U;
}

static void vis_planet_mark_planet(const StarsSetVisPFPlanetsOps *ops,
    void *context, StarsU16 id, StarsI16 viewer, StarsI16 detail)
{
    if (ops && ops->mark_planet)
        ops->mark_planet(context, id, viewer, detail);
}

static void vis_planet_mark_fleet(const StarsSetVisPFPlanetsOps *ops,
    void *context, StarsU16 id, StarsI16 detail)
{
    if (ops && ops->mark_fleet)
        ops->mark_fleet(context, id, detail);
}

static void planet_detail(StarsSetVisPFPlanetsInput *input, StarsI16 owner)
{
    StarsU16 flags;

    if (!input->player_detail_flags || owner < 0 ||
            owner >= (StarsI16)input->player_count)
        return;
    flags = input->player_detail_flags[(StarsU16)owner];
    if ((flags & 0x0100U) == 0U)
        input->player_detail_flags[(StarsU16)owner] =
            (StarsU16)((flags & 0xfff8U) | 0x0100U | 3U);
}

static StarsI16 planet_scanner(const StarsSetVisPFPlanetsOps *ops,
    void *context, const StarsVisPlanetScanPlanet *planet,
    StarsI16 *deep_range)
{
    if (ops && ops->scanner)
        return ops->scanner(context, planet, deep_range);
    *deep_range = planet->deep_range;
    return planet->scanner_range;
}

static StarsI16 planet_stargate(const StarsSetVisPFPlanetsOps *ops,
    void *context, const StarsVisPlanetScanPlanet *planet,
    StarsI16 viewer, StarsI16 design)
{
    if (ops && ops->stargate_range)
        return ops->stargate_range(context, planet, viewer, design);
    if (planet) return planet->stargate_range;
    return 0;
}

static StarsI16 planet_cloak(const StarsSetVisPFPlanetsOps *ops,
    void *context, const StarsVisFleetScanFleet *fleet)
{
    if (ops && ops->cloak_percent)
        return ops->cloak_percent(context, fleet);
    return fleet->cloak_percent;
}

static void planet_progress(const StarsSetVisPFPlanetsOps *ops,
    void *context)
{
    if (ops && ops->progress)
        ops->progress(context, (StarsI16)-0x39f);
}

static void mark_planet_by_starbase(StarsSetVisPFPlanetsInput *input,
    const StarsSetVisPFPlanetsOps *ops, void *context,
    const StarsVisPlanetScanPlanet *planet, StarsU32 radius_squared,
    StarsU32 distance)
{
    StarsU32 threshold;

    if ((planet->flags & 0x0200U) != 0U && planet->owner != -1 &&
            planet->starbase_delta < 1 &&
            (planet->starbase_delta < 0 ||
             planet->starbase_count < 10000UL)) {
        threshold = (radius_squared * planet->starbase_count) / 10000UL;
        if (threshold < distance) {
            vis_planet_mark_planet(ops, context, planet->id,
                input->viewer, 2);
            return;
        }
    }
    vis_planet_mark_planet(ops, context, planet->id,
        input->viewer, 3);
}

static void scan_planets_from_planet(StarsSetVisPFPlanetsInput *input,
    const StarsSetVisPFPlanetsOps *ops, void *context,
    const StarsVisPlanetScanPlanet *source, StarsI16 deep_range)
{
    StarsU16 i;
    StarsU32 radius_squared;
    StarsU32 distance;
    StarsVisPlanetScanPlanet *target;

    if (deep_range <= 0) return;
    radius_squared = planet_range_squared(deep_range);
    for (i = 0; i < input->planet_count; ++i) {
        target = &input->planets[i];
        if (!planet_is_visible_enough(target)) continue;
        distance = planet_visibility_distance(source->x, source->y,
            target->x, target->y);
        if (distance > radius_squared) continue;
        mark_planet_by_starbase(input, ops, context, target,
            radius_squared, distance);
    }
}

static void scan_stargate_planets(StarsSetVisPFPlanetsInput *input,
    const StarsSetVisPFPlanetsOps *ops, void *context,
    const StarsVisPlanetScanPlanet *source, StarsI16 gate_range)
{
    StarsU16 i;
    StarsU32 radius_squared;
    StarsU32 distance;
    StarsI16 target_gate;
    StarsVisPlanetScanPlanet *target;

    if (gate_range <= 0) return;
    radius_squared = planet_range_squared(gate_range);
    for (i = 0; i < input->planet_count; ++i) {
        target = &input->planets[i];
        if (!planet_is_visible_enough(target) ||
                (target->flags & 0x0200U) == 0U)
            continue;
        target_gate = planet_stargate(ops, context, target, 0, 0);
        if (target_gate == 0) continue;
        if (gate_range <= 9999) {
            distance = planet_visibility_distance(source->x, source->y,
                target->x, target->y);
            if (distance > radius_squared) continue;
        } else {
            distance = 0UL;
        }
        if (distance != 0UL) {
            mark_planet_by_starbase(input, ops, context, target,
                radius_squared, distance);
        } else {
            vis_planet_mark_planet(ops, context, target->id,
                input->viewer, 3);
        }
    }
}

static void reveal_planet_things(StarsSetVisPFPlanetsInput *input,
    const StarsSetVisPFPlanetsOps *ops, void *context,
    const StarsVisPlanetScanPlanet *source, StarsI16 scanner_range,
    StarsI16 deep_range, StarsU32 viewer_mask)
{
    StarsU16 i;
    StarsU16 kind;
    StarsU32 scanner_squared;
    StarsU32 deep_squared;
    StarsU32 distance;
    StarsU32 partial_radius;
    StarsVisThingTarget *thing;

    if (input->viewer == (StarsI16)-1) return;
    (void)ops; (void)context;
    scanner_squared = planet_range_squared(scanner_range);
    deep_squared = planet_range_squared(deep_range);
    for (i = 0; i < input->thing_count; ++i) {
        thing = &input->things[i];
        kind = thing->kind;
        if (kind > 3U) continue;
        if (kind == 0U && (thing->bits_8 & viewer_mask) != 0UL)
            continue;
        if (kind == 3U && (thing->bits_4 & 0x0010UL) != 0UL)
            continue;
        if (kind == 1U && (thing->dest_x & 0x8000U) != 0U)
            continue;
        if (kind == 2U && (thing->dest_x & 0x2000U) != 0U)
            continue;
        distance = planet_visibility_distance(source->x, source->y,
            thing->x, thing->y);
        if (distance > scanner_squared) continue;
        if (kind == 1U) {
            thing->dest_x = (StarsU16)((thing->dest_x & 0x7fffU) |
                0x8000U);
            planet_detail(input, thing->owner);
            continue;
        }
        if (kind == 3U) {
            thing->bits_4 |= 0x0010UL;
            continue;
        }
        if (kind == 2U) {
            if ((thing->bits_2 & viewer_mask) == 0UL) {
                partial_radius = scanner_squared / 16UL;
                if (partial_radius < distance &&
                        deep_squared < distance)
                    continue;
            }
            thing->bits_2 |= viewer_mask;
            thing->dest_x = (StarsU16)((thing->dest_x & 0xdfffU) |
                0x2000U);
            continue;
        }
        if ((thing->bits_4 & viewer_mask) == 0UL &&
                deep_squared < distance) {
            partial_radius = scanner_squared / 16UL;
            if (partial_radius < distance) continue;
        }
        thing->bits_4 |= viewer_mask;
        thing->bits_8 |= viewer_mask;
        planet_detail(input, thing->owner);
    }
}

static StarsU32 attenuated_planet_radius(StarsU32 radius_squared,
    StarsI16 cloak_percent)
{
    StarsI32 visible_percent = 100 - (StarsI32)cloak_percent;
    StarsU32 result;

    if (visible_percent <= 0) return 0UL;
    result = (radius_squared * (StarsU32)visible_percent) / 100UL;
    result = (result * (StarsU32)visible_percent) / 100UL;
    return result;
}

static void reveal_planet_fleets(StarsSetVisPFPlanetsInput *input,
    const StarsSetVisPFPlanetsOps *ops, void *context,
    const StarsVisPlanetScanPlanet *source, StarsI16 scanner_range,
    StarsI16 deep_range)
{
    StarsU16 i;
    StarsU32 scanner_squared;
    StarsU32 deep_squared;
    StarsU32 distance;
    StarsU32 cloak_radius;
    StarsI16 cloak;
    StarsVisFleetScanFleet *fleet;

    scanner_squared = planet_range_squared(scanner_range);
    deep_squared = planet_range_squared(deep_range);
    for (i = 0; i < input->fleet_count; ++i) {
        fleet = &input->fleets[i];
        if ((fleet->flags & 0x0100U) != 0U ||
                (fleet->flags & 0x0400U) != 0U)
            continue;
        distance = planet_visibility_distance(source->x, source->y,
            fleet->x, fleet->y);
        if (distance > scanner_squared) continue;
        if (fleet->planet_id != -1 && distance > deep_squared)
            continue;
        cloak = planet_cloak(ops, context, fleet);
        if (cloak == 0) {
            vis_planet_mark_fleet(ops, context, fleet->id, 3);
            continue;
        }
        cloak_radius = attenuated_planet_radius(scanner_squared, cloak);
        if (distance > cloak_radius) continue;
        if (fleet->planet_id != -1) {
            cloak_radius = attenuated_planet_radius(deep_squared, cloak);
            if (cloak_radius < distance) continue;
        }
        vis_planet_mark_fleet(ops, context, fleet->id, 3);
    }
}

/* NE 15:abde.  SetVisPFPlanets performs the four planet-centered visibility
 * passes: ordinary planet scanning, optional stargate scanning, special
 * thing visibility, and fleet visibility. */
void stars_set_vis_pf_planets(StarsSetVisPFPlanetsInput *input,
    const StarsSetVisPFPlanetsOps *ops, void *context)
{
    StarsU16 i;
    StarsU16 design;
    StarsU16 has_stargate;
    StarsU16 *design_flags;
    StarsI16 gate_ranges[10];
    StarsU32 viewer_mask;
    StarsI16 scanner_range;
    StarsI16 deep_range;
    StarsI16 gate_range;
    StarsI16 race_stat;
    StarsVisPlanetScanPlanet *source;

    if (!input || !input->planets) return;
    viewer_mask = input->viewer == (StarsI16)-1 ? 0UL :
        (StarsU32)1UL << ((StarsU16)input->viewer & 31U);
    race_stat = input->race_stat;
    if (ops && ops->race_stat)
        race_stat = ops->race_stat(context, input->viewer, 0x000e);
    has_stargate = 0U;
    design_flags = input->starbase_design_flags;
    for (design = 0; design < 10U; ++design) {
        gate_ranges[design] = 0;
        if (race_stat == 7 && (!design_flags ||
                (design_flags[design] & 0x0200U) == 0U)) {
            if (input->stargate_ranges)
                gate_ranges[design] = input->stargate_ranges[design];
            if (ops && ops->stargate_range)
                gate_ranges[design] = planet_stargate(ops, context,
                    (const StarsVisPlanetScanPlanet *)0,
                    input->viewer, (StarsI16)design);
            if (gate_ranges[design] > 0) has_stargate = 1U;
        }
    }

    if (input->viewer != (StarsI16)-1) planet_progress(ops, context);
    /* Pass 1: normal deep planet scanning. */
    for (i = 0; i < input->planet_count; ++i) {
        source = &input->planets[i];
        if (source->owner != input->viewer) continue;
        scanner_range = planet_scanner(ops, context, source, &deep_range);
        (void)scanner_range;
        scan_planets_from_planet(input, ops, context, source, deep_range);
    }

    if (input->viewer != (StarsI16)-1) planet_progress(ops, context);
    /* Pass 2: stargate-connected planets use the design's gate radius. */
    if (has_stargate) {
        for (i = 0; i < input->planet_count; ++i) {
            StarsU16 slot;
            source = &input->planets[i];
            if (source->owner != input->viewer ||
                    (source->flags & 0x0200U) == 0U)
                continue;
            slot = source->starbase_slot;
            if (slot >= 10U) continue;
            gate_range = gate_ranges[slot];
            scan_stargate_planets(input, ops, context, source, gate_range);
        }
    }

    if (input->viewer != (StarsI16)-1) planet_progress(ops, context);
    /* Pass 3: things use the ordinary scanner radius, with the deep radius
     * providing the same close-in exception used by the original. */
    for (i = 0; i < input->planet_count; ++i) {
        source = &input->planets[i];
        if (source->owner != input->viewer) continue;
        scanner_range = planet_scanner(ops, context, source, &deep_range);
        reveal_planet_things(input, ops, context, source, scanner_range,
            deep_range, viewer_mask);
    }

    if (input->viewer != (StarsI16)-1) planet_progress(ops, context);
    /* Pass 4: fleets require scanner range, and stationed fleets also obey
     * the deep planet range before cloak attenuation is applied. */
    for (i = 0; i < input->planet_count; ++i) {
        source = &input->planets[i];
        if (source->owner != input->viewer) continue;
        scanner_range = planet_scanner(ops, context, source, &deep_range);
        reveal_planet_fleets(input, ops, context, source, scanner_range,
            deep_range);
    }
}
