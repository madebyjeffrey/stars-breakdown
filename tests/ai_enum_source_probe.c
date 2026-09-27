#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsI16 random_value(void *context, StarsI16 limit)
{
    (void)limit;
    return *(StarsI16 *)context;
}

static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{
    (void)player; (void)stat;
    return *(StarsI16 *)context;
}

int main(void)
{
    StarsU8 ai[32];
    StarsI16 random_number = 24;
    StarsI16 stat = 7;
    int failures = 0;
    memset(ai, 0, sizeof ai);
    ai[1] = 0x81U;
    ai[10] = 1U; ai[3] = 5U;
    if (stars_enum_calc_miner_dest(1UL, 2UL, ai, 0U,
        random_value, &random_number) != (StarsI16)0x81)
        ++failures;
    random_number = 25;
    if (stars_enum_calc_miner_dest(1UL, 2UL, ai, 0U,
        random_value, &random_number) != 0 ||
        stars_enum_calc_miner_dest(4UL, 4UL, ai, 0U,
        random_value, &random_number) != 0)
        ++failures;
    if (stars_enum_calc_colonist_drop(1UL, 2UL, ai, 0U, 3,
        race_stat, &stat) != 5)
        ++failures;
    stat = 8;
    if (stars_enum_calc_colonist_drop(1UL, 2UL, ai, 0U, 3,
        race_stat, &stat) != 0)
        ++failures;
    printf("AI_ENUM_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
