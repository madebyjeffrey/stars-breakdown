#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct StarbaseProbe {
    int calls;
    StarsI16 design[10];
    StarsI16 kind[10];
} StarbaseProbe;

static void create_starbase(void *context, StarsI16 design, StarsI16 kind)
{
    StarbaseProbe *probe = (StarbaseProbe *)context;
    probe->design[probe->calls] = design;
    probe->kind[probe->calls] = kind;
    ++probe->calls;
}

int main(void)
{
    StarsU8 capabilities[3];
    StarsAiStarbaseDesign designs[10];
    StarbaseProbe probe;
    int failures = 0;

    memset(capabilities, 0, sizeof(capabilities));
    memset(designs, 0, sizeof(designs));
    probe.calls = 0;
    capabilities[0] = 1; capabilities[1] = 1; capabilities[2] = 1;
    stars_ensure_ai_starbase_designs(20, capabilities, designs,
        create_starbase, &probe);
    if (probe.calls != 4 || probe.design[0] != 2 || probe.design[1] != 4 ||
            probe.design[2] != 1 || probe.design[3] != 3)
        ++failures;
    probe.calls = 0;
    designs[0].age = 10; designs[2].age = 20; designs[4].age = 5;
    designs[1].age = 10; designs[3].age = 20; designs[6].age = 5;
    stars_ensure_ai_starbase_designs(70, capabilities, designs,
        create_starbase, &probe);
    if (probe.calls != 9 || probe.design[4] != 5 || probe.design[5] != 7 ||
            probe.design[6] != 9 || probe.design[7] != 6 ||
            probe.design[8] != 8)
        ++failures;
    printf("ENSURE_AI_STARBASE_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
