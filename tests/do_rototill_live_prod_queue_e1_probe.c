#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    const StarsU8 *live;
    StarsU16 live_size;
    StarsI16 fail_bind;
    StarsI16 bad_args;
    StarsI16 adds;
    StarsI16 finishes;
    const StarsU8 *after_first;
    const StarsU8 *after_second;
    StarsI16 expected_design;
    StarsI16 expected_changed;
    char order[8];
    StarsI16 events;
} Trace;

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

static StarsI16 init(void *opaque, const StarsRototillTurnPlanet *planet)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->order[trace->events++] = 'I';
    if (planet->id != 3)
        trace->bad_args = 1;
    return 1;
}

static StarsI16 bind(void *opaque, const StarsRototillTurnPlanet *planet,
    const StarsU8 **queue, StarsU16 *size)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->order[trace->events++] = 'B';
    if (planet->id != 3 ||
            (trace->events != 2 &&
             trace->order[trace->events - 2] != 'Q'))
        trace->bad_args = 1;
    *queue = trace->live;
    *size = trace->live_size;
    return !trace->fail_bind;
}

static StarsI16 add(void *opaque, const StarsRototillTurnPlanet *planet,
    StarsI16 design, StarsI16 count, StarsI16 kind, StarsI16 flag)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->order[trace->events++] = 'Q';
    ++trace->adds;
    if (planet->raw_prod_queue != trace->live ||
            design != trace->expected_design ||
            count != 1 || kind != 2 || flag != 1)
        trace->bad_args = 1;
    if (trace->adds == 1 && trace->after_first)
        trace->live = trace->after_first;
    if (trace->adds == 2 && trace->after_second)
        trace->live = trace->after_second;
    return 1;
}

static StarsI16 finish(void *opaque,
    const StarsRototillTurnPlanet *planet, StarsI16 changed)
{
    Trace *trace;
    trace = (Trace *)opaque;
    trace->order[trace->events++] = 'F';
    ++trace->finishes;
    if (planet->raw_prod_queue != trace->live ||
            changed != trace->expected_changed)
        trace->bad_args = 1;
    return 1;
}

static void reset_trace(Trace *trace, const StarsU8 *live, StarsU16 size)
{
    memset(trace, 0, sizeof *trace);
    trace->live = live;
    trace->live_size = size;
}

int main(void)
{
    StarsRototillAiTurnInput input;
    StarsRototillAiTurnOps ops;
    StarsRototillTurnPlanet planet;
    StarsU8 stale[4];
    StarsU8 live_bypass[8];
    StarsU8 live_open[4];
    StarsU8 after_first[4];
    StarsU8 after_second[4];
    StarsU8 malformed_after_add[4];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&planet, 0, sizeof planet);
    memset(stale, 0, sizeof stale);
    memset(live_bypass, 0, sizeof live_bypass);
    memset(live_open, 0, sizeof live_open);
    memset(after_first, 0, sizeof after_first);
    memset(after_second, 0, sizeof after_second);
    memset(malformed_after_add, 0, sizeof malformed_after_add);
    stale[3] = 250;
    live_bypass[3] = 1;
    live_bypass[5] = 0x44U;
    live_bypass[6] = 0x04U;
    malformed_after_add[3] = 1;
    planet.id = 3;
    planet.owner = 2;
    planet.visibility_flags = 0x0200U;
    planet.mineral_rate_high = 1;
    planet.raw_prod_queue = stale;
    planet.raw_prod_queue_size = 4;
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

    reset_trace(&trace, live_bypass, 8);
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.events == 3 && memcmp(trace.order, "IBF", 3) == 0 &&
        trace.adds == 0 && trace.finishes == 1 &&
        input.queue_bypass_count == 1 && !trace.bad_args,
        &checks, &failures);

    reset_trace(&trace, live_open, 4);
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.events == 7 && memcmp(trace.order, "IBQBQBF", 7) == 0 &&
        trace.adds == 2 && trace.finishes == 1 &&
        input.production_items_added == 2 && !trace.bad_args,
        &checks, &failures);

    reset_trace(&trace, live_bypass, 7);
    check(!stars_do_rototill_ai_turn(&input) &&
        trace.events == 2 && memcmp(trace.order, "IB", 2) == 0 &&
        trace.adds == 0 && trace.finishes == 0,
        &checks, &failures);
    reset_trace(&trace, live_open, 4);
    trace.fail_bind = 1;
    check(!stars_do_rototill_ai_turn(&input) &&
        trace.events == 2 && trace.adds == 0 && trace.finishes == 0,
        &checks, &failures);

    planet.raw_prod_queue = 0;
    reset_trace(&trace, live_open, 4);
    check(stars_do_rototill_ai_turn(&input) && trace.adds == 2 &&
        trace.finishes == 1 && !trace.bad_args,
        &checks, &failures);

    reset_trace(&trace, live_open, 4);
    trace.after_first = after_first;
    trace.after_second = after_second;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.events == 7 && memcmp(trace.order, "IBQBQBF", 7) == 0 &&
        trace.live == after_second && trace.adds == 2 &&
        trace.finishes == 1 && !trace.bad_args,
        &checks, &failures);

    reset_trace(&trace, live_open, 4);
    trace.after_first = after_first;
    trace.expected_design = 1;
    trace.expected_changed = 1;
    input.turn = 20;
    check(stars_do_rototill_ai_turn(&input), &checks, &failures);
    check(trace.events == 5 && memcmp(trace.order, "IBQBF", 5) == 0 &&
        trace.live == after_first && trace.adds == 1 &&
        trace.finishes == 1 && !trace.bad_args,
        &checks, &failures);

    reset_trace(&trace, live_open, 4);
    trace.after_first = malformed_after_add;
    input.turn = 0;
    check(!stars_do_rototill_ai_turn(&input) &&
        trace.events == 4 && memcmp(trace.order, "IBQB", 4) == 0 &&
        trace.adds == 1 && trace.finishes == 0 &&
        input.production_items_added == 1,
        &checks, &failures);

    printf("ROTOTILL_LIVE_QUEUE_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
