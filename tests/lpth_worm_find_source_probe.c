#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct WormProbe {
    int random_calls;
    StarsI16 random_bound;
    StarsI16 random_result;
} WormProbe;

static StarsI16 move_percent(void *context, const StarsU8 *thing)
{
    (void)context;
    (void)thing;
    return 0;
}

static StarsI16 random_value(void *context, StarsI16 bound)
{
    WormProbe *probe = (WormProbe *)context;
    ++probe->random_calls;
    probe->random_bound = bound;
    return probe->random_result;
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    StarsU16 bits = (StarsU16)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
}

int main(void)
{
    StarsU8 near_visited[18];
    StarsU8 farther_unvisited[18];
    StarsU8 wrong_type[18];
    const StarsU8 *things[3];
    WormProbe probe;
    StarsPoint source;
    const StarsU8 *result;
    int failures = 0;

    memset(near_visited, 0, sizeof(near_visited));
    memset(farther_unvisited, 0, sizeof(farther_unvisited));
    memset(wrong_type, 0, sizeof(wrong_type));
    put_word(near_visited, 0, (StarsI16)(2 << 13));
    put_word(near_visited, 2, 1); put_word(near_visited, 4, 0);
    put_word(near_visited, 10, 1);
    put_word(farther_unvisited, 0, (StarsI16)(2 << 13));
    put_word(farther_unvisited, 2, 2); put_word(farther_unvisited, 4, 0);
    put_word(wrong_type, 0, (StarsI16)(1 << 13));
    put_word(wrong_type, 2, 0); put_word(wrong_type, 4, 0);
    things[0] = near_visited; things[1] = farther_unvisited;
    things[2] = wrong_type;
    source.x = 0; source.y = 0;
    probe.random_calls = 0; probe.random_result = 0;
    result = stars_lpth_worm_find(source, 1UL, 0, things, 3,
        move_percent, random_value, &probe);
    if (result != near_visited || probe.random_calls != 1 ||
        probe.random_bound != 100)
        ++failures;
    probe.random_result = 90;
    result = stars_lpth_worm_find(source, 1UL, 0, things, 3,
        move_percent, random_value, &probe);
    if (result != 0)
        ++failures;
    printf("LPTH_WORM_FIND_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
