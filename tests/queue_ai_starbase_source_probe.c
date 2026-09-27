#include "core.h"
#include <stdio.h>

typedef struct QueueProbe { int calls; StarsU16 planet; StarsI16 design; } QueueProbe;
static StarsI16 is_cyber(void *context, StarsU16 planet)
{
    (void)context;
    return planet == 2 ? 1 : 0;
}
static StarsI16 queue_starbase(void *context, StarsU16 planet, StarsI16 design)
{
    QueueProbe *probe = (QueueProbe *)context;
    ++probe->calls; probe->planet = planet; probe->design = design;
    return 1;
}
int main(void)
{
    StarsAiQueueStarbasePlanet planets[2];
    StarsQueueAiStarbaseOps ops;
    QueueProbe probe;
    int failures = 0;
    planets[0].id = 1; planets[0].owner = 3; planets[0].starbase_family = 1;
    planets[0].mining_level = 1; planets[0].mineral_total = 0; planets[0].ai_flags = 0;
    planets[0].history_present = 0;
    planets[1] = planets[0]; planets[1].id = 2; planets[1].starbase_family = 5;
    ops.is_cyber = is_cyber; ops.queue_starbase = queue_starbase;
    probe.calls = 0; probe.planet = 0; probe.design = 0;
    stars_queue_ai_starbase(planets, 2, 3, 4, &ops, &probe);
    if (probe.calls != 1 || probe.planet != 1 || probe.design != 4)
        ++failures;
    printf("QUEUE_AI_STARBASE_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
