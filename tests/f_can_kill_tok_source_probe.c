#include "core.h"
#include <stdio.h>
#include <string.h>

static const StarsU8 *lookup(void *context, StarsU16 player,
    StarsU16 index, StarsU16 alternate, StarsU16 *selector)
{
    StarsU8 (*hulls)[0x8b] = (StarsU8 (*)[0x8b])context;
    (void)player;
    (void)alternate;
    *selector = 0U;
    return hulls[index];
}

static void set_word(StarsU8 *bytes, StarsU16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
}

int main(void)
{
    StarsToken first;
    StarsToken second;
    StarsU8 hulls[2][0x8b];
    int failures = 0;

    memset(&first, 0, sizeof first);
    memset(&second, 0, sizeof second);
    memset(hulls, 0, sizeof hulls);
    first.raw[2] = 0U;
    first.raw[4] = 0U;
    second.raw[2] = 1U;
    second.raw[4] = 1U;

    set_word(hulls[0], 0x87U, 0x1000U);
    set_word(hulls[0], 0x89U, 0x0004U);
    set_word(hulls[1], 0x87U, 0x1000U);
    set_word(hulls[1], 0x89U, 0x0005U);
    if (stars_f_can_kill_tok(&first, &second, lookup, hulls) != 0)
        ++failures;

    set_word(hulls[1], 0x89U, 0x8005U);
    if (stars_f_can_kill_tok(&first, &second, lookup, hulls) != 1)
        ++failures;

    set_word(hulls[1], 0x89U, 0x0004U);
    set_word(hulls[1], 0x87U, 0x1000U);
    if (stars_f_can_kill_tok(&first, &second, lookup, hulls) != 1)
        ++failures;
    set_word(hulls[1], 0x87U, 0x0000U);
    if (stars_f_can_kill_tok(&first, &second, lookup, hulls) != 0)
        ++failures;

    printf("F_CAN_KILL_TOK_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
