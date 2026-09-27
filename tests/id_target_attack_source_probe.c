#include "core.h"
#include <stdio.h>

typedef struct TargetProbe {
    int moves;
    StarsU16 id;
    StarsU16 flags;
} TargetProbe;

static StarsI16 move_target(void *context, StarsU16 id, StarsPoint point,
    StarsU16 flags)
{
    TargetProbe *probe = (TargetProbe *)context;
    ++probe->moves;
    probe->id = id;
    probe->flags = flags;
    (void)point;
    return 1;
}

int main(void)
{
    StarsAiTargetFleet self;
    StarsAiTargetFleet enemies[2];
    StarsTargetAttackOps ops;
    TargetProbe probe;
    int failures = 0;

    self.id = 9; self.planet_id = 1; self.point.x = 0; self.point.y = 0;
    self.combat_power = 20; self.human = 0; self.in_transit = 0;
    enemies[0].id = 20; enemies[0].planet_id = 2;
    enemies[0].point.x = 100; enemies[0].point.y = 0;
    enemies[0].combat_power = 5; enemies[0].human = 1; enemies[0].in_transit = 0;
    enemies[1] = enemies[0]; enemies[1].id = 21; enemies[1].point.x = 200;
    ops.random_value = 0; ops.move_target = move_target;
    ops.retreat_to_base = 0; probe.moves = 0; probe.id = 0; probe.flags = 0;
    if (stars_id_target_attack(&self, 0, 0, enemies, 2, 0, 0, 0, 0,
            &ops, &probe) != 0 || probe.moves != 1 || probe.id != 20 ||
            probe.flags != 0x0200U)
        ++failures;
    printf("ID_TARGET_ATTACK_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
