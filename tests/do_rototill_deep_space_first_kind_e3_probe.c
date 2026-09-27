#include "core.h"
#include <stdio.h>
#include <string.h>

#define ORDER_BYTES (4U + 3U * 18U)

typedef struct Trace {
    StarsI16 select_count;
    StarsI16 refresh_count;
    StarsI16 starbase_count;
    StarsI16 starbase_result;
    StarsI16 bad;
} Trace;

static StarsI16 select_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->select_count;
    if (player != 4 || fleet->id != 3 ||
            fleet->raw_plord[10] != 0xa2U ||
            fleet->raw_fleet_record[5] != 0x20U)
        trace->bad = 1;
    return 1;
}

static StarsI16 refresh_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->refresh_count;
    if (player != 4 || fleet->raw_plord[10] != 0xa5U ||
            fleet->raw_plord[3] != 3U)
        trace->bad = 1;
    return 1;
}

static StarsI16 nearest_starbase(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 mode)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->starbase_count;
    if (fleet->id != 3 || mode != 0 || trace->select_count != 1)
        trace->bad = 1;
    return trace->starbase_result;
}

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes) {
#ifndef TARGET_NATIVE
        fprintf(stderr, "failed check %d\n", *checks);
#endif
        ++*failures;
    }
}

static void seed(StarsU8 raw[124], StarsU8 plord[ORDER_BYTES])
{
    StarsU16 index;
    memset(raw, 0, 124U);
    memset(plord, 0, ORDER_BYTES);
    raw[0] = 3U;
    raw[2] = 4U;
    raw[5] = 0xa0U;
    raw[6] = 0xffU;
    raw[7] = 0xffU;
    raw[14] = 1U;
    raw[98] = 1U;
    plord[3] = 3U;
    for (index = 4U; index < ORDER_BYTES; ++index)
        plord[index] = (StarsU8)(index + 0x80U);
    plord[10] = 0xa2U;
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnFleet fleet;
    StarsRototillTurnPlanet planet;
    StarsU8 raw[124];
    StarsU8 raw_planet[56];
    StarsU8 plord[ORDER_BYTES];
    StarsU8 expected[ORDER_BYTES];
    StarsU8 ai[12U * 16U];
    StarsU8 ai_before[12U * 16U];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    seed(raw, plord);
    memcpy(expected, plord, sizeof expected);
    expected[10] = 0xa5U;
    memset(ai, 0x71, sizeof ai);
    memcpy(ai_before, ai, sizeof ai);
    fleet.id = 3;
    fleet.raw_fleet_record = raw;
    fleet.raw_fleet_record_size = sizeof raw;
    fleet.raw_plord = plord;
    fleet.raw_plord_bytes = sizeof plord;
    ops.context = &trace;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.move_nearest_starbase = nearest_starbase;
    input.turn = 20;
    input.player_id = 4;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.ops = &ops;
    input.raw_ai_planet_table = ai;
    input.raw_ai_planet_record_count = 12;
    input.fleet_first_kind_deep_space_bound = 1;
    input.ship1_first_hull_slot_byte = 1U;

    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(memcmp(plord, expected, sizeof plord) == 0,
        &checks, &failures);
    check(memcmp(ai, ai_before, sizeof ai) == 0 &&
        raw[5] == 0x20U, &checks, &failures);
    check(trace.select_count == 2 && trace.refresh_count == 1 &&
        !trace.bad, &checks, &failures);

    seed(raw, plord);
    memcpy(expected, plord, sizeof expected);
    memset(&trace, 0, sizeof trace);
    input.ship1_first_hull_slot_byte = 2U;
    trace.starbase_result = 1;
    check(stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        trace.select_count == 1 && trace.starbase_count == 1,
        &checks, &failures);
    input.ship1_first_hull_slot_byte = 1U;

    seed(raw, plord);
    memset(&trace, 0, sizeof trace);
    raw[88] = 1U;
    check(stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        trace.select_count == 0, &checks, &failures);

    seed(raw, plord);
    memset(&trace, 0, sizeof trace);
    raw[26] = 1U;
    check(stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        trace.select_count == 0, &checks, &failures);

    seed(raw, plord);
    memset(&trace, 0, sizeof trace);
    raw[2] = 5U;
    check(stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        trace.select_count == 0, &checks, &failures);

    seed(raw, plord);
    fleet.raw_plord_bytes = 21U;
    check(!stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        raw[5] == 0xa0U, &checks, &failures);
    fleet.raw_plord_bytes = sizeof plord;

    input.fleet_first_kind_deep_space_bound = 0;
    input.fleet_first_kind_selected_planet_bound = 1;
    input.planet_count = 1;
    input.planets = &planet;
    planet.id = 10;
    planet.raw_planet_record = raw_planet;
    planet.raw_planet_record_size = sizeof raw_planet;
    memset(raw_planet, 0, sizeof raw_planet);
    raw_planet[0] = 10U;
    raw_planet[2] = 4U;
    raw_planet[40] = 40U;
    seed(raw, plord);
    raw[6] = 10U;
    raw[7] = 0U;
    memcpy(expected, plord, sizeof expected);
    expected[10] = 0xa5U;
    memset(&trace, 0, sizeof trace);
    check(stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0,
        &checks, &failures);
    check(trace.select_count == 2 && trace.refresh_count == 1 &&
        trace.starbase_count == 0 && !trace.bad,
        &checks, &failures);

    seed(raw, plord);
    raw[6] = 10U;
    raw[7] = 0U;
    raw_planet[5] = 2U;
    memcpy(expected, plord, sizeof expected);
    memset(&trace, 0, sizeof trace);
    check(stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        trace.select_count == 0, &checks, &failures);

    raw_planet[5] = 0U;
    raw_planet[40] = 50U;
    check(stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        trace.select_count == 0, &checks, &failures);

    raw_planet[2] = 5U;
    raw_planet[5] = 2U;
    memset(&trace, 0, sizeof trace);
    check(stars_do_rototill_ai_turn(&input) &&
        plord[10] == 0xa5U && trace.select_count == 2 &&
        trace.refresh_count == 1, &checks, &failures);

    raw_planet[2] = 4U;
    raw_planet[5] = 0U;
    raw_planet[40] = 40U;
    seed(raw, plord);
    raw[6] = 10U;
    raw[7] = 0U;
    memcpy(expected, plord, sizeof expected);
    input.ship1_first_hull_slot_byte = 2U;
    memset(&trace, 0, sizeof trace);
    trace.starbase_result = 1;
    check(stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        trace.select_count == 1 && trace.starbase_count == 1 &&
        trace.refresh_count == 0 && !trace.bad,
        &checks, &failures);

    seed(raw, plord);
    raw[6] = 10U;
    raw[7] = 0U;
    memset(&trace, 0, sizeof trace);
    trace.starbase_result = 0;
    check(stars_do_rototill_ai_turn(&input) &&
        plord[10] == 0xa5U && trace.select_count == 2 &&
        trace.starbase_count == 1 && trace.refresh_count == 1 &&
        !trace.bad, &checks, &failures);

    seed(raw, plord);
    raw[6] = 10U;
    raw[7] = 0U;
    memcpy(expected, plord, sizeof expected);
    ops.move_nearest_starbase = 0;
    check(!stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        raw[5] == 0xa0U, &checks, &failures);
    ops.move_nearest_starbase = nearest_starbase;

    input.ship1_first_hull_slot_byte = 1U;
    planet.raw_planet_record = 0;
    check(!stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        raw[5] == 0xa0U, &checks, &failures);
    planet.raw_planet_record = raw_planet;

    seed(raw, plord);
    raw[6] = 10U;
    raw[7] = 0U;
    plord[3] = 0U;
    memcpy(expected, plord, sizeof expected);
    check(!stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, expected, sizeof plord) == 0 &&
        raw[5] == 0xa0U, &checks, &failures);

#ifndef TARGET_NATIVE
    printf("ROTOTILL_FIRST_KIND_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
