#include "core.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    StarsU16 counts[13];
    int failures = 0;
    memset(counts, 0, sizeof counts);
    counts[9] = 2;
    counts[10] = 3;
    counts[11] = 4;
    counts[12] = 5;
    if (stars_potent_is_war_fleet(counts, 1, 255) != 1)
        ++failures;
    if (stars_potent_is_war_fleet(counts, 2, 19) != 1)
        ++failures;
    if (stars_potent_is_war_fleet(counts, 2, 20) != 0)
        ++failures;
    if (stars_potent_is_war_fleet(counts, 2, 0x0113U) != 1)
        ++failures;
    counts[9] = counts[10] = counts[11] = counts[12] = 0;
    if (stars_potent_is_war_fleet(counts, 2, 1) != 0)
        ++failures;
    printf("AI_WAR_FLEET_CHECKS=5 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
