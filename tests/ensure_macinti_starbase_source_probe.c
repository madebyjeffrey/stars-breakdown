#include "core.h"
#include <stdio.h>

static void ensure_design(void *context, StarsI16 index, StarsI16 level)
{
    int *calls = (int *)context;
    (void)index;
    (void)level;
    ++*calls;
}

int main(void)
{
    StarsU8 status[10];
    StarsAiStarbaseDesign designs[10];
    int calls = 0;
    int failures = 0;
    int index;

    for (index = 0; index < 10; ++index) {
        status[index] = 9;
        designs[index].active = 0;
        designs[index].age = 0;
        designs[index].cost_a = 0UL;
        designs[index].cost_b = 0UL;
    }
    designs[1].active = 1; designs[1].age = 10;
    stars_ensure_macinti_starbase_designs(status, 50, designs, 10,
        1UL, 2UL, ensure_design, &calls);
    if (status[0] != 0 || status[1] != 3 || status[4] != 2 ||
            status[6] != 2 || calls != 2)
        ++failures;
    printf("ENSURE_MACINTI_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
