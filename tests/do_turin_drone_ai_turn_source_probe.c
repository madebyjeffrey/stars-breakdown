#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 merges;
    StarsI16 masks[4];
    StarsI16 status;
    StarsI16 budget;
    StarsI16 splits;
    StarsI16 ensured;
    StarsI16 ensure_ai_calls;
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
    StarsI16 tasks;
    StarsI16 filled;
    StarsI16 planets;
    StarsI16 fleets;
    StarsI16 typed_fleets;
    StarsI16 typed_fleet_valid;
    StarsI16 production_prepared;
    StarsI16 opt_calls;
} Trace;

static StarsI16 merge(void *context, StarsI16 mask)
{
    Trace *trace = (Trace *)context;
    trace->masks[trace->merges++] = mask;
    return 1;
}

static StarsI16 merge_needed(void *context, StarsI16 mask)
{
    (void)context;
    return mask != 0x1000;
}

static StarsI16 status(void *context, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace = (Trace *)context;
    (void)first;
    (void)last;
    trace->budget = budget;
    *selected = 8;
    if (first == 6 && last == 7)
        mask[4] = 2;
    ++trace->status;
    return 1;
}

static StarsI16 split(void *context, const StarsU8 mask[16])
{
    Trace *trace = (Trace *)context;
    if (mask[4] != 2)
        return 0;
    ++trace->splits;
    return 1;
}

static StarsI16 ensure(void *context, StarsI16 selected)
{
    Trace *trace = (Trace *)context;
    if (selected != 21)
        return 0;
    ++trace->ensured;
    return 1;
}

static StarsI16 ensure_ai(void *context, StarsI16 percent,
    StarsI16 *latest_starbase_design)
{
    Trace *trace = (Trace *)context;
    if (percent != 15)
        return -1;
    ++trace->ensure_ai_calls;
    trace->research_order = 21;
    trace->latest_starbase_design = 6;
    *latest_starbase_design = trace->latest_starbase_design;
    return trace->research_order;
}

static StarsI16 handle_tasks(void *context, StarsI16 research_order,
    StarsI16 latest_starbase_design)
{
    Trace *trace = (Trace *)context;
    if (research_order != 21 || latest_starbase_design != 6)
        return 0;
    ++trace->tasks;
    return 1;
}

static StarsI16 fill_queue(void *context)
{
    Trace *trace = (Trace *)context;
    ++trace->filled;
    return 1;
}

static StarsI16 opt(void *context, const StarsTurinTurnPlanet *planet,
    StarsI16 player_id)
{
    Trace *trace = (Trace *)context;
    (void)player_id;
    ++trace->opt_calls;
    return planet->desirability;
}

static StarsI16 desirability(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 player_id)
{
    (void)context;
    (void)player_id;
    return planet->desirability;
}

static StarsI16 prepare_production(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 turn)
{
    Trace *trace = (Trace *)context;
    if (planet->owner != 4 || !planet->production_slot_open || turn != 150)
        return 0;
    ++trace->production_prepared;
    return 1;
}

static StarsI16 visit_planet(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 value)
{
    Trace *trace = (Trace *)context;
    (void)planet;
    (void)value;
    ++trace->planets;
    return 1;
}

static StarsI16 visit_fleets(void *context, StarsI16 player_id)
{
    Trace *trace = (Trace *)context;
    (void)player_id;
    ++trace->fleets;
    return 1;
}

static StarsI16 visit_fleet_typed(void *context,
    const StarsTurinTurnFleet *fleet, const StarsTurinFleetPolicy *policy,
    const StarsU8 potency[4])
{
    Trace *trace = (Trace *)context;
    ++trace->typed_fleets;
    if (fleet->owner == 4 && potency[0] == 4 && potency[2] == 8 &&
            policy->total_ships == 12 && policy->colonizer_ships == 1 &&
            policy->transport_ships == 2 && policy->armed_ships == 3 &&
            policy->support_design_ships == 3 &&
            policy->attack_design_ships == 3 &&
            !policy->needs_order_repair)
        ++trace->typed_fleet_valid;
    return 1;
}

int main(void)
{
    StarsTurinDroneAiTurnOps ops;
    StarsTurinDroneAiTurnInput input;
    StarsTurinTurnPlanet planets[4];
    StarsTurinTurnFleet fleet;
    Trace trace;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&planets, 0, sizeof planets);
    memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    ops.merge_shdefs = merge;
    ops.ensure_ai = ensure_ai;
    ops.check_design_status = status;
    ops.split_shdefs = split;
    ops.ensure_shdefs = ensure;
    ops.planet_opt = opt;
    ops.planet_desirability = desirability;
    ops.prepare_production = prepare_production;
    ops.visit_planet = visit_planet;
    ops.visit_fleets = visit_fleets;
    ops.handle_tasks = handle_tasks;
    ops.fill_queue = fill_queue;
    ops.visit_fleet_typed = visit_fleet_typed;
    ops.context = &trace;
    planets[0].id = 1;
    planets[0].owner = -1;
    planets[0].visibility_flags = 3;
    planets[0].mineral_concentration[0] = 80;
    planets[0].mineral_concentration[1] = 90;
    planets[0].mineral_concentration[2] = 100;
    planets[0].desirability = 10;
    planets[1].id = 2;
    planets[1].owner = 4;
    planets[1].production_slot_open = 1;
    planets[1].desirability = -1;
    planets[2].id = 3;
    planets[2].owner = 4;
    planets[2].desirability = 2;
    planets[3].id = 4;
    planets[3].owner = -1;
    planets[3].visibility_flags = 3;
    fleet.owner = 4;
    fleet.cord = 0;
    fleet.order_kind = 0;
    fleet.ship_counts[0] = 1;
    fleet.ship_counts[1] = 2;
    fleet.ship_counts[2] = 3;
    fleet.ship_counts[0xc] = 2;
    fleet.ship_counts[0xd] = 1;
    fleet.ship_counts[0xe] = 2;
    fleet.ship_counts[0xf] = 1;
    input.turn = 150;
    input.player_id = 4;
    input.planet_count = 4;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    if (!stars_do_turin_drone_ai_turn(&input) || trace.merges != 4 ||
            trace.masks[0] != -0x1f10 || trace.masks[1] != 0x1000 ||
            trace.masks[2] != 0x0c00 || trace.masks[3] != 0x000c ||
            trace.status != 8 || trace.budget != 70 || trace.splits != 1 ||
            trace.ensured != 1 || trace.ensure_ai_calls != 1 ||
            trace.tasks != 1 || trace.filled != 1 || trace.planets != 4 ||
            trace.typed_fleets != 1 || trace.typed_fleet_valid != 1 ||
            trace.production_prepared != 1 ||
            trace.status != 8 || input.armada_potency[0] != 4 ||
            input.armada_potency[1] != 2 || input.armada_potency[2] != 8 ||
            input.armada_potency[3] != 3 || input.unowned_good_count != 2 ||
            input.unowned_viable_count != 1 ||
            input.unowned_opt_marked_count != 2 || input.owned_bad_count != 1 ||
            trace.opt_calls != 2)
        return 1;
    fleet.owner = 7;
    memset(&trace, 0, sizeof trace);
    ops.merge_needed = merge_needed;
    if (!stars_do_turin_drone_ai_turn(&input) || trace.typed_fleets != 0)
        return 2;
    if (trace.merges != 3 || trace.production_prepared != 1)
        return 3;
    input.player_tech_level_valid = 1;
    input.player_tech_level = 6;
    memset(&trace, 0, sizeof trace);
    if (!stars_do_turin_drone_ai_turn(&input) ||
            input.design_selection[6] != -1 || trace.status != 7)
        return 4;
    puts("TURIN_DRONE_AI_TURN_CHECKS=35 FAILURES=0 RESULT=PASS");
    return 0;
}
