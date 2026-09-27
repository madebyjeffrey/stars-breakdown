#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 events[6];
    StarsI16 count;
    StarsI16 order_ok;
    StarsI16 init_calls;
} Trace;

static void check(StarsI16 yes, StarsI16 *checks, StarsI16 *failures)
{
    ++*checks;
    if (!yes)
        ++*failures;
}

static StarsI16 optimal(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI16 player)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->count < 6)
        trace->events[trace->count] = planet->id;
    ++trace->count;
    if (player != 2)
        trace->order_ok = 0;
    if (planet->id == 1) {
        if (planet->raw_ai_planet[9] != 0)
            trace->order_ok = 0;
        return 3;
    }
    if (planet->raw_ai_planet[9] != 1 ||
            planet->raw_ai_planet[10] == 0)
        trace->order_ok = 0;
    return planet->id == 2 ? 5 : 0;
}

static StarsI16 desirability(void *opaque,
    const StarsAutomitronTurnPlanet *planet, StarsI16 player)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->count < 6)
        trace->events[trace->count] = planet->id;
    ++trace->count;
    if (player != 2 || planet->id != 0 ||
            planet->raw_ai_planet[9] != 1 ||
            planet->raw_ai_planet[2] != 0)
        trace->order_ok = 0;
    return -1;
}

static StarsI16 init(void *opaque,
    const StarsAutomitronTurnPlanet *planet)
{
    Trace *trace;
    trace = (Trace *)opaque;
    (void)planet;
    ++trace->init_calls;
    return 1;
}

int main(void)
{
    StarsAutomitronAiTurnInput input;
    StarsAutomitronAiTurnOps ops;
    StarsAutomitronTurnPlanet planets[4];
    StarsU8 records[4][16];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;

    checks = 0;
    failures = 0;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(planets, 0, sizeof planets);
    memset(records, 0, sizeof records);
    memset(&trace, 0, sizeof trace);
    for (index = 0; index < 4; ++index) {
        planets[index].id = index;
        planets[index].raw_ai_planet = records[index];
        planets[index].raw_ai_planet_size = 16;
    }
    planets[0].owner = 2;
    planets[0].desirability = 4;
    planets[0].visibility_flags = 0x0200U;
    planets[0].mineral_rate = 1;
    planets[1].owner = -1;
    planets[1].planet_class = 3;
    planets[1].optimal_value = 0;
    planets[2].owner = 3;
    planets[2].optimal_value = -1;
    planets[2].visibility_flags = 0x0200U;
    planets[3].owner = 3;
    planets[3].optimal_value = 7;
    trace.order_ok = 1;
    ops.pct_planet_opt_value = optimal;
    ops.pct_planet_desirability = desirability;
    ops.init_production = init;
    ops.context = &trace;
    input.turn = 20;
    input.player_id = 2;
    input.planet_count = 4;
    input.planets = planets;
    input.ops = &ops;

    check(stars_do_automitron_ai_turn(&input), &checks, &failures);
    check(trace.count == 4 && trace.events[0] == 1 &&
        trace.events[1] == 0 && trace.events[2] == 2 &&
        trace.events[3] == 3, &checks, &failures);
    check(trace.order_ok, &checks, &failures);
    check(input.unowned_viable_count == 1 &&
        input.owned_bad_count == 1, &checks, &failures);
    check(records[0][2] == 1 && records[0][9] == 1 &&
        trace.init_calls == 0, &checks, &failures);
    check(records[1][9] == 1, &checks, &failures);
    check(records[2][3] == 1 && records[2][10] == 2,
        &checks, &failures);
    check(records[3][3] == 0 && records[3][10] == 1,
        &checks, &failures);

    printf("AUTOMITRON_PLANET_VALUES_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
