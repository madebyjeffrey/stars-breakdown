#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsRototillAiTurnInput *input;
    StarsI16 visits;
    StarsI16 census_ready;
    StarsI16 init_ids[4];
    StarsI16 init_count;
    StarsI16 finish_ids[4];
    StarsI16 finish_flags[4];
    StarsI16 finish_count;
    StarsI16 queue_ids[8];
    StarsI16 queue_designs[8];
    StarsI16 queue_count;
    StarsI16 bad_args;
    StarsI16 raw_view_ok;
} Trace;

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

static StarsI16 visit(void *opaque,
    const StarsRototillTurnPlanet *planet, StarsU16 score, StarsI16 player)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->visits == 0)
        trace->census_ready = trace->input->unowned_viable_count == 1 &&
            planet->id == 0 && score == 127;
    ++trace->visits;
    if (player != 2)
        trace->bad_args = 1;
    if (planet->id == 5)
        trace->raw_view_ok = planet->owner == 2 &&
            planet->planet_class == 3 &&
            planet->visibility_flags == 0x0203U &&
            planet->mineral_concentration[0] == 80 &&
            planet->mineral_reserve == 1000U &&
            planet->mineral_rate_high == 0;
    return 1;
}

static StarsI16 init(void *opaque, const StarsRototillTurnPlanet *planet)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->init_count < 4)
        trace->init_ids[trace->init_count] = planet->id;
    ++trace->init_count;
    return 1;
}

static StarsI16 finish(void *opaque,
    const StarsRototillTurnPlanet *planet, StarsI16 changed)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->finish_count < 4) {
        trace->finish_ids[trace->finish_count] = planet->id;
        trace->finish_flags[trace->finish_count] = changed;
    }
    ++trace->finish_count;
    return 1;
}

static StarsI16 queue(void *opaque,
    const StarsRototillTurnPlanet *planet, StarsI16 design,
    StarsI16 count, StarsI16 kind, StarsI16 flag)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->queue_count < 8) {
        trace->queue_ids[trace->queue_count] = planet->id;
        trace->queue_designs[trace->queue_count] = design;
    }
    ++trace->queue_count;
    if (count != 1 || kind != 2 || flag != 1)
        trace->bad_args = 1;
    return 1;
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnPlanet planets[6];
    StarsU8 ai[6][16];
    StarsU8 queue_bytes[3][8];
    StarsU8 packed[56];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(planets, 0, sizeof planets);
    memset(ai, 0, sizeof ai);
    memset(queue_bytes, 0, sizeof queue_bytes);
    memset(&trace, 0, sizeof trace);
    for (index = 0; index < 6; ++index) {
        planets[index].id = index;
        planets[index].raw_ai_planet = ai[index];
        planets[index].raw_ai_planet_size = 16;
    }
    planets[0].owner = -1;
    planets[0].planet_class = 3;
    planets[0].optimal_value = 1;
    planets[0].mineral_concentration[0] = 80;
    planets[0].mineral_concentration[1] = 90;
    planets[0].mineral_concentration[2] = 100;
    for (index = 1; index <= 3; ++index) {
        planets[index].owner = 2;
        planets[index].visibility_flags = 0x0200U;
        planets[index].mineral_rate_high = 0;
        planets[index].mineral_reserve = 1000;
        planets[index].raw_prod_queue = queue_bytes[index - 1];
        planets[index].raw_prod_queue_size = 8;
    }
    queue_bytes[2][3] = 1;
    queue_bytes[2][5] = 0x44U;
    queue_bytes[2][6] = 0x04U;
    planets[4].owner = 2;
    planets[4].desirability = -1;
    planets[4].visibility_flags = 0x0200U;
    planets[4].mineral_rate_high = 1;
    planets[5].owner = 3;
    planets[5].optimal_value = 2;
    planets[5].visibility_flags = 0x0200U;
    trace.input = &input;
    ops.visit_planet = visit;
    ops.init_production = init;
    ops.finish_production = finish;
    ops.add_raw_queue_item = queue;
    ops.context = &trace;
    input.turn = 20;
    input.player_id = 2;
    input.planet_count = 6;
    input.planets = planets;
    input.ops = &ops;
    input.production_inputs_bound = 1;
    input.design_one_existing = 0;

    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.visits == 6 && trace.census_ready && !trace.bad_args,
        &checks, &failures);
    check(input.unowned_viable_count == 1 &&
        input.owned_bad_count == 1 &&
        input.mineral_score == 127, &checks, &failures);
    check(ai[0][9] == 1 && ai[0][1] == 127 &&
        ai[4][2] == 1 && ai[5][3] == 1 && ai[5][10] == 2,
        &checks, &failures);
    check(trace.init_count == 3 && trace.init_ids[0] == 1 &&
        trace.init_ids[1] == 2 && trace.init_ids[2] == 3,
        &checks, &failures);
    check(trace.queue_count == 1 && trace.queue_ids[0] == 1 &&
        trace.queue_designs[0] == 1, &checks, &failures);
    check(trace.finish_count == 3 && trace.finish_ids[0] == 1 &&
        trace.finish_flags[0] == 1 && trace.finish_ids[1] == 2 &&
        trace.finish_flags[1] == 0 && trace.finish_ids[2] == 3 &&
        trace.finish_flags[2] == 0, &checks, &failures);
    check(input.production_items_added == 1 &&
        input.design_one_queued == 1 &&
        input.queue_bypass_count == 1, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.input = &input;
    input.turn = 0;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.queue_count == 4 && trace.queue_ids[0] == 1 &&
        trace.queue_ids[1] == 1 && trace.queue_ids[2] == 2 &&
        trace.queue_ids[3] == 2, &checks, &failures);
    check(trace.queue_designs[0] == 0 && trace.queue_designs[3] == 0 &&
        trace.finish_flags[0] == 0 && trace.finish_flags[1] == 0 &&
        input.production_items_added == 4 &&
        input.design_one_queued == 0, &checks, &failures);

    memset(&trace, 0, sizeof trace);
    trace.input = &input;
    input.turn = 20;
    input.design_one_existing = 1;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.queue_count == 0 && input.design_one_queued == 0 &&
        trace.finish_count == 3, &checks, &failures);

    memset(ai, 0, sizeof ai);
    planets[3].raw_prod_queue_size = 7;
    check(!stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(ai[0][9] == 0 && trace.queue_count == 0,
        &checks, &failures);

    memset(&trace, 0, sizeof trace);
    memset(ai, 0, sizeof ai);
    memset(packed, 0, sizeof packed);
    trace.input = &input;
    packed[0] = 5;
    packed[2] = 2;
    packed[4] = 3;
    packed[5] = 2;
    packed[9] = 80;
    packed[40] = 0xe8U;
    packed[41] = 3;
    planets[5].raw_ai_planet = 0;
    planets[5].raw_planet_record = packed;
    planets[5].raw_planet_record_size = 56;
    planets[5].raw_prod_queue = queue_bytes[0];
    planets[5].raw_prod_queue_size = 8;
    input.planet_count = 1;
    input.planets = &planets[5];
    input.turn = 0;
    input.raw_ai_planet_table = &ai[0][0];
    input.raw_ai_planet_record_count = 6;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.raw_view_ok && trace.init_count == 1 &&
        trace.queue_count == 2 && trace.finish_count == 1 &&
        ai[5][9] == 1 && ai[0][9] == 0, &checks, &failures);
    memset(ai, 0, sizeof ai);
    planets[5].raw_planet_record_size = 55;
    check(!stars_do_rototill_ai_turn(&input) && ai[5][9] == 0,
        &checks, &failures);
    planets[5].raw_planet_record_size = 56;
    packed[0] = 4;
    check(!stars_do_rototill_ai_turn(&input) && ai[5][9] == 0,
        &checks, &failures);
    packed[0] = 5;
    input.raw_ai_planet_record_count = 5;
    check(!stars_do_rototill_ai_turn(&input) && ai[5][9] == 0,
        &checks, &failures);

    printf("ROTOTILL_PRODUCTION_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
