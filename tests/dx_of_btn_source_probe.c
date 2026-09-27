#include "core.h"
#include <stdio.h>

int main(void)
{
    int failures = 0;
    if (stars_dx_of_btn(-1, 8) != 6 || stars_dx_of_btn(-2, 8) != 2)
        ++failures;
    if (stars_dx_of_btn(-3, 15) != 60 || stars_dx_of_btn(-3, 16) != 70)
        ++failures;
    if (stars_dx_of_btn(-4, 8) != 0)
        ++failures;
    if (stars_dx_of_btn(13, 8) != 11 || stars_dx_of_btn(15, 8) != 11)
        ++failures;
    if (stars_dx_of_btn(14, 8) != 29)
        ++failures;
    printf("DX_BUTTON_CHECKS=5 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
