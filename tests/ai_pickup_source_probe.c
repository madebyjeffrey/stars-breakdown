#include "core.h"
#include <stdio.h>
#include <string.h>

static void set_planet(StarsEnumPickupPlanet *planet, StarsI16 player,
    StarsU16 flags, StarsU16 threshold, StarsI16 count)
{
    memset(planet, 0, sizeof *planet);
    planet->player = player;
    planet->overlap_flags = flags;
    planet->mineral_threshold = threshold;
    planet->pickup_count = count;
}

int main(void)
{
    StarsEnumPickupPlanet source;
    StarsEnumPickupPlanet test;
    int failures = 0;
    set_planet(&source, 3, 0, 0, 0);
    set_planet(&test, 3, 0x0200U, 0x0899U, 0);
    if (stars_enum_pick_up(&source, &test, 3) != 1)
        ++failures;
    test.mineral_threshold = 0x0898U;
    if (stars_enum_pick_up(&source, &test, 3) != 0)
        ++failures;
    test.mineral_threshold = 0;
    test.pickup_count = 1;
    if (stars_enum_pick_up(&source, &test, 3) != 1)
        ++failures;
    test.pickup_count = -1;
    if (stars_enum_pick_up(&source, &test, 3) != 0)
        ++failures;
    test.pickup_count = 1;
    test.overlap_flags = 0;
    if (stars_enum_pick_up(&source, &test, 3) != 0)
        ++failures;
    if (stars_enum_pick_up(&source, &test, 4) != 0)
        ++failures;
    printf("AI_PICKUP_CHECKS=6 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
