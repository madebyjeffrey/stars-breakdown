#include "core.h"
#include <stdio.h>
#include <string.h>

#define ORDER_BYTES (4U + 3U * 18U)

typedef struct Trace {
    StarsI16 select_count;
    StarsI16 refresh_count;
    StarsI16 clear_count;
    StarsI16 fail_select;
    StarsI16 bad;
} Trace;

static StarsI16 select_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->select_count;
    if (player != 4 || fleet->id != 3 ||
            fleet->raw_plord[3] != 3U ||
            fleet->raw_fleet_record[98] != 3U)
        trace->bad = 1;
    return !trace->fail_select;
}

static StarsI16 refresh_fleet(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->refresh_count;
    if (player != 4 || fleet->raw_plord[3] != 1U ||
            fleet->raw_fleet_record[98] != 1U)
        trace->bad = 1;
    return 1;
}

static StarsI16 clear_task(void *context,
    const StarsRototillTurnFleet *fleet, StarsI16 task)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->clear_count;
    if (task != 0 || fleet->id != 3)
        trace->bad = 1;
    return 1;
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
    raw[12U + 7U * 2U] = 1U;
    raw[98] = 3U;
    plord[3] = 3U;
    for (index = 4U; index < ORDER_BYTES; ++index)
        plord[index] = (StarsU8)(index + 0x30U);
    plord[4U + 18U + 4U] = 10U;
    plord[4U + 18U + 5U] = 0U;
    plord[4U + 36U + 4U] = 11U;
    plord[4U + 36U + 5U] = 0U;
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnFleet fleet;
    StarsRototillTurnPlanet planets[2];
    StarsU8 raw[124];
    StarsU8 plord[ORDER_BYTES];
    StarsU8 before[ORDER_BYTES];
    StarsU8 table[12U * 16U];
    StarsU8 table_before[sizeof table];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(planets, 0, sizeof planets);
    memset(&trace, 0, sizeof trace);
    seed(raw, plord);
    memcpy(before, plord, sizeof plord);
    memset(table, 0x23, sizeof table);
    memcpy(table_before, table, sizeof table);
    planets[0].id = 10;
    planets[0].owner = -1;
    planets[1].id = 11;
    planets[1].owner = -1;
    fleet.id = 3;
    fleet.owner = 4;
    fleet.raw_fleet_record = raw;
    fleet.raw_fleet_record_size = sizeof raw;
    fleet.raw_plord = plord;
    fleet.raw_plord_bytes = sizeof plord;
    ops.context = &trace;
    input.turn = 20;
    input.player_id = 4;
    input.planet_count = 0;
    input.planets = 0;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.raw_ai_planet_table = table;
    input.raw_ai_planet_record_count = 12;
    input.fleet_unowned_marker_bound = 1;
    input.ops = &ops;

    raw[6] = 0xffU;
    raw[7] = 0xffU;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(memcmp(plord, before, sizeof plord) == 0,
        &checks, &failures);
    check(table[10U * 16U + 1U] == (0x23U | 0x80U) &&
        table[11U * 16U + 1U] == 0x23U,
        &checks, &failures);
    table_before[10U * 16U + 1U] = (StarsU8)(0x23U | 0x80U);
    check(memcmp(table, table_before, sizeof table) == 0 &&
        raw[5] == 0x20U, &checks, &failures);

    seed(raw, plord);
    memcpy(before, plord, sizeof plord);
    memcpy(table, table_before, sizeof table);
    raw[6] = 10U;
    planets[0].owner = 4;
    input.planet_count = 2;
    input.planets = planets;
    table_before[10U * 16U + 9U] = 1U;
    table_before[11U * 16U + 9U] = 1U;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.clear_ai_current_task = clear_task;
    input.fleet_occupied_repair_bound = 1;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(raw[98] == 1U && raw[99] == 0U &&
        plord[3] == 1U, &checks, &failures);
    before[3] = 1U;
    check(memcmp(plord, before, sizeof plord) == 0,
        &checks, &failures);
    check(trace.select_count == 1 && trace.refresh_count == 1 &&
        trace.clear_count == 1 && !trace.bad,
        &checks, &failures);
    check(memcmp(table, table_before, sizeof table) == 0 &&
        raw[5] == 0x20U, &checks, &failures);

    seed(raw, plord);
    raw[6] = 10U;
    memcpy(before, plord, sizeof plord);
    memset(&trace, 0, sizeof trace);
    trace.fail_select = 1;
    check(!stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(memcmp(plord, before, sizeof plord) == 0 &&
        raw[98] == 3U && trace.select_count == 1 &&
        trace.refresh_count == 0 && trace.clear_count == 0,
        &checks, &failures);

#ifndef TARGET_NATIVE
    printf("ROTOTILL_MULTI_ORDER_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
