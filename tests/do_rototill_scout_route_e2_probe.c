#include "core.h"
#include "ROTSCFX.H"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    char calls[12];
    StarsI16 count;
    StarsI16 draw;
    StarsI16 bad;
    StarsI16 engine_lookups;
    StarsI16 realloc_calls;
    StarsU8 *expected_plord;
    StarsU8 *ai_planets;
    StarsI16 worm_route;
    StarsI16 fallback_mode;
    StarsI16 home_mode;
    StarsI16 expected_target;
    StarsI16 enemy_mode;
    StarsI16 route_enemy;
    StarsI16 route_far_planet;
    StarsI16 teeth_calls;
    StarsI16 teeth_lookups;
} Trace;

static Trace trace;
static StarsU8 expanded_plord[58];
static const StarsU8 near_wormhole[18] = {
    2U, 64U, 20U, 5U, 226U, 4U, 0U, 0U, 0U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};

static void call(char value)
{
    if (trace.count >= 11)
        trace.bad = 1;
    else
        trace.calls[trace.count++] = value;
}

static StarsI16 is_transport(void *context,
    const StarsRototillTurnFleet *fleet)
{
    (void)context;
    call('I');
    if (fleet->id != 0 || fleet->ship_counts[0] != 1)
        trace.bad = 1;
    return 0;
}

static StarsI16 select_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    (void)context;
    call('S');
    if (fleet->id != 0 || player != 1 ||
            fleet->raw_fleet_record[5] != 0x20U)
        trace.bad = 1;
    return 1;
}

static StarsI16 refresh_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    (void)context;
    call('R');
    if (player != 1 || fleet->raw_fleet_record[98] != 2U ||
            fleet->raw_plord[3] != 2U ||
            fleet->raw_plord != trace.expected_plord ||
            fleet->raw_plord_bytes != 58U)
        trace.bad = 1;
    return 1;
}

static StarsI16 ensure_plord(void *context,
    const StarsRototillTurnFleet *fleet, StarsU16 required,
    StarsU8 **plord, StarsU16 *bytes)
{
    (void)context;
    call('A');
    ++trace.realloc_calls;
    if (required != 2U || fleet->raw_plord_bytes != 22U ||
            fleet->raw_plord[3] != 1U)
        trace.bad = 1;
    memset(expanded_plord, 0xa7, sizeof expanded_plord);
    memcpy(expanded_plord, fleet->raw_plord, 22U);
    *plord = expanded_plord;
    *bytes = sizeof expanded_plord;
    return 1;
}

static StarsI16 random_value(void *context, StarsI16 maximum)
{
    (void)context;
    call('N');
    if (maximum == 100)
        return trace.draw;
    if (trace.enemy_mode && maximum == 3)
        return trace.enemy_mode == 3 ? 1 : 0;
    if (trace.fallback_mode && maximum == 32)
        return 7;
    trace.bad = 1;
    return 0;
}

static const StarsHullComponent *lookup_hull(void *context,
    StarsU16 owner, StarsU16 design, StarsU16 *count)
{
    (void)context;
    ++trace.teeth_lookups;
    if (owner != 1U || design != 0U || !count)
        trace.bad = 1;
    if (count)
        *count = sizeof retained_hull_0 / sizeof retained_hull_0[0];
    return retained_hull_0;
}

static StarsI16 has_teeth(void *context,
    const StarsRototillTurnFleet *fleet)
{
    StarsI16 result;
    call('T');
    ++trace.teeth_calls;
    if (fleet->id != 0)
        trace.bad = 1;
    result = stars_fleet_might_have_teeth(fleet->ship_counts,
        (StarsU16)fleet->owner << 9, lookup_hull, context);
    if (result != 1)
        trace.bad = 1;
    return result;
}

static StarsU32 fuel_capacity(void *context,
    const StarsRototillTurnFleet *fleet)
{
    (void)context;
    call('F');
    if (fleet->id != 0 || trace.enemy_mode < 5)
        trace.bad = 1;
    return trace.enemy_mode == 7 || trace.enemy_mode == 8 ?
        700UL : 500UL;
}

static StarsI16 move_to_starbase(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    (void)context;
    call('M');
    if (fleet->id != 0 || player != 1 ||
            (trace.enemy_mode != 7 && trace.enemy_mode != 8))
        trace.bad = 1;
    return trace.enemy_mode == 8;
}

static StarsI16 best_home_target(void *context, StarsI16 home)
{
    (void)context;
    call('B');
    if (home != 24 || !trace.fallback_mode || trace.home_mode == 0)
        trace.bad = 1;
    return trace.home_mode == 1 ? 7 : -1;
}

static const StarsU8 *find_wormhole(void *context,
    StarsPoint source, StarsU32 distance)
{
    call('H');
    if (source.x != SOURCE_X || source.y != SOURCE_Y || distance != 545UL)
        trace.bad = 1;
    if (trace.worm_route)
        return near_wormhole;
    return stars_lpth_worm_find(source, distance, 1,
        retained_wormholes, 4U, 0, random_value, context);
}

static const StarsIdealWarpEngine *lookup_engine(void *context,
    StarsU16 engine_id)
{
    static StarsIdealWarpEngine engine;
    StarsU16 index;
    (void)context;
    call('E');
    ++trace.engine_lookups;
    if (engine_id != RETAINED_ENGINE_ID)
        trace.bad = 1;
    memset(&engine, 0, sizeof engine);
    for (index = 0U; index < 12U; ++index)
        engine.fuel_used[index] = retained_engine_fuel[index];
    return &engine;
}

static StarsI16 ideal_warp(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 target, StarsPoint point)
{
    StarsFindIdealWarpInput input;
    call('W');
    if (fleet->id != 0 || !trace.ai_planets ||
            target != (trace.worm_route ? 0x4002 :
                      trace.expected_target) ||
            point.x != (trace.worm_route || trace.route_enemy ? 1300 :
                       retained_points[trace.expected_target].x) ||
            point.y != (trace.worm_route || trace.route_enemy ? 1250 :
                       retained_points[trace.expected_target].y) ||
            trace.ai_planets[trace.expected_target * 16U + 15U] !=
                (trace.route_enemy ? 0U :
                 trace.worm_route ? 0x10U : 4U))
        trace.bad = 1;
    memset(&input, 0, sizeof input);
    memcpy(input.ship_counts, fleet->ship_counts, sizeof input.ship_counts);
    input.designs[0].engine_count = 1U;
    input.designs[0].engine_flags[0] = 1U;
    input.designs[0].engine_ids[0] = RETAINED_ENGINE_ID;
    input.lookup_engine = lookup_engine;
    input.context = context;
    return stars_find_ideal_warp(&input, 1);
}

static void seed(StarsU8 raw[124], StarsU8 plord[58])
{
    memset(raw, 0, 124U);
    memset(plord, 0xa7, 58U);
    raw[2] = 1U;
    raw[5] = 0xa0U;
    raw[6] = 24U;
    raw[8] = (StarsU8)SOURCE_X;
    raw[9] = (StarsU8)(SOURCE_X >> 8);
    raw[10] = (StarsU8)SOURCE_Y;
    raw[11] = (StarsU8)(SOURCE_Y >> 8);
    raw[12] = 1U;
    raw[92] = (StarsU8)SOURCE_FUEL;
    raw[93] = (StarsU8)(SOURCE_FUEL >> 8);
    raw[98] = 1U;
    plord[3] = 1U;
    plord[4] = (StarsU8)SOURCE_X;
    plord[5] = (StarsU8)(SOURCE_X >> 8);
    plord[6] = (StarsU8)SOURCE_Y;
    plord[7] = (StarsU8)(SOURCE_Y >> 8);
    plord[8] = 24U;
    plord[9] = 0U;
    plord[10] = 0U;
    plord[11] = 0x11U;
}

static void check(StarsI16 condition, StarsI16 *checks,
    StarsI16 *failures)
{
    ++*checks;
    if (!condition)
        ++*failures;
}

static void run_case(StarsI16 draw, StarsI16 grow, StarsI16 worm,
    StarsI16 fallback, StarsI16 home_mode, StarsI16 enemy_mode,
    StarsI16 *checks,
    StarsI16 *failures)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillScoutAdapter adapter;
    StarsRototillTurnFleet fleet;
    StarsRototillTurnPlanet planet;
    StarsU8 raw[124];
    StarsU8 plord[58];
    StarsU8 before[58];
    StarsU8 *active_plord;
    StarsU8 ai[32U * 16U];
    StarsU8 status[32];
    StarsRototillScoutEnemy enemy;
    StarsI16 owners[32];
    StarsI16 reserved_planet;
    StarsU16 index;
    StarsI16 failures_before;

    failures_before = *failures;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&adapter, 0, sizeof adapter);
    memset(&fleet, 0, sizeof fleet);
    memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    seed(raw, plord);
    memcpy(before, plord, sizeof before);
    memset(ai, 0, sizeof ai);
    memset(status, 0, sizeof status);
    trace.draw = draw;
    trace.worm_route = worm;
    trace.fallback_mode = fallback;
    trace.home_mode = home_mode;
    trace.enemy_mode = enemy_mode;
    trace.route_enemy = enemy_mode == 1 || enemy_mode == 3 ||
        enemy_mode == 4;
    trace.route_far_planet = enemy_mode == 6 || enemy_mode == 7;
    trace.expected_target = trace.route_far_planet ? 31 :
        trace.route_enemy ? 3 :
        fallback ? 7 : EXPECTED_TARGET;
    trace.expected_plord = grow ? expanded_plord : plord;
    trace.ai_planets = ai;
    if (fallback) {
        ai[24U * 16U + 15U] = 1U;
        adapter.marked_planets = 1;
    }
    fleet.raw_fleet_record = raw;
    fleet.raw_fleet_record_size = sizeof raw;
    fleet.raw_plord = plord;
    fleet.raw_plord_bytes = grow ? 22U : sizeof plord;
    planet.id = 24;
    planet.owner = 1;
    adapter.raw_ai_planet_table = ai;
    adapter.ai_planet_record_count = 32;
    adapter.planet_points = retained_points;
    adapter.planet_count = 32;
    memcpy(owners, retained_owners, sizeof owners);
    if (trace.route_far_planet || enemy_mode == 9)
        owners[31] = 1;
    adapter.planet_owners = owners;
    adapter.status_scratch = status;
    adapter.status_scratch_count = 32;
    adapter.race_mode = 3;
    adapter.turn = 0;
    adapter.random_value = random_value;
    adapter.find_wormhole = find_wormhole;
    memset(&enemy, 0, sizeof enemy);
    enemy.id = 3;
    enemy.point.x = enemy_mode >= 5 ? 1500 : 1300;
    enemy.point.y = enemy_mode >= 5 ? 1300 : 1250;
    enemy.human = enemy_mode == 4 ? 0 : 1;
    enemy.attack_reserved = enemy_mode == 2 || enemy_mode == 3;
    adapter.enemy_candidate_count = enemy_mode ? 1U : 0U;
    adapter.enemy_candidates = enemy_mode ? &enemy : 0;
    adapter.only_humans = enemy_mode == 4;
    adapter.has_teeth = has_teeth;
    adapter.fuel_capacity = enemy_mode >= 5 ? fuel_capacity : 0;
    adapter.move_to_starbase = enemy_mode >= 5 ?
        move_to_starbase : 0;
    reserved_planet = 31;
    adapter.attack_planet_ids = enemy_mode == 9 ?
        &reserved_planet : 0;
    adapter.attack_planet_count = enemy_mode == 9 ? 1U : 0U;
    adapter.home_planet_id = fallback && home_mode ? 24 : -1;
    adapter.best_home_target = fallback && home_mode ?
        best_home_target : 0;
    adapter.ideal_warp = ideal_warp;
    adapter.ensure_plord_capacity = grow ? ensure_plord : 0;
    adapter.select_fleet = select_fleet;
    adapter.refresh_fleet = refresh_fleet;
    input.player_id = 1;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.fleet_scout_bound = 1;
    input.raw_ai_planet_table = ai;
    input.raw_ai_planet_record_count = 32;
    ops.context = &adapter;
    ops.is_ai_transport = is_transport;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.target_scout = stars_rototill_target_scout;
    input.ops = &ops;

    check(stars_do_rototill_ai_turn(&input) == 1, checks, failures);
    if (trace.teeth_calls != (enemy_mode == 4 ? 2 : 1) ||
            trace.teeth_lookups != trace.teeth_calls)
        trace.bad = 1;
    check(strcmp(trace.calls,
          enemy_mode ?
              (enemy_mode == 1 ? "ISTWESR" :
               enemy_mode == 2 ? "ISTNNWESR" :
               enemy_mode == 3 ? "ISTNWESR" :
               enemy_mode == 4 ? "ISTTWESR" :
               enemy_mode == 5 ? "ISTFNWESR" :
               enemy_mode == 6 ? "ISTFWESR" :
               enemy_mode == 7 ? "ISTFMWESR" :
               enemy_mode == 8 ? "ISTFM" : "ISTFNWESR") :
          fallback ? (home_mode == 0 ? "ISTNNWESR" :
                      home_mode == 1 ? "ISTNBWESR" : "ISTNBNWESR") :
          draw < 5 ? (grow ? "ISTNHWESAR" : "ISTNHWESR") :
                     (grow ? "ISTNWESAR" : "ISTNWESR")) == 0 &&
          !trace.bad, checks, failures);
    if (enemy_mode == 8) {
        check(!trace.bad, checks, failures);
        check(adapter.marked_planets == 0, checks, failures);
        for (index = 0U; index < 32U; ++index)
            if (ai[index * 16U + 15U] != 0U)
                trace.bad = 1;
        check(!trace.bad, checks, failures);
        check(raw[98U] == 1U && plord[3U] == 1U,
            checks, failures);
        check(memcmp(plord, before, sizeof before) == 0,
            checks, failures);
        check(trace.engine_lookups == 0, checks, failures);
        check(trace.realloc_calls == 0, checks, failures);
        check(raw[5U] == 0x20U, checks, failures);
        return;
    }
    check(adapter.marked_planets ==
              (trace.route_enemy || trace.route_far_planet ? 0 : 1) &&
          ai[24U * 16U + 15U] ==
              (trace.route_enemy || trace.route_far_planet ? 0U : 1U) &&
          ai[trace.expected_target * 16U + 15U] ==
              (trace.route_enemy ? 0U : worm ? 0x10U : 4U),
          checks, failures);
    for (index = 0; index < 32U; ++index)
        if ((trace.route_enemy || trace.route_far_planet ||
             index != 24U) &&
                (worm || trace.route_enemy ||
                 index != trace.expected_target) &&
                ai[index * 16U + 15U] !=
                    (enemy_mode == 9 && index == 31U ? 1U :
                     trace.route_enemy || trace.route_far_planet ||
                     fallback ? 0U : 0x10U))
            trace.bad = 1;
    check(!trace.bad, checks, failures);
    check(trace.engine_lookups == 1 && EXPECTED_WARP == 5,
          checks, failures);
    active_plord = grow ? expanded_plord : plord;
    check(raw[98] == 2U && active_plord[3] == 2U,
          checks, failures);
    check(memcmp(active_plord, before, 3U) == 0 &&
          memcmp(active_plord + 4U, before + 4U, 18U) == 0 &&
          memcmp(active_plord + 40U, before + 40U, 18U) == 0 &&
          (!grow || memcmp(plord, before, 22U) == 0),
          checks, failures);
    check(active_plord[22] == (StarsU8)(worm || trace.route_enemy ? 1300 :
              retained_points[trace.expected_target].x) &&
          active_plord[23] == (StarsU8)((worm || trace.route_enemy ? 1300 :
              retained_points[trace.expected_target].x) >> 8) &&
          active_plord[24] == (StarsU8)(worm || trace.route_enemy ? 1250 :
              retained_points[trace.expected_target].y) &&
          active_plord[25] == (StarsU8)((worm || trace.route_enemy ? 1250 :
              retained_points[trace.expected_target].y) >> 8) &&
          active_plord[26] == (worm ? 2U : trace.expected_target) &&
          active_plord[27] == (worm ? 0x40U : 0U) &&
          active_plord[28] == 0x50U &&
          active_plord[29] == (worm ? 0x18U :
                               trace.route_enemy ? 0x12U : 0x11U),
          checks, failures);
    for (index = 30U; index < 40U; ++index)
        if (active_plord[index] != 0U)
            trace.bad = 1;
    check(!trace.bad, checks, failures);
    check(trace.realloc_calls == grow, checks, failures);
#ifndef TARGET_NATIVE
    if (*failures != failures_before)
        printf("CASE draw=%d grow=%d worm=%d fallback=%d home=%d enemy=%d failures=%d calls=%s target=%d order=%u,%u,%u,%u id=%u flags=%u,%u ai=%u marked=%d bad=%d\n",
            draw, grow, worm, fallback, home_mode, enemy_mode,
            *failures - failures_before, trace.calls, trace.expected_target,
            active_plord[22], active_plord[23], active_plord[24],
            active_plord[25], active_plord[26], active_plord[28],
            active_plord[29], ai[trace.expected_target * 16U + 15U],
            adapter.marked_planets, trace.bad);
#endif
}

int main(void)
{
    StarsI16 checks = 0;
    StarsI16 failures = 0;
    run_case(99, 0, 0, 0, 0, 0, &checks, &failures);
    run_case(0, 0, 0, 0, 0, 0, &checks, &failures);
    run_case(99, 1, 0, 0, 0, 0, &checks, &failures);
    run_case(0, 1, 0, 0, 0, 0, &checks, &failures);
    run_case(0, 0, 1, 0, 0, 0, &checks, &failures);
    run_case(0, 1, 1, 0, 0, 0, &checks, &failures);
    run_case(99, 0, 0, 1, 0, 0, &checks, &failures);
    run_case(99, 0, 0, 1, 1, 0, &checks, &failures);
    run_case(99, 0, 0, 1, 2, 0, &checks, &failures);
    run_case(99, 0, 0, 0, 0, 1, &checks, &failures);
    run_case(99, 0, 0, 0, 0, 2, &checks, &failures);
    run_case(99, 0, 0, 0, 0, 3, &checks, &failures);
    run_case(99, 0, 0, 0, 0, 4, &checks, &failures);
    run_case(99, 0, 0, 0, 0, 5, &checks, &failures);
    run_case(99, 0, 0, 0, 0, 6, &checks, &failures);
    run_case(99, 0, 0, 0, 0, 7, &checks, &failures);
    run_case(99, 0, 0, 0, 0, 8, &checks, &failures);
    run_case(99, 0, 0, 0, 0, 9, &checks, &failures);
#ifndef TARGET_NATIVE
    printf("ROTOTILL_SCOUT_ROUTE_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
