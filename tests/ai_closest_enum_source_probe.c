#include "core.h"
#include <stdio.h>

typedef struct EnumRecord {
    StarsPoint point;
    StarsI16 score;
    StarsI16 accepted;
} EnumRecord;

static void get_point(void *context, const StarsU8 *record, StarsPoint *point)
{
    EnumRecord *item = (EnumRecord *)record;
    (void)context;
    *point = item->point;
}

static StarsI16 predicate(void *context, const StarsU8 *source,
    const StarsU8 *candidate)
{
    EnumRecord *item = (EnumRecord *)candidate;
    (void)context;
    (void)source;
    return item->accepted ? item->score : 0;
}

int main(void)
{
    EnumRecord source = {{0, 0}, 0, 1};
    EnumRecord first = {{3, 4}, 4, 1};
    EnumRecord second = {{1, 1}, 2, 1};
    EnumRecord rejected = {{0, 1}, 20, 0};
    EnumRecord tie_far = {{2, 0}, 2, 1};
    const StarsU8 *candidates[4];
    StarsU16 selectors[4];
    StarsAiEnumOps ops;
    StarsU16 selector;
    const StarsU8 *result;
    int failures = 0;

    candidates[0] = (const StarsU8 *)&first;
    candidates[1] = (const StarsU8 *)&rejected;
    candidates[2] = (const StarsU8 *)&second;
    candidates[3] = (const StarsU8 *)&tie_far;
    selectors[0] = 7; selectors[1] = 8; selectors[2] = 9; selectors[3] = 0;
    ops.get_point = get_point;
    ops.predicate = predicate;

    result = stars_lppl_find_closest_enum((const StarsU8 *)&source,
        candidates, 4, &ops, 0);
    if (result != (const StarsU8 *)&second)
        ++failures;
    result = stars_lppl_find_best_enum((const StarsU8 *)&source,
        candidates, 4, &ops, 0);
    if (result != (const StarsU8 *)&first)
        ++failures;

    candidates[3] = 0;
    result = stars_lpfl_find_closest_enum((const StarsU8 *)&source,
        candidates, selectors, 4, &selector, &ops, 0);
    if (result != (const StarsU8 *)&second || selector != 9)
        ++failures;
    printf("AI_CLOSEST_ENUM_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
