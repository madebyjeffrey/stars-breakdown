#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI16 is_set(const StarsMessageState *state, StarsI16 id)
{
    return (StarsI16)((state->filtered[(StarsU16)id >> 3] >>
        ((StarsU16)id & 7U)) & 1U);
}

static StarsI16 range_set(const StarsMessageState *state, StarsI16 first,
    StarsI16 last)
{
    StarsI16 id;
    for (id = first; id <= last; ++id)
        if (!is_set(state, id)) return 0;
    return 1;
}

int main(void)
{
    StarsMessageState state;
    int failures = 0;
    memset(&state, 0, sizeof state);

    stars_set_filtering_group(&state, 0x20, 1);
    if (!is_set(&state, 0x20)) ++failures;
    stars_set_filtering_group(&state, 0x20, 0);
    if (is_set(&state, 0x20)) ++failures;

    stars_set_filtering_group(&state, 0x35, 1);
    if (!is_set(&state, 0x35) || !is_set(&state, 0x36)) ++failures;

    memset(&state, 0, sizeof state);
    stars_set_filtering_group(&state, 0x2c, 1);
    if (!range_set(&state, 0x2b, 0x2e)) ++failures;
    stars_set_filtering_group(&state, 0x62, 0);
    if (range_set(&state, 0x60, 0x64)) ++failures;

    memset(&state, 0, sizeof state);
    stars_set_filtering_group(&state, 0x93, 1);
    if (!range_set(&state, 0x91, 0xa8)) ++failures;

#ifndef TARGET_NATIVE
    printf("SET_FILTER_GROUP_CHECKS=5 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
