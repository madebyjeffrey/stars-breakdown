#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsI16 random_zero(void *context, StarsI16 count)
{
    (void)context; (void)count;
    return 0;
}

static StarsI16 random_one(void *context, StarsI16 count)
{
    (void)context; (void)count;
    return 1;
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    StarsU16 bits = (StarsU16)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
}

int main(void)
{
    StarsU8 fleet[48];
    const StarsU8 *fleets[1];
    StarsU8 available[16];
    StarsU16 hulls[16];
    StarsU32 built[16];
    StarsU8 player_flags[2];
    StarsI16 colonies[2];
    StarsI16 count;
    int failures = 0;

    memset(fleet, 0, sizeof(fleet));
    memset(available, 0, sizeof(available));
    memset(hulls, 0, sizeof(hulls));
    memset(built, 0, sizeof(built));
    memset(player_flags, 0, sizeof(player_flags));
    memset(colonies, 0, sizeof(colonies));
    put_word(fleet, 2, 4); put_word(fleet, 12, 1);
    fleets[0] = fleet; available[0] = 1; hulls[0] = 14;
    if (stars_f_should_we_build_colonizers(4, 30, 1, 2, 20, fleets, 1,
            available, hulls, built, player_flags, colonies, 2,
            random_zero, 0, &count) != 1 || count != 1)
        ++failures;
    if (stars_f_should_we_build_colonizers(4, 1, 0, 2, 20, fleets, 1,
            available, hulls, built, player_flags, colonies, 2,
            random_one, 0, &count) != 0)
        ++failures;
    built[0] = 100;
    if (stars_f_should_we_build_colonizers(4, 30, 1, 2, 20, fleets, 1,
            available, hulls, built, player_flags, colonies, 2,
            random_one, 0, &count) != 0)
        ++failures;
    printf("SHOULD_BUILD_COLONIZERS_CHECKS=3 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
