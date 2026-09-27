#include "core.h"
#include <stdio.h>
#include <string.h>

static void set_word(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[offset + 1U] = (StarsU8)(value >> 8);
}

static void set_long(StarsU8 *record, StarsU16 offset, StarsU32 value)
{
    set_word(record, offset, (StarsU16)value);
    set_word(record, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

int main(void)
{
    StarsU8 score[0x18];
    int failures = 0;

    memset(score, 0, sizeof score);
    set_word(score, 0x0cU, 12U);
    set_word(score, 0x0eU, 13U);
    set_word(score, 0x10U, 0xa123U);
    set_word(score, 0x12U, 0x2123U);
    set_word(score, 0x14U, 0x0123U);
    set_word(score, 0x16U, 15U);
    set_long(score, 0x04U, 0x12345678UL);
    set_long(score, 0x08U, 0x89abcdefUL);
    if (stars_fetch_score_x_val(score, 0) != 12L ||
        stars_fetch_score_x_val(score, 1) != 13L ||
        stars_fetch_score_x_val(score, 2) != 0x48c00L ||
        stars_fetch_score_x_val(score, 3) != 0x48cL ||
        stars_fetch_score_x_val(score, 4) != 0x0123L ||
        stars_fetch_score_x_val(score, 5) != 15L ||
        stars_fetch_score_x_val(score, 6) != 0x12345678L ||
        (StarsU32)stars_fetch_score_x_val(score, 7) != 0x89abcdefUL)
        ++failures;
    if (stars_fetch_score_x_val(score, 8) != 0L)
        ++failures;

    printf("FETCH_SCORE_X_VAL_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
