#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct StatusProbe {
    int recycled;
    StarsI16 index;
} StatusProbe;

static void recycle(void *context, StarsI16 index)
{
    StatusProbe *probe = (StatusProbe *)context;
    ++probe->recycled; probe->index = index;
}

int main(void)
{
    StarsAiShdefStatus designs[4];
    StarsU8 old_designs[4];
    StarsI16 latest;
    StatusProbe probe;
    StarsI16 result;
    int failures = 0;

    memset(designs, 0, sizeof(designs));
    memset(old_designs, 0, sizeof(old_designs));
    designs[0].existing = 3; designs[0].turn = 2;
    designs[1].existing = 0; designs[1].turn = 1;
    designs[2].existing = 40000UL; designs[2].turn = 7;
    designs[3].active = 1; designs[3].existing = 100; designs[3].turn = 9;
    probe.recycled = 0; probe.index = -1;
    result = stars_check_ai_shdef_status(0, 3, 5, &latest, old_designs, 10,
        designs, recycle, &probe);
    if (result != 32000 || latest != 2 || probe.recycled != 1 ||
            probe.index != 1 || old_designs[0] != 1 || old_designs[2] != 0)
        ++failures;
    printf("CHECK_AI_SHDEF_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
