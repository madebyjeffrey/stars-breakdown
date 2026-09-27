#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ResourcesProbe {
    int estimate_calls;
    StarsI16 estimate_player;
    StarsI16 estimate_flag;
    StarsI16 resource_player;
    StarsI16 percent_player;
} ResourcesProbe;

static void estimate(void *context, const StarsU8 *planet, StarsI32 *values,
    StarsI16 player, StarsI16 flag)
{
    ResourcesProbe *probe = (ResourcesProbe *)context;
    (void)planet;
    ++probe->estimate_calls;
    probe->estimate_player = player;
    probe->estimate_flag = flag;
    values[0] = 100L;
    values[1] = 200L;
    values[2] = 300L;
}

static StarsI16 resources_at_planet(void *context, const StarsU8 *planet,
    StarsI16 player)
{
    ResourcesProbe *probe = (ResourcesProbe *)context;
    (void)planet;
    probe->resource_player = player;
    return 1000;
}

static StarsI16 research_percent(void *context, StarsI16 player)
{
    ResourcesProbe *probe = (ResourcesProbe *)context;
    probe->percent_player = player;
    return 10;
}

static void put_long(StarsU8 *bytes, StarsU16 offset, StarsI32 value)
{
    StarsU32 bits = (StarsU32)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
    bytes[offset + 2U] = (StarsU8)(bits >> 16);
    bytes[offset + 3U] = (StarsU8)(bits >> 24);
}

int main(void)
{
    StarsU8 planet[56];
    StarsI32 resources[4];
    StarsGetResourcesAvailableOps ops;
    ResourcesProbe probe;
    int failures = 0;

    memset(planet, 0, sizeof(planet));
    put_long(planet, 0x1cU, 10L);
    put_long(planet, 0x20U, -2L);
    put_long(planet, 0x24U, 3L);
    memset(&probe, 0, sizeof(probe));
    ops.estimate_minerals = estimate;
    ops.resources_at_planet = resources_at_planet;
    ops.research_percent = research_percent;
    stars_get_resources_available(planet, resources, 4, 0UL, &ops, &probe);
    if (resources[0] != 110L || resources[1] != 198L ||
        resources[2] != 303L || resources[3] != 900L ||
        probe.estimate_calls != 1 || probe.estimate_player != -1 ||
        probe.estimate_flag != 0 || probe.resource_player != 4 ||
        probe.percent_player != 4)
        ++failures;

    stars_get_resources_available(planet, resources, 4, 0x00800000UL,
        &ops, &probe);
    if (resources[3] != 1000L)
        ++failures;
    printf("GET_RESOURCES_AVAILABLE_CHECKS=2 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
