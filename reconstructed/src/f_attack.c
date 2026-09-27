#include "core.h"

#define STARS_ATTACK_WEAPON_TORPEDO 2
#define STARS_ATTACK_TORPEDO_SHIELD_FLAG 0x0080
#define STARS_ATTACK_DEFAULT_TORPEDO_PASSES 2

/* Typed semantic reconstruction of the reusable section-selection and
 * target/damage dispatch in FAttack, NE 31:69AC-7CDD.  The original stores
 * the hull, token, battle record, and damage engine in packed Win16 records;
 * those records are represented by the typed views and callback below. */

static StarsI16 attack_player_allowed(StarsI16 attacker, StarsI16 target,
    StarsU16 players)
{
    if (target == attacker || target < 0 || target >= 16)
        return 0;
    return (players & (StarsU16)(1U << target)) != 0;
}

static StarsI16 attack_section_init(const StarsAttackSection *section)
{
    StarsU16 init;

    if (section->init >= 0)
        return (StarsI16)section->init;
    /* The raw FAttack path writes initBase + part.dp into an unsigned
     * 16-bit temporary and only caps values above 63.  Preserve that
     * wrap-before-cap behavior; a negative signed part delta therefore
     * selects initiative 63 after underflow rather than initiative 0. */
    init = (StarsU16)((StarsU16)section->init_base +
        (StarsU16)section->init_delta);
    if (init > 63) init = 63;
    return (StarsI16)init;
}

static StarsU16 attack_target_mask(const StarsAttackSection *section,
    StarsI16 mode, StarsI16 combine_modes)
{
    StarsU16 target_slots;

    if (section->target_mode_masks[0] || section->target_mode_masks[1]) {
        if (combine_modes)
            return (StarsU16)(section->target_mode_masks[0] |
                section->target_mode_masks[1]);
        if (mode < 0 || mode > 1)
            return 0U;
        return section->target_mode_masks[mode];
    }
    target_slots = section->target_slots;
    if (target_slots == 0U) {
        if (section->target_slot < 0 || section->target_slot >= 16)
            return 0U;
        target_slots = (StarsU16)(1U << section->target_slot);
    }
    return target_slots;
}

static StarsI16 attack_target_matches(const StarsAttackSection *section,
    const StarsAttackInput *input, const StarsAttackToken *target,
    StarsI16 mode, StarsI16 combine_modes)
{
    if (!target->active_flags)
        return 0;
    if ((target->target_mask & attack_target_mask(section, mode,
            combine_modes)) == 0U)
        return 0;
    if (section->range_enabled)
        return stars_dz_from_brc_brc(input->attacker_coordinate,
            target->battle_coordinate) <= section->target_range;
    return target->battle_rating <= section->battle_rating;
}

static StarsI32 attack_torpedo_hits(const StarsAttackSection *section,
    StarsI32 packet_budget, const StarsAttackToken *target,
    const StarsAttackOps *ops)
{
    StarsTorpHitToken torpedo_target;

    torpedo_target.jam_percent = target->torpedo_jam_percent;
    return stars_c_torp_hit(packet_budget, &torpedo_target,
        section->torpedo_base_percent, section->battle_computer_percent,
        ops->torpedo_random, ops->context);
}

/* Raw FAttack ranks torpedo candidates before calling CTorpHit.  The score is
 * resource value divided by expected torpedoes-to-destroy, with the original
 * shield/hull truncation order retained. */
static StarsI32 attack_torpedo_target_score(
    const StarsAttackSection *section, const StarsAttackToken *target)
{
    StarsI32 resource_value;
    StarsI32 shield_value;
    StarsI32 hull_value;
    StarsI32 defense_reduction;
    StarsI32 defense_shield_percent;
    StarsI32 defense_dp_percent;
    StarsI32 hit_percent;
    StarsI32 damage_factor;
    StarsI32 shield_factor;
    StarsI32 hull_shots;
    StarsI32 shield_shots;
    StarsI32 hull_only_shots;
    StarsI32 expected_shots;
    StarsI32 denominator;
    StarsI32 item;

    if (!section || !target)
        return 0L;
    hit_percent = section->torpedo_base_percent;
    if (section->battle_computer_percent <
            (StarsI16)target->torpedo_jam_percent) {
        hit_percent -= hit_percent *
            ((StarsI32)target->torpedo_jam_percent -
                section->battle_computer_percent) / 100L;
    } else {
        hit_percent += (100L - hit_percent) *
            ((StarsI32)section->battle_computer_percent -
                target->torpedo_jam_percent) / 100L;
    }
    if (hit_percent < 1L)
        return 0L;

    resource_value = (target->resource_cost + target->ore_cost_ironium) *
        (StarsI32)target->ship_count;
    if (resource_value < 100000L)
        resource_value *= 100L;
    else
        resource_value = 10000000L;
    shield_value = target->shield_points * (StarsI32)target->ship_count;
    hull_value = target->hull_dp * (StarsI32)target->ship_count;
    if (target->defense_value != 0U) {
        defense_shield_percent =
            (StarsI32)(target->defense_value & 0x007fU);
        defense_dp_percent = (StarsI32)(target->defense_value >> 7);
        defense_reduction = target->hull_dp * defense_dp_percent / 10L;
        defense_reduction = defense_reduction * defense_shield_percent /
            10L;
        defense_reduction = defense_reduction *
            (StarsI32)target->ship_count / 500L;
        hull_value -= defense_reduction;
    }
    if (hull_value < 1L)
        hull_value = 1L;

    /* FAttack deliberately changes operation order at the 100000 boundary
     * to avoid the larger intermediate products used by the fast path. */
    if (hull_value < 100000L)
        hull_shots = hull_value * 100L * 2L / hit_percent;
    else
        hull_shots = (hull_value / hit_percent) * 200L;

    shield_factor = (100L - hit_percent) / 8L + hit_percent / 2L;
    if (shield_factor <= 0L)
        shield_shots = 0L;
    else if (shield_value < 100000L)
        shield_shots = shield_value * 100L / shield_factor;
    else
        shield_shots = (shield_value / shield_factor) * 100L;

    item = (StarsI32)(section->part_item & 0x00ffU);
    damage_factor = (item >= 8L && item <= 11L) ? 2L : 1L;
    hull_only_shots = (hull_value - shield_value * hit_percent / 200L) *
        100L / (hit_percent * damage_factor);
    expected_shots = hull_shots;
    if (hull_only_shots + shield_shots <= expected_shots)
        expected_shots = hull_only_shots + shield_shots;
    if (expected_shots < 1L)
        return 0L;
    denominator = expected_shots;
    resource_value /= denominator;
    return resource_value < 1L ? 1L : resource_value;
}

static void attack_record_hit(StarsAttackInput *input, StarsI16 target_index,
    StarsI16 section_index)
{
    if (input->first_hit_target < 0) {
        input->first_hit_target = target_index;
        input->first_hit_section = section_index;
        if (input->battle_record)
            input->battle_record[5] = (StarsU8)target_index;
    }
    ++input->hit_count;
    if (input->battle_record) {
        input->battle_record[2] = (StarsU8)(input->hit_count & 0xff);
        input->battle_record[3] = (StarsU8)((StarsU16)input->hit_count >> 8);
    }
}

static StarsI32 attack_beam_damage(const StarsAttackSection *section,
    const StarsAttackToken *target, StarsI16 apply_distance_falloff,
    StarsI32 damage_pool)
{
    StarsI32 damage;
    StarsI32 distance_loss;

    damage = (section->raw_scaling != 0U && damage_pool > 0L) ? damage_pool :
        section->base_damage;
    if (section->raw_scaling != 0U) {
        if (section->raw_capacity_percent != 0U)
            damage = damage * (StarsI32)section->raw_capacity_percent /
                100L;
    } else if (section->fuel_factor > 0L) {
        damage *= section->fuel_factor;
    }
    if (section->weapon_mode != STARS_ATTACK_WEAPON_TORPEDO &&
            target->beam_defense_percent < 100)
        damage = damage * target->beam_defense_percent / 100L;
    if (apply_distance_falloff && section->raw_scaling != 0U &&
            section->raw_distance > 0U &&
            section->raw_part_dp > 0U) {
        distance_loss = (StarsI32)section->raw_distance * 10L /
            (StarsI32)section->raw_part_dp;
        damage = damage * (100L - distance_loss) / 100L;
    }
    if (section->cargo_flags != 0)
        damage += section->fuel_factor;
    return damage;
}

/* Raw FAttack chooses FDamageTok's beam weapon class from byte offset 54 of
 * the resolved group-0x10 part record.  The recovered _beam view names that
 * word dp; the decompiler's overlapping _hul view calls it wtFuelMax. */
static StarsU16 attack_raw_beam_damage_class(
    const StarsAttackSection *section)
{
    return section->raw_part_dp < 200U ? 1U : 2U;
}

static StarsI16 attack_apply_beam_damage(StarsAttackInput *input,
    const StarsAttackSection *section, StarsI16 target_index,
    StarsI16 section_index, StarsI32 *damage)
{
    if (section->raw_scaling != 0U && input->ops.raw_damage_token)
        return input->ops.raw_damage_token(input->ops.context,
            input->attacker_index, target_index, section_index, damage,
            0L, attack_raw_beam_damage_class(section),
            (StarsI16)(section->raw_weapon_flags & 1U),
            (StarsU32 *)0);
    return input->ops.damage_token(input->ops.context,
        input->attacker_index, target_index, section_index, *damage,
        section->weapon_mode);
}

static StarsI16 attack_read_target(const StarsAttackInput *input,
    StarsI16 target_index, StarsAttackToken *target)
{
    if (input->ops.read_target)
        return input->ops.read_target(input->ops.context, target_index,
            target);
    if (!input->targets || target_index < 0 ||
            target_index >= input->target_count)
        return 0;
    *target = input->targets[target_index];
    return 1;
}

static void attack_multiply_u32(StarsU32 left, StarsU32 right,
    StarsU32 *high, StarsU32 *low)
{
    StarsU32 left_low;
    StarsU32 left_high;
    StarsU32 right_low;
    StarsU32 right_high;
    StarsU32 p00;
    StarsU32 p01;
    StarsU32 p10;
    StarsU32 p11;
    StarsU32 old_low;
    StarsU32 carry;

    left_low = left & 0xffffU;
    left_high = left >> 16;
    right_low = right & 0xffffU;
    right_high = right >> 16;
    p00 = left_low * right_low;
    p01 = left_low * right_high;
    p10 = left_high * right_low;
    p11 = left_high * right_high;
    *low = p00;
    old_low = *low;
    *low += p01 << 16;
    carry = (*low < old_low) ? 1UL : 0UL;
    old_low = *low;
    *low += p10 << 16;
    if (*low < old_low)
        ++carry;
    *high = p11 + (p01 >> 16) + (p10 >> 16) + carry;
}

static StarsU32 attack_muldiv_capped(StarsU32 left, StarsU32 right,
    StarsU32 divisor, StarsU32 cap)
{
    StarsU32 high;
    StarsU32 low;
    StarsU32 remainder;
    StarsU32 quotient;
    StarsU32 bit;
    StarsI16 index;

    if (divisor == 0U)
        return 0U;
    attack_multiply_u32(left, right, &high, &low);
    remainder = 0U;
    quotient = 0U;
    for (index = 63; index >= 0; --index) {
        if (index >= 32)
            bit = (high >> (index - 32)) & 1U;
        else
            bit = (low >> index) & 1U;
        remainder = (remainder << 1) | bit;
        if (remainder >= divisor) {
            remainder -= divisor;
            if (index >= 32)
                return cap;
            quotient |= (1UL << index);
            if (quotient > cap)
                return cap;
        }
    }
    return quotient;
}

/* Raw FAttack target scoring before the selected target reaches FDamageTok.
 * The original computes the value of a target's remaining resources and
 * divides it by the effective shield/hull capacity.  Keep the arithmetic
 * explicit here instead of hiding it in the damage callback. */
StarsI32 stars_f_attack_complex_target_score(const StarsAttackSection *section,
    const StarsAttackToken *target)
{
    StarsI32 resource_value;
    StarsI32 shield_value;
    StarsI32 hull_value;
    StarsI32 defense_reduction;
    StarsI32 defense_shield_percent;
    StarsI32 defense_dp_percent;
    StarsI32 denominator;

    if (!section || !target)
        return 0L;
    resource_value = (target->resource_cost + target->ore_cost_ironium) *
        (StarsI32)target->ship_count;
    if (resource_value < 100000L)
        resource_value *= 100L;
    else
        resource_value = 10000000L;
    if (target->beam_defense_percent < 100U)
        resource_value = resource_value *
            (StarsI32)target->beam_defense_percent / 100L;
    shield_value = target->shield_points * (StarsI32)target->ship_count;
    hull_value = target->hull_dp * (StarsI32)target->ship_count;
    if (target->defense_value != 0U) {
        /* Preserve the raw sequence: each division truncates before the
         * following multiply, rather than collapsing the expression. */
        defense_shield_percent = (StarsI32)(target->defense_value & 0x007fU);
        defense_dp_percent = (StarsI32)(target->defense_value >> 7);
        defense_reduction = target->hull_dp * defense_dp_percent / 10L;
        defense_reduction = defense_reduction * defense_shield_percent / 10L;
        defense_reduction = defense_reduction *
            (StarsI32)target->ship_count / 500L;
        hull_value -= defense_reduction;
    }
    if (hull_value < 1L)
        hull_value = 1L;
    if ((section->raw_weapon_flags & 1U) != 0U) {
        if (shield_value < 1L)
            return 0L;
        return (resource_value * 100L + shield_value - 1L) /
            shield_value;
    }
    denominator = hull_value + shield_value + 1L;
    if (denominator <= 0L)
        return 0L;
    return (resource_value * 100L) / denominator;
}

static StarsI16 attack_beam_section_raw_residual(StarsAttackInput *input,
    const StarsAttackSection *section, StarsI16 section_index)
{
    StarsI16 target_mode;
    StarsI16 target_index;
    StarsI16 best_target;
    StarsI16 applied;
    StarsI32 best_score;
    StarsI32 candidate_score;
    StarsI32 damage_before;
    StarsI32 damage;
    StarsI32 next_pool;
    StarsI32 old_pool;
    StarsU32 product;
    StarsAttackToken target;
    StarsAttackToken best_state;

    product = (StarsU32)section->raw_part_dp *
        (StarsU32)section->part_count;
    product *= (StarsU32)input->attacker_ship_count;
    next_pool = (StarsI32)product;
    applied = 0;
    while (next_pool > 0L) {
        best_target = -1;
        best_score = 0L;
        for (target_mode = 1; target_mode >= 0; --target_mode) {
            best_target = -1;
            best_score = 0L;
            for (target_index = 0; target_index < input->target_count;
                    ++target_index) {
                if (!attack_read_target(input, target_index, &target))
                    continue;
                if (!attack_player_allowed(input->attacker_owner,
                        target.owner, input->attack_players) ||
                        !attack_target_matches(section, input, &target,
                            target_mode, 0))
                    continue;
                candidate_score = stars_f_attack_complex_target_score(
                    section, &target);
                if (candidate_score > best_score) {
                    best_score = candidate_score;
                    best_target = target_index;
                }
            }
            if (best_target >= 0)
                break;
        }
        if (best_target < 0 ||
                !attack_read_target(input, best_target, &best_state))
            break;

        damage_before = attack_beam_damage(section, &best_state, 1,
            next_pool);
        if (damage_before <= 0L) {
            next_pool = 0L;
            break;
        }
        damage = damage_before;
        if (attack_apply_beam_damage(input, section, best_target,
                section_index, &damage)) {
            attack_record_hit(input, best_target, section_index);
            applied = 1;
        }
        old_pool = next_pool;
        if (damage <= 0L) {
            next_pool = 0L;
        } else {
            product = attack_muldiv_capped((StarsU32)old_pool,
                (StarsU32)damage, (StarsU32)damage_before,
                (StarsU32)(old_pool - 1L));
            next_pool = (StarsI32)product;
        }
    }
    return applied;
}

static StarsI16 attack_beam_section(StarsAttackInput *input,
    const StarsAttackSection *section, StarsI16 section_index)
{
    StarsI16 pass;
    StarsI16 passes;
    StarsI16 target_index;
    StarsI16 target_mode;
    StarsI16 best_target;
    StarsI32 best_score;
    StarsI32 best_damage;
    StarsI32 candidate_score;
    StarsI32 damage;
    const StarsAttackToken *target;

    if (section->raw_scaling != 0U && input->ops.raw_damage_token &&
            input->ops.read_target && input->attacker_ship_count != 0U &&
            section->part_count != 0U && section->raw_part_dp != 0U)
        return attack_beam_section_raw_residual(input, section,
            section_index);

    passes = section->beam_passes > 0 ? section->beam_passes : 1;
    best_target = -1;
    for (pass = 0; pass < passes; ++pass) {
        /* The raw two-pass loop starts with the high target-mode nibble
         * (iStack_64 == 1), then evaluates the low nibble. */
        target_mode = (passes == 2) ? (StarsI16)(1 - pass) : pass;
        best_target = -1;
        best_score = 0L;
        best_damage = 0L;
        for (target_index = 0; target_index < input->target_count;
                ++target_index) {
            target = &input->targets[target_index];
            if (!attack_player_allowed(input->attacker_owner, target->owner,
                    input->attack_players) ||
                    !attack_target_matches(section, input, target, target_mode,
                    0))
                continue;
            if (section->raw_scaling != 0U) {
                candidate_score = stars_f_attack_complex_target_score(section,
                    target);
                if (candidate_score <= 0L)
                    continue;
                if (candidate_score > best_score) {
                    best_score = candidate_score;
                    best_target = target_index;
                }
            } else {
                damage = attack_beam_damage(section, target, 1, 0L);
                if (damage <= 0L)
                    continue;
                if (damage > best_damage) {
                    best_damage = damage;
                    best_target = target_index;
                }
            }
        }
        if (section->raw_scaling != 0U && best_target >= 0) {
            best_damage = attack_beam_damage(section,
                &input->targets[best_target], 1, 0L);
        }
        if (best_target >= 0 && best_damage <= 0L) {
            best_target = -1;
        }
        if (best_target >= 0) {
            if (section->raw_scaling == 0U)
                best_score = best_damage;
            if (best_damage > 0L) {
                /* Keep the ordinary callback amount as damage even when the
                 * raw complex path used a separate target-ranking score. */
                damage = best_damage;
            }
        }
        if (best_target < 0 || best_damage <= 0L)
            break;
        damage = best_damage;
        if (attack_apply_beam_damage(input, section, best_target,
                section_index, &damage))
            attack_record_hit(input, best_target, section_index);
    }
    return best_target >= 0;
}

static StarsI16 attack_beam_broadcast(StarsAttackInput *input,
    const StarsAttackSection *section, StarsI16 section_index)
{
    StarsI16 target_index;
    StarsI16 applied;
    StarsI32 damage;
    const StarsAttackToken *target;

    applied = 0;
    for (target_index = 0; target_index < input->target_count;
            ++target_index) {
        target = &input->targets[target_index];
        if (!attack_player_allowed(input->attacker_owner, target->owner,
                input->attack_players) ||
                !attack_target_matches(section, input, target, 0, 1))
            continue;
        /* Raw broadcast/simple beams apply capacity and target-defense
         * scaling, but the original broadcast branch has no distance-loss
         * multiply.  Only the complex target-selection path applies it. */
        damage = attack_beam_damage(section, target, 0, 0L);
        if (damage <= 0L)
            continue;
        if (attack_apply_beam_damage(input, section, target_index,
                section_index, &damage)) {
            attack_record_hit(input, target_index, section_index);
            applied = 1;
        }
    }
    return applied;
}

/* Raw torpedo FAttack starts with grfWeapon=4. The four special weapon
 * items 8..11 add bit 3 before both shield and hull FDamageTok calls. */
static StarsU16 attack_raw_torpedo_damage_flags(
    const StarsAttackSection *section)
{
    StarsU16 flags;

    flags = 4U;
    if (section->part_item > 7U && section->part_item < 12U)
        flags = (StarsU16)(flags | 8U);
    return flags;
}

typedef struct AttackTorpedoAllocation {
    StarsI32 shield_term;
    StarsI32 hull_term;
    StarsI32 packet_count;
    StarsI32 shield_damage;
    StarsI32 hull_damage;
} AttackTorpedoAllocation;

/* Reproduce the raw split scan before FDamageTok. The two terms are the
 * complement/hit allocation; preserve every truncating division and the
 * target-capacity threshold order from FAttack. */
static AttackTorpedoAllocation attack_torpedo_allocation(
    const StarsAttackSection *section, const StarsAttackToken *target,
    StarsI32 hits, StarsI32 packet_budget)
{
    AttackTorpedoAllocation allocation;
    StarsI32 budget;
    StarsI32 ships;
    StarsI32 shield_capacity;
    StarsI32 hull_capacity;
    StarsI32 defense_reduction;
    StarsI32 shield_percent;
    StarsI32 dp_percent;
    StarsI32 power;
    StarsI32 candidate;
    StarsI32 allocated_hits;
    StarsI32 shield_after;
    StarsI32 hull_damage;
    StarsI32 effective_hull_damage;

    allocation.shield_term = 0L;
    allocation.hull_term = 0L;
    allocation.packet_count = 0L;
    allocation.shield_damage = 0L;
    allocation.hull_damage = 0L;
    budget = packet_budget;
    ships = (StarsI32)target->ship_count;
    if (budget <= 0L || ships <= 0L)
        return allocation;

    shield_capacity = target->shield_points * ships;
    hull_capacity = target->hull_dp * ships;
    if (target->defense_value != 0U) {
        shield_percent =
            (StarsI32)(target->defense_value & 0x007fU);
        dp_percent = (StarsI32)(target->defense_value >> 7);
        defense_reduction = target->hull_dp * dp_percent / 10L;
        defense_reduction = defense_reduction * shield_percent / 10L;
        defense_reduction = defense_reduction * ships / 500L;
        hull_capacity -= defense_reduction;
    }

    power = (StarsI32)section->raw_torpedo_power;
    if (section->part_item > 7U && section->part_item < 12U &&
            shield_capacity < 1L)
        power *= 2L;

    if (ships < budget && hits * power > hull_capacity) {
        for (candidate = ships; candidate <= budget; ++candidate) {
            allocated_hits = (candidate * hits + budget - 1L) / budget;
            allocation.shield_term = candidate - allocated_hits;
            allocation.hull_term = allocated_hits;
            shield_after = shield_capacity -
                allocation.shield_term * power / 8L;
            if (shield_after < 0L)
                shield_after = 0L;
            hull_damage = allocation.hull_term * power / 2L;
            effective_hull_damage = hull_damage;
            if (shield_after - hull_damage < 0L)
                effective_hull_damage = hull_damage -
                    (shield_after - hull_damage);
            if (hull_capacity <= effective_hull_damage)
                break;
        }
    } else {
        allocation.shield_term = budget - hits;
        allocation.hull_term = hits;
    }

    allocation.packet_count = allocation.shield_term + allocation.hull_term;
    allocation.shield_damage = allocation.shield_term * power / 8L;
    allocation.hull_damage = allocation.hull_term * power / 2L;
    return allocation;
}

static StarsI16 attack_torpedo_section(StarsAttackInput *input,
    const StarsAttackSection *section, StarsI16 section_index)
{
    StarsI16 pass;
    StarsI16 target_index;
    StarsI16 target_mode;
    StarsI16 best_target;
    StarsI32 best_score;
    StarsI32 candidate_score;
    StarsI32 best_hits;
    StarsI32 hits;
    StarsI32 damage;
    const StarsAttackToken *target;
    const StarsAttackToken *best;
    StarsI16 passes;
    StarsI16 applied;
    StarsI16 made_progress;
    StarsU32 packets;
    StarsI32 remaining_packets;
    StarsU16 raw_damage_flags;
    AttackTorpedoAllocation allocation;

    passes = section->torpedo_passes > 0 ? section->torpedo_passes
                                         : STARS_ATTACK_DEFAULT_TORPEDO_PASSES;
    raw_damage_flags = attack_raw_torpedo_damage_flags(section);
    applied = 0;
    remaining_packets = section->torpedo_count;
    while (remaining_packets > 0L) {
        made_progress = 0;
        for (pass = 0; pass < passes && remaining_packets > 0L; ++pass) {
            /* Raw FAttack tries target mode one before mode zero, then repeats
             * both modes while the residual launched-packet budget is nonzero.
             */
            target_mode = passes == 2 ? (StarsI16)(1 - pass) : pass;
            best_target = -1;
            best_score = 0L;
            best_hits = 0L;
            best = (const StarsAttackToken *)0;
            for (target_index = 0; target_index < input->target_count;
                 ++target_index) {
                target = &input->targets[target_index];
                if (!attack_player_allowed(input->attacker_owner, target->owner,
                                           input->attack_players) ||
                    !attack_target_matches(section, input, target, target_mode,
                                           0))
                    continue;
                candidate_score = attack_torpedo_target_score(section, target);
                if (candidate_score > best_score) {
                    best_score = candidate_score;
                    best_target = target_index;
                    best = target;
                }
            }
            if (best_target < 0 || !best)
                continue;
            /* CTorpHit consumes the original random stream only for the target
             * selected by the deterministic raw score. */
            hits = attack_torpedo_hits(section, remaining_packets, best,
                                       &input->ops);
            best_hits = hits;
            allocation = attack_torpedo_allocation(section, best, best_hits,
                                                   remaining_packets);
            if (input->ops.raw_damage_token && allocation.shield_damage > 0L) {
                damage = allocation.shield_damage;
                if (input->ops.raw_damage_token(
                        input->ops.context, input->attacker_index, best_target,
                        section_index, &damage, 0L,
                        (StarsU16)(raw_damage_flags |
                                   STARS_ATTACK_TORPEDO_SHIELD_FLAG),
                        1, (StarsU32 *)0)) {
                    attack_record_hit(input, best_target, section_index);
                    applied = 1;
                }
            } else if (!input->ops.raw_damage_token &&
                       section->torpedo_shield_damage > 0L) {
                damage = best_hits * section->torpedo_shield_damage;
                if (input->ops.damage_token(
                        input->ops.context, input->attacker_index, best_target,
                        section_index, damage,
                        (StarsI16)(section->torpedo_weapon_flags |
                                   STARS_ATTACK_TORPEDO_SHIELD_FLAG))) {
                    attack_record_hit(input, best_target, section_index);
                    applied = 1;
                }
            }
            if (input->ops.raw_damage_token && allocation.hull_damage > 0L) {
                damage = allocation.hull_damage;
                /* pcTorp is the launched packet budget passed through the raw
                 * split, not CTorpHit's successful-hit count. */
                packets = (StarsU32)allocation.packet_count;
                /* The original FDamageTok return value is deliberately ignored
                 * for this second (hull) dispatch: the battle record's hit
                 * count advances after the call even when the damage service
                 * rejects the event.  The shield dispatch above has the
                 * opposite gate. */
                (void)input->ops.raw_damage_token(
                    input->ops.context, input->attacker_index, best_target,
                    section_index, &damage, damage, raw_damage_flags, 0,
                    &packets);
                attack_record_hit(input, best_target, section_index);
                applied = 1;
            } else if (!input->ops.raw_damage_token &&
                       section->torpedo_hull_damage > 0L) {
                damage = best_hits * section->torpedo_hull_damage;
                (void)input->ops.damage_token(
                    input->ops.context, input->attacker_index, best_target,
                    section_index, damage,
                    (StarsI16)section->torpedo_weapon_flags);
                attack_record_hit(input, best_target, section_index);
                applied = 1;
            }
            if (allocation.packet_count <= 0L) {
                remaining_packets = 0L;
                break;
            }
            remaining_packets -= allocation.packet_count;
            made_progress = 1;
        }
        if (!made_progress)
            break;
    }
    return applied;
}

static StarsU16 attack_part_word(const StarsU8 *record, StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1U] << 8));
}

StarsI16 stars_f_attack(StarsAttackInput *input)
{
    StarsI16 section_index;
    const StarsAttackSection *section;
    StarsAttackSection resolved;
    StarsU16 part_group;
    StarsU16 part_item;
    StarsU16 part_count;
    const StarsU8 *part_record;
    StarsU16 part_record_size;

    if (!input || !input->sections || !input->targets ||
        !input->ops.damage_token || input->section_count < 0 ||
        input->target_count < 0)
        return 0;
    input->first_hit_target = -1;
    input->first_hit_section = -1;
    input->hit_count = 0;
    if (input->battle_record) {
        input->battle_record[2] = 0U;
        input->battle_record[3] = 0U;
    }
    for (section_index = 0; section_index < input->section_count;
         ++section_index) {
        section = &input->sections[section_index];
        part_group = section->part_group;
        part_item = section->part_item;
        part_count = section->part_count;
        if (input->packed_parts &&
            (StarsU16)section_index < input->packed_part_count) {
            part_group = input->packed_parts[section_index].group;
            part_item = input->packed_parts[section_index].item;
            part_count = input->packed_parts[section_index].count;
        }
        if (part_group != 0U && (part_group & 0x0030U) != 0U) {
            /* The raw loop ignores an empty hull slot before calling
             * FLookupPart.  A successful lookup fills the local typed view;
             * the caller's packed section remains unchanged. */
            if (part_count == 0U || !input->ops.lookup_part)
                continue;
            resolved = *section;
            if (!input->ops.lookup_part(input->ops.context, part_group,
                                        part_item, part_count, &resolved))
                continue;
            /* The packed `_hs` slot is authoritative even when a resolver
             * only fills the semantic fields of the local section view. */
            resolved.part_group = part_group;
            resolved.part_item = part_item;
            resolved.part_count = part_count;
            if (part_group == 0x0010U) {
                StarsI16 beam_dp;
                StarsI16 beam_init;
                StarsI16 beam_range;

                if (input->ops.lookup_part_record) {
                    part_record_size = 0U;
                    part_record = input->ops.lookup_part_record(
                        input->ops.context, part_group, part_item,
                        &part_record_size);
                    if (!part_record || part_record_size < 58U)
                        continue;
                    beam_range = (StarsI16)attack_part_word(part_record, 52U);
                    beam_dp = (StarsI16)attack_part_word(part_record, 54U);
                    beam_init = (StarsI16)attack_part_word(part_record, 56U);
                } else {
                    beam_dp = stars_lookup_beam_dp(part_item);
                    beam_init = stars_lookup_beam_init(part_item);
                    beam_range = stars_lookup_beam_range(part_item);
                }
                if (beam_dp < 0 || beam_init < 0 || beam_range < 0)
                    continue;
                resolved.raw_part_dp = (StarsU16)beam_dp;
                resolved.init_delta = beam_init;
                resolved.range_enabled = 1U;
                resolved.target_range = (StarsU8)(beam_range +
                    (input->attacker_object_kind == 1U ? 1 : 0));
            } else if (part_group == 0x0020U) {
                StarsI16 torpedo_dp;
                StarsI16 torpedo_init;
                StarsI16 torpedo_range;

                if (input->ops.lookup_part_record) {
                    part_record_size = 0U;
                    part_record = input->ops.lookup_part_record(
                        input->ops.context, part_group, part_item,
                        &part_record_size);
                    if (!part_record || part_record_size < 58U)
                        continue;
                    torpedo_range = (StarsI16)attack_part_word(part_record,
                        52U);
                    torpedo_dp = (StarsI16)attack_part_word(part_record, 54U);
                    torpedo_init = (StarsI16)attack_part_word(part_record,
                        56U);
                } else {
                    torpedo_dp = stars_lookup_torpedo_dp(part_item);
                    torpedo_init = stars_lookup_torpedo_init(part_item);
                    torpedo_range = stars_lookup_torpedo_range(part_item);
                }
                if (torpedo_dp < 0 || torpedo_init < 0 ||
                        torpedo_range < 0)
                    continue;
                resolved.raw_torpedo_power = (StarsU16)torpedo_dp;
                resolved.init_delta = torpedo_init;
                resolved.range_enabled = 1U;
                resolved.target_range = (StarsU8)(torpedo_range +
                    (input->attacker_object_kind == 1U ? 1 : 0));
            }
            section = &resolved;
        }
        if (!section->active || attack_section_init(section) != input->init)
            continue;
        if (section->weapon_mode == STARS_ATTACK_WEAPON_TORPEDO) {
            (void)attack_torpedo_section(input, section, section_index);
            continue;
        }
        if (section->beam_broadcast)
            (void)attack_beam_broadcast(input, section, section_index);
        else
            (void)attack_beam_section(input, section, section_index);
    }
    return input->hit_count != 0;
}

/* FDoCoolBattle's attack callback owns hull/weapon and service resolution.
 * Keep the selected packed `_tok` caller fields in one adapter so consumers
 * cannot silently omit grobj or the battle coordinate used by FAttack. */
StarsI16 stars_attack_bind_cool_battle_token(StarsAttackInput *attack,
    StarsI16 attacker_index, StarsI16 initiative, StarsU16 attack_players,
    const StarsCoolBattleToken *token)
{
    if (!attack || !token)
        return 0;
    attack->attacker_index = attacker_index;
    attack->attacker_owner = token->player;
    attack->attacker_coordinate = token->battle_coordinate;
    attack->attacker_object_kind = token->object_kind;
    attack->init = initiative;
    attack->attack_players = attack_players;
    return 1;
}

/* `_hs` is a four-byte little-endian record: group word, item byte, count
 * byte. Decode it explicitly instead of type-punning packed game storage. */
StarsI16 stars_attack_bind_raw_hull_slots(StarsAttackInput *attack,
    const StarsU8 *raw_slots, StarsU16 raw_stride, StarsU16 slot_count,
    StarsAttackPackedPartSlot *slots, StarsU16 slot_capacity)
{
    StarsU16 index;
    StarsU32 byte_offset;
    const StarsU8 *raw;

    if (!attack || slot_count > slot_capacity ||
            (slot_count != 0U && (!raw_slots || !slots || raw_stride < 4U)))
        return 0;
    for (index = 0; index < slot_count; ++index) {
        byte_offset = (StarsU32)index * (StarsU32)raw_stride;
        raw = raw_slots + byte_offset;
        slots[index].group = (StarsU16)(raw[0] |
            ((StarsU16)raw[1] << 8));
        slots[index].item = raw[2];
        slots[index].count = raw[3];
    }
    attack->packed_parts = slots;
    attack->packed_part_count = slot_count;
    return 1;
}

/* Compose the per-token battle caller fields and its already-resolved raw
 * hull slots at the FAttack boundary. Hull/design selection stays with the
 * game callback that owns the original LpshdefFromTok data. */
StarsI16 stars_attack_bind_cool_battle_hull(StarsAttackInput *attack,
    StarsI16 attacker_index, StarsI16 initiative, StarsU16 attack_players,
    const StarsCoolBattleToken *token, const StarsU8 *raw_slots,
    StarsU16 raw_stride, StarsU16 slot_count,
    StarsAttackPackedPartSlot *slots, StarsU16 slot_capacity)
{
    if (!attack || !token ||
            !stars_attack_bind_raw_hull_slots(attack, raw_slots, raw_stride,
                slot_count, slots, slot_capacity))
        return 0;
    return stars_attack_bind_cool_battle_token(attack, attacker_index,
        initiative, attack_players, token);
}

static void attack_build_hull_lookup_token(StarsToken *packed_token,
    const StarsCoolBattleToken *token)
{
    StarsU16 index;

    for (index = 0; index < (StarsU16)sizeof packed_token->raw; ++index)
        packed_token->raw[index] = 0U;
    packed_token->raw[2] = (StarsU8)token->player;
    packed_token->raw[4] = (StarsU8)token->design;
}

static StarsI16 attack_bind_cool_battle_hull_record(
    StarsAttackInput *attack, StarsI16 attacker_index, StarsI16 initiative,
    StarsU16 attack_players, const StarsCoolBattleToken *token,
    const StarsU8 *hull, StarsAttackPackedPartSlot *slots,
    StarsU16 slot_capacity)
{
    StarsU16 slot_count;

    if (!hull)
        return 0;
    slot_count = hull[122];
    if (slot_count > 16U)
        return 0;
    return stars_attack_bind_cool_battle_hull(attack, attacker_index,
        initiative, attack_players, token, hull + 58, 4U, slot_count,
        slots, slot_capacity);
}

/* Resolve through the callback form of the original per-player table lookup,
 * then bind the inline _hul.rghs array. */
StarsI16 stars_attack_bind_cool_battle_design_hull(StarsAttackInput *attack,
    StarsI16 attacker_index, StarsI16 initiative, StarsU16 attack_players,
    const StarsCoolBattleToken *token, StarsLookupTokenHull lookup_hull,
    void *hull_context, StarsAttackPackedPartSlot *slots,
    StarsU16 slot_capacity)
{
    StarsToken packed_token;
    const StarsU8 *hull;

    if (!attack || !token || !lookup_hull)
        return 0;
    attack_build_hull_lookup_token(&packed_token, token);
    hull = stars_lpshdef_from_tok(&packed_token, lookup_hull, hull_context);
    return attack_bind_cool_battle_hull_record(attack, attacker_index,
        initiative, attack_players, token, hull, slots, slot_capacity);
}

/* Use the original DGROUP far-pointer arrays when available, mapping only the
 * final segmented address through the caller's address-space adapter. */
StarsI16 stars_attack_bind_cool_battle_global_hull(StarsAttackInput *attack,
    StarsI16 attacker_index, StarsI16 initiative, StarsU16 attack_players,
    const StarsCoolBattleToken *token, const StarsU8 *dgroup,
    StarsResolveFarHull resolve_hull, void *hull_context,
    StarsAttackPackedPartSlot *slots, StarsU16 slot_capacity)
{
    StarsToken packed_token;
    const StarsU8 *hull;

    if (!attack || !token || !dgroup || !resolve_hull)
        return 0;
    attack_build_hull_lookup_token(&packed_token, token);
    hull = stars_lpshdef_from_tok_globals(&packed_token, dgroup,
        resolve_hull, hull_context);
    return attack_bind_cool_battle_hull_record(attack, attacker_index,
        initiative, attack_players, token, hull, slots, slot_capacity);
}
