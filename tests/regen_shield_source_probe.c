#include "core.h"
#include <stdio.h>
#include <string.h>

static const StarsU8 *lookup(void *context, StarsU16 player,
    StarsU16 index, StarsU16 alternate, StarsU16 *selector)
{
    StarsU8 *hulls = (StarsU8 *)context;
    (void)player;
    (void)index;
    *selector = alternate;
    return hulls;
}

static StarsU32 shield_power(void *context, const StarsU8 *hull,
    StarsU16 player)
{
    (void)context;
    (void)hull;
    return 100U + player * 10U;
}

int main(void)
{
    StarsToken token;
    StarsU8 hull[4];
    int failures = 0;

    memset(&token, 0, sizeof token);
    token.raw[2] = 2;
    token.raw[4] = 0x12U;
    token.raw[0x11] = 5;
    stars_regen_shield(&token, lookup, shield_power, hull);
    if (token.raw[0x11] != 17U || token.raw[0x12] != 0U)
        ++failures;

    token.raw[0x11] = 250U;
    token.raw[0x12] = 0U;
    stars_regen_shield(&token, lookup, shield_power, hull);
    if (token.raw[0x11] != 120U || token.raw[0x12] != 0U)
        ++failures;

    token.raw[0x11] = 0U;
    token.raw[0x12] = 9U;
    stars_regen_shield(&token, lookup, shield_power, hull);
    if (token.raw[0x11] != 120U || token.raw[0x12] != 0U)
        ++failures;

    printf("REGEN_SHIELD_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
