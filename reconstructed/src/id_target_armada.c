#include "core.h"

#define ARMADA_NO_PLANET (-1)
#define ARMADA_NO_TARGET 0xffffU
#define ARMADA_ORDER_PLANET 1U
#define ARMADA_ORDER_FLEET 2U
#define ARMADA_ORDER_MOVE_FLAGS 0x1040U
#define ARMADA_ATTACK_FLAGS 0x0200U
#define ARMADA_STARBASE_SUPPLY 3
#define ARMADA_TROOPER_TRANSFER 4
#define ARMADA_RESOURCE_STEP 5
#define ARMADA_MAX_TROOPERS 30000L

static StarsI32 armada_combat_strength(const StarsArmadaTargetFleet *fleet)
{
    StarsI16 index;
    StarsI32 strength = 0;
    for (index = 2; index < 6; ++index)
        strength += fleet->ship_counts[index];
    for (index = 6; index < 8; ++index)
        strength += (StarsI32)fleet->ship_counts[index] * 2L;
    return strength;
}

static StarsI32 armada_weapon_strength(const StarsArmadaTargetFleet *fleet)
{
    return (StarsI32)fleet->ship_counts[9] + fleet->ship_counts[10];
}

static StarsI16 armada_random(const StarsIdTargetArmadaOps *ops,
    StarsI16 limit)
{
    StarsI16 value;
    if (!ops->random || limit <= 0)
        return 0;
    value = ops->random(ops->context, limit);
    return value < 0 || value >= limit ? 0 : value;
}

static StarsI16 armada_should_leave_base(const StarsArmadaTargetFleet *fleet,
    StarsI32 strength, StarsI32 weapons, StarsU8 strength_limit,
    StarsU8 weapon_limit, const StarsIdTargetArmadaOps *ops)
{
    StarsI16 roll;
    (void)fleet;
    if (strength >= strength_limit && weapons >= weapon_limit)
        return 0;
    /* The original uses progressively stricter random gates as the fleet
     * approaches its normal strength.  Keep those gates visible rather than
     * hiding them in a generic “AI chance” helper. */
    if (strength < (StarsI32)strength_limit * 2L && strength < 60L)
        return 1;
    roll = armada_random(ops, 10);
    if (roll > 4 && strength <= (StarsI32)strength_limit * 3L &&
            armada_random(ops, 10) > 6)
        return 1;
    if (strength < 121L && armada_random(ops, 10) > 6)
        return 1;
    return 0;
}

/* Genuine semantic reconstruction of IdTargetArmada, NE 18:288E-31BC.
 *
 * This routine decides whether a Robotoid armada should keep its current
 * order, resupply or reinforce at a friendly starbase, transfer colonists'
 * troopers to an enemy world, or select a new planet/fleet target.  The raw
 * routine reads a large number of global fleet and selection records; the
 * typed input below exposes the values that affect those decisions and uses
 * callbacks for the original target enumerators and order writer. */
StarsI16 stars_id_target_armada(StarsArmadaTargetFleet *fleet, StarsI16 player,
    StarsU16 game_turn, StarsU16 player_flags,
    const StarsArmadaTargetPlanet *planets,
    StarsU16 planet_count, const StarsIdTargetArmadaOps *ops)
{
    StarsArmadaTargetPlanet current;
    StarsArmadaTargetPlanet target;
    StarsArmadaTargetFleet enemy;
    StarsI32 strength;
    StarsI32 weapons;
    StarsI32 transfer;
    StarsI16 found;
    StarsU16 index;
    StarsU16 target_kind;
    StarsU16 target_id;
    StarsPoint target_point;

    if (!fleet || !ops || !ops->move)
        return 0;
    strength = armada_combat_strength(fleet);
    weapons = armada_weapon_strength(fleet);

    /* An in-transit order is retained unless it is a wormhole order or its
     * destination has become invalid.  The original also accepts a nearby
     * order without consulting the destination planet again. */
    if (fleet->order_kind != 0U && fleet->planet_id != ARMADA_NO_PLANET) {
        if (fleet->order_kind == 2U)
            return 0;
        if (fleet->order_kind == ARMADA_ORDER_PLANET && ops->lookup_planet) {
            if (!ops->lookup_planet(ops->context, fleet->order_target,
                    &current))
                return 0;
            if (current.turn_seen != 0U && current.turn_seen != game_turn)
                return 0;
            if (current.owner != -1 && current.owner != player &&
                    current.starbase == 0U)
                return 0;
        }
    }

    if (fleet->planet_id == ARMADA_NO_PLANET) {
        (void)ops->move(ops->context, fleet, ARMADA_ORDER_PLANET,
            ARMADA_NO_TARGET, fleet->point);
        return 0;
    }
    if (!ops->lookup_planet || !ops->lookup_planet(ops->context,
            (StarsU16)fleet->planet_id, &current))
        return 0;

    /* At a friendly starbase, a weak armada may be held for assembly.  A
     * sufficiently strong one can donate supply to the base before leaving. */
    if (current.owner == player && current.starbase != 0U) {
        if (armada_should_leave_base(fleet, strength, weapons, 4U, 6U, ops))
            return 0;
        if (fleet->minerals > 0 && ops->transfer_supply) {
            transfer = fleet->minerals / 20L;
            if (fleet->minerals < 1000L) transfer = 0;
            if (transfer > 0) {
                ops->transfer_supply(ops->context, current.id, player,
                    (StarsI16)transfer);
                (void)ops->move(ops->context, fleet, ARMADA_ORDER_FLEET,
                    fleet->id, fleet->point);
            }
        }
    } else if (current.owner == player) {
        /* Friendly non-starbase planets are staging points.  If the fleet is
         * below the weaker armada threshold, clear its stale task and seek a
         * friendly base; otherwise continue to strategic target selection. */
        if (strength < 6L || weapons < 3L) {
            if (ops->clear_task)
                ops->clear_task(ops->context, fleet, 0);
            if (ops->find_destination && ops->find_destination(ops->context,
                    &current, player, &target))
                return ops->move(ops->context, fleet, ARMADA_ORDER_PLANET,
                    target.id, target.point) ? 0 : -1;
        }
    } else if (current.owner != -1) {
        /* An enemy planet is a troop destination.  Send at most half of the
         * fleet's troop reserve and cap the transfer at the original 30,000
         * population safety limit. */
        if (fleet->troopers > 0 && ops->transfer_troopers) {
            transfer = fleet->troopers / ARMADA_RESOURCE_STEP;
            if (transfer > ARMADA_MAX_TROOPERS)
                transfer = ARMADA_MAX_TROOPERS;
            if (transfer > 0) {
                ops->transfer_troopers(ops->context, current.id, player,
                    (StarsI16)transfer);
                (void)ops->move(ops->context, fleet, ARMADA_ORDER_FLEET,
                    fleet->id, fleet->point);
                return 0;
            }
        }
    }

    target_kind = ARMADA_ORDER_PLANET;
    target_id = ARMADA_NO_TARGET;
    target_point = fleet->point;
    found = 0;
    if (ops->find_destination) {
        /* Bit four is the original “human destination” preference. */
        if ((player_flags & 0x0010U) != 0U)
            found = ops->find_destination(ops->context, &current, 1, &target);
        if (!found)
            found = ops->find_destination(ops->context, &current, 0, &target);
        if (found) {
            target_id = target.id;
            target_point = target.point;
        }
    }
    if (!found && ops->find_enemy_fleet && ops->find_enemy_fleet(
            ops->context, fleet, &enemy)) {
        target_kind = ARMADA_ORDER_FLEET;
        target_id = enemy.id;
        target_point = enemy.point;
        found = 1;
    }
    if (!found) {
        for (index = 0; index < planet_count; ++index)
            if (planets[index].owner != player) {
                target_id = planets[index].id;
                target_point = planets[index].point;
                found = 1;
                break;
            }
    }
    if (!found)
        return 0;
    return ops->move(ops->context, fleet, target_kind, target_id,
        target_point) ? 0 : -1;
}
