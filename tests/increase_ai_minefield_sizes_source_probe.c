#include "core.h"
#include <stdio.h>

static void put_word(StarsU8 *raw, StarsU16 value)
{
    raw[0] = (StarsU8)value;
    raw[1] = (StarsU8)(value >> 8);
}

static void put_long(StarsU8 *raw, StarsI32 value)
{
    raw[0] = (StarsU8)value;
    raw[1] = (StarsU8)(value >> 8);
    raw[2] = (StarsU8)(value >> 16);
    raw[3] = (StarsU8)(value >> 24);
}

static StarsI32 get_long(const StarsU8 *raw)
{
    return (StarsI32)((StarsU32)raw[0] | ((StarsU32)raw[1] << 8) |
        ((StarsU32)raw[2] << 16) | ((StarsU32)raw[3] << 24));
}

int main(void)
{
    StarsAiMineThing things[3];
    int failures = 0;

    put_word(things[0].raw, 0U); put_long(things[0].raw + 6, 10L);
    put_word(things[1].raw, 0x2000U); put_long(things[1].raw + 6, 10L);
    put_word(things[2].raw, 0U); put_long(things[2].raw + 6, 25L);
    stars_increase_ai_minefield_sizes(things, 3U);
    if (get_long(things[0].raw + 6) != 9L ||
        get_long(things[1].raw + 6) != 10L ||
        get_long(things[2].raw + 6) != 25L)
        ++failures;
    printf("INCREASE_AI_MINEFIELD_SIZES_CHECKS=3 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
