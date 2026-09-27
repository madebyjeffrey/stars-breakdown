#include "core.h"
#include <stdio.h>

typedef struct FlingProbe { int calls; StarsI16 type; StarsI16 amount; } FlingProbe;
static StarsI16 random_value(void *context, StarsI16 bound)
{ (void)context; (void)bound; return 0; }
static StarsI16 queue_item(void *context, StarsI16 type, StarsI16 amount)
{
    FlingProbe *probe = (FlingProbe *)context;
    ++probe->calls; probe->type = type; probe->amount = amount; return 1;
}
int main(void)
{
    StarsAiFlingPlanet planet;
    StarsAiFlingOps ops;
    FlingProbe probe;
    StarsI32 resources[3] = { 1200L, 1200L, 1200L };
    StarsPoint origin;
    int failures = 0;
    planet.id = 5; planet.point.x = 20; planet.point.y = 0;
    planet.owner = 2; planet.turn_seen = 8; planet.flags = 1;
    planet.mineral_values[0] = 0; planet.mineral_values[1] = 0;
    planet.mineral_values[2] = 0;
    origin.x = 0; origin.y = 0;
    ops.random_value = random_value; ops.queue_item = queue_item;
    probe.calls = 0; probe.type = 0; probe.amount = 0;
    if (stars_f_ai_fling(&planet, 1, origin, 10, 1, resources, 25,
            &ops, &probe) != 1 || probe.calls < 1 || probe.type != 16)
        ++failures;
    printf("AI_FLING_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
