#include "core.h"
#include <stdio.h>

typedef struct UpgradeProbe { int calls; StarsI16 item; } UpgradeProbe;

static StarsI16 random_value(void *context, StarsI16 bound)
{
    (void)context;
    (void)bound;
    return 0;
}

static StarsI16 queue_upgrade(void *context, StarsI16 item)
{
    UpgradeProbe *probe = (UpgradeProbe *)context;
    ++probe->calls;
    probe->item = item;
    return 1;
}

int main(void)
{
    StarsAiUpgradeStarbase starbase;
    StarsUpgradeStarbaseOps ops;
    UpgradeProbe probe;
    int failures = 0;
    starbase.family = 1; starbase.recycle_state = 0;
    starbase.resource_values[0] = 200; starbase.resource_values[1] = 200;
    starbase.resource_values[2] = 200; starbase.planet_capacity = 20;
    probe.calls = 0; probe.item = 0;
    ops.random_value = random_value; ops.queue_upgrade = queue_upgrade;
    if (stars_f_upgrade_ai_starbase(&starbase, 1, 50, 1, &ops, &probe) != 1 ||
            probe.calls != 1 || probe.item != 18)
        ++failures;
    printf("UPGRADE_AI_STARBASE_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
