#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsU16 word_at(const StarsU8 *bytes, StarsI16 offset)
{
    return (StarsU16)(bytes[offset] |
        ((StarsU16)bytes[offset + 1] << 8));
}

int main(void)
{
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE * 2U];
    StarsU8 before[sizeof plord];
    StarsI16 failures;

    memset(plord, 0xa5, sizeof plord);
    plord[3] = 2U;
    memcpy(before, plord, sizeof plord);
    failures = stars_ai_repair_first_order(plord) != 1;
    failures += plord[3] != before[3];
    failures += (word_at(plord, 10) & 0x000fU) != 6U;
    failures += (word_at(plord, 10) & 0xfff0U) !=
        (word_at(before, 10) & 0xfff0U);
    failures += word_at(plord, 12) != 5U;
    failures += word_at(plord, 14) != 5U;
    failures += memcmp(plord + 4, before + 4, 6U) != 0;
    failures += memcmp(plord + 16, before + 16, sizeof plord - 16U) != 0;
    failures += stars_ai_repair_first_order((StarsU8 *)0) != 0;
    printf("AI_PLORD_REPAIR_SOURCE_CHECKS=9 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
