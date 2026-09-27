#include "core.h"

enum {
    CYBER_ORDER_PLANET = 1,
    CYBER_ORDER_FLEET = 2,
    CYBER_PLANET_STARBASE_FLAG = 9,
    CYBER_NEAREST_PLANET_RANGE = 0x01c2,
    CYBER_DEST_HUMAN = 1,
    CYBER_DEST_ORDINARY = 0,
    CYBER_DEST_OUR_STARBASE = 2,
    CYBER_ARMADA_WEAPON_0 = 6,
    CYBER_ARMADA_WEAPON_1 = 7,
    CYBER_ARMADA_WEAPON_2 = 8,
    CYBER_ARMADA_SUPPORT_0 = 9,
    CYBER_ARMADA_SUPPORT_1 = 13,
    CYBER_ARMADA_MAX_DAMAGE = 0x79,
    CYBER_ARMADA_MIN_DAMAGE = 0x3c
};

#define CYBER_CURRENT_ORDER_DISTANCE ((StarsI32)0xf425UL)

static StarsI16 cyber_random10(const StarsTargetCyberArmadaOps *ops)
{
    StarsI16 value;
    if (!ops->random_value)
        return 0;
    value = ops->random_value(ops->context, 10);
    return value < 0 || value >= 10 ? 0 : value;
}

static StarsI32 cyber_distance_squared(StarsPoint a, StarsPoint b)
{
    StarsI32 dx = (StarsI16)(a.x - b.x);
    StarsI32 dy = (StarsI16)(a.y - b.y);
    return dx * dx + dy * dy;
}

static StarsI16 cyber_is_order_kind(const StarsTargetCyberFleet *fleet,
    StarsI16 kind)
{
    return (StarsI16)(fleet->order_kind & 0x0fU) == kind;
}

static StarsI16 cyber_friendly_armada_is_weak(
    const StarsTargetCyberArmadaOps *ops, StarsI16 damage,
    StarsI16 support, const StarsI16 *potency)
{
    StarsI16 roll;
    if (damage >= potency[0] && support >= potency[2])
        return 0;
    if (ops->random_value == 0)
        return 1;
    if (damage <= potency[0] * 2 && damage < CYBER_ARMADA_MIN_DAMAGE)
        return 1;
    roll = cyber_random10(ops);
    if (roll > 4) {
        if (damage > potency[0] * 3)
            roll = cyber_random10(ops);
        if (damage <= potency[0] * 3 || roll > 6) {
            if (damage < CYBER_ARMADA_MAX_DAMAGE)
                return 1;
            roll = cyber_random10(ops);
            if (roll > 6)
                return 1;
        }
    }
    return 0;
}

/* Genuine semantic reconstruction of TargetCyberArmada, NE 16:51A4-5681.
 *
 * Cyber armadas retain a valid current order when it is still nearby or when
 * the order is a fleet attack.  Otherwise the routine evaluates weapon and
 * support potency, abandons stale or under-strength assignments, prefers a
 * human-designated destination when that mode is enabled, and falls back to
 * an ordinary destination or the nearest enemy fleet.  The callbacks expose
 * the original global enumerators and order services as typed operations. */
void stars_target_cyber_armada(StarsTargetCyberFleet *fleet,
    StarsI16 player, StarsU16 game_turn, StarsI16 ai_level,
    const StarsI16 *potency, const StarsTargetCyberArmadaOps *ops)
{
    StarsTargetCyberPlanet current;
    StarsTargetCyberPlanet destination;
    StarsTargetCyberFleet enemy;
    StarsI16 damage;
    StarsI16 support;
    StarsI16 weak;
    StarsI16 roll;
    StarsI16 retreat;
    StarsI16 use_human;

    if (!fleet || !potency || !ops || !ops->lookup_planet ||
            !ops->move_fleet)
        return;

    /* A fleet already in transit is not retargeted if it is close to its
     * order or its order explicitly names another fleet.  Planet orders are
     * also retained when their destination is current, owned by this player,
     * and not a stale starbase record. */
    if (fleet->cord > 1 &&
            (cyber_distance_squared(fleet->point, fleet->order_point) <
             CYBER_CURRENT_ORDER_DISTANCE ||
             !cyber_is_order_kind(fleet, CYBER_ORDER_FLEET))) {
        if (cyber_is_order_kind(fleet, CYBER_ORDER_FLEET))
            return;
        if (cyber_is_order_kind(fleet, CYBER_ORDER_PLANET)) {
            if (!ops->lookup_planet(ops->context, fleet->order_target,
                    &current))
                return;
            if (current.owner != -1 &&
                    (current.owner != player ||
                     (current.status_flags &
                      (1U << CYBER_PLANET_STARBASE_FLAG)) ||
                     current.turn_seen != game_turn))
                return;
        }
    }

    damage = (StarsI16)(fleet->ship_counts[CYBER_ARMADA_WEAPON_0] +
        fleet->ship_counts[CYBER_ARMADA_WEAPON_1] +
        fleet->ship_counts[CYBER_ARMADA_WEAPON_2] * 2 +
        fleet->ship_counts[10] + fleet->ship_counts[11] +
        fleet->ship_counts[12] * 2);
    support = (StarsI16)(fleet->ship_counts[CYBER_ARMADA_SUPPORT_0] +
        fleet->ship_counts[CYBER_ARMADA_SUPPORT_1]);

    if (ops->select_object)
        ops->select_object(ops->context, 2, fleet->id);
    if (fleet->planet_id == -1) {
        if (ops->move_nearest)
            ops->move_nearest(ops->context, fleet, CYBER_NEAREST_PLANET_RANGE);
        return;
    }
    if (!ops->lookup_planet(ops->context, (StarsU16)fleet->planet_id,
            &current))
        return;

    if (current.owner == player) {
        weak = (StarsI16)(damage < potency[0] || support < potency[2]);
        if (weak && ai_level < 2)
            return;
        if (weak && cyber_friendly_armada_is_weak(ops, damage, support,
                potency))
            return;
    } else if (current.owner != -1) {
        weak = (StarsI16)(damage < potency[1] || support < potency[3]);
        if (weak) {
            if (ops->clear_task)
                ops->clear_task(ops->context, fleet, 0);
            retreat = 1;
            if (ai_level >= 2) {
                if (damage > potency[0] * 2) {
                    roll = cyber_random10(ops);
                    if (roll <= 4)
                        retreat = 0;
                }
                if (retreat && damage > potency[0] * 4) {
                    roll = cyber_random10(ops);
                    if (roll <= 6)
                        retreat = 0;
                }
                if (retreat && damage >= CYBER_ARMADA_MAX_DAMAGE) {
                    roll = cyber_random10(ops);
                    if (roll <= 6)
                        retreat = 0;
                }
            }
            if (retreat) {
                if (ops->find_destination && ops->find_destination(
                        ops->context, &current, CYBER_DEST_OUR_STARBASE,
                        &destination)) {
                    if (ops->mark_planet)
                        ops->mark_planet(ops->context, destination.id);
                    ops->move_fleet(ops->context, fleet, CYBER_ORDER_PLANET,
                        destination.id, destination.point);
                }
                return;
            }
        } else {
            return;
        }
    }

    use_human = 0;
    if (ops->find_destination && ai_level != 0)
        use_human = (StarsI16)ops->find_destination(ops->context, &current,
            CYBER_DEST_HUMAN, &destination);
    if (!use_human && ops->find_destination)
        use_human = (StarsI16)ops->find_destination(ops->context, &current,
            CYBER_DEST_ORDINARY, &destination);
    if (use_human) {
        if (ops->mark_planet)
            ops->mark_planet(ops->context, destination.id);
        ops->move_fleet(ops->context, fleet, CYBER_ORDER_PLANET,
            destination.id, destination.point);
        return;
    }
    if (ops->find_enemy_fleet && ops->find_enemy_fleet(ops->context, fleet,
            &enemy)) {
        ops->move_fleet(ops->context, fleet, CYBER_ORDER_FLEET, enemy.id,
            enemy.point);
    }
}
