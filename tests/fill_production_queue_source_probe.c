#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProductionProbe {
    int events;
    int selections;
    int initializations;
    int fills;
    int finishes;
    StarsI16 selected_id;
    StarsU16 scratch_size;
    StarsI16 finish_value;
} ProductionProbe;

static void change_selection(void *context, StarsI16 kind, StarsI16 id)
{
    ProductionProbe *probe = (ProductionProbe *)context;
    ++probe->events;
    ++probe->selections;
    if (kind != 1) probe->selected_id = -2;
    else probe->selected_id = id;
}

static void initialize(void *context, StarsU8 *scratch, StarsU16 size)
{
    ProductionProbe *probe = (ProductionProbe *)context;
    ++probe->events;
    ++probe->initializations;
    probe->scratch_size = size;
    scratch[0] = 0;
}

static StarsI16 fill(void *context, const StarsU8 *planet)
{
    ProductionProbe *probe = (ProductionProbe *)context;
    ++probe->events;
    ++probe->fills;
    return (StarsI16)planet[2];
}

static void finish(void *context, StarsI16 value)
{
    ProductionProbe *probe = (ProductionProbe *)context;
    ++probe->events;
    ++probe->finishes;
    probe->finish_value = value;
}

int main(void)
{
    StarsU8 first[4] = {7, 0, 3, 0};
    StarsU8 second[4] = {9, 0, 5, 0};
    const StarsU8 *planets[3];
    StarsProductionQueueOps ops;
    ProductionProbe probe;
    int failures = 0;
    planets[0] = first;
    planets[1] = second;
    planets[2] = 0;
    ops.change_selection = change_selection;
    ops.initialize = initialize;
    ops.fill = fill;
    ops.finish = finish;
    memset(&probe, 0, sizeof probe);
    stars_fill_production_queue(planets, 3, &ops, &probe);
    if (probe.events != 8 || probe.selections != 2 ||
        probe.initializations != 2 || probe.fills != 2 || probe.finishes != 2 ||
        probe.selected_id != 9 || probe.scratch_size != 256 ||
        probe.finish_value != 5)
        ++failures;
    printf("FILL_PRODUCTION_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
