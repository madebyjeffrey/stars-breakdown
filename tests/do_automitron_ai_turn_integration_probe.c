#include "core.h"
#include <stdio.h>
#include <string.h>

/* Bind only fields recovered from the packed Win16 records.  Planet class,
 * mineral/service values, design selections, order decisions, and production
 * costs enter through the same typed service boundaries used by the original
 * procedure; they are not assigned invented raw offsets here. */
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

static void put_i16(StarsU8 *raw, StarsI16 offset, StarsI16 value)
{
    StarsU16 word;

    word = (StarsU16)value;
    raw[offset] = (StarsU8)(word & 0xffU);
    raw[offset + 1] = (StarsU8)(word >> 8);
}

static void put_planet(StarsU8 *raw, StarsI16 id, StarsI16 owner,
    StarsU16 flags)
{
    memset(raw, 0, RAW_PLANET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    put_i16(raw, 4, (StarsI16)flags);
}

static StarsI16 raw_planet_view(const StarsU8 *raw, StarsI16 planet_class,
    StarsI16 desirability, StarsI16 optimal_value, StarsI32 mineral_rate,
    StarsI32 mineral_concentration, StarsI16 production_slot_open,
    const StarsI32 resources[4], const StarsI32 queue_cost[4],
    const StarsI32 hull_cost[4], StarsAutomitronTurnPlanet *out)
{
    StarsI16 i;

    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->starbase_flags = raw_u16(raw, 4);
    out->visibility_flags = raw_u16(raw, 4);
    out->planet_class = planet_class;
    out->desirability = desirability;
    out->optimal_value = optimal_value;
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
    StarsI16 planet_id, const StarsI16 counts[16])
{
    StarsI16 i;

    memset(raw, 0, RAW_FLEET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    put_i16(raw, 6, planet_id);
    for (i = 0; i < 16; ++i)
        put_i16(raw, (StarsI16)(12 + i * 2), counts[i]);
}

static StarsI16 raw_fleet_view(const StarsU8 *raw,
    StarsAutomitronTurnFleet *out)
{
    StarsI16 i;
    StarsI16 count;

    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->planet_id = raw_i16(raw, 6);
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
    StarsI16 status_budget;
    StarsI16 split_count;
    StarsI16 ensure_count;
    StarsI16 ensure_ai_count;
    StarsI16 planet_count;
    StarsI16 queue_count;
    StarsI16 fleet_count;
    StarsI16 basic_count;
    StarsI16 fill_count;
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

static StarsI16 ensure_ai(void *context, StarsI16 percent,
    StarsI16 *latest_starbase_design)
{
    Trace *trace;

    trace = (Trace *)context;
    if (percent != 20)
        return -1;
    ++trace->ensure_ai_count;
    *latest_starbase_design = 7;
    return 19;
}

static StarsI16 status_ex(void *context, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace;

    trace = (Trace *)context;
    (void)last;
    if (budget != 70)
        return 0;
    if (first == 11)
        *selected = 4;
    else if (first == 4)
        *selected = 6;
    else if (first == 2)
        *selected = 7;
    else if (first == 9)
        *selected = 8;
    else
        return 0;
    mask[first] = (StarsU8)(first + 1);
    ++trace->status_count;
    trace->status_budget = budget;
    return 1;
}

static StarsI16 split(void *context, const StarsU8 mask[16])
{
    Trace *trace;

    trace = (Trace *)context;
    if (mask[11] != 12U || mask[4] != 5U || mask[2] != 3U ||
            mask[9] != 10U)
        return 0;
    ++trace->split_count;
    return 1;
}

static StarsI16 ensure(void *context, StarsI16 selected)
{
    Trace *trace;

    trace = (Trace *)context;
    if (selected != 19)
        return 0;
    ++trace->ensure_count;
    return 1;
}

static StarsI16 visit_planet(void *context,
    const StarsAutomitronTurnPlanet *planet, StarsI16 budget)
{
    Trace *trace;

    trace = (Trace *)context;
    if (budget != 70 || planet->id < 0)
        return 0;
    ++trace->planet_count;
    return 1;
}

static StarsI16 queue_production(void *context,
    const StarsAutomitronTurnPlanet *planet, StarsI16 design,
    StarsI16 quantity)
{
    Trace *trace;

    trace = (Trace *)context;
    if (planet->id != 1 || design != 4 || quantity != 5)
        return 0;
    ++trace->queue_count;
    ++trace->valid_queue;
    return 1;
}

static StarsI16 visit_fleet_typed(void *context,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPolicy *policy, StarsI16 potency)
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 19 || fleet->owner != 4 || fleet->planet_id != 1 ||
            potency != 4 || policy->total_ships != 20 ||
            policy->slot_1_ships != 2 || policy->slot_2_3_ships != 7 ||
            policy->slot_6_ships != 4 || policy->slot_9_10_ships != 7)
        return 0;
    ++trace->fleet_count;
    ++trace->valid_policy;
    return 1;
}

static StarsI16 basic(void *context, StarsI16 research_order,
    StarsI16 latest_starbase_design, StarsI16 player_id)
{
    Trace *trace;

    trace = (Trace *)context;
    if (research_order != 19 || latest_starbase_design != 7 ||
            player_id != 4)
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
    StarsAutomitronTurnPlanet planets[3];
    StarsAutomitronTurnFleet fleet;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronAiTurnInput input;
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
    counts[1] = 2;
    counts[2] = 3;
    counts[3] = 4;
    counts[6] = 4;
    counts[9] = 5;
    counts[10] = 2;
    put_planet(planet_raw[0], 1, 4, 0x0200U);
    put_planet(planet_raw[1], 2, -1, 0x0200U);
    put_planet(planet_raw[2], 3, 4, 0x0200U);
    if (!raw_planet_view(planet_raw[0], 0, 0, 0, 1, 0, 1, resources,
            queue_cost, hull_cost, &planets[0]) ||
            !raw_planet_view(planet_raw[1], 3, 0, 1, 0, 0, 0, resources,
            queue_cost, hull_cost, &planets[1]) ||
            !raw_planet_view(planet_raw[2], 0, -1, 0, 0, 0, 0, resources,
            queue_cost, hull_cost, &planets[2]))
        return 1;
    put_fleet(fleet_raw, 19, 4, 1, counts);
    if (!raw_fleet_view(fleet_raw, &fleet))
        return 2;
    if (fleet.id != 19 || fleet.owner != 4 || fleet.planet_id != 1 ||
            fleet.ship_counts[1] != 2 || fleet.ship_counts[3] != 4 ||
            fleet.ship_counts[6] != 4 || fleet.ship_counts[10] != 2)
        return 3;
    ops.merge_shdefs = merge;
    ops.ensure_ai = ensure_ai;
    ops.check_design_status_ex = status_ex;
    ops.split_shdefs = split;
    ops.ensure_shdefs = ensure;
    ops.visit_planet = visit_planet;
    ops.queue_production = queue_production;
    ops.visit_fleet_typed = visit_fleet_typed;
    ops.run_basic_tasks = basic;
    ops.fill_production_queue = fill;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 3;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    if (!stars_do_automitron_ai_turn(&input))
        return 4;
    if (trace.merge_count != 3 || trace.merge_masks[0] != 0x1e0c ||
            trace.merge_masks[1] != 0x0040 || trace.merge_masks[2] != 0x4000 ||
            trace.status_count != 4 || trace.status_budget != 70 ||
            trace.split_count != 1 || trace.ensure_count != 1 ||
            trace.ensure_ai_count != 1 || trace.planet_count != 3 ||
            trace.queue_count != 1 || trace.fleet_count != 1 ||
            trace.basic_count != 1 || trace.fill_count != 1 ||
            trace.valid_queue != 1 || trace.valid_policy != 1 ||
            input.unowned_viable_count != 1 || input.owned_bad_count != 1 ||
            input.production_queued != 1 || input.production_hulls_queued != 5 ||
            input.research_order != 19 || input.latest_starbase_design != 7 ||
            input.armada_potency[0] != 4 || input.armada_potency[1] != 2 ||
            input.armada_potency[2] != 8 || input.armada_potency[3] != 3)
        return 5;
    return 0;
}

int main(void)
{
    int failures;

    failures = run_case();
    if (failures == 0)
        puts("AUTOMITRON_AI_TURN_INTEGRATION_CHECKS=20 FAILURES=0 RESULT=PASS");
    else
        puts("AUTOMITRON_AI_TURN_INTEGRATION_CHECKS=20 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
