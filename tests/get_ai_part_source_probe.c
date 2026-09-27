#include "core.h"
#include <stdio.h>

static StarsI16 lookup_part(void *context, StarsU16 tech_group,
    StarsU16 attributes, StarsAiPart *part)
{
    (void)context;
    if (tech_group != 4U || attributes != 2U)
        return 0;
    part->component = (const StarsU8 *)"OK";
    return 1;
}

int main(void)
{
    const StarsU8 counts[2] = { 0, 1 };
    const StarsU16 packed[2] = { (StarsU16)(2U | (4U << 4) |
        (3U << 9)), 0 };
    StarsAiPart part;
    int failures = 0;
    if (stars_f_get_ai_part(1, counts, 2, packed, lookup_part, &part, 0)
            != 1)
        ++failures;
    if (part.tech_group != 4U || part.attributes != 2U ||
            part.component[0] != 'O')
        ++failures;
    if (stars_f_get_ai_part(0, counts, 2, packed, lookup_part, &part, 0)
            != 0)
        ++failures;
    printf("GET_AI_PART_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
