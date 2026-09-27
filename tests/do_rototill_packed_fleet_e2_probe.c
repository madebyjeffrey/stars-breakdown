#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 repairs;
    StarsI16 typed_visits;
    StarsI16 legacy_visits;
    StarsI16 callback_ok;
    char sequence[4];
    StarsI16 sequence_count;
    StarsI16 reject_select;
} Trace;

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

static StarsI16 repair(void *opaque, const StarsRototillTurnFleet *fleet,
    StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->repairs;
    if (player_id != 4 || fleet->owner != 4 ||
            fleet->raw_fleet_record[5] != 0x20U)
        trace->callback_ok = 0;
    return 1;
}

static StarsI16 typed_visit(void *opaque,
    const StarsRototillTurnFleet *fleet,
    const StarsRototillFleetPolicy *policy, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->typed_visits;
    if (player_id != 4 || fleet->id != 0x1234 ||
            fleet->planet_id != 9 || fleet->cord != 0 ||
            fleet->ship_counts[0] != 300 ||
            policy->colonizer_ships != 300 ||
            policy->transport_ships != 2 ||
            policy->armed_ships != 3 ||
            policy->total_ships != 305 ||
            !policy->needs_order_repair)
        trace->callback_ok = 0;
    return 1;
}

static StarsI16 legacy_visit(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)opaque;
    ++trace->legacy_visits;
    if (player_id != 4 || fleet->owner != 4 ||
            fleet->ship_counts[0] != 300)
        trace->callback_ok = 0;
    return 1;
}

static StarsI16 select_fleet(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->sequence[trace->sequence_count++] = 'S';
    if (player_id != 4 || fleet->raw_fleet_record[5] != 0x20U ||
            fleet->raw_fleet_record[98] != 2 ||
            fleet->raw_plord[3] != 2)
        trace->callback_ok = 0;
    return !trace->reject_select;
}

static StarsI16 refresh_fleet(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player_id)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->sequence[trace->sequence_count++] = 'R';
    if (player_id != 4 || fleet->cord != 1 ||
            fleet->raw_fleet_record[98] != 1 ||
            fleet->raw_plord[3] != 1)
        trace->callback_ok = 0;
    return 1;
}

static StarsI16 clear_task(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 task)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->sequence[trace->sequence_count++] = 'C';
    if (fleet->id != 0x1234 || task != 0 ||
            fleet->raw_fleet_record[98] != 1)
        trace->callback_ok = 0;
    return 1;
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnFleet fleets[2];
    StarsRototillTurnPlanet planets[2];
    StarsU8 raw[2][124];
    StarsU8 plord[40];
    StarsU8 ai[12][16];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(fleets, 0, sizeof fleets);
    memset(raw, 0, sizeof raw);
    memset(planets, 0, sizeof planets);
    memset(plord, 0, sizeof plord);
    memset(ai, 0, sizeof ai);
    memset(&trace, 0, sizeof trace);
    trace.callback_ok = 1;
    raw[0][0] = 0x34U;
    raw[0][1] = 0x12U;
    raw[0][2] = 4;
    raw[0][5] = 0xa0U;
    raw[0][6] = 9;
    raw[0][12] = 0x2cU;
    raw[0][13] = 1;
    raw[0][14] = 2;
    raw[0][16] = 3;
    raw[1][0] = 7;
    raw[1][2] = 3;
    raw[1][5] = 0x80U;
    fleets[0].id = 0x1234;
    fleets[0].owner = 7;
    fleets[0].raw_fleet_record = raw[0];
    fleets[0].raw_fleet_record_size = 124;
    fleets[1].id = 7;
    fleets[1].owner = 4;
    fleets[1].raw_fleet_record = raw[1];
    fleets[1].raw_fleet_record_size = 124;
    ops.repair_order = repair;
    ops.visit_fleet_typed = typed_visit;
    ops.visit_fleet = legacy_visit;
    ops.context = &trace;
    input.turn = 20;
    input.player_id = 4;
    input.fleet_count = 2;
    input.fleets = fleets;
    input.ops = &ops;

    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(raw[0][5] == 0x20U && raw[1][5] == 0x80U,
        &checks, &failures);
    check(trace.repairs == 1 && trace.typed_visits == 1 &&
        trace.legacy_visits == 1 && trace.callback_ok,
        &checks, &failures);
    raw[0][5] = 0xa0U;
    fleets[1].raw_fleet_record_size = 123;
    check(!stars_do_rototill_ai_turn(&input) && raw[0][5] == 0xa0U,
        &checks, &failures);
    fleets[1].raw_fleet_record_size = 124;
    raw[1][0] = 8;
    check(!stars_do_rototill_ai_turn(&input) && raw[0][5] == 0xa0U,
        &checks, &failures);

    raw[1][0] = 7;
    raw[0][5] = 0xa0U;
    raw[0][6] = 9;
    raw[0][12 + 7 * 2] = 1;
    raw[0][98] = 1;
    planets[0].id = 9;
    planets[0].owner = -1;
    planets[1].id = 10;
    planets[1].owner = -1;
    input.planet_count = 2;
    input.planets = planets;
    input.raw_ai_planet_table = &ai[0][0];
    input.raw_ai_planet_record_count = 12;
    input.fleet_unowned_marker_bound = 1;
    ops.repair_order = 0;
    ops.visit_fleet_typed = 0;
    ops.visit_fleet = 0;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(ai[9][1] == 0x80U && ai[10][1] == 0 &&
        raw[0][5] == 0x20U, &checks, &failures);

    memset(ai, 0, sizeof ai);
    raw[0][6] = 0xffU;
    raw[0][7] = 0xffU;
    raw[0][98] = 2;
    plord[3] = 2;
    plord[26] = 10;
    fleets[0].raw_plord = plord;
    fleets[0].raw_plord_bytes = 40;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(ai[10][1] == 0x80U && ai[9][1] == 0,
        &checks, &failures);

    memset(ai, 0, sizeof ai);
    raw[0][5] = 0xa0U;
    fleets[0].raw_plord_bytes = 39;
    check(!stars_do_rototill_ai_turn(&input) &&
        raw[0][5] == 0xa0U && ai[10][9] == 0,
        &checks, &failures);
    fleets[0].raw_plord_bytes = 40;
    plord[26] = 11;
    input.raw_ai_planet_record_count = 11;
    check(!stars_do_rototill_ai_turn(&input) &&
        raw[0][5] == 0xa0U && ai[9][9] == 0,
        &checks, &failures);

    memset(ai, 0, sizeof ai);
    input.raw_ai_planet_record_count = 12;
    planets[0].owner = 4;
    raw[0][6] = 9;
    raw[0][7] = 0;
    raw[0][98] = 1;
    check(stars_do_rototill_ai_turn(&input) && ai[9][1] == 0,
        &checks, &failures);

    raw[0][5] = 0xa0U;
    raw[0][98] = 2;
    plord[3] = 2;
    memset(&trace, 0, sizeof trace);
    trace.callback_ok = 1;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.clear_ai_current_task = clear_task;
    input.fleet_occupied_repair_bound = 1;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(raw[0][98] == 1 && plord[3] == 1 &&
        trace.sequence_count == 3 &&
        trace.sequence[0] == 'S' && trace.sequence[1] == 'R' &&
        trace.sequence[2] == 'C' && trace.callback_ok,
        &checks, &failures);

    raw[0][5] = 0xa0U;
    raw[0][98] = 2;
    plord[3] = 2;
    ops.clear_ai_current_task = 0;
    check(!stars_do_rototill_ai_turn(&input) &&
        raw[0][5] == 0xa0U && raw[0][98] == 2 && plord[3] == 2,
        &checks, &failures);
    ops.clear_ai_current_task = clear_task;
    fleets[0].raw_plord = 0;
    check(!stars_do_rototill_ai_turn(&input) &&
        raw[0][5] == 0xa0U && raw[0][98] == 2,
        &checks, &failures);
    fleets[0].raw_plord = plord;
    memset(&trace, 0, sizeof trace);
    trace.callback_ok = 1;
    trace.reject_select = 1;
    check(!stars_do_rototill_ai_turn(&input) &&
        trace.sequence_count == 1 && trace.sequence[0] == 'S' &&
        raw[0][98] == 2 && plord[3] == 2,
        &checks, &failures);
    printf("ROTOTILL_PACKED_FLEET_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
