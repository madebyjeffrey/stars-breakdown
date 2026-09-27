#include "core.h"

#define MOVE_RANGE_CLASS_FREIGHTER 3
#define MOVE_RANGE_PRIMARY_MISSILE 3
#define MOVE_RANGE_PRIMARY_PLANET 5
#define MOVE_RANGE_TARGET_ACTIVE 1
#define MOVE_RANGE_INITIAL_BEST 10
#define MOVE_RANGE_NO_COORDINATE 0xff

/* NE 31:5312-56D8. Find the battle-token range at which a moving missile
 * defense should stop considering targets.  A target inside the ordinary
 * range wins immediately; otherwise the closest target with positive
 * projected damage is returned through candidate_coordinate for a later
 * movement step. */
StarsI16 stars_dz_move_range_to_consider(const StarsMoveRangeInput *input,
    StarsU8 *candidate_coordinate)
{
    StarsU16 i;
    StarsU16 target_mask;
    StarsI16 target_class;
    StarsI16 range_limit;
    StarsI16 cargo_limit;
    StarsI16 best_range;
    StarsI16 distance;
    StarsMdTargetToken md_target;
    const StarsMoveRangeToken *self;
    const StarsMoveRangeToken *target;

    if (!input || !input->self || !candidate_coordinate)
        return 0;
    self = input->self;
    *candidate_coordinate = MOVE_RANGE_NO_COORDINATE;
    target_class = input->primary_targets_exist ?
        (StarsI16)(self->md_descriptor & 0x000fU) :
        (StarsI16)((self->md_descriptor >> 4) & 0x000fU);
    range_limit = (StarsI16)(self->targeting_class + self->movement_phase);
    cargo_limit = self->targeting_class == MOVE_RANGE_CLASS_FREIGHTER ?
        (StarsI16)input->cargo_capacity : self->targeting_class;
    if (self->movement_limit < self->targeting_class &&
            ((((self->md_descriptor >> 8) & 0x000fU) ==
                MOVE_RANGE_PRIMARY_MISSILE) ||
             (((self->md_descriptor >> 8) & 0x000fU) ==
                MOVE_RANGE_PRIMARY_PLANET))) {
        range_limit = (StarsI16)(self->movement_limit + self->movement_phase);
    }
    best_range = MOVE_RANGE_INITIAL_BEST;
    for (i = 0; i < input->target_count; ++i) {
        target = &input->targets[i];
        if (target->owner == self->owner || target->owner < 0 ||
                target->owner >= 16)
            continue;
        target_mask = (StarsU16)(1U << target->owner);
        if ((input->attack_players & target_mask) == 0U ||
                (target->active_flags & MOVE_RANGE_TARGET_ACTIVE) == 0U)
            continue;
        md_target.grobj = target->md_grobj;
        md_target.overlap_23 = target->md_descriptor;
        if (!stars_is_target_of_md_target(&md_target, target_class))
            continue;
        distance = stars_dz_from_brc_brc(self->battle_coordinate,
            target->battle_coordinate);
        if (self->movement_phase <= target->movement_phase)
            ++distance;
        if (distance <= range_limit &&
                (target->shield_points != 0U ||
                 cargo_limit == self->targeting_class ||
                 distance <= cargo_limit + target->movement_phase)) {
            *candidate_coordinate = MOVE_RANGE_NO_COORDINATE;
            return self->movement_phase;
        }
        if (distance < best_range && input->damage_projection &&
                input->damage_projection(input->context, self, target) > 0L) {
            best_range = distance;
            *candidate_coordinate = target->battle_coordinate;
        }
    }
    return 1;
}
