#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 allocations[3][20];
    StarsU8 *live;
    StarsI16 add_count;
    StarsI16 bind_count;
    StarsI16 finish_count;
    StarsI16 finish_changed;
    StarsI16 bad;
    char events[9];
    StarsI16 event_count;
} Trace;

static void event(Trace *trace, char code)
{
    if (trace->event_count < 9)
        trace->events[trace->event_count] = code;
    ++trace->event_count;
}

static StarsI16 init(void *context, const StarsRototillTurnPlanet *planet)
{
    Trace *trace;
    trace = (Trace *)context;
    event(trace, 'I');
    if (planet->id != 3)
        trace->bad = 1;
    trace->live = trace->allocations[0];
    return 1;
}

static StarsI16 bind(void *context, const StarsRototillTurnPlanet *planet,
    const StarsU8 **queue, StarsU16 *size)
{
    Trace *trace;
    trace = (Trace *)context;
    event(trace, 'B');
    ++trace->bind_count;
    if (planet->id != 3)
        trace->bad = 1;
    *queue = trace->live;
    *size = 20U;
    return 1;
}

static StarsI16 add(void *context, const StarsRototillTurnPlanet *planet,
    StarsI16 design, StarsI16 count, StarsI16 kind, StarsI16 mode)
{
    Trace *trace;
    StarsAiProductionQueue global;
    StarsU8 *next;
    trace = (Trace *)context;
    event(trace, 'A');
    if (planet->id != 3 || planet->raw_prod_queue != trace->live ||
            count != 1 || kind != 2 || mode != 1 ||
            trace->add_count >= 2)
        return 0;
    next = trace->allocations[trace->add_count + 1];
    memcpy(next, trace->live, 20U);
    global.records = next + 4U;
    global.count = next[3];
    global.capacity = 4U;
    stars_add_item_to_queue(0, &global, (StarsU16)design,
        (StarsU16)count, (StarsU16)kind, mode, 0, 0);
    next[3] = (StarsU8)global.count;
    trace->live = next;
    ++trace->add_count;
    return 1;
}

static StarsI16 finish(void *context,
    const StarsRototillTurnPlanet *planet, StarsI16 changed)
{
    Trace *trace;
    trace = (Trace *)context;
    event(trace, 'F');
    ++trace->finish_count;
    trace->finish_changed = changed;
    if (planet->raw_prod_queue != trace->live)
        trace->bad = 1;
    return 1;
}

static void reset(Trace *trace)
{
    memset(trace, 0, sizeof *trace);
    trace->allocations[0][2] = 4U;
}

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnPlanet planet;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    const StarsU8 zero_item[4] = {1U, 0U, 4U, 0U};
    const StarsU8 one_item[4] = {1U, 4U, 4U, 0U};

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&planet, 0, sizeof planet);
    reset(&trace);
    planet.id = 3;
    planet.owner = 2;
    planet.visibility_flags = 0x0200U;
    planet.mineral_rate_high = 1;
    ops.init_production = init;
    ops.current_prod_queue = bind;
    ops.add_raw_queue_item = add;
    ops.finish_production = finish;
    ops.context = &trace;
    input.turn = 0;
    input.player_id = 2;
    input.planet_count = 1;
    input.planets = &planet;
    input.ops = &ops;
    input.production_inputs_bound = 1;

    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.event_count == 7 &&
        memcmp(trace.events, "IBABABF", 7U) == 0,
        &checks, &failures);
    check(trace.add_count == 2 && trace.bind_count == 3 &&
        trace.finish_count == 1 && trace.finish_changed == 0 &&
        !trace.bad, &checks, &failures);
    check(trace.live == trace.allocations[2] &&
        trace.live[3] == 2U, &checks, &failures);
    check(memcmp(trace.live + 4U, zero_item, 4U) == 0 &&
        memcmp(trace.live + 8U, zero_item, 4U) == 0,
        &checks, &failures);
    check(trace.allocations[0][3] == 0U &&
        trace.allocations[1][3] == 1U, &checks, &failures);

    reset(&trace);
    input.turn = 20;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.event_count == 5 &&
        memcmp(trace.events, "IBABF", 5U) == 0 &&
        trace.add_count == 1 && trace.bind_count == 2 &&
        trace.finish_changed == 1 && !trace.bad, &checks, &failures);
    check(trace.live[3] == 1U &&
        memcmp(trace.live + 4U, one_item, 4U) == 0,
        &checks, &failures);

    reset(&trace);
    trace.allocations[0][3] = 1U;
    trace.allocations[0][5] = 0x44U;
    trace.allocations[0][6] = 0x04U;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.event_count == 3 &&
        memcmp(trace.events, "IBF", 3U) == 0 &&
        trace.add_count == 0 && trace.finish_changed == 0,
        &checks, &failures);

    printf("ROTOTILL_QUEUE_WRITER_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
