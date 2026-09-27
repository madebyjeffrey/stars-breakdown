#include "core.h"
#include <stdio.h>

int main(void)
{
    static const StarsI16 buttons[] = {-1, 0, 0x0d, -2, 3};
    StarsPoint point;
    int failures = 0;

    point.x = 4; point.y = 4;
    if (stars_itb_from_ppt(&point, buttons, 5U, 15) != -1 ||
        point.x != 4 || point.y != 4)
        ++failures;

    point.x = 11; point.y = 31;
    if (stars_itb_from_ppt(&point, buttons, 5U, 15) != 0 ||
        point.x != 10 || point.y != 4)
        ++failures;

    point.x = 40; point.y = 4;
    if (stars_itb_from_ppt(&point, buttons, 5U, 15) != 0x0d ||
        point.x != 39 || point.y != 4)
        ++failures;

    point.x = 51; point.y = 4;
    if (stars_itb_from_ppt(&point, buttons, 5U, 15) != -2 ||
        point.x != 50 || point.y != 4)
        ++failures;

    point.x = 99; point.y = 4;
    if (stars_itb_from_ppt(&point, buttons, 5U, 15) != -1)
        ++failures;

    point.x = 5; point.y = 4;
    if (stars_itb_from_ppt(&point, buttons, 0U, 15) != -1)
        ++failures;

    printf("ITB_FROM_PPT_CHECKS=6 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
