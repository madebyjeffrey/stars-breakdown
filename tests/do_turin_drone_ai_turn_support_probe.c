#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct SupportTrace {
    StarsI16 potent_calls;
    StarsI16 potent_ids[6];
    StarsI16 potent_arg[6];
    StarsI16 queue_calls;
    StarsI16 queue_design[8];
    StarsI16 queue_quantity[8];
    StarsI16 queue_mode[8];
    StarsI16 queue_flag[8];
    StarsI16 reject_queue;
    StarsI16 finish_calls;
    StarsI16 finish_changed;
    StarsI16 select_support;
} SupportTrace;

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
#ifndef TURIN_SUPPORT_NATIVE
        fprintf(stderr, "Turin support check failed at line %d\n", line);
#else
        (void)line;
#endif
    }
}

#define check(condition) check_at((condition), __LINE__)

static StarsI16 status(void *opaque, StarsI16 begin, StarsI16 end,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    SupportTrace *trace;
    (void)end;
    (void)budget;
    (void)mask;
    trace = (SupportTrace *)opaque;
    *selected = begin == 13 && trace->select_support ? 13 :
        (begin == 4 ? 4 : -1);
    return 1;
}

static StarsI16 potent(void *opaque, const StarsTurinTurnFleet *fleet,
    StarsI16 arg)
{
    SupportTrace *trace;
    StarsI16 index;
    trace = (SupportTrace *)opaque;
    index = trace->potent_calls++;
    if (index < 6) {
        trace->potent_ids[index] = fleet->id;
        trace->potent_arg[index] = arg;
    }
    return (StarsI16)(fleet->id != 0);
}

static StarsI16 queue_full(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 design,
    StarsI16 quantity, StarsI16 mode, StarsI16 flag)
{
    SupportTrace *trace;
    StarsI16 index;
    (void)planet;
    trace = (SupportTrace *)opaque;
    index = trace->queue_calls++;
    if (index < 8) {
        trace->queue_design[index] = design;
        trace->queue_quantity[index] = quantity;
        trace->queue_mode[index] = mode;
        trace->queue_flag[index] = flag;
    }
    return (StarsI16)(!trace->reject_queue);
}

static StarsI16 finish(void *opaque, const StarsTurinTurnPlanet *planet,
    StarsI16 changed)
{
    SupportTrace *trace;
    (void)planet;
    trace = (SupportTrace *)opaque;
    ++trace->finish_calls;
    trace->finish_changed = changed;
    return 1;
}

static void seed(StarsTurinDroneAiTurnInput *input,
    StarsTurinDroneAiTurnOps *ops, StarsTurinTurnPlanet *planet,
    StarsTurinTurnFleet fleets[3], SupportTrace *trace)
{
    StarsI16 i;
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    memset(planet, 0, sizeof *planet);
    memset(fleets, 0, 3 * sizeof *fleets);
    memset(trace, 0, sizeof *trace);
    input->turn = 150;
    input->player_id = 4;
    input->plan_max = 240U;
    input->planet_count = 1;
    input->planets = planet;
    input->fleet_count = 3;
    input->fleets = fleets;
    input->ops = ops;
    input->production_designs_valid = 1;
    input->support_policy_valid = 1;
    planet->id = 7;
    planet->owner = 4;
    planet->production_slot_open = 1;
    for (i = 0; i < 4; ++i) {
        planet->resources_available[i] = 100L;
        input->design_hull_cost[3][i] = 1L;
    }
    for (i = 0; i < 3; ++i) {
        fleets[i].id = (StarsI16)(i + 1);
        fleets[i].owner = 4;
        fleets[i].planet_id = 7;
    }
    trace->select_support = 1;
    ops->check_design_status = status;
    ops->potent_rob_war_fleet = potent;
    ops->queue_production_full = queue_full;
    ops->finish_production = finish;
    ops->context = trace;
}

int turin_support_probe(void)
{
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    StarsTurinTurnPlanet planet;
    StarsTurinTurnFleet fleets[3];
    SupportTrace trace;
    StarsI16 i;

    checks = failures = 0;
    seed(&input, &ops, &planet, fleets, &trace);
    fleets[0].ship_counts[13] = 3;
    fleets[0].ship_counts[14] = 5;
    check(stars_do_turin_drone_ai_turn(&input));
    check(input.armada_potency[2] == 8 && trace.potent_calls == 1 &&
        trace.potent_ids[0] == 1 && trace.potent_arg[0] == 2);
    check(trace.queue_calls == 1 && trace.queue_design[0] == 13 &&
        trace.queue_quantity[0] == 4 && trace.queue_mode[0] == 2 &&
        trace.queue_flag[0] == 1);
    check(trace.finish_calls == 1 && trace.finish_changed == 1 &&
        input.production_hulls_queued == 4);

    seed(&input, &ops, &planet, fleets, &trace);
    fleets[0].ship_counts[13] = 3;
    fleets[0].ship_counts[14] = 4;
    fleets[1].ship_counts[13] = 10;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.potent_calls == 1 && trace.queue_calls == 5 &&
        input.production_hulls_queued == 5);
    for (i = 0; i < 5; ++i)
        check(trace.queue_design[i] == 4 &&
            trace.queue_quantity[i] == 1);

    seed(&input, &ops, &planet, fleets, &trace);
    fleets[0].id = 0;
    fleets[1].ship_counts[13] = 8;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.potent_calls == 2 && trace.potent_ids[0] == 0 &&
        trace.potent_ids[1] == 2 && trace.queue_calls == 1 &&
        trace.queue_design[0] == 13);

    seed(&input, &ops, &planet, fleets, &trace);
    fleets[0].owner = 3;
    fleets[1].planet_id = 8;
    fleets[2].ship_counts[14] = 8;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.potent_calls == 1 && trace.potent_ids[0] == 3 &&
        trace.queue_design[0] == 13);

    seed(&input, &ops, &planet, fleets, &trace);
    trace.select_support = 0;
    fleets[0].ship_counts[13] = 8;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.potent_calls == 0 && trace.queue_calls == 5 &&
        trace.queue_design[0] == 4);

    seed(&input, &ops, &planet, fleets, &trace);
    ops.potent_rob_war_fleet = 0;
    fleets[0].ship_counts[2] = 4;
    fleets[0].ship_counts[13] = 8;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.potent_calls == 0 && trace.queue_calls == 1 &&
        trace.queue_design[0] == 13 && trace.finish_changed == 1);

    seed(&input, &ops, &planet, fleets, &trace);
    fleets[0].ship_counts[13] = 8;
    trace.reject_queue = 1;
    check(!stars_do_turin_drone_ai_turn(&input));
    check(trace.queue_calls == 1 && trace.finish_calls == 0 &&
        input.production_queued == 0);

    seed(&input, &ops, &planet, fleets, &trace);
    planet.production_queue_has_ship = 1;
    fleets[0].ship_counts[13] = 8;
    check(stars_do_turin_drone_ai_turn(&input));
    check(trace.potent_calls == 0 && trace.queue_calls == 0 &&
        trace.finish_calls == 1 && trace.finish_changed == 0);

#ifndef TURIN_SUPPORT_NATIVE
    printf("TURIN_SUPPORT_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef TURIN_SUPPORT_NATIVE
int main(void)
{
    return turin_support_probe();
}
#endif
