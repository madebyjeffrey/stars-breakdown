#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 count;
    StarsU16 type[8];
    StarsU16 length[8];
    StarsU8 record[8][40];
    StarsU16 previous_calls;
} Trace;

static void write_memory(void *context, StarsU16 type, StarsU16 length,
    const StarsU8 *record)
{
    Trace *trace = (Trace *)context;
    StarsU16 n = trace->count++;
    trace->type[n] = type;
    trace->length[n] = length;
    memcpy(trace->record[n], record, length);
}

static StarsI16 previous_production(void *context, StarsI16 id)
{
    Trace *trace = (Trace *)context;
    (void)id;
    ++trace->previous_calls;
    return 1;
}

static void init_planet(StarsLogPlanet *planet)
{ memset(planet, 0, sizeof *planet); planet->object_id = 0x2345; }

int main(void)
{
    StarsLogPlanet old_planet, new_planet;
    StarsLogChangePlanetState state;
    StarsLogChangePlanetOps ops;
    Trace trace;
    StarsU8 old_prod[8];
    StarsU8 new_prod[8];

    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.write_memory = write_memory;
    ops.previous_production = previous_production;
    init_planet(&old_planet);
    init_planet(&new_planet);
    new_planet.minerals[1] = 7U;
    stars_log_change_planet(&old_planet, &new_planet, 0U, &state, &ops,
        &trace);
    if (!state.valid_xfer || trace.count != 0U) return 1;
    init_planet(&old_planet);
    init_planet(&new_planet);
    new_planet.minerals[1] = 9U;
    stars_log_change_planet(&old_planet, &new_planet, 0U, &state, &ops,
        &trace);
    if (state.valid_xfer || trace.count != 1U || trace.type[0] != 1U ||
            trace.length[0] != 7U || trace.record[0][6] != 16U) return 2;

    memset(&trace, 0, sizeof trace);
    init_planet(&old_planet);
    init_planet(&new_planet);
    old_planet.has_production = 1U;
    old_planet.production = old_prod;
    new_planet.has_production = 0U;
    stars_log_change_planet(&old_planet, &new_planet, 0U, &state, &ops,
        &trace);
    if (trace.count != 1U || trace.type[0] != 0x1dU ||
            trace.length[0] != 2U) return 3;

    memset(&trace, 0, sizeof trace);
    init_planet(&old_planet);
    init_planet(&new_planet);
    memset(old_prod, 0, sizeof old_prod);
    memset(new_prod, 0, sizeof new_prod);
    new_prod[0] = 8U;
    old_planet.has_production = 1U;
    old_planet.production_count = 1U;
    old_planet.production = old_prod;
    new_planet.has_production = 1U;
    new_planet.production_count = 1U;
    new_planet.production = new_prod;
    stars_log_change_planet(&old_planet, &new_planet, 0U, &state, &ops,
        &trace);
    if (trace.count != 0U || trace.previous_calls != 1U) return 4;
    ops.previous_production = 0;
    stars_log_change_planet(&old_planet, &new_planet, 0U, &state, &ops,
        &trace);
    if (trace.count != 1U || trace.type[0] != 0x1dU ||
            trace.length[0] != 6U || trace.record[0][2] != 8U) return 5;

    memset(&trace, 0, sizeof trace);
    new_planet.production_count = 2U;
    new_prod[7] = 0x7eU;
    stars_log_change_planet(&old_planet, &new_planet, 0U, &state, &ops,
        &trace);
    if (trace.count != 1U || trace.type[0] != 0x1dU ||
            trace.length[0] != 10U || trace.record[0][9] != 0x7eU) return 7;

    memset(&trace, 0, sizeof trace);
    init_planet(&old_planet);
    init_planet(&new_planet);
    new_planet.overlap_20 = 1UL << 23;
    new_planet.overlap_44 = 0x1234U;
    new_planet.overlap_48 = 0x155U;
    stars_log_change_planet(&old_planet, &new_planet, 0U, &state, &ops,
        &trace);
    if (trace.count != 1U || trace.type[0] != 0x23U ||
            trace.length[0] != 6U || trace.record[0][2] != 0x69U) return 6;

    puts("LOG_CHANGE_PLANET_SOURCE=PASS");
    return 0;
}
