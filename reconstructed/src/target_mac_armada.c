#include "core.h"

#define MAC_NO_PLANET (-1)
#define MAC_NO_TARGET 0xffffU
#define MAC_ORDER_PLANET 1U
#define MAC_ORDER_FLEET 2U
#define MAC_MOVE_FLAGS 0x1040U
#define MAC_ATTACK_FLAGS 0x0200U
#define MAC_PLANET_FLAGS 0x0100U
#define MAC_HUMAN_DESTINATION 0x0010U

static StarsI16 mac_random(const StarsTargetMacArmadaOps *ops, StarsI16 limit)
{
    StarsI16 value;
    if (!ops->random || limit <= 0)
        return 0;
    value = ops->random(ops->context, limit);
    return value < 0 || value >= limit ? 0 : value;
}

static StarsI16 mac_order_is_valid(const StarsArmadaTargetFleet *fleet,
    StarsU16 game_turn, StarsI16 player, const StarsTargetMacArmadaOps *ops)
{
    StarsArmadaTargetPlanet destination;
    if (fleet->order_kind == 0U || fleet->planet_id == MAC_NO_PLANET)
        return 1;
    if (fleet->order_kind == MAC_ORDER_FLEET)
        return 0;
    if (!ops->lookup_planet || !ops->lookup_planet(ops->context,
            fleet->order_target, &destination))
        return 0;
    if (destination.owner != -1 && destination.owner != player)
        return 0;
    if (destination.owner == player && destination.starbase != 0U)
        return 0;
    return destination.turn_seen == 0U || destination.turn_seen == game_turn;
}

static StarsI16 mac_should_hold(const StarsTargetMacArmadaOps *ops,
    StarsI32 strength, StarsI32 weapons, const StarsU8 *potency,
    StarsI16 advanced)
{
    StarsI16 roll;
    if (strength >= potency[0] && weapons >= potency[2])
        return 0;
    if (!advanced)
        return 1;
    if (strength <= (StarsI32)potency[0] * 2L && strength < 60L)
        return 1;
    roll = mac_random(ops, 10);
    if (roll > 4 && (strength <= (StarsI32)potency[0] * 3L ||
            mac_random(ops, 10) > 6)) {
        if (strength < 121L || mac_random(ops, 10) > 6)
            return 1;
    }
    return 0;
}

/* Genuine semantic reconstruction of TargetMacArmada, NE 21:3E3A-42EB.
 *
 * The Macinti personality uses the same broad armada lifecycle as the other
 * personalities, but it has its own potency callback and does not transfer
 * cargo here.  It rejects stale or hostile current orders, holds weak fleets
 * at friendly locations, clears weak enemy-facing tasks when a starbase
 * fallback is appropriate, prefers human destinations when the game mode
 * permits them, and finally falls back to an enemy fleet.  The order flags
 * and target kinds are named to make the reconstructed policy readable. */
void stars_target_mac_armada(StarsArmadaTargetFleet *fleet, StarsI16 player,
    StarsU16 game_turn, StarsU16 player_flags, const StarsU8 *potency,
    const StarsArmadaTargetPlanet *planets, StarsU16 planet_count,
    const StarsTargetMacArmadaOps *ops)
{
    StarsArmadaTargetPlanet current;
    StarsArmadaTargetPlanet target;
    StarsArmadaTargetFleet enemy;
    StarsI32 strength = 0;
    StarsI16 equivalent = 0;
    StarsI16 advanced = 1;
    StarsI16 found = 0;
    StarsU16 index;
    StarsU16 target_kind;
    StarsU16 target_id;
    StarsPoint target_point;

    if (!fleet || !potency || !ops || !ops->move)
        return;
    if (!mac_order_is_valid(fleet, game_turn, player, ops))
        return;
    if (ops->potency)
        (void)ops->potency(ops->context, fleet, 0, &equivalent);
    strength = equivalent;
    if (fleet->planet_id == MAC_NO_PLANET) {
        (void)ops->move(ops->context, fleet, MAC_ORDER_PLANET,
            MAC_NO_TARGET, fleet->point);
        return;
    }
    if (!ops->lookup_planet || !ops->lookup_planet(ops->context,
            (StarsU16)fleet->planet_id, &current))
        return;
    if (current.owner == player) {
        if (mac_should_hold(ops, strength,
                fleet->ship_counts[8] + fleet->ship_counts[9], potency,
                advanced))
            return;
    } else if (strength < potency[1] ||
            fleet->ship_counts[8] + fleet->ship_counts[9] < potency[3]) {
        if (ops->clear_task)
            ops->clear_task(ops->context, fleet, 0);
        if (advanced && ops->find_destination &&
                ops->find_destination(ops->context, &current, player, &target))
            (void)ops->move(ops->context, fleet, MAC_ORDER_PLANET, target.id,
                target.point);
        return;
    } else if (current.owner != -1) {
        return;
    }
    if (ops->find_destination) {
        if ((player_flags & MAC_HUMAN_DESTINATION) != 0U)
            found = ops->find_destination(ops->context, &current, 1, &target);
        if (!found)
            found = ops->find_destination(ops->context, &current, 0, &target);
    }
    target_kind = MAC_ORDER_PLANET;
    target_id = MAC_NO_TARGET;
    target_point = fleet->point;
    if (found) {
        target_id = target.id;
        target_point = target.point;
    } else if (ops->find_enemy_fleet && ops->find_enemy_fleet(
            ops->context, fleet, &enemy)) {
        target_kind = MAC_ORDER_FLEET;
        target_id = enemy.id;
        target_point = enemy.point;
        found = 1;
    } else {
        for (index = 0; index < planet_count; ++index)
            if (planets[index].owner != player) {
                target_id = planets[index].id;
                target_point = planets[index].point;
                found = 1;
                break;
            }
    }
    if (found)
        (void)ops->move(ops->context, fleet, target_kind, target_id,
            target_point);
    (void)MAC_ATTACK_FLAGS;
    (void)MAC_PLANET_FLAGS;
    (void)MAC_MOVE_FLAGS;
}
