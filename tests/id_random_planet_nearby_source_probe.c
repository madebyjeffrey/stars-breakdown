#include "core.h"
#include <stdio.h>

typedef struct RandomProbe {
    int calls;
    StarsI16 bounds[8];
    StarsI16 values[8];
} RandomProbe;

static StarsI16 random_value(void *context, StarsI16 bound)
{
    RandomProbe *probe = (RandomProbe *)context;
    int slot = probe->calls++;
    probe->bounds[slot] = bound;
    return probe->values[slot];
}

int main(void)
{
    StarsPoint points[3] = {{1, 0}, {2, 0}, {10, 0}};
    StarsU16 flags[3] = {0, 0x0200U, 0};
    RandomProbe probe;
    StarsPoint center;
    StarsI16 result;
    int failures = 0;

    center.x = 0; center.y = 0;
    probe.calls = 0;
    probe.values[0] = 0; probe.values[1] = 0;
    probe.values[2] = 0; probe.values[3] = 1;
    result = stars_id_random_planet_nearby(center, 3, 1,
        points, flags, 3, random_value, &probe);
    if (result != 0 || probe.calls != 4 || probe.bounds[0] != 1 ||
        probe.bounds[1] != 2 || probe.bounds[2] != 1 ||
        probe.bounds[3] != 2)
        ++failures;

    probe.calls = 0;
    probe.values[0] = 0; probe.values[1] = 0;
    result = stars_id_random_planet_nearby(center, 3, 0,
        points, flags, 3, random_value, &probe);
    if (result != 1 || probe.calls != 2)
        ++failures;
    printf("ID_RANDOM_PLANET_NEARBY_CHECKS=2 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
