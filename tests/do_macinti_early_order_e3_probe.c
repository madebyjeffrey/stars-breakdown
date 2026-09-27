#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 *raw;
    StarsU8 *plord;
    StarsU8 selected_raw[124];
    StarsU8 selected_plord[22];
    StarsI16 short_selected;
    StarsU8 expected_kind;
    char events[4];
    StarsI16 count;
    StarsI16 bad;
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

static void setup(Trace *trace, StarsU16 owner, StarsU16 ship_slot)
{
    memset(trace->raw, 0, 124);
    memset(trace->plord, 0, 22);
    word(trace->raw, 0, 9);
    word(trace->raw, 2, owner);
    word(trace->raw, 12 + ship_slot * 2, 1);
    word(trace->raw, 98, 1);
    trace->plord[3] = 1U;
    word(trace->plord, 10, 0xa3f2U);
    trace->count = 0;
    trace->bad = 0;
    trace->short_selected = 0;
    trace->expected_kind = 5U;
    memset(trace->events, 0, sizeof trace->events);
}

static StarsI16 select_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet, StarsMacintiSelectedFleet *selected)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'S';
    if (fleet->id != 9 || fleet->raw_record[98] != 1U)
        trace->bad = 1;
    memcpy(trace->selected_raw, trace->raw, 124);
    memcpy(trace->selected_plord, trace->plord, 22);
    selected->fleet_record = trace->selected_raw;
    selected->fleet_record_size = 124U;
    selected->plord = trace->selected_plord;
    selected->plord_size = trace->short_selected ? 21U : 22U;
    return 1;
}

static StarsI16 refresh_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiSelectedFleet *selected)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'R';
    if (fleet->id != 9 ||
            selected->plord[10] != (StarsU8)(0xf0U | trace->expected_kind) ||
            selected->plord[11] != 0xa3U)
        trace->bad = 1;
    memcpy(trace->raw, selected->fleet_record, 124);
    memcpy(trace->plord, selected->plord, 22);
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnInput input;
    StarsMacintiAiTurnOps ops;
    StarsMacintiTurnFleet fleet;
    StarsU8 raw[124];
    StarsU8 plord[22];
    StarsU8 ai[4][16];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(ai, 0, sizeof ai);
    memset(&trace, 0, sizeof trace);
    trace.raw = raw;
    trace.plord = plord;
    fleet.id = 9;
    fleet.raw_record = raw;
    fleet.raw_record_size = 124;
    fleet.raw_plord = plord;
    fleet.raw_plord_size = 22;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.context = &trace;
    input.turn = 10;
    input.player_id = 2;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.planet_table_bound = 1;
    input.game_planet_max = 4;
    input.raw_ai_planet_table = &ai[0][0];
    input.raw_ai_planet_record_count = 4;
    input.ops = &ops;

    setup(&trace, 2, 0);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "SR") == 0 && !trace.bad &&
        plord[10] == 0xf5U && plord[11] == 0xa3U &&
        input.typed_fleets_visited == 0,
        &checks, &failures);
    setup(&trace, 2, 2);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "SR") == 0 && !trace.bad &&
        plord[10] == 0xf5U, &checks, &failures);
    setup(&trace, 2, 0);
    input.turn = 11;
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        plord[10] == 0xf2U, &checks, &failures);
    input.turn = 10;
    setup(&trace, 3, 0);
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        plord[10] == 0xf2U, &checks, &failures);
    setup(&trace, 2, 0);
    ops.select_fleet = 0;
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        plord[10] == 0xf2U, &checks, &failures);
    ops.select_fleet = select_fleet;
    fleet.raw_plord_size = 21;
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    fleet.raw_plord_size = 22;
    trace.short_selected = 1;
    check(!stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "S") == 0 && plord[10] == 0xf2U,
        &checks, &failures);

    setup(&trace, 2, 14);
    trace.expected_kind = 3U;
    input.turn = 40;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "SR") == 0 && !trace.bad &&
        plord[10] == 0xf3U && plord[11] == 0xa3U &&
        input.typed_fleets_visited == 0,
        &checks, &failures);
    setup(&trace, 2, 14);
    trace.expected_kind = 3U;
    input.turn = 41;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "SR") == 0 && !trace.bad &&
        plord[10] == 0xf3U, &checks, &failures);
    setup(&trace, 2, 14);
    word(raw, 12, 1);
    input.turn = 41;
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        plord[10] == 0xf2U, &checks, &failures);
    setup(&trace, 2, 14);
    word(plord, 10, 0xa3f3U);
    input.turn = 40;
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        plord[10] == 0xf3U, &checks, &failures);
    setup(&trace, 2, 14);
    word(raw, 12, 1);
    trace.expected_kind = 3U;
    input.turn = 10;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "SR") == 0 && !trace.bad &&
        plord[10] == 0xf3U, &checks, &failures);
    setup(&trace, 2, 14);
    input.turn = 40;
    ops.select_fleet = 0;
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        plord[10] == 0xf2U, &checks, &failures);
    ops.select_fleet = select_fleet;
    setup(&trace, 2, 14);
    trace.short_selected = 1;
    check(!stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "S") == 0 && plord[10] == 0xf2U,
        &checks, &failures);

    printf("MACINTI_EARLY_ORDER_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
