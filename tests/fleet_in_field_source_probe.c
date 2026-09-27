#include "core.h"
#include <stdio.h>

int main(void)
{
    StarsPoint fleet;
    StarsPoint thing;
    int failures = 0;

    fleet.x = 100; fleet.y = 100;
    thing.x = 103; thing.y = 104;
    if (stars_f_fleet_in_field(fleet, thing, 25U) != 0)
        ++failures;
    if (stars_f_fleet_in_field(fleet, thing, 26U) != 1)
        ++failures;
    thing.x = 100; thing.y = 100;
    if (stars_f_fleet_in_field(fleet, thing, 1U) != 1)
        ++failures;
    if (stars_f_fleet_in_field(fleet, thing, 2U) != 1)
        ++failures;

    printf("FLEET_IN_FIELD_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
