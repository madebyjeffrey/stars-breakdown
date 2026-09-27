#include "core.h"
#include <stdio.h>
#include <string.h>

static const StarsHullPart *lookup(void *context, StarsU16 owner,
    StarsU16 index, StarsU16 *count)
{
    StarsHullPart (*parts)[16][2] = (StarsHullPart (*)[16][2])context;
    (void)owner;
    *count = 2U;
    return parts[0][index];
}

int main(void)
{
    StarsI16 counts[16];
    StarsHullPart parts[1][16][2];
    int failures = 0;

    memset(counts, 0, sizeof counts);
    memset(parts, 0, sizeof parts);
    counts[3] = 1;
    parts[0][3][1].kind = 0x1000U;
    parts[0][3][1].packed_count_type = 0x0101U;
    if (stars_fleet_has_bombs(counts, 4U, lookup, parts) != 1)
        ++failures;
    parts[0][3][1].packed_count_type = 0U;
    if (stars_fleet_has_bombs(counts, 4U, lookup, parts) != 0)
        ++failures;
    counts[3] = 0;
    if (stars_fleet_has_bombs(counts, 4U, lookup, parts) != 0)
        ++failures;

    printf("FLEET_HAS_BOMBS_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
