#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct UnknownProbe {
    int prepared;
    int random_calls;
    StarsU32 worm_distance;
    StarsI16 random_result;
    StarsU8 wormhole[18];
} UnknownProbe;

static void prepare(void *context, StarsU8 *status, StarsU16 count)
{
    UnknownProbe *probe = (UnknownProbe *)context;
    ++probe->prepared;
    if (count > 1) status[1] = 0x01;
}

static StarsI16 random_value(void *context, StarsI16 bound)
{
    UnknownProbe *probe = (UnknownProbe *)context;
    (void)bound;
    ++probe->random_calls;
    return probe->random_result;
}

static const StarsU8 *find_wormhole(void *context, StarsPoint source,
    StarsU32 distance)
{
    UnknownProbe *probe = (UnknownProbe *)context;
    (void)source;
    probe->worm_distance = distance;
    return probe->wormhole;
}

int main(void)
{
    StarsU8 status[3] = {0, 0, 0x10};
    StarsPoint points[3];
    UnknownProbe probe;
    const StarsU8 *wormhole;
    StarsI16 result;
    int failures = 0;

    points[0].x = 3; points[0].y = 0;
    points[1].x = 1; points[1].y = 0;
    points[2].x = 2; points[2].y = 0;
    memset(&probe, 0, sizeof(probe));
    probe.random_result = 4;
    result = stars_id_nearest_unknown_planet(points[0], status, points, 3,
        1, 0, prepare, find_wormhole, random_value, &probe, &wormhole);
    if (result != -1 || wormhole != probe.wormhole || probe.prepared != 1 ||
        probe.random_calls != 1 || probe.worm_distance != 1UL)
        ++failures;
    wormhole = 0;
    probe.random_result = 5;
    result = stars_id_nearest_unknown_planet(points[0], status, points, 3,
        1, 1, prepare, find_wormhole, random_value, &probe, &wormhole);
    if (result != 2 || wormhole != 0)
        ++failures;
    printf("ID_NEAREST_UNKNOWN_PLANET_CHECKS=2 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
