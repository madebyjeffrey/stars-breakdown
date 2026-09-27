#include "core.h"
#include <stdio.h>
#include <string.h>

static void put_long(StarsU8 *bytes, StarsU16 offset, StarsU32 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
    bytes[offset + 2U] = (StarsU8)(value >> 16);
    bytes[offset + 3U] = (StarsU8)(value >> 24);
}

static StarsU32 get_long(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU32)bytes[offset] |
        ((StarsU32)bytes[offset + 1U] << 8) |
        ((StarsU32)bytes[offset + 2U] << 16) |
        ((StarsU32)bytes[offset + 3U] << 24);
}

int main(void)
{
    StarsI16 counts[2];
    StarsU8 records[294];
    int failures = 0;
    counts[0] = 2;
    counts[1] = 4;
    memset(records, 0, sizeof records);
    put_long(records + 147, 0x83U, 100);
    put_long(records + 147, 0x7fU, 200);
    stars_create_ship(counts, records, 2, 1, 5);
    if (counts[1] != 9 || get_long(records + 147, 0x83U) != 105 ||
        get_long(records + 147, 0x7fU) != 205)
        ++failures;
    stars_create_ship(counts, records, 2, 1, -2);
    if (counts[1] != 7 || get_long(records + 147, 0x83U) != 103 ||
        get_long(records + 147, 0x7fU) != 203)
        ++failures;
    if (counts[0] != 2)
        ++failures;
    printf("CREATE_SHIP_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
