#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 (*ai)[16];
    StarsU8 live_queue[8];
    StarsU16 live_queue_bytes;
    char sequence[16];
    StarsI16 steps;
    StarsI16 mission_design;
    StarsI16 visits;
    StarsI16 ids[3];
    StarsI16 bad;
} Trace;

static StarsI16 select_planet(void *opaque,
    const StarsMacintiTurnPlanet *planet)
{
    Trace *trace;
    (void)planet;
    trace = (Trace *)opaque;
    if (trace->steps < 15)
        trace->sequence[trace->steps++] = 'S';
    return 1;
}

static StarsI16 init_production(void *opaque,
    const StarsMacintiTurnPlanet *planet)
{
    Trace *trace;
    (void)planet;
    trace = (Trace *)opaque;
    if (trace->steps < 15)
        trace->sequence[trace->steps++] = 'I';
    return 1;
}

static StarsI16 current_queue(void *opaque,
    const StarsMacintiTurnPlanet *planet, const StarsU8 **queue,
    StarsU16 *bytes)
{
    Trace *trace;
    (void)planet;
    trace = (Trace *)opaque;
    if (trace->steps < 15)
        trace->sequence[trace->steps++] = 'C';
    *queue = trace->live_queue;
    *bytes = trace->live_queue_bytes;
    return 1;
}

static StarsI16 finish_production(void *opaque,
    const StarsMacintiTurnPlanet *planet, StarsI16 mode)
{
    Trace *trace;
    (void)planet;
    (void)mode;
    trace = (Trace *)opaque;
    if (trace->steps < 15)
        trace->sequence[trace->steps++] = 'F';
    return 1;
}

static StarsI16 design_status(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace;
    (void)last;
    (void)budget;
    (void)mask;
    trace = (Trace *)opaque;
    *selected = first == 10 ? trace->mission_design : -1;
    return 1;
}

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes) {
        ++*failures;
        fprintf(stderr, "failed check %d\n", *checks);
    }
}

static void put_word(StarsU8 *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static StarsI16 pick_last(void *opaque, StarsI16 limit)
{
    (void)opaque;
    return (StarsI16)(limit - 1);
}

static StarsI16 visit(void *opaque, const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 turn)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->visits < 3)
        trace->ids[trace->visits] = planet->id;
    ++trace->visits;
    if ((turn != 50 && turn != 25) || trace->ai[9][13] != 1 ||
            trace->ai[1][13] != 1 || trace->ai[4][13] != 0 ||
            trace->ai[4][10] != 5 || trace->ai[4][9] != 1)
        trace->bad = 1;
    if (planet->id == 9 &&
            (!policy->owned || policy->hostile ||
             planet->owner != 2 || planet->mineral_concentration[0] != 80))
        trace->bad = 1;
    if (planet->id == 1 && (!policy->unowned || planet->owner != -1))
        trace->bad = 1;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnInput input;
    StarsMacintiAiTurnOps ops;
    StarsMacintiTurnPlanet planets[3];
    StarsMacintiTurnFleet fleet;
    StarsU8 raw[3][56];
    StarsU8 fleet_raw[124];
    StarsU8 plord[40];
    StarsU8 existing_queue[100];
    StarsU8 ai[12][16];
    StarsU8 pointers[8];
    const StarsU8 *source_planets[3];
    const StarsU8 *owned_planets[3];
    StarsU16 owned_count;
    StarsI16 selected[2];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(planets, 0, sizeof planets);
    memset(&fleet, 0, sizeof fleet);
    memset(raw, 0, sizeof raw);
    memset(fleet_raw, 0, sizeof fleet_raw);
    memset(plord, 0, sizeof plord);
    memset(existing_queue, 0, sizeof existing_queue);
    memset(ai, 0, sizeof ai);
    memset(&trace, 0, sizeof trace);
    trace.live_queue_bytes = 8;
    for (index = 0; index < 3; ++index) {
        planets[index].owner = 0;
        planets[index].raw_record = raw[index];
        planets[index].raw_record_size = 56;
    }
    planets[0].id = 9;
    planets[1].id = 4;
    planets[2].id = 1;
    put_word(raw[0], 0, 9);
    put_word(raw[0], 2, 2);
    put_word(raw[0], 4, 0x0200U);
    put_word(raw[0], 40, 200U);
    put_word(raw[0], 44, 2U);
    raw[0][9] = 80;
    put_word(raw[1], 0, 4);
    put_word(raw[1], 2, 3);
    put_word(raw[1], 4, 0x0200U);
    put_word(raw[1], 18, 750);
    put_word(raw[2], 0, 1);
    put_word(raw[2], 2, 0xffffU);
    selected[0] = 9;
    selected[1] = 1;
    for (index = 0; index < 12; ++index)
        ai[index][13] = 0xa5U;
    trace.ai = ai;
    ops.visit_planet_typed = visit;
    ops.select_planet = select_planet;
    ops.init_production = init_production;
    ops.current_production_queue = current_queue;
    ops.finish_production = finish_production;
    ops.context = &trace;
    input.turn = 50;
    input.player_id = 2;
    input.planet_count = 3;
    input.planets = planets;
    input.ops = &ops;
    input.planet_table_bound = 1;
    input.game_planet_max = 12;
    input.raw_ai_planet_table = &ai[0][0];
    input.raw_ai_planet_record_count = 12;
    input.selected_ai_planet_ids = selected;
    input.selected_ai_planet_count = 2;

    check(stars_do_macinti_ai_turn(&input), &checks, &failures);
    check(trace.visits == 2 && trace.ids[0] == 9 &&
        trace.ids[1] == 1 && !trace.bad &&
        input.typed_planets_visited == 2,
        &checks, &failures);
    check(ai[0][13] == 0 && ai[1][13] == 1 &&
        ai[4][13] == 0 && ai[9][13] == 1 &&
        ai[11][13] == 0 && ai[4][10] == 5 && ai[4][9] == 1,
        &checks, &failures);
    check(input.typed_production_candidates == 1,
        &checks, &failures);

    memset(ai, 0, sizeof ai);
    planets[1].raw_record_size = 55;
    check(!stars_do_macinti_ai_turn(&input) &&
        ai[9][13] == 0 && ai[4][9] == 0,
        &checks, &failures);
    planets[1].raw_record_size = 56;
    selected[1] = 8;
    check(!stars_do_macinti_ai_turn(&input) &&
        ai[9][13] == 0 && ai[4][9] == 0,
        &checks, &failures);
    selected[1] = 1;
    raw[0][0] = 10;
    check(!stars_do_macinti_ai_turn(&input) && ai[9][13] == 0,
        &checks, &failures);
    raw[0][0] = 9;
    input.raw_ai_planet_record_count = 11;
    check(!stars_do_macinti_ai_turn(&input) && ai[9][13] == 0,
        &checks, &failures);

    input.raw_ai_planet_record_count = 12;
    put_word(raw[0], 4, 0U);
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_production_candidates == 0, &checks, &failures);
    put_word(raw[0], 4, 0x0200U);
    put_word(raw[0], 42, 0xffffU);
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_production_candidates == 0, &checks, &failures);
    put_word(raw[0], 42, 0U);
    put_word(raw[0], 40, 199U);
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_production_candidates == 0, &checks, &failures);
    put_word(raw[0], 40, 200U);
    put_word(raw[0], 44, 0U);
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_production_candidates == 0, &checks, &failures);
    put_word(raw[0], 44, 1U);
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_production_candidates == 0, &checks, &failures);
    input.turn = 25;
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_production_candidates == 1, &checks, &failures);
    input.turn = 50;
    put_word(raw[0], 44, 2U);

    ops.check_design_status = design_status;
    trace.mission_design = 10;
    fleet.id = 77;
    fleet.raw_record = fleet_raw;
    fleet.raw_record_size = 124;
    fleet.raw_plord = plord;
    fleet.raw_plord_size = 40;
    input.fleet_count = 1;
    input.fleets = &fleet;
    put_word(fleet_raw, 0, 77);
    put_word(fleet_raw, 2, 2);
    put_word(fleet_raw, 6, 0xffffU);
    put_word(fleet_raw, 32, 1);
    put_word(fleet_raw, 90, 1);
    put_word(fleet_raw, 98, 2);
    plord[3] = 2;
    put_word(plord, 26, 4);
    put_word(plord, 28, 0x0100U);
    ai[4][14] = 0xa5U;
    check(stars_do_macinti_ai_turn(&input) && ai[4][14] == 1 &&
        ai[0][14] == 0, &checks, &failures);

    trace.mission_design = -1;
    ai[4][14] = 0xa5U;
    check(stars_do_macinti_ai_turn(&input) && ai[4][14] == 0,
        &checks, &failures);
    trace.mission_design = 10;
    put_word(fleet_raw, 32, 0);
    ai[4][14] = 0xa5U;
    check(stars_do_macinti_ai_turn(&input) && ai[4][14] == 0,
        &checks, &failures);
    put_word(fleet_raw, 32, 1);
    put_word(fleet_raw, 90, 0);
    ai[4][14] = 0xa5U;
    check(stars_do_macinti_ai_turn(&input) && ai[4][14] == 0,
        &checks, &failures);
    put_word(fleet_raw, 90, 1);
    put_word(plord, 28, 0x0200U);
    ai[4][14] = 0xa5U;
    check(stars_do_macinti_ai_turn(&input) && ai[4][14] == 0,
        &checks, &failures);
    put_word(plord, 28, 0x0100U);
    put_word(fleet_raw, 2, 3);
    ai[4][14] = 0xa5U;
    check(stars_do_macinti_ai_turn(&input) && ai[4][14] == 0,
        &checks, &failures);
    put_word(fleet_raw, 2, 2);
    fleet.raw_plord_size = 29;
    ai[4][14] = 0xa5U;
    check(!stars_do_macinti_ai_turn(&input) && ai[4][14] == 0xa5U,
        &checks, &failures);
    fleet.raw_plord_size = 40;
    put_word(plord, 26, 12);
    ai[4][14] = 0xa5U;
    check(!stars_do_macinti_ai_turn(&input) && ai[4][14] == 0xa5U,
        &checks, &failures);
    put_word(plord, 26, 4);
    fleet.raw_record_size = 123;
    check(!stars_do_macinti_ai_turn(&input) && ai[4][14] == 0xa5U,
        &checks, &failures);

    input.fleet_count = 0;
    input.fleets = 0;
    raw[0][52] = 1;
    existing_queue[3] = 24;
    planets[0].raw_existing_prod_queue = existing_queue;
    planets[0].raw_existing_prod_queue_size = sizeof existing_queue;
    trace.steps = 0;
    memset(trace.sequence, 0, sizeof trace.sequence);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.sequence, "S") == 0 &&
        input.typed_production_candidates == 0,
        &checks, &failures);

    raw[0][52] = 0;
    trace.live_queue[3] = 1;
    trace.live_queue[4] = 0;
    trace.live_queue[5] = 0x3cU;
    trace.live_queue[6] = 4;
    trace.live_queue[7] = 0;
    trace.steps = 0;
    memset(trace.sequence, 0, sizeof trace.sequence);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.sequence, "SICF") == 0 &&
        input.typed_production_candidates == 0 &&
        input.typed_production_finished == 1,
        &checks, &failures);

    trace.live_queue[5] = 0x40U;
    trace.steps = 0;
    memset(trace.sequence, 0, sizeof trace.sequence);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.sequence, "SICF") == 0 &&
        input.typed_production_candidates == 1 &&
        input.typed_production_finished == 1,
        &checks, &failures);

    trace.live_queue_bytes = 7;
    trace.steps = 0;
    memset(trace.sequence, 0, sizeof trace.sequence);
    check(!stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.sequence, "SIC") == 0,
        &checks, &failures);
    trace.live_queue_bytes = 8;

    ops.current_production_queue = 0;
    ai[9][13] = 0xa5U;
    check(!stars_do_macinti_ai_turn(&input) && ai[9][13] == 0xa5U,
        &checks, &failures);
    ops.current_production_queue = current_queue;

    raw[0][52] = 1;
    planets[0].raw_existing_prod_queue_size = 99;
    check(!stars_do_macinti_ai_turn(&input) && ai[9][13] == 0xa5U,
        &checks, &failures);

    raw[0][52] = 0;
    planets[0].raw_existing_prod_queue_size = sizeof existing_queue;
    ops.visit_planet_typed = 0;
    trace.live_queue[5] = 0x40U;
    trace.steps = 0;
    memset(trace.sequence, 0, sizeof trace.sequence);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.sequence, "SICF") == 0 &&
        input.typed_planets_visited == 2 &&
        input.typed_production_candidates == 1,
        &checks, &failures);
    trace.live_queue[5] = 0x3cU;
    trace.steps = 0;
    memset(trace.sequence, 0, sizeof trace.sequence);
    check(stars_do_macinti_ai_turn(&input) &&
        strcmp(trace.sequence, "SICF") == 0 &&
        input.typed_production_finished == 1,
        &checks, &failures);

    /* The original vrglpplAi is an ordered array of far pointers, not IDs.
     * Resolve complete offset:segment tokens before any AI-table mutation. */
    planets[0].guest_far_pointer[0] = 0x34U;
    planets[0].guest_far_pointer[1] = 0x12U;
    planets[0].guest_far_pointer[2] = 0x78U;
    planets[0].guest_far_pointer[3] = 0x56U;
    planets[1].guest_far_pointer[0] = 0x35U;
    planets[1].guest_far_pointer[1] = 0x12U;
    planets[1].guest_far_pointer[2] = 0x78U;
    planets[1].guest_far_pointer[3] = 0x56U;
    planets[2].guest_far_pointer[0] = 0x36U;
    planets[2].guest_far_pointer[1] = 0x12U;
    planets[2].guest_far_pointer[2] = 0x78U;
    planets[2].guest_far_pointer[3] = 0x56U;
    memcpy(pointers, planets[2].guest_far_pointer, 4U);
    memcpy(pointers + 4, planets[0].guest_far_pointer, 4U);
    input.selected_ai_planet_ids = 0;
    input.selected_ai_planet_pointers = pointers;
    input.selected_ai_planet_pointers_size = sizeof pointers;
    ops.visit_planet_typed = visit;
    trace.visits = 0;
    trace.bad = 0;
    check(stars_do_macinti_ai_turn(&input) && trace.visits == 2 &&
        trace.ids[0] == 1 && trace.ids[1] == 9 && !trace.bad &&
        ai[1][13] == 1 && ai[9][13] == 1,
        &checks, &failures);
    input.selected_ai_planet_pointers_size = 7;
    ai[9][13] = 0xa5U;
    check(!stars_do_macinti_ai_turn(&input) && ai[9][13] == 0xa5U,
        &checks, &failures);
    input.selected_ai_planet_pointers_size = sizeof pointers;
    pointers[0] = 0x37U;
    check(!stars_do_macinti_ai_turn(&input) && ai[9][13] == 0xa5U,
        &checks, &failures);
    pointers[0] = 0x36U;
    memcpy(planets[1].guest_far_pointer, planets[2].guest_far_pointer, 4U);
    check(!stars_do_macinti_ai_turn(&input) && ai[9][13] == 0xa5U,
        &checks, &failures);
    planets[1].guest_far_pointer[0] = 0x35U;
    ops.visit_planet_typed = 0;
    memset(pointers + 4, 0, 4U);
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_planets_visited == 1 && ai[1][13] == 1 &&
        ai[9][13] == 0,
        &checks, &failures);
    memset(pointers, 0, 4U);
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_planets_visited == 0 && ai[1][13] == 0 &&
        ai[9][13] == 0,
        &checks, &failures);

    /* Compose the source InitRandomPlanetList output with the bound Macinti
     * far-pointer input. The ordinary selected-ID fixture above includes an
     * unowned planet and is not an upstream list from this source service. */
    for (index = 0; index < 3; ++index)
        source_planets[index] = raw[index];
    stars_init_random_planet_list(source_planets, 3U, 2, 1U,
        owned_planets, &owned_count, pick_last, 0);
    check(owned_count == 1U && owned_planets[0] == raw[0],
        &checks, &failures);
    memcpy(pointers, planets[0].guest_far_pointer, 4U);
    memset(pointers + 4U, 0, 4U);
    input.selected_ai_planet_count = (StarsI16)owned_count;
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_planets_visited == 1 && ai[9][13] == 1 &&
        ai[1][13] == 0, &checks, &failures);

    put_word(raw[2], 2, 2U);
    stars_init_random_planet_list(source_planets, 3U, 2, 0U,
        owned_planets, &owned_count, pick_last, 0);
    check(owned_count == 2U && owned_planets[0] == raw[2] &&
        owned_planets[1] == raw[0], &checks, &failures);
    memcpy(pointers, planets[2].guest_far_pointer, 4U);
    memcpy(pointers + 4U, planets[0].guest_far_pointer, 4U);
    input.selected_ai_planet_count = (StarsI16)owned_count;
    check(stars_do_macinti_ai_turn(&input) &&
        input.typed_planets_visited == 2 && ai[1][13] == 1 &&
        ai[9][13] == 1, &checks, &failures);

    printf("MACINTI_PLANET_TABLE_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
