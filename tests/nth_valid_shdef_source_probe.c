#include "core.h"
#include <stdio.h>
#include <string.h>

static void mark_reserved(StarsU8 *design)
{ design[0x7c] = (StarsU8)(design[0x7c] | 0x02U); }

int main(void)
{
    StarsU8 normal[3 * 0x93];
    StarsU8 starbase[3 * 0x93];
    const StarsU8 *result;
    int failures = 0;

    memset(normal, 0, sizeof(normal)); memset(starbase, 0, sizeof(starbase));
    normal[0] = 1; normal[0x93] = 2; normal[2 * 0x93] = 3;
    mark_reserved(normal + 0x93);
    starbase[0] = 4; starbase[0x93] = 5; starbase[2 * 0x93] = 6;
    mark_reserved(starbase);
    result = stars_nth_valid_shdef(normal, 3U, 1, starbase, 3U, 0);
    if (result != normal + 2 * 0x93)
        ++failures;
    result = stars_nth_valid_shdef(normal, 3U, 0, starbase, 3U, 1);
    if (result != starbase + 0x93)
        ++failures;
    result = stars_nth_valid_shdef(normal, 3U, 2, starbase, 3U, 0);
    if (result != 0)
        ++failures;
    printf("NTH_VALID_SHDEF_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
