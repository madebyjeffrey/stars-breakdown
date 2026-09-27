#include "core.h"
#include <stdio.h>
#include <string.h>

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 get_word(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU16)(bytes[offset] | ((StarsU16)bytes[offset + 1U] << 8));
}

int main(void)
{
    StarsU8 things[54];
    int failures = 0;
    memset(things, 0, sizeof things);
    put_word(things, 0x0eU, 0x1234U);
    put_word(things, 0x12U, 0x2000U);
    put_word(things, 0x12U + 0x0eU, 0x5678U);
    put_word(things, 0x24U + 0x0eU, 0x9abcu);
    stars_unmark_mine_fields(things, 3);
    if (get_word(things, 0x0eU) != 0 ||
        get_word(things, 0x12U + 0x0eU) != 0x5678U ||
        get_word(things, 0x24U + 0x0eU) != 0)
        ++failures;
    printf("UNMARK_MINE_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
