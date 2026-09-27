#include "core.h"
#include <stdio.h>

int main(void)
{
    int failures = 0;
    if (stars_score_from_give_take_tactic(5L, 7L, 0) != 7L ||
        stars_score_from_give_take_tactic(5L, 7L, 2) != 7L ||
        stars_score_from_give_take_tactic(5L, 7L, 1) != -5L ||
        stars_score_from_give_take_tactic(5L, 7L, 5) != -5L ||
        stars_score_from_give_take_tactic(10L, 4L, 3) != -200L ||
        stars_score_from_give_take_tactic(-10L, 4L, 4) != -1L ||
        stars_score_from_give_take_tactic(5L, 7L, 9) != 0L)
        ++failures;
    printf("SCORE_FROM_GIVE_TAKE_TACTIC_CHECKS=7 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
