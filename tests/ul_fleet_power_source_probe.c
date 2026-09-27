#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct HullState {
    StarsU8 hull[16][0x93];
    int lookups;
} HullState;

static const StarsU8 *lookup(void *context, StarsI16 player, StarsU16 design)
{
    HullState *state = (HullState *)context;
    (void)player;
    ++state->lookups;
    return state->hull[design];
}

static void put_word(StarsU8 *p, StarsU16 value)
{
    p[0] = (StarsU8)value; p[1] = (StarsU8)(value >> 8);
}

static void put_long(StarsU8 *p, StarsU32 value)
{
    p[0] = (StarsU8)value; p[1] = (StarsU8)(value >> 8);
    p[2] = (StarsU8)(value >> 16); p[3] = (StarsU8)(value >> 24);
}

int main(void)
{
    StarsU8 fleet[44];
    HullState state;
    StarsU32 result;
    int failures = 0;

    memset(fleet, 0, sizeof(fleet));
    memset(&state, 0, sizeof(state));
    put_word(fleet + 2, 3U);
    put_word(fleet + 12, 3U); put_word(fleet + 14, 2U);
    put_long(state.hull[0] + 0x87, 100UL);
    put_long(state.hull[1] + 0x87, 200UL);
    result = stars_ul_fleet_power(fleet, lookup, &state);
    if (result != 700UL || state.lookups != 2)
        ++failures;

    memset(fleet, 0, sizeof(fleet));
    memset(&state, 0, sizeof(state));
    put_word(fleet + 12, 2U); put_word(fleet + 14, 1U);
    put_long(state.hull[0] + 0x87, 0x70000000UL);
    put_long(state.hull[1] + 0x87, 1UL);
    result = stars_ul_fleet_power(fleet, lookup, &state);
    if (result != 0xe0000000UL || state.lookups != 1)
        ++failures;
    printf("UL_FLEET_POWER_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
