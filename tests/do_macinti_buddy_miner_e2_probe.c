#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 draws[4];
    StarsI16 bounds[4];
    StarsI16 draw_count;
    StarsI16 joined;
    StarsI16 design_available;
    StarsI16 bad;
    char events[12];
    StarsI16 count;
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

static void setup(Trace *trace, StarsU8 *raw, StarsU8 *plord,
    StarsU8 *planet)
{
    memset(raw, 0, 124);
    memset(plord, 0, 22);
    memset(planet, 0, 56);
    word(raw, 0, 9);
    word(raw, 2, 2);
    word(raw, 6, 1);
    word(raw, 98, 1);
    word(raw, 40, 1);
    plord[3] = 1U;
    word(plord, 10, 0xa3f3U);
    word(planet, 0, 1);
    word(planet, 2, 2);
    planet[9] = 10U;
    planet[10] = 10U;
    planet[11] = 10U;
    trace->draw_count = 0;
    trace->joined = 0;
    trace->design_available = 1;
    trace->bad = 0;
    trace->count = 0;
    trace->draws[0] = 1;
    trace->draws[1] = 1;
    trace->draws[2] = 1;
    trace->draws[3] = 1;
    memset(trace->events, 0, sizeof trace->events);
}

static StarsI16 random_value(void *opaque, StarsI16 bound)
{
    Trace *trace;
    StarsI16 index;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'Q';
    index = trace->draw_count++;
    if (index >= 4)
        trace->bad = 1;
    else
        trace->bounds[index] = bound;
    return index < 4 ? trace->draws[index] : -1;
}

static StarsI16 find_buddy(void *opaque,
    const StarsMacintiTurnFleet *fleet, StarsI16 low, StarsI16 high,
    StarsI32 near_distance, StarsI32 far_distance, StarsI16 *joined)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'B';
    if (fleet->id != 9 || fleet->cord != 1 ||
            low != 14 || high != 15 ||
            near_distance != 72L || far_distance != 108L)
        trace->bad = 1;
    *joined = trace->joined;
    return 1;
}

static StarsI16 retarget(void *opaque,
    const StarsMacintiTurnFleet *fleet)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'R';
    if (fleet->id != 9 || fleet->cord != 1 ||
            fleet->order_kind != 3)
        trace->bad = 1;
    return 1;
}

static StarsI16 design_status(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace;
    trace = (Trace *)opaque;
    (void)last;
    (void)budget;
    (void)mask;
    *selected = first == 14 && trace->design_available ? 14 : -1;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnInput input;
    StarsMacintiAiTurnOps ops;
    StarsMacintiTurnFleet fleet;
    StarsMacintiTurnPlanet planet;
    StarsU8 raw[124];
    StarsU8 plord[22];
    StarsU8 planet_raw[56];
    StarsU8 ai[4][16];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(&planet, 0, sizeof planet);
    memset(ai, 0, sizeof ai);
    memset(&trace, 0, sizeof trace);
    fleet.id = 9;
    fleet.raw_record = raw;
    fleet.raw_record_size = 124;
    fleet.raw_plord = plord;
    fleet.raw_plord_size = 22;
    planet.id = 1;
    planet.raw_record = planet_raw;
    planet.raw_record_size = 56;
    ops.random_value = random_value;
    ops.find_buddy = find_buddy;
    ops.retarget_miner_live = retarget;
    ops.context = &trace;
    input.turn = 50;
    input.first_sweep_attack_pressure_bound = 1;
    input.first_sweep_attack_pressure = 48;
    input.player_id = 2;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.planet_table_bound = 1;
    input.game_planet_max = 4;
    input.raw_ai_planet_table = &ai[0][0];
    input.raw_ai_planet_record_count = 4;
    input.ops = &ops;

    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure = 59;
    trace.joined = 1;
    check(stars_do_macinti_ai_turn(&input) && !trace.bad &&
        strcmp(trace.events, "B") == 0 &&
        input.typed_fleets_visited == 0,
        &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure = 49;
    planet_raw[11] = 9U;
    check(stars_do_macinti_ai_turn(&input) && !trace.bad &&
        strcmp(trace.events, "QBR") == 0 &&
        trace.bounds[0] == 3, &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure = 58;
    trace.draws[0] = 0;
    planet_raw[11] = 9U;
    check(stars_do_macinti_ai_turn(&input) && !trace.bad &&
        strcmp(trace.events, "QR") == 0 &&
        trace.bounds[0] == 3, &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure = 49;
    trace.draws[0] = 0;
    planet_raw[11] = 9U;
    check(stars_do_macinti_ai_turn(&input) && !trace.bad &&
        strcmp(trace.events, "QR") == 0 &&
        trace.bounds[0] == 3, &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure = 48;
    planet_raw[11] = 9U;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "R") == 0, &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    trace.draws[0] = 0;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "QR") == 0 &&
        trace.bounds[0] == 3, &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    planet_raw[11] = 39U;
    trace.draws[0] = 1;
    trace.draws[1] = 0;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "QQR") == 0 &&
        trace.bounds[0] == 3 && trace.bounds[1] == 10,
        &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    planet_raw[11] = 40U;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "Q") == 0 &&
        trace.bounds[0] == 10, &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    word(planet_raw, 2, 3);
    trace.draws[0] = 0;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "QR") == 0 &&
        trace.bounds[0] == 10, &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure = 59;
    ops.random_value = 0;
    check(!stars_do_macinti_ai_turn(&input) &&
        trace.count == 0, &checks, &failures);
    ops.random_value = random_value;
    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure = 48;
    trace.draws[0] = 3;
    check(!stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "Q") == 0,
        &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure_bound = 0;
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    ops.check_design_status = design_status;
    input.first_sweep_attack_pressure_bound = 1;
    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure = 1;
    check(stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_census_bound == 1 &&
        input.first_sweep_attack_census == 1 &&
        strcmp(trace.events, "QQ") == 0,
        &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    input.first_sweep_attack_pressure = 0;
    check(!stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_census == 1 && trace.count == 0,
        &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    trace.design_available = 0;
    check(stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_census == 0,
        &checks, &failures);
    setup(&trace, raw, plord, planet_raw);
    word(raw, 2, 3);
    check(stars_do_macinti_ai_turn(&input) &&
        input.first_sweep_attack_census == 0 && trace.count == 0,
        &checks, &failures);

    printf("MACINTI_BUDDY_MINER_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
