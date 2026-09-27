#include "core.h"
#include <stdio.h>

typedef struct ScoutProbe { int moves; StarsU16 id; StarsU16 flags; } ScoutProbe;
static StarsI16 has_teeth(void *context, const StarsAiTargetFleet *fleet)
{
    (void)context; (void)fleet; return 0;
}
static StarsI16 move_target(void *context, StarsU16 id, StarsPoint point,
    StarsU16 flags)
{
    ScoutProbe *probe = (ScoutProbe *)context;
    ++probe->moves; probe->id = id; probe->flags = flags; (void)point;
    return 1;
}
int main(void)
{
    StarsAiTargetFleet self;
    StarsAiTargetPlanet unknown[2];
    StarsScoutTargetOps ops;
    ScoutProbe probe;
    StarsU16 worm_id = 0;
    int failures = 0;
    self.id = 1; self.point.x = 0; self.point.y = 0;
    unknown[0].id = 8; unknown[0].point.x = 200; unknown[0].point.y = 0;
    unknown[1] = unknown[0]; unknown[1].id = 9; unknown[1].point.x = 50;
    ops.has_teeth = has_teeth; ops.move_target = move_target;
    probe.moves = 0; probe.id = 0; probe.flags = 0;
    if (stars_id_target_scout(&self, 0, 0, 0, unknown, 2, &worm_id,
            &ops, &probe) != 0 || probe.moves != 1 || probe.id != 9 ||
            probe.flags != 0x0100U || worm_id != 9)
        ++failures;
    printf("ID_TARGET_SCOUT_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
