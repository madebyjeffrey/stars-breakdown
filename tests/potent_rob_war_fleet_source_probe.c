#include "core.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    StarsU16 counts[16];
    int failures = 0;

    memset(counts, 0, sizeof counts);
    if (stars_potent_rob_war_fleet(counts, 1, 200U) != 1)
        ++failures;
    counts[2] = 50U;
    counts[6] = 25U;
    if (stars_potent_rob_war_fleet(counts, 2, 99U) != 1)
        ++failures;
    if (stars_potent_rob_war_fleet(counts, 2, 101U) != 0)
        ++failures;
    counts[7] = 1U;
    if (stars_potent_rob_war_fleet(counts, 2, 101U) != 1)
        ++failures;

    printf("POTENT_ROB_WAR_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
