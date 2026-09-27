#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct FleetHullState {
    StarsHullComponent (*hulls)[16][2];
    StarsU16 (*groups)[16];
} FleetHullState;

static const StarsHullComponent *lookup(void *context, StarsU16 owner,
    StarsU16 index, StarsU16 *count)
{
    FleetHullState *state = (FleetHullState *)context;
    (void)owner;
    *count = 2U;
    return state->hulls[0][index];
}

static StarsU16 hull_group(void *context, StarsU16 owner, StarsU16 index)
{
    FleetHullState *state = (FleetHullState *)context;
    (void)owner;
    return state->groups[0][index];
}

int main(void)
{
    StarsI16 counts[16];
    StarsHullComponent hulls[1][16][2];
    StarsU16 groups[1][16];
    FleetHullState state;
    int failures = 0;

    memset(counts, 0, sizeof counts);
    memset(hulls, 0, sizeof hulls);
    memset(groups, 0, sizeof groups);
    state.hulls = hulls;
    state.groups = groups;
    counts[1] = 1;
    hulls[0][1][0].attributes = 0x0010U;
    hulls[0][1][0].packed_damage = 0x0100U;
    groups[0][1] = 7U;
    if (stars_fleet_has_teeth(counts, 2U, lookup, hull_group, &state) != 1)
        ++failures;
    groups[0][1] = 6U;
    if (stars_fleet_has_teeth(counts, 2U, lookup, hull_group, &state) != 0)
        ++failures;
    groups[0][1] = 7U;
    hulls[0][1][0].packed_damage = 0U;
    if (stars_fleet_has_teeth(counts, 2U, lookup, hull_group, &state) != 0)
        ++failures;

    printf("FLEET_HAS_TEETH_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
