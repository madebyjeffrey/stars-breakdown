#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsU16 hull_type(void *context, StarsU16 player, StarsU16 index)
{
    StarsU16 (*types)[16] = (StarsU16 (*)[16])context;
    return types[player][index];
}

int main(void)
{
    StarsI16 counts[16];
    StarsU16 types[2][16];
    int failures = 0;

    memset(counts, 0, sizeof counts);
    memset(types, 0, sizeof types);
    counts[2] = 1;
    types[0][2] = 14;
    if (stars_f_colonizer(counts, 0U, hull_type, types) != 1 ||
        stars_f_scout(counts, 0U, hull_type, types) != 0)
        ++failures;

    memset(counts, 0, sizeof counts);
    counts[4] = 2;
    types[1][4] = 5;
    if (stars_f_colonizer(counts, 1U, hull_type, types) != 0 ||
        stars_f_scout(counts, 1U, hull_type, types) != 1)
        ++failures;

    memset(counts, 0, sizeof counts);
    if (stars_f_colonizer(counts, 0U, hull_type, types) != 0 ||
        stars_f_scout(counts, 0U, hull_type, types) != 0)
        ++failures;

    printf("FLEET_ROLE_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
