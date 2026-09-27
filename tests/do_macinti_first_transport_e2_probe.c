#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 *raw;
    StarsU8 *plord;
    StarsU8 selected_raw[124];
    StarsU8 selected_plord[40];
    StarsI16 attack;
    StarsI16 transport;
    StarsI16 short_selected;
    char events[12];
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

static void setup(Trace *trace)
{
    memset(trace->raw, 0, 124);
    memset(trace->plord, 0, 40);
    word(trace->raw, 0, 9);
    word(trace->raw, 2, 2);
    word(trace->raw, 6, 1);
    word(trace->raw, 98, 2);
    trace->plord[3] = 2;
    word(trace->plord, 10, 0xa3f0U);
    word(trace->plord, 26, 2);
    word(trace->plord, 28, 0x0100U);
    trace->attack = 0;
    trace->transport = 1;
    trace->short_selected = 0;
    trace->count = 0;
    trace->bad = 0;
    memset(trace->events, 0, sizeof trace->events);
}

static StarsI16 attack(void *opaque, const StarsMacintiTurnFleet *fleet,
    StarsI16 *answer)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'A';
    if (fleet->id != 9 || fleet->cord != 2)
        trace->bad = 1;
    *answer = trace->attack;
    return 1;
}

static StarsI16 transport(void *opaque, const StarsMacintiTurnFleet *fleet,
    StarsI16 *answer)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'T';
    if (fleet->id != 9 || fleet->cord != 2)
        trace->bad = 1;
    *answer = trace->transport;
    return 1;
}

static StarsI16 select_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet, StarsMacintiSelectedFleet *selected)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'S';
    if (fleet->id != 9 || fleet->cord != 2)
        trace->bad = 1;
    memcpy(trace->selected_raw, trace->raw, 124);
    memcpy(trace->selected_plord, trace->plord, 40);
    selected->fleet_record = trace->selected_raw;
    selected->fleet_record_size = 124;
    selected->plord = trace->selected_plord;
    selected->plord_size = trace->short_selected ? 21U : 40U;
    return 1;
}

static StarsI16 refresh_fleet(void *opaque,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiSelectedFleet *selected)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'R';
    if (fleet->id != 9 || selected->fleet_record[98] != 1U ||
            selected->plord[3] != 1U)
        trace->bad = 1;
    memcpy(trace->raw, selected->fleet_record, 124);
    memcpy(trace->plord, selected->plord, 40);
    return 1;
}

static StarsI16 clear_task(void *opaque, const StarsMacintiTurnFleet *fleet,
    StarsI16 mode)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->events[trace->count++] = 'C';
    if (fleet->id != 9 || fleet->cord != 1 || mode != 0 ||
            fleet->raw_plord[3] != 1U)
        trace->bad = 1;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnInput input;
    StarsMacintiAiTurnOps ops;
    StarsMacintiTurnFleet fleet;
    StarsMacintiTurnPlanet planets[2];
    StarsU8 raw[124];
    StarsU8 plord[40];
    StarsU8 planet_raw[2][56];
    StarsU8 ai[4][16];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&fleet, 0, sizeof fleet);
    memset(planets, 0, sizeof planets);
    memset(planet_raw, 0, sizeof planet_raw);
    memset(ai, 0, sizeof ai);
    memset(&trace, 0, sizeof trace);
    trace.raw = raw;
    trace.plord = plord;
    word(planet_raw[0], 0, 1);
    word(planet_raw[0], 2, 2);
    word(planet_raw[1], 0, 2);
    word(planet_raw[1], 2, 3);
    planets[0].id = 1;
    planets[0].raw_record = planet_raw[0];
    planets[0].raw_record_size = 56;
    planets[1].id = 2;
    planets[1].raw_record = planet_raw[1];
    planets[1].raw_record_size = 56;
    fleet.id = 9;
    fleet.raw_record = raw;
    fleet.raw_record_size = 124;
    fleet.raw_plord = plord;
    fleet.raw_plord_size = 40;
    ops.is_ai_attack = attack;
    ops.is_ai_transport = transport;
    ops.select_fleet = select_fleet;
    ops.refresh_fleet = refresh_fleet;
    ops.clear_current_task = clear_task;
    ops.context = &trace;
    input.turn = 50;
    input.player_id = 2;
    input.planet_count = 2;
    input.planets = planets;
    input.fleet_count = 1;
    input.fleets = &fleet;
    input.planet_table_bound = 1;
    input.game_planet_max = 4;
    input.raw_ai_planet_table = &ai[0][0];
    input.raw_ai_planet_record_count = 4;
    input.ops = &ops;

    setup(&trace);
    check(stars_do_macinti_ai_turn(&input) && !trace.bad &&
        strcmp(trace.events, "ATSRC") == 0 &&
        raw[98] == 1U && plord[3] == 1U &&
        plord[10] == 0xf0U && plord[26] == 2U,
        &checks, &failures);
    setup(&trace);
    word(plord, 10, 0xa3f2U);
    check(stars_do_macinti_ai_turn(&input) && !trace.bad &&
        strcmp(trace.events, "AT") == 0 &&
        raw[98] == 2U && plord[3] == 2U,
        &checks, &failures);
    setup(&trace);
    word(plord, 28, 0x0200U);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "AT") == 0 && raw[98] == 2U,
        &checks, &failures);
    setup(&trace);
    word(raw, 88, 1);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "AT") == 0 && raw[98] == 2U,
        &checks, &failures);
    setup(&trace);
    word(planet_raw[1], 2, 2);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "AT") == 0 && raw[98] == 2U,
        &checks, &failures);
    word(planet_raw[1], 2, 3);
    setup(&trace);
    trace.attack = 1;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "A") == 0 && raw[98] == 2U,
        &checks, &failures);
    setup(&trace);
    trace.transport = 0;
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "AT") == 0 && raw[98] == 2U,
        &checks, &failures);
    setup(&trace);
    word(raw, 12, 1);
    check(stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        raw[98] == 2U, &checks, &failures);
    setup(&trace);
    ops.clear_current_task = 0;
    check(!stars_do_macinti_ai_turn(&input) && trace.count == 0 &&
        raw[98] == 2U, &checks, &failures);
    ops.clear_current_task = clear_task;
    setup(&trace);
    trace.short_selected = 1;
    check(!stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.events, "ATS") == 0 &&
        raw[98] == 2U && plord[3] == 2U,
        &checks, &failures);

    printf("MACINTI_FIRST_TRANSPORT_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
