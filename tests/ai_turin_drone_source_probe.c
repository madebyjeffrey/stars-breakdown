#include "core.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    StarsI16 counts[16];
    StarsI16 hulls[16];
    int failures = 0;
    memset(counts, 0, sizeof counts);
    memset(hulls, 0, sizeof hulls);
    counts[3] = 1;
    hulls[3] = 4;
    if (stars_is_turin_drone_ai_attack(counts, hulls) != 1)
        ++failures;
    hulls[3] = 3;
    if (stars_is_turin_drone_ai_attack(counts, hulls) != 0)
        ++failures;
    hulls[3] = 11;
    if (stars_is_turin_drone_ai_attack(counts, hulls) != 0)
        ++failures;
    counts[3] = 0;
    counts[15] = 2;
    hulls[15] = 10;
    if (stars_is_turin_drone_ai_attack(counts, hulls) != 1)
        ++failures;
    printf("AI_TURIN_DRONE_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
