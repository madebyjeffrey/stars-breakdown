#include "core.h"
#include <stdio.h>
#include <string.h>

/* Bind only fields recovered from the original packed records.  Production
 * resources/costs and design selections are service results in the original
 * routine, so this adapter supplies them explicitly instead of inventing raw
 * _planet offsets.  The raw _fleet order pointer is likewise represented by
 * the typed order discriminator at the service boundary. */
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
    StarsU8 visibility, StarsU8 a, StarsU8 b, StarsU8 c)
{
    memset(raw, 0, RAW_PLANET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    raw[4] = visibility;
    raw[9] = a;
    raw[10] = b;
    raw[11] = c;
}

static StarsI16 raw_planet_view(const StarsU8 *raw, StarsI16 desirability,
    StarsI16 production_slot_open, const StarsI32 resources[4],
    const StarsI32 queue_cost[4], const StarsI32 hull_cost[4],
    StarsTurinTurnPlanet *out)
{
    StarsI16 i;

    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    /* DoTurinDroneAiTurn compares the recovered det byte with 2. */
    out->visibility_flags = (StarsU16)raw[4];
    for (i = 0; i < 3; ++i)
        out->mineral_concentration[i] = raw[9 + i];
    out->desirability = desirability;
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

static StarsI16 raw_fleet_view(const StarsU8 *raw, StarsI16 order_kind,
    StarsTurinTurnFleet *out)
{
    StarsI16 i;
    StarsI16 count;

    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->planet_id = raw_i16(raw, 6);
    out->cord = raw_i16(raw, 98);
    out->order_kind = order_kind;
    for (i = 0; i < 16; ++i) {
        count = raw_i16(raw, (StarsI16)(12 + i * 2));
        out->ship_counts[i] = count;
    }
    return out->id >= 0;
}

typedef struct Trace {
    StarsI16 merge_count;
    StarsI16 merge_masks[4];
    StarsI16 status_count;
    StarsI16 split_count;
    StarsI16 ensure_count;
    StarsI16 ensure_ai_count;
    StarsI16 planet_count;
    StarsI16 opt_count;
    StarsI16 desirability_count;
    StarsI16 prepare_count;
    StarsI16 queue_count;
    StarsI16 fleet_count;
    StarsI16 fleets_count;
    StarsI16 tasks_count;
    StarsI16 fill_count;
    StarsI16 valid_queue;
    StarsI16 valid_policy;
    StarsI16 budget;
} Trace;

static StarsI16 merge(void *context, StarsI16 mask)
{
    Trace *trace;

    trace = (Trace *)context;
    if (trace->merge_count >= 4)
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
    if (percent != 15)
        return -1;
    ++trace->ensure_ai_count;
    *latest_starbase_design = 6;
    return 21;
}

static StarsI16 status(void *context, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace;

    trace = (Trace *)context;
    if (budget != 70)
        return 0;
    trace->budget = budget;
    *selected = 9;
    if (first == 6 && last == 7)
        mask[4] = 2;
    ++trace->status_count;
    return 1;
}

static StarsI16 split(void *context, const StarsU8 mask[16])
{
    Trace *trace;

    trace = (Trace *)context;
    if (mask[4] != 2)
        return 0;
    ++trace->split_count;
    return 1;
}

static StarsI16 ensure(void *context, StarsI16 selected)
{
    Trace *trace;

    trace = (Trace *)context;
    if (selected != 21)
        return 0;
    ++trace->ensure_count;
    return 1;
}

static StarsI16 opt(void *context, const StarsTurinTurnPlanet *planet,
    StarsI16 player_id)
{
    Trace *trace;

    trace = (Trace *)context;
    if (player_id != 4)
        return -1;
    ++trace->opt_count;
    return planet->desirability;
}

static StarsI16 desirability(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 player_id)
{
    Trace *trace;

    trace = (Trace *)context;
    if (player_id != 4)
        return -1;
    ++trace->desirability_count;
    return planet->desirability;
}

static StarsI16 prepare(void *context, const StarsTurinTurnPlanet *planet,
    StarsI16 turn)
{
    Trace *trace;

    trace = (Trace *)context;
    if (planet->id != 2 || planet->owner != 4 ||
            !planet->production_slot_open || turn != 150)
        return 0;
    ++trace->prepare_count;
    return 1;
}

static StarsI16 visit_planet(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 value)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)value;
    if (planet->id < 0)
        return 0;
    ++trace->planet_count;
    return 1;
}

static StarsI16 queue_production(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 design, StarsI16 quantity)
{
    Trace *trace;

    trace = (Trace *)context;
    if (planet->id != 2 || design != 9 || quantity != 1)
        return 0;
    ++trace->queue_count;
    ++trace->valid_queue;
    return 1;
}

static StarsI16 visit_fleet(void *context, const StarsTurinTurnFleet *fleet,
    const StarsTurinFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 19 || fleet->owner != 4 || fleet->planet_id != 2 ||
            potency[0] != 4 || potency[1] != 2 || potency[2] != 8 ||
            potency[3] != 3 || policy->total_ships != 12 ||
            policy->colonizer_ships != 1 || policy->transport_ships != 2 ||
            policy->armed_ships != 3 || policy->support_design_ships != 3 ||
            policy->attack_design_ships != 3 || policy->needs_order_repair)
        return 0;
    ++trace->fleet_count;
    ++trace->valid_policy;
    return 1;
}

static StarsI16 visit_fleets(void *context, StarsI16 player_id)
{
    Trace *trace;

    trace = (Trace *)context;
    if (player_id != 4)
        return 0;
    ++trace->fleets_count;
    return 1;
}

static StarsI16 tasks(void *context, StarsI16 research_order,
    StarsI16 latest_starbase_design)
{
    Trace *trace;

    trace = (Trace *)context;
    if (research_order != 21 || latest_starbase_design != 6)
        return 0;
    ++trace->tasks_count;
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
    StarsU8 planet_raw[4][RAW_PLANET_BYTES];
    StarsU8 fleet_raw[RAW_FLEET_BYTES];
    StarsI16 counts[16];
    StarsI32 resources[4];
    StarsI32 queue_cost[4];
    StarsI32 hull_cost[4];
    StarsTurinTurnPlanet planets[4];
    StarsTurinTurnFleet fleet;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinDroneAiTurnInput input;
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
    counts[0] = 1;
    counts[1] = 2;
    counts[2] = 3;
    counts[0xc] = 2;
    counts[0xd] = 1;
    counts[0xe] = 2;
    counts[0xf] = 1;
    put_planet(planet_raw[0], 1, -1, 3, 80, 90, 100);
    put_planet(planet_raw[1], 2, 4, 3, 20, 20, 20);
    put_planet(planet_raw[2], 3, 4, 3, 20, 20, 20);
    put_planet(planet_raw[3], 4, -1, 3, 20, 20, 20);
    if (!raw_planet_view(planet_raw[0], 10, 0, resources, queue_cost,
            hull_cost, &planets[0]) ||
            !raw_planet_view(planet_raw[1], -1, 1, resources, queue_cost,
            hull_cost, &planets[1]) ||
            !raw_planet_view(planet_raw[2], 2, 0, resources, queue_cost,
            hull_cost, &planets[2]) ||
            !raw_planet_view(planet_raw[3], 0, 0, resources, queue_cost,
            hull_cost, &planets[3]))
        return 1;
    put_fleet(fleet_raw, 19, 4, 2, 0, counts);
    if (!raw_fleet_view(fleet_raw, 0, &fleet))
        return 2;
    if (fleet.id != 19 || fleet.owner != 4 || fleet.planet_id != 2 ||
            fleet.cord != 0 || fleet.ship_counts[0] != 1 ||
            fleet.ship_counts[1] != 2 || fleet.ship_counts[2] != 3 ||
            fleet.ship_counts[0xc] != 2 || fleet.ship_counts[0xf] != 1)
        return 3;
    ops.merge_shdefs = merge;
    ops.ensure_ai = ensure_ai;
    ops.check_design_status = status;
    ops.split_shdefs = split;
    ops.ensure_shdefs = ensure;
    ops.planet_opt = opt;
    ops.planet_desirability = desirability;
    ops.prepare_production = prepare;
    ops.visit_planet = visit_planet;
    ops.visit_fleets = visit_fleets;
    ops.queue_production = queue_production;
    ops.handle_tasks = tasks;
    ops.fill_queue = fill;
    ops.visit_fleet_typed = visit_fleet;
    ops.context = &trace;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 4;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    if (!stars_do_turin_drone_ai_turn(&input))
        return 4;
    if (trace.merge_count != 4 || trace.merge_masks[0] != -0x1f10 ||
            trace.merge_masks[1] != 0x1000 || trace.merge_masks[2] != 0x0c00 ||
            trace.merge_masks[3] != 0x000c || trace.status_count != 8 ||
            trace.budget != 70 || trace.split_count != 1 ||
            trace.ensure_count != 1 || trace.ensure_ai_count != 1 ||
            trace.planet_count != 4 || trace.opt_count != 2 ||
            trace.desirability_count != 2 || trace.prepare_count != 1 ||
            trace.queue_count != 5 || trace.fleet_count != 1 ||
            trace.fleets_count != 1 || trace.tasks_count != 1 ||
            trace.fill_count != 1 || trace.valid_queue != 5 ||
            trace.valid_policy != 1 || input.unowned_good_count != 2 ||
            input.unowned_viable_count != 1 ||
            input.unowned_opt_marked_count != 2 || input.owned_bad_count != 1 ||
            input.production_queued != 1 || input.production_hulls_queued != 5 ||
            input.design_selection[0] != 9 || input.research_order != 21 ||
            input.latest_starbase_design != 6)
        return 5;
    return 0;
}

int main(void)
{
    int failures;

    failures = run_case();
    if (failures == 0)
        puts("TURIN_DRONE_AI_TURN_INTEGRATION_CHECKS=20 FAILURES=0 RESULT=PASS");
    else
        puts("TURIN_DRONE_AI_TURN_INTEGRATION_CHECKS=20 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
