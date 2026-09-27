#include "core.h"
#include <stdio.h>

int main(void)
{
    int failures = 0;

    if (stars_dxy_from_spd_round(0U, 0) != 1)
        ++failures;
    if (stars_dxy_from_spd_round(4U, 1) != 1)
        ++failures;
    if (stars_dxy_from_spd_round(5U, 2) != 1)
        ++failures;
    if (stars_dxy_from_spd_round(5U, 1) != 2)
        ++failures;
    if (stars_dxy_from_spd_round(6U, 0) != 2)
        ++failures;
    if (stars_dxy_from_spd_round(7U, 0) != 3)
        ++failures;
    if (stars_dxy_from_spd_round(7U, 1) != 2)
        ++failures;
    if (stars_dxy_from_spd_round(65535U, 0) != 16385)
        ++failures;

    printf("DXY_FROM_SPD_ROUND_CHECKS=8 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
