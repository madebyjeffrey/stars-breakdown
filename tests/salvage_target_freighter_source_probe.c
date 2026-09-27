#include "core.h"
#include <stdio.h>

typedef struct SalvageProbe {
    int transfers;
    StarsI16 resource;
    StarsI16 amount;
} SalvageProbe;

static void transfer(void *context, StarsI16 thing, StarsI16 fleet,
    StarsI16 resource, StarsI16 amount)
{
    SalvageProbe *probe = (SalvageProbe *)context;
    (void)thing; (void)fleet;
    ++probe->transfers; probe->resource = resource; probe->amount = amount;
}

int main(void)
{
    StarsAiFreighterThing things[2];
    StarsPoint fleet_point;
    SalvageProbe probe;
    StarsI16 best_index = -1;
    int failures = 0;

    things[0].id = 3; things[0].flags = (StarsU16)(1U << 13);
    things[0].point.x = 0; things[0].point.y = 0;
    things[0].destination_class = 0; things[0].cargo[0] = 9;
    things[0].cargo[1] = 2; things[0].cargo[2] = 0;
    things[1].id = 4; things[1].flags = (StarsU16)(1U << 13);
    things[1].point.x = 25; things[1].point.y = 0;
    things[1].destination_class = 0; things[1].cargo[0] = 100;
    things[1].cargo[1] = 0; things[1].cargo[2] = 0;
    fleet_point.x = 0; fleet_point.y = 0;
    probe.transfers = 0; probe.resource = -1; probe.amount = 0;
    if (stars_f_salvage_target_freighter(things, 2, fleet_point, 7, 1, 0,
            0, 100, 0, -1, &best_index, transfer, &probe) != 1 ||
            probe.transfers != 3 || probe.resource != 1 || probe.amount != 2)
        ++failures;
    if (best_index != 1)
        ++failures;
    printf("SALVAGE_FREIGHTER_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
