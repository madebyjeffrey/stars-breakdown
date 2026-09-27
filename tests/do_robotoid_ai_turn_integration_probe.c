#include "core.h"
#include <stdio.h>
#include <string.h>

/* Bind only fields recovered from the original CodeView records.  Production
 * resources/costs and design selections are service results in the original
 * procedure, so they enter this adapter explicitly rather than being assigned
 * invented offsets in _planet. */
#define RAW_PLANET_BYTES 56
#define RAW_FLEET_BYTES 124

static StarsU16 raw_u16(const StarsU8 *raw, StarsI16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1] << 8));
}

static StarsI16 raw_i16(const StarsU8 *raw, StarsI16 offset)
{
    return (StarsI16)raw_u16(raw, offset);
}

static void put_u16(StarsU8 *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static void put_i16(StarsU8 *raw, StarsI16 offset, StarsI16 value)
{
    put_u16(raw, offset, (StarsU16)value);
}

static void put_planet(StarsU8 *raw, StarsI16 id, StarsI16 owner,
    StarsU16 flags, StarsU16 packet_strength)
{
    memset(raw, 0, RAW_PLANET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    put_u16(raw, 4, flags);
    /* The hostile-threat branch consumes the low twelve bits of uPopGuess. */
    put_u16(raw, 18, (StarsU16)(packet_strength & 0x0fffU));
}

static StarsI16 raw_planet_view(const StarsU8 *raw,
    StarsI32 mineral_rate, StarsI32 mineral_concentration,
    StarsI16 production_slot_open, const StarsI32 resources[4],
    const StarsI32 queue_cost[4], const StarsI32 hull_cost[4],
    StarsRobotoidTurnPlanet *out)
{
    StarsI16 i;
    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->starbase_flags = raw_u16(raw, 4);
    out->visibility_flags = raw_u16(raw, 4);
    out->packet_strength = (StarsU16)(raw_u16(raw, 18) & 0x0fffU);
    out->mineral_rate = mineral_rate;
    out->mineral_concentration = mineral_concentration;
    out->production_slot_open = production_slot_open;
    for (i = 0; i < 4; ++i) {
        out->resources_available[i] = resources[i];
        out->production_queue_cost[i] = queue_cost[i];
        out->hull_cost[i] = hull_cost[i];
    }
    return out->id >= 0;
}

static void put_fleet(StarsU8 *raw, StarsI16 id, StarsI16 owner,
    StarsI16 planet_id, StarsI16 cord, const StarsI16 counts[16])
{
    StarsI16 i;
    memset(raw, 0, RAW_FLEET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    put_i16(raw, 6, planet_id);
    put_i16(raw, 98, cord);
    for (i = 0; i < 16; ++i)
        put_i16(raw, (StarsI16)(12 + i * 2), counts[i]);
}

static StarsI16 raw_fleet_view(const StarsU8 *raw,
    StarsRobotoidTurnFleet *out)
{
    StarsI16 i;
    StarsI16 count;
    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->planet_id = raw_i16(raw, 6);
    out->cord = raw_i16(raw, 98);
    for (i = 0; i < 16; ++i) {
        count = raw_i16(raw, (StarsI16)(12 + i * 2));
        if (count < 0 || count > 255)
            return 0;
        out->ship_counts[i] = (StarsU8)count;
    }
    return out->id >= 0;
}

typedef struct Trace {
    StarsI16 merge_count;
    StarsI16 merge_masks[3];
    StarsI16 status_count;
    StarsI16 split_count;
    StarsI16 ensure_count;
    StarsI16 war_count;
    StarsI16 mark_count;
    StarsI16 production_count;
    StarsI16 queue_count;
    StarsI16 fleet_count;
    StarsI16 run_fleets_count;
    StarsI16 basic_count;
    StarsI16 fill_count;
    StarsI16 valid_threat;
    StarsI16 valid_queue;
    StarsI16 valid_policy;
} Trace;

static StarsI16 merge(void *context, StarsI16 mask)
{
    Trace *trace;
    trace = (Trace *)context;
    if (trace->merge_count >= 3)
        return 0;
    trace->merge_masks[trace->merge_count] = mask;
    ++trace->merge_count;
    return 1;
}

static StarsI16 ensure_ai(void *context, StarsI16 research_percent,
    StarsI16 *latest_starbase_design)
{
    (void)context;
    if (research_percent != 15)
        return -1;
    *latest_starbase_design = 23;
    return 41;
}

static StarsI16 status(void *context, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace;
    (void)budget;
    trace = (Trace *)context;
    *selected = 9;
    if (first == 14 && last == 15)
        mask[14] = 2;
    ++trace->status_count;
    return 1;
}

static StarsI16 split(void *context, const StarsU8 mask[16])
{
    Trace *trace;
    trace = (Trace *)context;
    if ((trace->split_count == 0 && mask[14] != 2) ||
            (trace->split_count == 1 && mask[0] != 2) ||
            (trace->split_count == 2 && mask[1] != 2) ||
            (trace->split_count == 3 && (mask[11] != 2 ||
                mask[12] != 2 || mask[13] != 2)))
        return 0;
    ++trace->split_count;
    return 1;
}

static StarsI16 ensure_shdefs(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->ensure_count;
    return 1;
}

static StarsI16 count_war(void *context,
    const StarsRobotoidTurnFleet *fleet, StarsI16 potency)
{
    Trace *trace;
    trace = (Trace *)context;
    if (fleet->id != 19 || potency != 8)
        return 0;
    ++trace->war_count;
    return 1;
}

static StarsI16 mark_planet(void *context,
    const StarsRobotoidTurnPlanet *planet, StarsI16 player_id,
    StarsI16 threat_level)
{
    Trace *trace;
    trace = (Trace *)context;
    if (planet->id != 8 || player_id != 4 || threat_level != 4)
        return 0;
    ++trace->mark_count;
    ++trace->valid_threat;
    return 1;
}

static StarsI16 run_production(void *context,
    const StarsRobotoidTurnPlanet *planet, StarsI16 budget)
{
    Trace *trace;
    trace = (Trace *)context;
    if (planet->id != 3 || budget != 70)
        return 0;
    ++trace->production_count;
    return 1;
}

static StarsI16 queue_production(void *context,
    const StarsRobotoidTurnPlanet *planet, StarsI16 design,
    StarsI16 quantity)
{
    Trace *trace;
    trace = (Trace *)context;
    if (planet->id != 3 || design != 9 || quantity != 5)
        return 0;
    ++trace->queue_count;
    ++trace->valid_queue;
    return 1;
}

static StarsI16 visit_fleet(void *context,
    const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;
    trace = (Trace *)context;
    if (fleet->id != 19 || potency[0] != 5 || potency[2] != 8 ||
            policy->total_ships != 1 || policy->attack_design_ships != 1 ||
            policy->needs_order_repair)
        return 0;
    ++trace->fleet_count;
    ++trace->valid_policy;
    return 1;
}

static StarsI16 run_fleets(void *context, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)context;
    if (player_id != 4)
        return 0;
    ++trace->run_fleets_count;
    return 1;
}

static StarsI16 basic_with_research(void *context, StarsI16 order,
    StarsI16 latest_design, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)context;
    if (order != 41 || latest_design != 23 || player_id != 4)
        return 0;
    ++trace->basic_count;
    return 1;
}

static StarsI16 fill(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->fill_count;
    return 1;
}

static int run_case(void)
{
    StarsU8 planet_raw[3][RAW_PLANET_BYTES];
    StarsU8 fleet_raw[RAW_FLEET_BYTES];
    StarsI16 counts[16];
    StarsI32 resources[4];
    StarsI32 queue_cost[4];
    StarsI32 hull_cost[4];
    StarsRobotoidTurnPlanet planets[3];
    StarsRobotoidTurnFleet fleet;
    StarsRobotoidAiTurnOps ops;
    StarsRobotoidAiTurnInput input;
    Trace trace;
    StarsI16 i;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    for (i = 0; i < 4; ++i) {
        resources[i] = 100;
        queue_cost[i] = 10;
        hull_cost[i] = 10;
    }
    for (i = 0; i < 16; ++i)
        counts[i] = 0;
    counts[14] = 1;
    put_planet(planet_raw[0], 3, 4, 0x0200U, 0);
    put_planet(planet_raw[1], 8, 9, 0x0200U, 501);
    put_planet(planet_raw[2], 12, -1, 0, 0);
    if (!raw_planet_view(planet_raw[0], 1, 0, 1, resources, queue_cost,
            hull_cost, &planets[0]) ||
            !raw_planet_view(planet_raw[1], 0, 0, 0, resources, queue_cost,
            hull_cost, &planets[1]) ||
            !raw_planet_view(planet_raw[2], 0, 0, 0, resources, queue_cost,
            hull_cost, &planets[2]))
        return 1;
    put_fleet(fleet_raw, 19, 4, 3, 0, counts);
    if (!raw_fleet_view(fleet_raw, &fleet))
        return 2;
    if (fleet.id != 19 || fleet.owner != 4 || fleet.planet_id != 3 ||
            fleet.cord != 0 || fleet.ship_counts[14] != 1)
        return 3;
    ops.merge_shdefs = merge;
    ops.ensure_ai = ensure_ai;
    ops.check_design_status = status;
    ops.split_shdefs = split;
    ops.ensure_shdefs = ensure_shdefs;
    ops.count_war_fleet = count_war;
    ops.mark_planet_typed = mark_planet;
    ops.run_production = run_production;
    ops.queue_production = queue_production;
    ops.run_fleets = run_fleets;
    ops.visit_fleet_typed = visit_fleet;
    ops.run_basic_tasks_with_research = basic_with_research;
    ops.fill_production_queue = fill;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 3;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    if (!stars_do_robotoid_ai_turn(&input))
        return 4;
    if (trace.merge_count != 3 || trace.merge_masks[0] != 0x06fc ||
            trace.merge_masks[1] != 1 || trace.merge_masks[2] != -0x4000 ||
            trace.status_count != 5 || trace.split_count != 4 ||
            trace.ensure_count != 1 || trace.war_count != 1 ||
            trace.mark_count != 1 || trace.valid_threat != 1 ||
            trace.production_count != 1 || trace.queue_count != 1 ||
            trace.valid_queue != 1 || trace.fleet_count != 1 ||
            trace.valid_policy != 1 || trace.run_fleets_count != 1 ||
            trace.basic_count != 1 || trace.fill_count != 1 ||
            input.hostile_planet_count != 1 ||
            input.production_queued != 1 ||
            input.production_hulls_queued != 5 ||
            input.research_order != 41 ||
            input.latest_starbase_design != 23 ||
            input.armada_potency[0] != 5 || input.armada_potency[2] != 8)
        return 5;
    return 0;
}

int main(void)
{
    int failures;
    failures = run_case();
    if (failures == 0)
        puts("ROBOTOID_AI_TURN_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS");
    else
        puts("ROBOTOID_AI_TURN_INTEGRATION_CHECKS=18 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
