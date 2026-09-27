#include "core.h"
#include <stdio.h>

int main(void)
{
    StarsHullPart parts[2];
    int failures = 0;

    parts[0].kind = 0x0040U;
    parts[0].packed_count_type = 0x0100U;
    parts[1].kind = 0U;
    parts[1].packed_count_type = 0U;
    if (stars_hull_has_bombs(parts, 2U) != 1)
        ++failures;
    parts[0].packed_count_type = 0U;
    parts[0].kind = 0x0010U;
    parts[0].packed_count_type = 0x0112U;
    if (stars_hull_has_bombs(parts, 1U) != 1)
        ++failures;
    parts[0].kind = 0x1000U;
    parts[0].packed_count_type = 0x0101U;
    if (stars_hull_has_bombs(parts, 1U) != 1)
        ++failures;
    parts[0].packed_count_type = 0x0001U;
    if (stars_hull_has_bombs(parts, 1U) != 0)
        ++failures;
    parts[0].kind = 0x0041U;
    parts[0].packed_count_type = 0x0100U;
    if (stars_hull_has_bombs(parts, 1U) != 0)
        ++failures;

    printf("HULL_HAS_BOMBS_CHECKS=5 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
