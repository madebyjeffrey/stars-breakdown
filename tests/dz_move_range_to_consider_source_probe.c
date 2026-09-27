#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI32 damage(void *context, const StarsMoveRangeToken *self,
    const StarsMoveRangeToken *target)
{ (void)context; (void)self; return target->owner == 1 ? 1L : 0L; }

int main(void)
{
    StarsMoveRangeToken self;
    StarsMoveRangeToken target;
    StarsMoveRangeInput input;
    StarsU8 coordinate;
    memset(&self, 0, sizeof self);
    memset(&target, 0, sizeof target);
    memset(&input, 0, sizeof input);
    self.owner = 0;
    self.battle_coordinate = 0x11U;
    self.targeting_class = 2U;
    self.movement_limit = 2U;
    self.movement_phase = 1U;
    self.md_descriptor = 1U;
    target.owner = 1;
    target.battle_coordinate = 0x12U;
    target.md_descriptor = (StarsU16)(7U << 12);
    target.active_flags = 1U;
    target.shield_points = 1U;
    input.self = &self;
    input.targets = &target;
    input.target_count = 1U;
    input.attack_players = 2U;
    input.primary_targets_exist = 1;
    input.damage_projection = damage;
    if (stars_dz_move_range_to_consider(&input, &coordinate) != 1 ||
            coordinate != 0xffU)
        return 1;
    target.battle_coordinate = 0x55U;
    target.shield_points = 0U;
    self.movement_phase = 0U;
    if (stars_dz_move_range_to_consider(&input, &coordinate) != 1 ||
            coordinate != 0x55U)
        return 2;
    target.active_flags = 0U;
    coordinate = 0U;
    if (stars_dz_move_range_to_consider(&input, &coordinate) != 1 ||
            coordinate != 0xffU)
        return 3;
    puts("DZ_MOVE_RANGE_SOURCE=PASS");
    return 0;
}
