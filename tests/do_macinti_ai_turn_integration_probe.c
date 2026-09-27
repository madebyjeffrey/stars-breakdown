#include "core.h"
#include <stdio.h>
#include <string.h>

/* Bind only fields recovered from the packed Win16 records.  Mineral/service
 * values, production costs, design selections, order state, and point data
 * remain typed service results; no unverified offsets are assigned to them. */
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
    StarsU16 flags, StarsU16 packet_strength)
{
    memset(raw, 0, RAW_PLANET_BYTES);
    put_i16(raw, 0, id);
    put_i16(raw, 2, owner);
    put_i16(raw, 4, (StarsI16)flags);
    put_i16(raw, 18, (StarsI16)(packet_strength & 0x0fffU));
}

static StarsI16 raw_planet_view(const StarsU8 *raw,
    const StarsU8 mineral_concentration[3],
    const StarsI32 resources[4], const StarsI32 queue_cost[4],
    const StarsI32 hull_cost[4], StarsI16 production_design,
    StarsI16 production_batch, StarsI16 queue_count, StarsI16 queue_limit,
    StarsMacintiTurnPlanet *out)
{
    StarsI16 i;

    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->raw_record = raw;
    out->starbase_flags = raw_u16(raw, 4);
    out->packet_strength = (StarsU16)(raw_u16(raw, 18) & 0x0fffU);
    for (i = 0; i < 3; ++i)
        out->mineral_concentration[i] = mineral_concentration[i];
    for (i = 0; i < 4; ++i) {
        out->resources_available[i] = resources[i];
        out->production_queue_cost[i] = queue_cost[i];
        out->hull_cost[i] = hull_cost[i];
    }
    out->production_design = production_design;
    out->production_batch = production_batch;
    out->production_queue_count = queue_count;
    out->production_queue_limit = queue_limit;
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
    StarsMacintiTurnFleet *out)
{
    StarsI16 i;
    StarsI16 count;

    memset(out, 0, sizeof *out);
    out->id = raw_i16(raw, 0);
    out->owner = raw_i16(raw, 2);
    out->raw_record = raw;
    out->planet_id = raw_i16(raw, 6);
    out->cord = raw_i16(raw, 98);
    out->order_kind = order_kind;
    for (i = 0; i < 16; ++i) {
        count = raw_i16(raw, (StarsI16)(12 + i * 2));
        if (count < 0)
            return 0;
        out->ship_counts[i] = count;
    }
    return out->id >= 0;
}

typedef struct Trace {
    StarsI16 merge_count;
    StarsI16 merge_masks[4];
    StarsI16 status_count;
    StarsI16 status_budgets[6];
    StarsI16 status_info_count;
    StarsI16 split_count;
    StarsU8 split_masks[5][16];
    StarsI16 ensure_count;
    StarsI16 starbase_count;
    StarsI16 colonizer_count;
    StarsI16 planets_count;
    StarsI16 fleets_count;
    StarsI16 typed_planets;
    StarsI16 production_plans;
    StarsI16 queue_count;
    StarsI16 fleet_tasks;
    StarsI16 typed_fleet_count;
    StarsI16 fleet_plans;
    StarsI16 fleet_actions;
    StarsI16 transport_routes;
    StarsI16 finalizations;
    StarsI16 basic_tasks;
    StarsI16 filled;
    StarsI16 valid_planet;
    StarsI16 valid_queue;
    StarsI16 valid_fleet;
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

static StarsI16 status(void *context, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace;

    trace = (Trace *)context;
    (void)last;
    if (trace->status_count >= 6)
        return 0;
    if (first == 12) {
        mask[12] = 2;
        mask[13] = 2;
        *selected = 6;
    } else if (first == 14) {
        mask[14] = 2;
        mask[15] = 2;
        *selected = 7;
    } else if (first == 10) {
        mask[10] = 2;
        mask[11] = 2;
        *selected = 8;
    } else if (first == 8) {
        mask[8] = 2;
        mask[9] = 2;
        *selected = 9;
    } else if (first == 2) {
        mask[2] = 2;
        mask[3] = 2;
        mask[4] = 2;
        *selected = 4;
    } else if (first == 5) {
        mask[5] = 2;
        mask[6] = 2;
        mask[7] = 2;
        *selected = 5;
    } else
        return 0;
    trace->status_budgets[trace->status_count] = budget;
    ++trace->status_count;
    return 1;
}

static StarsI16 status_info(void *context, StarsI16 design,
    StarsI16 *active, StarsI16 *hull_kind)
{
    Trace *trace;

    trace = (Trace *)context;
    if (design != 12 && design != 13)
        return 0;
    *active = 1;
    *hull_kind = 0;
    ++trace->status_info_count;
    return 1;
}

static StarsI16 split(void *context, const StarsU8 mask[16])
{
    Trace *trace;

    trace = (Trace *)context;
    if (trace->split_count == 0) {
        if (mask[12] != 2 || mask[13] != 2 || mask[14] != 2 ||
                mask[15] != 2 || mask[10] != 2 || mask[11] != 2 ||
                mask[8] != 2 || mask[9] != 2 || mask[2] != 2 ||
                mask[3] != 2 || mask[4] != 2 || mask[5] != 2 ||
                mask[6] != 2 || mask[7] != 2)
            return 0;
    } else if (trace->split_count == 1) {
        if (mask[0] != 2)
            return 0;
    } else if (trace->split_count == 2) {
        if (mask[1] != 2)
            return 0;
    } else if (trace->split_count == 3) {
        if (mask[14] != 2 || mask[15] != 2)
            return 0;
    } else if (trace->split_count == 4) {
        if (mask[10] != 2 || mask[11] != 2)
            return 0;
    } else
        return 0;
    memcpy(trace->split_masks[trace->split_count], mask, 16);
    ++trace->split_count;
    return 1;
}

static StarsI16 ensure(void *context)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->ensure_count;
    return 1;
}

static StarsI16 ensure_starbase(void *context)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->starbase_count;
    return 1;
}

static StarsI16 colonizer(void *context, StarsI16 *count)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->colonizer_count;
    *count = 3;
    return 1;
}

static StarsI16 visit_planets(void *context)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->planets_count;
    return 1;
}

static StarsI16 visit_fleets(void *context)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->fleets_count;
    return 1;
}

static StarsI16 visit_planet_typed(void *context,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn)
{
    Trace *trace;

    trace = (Trace *)context;
    if (turn != 150 || planet->id < 0)
        return 0;
    ++trace->typed_planets;
    if (planet->id == 4) {
        if (!policy->owned || policy->mineral_score != 127 ||
                !policy->production_affordable ||
                !policy->production_batch_affordable)
            return 0;
        ++trace->valid_planet;
    }
    return 1;
}

static StarsI16 plan_production(void *context,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn,
    StarsMacintiProductionPlan *plan)
{
    Trace *trace;

    trace = (Trace *)context;
    if (planet->id != 4 || !policy->owned || turn != 150)
        return 0;
    ++trace->production_plans;
    plan->action = STARS_MACINTI_PRODUCTION_QUEUE;
    plan->design = 7;
    plan->quantity = 1;
    plan->object_group = 2;
    plan->queue_mode = 1;
    plan->finish_mode = 0;
    plan->reason = 4;
    return 1;
}

static StarsI16 queue_production(void *context,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiProductionPlan *plan)
{
    Trace *trace;

    trace = (Trace *)context;
    if (planet->id != 4 || plan->design != 7 || plan->quantity != 1)
        return 0;
    ++trace->queue_count;
    ++trace->valid_queue;
    return 1;
}

static StarsI16 choose_fleet_task(void *context,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, StarsI16 turn)
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 8 || turn != 150 || policy->total_ships != 45 ||
            policy->recommended_task != STARS_MACINTI_TASK_REPAIR_ORDER)
        return 0;
    ++trace->fleet_tasks;
    return 1;
}

static StarsI16 visit_fleet_typed(void *context,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 8 || policy->total_ships != 45 ||
            policy->colonizer_ships != 1 || policy->transport_ships != 2 ||
            policy->armed_ships != 3 || policy->mission_design_ships != 9 ||
            policy->support_design_ships != 13 ||
            policy->attack_design_ships != 17 || !policy->needs_order_repair ||
            potency[0] != 7 || potency[2] != 8)
        return 0;
    ++trace->typed_fleet_count;
    ++trace->valid_fleet;
    return 1;
}

static StarsI16 plan_fleet_action(void *context,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, StarsI16 turn,
    StarsMacintiFleetPlan *plan)
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 8 || !policy->needs_order_repair || turn != 150)
        return 0;
    ++trace->fleet_plans;
    plan->action = STARS_MACINTI_FLEET_ACTION_ATTACK;
    plan->target_planet = 12;
    plan->order_kind = 1;
    plan->append = 0;
    plan->reason = 7;
    return 1;
}

static StarsI16 attack(void *context,
    const StarsMacintiTurnFleet *fleet, const StarsMacintiFleetPlan *plan)
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 8 || plan->action != STARS_MACINTI_FLEET_ACTION_ATTACK ||
            plan->target_planet != 12)
        return 0;
    ++trace->fleet_actions;
    return 1;
}

static StarsI16 route_transport(void *context,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 8 || policy->transport_ships != 2 || potency[0] != 7)
        return 0;
    ++trace->transport_routes;
    return 1;
}

static StarsI16 finalize_fleet(void *context,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy, const StarsU8 potency[4])
{
    Trace *trace;

    trace = (Trace *)context;
    if (fleet->id != 8 || policy->total_ships != 45 || potency[2] != 8)
        return 0;
    ++trace->finalizations;
    return 1;
}

static StarsI16 basic(void *context)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->basic_tasks;
    return 1;
}

static StarsI16 fill(void *context)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->filled;
    return 1;
}

static int run_case(void)
{
    StarsU8 planet_raw[2][RAW_PLANET_BYTES];
    StarsU8 fleet_raw[RAW_FLEET_BYTES];
    StarsU8 minerals[3];
    StarsI16 counts[16];
    StarsI32 resources[4];
    StarsI32 queue_cost[4];
    StarsI32 hull_cost[4];
    StarsMacintiTurnPlanet planets[2];
    StarsMacintiTurnFleet fleet;
    StarsMacintiAiTurnOps ops;
    StarsMacintiAiTurnInput input;
    Trace trace;
    StarsI16 i;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    minerals[0] = 100;
    minerals[1] = 90;
    minerals[2] = 80;
    for (i = 0; i < 4; ++i) {
        resources[i] = 10;
        queue_cost[i] = 1;
        hull_cost[i] = 9;
    }
    for (i = 0; i < 16; ++i)
        counts[i] = 0;
    counts[0] = 1;
    counts[1] = 2;
    counts[2] = 3;
    counts[10] = 4;
    counts[11] = 5;
    counts[12] = 6;
    counts[13] = 7;
    counts[14] = 8;
    counts[15] = 9;
    put_planet(planet_raw[0], 4, 3, STARS_MACINTI_STARBASE_FLAG, 501);
    put_planet(planet_raw[1], 5, -1, 0x0200U, 0);
    if (!raw_planet_view(planet_raw[0], minerals, resources, queue_cost,
            hull_cost, 7, 1, 0, 0, &planets[0]) ||
            !raw_planet_view(planet_raw[1], minerals, resources, queue_cost,
            hull_cost, -1, 0, 0, 0, &planets[1]))
        return 1;
    put_fleet(fleet_raw, 8, 3, 4, 1, counts);
    if (!raw_fleet_view(fleet_raw, 0, &fleet))
        return 2;
    if (planets[0].id != 4 || planets[0].owner != 3 ||
            planets[0].starbase_flags != STARS_MACINTI_STARBASE_FLAG ||
            planets[0].packet_strength != 501 || planets[0].raw_record != planet_raw[0] ||
            fleet.id != 8 || fleet.owner != 3 || fleet.planet_id != 4 ||
            fleet.cord != 1 || fleet.ship_counts[0] != 1 ||
            fleet.ship_counts[1] != 2 || fleet.ship_counts[15] != 9 ||
            fleet.raw_record != fleet_raw)
        return 3;
    ops.merge_shdefs = merge;
    ops.check_design_status = status;
    ops.design_status_info = status_info;
    ops.split_shdefs = split;
    ops.ensure_shdefs = ensure;
    ops.ensure_starbase_designs = ensure_starbase;
    ops.colonizer_decision = colonizer;
    ops.visit_planets = visit_planets;
    ops.visit_fleets = visit_fleets;
    ops.visit_planet_typed = visit_planet_typed;
    ops.plan_production = plan_production;
    ops.queue_production = queue_production;
    ops.choose_fleet_task = choose_fleet_task;
    ops.plan_fleet_action = plan_fleet_action;
    ops.attack = attack;
    ops.route_transport_fleet = route_transport;
    ops.finalize_fleet = finalize_fleet;
    ops.handle_basic_tasks = basic;
    ops.fill_production_queue = fill;
    ops.visit_fleet_typed = visit_fleet_typed;
    input.turn = 150;
    input.advanced_player = 1;
    input.starter_missing = 0;
    input.player_id = 3;
    input.planet_count = 2;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    ops.context = &trace;
    if (!stars_do_macinti_ai_turn(&input))
        return 4;
    if (trace.merge_count != 4 || trace.merge_masks[0] != 0x03fc ||
            trace.merge_masks[1] != 1 || trace.merge_masks[2] != -0x4000 ||
            trace.merge_masks[3] != 0x3000 || trace.status_count != 6 ||
            trace.status_budgets[0] != 70 || trace.status_budgets[1] != 5000 ||
            trace.status_budgets[5] != 70 || trace.status_info_count != 2 ||
            trace.split_count != 5 || trace.ensure_count != 1 ||
            trace.starbase_count != 1 || trace.colonizer_count != 1 ||
            trace.planets_count != 1 || trace.fleets_count != 1 ||
            trace.typed_planets != 2 || trace.production_plans != 1 ||
            trace.queue_count != 1 || trace.fleet_tasks != 1 ||
            trace.typed_fleet_count != 1 || trace.fleet_plans != 1 ||
            trace.fleet_actions != 1 ||
            trace.transport_routes != 1 || trace.finalizations != 1 ||
            trace.basic_tasks != 1 || trace.filled != 1 ||
            trace.valid_planet != 1 || trace.valid_queue != 1 ||
            trace.valid_fleet != 1 || input.starter_order != 6 ||
            input.colonizer_count != 3 || input.armada_potency[0] != 7 ||
            input.armada_potency[1] != 3 || input.armada_potency[2] != 8 ||
            input.armada_potency[3] != 3 || input.typed_planets_visited != 2 ||
            input.typed_fleets_visited != 1 || input.typed_armed_fleets != 1 ||
            input.typed_colonizer_fleets != 1 || input.typed_mission_fleets != 1 ||
            input.typed_support_design_fleets != 1 ||
            input.typed_attack_design_fleets != 1 ||
            input.typed_production_candidates != 1 ||
            input.typed_production_queued != 1 ||
            input.typed_fleet_actions != 1 || input.typed_transport_routes != 1 ||
            input.typed_fleet_finalizations != 1 || input.basic_tasks_run != 1 ||
            input.production_queue_filled != 1)
        return 5;
    return 0;
}

int main(void)
{
    int failures;

    failures = run_case();
    if (failures == 0)
        puts("MACINTI_AI_TURN_INTEGRATION_CHECKS=31 FAILURES=0 RESULT=PASS");
    else
        puts("MACINTI_AI_TURN_INTEGRATION_CHECKS=31 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
