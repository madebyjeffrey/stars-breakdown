#include "core.h"
#include <stdio.h>
#include <string.h>

#define PLORD_BYTES (4U + 3U * 18U)

typedef struct Trace {
    StarsI16 transport;
    StarsI16 race;
    StarsI16 select_count;
    StarsI16 refresh_count;
    StarsI16 transport_count;
    StarsI16 race_count;
    StarsI16 bad;
} Trace;

static StarsI16 is_transport(void *context,
    const StarsRototillTurnFleet *fleet)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->transport_count;
    if (fleet->id != 3 || fleet->ship_counts[1] != 1)
        trace->bad = 1;
    return trace->transport;
}

static StarsI16 race_stat(void *context, StarsI16 owner, StarsI16 kind)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->race_count;
    if (owner != 6 || kind != 14)
        trace->bad = 1;
    return trace->race;
}

static StarsI16 select_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->select_count;
    if (fleet->id != 3 || player != 4 ||
            fleet->raw_plord[3] != 3U)
        trace->bad = 1;
    return 1;
}

static StarsI16 refresh_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->refresh_count;
    if (fleet->id != 3 || player != 4 ||
            fleet->raw_plord[3] != 3U)
        trace->bad = 1;
    return 1;
}

static void seed(StarsU8 raw[124], StarsU8 plord[PLORD_BYTES],
    StarsI16 first_id)
{
    StarsU16 index;
    memset(raw, 0, 124U);
    memset(plord, 0, PLORD_BYTES);
    raw[0] = 3U;
    raw[2] = 4U;
    raw[5] = 0xa0U;
    raw[6] = 0xffU;
    raw[7] = 0xffU;
    raw[14] = 1U;
    raw[88] = 1U;
    raw[98] = 3U;
    plord[3] = 3U;
    for (index = 4U; index < PLORD_BYTES; ++index)
        plord[index] = (StarsU8)(index + 0x30U);
    plord[8] = (StarsU8)first_id;
    plord[9] = 0U;
    plord[26] = 10U;
    plord[27] = 0U;
    plord[29] = (StarsU8)((plord[29] & 0xf0U) | 1U);
}

static void expected_order(StarsU8 order[18])
{
    memset(order, 0, 18U);
    order[0] = 100U;
    order[2] = 101U;
    order[4] = 10U;
    order[6] = 1U;
    order[7] = 0x11U;
    order[15] = 0x20U;
}

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnFleet fleet;
    StarsRototillTurnPlanet planet;
    StarsPoint points[12];
    StarsU8 raw[124];
    StarsU8 plord[PLORD_BYTES];
    StarsU8 before[PLORD_BYTES];
    StarsU8 expected[PLORD_BYTES];
    StarsU8 order[18];
    StarsU8 ai[12U * 16U];
    StarsU8 ai_expected[sizeof ai];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(&planet, 0, sizeof planet);
    memset(points, 0, sizeof points);
    memset(&trace, 0, sizeof trace);
    points[10].x = 100;
    points[10].y = 101;
    planet.id = 10;
    planet.owner = 6;
    planet.optimal_value = 1;
    ops.is_ai_transport = is_transport;
    ops.race_stat = race_stat;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.context = &trace;
    fleet.id = 3;
    fleet.owner = 4;
    fleet.raw_fleet_record = raw;
    fleet.raw_fleet_record_size = sizeof raw;
    fleet.raw_plord = plord;
    fleet.raw_plord_bytes = sizeof plord;
    input.turn = 20;
    input.player_id = 4;
    input.planet_count = 1;
    input.planets = &planet;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.raw_ai_planet_table = ai;
    input.raw_ai_planet_record_count = 12;
    input.planet_points = points;
    input.planet_point_count = 12;
    input.fleet_transport_reroute_bound = 1;
    input.ops = &ops;
    trace.transport = 1;
    trace.race = 7;
    expected_order(order);

    seed(raw, plord, 10);
    memcpy(before, plord, sizeof plord);
    memcpy(expected, before, sizeof expected);
    memcpy(expected + 4U, order, 18U);
    memset(ai, 0, sizeof ai);
    ai[10U * 16U + 3U] = 1U;
    memcpy(ai_expected, ai, sizeof ai);
    ai_expected[10U * 16U + 9U] = 1U;
    ai_expected[10U * 16U + 10U] = 1U;
    ai_expected[10U * 16U + 3U] = 0x81U;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(memcmp(plord, expected, sizeof plord) == 0,
        &checks, &failures);
    check(memcmp(ai, ai_expected, sizeof ai) == 0 &&
        raw[5] == 0x20U, &checks, &failures);
    check(trace.transport_count == 1 && trace.race_count == 1 &&
        trace.select_count == 1 && trace.refresh_count == 1 &&
        !trace.bad, &checks, &failures);

    seed(raw, plord, 9);
    memcpy(before, plord, sizeof plord);
    memcpy(expected, before, sizeof expected);
    memcpy(expected + 22U, order, 18U);
    memset(ai, 0, sizeof ai);
    ai[10U * 16U + 3U] = 1U;
    memset(&trace, 0, sizeof trace);
    trace.transport = 0;
    trace.race = 7;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(memcmp(plord, expected, sizeof plord) == 0 &&
        ai[10U * 16U + 3U] == 0x81U,
        &checks, &failures);
    check(trace.transport_count == 1 && trace.race_count == 1 &&
        trace.select_count == 1 && trace.refresh_count == 1 &&
        !trace.bad, &checks, &failures);

    seed(raw, plord, 9);
    memcpy(before, plord, sizeof plord);
    memset(ai, 0, sizeof ai);
    ai[10U * 16U + 3U] = 1U;
    memset(&trace, 0, sizeof trace);
    trace.transport = 1;
    trace.race = 8;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(memcmp(plord, before, sizeof plord) == 0 &&
        ai[10U * 16U + 3U] == 1U &&
        trace.select_count == 0 && trace.refresh_count == 0,
        &checks, &failures);

    seed(raw, plord, 9);
    memcpy(before, plord, sizeof plord);
    raw[88] = 0U;
    memset(&trace, 0, sizeof trace);
    trace.transport = 1;
    trace.race = 7;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(memcmp(plord, before, sizeof plord) == 0 &&
        trace.race_count == 0 && trace.select_count == 0,
        &checks, &failures);

    seed(raw, plord, 9);
    memcpy(before, plord, sizeof plord);
    ops.refresh_fleet = 0;
    check(!stars_do_rototill_ai_turn(&input) &&
        memcmp(plord, before, sizeof plord) == 0 &&
        raw[5] == 0xa0U, &checks, &failures);
    ops.refresh_fleet = refresh_fleet;

    seed(raw, plord, 9);
    plord[26] = 12U;
    memcpy(before, plord, sizeof plord);
    memset(ai, 0xa5, sizeof ai);
    check(!stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(memcmp(plord, before, sizeof plord) == 0 &&
        raw[5] == 0xa0U && ai[10U * 16U + 9U] == 0xa5U,
        &checks, &failures);

#ifndef TARGET_NATIVE
    printf("ROTOTILL_REROUTE_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
