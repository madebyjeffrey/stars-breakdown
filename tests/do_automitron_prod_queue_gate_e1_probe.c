#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsU8 *queue;
    StarsI16 calls;
    StarsI16 designs[3];
    StarsI16 mutate_first;
} Trace;

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

static void set_item(StarsU8 *raw, StarsU32 item)
{
    raw[4] = (StarsU8)item;
    raw[5] = (StarsU8)(item >> 8);
    raw[6] = (StarsU8)(item >> 16);
    raw[7] = (StarsU8)(item >> 24);
    raw[3] = 1;
}

static StarsI16 status(void *opaque, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected)
{
    (void)opaque;
    (void)last;
    (void)budget;
    *selected = first == 4 ? 7 : -1;
    return 0;
}

static StarsI16 queue_item(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI16 design,
    StarsI16 count, StarsI16 kind, StarsI16 flag)
{
    Trace *trace;
    trace = (Trace *)opaque;
    (void)planet;
    if (count != 1 || kind != 2 || flag != 1)
        return 0;
    if (trace->calls < 3)
        trace->designs[trace->calls] = design;
    ++trace->calls;
    if (trace->calls == 1 && trace->mutate_first)
        set_item(trace->queue, ((StarsU32)2 << 17) |
            ((StarsU32)16 << 10));
    return 1;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnPlanet planets[2];
    StarsU8 queue[12];
    StarsU8 ai_record[16];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(planets, 0, sizeof planets);
    memset(queue, 0, sizeof queue);
    memset(ai_record, 0, sizeof ai_record);
    memset(&trace, 0, sizeof trace);
    planets[0].id = 4;
    planets[0].owner = 2;
    planets[0].visibility_flags = 0x0200U;
    planets[0].mineral_rate = 1;
    planets[0].raw_prod_queue = queue;
    planets[0].raw_prod_queue_size = sizeof queue;
    planets[0].raw_ai_planet = ai_record;
    planets[0].raw_ai_planet_size = sizeof ai_record;
    planets[1].id = 5;
    planets[1].owner = -1;
    planets[1].planet_class = 3;
    planets[1].optimal_value = 1;
    ops.check_design_status = status;
    ops.add_raw_queue_item = queue_item;
    ops.context = &trace;
    trace.queue = queue;
    input.turn = 20;
    input.player_id = 2;
    input.planet_count = 2;
    input.planets = planets;
    input.ops = &ops;
    input.early_queue_inputs_bound = 1;
    input.ai_data_group_count = 3;
    input.owned_planet_count = 10;
    input.tech_level_two = 5;

    trace.mutate_first = 1;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 2 && trace.designs[0] == 7 &&
        trace.designs[1] == 1, &checks, &failures);
    check(queue[3] == 1 && queue[5] == 0x40U &&
        queue[6] == 0x04U, &checks, &failures);

    trace.calls = 0;
    trace.mutate_first = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 0, &checks, &failures);

    set_item(queue, ((StarsU32)2 << 17) | ((StarsU32)15 << 10));
    trace.calls = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 2, &checks, &failures);

    set_item(queue, ((StarsU32)1 << 17) | ((StarsU32)16 << 10));
    trace.calls = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 2, &checks, &failures);

    set_item(queue, ((StarsU32)2 << 17) | ((StarsU32)127 << 10));
    trace.calls = 0;
    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.calls == 0, &checks, &failures);

    memset(ai_record, 0, sizeof ai_record);
    planets[0].raw_prod_queue_size = 7;
    trace.calls = 0;
    check(!stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(ai_record[9] == 0 && trace.calls == 0,
        &checks, &failures);

    printf("AUTOMITRON_PROD_GATE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
