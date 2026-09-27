#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Engines { StarsIdealWarpEngine e[16]; } Engines;
static const StarsIdealWarpEngine *lookup(void *v, StarsU16 id)
{
    return &((Engines *)v)->e[id & 15U];
}

/* Original NE11:A902-A98B chooses the first zero-fuel speed one, two,
 * or three steps below, subject to minimum current speeds 5, 6, and 7.
 * NE11:A96B-A97E addresses fuel_used[warp-3], not a separate ore array. */
static int check(StarsI16 warp, StarsI16 zero, StarsU16 id,
    StarsI16 ignore, StarsI16 expected)
{
    StarsFindIdealWarpInput input;
    Engines engines;
    StarsU16 i;
    memset(&input, 0, sizeof input);
    memset(&engines, 0, sizeof engines);
    input.ship_counts[0] = 1;
    input.designs[0].engine_count = 1;
    input.designs[0].engine_flags[0] = 1;
    input.designs[0].engine_ids[0] = id;
    input.lookup_engine = lookup;
    input.context = &engines;
    for (i = 0; i < 12U; ++i) engines.e[id].fuel_used[i] = 121;
    engines.e[id].fuel_used[warp] = 10;
    if (zero >= 0) engines.e[id].fuel_used[zero] = 0;
    return stars_find_ideal_warp(&input, ignore) == expected;
}

int main(void)
{
    int failures = 0;
    if (!check(8, 7, 1, 0, 7)) ++failures;
    if (!check(8, 6, 1, 0, 6)) ++failures;
    if (!check(8, 5, 1, 0, 5)) ++failures;
    if (!check(8, -1, 1, 0, 8)) ++failures;
    if (!check(4, 3, 1, 0, 4)) ++failures;
    if (!check(5, 4, 1, 0, 4)) ++failures;
    if (!check(5, 3, 1, 0, 5)) ++failures;
    if (!check(6, 4, 1, 0, 4)) ++failures;
    if (!check(6, 3, 1, 0, 6)) ++failures;
    if (!check(7, 4, 1, 0, 4)) ++failures;
    if (!check(8, 7, 1, 1, 8)) ++failures;
    if (!check(10, -1, 1, 1, 9)) ++failures;
    if (!check(10, -1, 7, 0, 10)) ++failures;
    if (!check(10, 9, 14, 0, 10)) ++failures;
    if (!check(10, 9, 15, 0, 10)) ++failures;
    printf("FIND_IDEAL_WARP_CHECKS=15 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
