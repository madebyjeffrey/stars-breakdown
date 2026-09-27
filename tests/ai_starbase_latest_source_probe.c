#include "core.h"
#include <stdio.h>
#include <string.h>

static void put_word(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[offset + 1U] = (StarsU8)(value >> 8);
}

int main(void)
{
    StarsU8 record[0x3f1];
    int failures = 0;
    memset(record, 0, sizeof record);
    put_word(record, 0x0110U, 4);
    put_word(record, 0x03efU, 5);
    if (stars_ishdef_ai_sb_latest_of(record) != 6)
        ++failures;
    put_word(record, 0x03edU, 0x0200U);
    if (stars_ishdef_ai_sb_latest_of(record) != 1)
        ++failures;
    put_word(record, 0x03edU, 0);
    put_word(record, 0x03efU, 4);
    if (stars_ishdef_ai_sb_latest_of(record) != 1)
        ++failures;
    put_word(record, 0x007dU, 7);
    put_word(record, 0x035cU, 8);
    if (stars_ishdef_ai_sb_latest(record) != 5)
        ++failures;
    put_word(record, 0x035aU, 0x0200U);
    if (stars_ishdef_ai_sb_latest(record) != 0)
        ++failures;
    put_word(record, 0x035aU, 0);
    put_word(record, 0x035cU, 7);
    if (stars_ishdef_ai_sb_latest(record) != 0)
        ++failures;
    printf("AI_STARBASE_CHECKS=6 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
