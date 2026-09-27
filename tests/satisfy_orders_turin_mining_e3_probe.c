#include "core.h"
#include "TURINMINE.H"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MiningTrace {
    StarsI16 values[3];
    StarsI16 calls;
    StarsI16 failures;
} MiningTrace;

static StarsI16 random_mineral(void *opaque, StarsI16 limit)
{
    MiningTrace *trace;
    StarsI16 index;

    trace = (MiningTrace *)opaque;
    index = trace->calls;
    ++trace->calls;
    if (limit != 100 || index < 0 || index >= 3) {
        ++trace->failures;
        return 99;
    }
    return trace->values[index];
}

int main(int argc, char **argv)
{
    MiningTrace trace;
    StarsMineralMinedInput input;
    StarsMineralMinedOps ops;
    StarsI32 stock[3];
    StarsI16 index;

    if (argc != 4)
        return 2;
    memset(&trace, 0, sizeof trace);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    for (index = 0; index < 3; ++index) {
        trace.values[index] = (StarsI16)atoi(argv[index + 1]);
        input.concentration[index] = retained_concentration[index];
        input.percent_level[index] = retained_percent_level[index];
        stock[index] = retained_stock[index];
    }
    input.player = 1;
    input.special_race = retained_special_race;
    input.efficiency_tenths = retained_efficiency_tenths;
    input.operating_mines = retained_operating_mines;
    input.c_mines = -1;
    input.random_rounding = 1;
    input.apply = 1;
    input.mineral_weight = stock;
    input.ops = &ops;
    ops.random = random_mineral;
    ops.context = &trace;
    stars_est_minerals_mined(&input);

    printf("MINING=%ld,%ld,%ld STOCK=%ld,%ld,%ld LEVELS=%u,%u,%u "
        "CONC=%u,%u,%u DRAWS=%d FAILURES=%d\n",
        (long)input.output[0], (long)input.output[1],
        (long)input.output[2], (long)stock[0], (long)stock[1],
        (long)stock[2], (unsigned)input.percent_level[0],
        (unsigned)input.percent_level[1], (unsigned)input.percent_level[2],
        (unsigned)input.concentration[0], (unsigned)input.concentration[1],
        (unsigned)input.concentration[2], (int)trace.calls,
        (int)trace.failures);
    return trace.failures != 0 || trace.calls != 3;
}
