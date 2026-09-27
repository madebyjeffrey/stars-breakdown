#include "core.h"

static StarsU32 thing_distance_squared(StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    StarsI32 dx = (StarsI32)x1 - (StarsI32)x2;
    StarsI32 dy = (StarsI32)y1 - (StarsI32)y2;
    return (StarsU32)(dx * dx + dy * dy);
}

static void thing_detail(StarsU16 *details, StarsU16 count,
    StarsI16 owner)
{
    StarsU16 flags;
    if (!details || owner < 0 || owner >= (StarsI16)count) return;
    flags = details[(StarsU16)owner];
    if ((flags & 0x0100U) == 0U)
        details[(StarsU16)owner] = (StarsU16)((flags & 0xfff8U) |
            0x0100U | 3U);
}

static StarsI16 thing_cloak(const StarsSetVisPFThingsOps *ops,
    void *context, const StarsVisThingFleet *fleet)
{
    if (ops && ops->cloak_percent)
        return ops->cloak_percent(context, fleet);
    return fleet->cloak_percent;
}

static void vis_thing_mark_fleet(const StarsSetVisPFThingsOps *ops, void *context,
    StarsU16 id)
{
    if (ops && ops->mark_fleet) ops->mark_fleet(context, id, 3);
}

static void vis_thing_mark_planet(const StarsSetVisPFThingsOps *ops, void *context,
    StarsU16 id, StarsI16 viewer, StarsI16 detail)
{
    if (ops && ops->mark_planet)
        ops->mark_planet(context, id, viewer, detail);
}

static void mark_nearby_race6(StarsSetVisPFThingsInput *input,
    const StarsSetVisPFThingsOps *ops, void *context,
    const StarsVisThingTarget *source, StarsU32 radius_squared)
{
    StarsU16 i;
    StarsU32 distance;
    StarsU32 reduced;
    StarsI16 cloak;
    StarsVisThingFleet *fleet;
    StarsVisThingTarget *thing;
    StarsVisThingPlanet *planet;
    StarsU32 viewer_mask = (StarsU32)1UL <<
        ((StarsU16)input->viewer & 31U);

    for (i = 0; i < input->fleet_count; ++i) {
        fleet = &input->fleets[i];
        if ((fleet->flags & 0x0100U) != 0U ||
                (fleet->flags & 0x0400U) != 0U) continue;
        distance = thing_distance_squared(source->x, source->y,
            fleet->x, fleet->y);
        if (distance > radius_squared) continue;
        cloak = thing_cloak(ops, context, fleet);
        if (cloak == 0) {
            vis_thing_mark_fleet(ops, context, fleet->id);
        } else {
            reduced = (radius_squared * (StarsU32)(100 - cloak)) / 100U;
            reduced = (reduced * (StarsU32)(100 - cloak)) / 100U;
            if (distance <= reduced)
                vis_thing_mark_fleet(ops, context, fleet->id);
        }
    }
    for (i = 0; i < input->thing_count; ++i) {
        StarsU16 kind;
        thing = &input->things[i];
        kind = thing->kind;
        if (input->viewer == (StarsI16)-1 || kind > 3U) continue;
        if (kind == 0U && (thing->bits_8 & viewer_mask) != 0UL) continue;
        if (kind == 3U && (thing->bits_4 & 0x0010UL) != 0UL) continue;
        if (kind == 1U && (thing->dest_x & 0x8000U) != 0U) continue;
        if (kind == 2U && (thing->dest_x & 0x2000U) != 0U) continue;
        distance = thing_distance_squared(source->x, source->y,
            thing->x, thing->y);
        if (distance > radius_squared) continue;
        if (kind == 1U) {
            thing->dest_x = (StarsU16)(thing->dest_x | 0x8000U);
            thing_detail(input->player_detail_flags, input->player_count,
                thing->owner);
        } else if (kind == 3U) {
            thing->bits_4 |= 0x0010UL;
        } else if (kind == 2U) {
            if ((thing->bits_2 & viewer_mask) != 0UL ||
                    distance <= radius_squared) {
                thing->bits_2 |= viewer_mask;
                thing->dest_x = (StarsU16)(thing->dest_x | 0x2000U);
            }
        } else {
            thing->bits_4 |= viewer_mask;
            thing->bits_8 |= viewer_mask;
            thing_detail(input->player_detail_flags, input->player_count,
                thing->owner);
        }
    }
    for (i = 0; i < input->planet_count; ++i) {
        StarsU32 threshold;
        planet = &input->planets[i];
        if (((planet->flags & 0x0100U) != 0U &&
                (planet->flags & 0x00ffU) >= 3U)) continue;
        distance = thing_distance_squared(source->x, source->y,
            planet->x, planet->y);
        if (distance > radius_squared) continue;
        if ((planet->flags & 0x0200U) != 0U && planet->owner != -1 &&
                planet->starbase_delta < 1 &&
                (planet->starbase_delta < 0 ||
                 planet->starbase_count < 10000UL)) {
            threshold = (radius_squared *
                (StarsU32)planet->starbase_count) / 10000UL;
            if (threshold < distance) {
                vis_thing_mark_planet(ops, context, planet->id, input->viewer, 2);
                continue;
            }
        }
        vis_thing_mark_planet(ops, context, planet->id, input->viewer, 3);
    }
}

/* NE 15:b9ee-c41c. Race-stat 6 reveals objects around a source thing using
 * cloak-attenuated circular ranges; race-stat 5 instead reveals unowned
 * fleets around each owned object with the original random cloak test. */
void stars_set_vis_pf_things(StarsSetVisPFThingsInput *input,
    const StarsSetVisPFThingsOps *ops, void *context)
{
    StarsU16 i;
    StarsU32 distance;
    StarsU32 viewer_mask;
    StarsI16 cloak;
    StarsVisThingTarget *thing;
    StarsVisThingFleet *fleet;

    if (!input) return;
    viewer_mask = input->viewer == (StarsI16)-1 ? 0UL :
        (StarsU32)1UL << ((StarsU16)input->viewer & 31U);
    if (input->race_stat == 6) {
        for (i = 0; i < input->thing_count; ++i) {
            StarsU16 range_code;
            StarsU32 radius;
            StarsU32 radius_squared;
            thing = &input->things[i];
            range_code = (StarsU16)((thing->dest_x >> 10) & 0x000fU);
            if (thing->kind != 1U || thing->owner != input->viewer ||
                    range_code == 0U) continue;
            thing->dest_x = (StarsU16)(thing->dest_x | 0x8000U);
            radius = (StarsU32)(range_code + 4U);
            radius *= radius;
            radius_squared = radius * radius;
            mark_nearby_race6(input, ops, context, thing, radius_squared);
        }
    } else if (input->race_stat == 5 && input->viewer != (StarsI16)-1) {
        for (i = 0; i < input->thing_count; ++i) {
            StarsU16 j;
            thing = &input->things[i];
            if (thing->kind != 0U || thing->owner != input->viewer) continue;
            for (j = 0; j < input->fleet_count; ++j) {
                fleet = &input->fleets[j];
                if ((fleet->flags & 0x0100U) != 0U ||
                        (fleet->flags & 0x0400U) != 0U ||
                        fleet->planet_id != -1) continue;
                distance = thing_distance_squared(thing->x, thing->y,
                    fleet->x, fleet->y);
                if (distance > thing->range_squared) continue;
                cloak = thing_cloak(ops, context, fleet);
                if (cloak == 0 || !ops || !ops->random_value ||
                        cloak <= ops->random_value(context, 100))
                    vis_thing_mark_fleet(ops, context, fleet->id);
            }
        }
    }
    (void)viewer_mask;
}
