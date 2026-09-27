#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct MergeProbe {
    int calls;
    StarsI16 survivor;
    StarsI16 source;
} MergeProbe;

static void merge_fleet(void *context, StarsAiShdefFleet *survivor,
    StarsAiShdefFleet *source)
{
    MergeProbe *probe = (MergeProbe *)context;
    ++probe->calls; probe->survivor = survivor->planet;
    probe->source = source->planet;
}

int main(void)
{
    StarsAiShdefFleet fleets[3];
    MergeProbe probe;
    int failures = 0;
    memset(fleets, 0, sizeof(fleets));
    fleets[0].player = 4; fleets[0].planet = 7;
    fleets[0].point.x = 10; fleets[0].point.y = 20;
    fleets[0].ship_counts[1] = 2;
    fleets[1].player = 4; fleets[1].planet = 7;
    fleets[1].point.x = 10; fleets[1].point.y = 20;
    fleets[1].ship_counts[4] = 1;
    fleets[2].player = 4; fleets[2].planet = 7;
    fleets[2].point.x = 11; fleets[2].point.y = 20;
    fleets[2].ship_counts[1] = 1;
    probe.calls = 0; probe.survivor = 0; probe.source = 0;
    stars_merge_all_shdefs((StarsU16)(1U << 1), fleets, 3, 4,
        merge_fleet, &probe);
    if (probe.calls != 1 || probe.survivor != 7 || probe.source != 7)
        ++failures;
    fleets[1].mine_value = 4000; probe.calls = 0;
    stars_merge_all_shdefs((StarsU16)(1U << 1), fleets, 3, 4,
        merge_fleet, &probe);
    if (probe.calls != 0)
        ++failures;
    printf("MERGE_ALL_SHDEFS_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
