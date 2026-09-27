#include "core.h"
#include <stdio.h>

typedef struct DefenseProbe {
    int queued;
    int finished;
    StarsI16 first_type;
    StarsI16 first_amount;
    StarsI16 finish_value;
} DefenseProbe;

static StarsI16 add_queue_item(void *context, StarsU16 planet, StarsI16 type,
    StarsI16 amount)
{
    DefenseProbe *probe = (DefenseProbe *)context;
    (void)planet;
    if (probe->queued == 0) {
        probe->first_type = type;
        probe->first_amount = amount;
    }
    ++probe->queued;
    return 1;
}

static void finish_production(void *context, StarsU16 planet, StarsI16 value)
{
    DefenseProbe *probe = (DefenseProbe *)context;
    (void)planet;
    ++probe->finished;
    probe->finish_value = value;
}

int main(void)
{
    StarsAiQuickDefensePlanet planet;
    StarsQuickDefenseOps ops;
    DefenseProbe probe;
    int failures = 0;

    planet.id = 4; planet.resources[0] = 100; planet.resources[1] = 100;
    planet.resources[2] = 100; planet.production_threshold = 100;
    planet.current_mine_capacity = 8; planet.defense_capacity = 8;
    probe.queued = 0; probe.finished = 0; probe.first_type = 0;
    probe.first_amount = 0; probe.finish_value = 0;
    ops.add_queue_item = add_queue_item;
    ops.finish_production = finish_production;
    stars_quick_build_defenses(&planet, &ops, &probe);
    if (probe.queued != 1 || probe.first_type != 9 ||
            probe.first_amount != 8 || probe.finished != 1 ||
            probe.finish_value != 1)
        ++failures;
    printf("QUICK_BUILD_DEFENSES_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
