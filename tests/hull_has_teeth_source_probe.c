#include "core.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    StarsHullComponent components[3];
    int failures = 0;
    memset(components, 0, sizeof components);
    components[0].attributes = 0x0010U;
    components[0].packed_damage = 0x0100U;
    if (stars_hull_has_teeth(components, 3) != 1)
        ++failures;
    components[0].packed_damage = 0x00ffU;
    components[1].attributes = 0x0008U;
    components[1].packed_damage = 0x0100U;
    if (stars_hull_has_teeth(components, 3) != 0)
        ++failures;
    components[2].attributes = 0x0020U;
    components[2].packed_damage = 0xff01U;
    if (stars_hull_has_teeth(components, 3) != 1)
        ++failures;
    if (stars_hull_has_teeth(components, 0) != 0)
        ++failures;
    printf("HULL_TEETH_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
