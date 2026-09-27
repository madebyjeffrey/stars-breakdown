#include "core.h"
#include <stdio.h>

int main(void)
{
    int failures = 0;
    if (stars_dz_from_brc_brc(0x12U, 0x45U) != 3)
        ++failures;
    if (stars_dz_from_brc_brc(0xa5U, 0x21U) != 8)
        ++failures;
    if (stars_dz_from_brc_brc(0xffU, 0x00U) != 15)
        ++failures;
    if (stars_dz_from_brc_brc(0x34U, 0x34U) != 0)
        ++failures;
    printf("DZ_BRC_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
