#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsU16 word_at(const StarsU8 *bytes, StarsI16 offset)
{
    return (StarsU16)(bytes[offset] |
        ((StarsU16)bytes[offset + 1] << 8));
}

static void put_word(StarsU8 *bytes, StarsI16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1] = (StarsU8)(value >> 8);
}

int main(void)
{
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE * 2U];
    StarsU8 before[sizeof plord];
    StarsU16 original;
    StarsI16 failures;

    memset(plord, 0xa5, sizeof plord);
    plord[3] = 3U;
    original = 0xb2c7U;
    put_word(plord, 10, original);
    memcpy(before, plord, sizeof plord);
    failures = !stars_ai_set_first_order_kind(plord, 5U);
    failures += plord[3] != before[3];
    failures += (word_at(plord, 10) & 0x000fU) != 5U;
    failures += (word_at(plord, 10) & 0xfff0U) !=
        (original & 0xfff0U);
    failures += memcmp(plord, before, 10U) != 0;
    failures += memcmp(plord + 12, before + 12, sizeof plord - 12U) != 0;

    failures += !stars_ai_set_first_order_kind(plord, 6U);
    failures += (word_at(plord, 10) & 0x000fU) != 6U;
    failures += (word_at(plord, 10) & 0xfff0U) !=
        (original & 0xfff0U);
    failures += stars_ai_set_first_order_kind(plord, 16U) != 0;
    failures += word_at(plord, 10) !=
        (StarsU16)((original & 0xfff0U) | 6U);
    failures += stars_ai_set_first_order_kind((StarsU8 *)0, 5U) != 0;

    printf("AI_PLORD_FIRST_KIND_SOURCE_CHECKS=12 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
