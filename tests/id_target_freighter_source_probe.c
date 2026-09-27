#include "core.h"
#include <stdio.h>

typedef struct FreighterProbe { StarsU16 id; StarsU16 flags; } FreighterProbe;
static StarsI32 score(void *context, const StarsAiTargetPlanet *planet,
    StarsI16 percent_full)
{ (void)context; return (StarsI32)planet->id * 10L - percent_full; }
static StarsI16 move_target(void *context, StarsU16 id, StarsPoint point,
    StarsU16 flags)
{ FreighterProbe *p = (FreighterProbe *)context; p->id = id; p->flags = flags; (void)point; return 1; }
int main(void)
{
    StarsAiTargetFleet freighter;
    StarsAiTargetPlanet planets[2];
    StarsFreighterTargetOps ops;
    FreighterProbe probe;
    StarsU16 target;
    int failures = 0;
    freighter.id = 1; freighter.planet_id = 3; freighter.point.x = 0; freighter.point.y = 0;
    planets[0].id = 4; planets[0].owner = 2; planets[0].has_attack_fleet = 0; planets[0].point.x = 2; planets[0].point.y = 0;
    planets[1] = planets[0]; planets[1].id = 5;
    ops.score_target = score; ops.move_target = move_target; probe.id = 0; probe.flags = 0;
    if (stars_id_target_freighter(&freighter, planets, 2, 3, 10, 20,
            &ops, &probe, &target) != 5 || target != 5 || probe.flags != 0x0100U)
        ++failures;
    printf("ID_TARGET_FREIGHTER_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
