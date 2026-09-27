#include "core.h"
#include <stdio.h>

static StarsI16 is_transport(void *context, StarsU16 id)
{ (void)context; return id == 8 ? 1 : 0; }
int main(void)
{
    StarsAiHistoryEntry entries[4];
    StarsAiHistoryPlanet planets[2];
    StarsAiHistoryTransport transport;
    StarsValidateHistoryOps ops;
    StarsU16 count = 2;
    int failures = 0;
    entries[0].planet_id = 9; entries[0].level = 20; entries[0].fleet_count = 0;
    entries[1].planet_id = 4; entries[1].level = 2; entries[1].fleet_count = 0;
    planets[0].id = 4; planets[0].owner = 1; planets[0].active_starbase = 1; planets[0].population = 30;
    planets[1] = planets[0]; planets[1].id = 7; planets[1].active_starbase = 1; planets[1].population = 30;
    planets[1].concentrations[0] = 50; planets[1].concentrations[1] = 50; planets[1].concentrations[2] = 50;
    planets[1].stored[0] = 0; planets[1].stored[1] = 0; planets[1].stored[2] = 0;
    transport.id = 8; transport.destination = 4;
    ops.is_transport = is_transport;
    stars_validate_starbase_history(entries, &count, 25, 1, 4, planets, 2,
        &transport, 1, &ops, 0);
    if (count != 2 || entries[0].planet_id != 4 || entries[0].level != 2 ||
            entries[0].fleet_count != 1 || entries[0].fleets[0] != 8)
        ++failures;
    printf("VALIDATE_HISTORY_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
