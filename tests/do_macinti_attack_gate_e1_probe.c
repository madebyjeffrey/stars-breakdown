#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 draws[3];
    StarsI16 draw_count;
    StarsI16 draw_index;
    StarsI16 bounds[3];
    StarsI16 mine_calls;
    StarsI32 mine_result;
    StarsI16 colonizer_result;
    StarsI16 colonizer_calls;
    StarsMacintiAttackProductionResult *production_result;
    StarsI16 queue_calls;
    StarsI16 queue_designs[5];
    StarsI16 queue_counts[5];
    StarsI16 queue_pressures[5];
    StarsI16 fail_queue;
} Trace;

static void word(StarsU8 *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static StarsI16 random_value(void *opaque, StarsI16 bound)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->draw_index >= trace->draw_count)
        return -1;
    trace->bounds[trace->draw_index] = bound;
    return trace->draws[trace->draw_index++];
}

static StarsI16 mine_from_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet, StarsI32 *count)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->mine_calls;
    if (fleet->id != 7 || fleet->planet_id != 3 ||
            fleet->owner != 2 || fleet->ship_counts[14] != 1)
        return 0;
    *count = trace->mine_result;
    return 1;
}

static StarsI16 should_planet_build_colonizer(void *opaque,
    const StarsMacintiTurnPlanet *planet)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->colonizer_calls;
    if (planet->id != 3)
        return -1;
    return trace->colonizer_result;
}

static StarsI16 queue_production(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiProductionPlan *plan)
{
    Trace *trace;
    StarsI16 index;
    trace = (Trace *)opaque;
    index = trace->queue_calls++;
    if (planet->id != 3 || index >= 5 || plan->quantity != 1 ||
            plan->object_group != 2 || plan->queue_mode != 1 ||
            plan->attack_pressure_increment != (plan->design == 14))
        return 0;
    trace->queue_designs[index] = plan->design;
    trace->queue_counts[index] =
        trace->production_result->colonizer_count;
    trace->queue_pressures[index] =
        trace->production_result->attack_pressure;
    return trace->fail_queue != trace->queue_calls;
}

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes) {
        ++*failures;
        fprintf(stderr, "failed check %d\n", *checks);
    }
}

static void reset(Trace *trace, StarsI16 first, StarsI16 second,
    StarsI32 mine_result)
{
    memset(trace, 0, sizeof *trace);
    trace->draws[0] = first;
    trace->draws[1] = second;
    trace->draw_count = 2;
    trace->mine_result = mine_result;
}

int main(void)
{
    StarsMacintiAttackQueueInput input;
    StarsMacintiAttackOuterInput outer;
    StarsMacintiAttackProductionInput production;
    StarsMacintiAttackProductionResult produced;
    StarsMacintiTurnPlanet planet;
    StarsMacintiTurnFleet fleets[2];
    StarsU8 planet_raw[56];
    StarsU8 fleet_raw[2][124];
    Trace trace;
    StarsI16 queue;
    StarsI16 route;
    StarsI16 checks;
    StarsI16 failures;
    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&outer, 0, sizeof outer);
    memset(&production, 0, sizeof production);
    memset(&produced, 0, sizeof produced);
    memset(&planet, 0, sizeof planet);
    memset(fleets, 0, sizeof fleets);
    memset(planet_raw, 0, sizeof planet_raw);
    memset(fleet_raw, 0, sizeof fleet_raw);
    word(planet_raw, 0, 3);
    planet_raw[9] = 20;
    planet_raw[10] = 20;
    planet_raw[11] = 20;
    planet.id = 3;
    planet.raw_record = planet_raw;
    planet.raw_record_size = 56;
    input.planet = &planet;
    input.player_id = 2;
    input.attack_design = 14;
    input.attack_pressure = 10;
    input.design_existing = 4999;
    input.outer_branch_enabled = 1;
    input.random_value = random_value;
    input.mine_from_fleet = mine_from_fleet;
    input.context = &trace;
    reset(&trace, 0, 0, 0);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 1 &&
        trace.draw_index == 1 && trace.bounds[0] == 2, &checks, &failures);
    reset(&trace, 1, 0, 0);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 0 &&
        trace.draw_index == 1, &checks, &failures);
    input.design_existing = 5000;
    reset(&trace, 0, 0, 0);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 0 &&
        trace.draw_index == 0, &checks, &failures);
    input.design_existing = 4999;
    input.attack_pressure = 60;
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 0 &&
        trace.draw_index == 0, &checks, &failures);
    input.attack_pressure = 10;
    input.outer_branch_enabled = 0;
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 0 &&
        trace.draw_index == 0, &checks, &failures);
    input.outer_branch_enabled = 1;
    word(fleet_raw[0], 0, 7);
    word(fleet_raw[0], 2, 2);
    word(fleet_raw[0], 6, 3);
    word(fleet_raw[0], 40, 1);
    fleets[0].id = 7;
    fleets[0].raw_record = fleet_raw[0];
    fleets[0].raw_record_size = 124;
    input.fleets = fleets;
    input.fleet_count = 1;
    reset(&trace, 0, 0, 100);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 1 &&
        trace.mine_calls == 1 && trace.draw_index == 1,
        &checks, &failures);
    reset(&trace, 0, 9, 1001);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 0 &&
        trace.mine_calls == 1 && trace.draw_index == 1,
        &checks, &failures);
    reset(&trace, 0, 9, 180);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 1 &&
        trace.bounds[1] == 10, &checks, &failures);
    reset(&trace, 0, 0, 180);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 0 &&
        trace.draw_index == 2, &checks, &failures);
    input.attack_pressure = 30;
    reset(&trace, 0, 9, 180);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 0 &&
        trace.draw_index == 1, &checks, &failures);
    input.attack_pressure = 10;
    planet_raw[9] = 50;
    planet_raw[10] = 0;
    planet_raw[11] = 0;
    reset(&trace, 0, 9, 0);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 1 &&
        trace.draw_index == 2, &checks, &failures);
    input.fleet_count = 0;
    planet_raw[9] = 51;
    reset(&trace, 0, 9, 0);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 1 &&
        trace.draw_index == 1, &checks, &failures);
    input.fleet_count = 2;
    word(fleet_raw[1], 0, 8);
    fleets[1].id = 8;
    fleets[1].raw_record = fleet_raw[1];
    fleets[1].raw_record_size = 123;
    reset(&trace, 0, 0, 0);
    check(!stars_macinti_should_queue_attack(&input, &queue) &&
        trace.draw_index == 0, &checks, &failures);
    fleets[1].raw_record_size = 124;
    input.fleet_count = 1;
    input.mine_from_fleet = 0;
    reset(&trace, 0, 0, 0);
    check(!stars_macinti_should_queue_attack(&input, &queue) &&
        trace.draw_index == 1, &checks, &failures);
    input.mine_from_fleet = mine_from_fleet;
    input.attack_design = 13;
    reset(&trace, 0, 0, 0);
    check(stars_macinti_should_queue_attack(&input, &queue) && queue == 0 &&
        trace.draw_index == 0, &checks, &failures);
    input.attack_design = 14;
    reset(&trace, 2, 0, 0);
    check(!stars_macinti_should_queue_attack(&input, &queue),
        &checks, &failures);
    outer.planet = &planet;
    outer.turn = 5;
    outer.random_value = random_value;
    outer.should_planet_build_colonizer = should_planet_build_colonizer;
    outer.context = &trace;
    reset(&trace, 99, 0, 0);
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_ATTACK && trace.draw_index == 1 &&
        trace.bounds[0] == 100 && trace.colonizer_calls == 0,
        &checks, &failures);
    outer.turn = 121;
    outer.colonizer_count = 50;
    reset(&trace, 7, 0, 0);
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_ATTACK && trace.draw_index == 1 &&
        trace.colonizer_calls == 0, &checks, &failures);
    outer.turn = 5;
    outer.colonizer_count = 0;
    reset(&trace, 7, 0, 0);
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_SKIP && trace.colonizer_calls == 0,
        &checks, &failures);
    word(planet_raw, 28, 30);
    word(planet_raw, 32, 30);
    word(planet_raw, 36, 30);
    reset(&trace, 7, 0, 0);
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_ATTACK && trace.colonizer_calls == 1,
        &checks, &failures);
    reset(&trace, 7, 0, 0);
    trace.colonizer_result = 1;
    outer.turn = 4;
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_COLONIZER_ONLY,
        &checks, &failures);
    outer.turn = 5;
    reset(&trace, 7, 0, 0);
    trace.colonizer_result = 1;
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_COLONIZER_THEN_ATTACK,
        &checks, &failures);
    outer.should_build_colonizers = 1;
    reset(&trace, 7, 0, 0);
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_ATTACK && trace.draw_index == 0,
        &checks, &failures);
    outer.turn = 121;
    outer.colonizer_count = 41;
    reset(&trace, 99, 0, 0);
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_ATTACK && trace.draw_index == 1,
        &checks, &failures);
    outer.turn = 100;
    reset(&trace, 99, 0, 0);
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_ATTACK && trace.draw_index == 0,
        &checks, &failures);
    outer.should_build_colonizers = 0;
    outer.colonizer_count = 0;
    word(planet_raw, 30, 0xffffU);
    reset(&trace, 7, 0, 0);
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_SKIP, &checks, &failures);
    word(planet_raw, 30, 1);
    word(planet_raw, 28, 0);
    reset(&trace, 7, 0, 0);
    check(stars_macinti_attack_outer_route(&outer, &route) &&
        route == STARS_MACINTI_OUTER_ATTACK,
        &checks, &failures);
    reset(&trace, 100, 0, 0);
    check(!stars_macinti_attack_outer_route(&outer, &route),
        &checks, &failures);
    production.outer = outer;
    production.attack = input;
    production.mission_design = -1;
    production.colonizer_design = 1;
    production.resources_at_planet = 51;
    production.colonizer_ability = 2;
    production.growth_percent = 40;
    production.queue_production = queue_production;
    production.queue_context = &trace;
    word(planet_raw, 40, 100);
    word(planet_raw, 42, 0);
    production.attack.attack_design = 14;
    production.attack.design_existing = 4999;
    production.attack.attack_pressure = 10;
    reset(&trace, 99, 0, 0);
    trace.production_result = &produced;
    check(stars_macinti_run_attack_production(&production, &produced) &&
        produced.attack_queued == 1 && produced.colonizer_queues == 0 &&
        produced.attack_pressure == 11 && trace.queue_calls == 1 &&
        trace.queue_designs[0] == 14 && trace.queue_pressures[0] == 11,
        &checks, &failures);
    reset(&trace, 7, 0, 0);
    trace.colonizer_result = 1;
    trace.production_result = &produced;
    check(stars_macinti_run_attack_production(&production, &produced) &&
        produced.outer_route == STARS_MACINTI_OUTER_COLONIZER_THEN_ATTACK &&
        produced.colonizer_queues == 3 && produced.colonizer_count == 2 &&
        produced.attack_queued == 1 && produced.attack_pressure == 11 &&
        trace.queue_calls == 4 && trace.queue_designs[0] == 1 &&
        trace.queue_designs[1] == 1 && trace.queue_designs[2] == 1 &&
        trace.queue_designs[3] == 14 && trace.queue_counts[0] == 1 &&
        trace.queue_counts[1] == 2 && trace.queue_counts[2] == 2 &&
        trace.queue_pressures[2] == 10 && trace.queue_pressures[3] == 11,
        &checks, &failures);
    production.outer.turn = 4;
    reset(&trace, 7, 0, 0);
    trace.colonizer_result = 1;
    trace.production_result = &produced;
    check(stars_macinti_run_attack_production(&production, &produced) &&
        produced.outer_route == STARS_MACINTI_OUTER_COLONIZER_ONLY &&
        produced.colonizer_queues == 1 && produced.colonizer_count == 1 &&
        produced.attack_queued == 0 && trace.draw_index == 1,
        &checks, &failures);
    production.outer.turn = 5;
    word(planet_raw, 28, 0);
    word(planet_raw, 30, 0);
    reset(&trace, 7, 0, 0);
    trace.production_result = &produced;
    check(stars_macinti_run_attack_production(&production, &produced) &&
        produced.outer_route == STARS_MACINTI_OUTER_SKIP &&
        trace.queue_calls == 0 && produced.attack_pressure == 10,
        &checks, &failures);
    word(planet_raw, 28, 30);
    production.attack.design_existing = 5000;
    reset(&trace, 7, 0, 0);
    trace.colonizer_result = 1;
    trace.production_result = &produced;
    check(stars_macinti_run_attack_production(&production, &produced) &&
        produced.colonizer_queues == 3 && produced.attack_queued == 0 &&
        trace.queue_calls == 3 && produced.attack_pressure == 10,
        &checks, &failures);
    production.attack.design_existing = 4999;
    reset(&trace, 7, 0, 0);
    trace.colonizer_result = 1;
    trace.fail_queue = 2;
    trace.production_result = &produced;
    check(!stars_macinti_run_attack_production(&production, &produced) &&
        produced.colonizer_queues == 1 && produced.colonizer_count == 2 &&
        produced.attack_pressure == 10 && trace.queue_calls == 2,
        &checks, &failures);
    reset(&trace, 7, 0, 0);
    trace.colonizer_result = 1;
    trace.fail_queue = 4;
    trace.production_result = &produced;
    check(!stars_macinti_run_attack_production(&production, &produced) &&
        produced.colonizer_queues == 3 && produced.attack_queued == 0 &&
        produced.attack_pressure == 11 && trace.queue_calls == 4,
        &checks, &failures);
    production.mission_design = 10;
    production.mission_count = 0;
    production.player_planet_count = 8;
    reset(&trace, 0, 99, 0);
    trace.draws[2] = 0;
    trace.draw_count = 3;
    trace.production_result = &produced;
    check(stars_macinti_run_attack_production(&production, &produced) &&
        produced.mission_queued == 1 && produced.mission_count == 1 &&
        produced.attack_queued == 1 && trace.queue_calls == 2 &&
        trace.queue_designs[0] == 10 && trace.queue_designs[1] == 14 &&
        trace.bounds[0] == 3 && trace.bounds[1] == 100 &&
        trace.bounds[2] == 2 && produced.attack_pressure == 11,
        &checks, &failures);
    production.mission_count = 2;
    reset(&trace, 99, 0, 0);
    trace.production_result = &produced;
    check(stars_macinti_run_attack_production(&production, &produced) &&
        produced.mission_queued == 0 && produced.mission_count == 2 &&
        trace.queue_calls == 1 && trace.queue_designs[0] == 14 &&
        trace.bounds[0] == 100, &checks, &failures);
    production.mission_count = 0;
    reset(&trace, 1, 99, 0);
    trace.draws[2] = 0;
    trace.draw_count = 3;
    trace.production_result = &produced;
    check(stars_macinti_run_attack_production(&production, &produced) &&
        produced.mission_queued == 0 && produced.mission_count == 0 &&
        produced.attack_queued == 1 && trace.queue_calls == 1 &&
        trace.draw_index == 3, &checks, &failures);
    fleets[0].raw_record_size = 123;
    reset(&trace, 0, 99, 0);
    trace.production_result = &produced;
    check(!stars_macinti_run_attack_production(&production, &produced) &&
        trace.queue_calls == 0 && trace.draw_index == 0,
        &checks, &failures);
    fleets[0].raw_record_size = 124;
    printf("MACINTI_ATTACK_GATE_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
