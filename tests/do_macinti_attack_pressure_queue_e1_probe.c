#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsMacintiAiTurnInput *input;
    StarsU8 queue[4];
    StarsI16 design_available;
    StarsI16 action;
    StarsI16 design;
    StarsI16 quantity;
    StarsI16 group;
    StarsI16 mode;
    StarsI16 increment;
    StarsI16 fail_queue;
    StarsI16 queue_calls;
    StarsI16 plan_calls;
    StarsI16 seen_pressure;
    StarsI16 finish_calls;
    StarsI16 finish_mode;
    StarsI16 builder_calls;
    StarsI16 random_calls;
    StarsI16 mine_calls;
    StarsI16 attack_random;
} Trace;

static void word(StarsU8 *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes) {
        ++*failures;
        fprintf(stderr, "failed check %d\n", *checks);
    }
}

static void setup(Trace *trace)
{
    memset(trace->queue, 0, sizeof trace->queue);
    trace->design_available = 1;
    trace->action = STARS_MACINTI_PRODUCTION_QUEUE;
    trace->design = 14;
    trace->quantity = 1;
    trace->group = 2;
    trace->mode = 1;
    trace->increment = 1;
    trace->fail_queue = 0;
    trace->queue_calls = 0;
    trace->plan_calls = 0;
    trace->seen_pressure = -1;
    trace->finish_calls = 0;
    trace->finish_mode = -1;
    trace->builder_calls = 0;
    trace->random_calls = 0;
    trace->mine_calls = 0;
    trace->attack_random = 0;
}

static StarsI16 design_status(void *opaque, StarsI16 first,
    StarsI16 last, StarsI16 budget, StarsI16 *selected,
    StarsU8 mask[16])
{
    Trace *trace;
    trace = (Trace *)opaque;
    (void)last;
    (void)budget;
    (void)mask;
    *selected = first == 14 && trace->design_available ? 14 : -1;
    return 1;
}

static StarsI16 select_planet(void *opaque,
    const StarsMacintiTurnPlanet *planet)
{
    (void)opaque;
    return planet->id == 0;
}

static StarsI16 init_production(void *opaque,
    const StarsMacintiTurnPlanet *planet)
{
    (void)opaque;
    return planet->id == 0;
}

static StarsI16 current_queue(void *opaque,
    const StarsMacintiTurnPlanet *planet, const StarsU8 **queue,
    StarsU16 *size)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (planet->id != 0)
        return 0;
    *queue = trace->queue;
    *size = 4;
    return 1;
}

static StarsI16 finish_production(void *opaque,
    const StarsMacintiTurnPlanet *planet, StarsI16 mode)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->finish_calls;
    trace->finish_mode = mode;
    return planet->id == 0;
}

static StarsI16 plan_production(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn,
    StarsMacintiProductionPlan *plan)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->plan_calls;
    if (planet->id != 0 || !policy->owned || turn != 50)
        return 0;
    plan->action = trace->action;
    plan->design = trace->design;
    plan->quantity = trace->quantity;
    plan->object_group = trace->group;
    plan->queue_mode = trace->mode;
    plan->attack_pressure_increment = trace->increment;
    return 1;
}

static StarsI16 queue_production(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiProductionPlan *plan)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->queue_calls;
    trace->seen_pressure = trace->input->first_sweep_attack_pressure;
    return planet->id == 0 && plan->design == 14 &&
        !trace->fail_queue;
}

static StarsI16 random_value(void *opaque, StarsI16 bound)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->random_calls;
    if (bound == 100)
        return 99;
    if (bound == 2)
        return trace->attack_random;
    return -1;
}

static StarsI16 should_planet_build_colonizer(void *opaque,
    const StarsMacintiTurnPlanet *planet)
{
    (void)opaque;
    (void)planet;
    return 0;
}

static StarsI16 mine_from_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet, StarsI32 *count)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->mine_calls;
    *count = 0;
    return fleet->id == 9 && fleet->planet_id == 0 &&
        fleet->ship_counts[14] == 1;
}

static StarsI16 build_attack_production(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn,
    StarsMacintiAttackProductionInput *slice)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->builder_calls;
    if (planet->id != 0 || !policy->owned || turn != 50)
        return 0;
    slice->outer.should_build_colonizers = 0;
    slice->outer.should_planet_build_colonizer =
        should_planet_build_colonizer;
    slice->attack.design_existing = 0;
    slice->attack.mine_from_fleet = mine_from_fleet;
    slice->colonizer_design = 1;
    slice->player_planet_count = 0;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnInput input;
    StarsMacintiAiTurnOps ops;
    StarsMacintiTurnFleet fleet;
    StarsMacintiTurnPlanet planet;
    StarsU8 fleet_raw[124];
    StarsU8 planet_raw[56];
    StarsU8 ai[2][16];
    StarsI16 selected_ids[1];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(&planet, 0, sizeof planet);
    memset(fleet_raw, 0, sizeof fleet_raw);
    memset(planet_raw, 0, sizeof planet_raw);
    memset(ai, 0, sizeof ai);
    memset(&trace, 0, sizeof trace);
    trace.input = &input;
    setup(&trace);
    word(fleet_raw, 0, 9);
    word(fleet_raw, 2, 2);
    word(fleet_raw, 40, 1);
    fleet.id = 9;
    fleet.raw_record = fleet_raw;
    fleet.raw_record_size = 124;
    word(planet_raw, 0, 0);
    word(planet_raw, 2, 2);
    word(planet_raw, 4, 0x0200U);
    word(planet_raw, 40, 200);
    word(planet_raw, 44, 2);
    planet.id = 0;
    planet.raw_record = planet_raw;
    planet.raw_record_size = 56;
    planet.resources_available[0] = 100;
    planet.resources_available[1] = 100;
    planet.resources_available[2] = 100;
    planet.resources_available[3] = 100;
    selected_ids[0] = 0;
    ops.check_design_status = design_status;
    ops.select_planet = select_planet;
    ops.init_production = init_production;
    ops.current_production_queue = current_queue;
    ops.finish_production = finish_production;
    ops.plan_production = plan_production;
    ops.queue_production = queue_production;
    ops.context = &trace;
    input.turn = 50;
    input.player_id = 2;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.planet_table_bound = 1;
    input.raw_ai_planet_table = &ai[0][0];
    input.raw_ai_planet_record_count = 2;
    input.game_planet_max = 2;
    input.selected_ai_planet_count = 1;
    input.selected_ai_planet_ids = selected_ids;
    input.derive_attack_pressure_from_plans = 1;
    input.ops = &ops;

    setup(&trace);
    check(stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_census == 1 &&
        input.first_sweep_attack_pressure == 2 &&
        input.first_sweep_attack_pressure_bound == 1 &&
        input.typed_production_queued == 1 &&
        trace.plan_calls == 1 && trace.queue_calls == 1 &&
        trace.seen_pressure == 2 && trace.finish_calls == 1 &&
        trace.finish_mode == 1,
        &checks, &failures);
    setup(&trace);
    trace.increment = 0;
    check(stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 1 &&
        input.typed_production_queued == 1 && trace.queue_calls == 1 &&
        trace.seen_pressure == 1,
        &checks, &failures);
    setup(&trace);
    trace.design = 15;
    check(!stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 1 &&
        trace.queue_calls == 0, &checks, &failures);
    setup(&trace);
    trace.quantity = 2;
    check(!stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 1 &&
        trace.queue_calls == 0, &checks, &failures);
    setup(&trace);
    trace.group = 1;
    check(!stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 1 &&
        trace.queue_calls == 0, &checks, &failures);
    setup(&trace);
    trace.mode = 0;
    check(!stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 1 &&
        trace.queue_calls == 0, &checks, &failures);
    setup(&trace);
    trace.action = STARS_MACINTI_PRODUCTION_FINISH;
    check(!stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 1 &&
        trace.queue_calls == 0, &checks, &failures);
    setup(&trace);
    trace.increment = 2;
    check(!stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 1 &&
        trace.queue_calls == 0, &checks, &failures);
    setup(&trace);
    trace.fail_queue = 1;
    check(!stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 2 &&
        trace.queue_calls == 1 && trace.seen_pressure == 2,
        &checks, &failures);
    setup(&trace);
    input.derive_attack_pressure_from_plans = 0;
    check(!stars_do_macinti_ai_turn(&input) &&
        trace.queue_calls == 0, &checks, &failures);
    input.derive_attack_pressure_from_plans = 1;
    setup(&trace);
    ops.check_design_status = 0;
    check(!stars_do_macinti_ai_turn(&input) &&
        trace.plan_calls == 0, &checks, &failures);
    ops.check_design_status = design_status;
    setup(&trace);
    trace.design_available = 0;
    check(!stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_census == 0 &&
        input.first_sweep_attack_pressure == 0 &&
        trace.queue_calls == 0, &checks, &failures);
    setup(&trace);
    input.selected_ai_planet_count = 0;
    check(stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 1 &&
        trace.plan_calls == 0 && trace.queue_calls == 0,
        &checks, &failures);
    input.selected_ai_planet_count = 1;
    planet.resources_available[0] = 0;
    planet.production_queue_cost[0] = 1;
    planet.hull_cost[0] = 1000;
    planet.production_queue_limit = 1;
    planet.production_queue_count = 1;
    setup(&trace);
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_production_candidates == 1 &&
        input.typed_production_queued == 1 &&
        input.first_sweep_attack_pressure == 2 &&
        trace.plan_calls == 1 && trace.queue_calls == 1 &&
        trace.finish_calls == 1 && trace.finish_mode == 1,
        &checks, &failures);
    setup(&trace);
    trace.action = STARS_MACINTI_PRODUCTION_NONE;
    trace.increment = 0;
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_production_queued == 0 &&
        input.first_sweep_attack_pressure == 1 &&
        trace.queue_calls == 0 && trace.finish_calls == 1 &&
        trace.finish_mode == 0, &checks, &failures);
    setup(&trace);
    trace.action = STARS_MACINTI_PRODUCTION_FINISH;
    trace.increment = 0;
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_production_queued == 0 &&
        trace.queue_calls == 0 && trace.finish_calls == 1 &&
        trace.finish_mode == 0, &checks, &failures);
    planet_raw[9] = 60;
    ops.plan_production = 0;
    ops.build_attack_production = build_attack_production;
    ops.random_value = random_value;
    input.bound_attack_production = 1;
    setup(&trace);
    check(stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_census == 1 &&
        input.first_sweep_attack_pressure == 2 &&
        input.first_sweep_mission_count == 0 &&
        input.typed_production_queued == 1 &&
        trace.builder_calls == 1 && trace.plan_calls == 0 &&
        trace.random_calls == 2 && trace.mine_calls == 1 &&
        trace.queue_calls == 1 && trace.seen_pressure == 2 &&
        trace.finish_calls == 1 && trace.finish_mode == 1,
        &checks, &failures);
    setup(&trace);
    trace.attack_random = 1;
    check(stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_pressure == 1 &&
        input.typed_production_queued == 0 &&
        trace.random_calls == 2 && trace.mine_calls == 0 &&
        trace.queue_calls == 0 && trace.finish_calls == 1 &&
        trace.finish_mode == 0, &checks, &failures);
    ops.build_attack_production = 0;
    setup(&trace);
    check(!stars_do_macinti_ai_turn(&input) &&
        trace.queue_calls == 0 && trace.random_calls == 0,
        &checks, &failures);

    printf("MACINTI_ATTACK_PRESSURE_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
