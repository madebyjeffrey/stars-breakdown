#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 *raw;
    StarsU8 *plord;
    StarsU8 selected_raw[124];
    StarsU8 selected_plord[40];
    StarsI16 short_selected;
    char events[8];
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

static void setup(Trace *trace, StarsU16 x, StarsU16 y,
    StarsU16 target_x, StarsU16 target_y, StarsU16 kind)
{
    memset(trace->raw, 0, 124);
    memset(trace->plord, 0, 40);
    word(trace->raw, 0, 7);
    word(trace->raw, 2, 3);
    word(trace->raw, 8, x);
    word(trace->raw, 10, y);
    word(trace->raw, 98, 2);
    trace->plord[3] = 2;
    trace->plord[12] = 0xa5U;
    word(trace->plord, 22, target_x);
    word(trace->plord, 24, target_y);
    word(trace->plord, 28, (StarsU16)(kind << 8));
    trace->count = 0;
    trace->bad = 0;
    memset(trace->events, 0, sizeof trace->events);
}

static StarsI16 select_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet, StarsMacintiSelectedFleet *selected)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'S';
    if (fleet->id != 7 || fleet->raw_record[98] != 2U)
        trace->bad = 1;
    memcpy(trace->selected_raw, trace->raw, 124);
    memcpy(trace->selected_plord, trace->plord, 40);
    selected->fleet_record = trace->selected_raw;
    selected->fleet_record_size = trace->short_selected ? 123U : 124U;
    selected->plord = trace->selected_plord;
    selected->plord_size = 40U;
    return 1;
}

static StarsI16 refresh_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiSelectedFleet *selected)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'R';
    if (fleet->id != 7 || selected->fleet_record[98] != 1U ||
            selected->fleet_record[99] != 0U || selected->plord[3] != 1U)
        trace->bad = 1;
    memcpy(trace->raw, selected->fleet_record, 124);
    memcpy(trace->plord, selected->plord, 40);
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnInput input;
    StarsMacintiAiTurnOps ops;
    StarsMacintiTurnFleet fleet;
    StarsU8 raw[124];
    StarsU8 plord[40];
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
    fleet.id = 7;
    fleet.owner = 2;
    fleet.raw_record = raw;
    fleet.raw_record_size = 124;
    fleet.raw_plord = plord;
    fleet.raw_plord_size = 40;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.context = &trace;
    input.turn = 50;
    input.player_id = 2;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.planet_table_bound = 1;
    input.game_planet_max = 4;
    input.raw_ai_planet_table = &ai[0][0];
    input.raw_ai_planet_record_count = 4;
    input.ops = &ops;

    setup(&trace, 0, 0, 201, 0, 8);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "SR") == 0 && !trace.bad &&
        raw[98] == 1U && plord[3] == 1U && plord[12] == 0xa5U &&
        input.typed_fleets_visited == 0,
        &checks, &failures);
    setup(&trace, 0, 0, 200, 0, 8);
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        raw[98] == 2U && plord[3] == 2U, &checks, &failures);
    setup(&trace, 32767U, 0, 0x8000U, 0, 8);
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    setup(&trace, 0, 0, 0x8000U, 0x8000U, 8);
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    setup(&trace, 0, 0, 201, 0, 7);
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    setup(&trace, 0, 0, 201, 0, 8);
    ops.select_fleet = 0;
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        raw[98] == 2U && plord[3] == 2U, &checks, &failures);
    ops.select_fleet = select_fleet;
    fleet.raw_plord_size = 39;
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0,
        &checks, &failures);
    fleet.raw_plord_size = 40;
    trace.short_selected = 1;
    check(!stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "S") == 0 &&
        raw[98] == 2U && plord[3] == 2U,
        &checks, &failures);

    printf("MACINTI_FAR_WAYPOINT_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
