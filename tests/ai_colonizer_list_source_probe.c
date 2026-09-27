#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsI16 random_two(void *context, StarsI16 count)
{
    (void)context;
    return count == 2 ? 1 : 0;
}

static StarsI16 random_list(void *context, StarsI16 count)
{
    (void)context;
    return count > 1 ? (StarsI16)(count - 1) : 0;
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    StarsU16 bits = (StarsU16)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
}

int main(void)
{
    StarsU8 status[32];
    StarsPoint points[2];
    StarsU8 first[8];
    StarsU8 second[8];
    StarsU8 third[8];
    const StarsU8 *planets[3];
    const StarsU8 *out[3];
    StarsU16 count;
    StarsPoint source;
    int failures = 0;

    memset(status, 0, sizeof(status));
    source.x = 0; source.y = 0;
    points[0].x = 4; points[0].y = 0;
    status[0x0d] = 0;
    if (stars_f_should_planet_build_colonizer(source, 59, points, status, 1,
            random_two, 0) != 1)
        ++failures;
    if (stars_f_should_planet_build_colonizer(source, 60, points, status, 1,
            random_two, 0) != 1)
        ++failures;
    points[0].x = 1000; points[0].y = 1000;
    if (stars_f_should_planet_build_colonizer(source, 60, points, status, 1,
            random_two, 0) != 0)
        ++failures;

    memset(first, 0, sizeof(first));
    memset(second, 0, sizeof(second));
    memset(third, 0, sizeof(third));
    put_word(first, 2, 4);
    put_word(second, 2, 3);
    put_word(third, 2, 4);
    planets[0] = first; planets[1] = second; planets[2] = third;
    stars_init_random_planet_list(planets, 3, 4, 0, out, &count,
        random_list, 0);
    if (count != 2 || out[0] != third || out[1] != first)
        ++failures;
    stars_init_random_planet_list(planets, 3, 4, 1, out, &count,
        random_list, 0);
    if (count != 2 || out[0] != first || out[1] != third)
        ++failures;
    printf("AI_COLONIZER_LIST_CHECKS=5 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
