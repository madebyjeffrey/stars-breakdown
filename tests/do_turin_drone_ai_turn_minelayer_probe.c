#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct MineTrace {
    StarsI16 opt_calls;
    StarsI16 prepared_after_opt;
    StarsI16 queue_calls;
    StarsI16 designs[8];
    StarsI16 quantities[8];
    StarsI16 modes[8];
    StarsI16 flags[8];
    StarsI16 reject_queue_at;
    StarsI16 random_calls;
    StarsI16 bounds[8];
    StarsI16 results[8];
    StarsI16 finishes;
    StarsI16 changed;
} MineTrace;

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
#ifndef TURIN_MINE_NATIVE
        fprintf(stderr, "Turin mine check failed at line %d\n", line);
#else
        (void)line;
#endif
    }
}

#define check(condition) check_at((condition), __LINE__)

static StarsI16 opt(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 player)
{
    MineTrace *trace;
    (void)player;
    trace = (MineTrace *)opaque;
    ++trace->opt_calls;
    return planet->desirability;
}

static StarsI16 prepare(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 turn)
{
    MineTrace *trace;
    (void)planet;
    (void)turn;
    trace = (MineTrace *)opaque;
    trace->prepared_after_opt = trace->opt_calls;
    return 1;
}

static StarsI16 random_value(void *opaque, StarsI16 bound)
{
    MineTrace *trace;
    StarsI16 index;
    trace = (MineTrace *)opaque;
    index = trace->random_calls++;
    if (index < 8)
        trace->bounds[index] = bound;
    return index < 8 ? trace->results[index] : 1;
}

static StarsI16 queue_full(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 design,
    StarsI16 quantity, StarsI16 mode, StarsI16 flag)
{
    MineTrace *trace;
    StarsI16 index;
    (void)planet;
    trace = (MineTrace *)opaque;
    index = trace->queue_calls++;
    if (index < 8) {
        trace->designs[index] = design;
        trace->quantities[index] = quantity;
        trace->modes[index] = mode;
        trace->flags[index] = flag;
    }
    return trace->queue_calls != trace->reject_queue_at;
}

static StarsI16 finish(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 changed)
{
    MineTrace *trace;
    (void)planet;
    trace = (MineTrace *)opaque;
    ++trace->finishes;
    trace->changed = changed;
    return 1;
}

static void seed(StarsTurinDroneAiTurnInput *input,
    StarsTurinDroneAiTurnOps *ops, StarsTurinTurnPlanet planets[2],
    StarsTurinTurnFleet fleets[3], MineTrace *trace)
{
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    memset(planets, 0, 2 * sizeof *planets);
    memset(fleets, 0, 3 * sizeof *fleets);
    memset(trace, 0, sizeof *trace);
    input->turn = 150;
    input->player_id = 4;
    input->planet_count = 2;
    input->planets = planets;
    input->fleet_count = 3;
    input->fleets = fleets;
    input->ops = ops;
    input->production_designs_valid = 1;
    planets[0].id = 7;
    planets[0].owner = 4;
    planets[0].production_slot_open = 1;
    planets[1].id = 8;
    planets[1].owner = -1;
    planets[1].visibility_flags = 3U;
    planets[1].desirability = 20;
    fleets[0].planet_id = 7;
    fleets[0].owner = 4;
    fleets[1].planet_id = 7;
    fleets[1].owner = 4;
    fleets[2].planet_id = 7;
    fleets[2].owner = 4;
    ops->planet_opt = opt;
    ops->prepare_production = prepare;
    ops->random_value = random_value;
    ops->queue_production_full = queue_full;
    ops->finish_production = finish;
    ops->context = trace;
}

int turin_minelayer_probe(void)
{
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinTurnPlanet planets[2];
    StarsTurinTurnFleet fleets[3];
    MineTrace trace;
    StarsI16 i;
    StarsTurinTurnPlanet swap;
    StarsU8 ai_records[9 * 16];

    checks = failures = 0;
    seed(&input, &ops, planets, fleets, &trace);
    input.design_1_policy_valid = 1;
    input.design_1_existing = 1UL;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.opt_calls == 1 && trace.prepared_after_opt == 1 &&
        input.unowned_viable_count == 1);
    check(trace.queue_calls == 4 && trace.finishes == 1 &&
        trace.changed == 1 && input.production_hulls_queued == 4);
    for (i = 0; i < 4; ++i)
        check(trace.designs[i] == 1 && trace.quantities[i] == 1 &&
            trace.modes[i] == 2 && trace.flags[i] == 1);

    /* Enemy worlds count only after they are visited in the second pass. */
    seed(&input, &ops, planets, fleets, &trace);
    input.design_1_policy_valid = 1;
    planets[1].owner = 3;
    planets[1].packed_flags = 0x200U;
    memset(ai_records, 0, sizeof ai_records);
    input.raw_ai_planet = ai_records;
    input.raw_ai_planet_size = sizeof ai_records;
    check(stars_do_turin_drone_ai_turn(&input));
    check(input.unowned_viable_count == 0 && input.enemy_viable_count == 1);
    check(trace.queue_calls == 0 && trace.prepared_after_opt == 0);
    check(ai_records[8 * 16 + 10] == 2 && ai_records[8 * 16 + 3] == 20);
    swap = planets[0]; planets[0] = planets[1]; planets[1] = swap;
    memset(&trace, 0, sizeof trace);
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queue_calls == 4 && trace.prepared_after_opt == 1);
    check(input.enemy_viable_count == 1);
    planets[0].desirability = 0;
    memset(&trace, 0, sizeof trace);
    check(stars_do_turin_drone_ai_turn(&input));
    check(input.enemy_viable_count == 0 && trace.queue_calls == 0);

    seed(&input, &ops, planets, fleets, &trace);
    input.design_1_policy_valid = 1;
    input.design_1_existing = 2UL;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.queue_calls == 0 && trace.changed == 0);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 0 && trace.queue_calls == 0);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    input.design_12_eligible = 1;
    trace.results[0] = 1;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 1 && trace.bounds[0] == 3 &&
        trace.queue_calls == 0);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    input.design_12_eligible = 1;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 2 && trace.bounds[0] == 3 &&
        trace.bounds[1] == 1);
    check(trace.queue_calls == 1 && trace.designs[0] == 12 &&
        trace.quantities[0] == 3 && trace.modes[0] == 2 &&
        trace.flags[0] == 1 && input.production_hulls_queued == 3);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    input.design_12_eligible = 1;
    fleets[0].ship_counts[12] = 9;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 2 && trace.bounds[1] == 19);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    input.design_12_eligible = 1;
    fleets[0].ship_counts[12] = 10;
    trace.results[1] = 1;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 2 && trace.bounds[1] == 8 &&
        trace.queue_calls == 0);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    input.design_12_eligible = 1;
    fleets[0].ship_counts[12] = 10;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 3 && trace.bounds[0] == 3 &&
        trace.bounds[1] == 8 && trace.bounds[2] == 21 &&
        trace.queue_calls == 1);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    input.design_12_eligible = 1;
    fleets[0].ship_counts[12] = 17;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 1 && trace.queue_calls == 0);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    input.design_12_eligible = 1;
    fleets[0].owner = 3;
    fleets[0].ship_counts[12] = 20;
    fleets[1].ship_counts[12] = 9;
    fleets[2].ship_counts[12] = 1;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 2 && trace.bounds[1] == 19 &&
        trace.queue_calls == 1);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    input.design_12_eligible = 1;
    fleets[0].ship_counts[12] = 256;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 1 && trace.queue_calls == 0);

    seed(&input, &ops, planets, fleets, &trace);
    input.design_1_policy_valid = 1;
    trace.reject_queue_at = 3;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(trace.queue_calls == 3 && trace.finishes == 0 &&
        input.production_hulls_queued == 2);

    seed(&input, &ops, planets, fleets, &trace);
    input.minelayer_policy_valid = 1;
    input.design_12_eligible = 1;
    ops.queue_production_full = 0;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(trace.random_calls == 0 && trace.finishes == 0);

#ifndef TURIN_MINE_NATIVE
    printf("TURIN_MINELAYER_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef TURIN_MINE_NATIVE
int main(void)
{
    return turin_minelayer_probe();
}
#endif
