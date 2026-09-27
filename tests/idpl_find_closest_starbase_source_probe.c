#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsI16 is_big(void *context, const StarsU8 *planet)
{
    (void)context;
    return planet[0] == 2U;
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    StarsU16 bits = (StarsU16)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
}

int main(void)
{
    StarsU8 first[56];
    StarsU8 second[56];
    StarsU8 foreign[56];
    const StarsU8 *planets[3];
    StarsPoint points[3];
    StarsPoint source;
    StarsI16 result;
    int failures = 0;

    memset(first, 0, sizeof(first));
    memset(second, 0, sizeof(second));
    memset(foreign, 0, sizeof(foreign));
    put_word(first, 0, 1); put_word(first, 2, 4); put_word(first, 4, 0x0200);
    put_word(second, 0, 2); put_word(second, 2, 4); put_word(second, 4, 0x0200);
    put_word(foreign, 0, 3); put_word(foreign, 2, 5); put_word(foreign, 4, 0x0200);
    planets[0] = first; planets[1] = second; planets[2] = foreign;
    points[0].x = 5; points[0].y = 0;
    points[1].x = 2; points[1].y = 0;
    points[2].x = 1; points[2].y = 0;
    source.x = 0; source.y = 0;
    result = stars_idpl_find_closest_starbase(source, planets,
        points, 3, 4, 0, is_big, 0);
    if (result != 2)
        ++failures;
    result = stars_idpl_find_closest_starbase(source, planets,
        points, 3, 4, 1, is_big, 0);
    if (result != 2)
        ++failures;
    printf("IDPL_FIND_CLOSEST_STARBASE_CHECKS=2 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
