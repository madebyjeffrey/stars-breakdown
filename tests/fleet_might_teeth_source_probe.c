#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct FleetTeethProbe {
    int calls;
    StarsU16 owner;
    StarsU16 last_index;
    StarsHullComponent hulls[16][2];
} FleetTeethProbe;

static const StarsHullComponent *lookup(void *context, StarsU16 owner,
    StarsU16 index, StarsU16 *count)
{
    FleetTeethProbe *probe = (FleetTeethProbe *)context;
    ++probe->calls;
    probe->owner = owner;
    probe->last_index = index;
    *count = 2;
    return probe->hulls[index];
}

int main(void)
{
    StarsI16 counts[16];
    FleetTeethProbe probe;
    int failures = 0;
    memset(counts, 0, sizeof counts);
    memset(&probe, 0, sizeof probe);
    counts[2] = 1;
    probe.hulls[2][0].attributes = 0x10;
    probe.hulls[2][0].packed_damage = 0x0100;
    if (stars_fleet_might_have_teeth(counts, 0x0a00U, lookup, &probe) != 1 ||
        probe.owner != 5 || probe.last_index != 2 || probe.calls != 1)
        ++failures;
    probe.hulls[2][0].packed_damage = 0;
    if (stars_fleet_might_have_teeth(counts, 0x0a00U, lookup, &probe) != 0)
        ++failures;
    if (stars_fleet_might_have_teeth(counts, 0, lookup, &probe) != 0)
        ++failures;
    printf("FLEET_MIGHT_TEETH_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
