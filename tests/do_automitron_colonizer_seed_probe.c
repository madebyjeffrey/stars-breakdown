#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 bounds[4];
    StarsI16 values[4];
    StarsI16 calls;
    StarsI16 queue_calls;
    StarsI16 queue_args[4];
    StarsI16 queue_result;
} Trace;

static StarsI16 random_value(void *context, StarsI16 bound)
{
    Trace *trace;
    StarsI16 index;
    trace = (Trace *)context;
    index = trace->calls++;
    if (index >= 4)
        return 1;
    trace->bounds[index] = bound;
    return trace->values[index];
}

static StarsI16 add_raw_item(void *context,
    const StarsAutomitronTurnPlanet *planet, StarsI16 a, StarsI16 b,
    StarsI16 c, StarsI16 d)
{
    Trace *trace;
    trace = (Trace *)context;
    if (planet->id != 10)
        return 0;
    ++trace->queue_calls;
    trace->queue_args[0] = a;
    trace->queue_args[1] = b;
    trace->queue_args[2] = c;
    trace->queue_args[3] = d;
    return trace->queue_result;
}

static void setup(StarsAutomitronAiTurnInput *input,
    StarsAutomitronAiTurnOps *ops, StarsAutomitronTurnPlanet *planet,
    StarsAutomitronTurnFleet fleets[3], Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    memset(planet, 0, sizeof *planet);
    memset(fleets, 0, sizeof(StarsAutomitronTurnFleet) * 3);
    memset(trace, 0, sizeof *trace);
    trace->queue_result = 1;
    ops->random = random_value;
    ops->add_raw_queue_item = add_raw_item;
    ops->context = trace;
    planet->id = 10;
    planet->owner = 0;
    planet->visibility_flags = 0x0200U;
    planet->mineral_rate = 1;
    planet->colonizer_queue_eligible = 1;
    input->player_id = 0;
    input->planet_count = 1;
    input->planets = planet;
    input->fleets = fleets;
    input->ops = ops;
    input->colonizer_seed_inputs_bound = 1;
}

static void check(StarsI16 value, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!value)
        ++*failures;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnPlanet planet;
    StarsAutomitronTurnFleet fleets[3];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    setup(&input, &ops, &planet, fleets, &trace);
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 2 && trace.bounds[0] == 3 &&
        trace.bounds[1] == 1, &checks, &failures);
    check(trace.queue_calls == 1 && input.colonizer_seed_queued == 1,
        &checks, &failures);
    check(trace.queue_args[0] == 6 && trace.queue_args[1] == 3 &&
        trace.queue_args[2] == 2 && trace.queue_args[3] == 1,
        &checks, &failures);

    setup(&input, &ops, &planet, fleets, &trace);
    input.fleet_count = 3;
    fleets[0].owner = 1;
    fleets[0].planet_id = 10;
    fleets[0].ship_counts[6] = 2;
    fleets[1].owner = 0;
    fleets[1].planet_id = 10;
    fleets[1].ship_counts[6] = 9;
    fleets[2].owner = 0;
    fleets[2].planet_id = 10;
    fleets[2].ship_counts[6] = 17;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 2 && trace.bounds[1] == 19 &&
        trace.queue_calls == 1, &checks, &failures);

    setup(&input, &ops, &planet, fleets, &trace);
    input.fleet_count = 1;
    fleets[0].planet_id = 10;
    fleets[0].ship_counts[6] = 10;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 3 && trace.bounds[0] == 3 &&
        trace.bounds[1] == 8 && trace.bounds[2] == 21,
        &checks, &failures);
    check(trace.queue_calls == 1, &checks, &failures);

    setup(&input, &ops, &planet, fleets, &trace);
    input.fleet_count = 1;
    fleets[0].planet_id = 10;
    fleets[0].ship_counts[6] = 16;
    trace.values[1] = 1;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 2 && trace.bounds[1] == 8 &&
        trace.queue_calls == 0, &checks, &failures);

    setup(&input, &ops, &planet, fleets, &trace);
    input.fleet_count = 1;
    fleets[0].planet_id = 10;
    fleets[0].ship_counts[6] = 17;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 1 && trace.queue_calls == 0,
        &checks, &failures);

    setup(&input, &ops, &planet, fleets, &trace);
    input.colonizer_design_flags = 0x0200U;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 0 && trace.queue_calls == 0,
        &checks, &failures);
    input.colonizer_design_flags = 0;
    planet.colonizer_queue_eligible = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 0 && trace.queue_calls == 0,
        &checks, &failures);

    setup(&input, &ops, &planet, fleets, &trace);
    trace.queue_result = 0;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.queue_calls == 1 && input.colonizer_seed_queued == 0,
        &checks, &failures);

    printf("AUTOMITRON_COLONIZER_SEED_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
