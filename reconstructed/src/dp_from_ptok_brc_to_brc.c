#include "core.h"

#define DAMAGE_GROUP_BEAM 0x10U
#define DAMAGE_GROUP_TORPEDO 0x20U
#define DAMAGE_RANGE_SCALE 10L
#define DAMAGE_TORPEDO_SCALE 200L
#define DAMAGE_SHIELD_SCALE 1600L
#define DAMAGE_MINIMUM 1L
#define DAMAGE_BATTLE_COMPUTER_FLAG 1U

static StarsI16 damage_distance(StarsU8 first, StarsU8 second)
{
    StarsI16 dx = (StarsI16)(first & 0x0fU) - (StarsI16)(second & 0x0fU);
    StarsI16 dy = (StarsI16)(first >> 4) - (StarsI16)(second >> 4);
    if (dx < 0) dx = (StarsI16)-dx;
    if (dy < 0) dy = (StarsI16)-dy;
    return dx > dy ? dx : dy;
}

static StarsI16 damage_count(const StarsDamageEstimateComponent *component)
{
    return (StarsI16)(component->packed_count_type >> 8);
}

static StarsI16 damage_type(const StarsDamageEstimateComponent *component)
{
    return (StarsI16)(component->packed_count_type & 0xffU);
}

static StarsI32 damage_apply_range(StarsI32 damage, StarsI16 distance,
    StarsI16 cargo_limit)
{
    StarsI32 denominator = (StarsI32)(distance + DAMAGE_RANGE_SCALE -
        cargo_limit);
    if (denominator <= 0) return damage;
    damage /= denominator;
    if (damage < 0 || damage < DAMAGE_MINIMUM) return DAMAGE_MINIMUM;
    return damage;
}

/* Genuine semantic reconstruction of DpFromPtokBrcToBrc, NE 31:4D2E-5312.
 * The routine estimates damage from one token to another at two battle
 * coordinates.  Beam systems use capacity, beam-defense percentage and
 * optional shield limitation; torpedo systems use the torpedo-hit model and
 * the shield-mitigated remainder.  A range miss is rejected normally but is
 * still scored in proximity mode, where the caller is evaluating candidate
 * movement squares. */
StarsI32 stars_dp_from_ptok_brc_to_brc(StarsDpFromPtokInput *input)
{
    StarsI16 index;
    StarsI16 count;
    StarsI16 type;
    StarsI16 dp;
    StarsI16 distance;
    StarsI16 cargo_limit;
    StarsI16 over_range;
    StarsI32 damage = 0L;
    StarsI32 weapon_damage;
    StarsI32 hit_damage;
    StarsI32 base_damage;
    const StarsDamageEstimateComponent *component;
    const StarsDamageEstimateToken *target;

    if (!input || !input->source || !input->target || !input->components)
        return 0L;
    target = input->target;
    distance = damage_distance(input->source_coordinate,
        input->target_coordinate);
    if (!input->proximity && distance > input->source->range) return 0L;
    for (index = 0; index < (StarsI16)input->component_count; ++index) {
        component = &input->components[index];
        count = damage_count(component);
        type = damage_type(component);
        if (count == 0 || !(component->group & 0x30U)) continue;
        dp = input->part_dp ? input->part_dp(input->context,
            component->group, (StarsU16)type) : 0;
        cargo_limit = (StarsI16)dp +
            (input->source->object_kind == 1U ? 1 : 0);
        over_range = distance > cargo_limit;
        if (over_range && !input->proximity) continue;
        if (component->group == DAMAGE_GROUP_BEAM) {
            weapon_damage = (StarsI32)component->fuel_max * count;
            if (input->source->pct_capacity)
                weapon_damage = weapon_damage * input->source->pct_capacity / 100L;
            if (distance > 0 && cargo_limit >= 0)
                weapon_damage -= weapon_damage * distance /
                    ((StarsI32)DAMAGE_RANGE_SCALE * (cargo_limit + 1));
            if (target->pct_beam_defense < 100U)
                weapon_damage = weapon_damage * target->pct_beam_defense / 100L;
            if (component->flags & DAMAGE_BATTLE_COMPUTER_FLAG) {
                base_damage = (StarsI32)target->dp_shield *
                    input->source->ship_count;
                if (weapon_damage > base_damage) weapon_damage = base_damage;
            }
            if (over_range)
                weapon_damage = damage_apply_range(weapon_damage, distance,
                    cargo_limit);
            damage += weapon_damage * input->source->ship_count;
        } else if (component->group == DAMAGE_GROUP_TORPEDO) {
            base_damage = (StarsI32)component->damage_rating *
                input->source->ship_count * DAMAGE_TORPEDO_SCALE;
            hit_damage = input->torp_hit ? input->torp_hit(input->context,
                base_damage, target, component->fuel_max,
                input->source->pct_battle_computer) : base_damage;
            weapon_damage = (StarsI32)component->fuel_max * hit_damage /
                DAMAGE_TORPEDO_SCALE;
            if (target->dp_shield != 0)
                weapon_damage += (base_damage - hit_damage) *
                    component->fuel_max / DAMAGE_SHIELD_SCALE;
            if (over_range)
                weapon_damage = damage_apply_range(weapon_damage, distance,
                    cargo_limit);
            damage += weapon_damage;
        }
    }
    weapon_damage = (StarsI32)(target->hull_damage + target->dp_shield) *
        target->ship_count;
    if (target->battle_computer && weapon_damage > 0) {
        hit_damage = weapon_damage * target->battle_computer / 10L;
        weapon_damage -= hit_damage;
        if (weapon_damage < DAMAGE_MINIMUM) weapon_damage = DAMAGE_MINIMUM;
    }
    if (!input->proximity && weapon_damage < damage) return weapon_damage;
    return damage;
}
