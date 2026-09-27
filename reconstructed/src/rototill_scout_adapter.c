#include "core.h"
#include <string.h>

static StarsI16 scout_word(const StarsU8 *raw)
{
    return (StarsI16)((StarsU16)raw[0] | ((StarsU16)raw[1] << 8));
}

/* Source-bound IdTargetScout routes for Rototill. The caller has
 * already applied its second-sweep scout gates and selected the fleet once. */
StarsI16 stars_rototill_target_scout(void *opaque,
    const StarsRototillTurnFleet *fleet, StarsI16 player)
{
    StarsRototillScoutAdapter *adapter;
    StarsNearestColonizableOps status_ops;
    StarsPoint source;
    StarsMoveOrder order;
    StarsRototillTurnFleet refreshed_view;
    const StarsU8 *wormhole;
    StarsU8 *plord;
    StarsI16 target;
    StarsI16 warp;
    StarsU16 index;
    StarsU16 pass;
    StarsU16 capacity;
    StarsU16 plord_bytes;
    StarsI16 enemy_best;
    StarsI16 far_target;
    StarsU32 enemy_best_distance;
    StarsU32 far_best_distance;
    StarsU32 current_fuel;
    StarsU32 distance;
    StarsI32 dx;
    StarsI32 dy;
    StarsI16 teeth;

    adapter = (StarsRototillScoutAdapter *)opaque;
    if (!adapter || !fleet || !fleet->raw_fleet_record ||
            fleet->raw_fleet_record_size < 124U || !fleet->raw_plord ||
            fleet->raw_plord_bytes < 22U || fleet->raw_plord[3] != 1U ||
            (adapter->enemy_candidate_count &&
             !adapter->enemy_candidates) || !adapter->has_teeth ||
            !adapter->raw_ai_planet_table || !adapter->planet_points ||
            !adapter->planet_owners || !adapter->status_scratch ||
            !adapter->random_value || !adapter->find_wormhole ||
            !adapter->ideal_warp || !adapter->select_fleet ||
            !adapter->refresh_fleet || adapter->planet_count == 0U ||
            adapter->planet_count > 4095U ||
            adapter->planet_count > adapter->ai_planet_record_count ||
            adapter->planet_count > adapter->status_scratch_count ||
            (adapter->attack_planet_count && !adapter->attack_planet_ids) ||
            (adapter->blocked_target_count && !adapter->blocked_targets))
        return 0;
    capacity = (StarsU16)((fleet->raw_plord_bytes - 4U) / 18U);
    if (capacity < 2U && !adapter->ensure_plord_capacity)
        return 0;
    source.x = scout_word(fleet->raw_fleet_record + 8U);
    source.y = scout_word(fleet->raw_fleet_record + 10U);
    memset(&order, 0, sizeof order);
    enemy_best = -1;
    far_target = -1;
    enemy_best_distance = 1000000UL;
    teeth = adapter->has_teeth(adapter->context, fleet);
    if (teeth) {
        for (pass = 0U; pass < 2U; ++pass) {
            if (pass != 0U &&
                    !adapter->has_teeth(adapter->context, fleet))
                break;
            for (index = 0; index < adapter->enemy_candidate_count;
                    ++index) {
                const StarsRototillScoutEnemy *enemy;
                enemy = adapter->enemy_candidates + index;
                if (enemy->id < 0 ||
                        (adapter->only_humans && pass == 0U &&
                         !enemy->human))
                    continue;
                if (enemy->attack_reserved &&
                        adapter->random_value(adapter->context, 3) == 0)
                    continue;
                dx = (StarsI32)enemy->point.x - (StarsI32)source.x;
                dy = (StarsI32)enemy->point.y - (StarsI32)source.y;
                distance = (StarsU32)(dx * dx + dy * dy);
                if (distance < enemy_best_distance) {
                    enemy_best = (StarsI16)index;
                    enemy_best_distance = distance;
                }
            }
            if (enemy_best >= 0 || !adapter->only_humans)
                break;
        }
        if (enemy_best >= 0 && enemy_best_distance >= 0x7e90UL) {
            const StarsPoint *enemy_point;
            StarsU16 reserved;
            StarsU16 attack;
            if (!adapter->fuel_capacity)
                return 0;
            current_fuel = (StarsU32)fleet->raw_fleet_record[92U] |
                ((StarsU32)fleet->raw_fleet_record[93U] << 8) |
                ((StarsU32)fleet->raw_fleet_record[94U] << 16) |
                ((StarsU32)fleet->raw_fleet_record[95U] << 24);
            if (current_fuel <
                    adapter->fuel_capacity(adapter->context, fleet) / 2UL) {
                if (!adapter->move_to_starbase)
                    return 0;
                if (adapter->move_to_starbase(adapter->context, fleet,
                        player))
                    return 1;
            }
            enemy_point = &adapter->enemy_candidates[
                (StarsU16)enemy_best].point;
            far_best_distance = 1000000UL;
            for (index = 0; index < adapter->planet_count; ++index) {
                if (adapter->planet_owners[index] == -2)
                    continue;
                reserved = 0U;
                for (attack = 0U; attack < adapter->attack_planet_count;
                        ++attack)
                    if (adapter->attack_planet_ids[attack] ==
                            (StarsI16)index) {
                        reserved = 1U;
                        break;
                    }
                if (reserved)
                    continue;
                dx = (StarsI32)adapter->planet_points[index].x -
                    (StarsI32)enemy_point->x;
                dy = (StarsI32)adapter->planet_points[index].y -
                    (StarsI32)enemy_point->y;
                distance = (StarsU32)(dx * dx + dy * dy);
                if (distance < far_best_distance) {
                    far_target = (StarsI16)index;
                    far_best_distance = distance;
                }
            }
            if (far_target == fleet->planet_id)
                far_target = -1;
            enemy_best = -1;
        }
    }
    if (far_target >= 0) {
        order.point = adapter->planet_points[(StarsU16)far_target];
        order.id = far_target;
        order.flags = 0x1100U;
        adapter->raw_ai_planet_table[(StarsU16)far_target * 16U + 15U] = 4U;
    } else if (enemy_best >= 0) {
        order.point = adapter->enemy_candidates[(StarsU16)enemy_best].point;
        order.id = adapter->enemy_candidates[(StarsU16)enemy_best].id;
        order.flags = 0x1200U;
    } else {
    if (!adapter->marked_planets) {
        if (adapter->race_mode != 0 && adapter->race_mode != 5 &&
                !adapter->planet_opt_value)
            for (index = 0; index < adapter->planet_count; ++index)
                if (adapter->planet_owners[index] == -1)
                    return 0;
        memset(&status_ops, 0, sizeof status_ops);
        status_ops.planet_opt_value = adapter->planet_opt_value;
        stars_id_nearest_colonizable_planet(source, fleet->planet_id,
            adapter->race_mode, adapter->turn, 0, player,
            adapter->status_scratch, adapter->planet_owners,
            adapter->planet_points, adapter->planet_count,
            adapter->blocked_targets, adapter->blocked_target_count,
            &status_ops, adapter->context, 0);
        for (index = 0; index < adapter->planet_count; ++index)
            adapter->raw_ai_planet_table[index * 16U + 15U] =
                adapter->status_scratch[index];
        adapter->marked_planets = 1;
    }
    wormhole = 0;
    target = stars_id_nearest_unknown_planet_strided(source,
        adapter->raw_ai_planet_table, 16U, 15U,
        adapter->planet_points, adapter->planet_count,
        (StarsI16)(fleet->planet_id >= 0), 1, 0,
        adapter->find_wormhole, adapter->random_value,
        adapter->context, &wormhole);
    if (!wormhole && target < 0) {
        if (adapter->home_planet_id >= 0) {
            if (!adapter->best_home_target ||
                    (StarsU16)adapter->home_planet_id >=
                        adapter->planet_count)
                return 0;
            target = adapter->best_home_target(adapter->context,
                adapter->home_planet_id);
        }
        if (target < 0)
            target = adapter->random_value(adapter->context,
                (StarsI16)adapter->planet_count);
    }
    /* A null wormhole still requires a planet target; a selected wormhole
     * carries its own point and packed thing id. */
    if (!wormhole && (target < 0 ||
            (StarsU16)target >= adapter->planet_count))
        return 0;
    if (wormhole) {
        order.point.x = scout_word(wormhole + 2U);
        order.point.y = scout_word(wormhole + 4U);
        order.id = scout_word(wormhole);
        order.flags = 0x1800U;
    } else {
        order.point = adapter->planet_points[(StarsU16)target];
        order.id = target;
        order.flags = 0x1100U;
        /* IdTargetScout marks a planet route before IFindIdealWarp and
         * FMoveAiFleet, so callbacks observe the live reservation. */
        adapter->raw_ai_planet_table[(StarsU16)target * 16U + 15U] = 4U;
    }
    }
    warp = adapter->ideal_warp(adapter->context, fleet, order.id,
        order.point);
    if (warp < 0 || warp > 15)
        return 0;
    order.flags = (StarsU16)(order.flags | ((StarsU16)warp << 4));
    if (!adapter->select_fleet(adapter->context, fleet, player))
        return 0;
    refreshed_view = *fleet;
    plord = fleet->raw_plord;
    plord_bytes = fleet->raw_plord_bytes;
    if (capacity < 2U) {
        if (!adapter->ensure_plord_capacity(adapter->context, fleet, 2U,
                &plord, &plord_bytes) || !plord || plord_bytes < 40U ||
                plord[3] != 1U)
            return 0;
        refreshed_view.raw_plord = plord;
        refreshed_view.raw_plord_bytes = plord_bytes;
        capacity = (StarsU16)((plord_bytes - 4U) / 18U);
    }
    if (!stars_ai_move_order_to_plord(plord, capacity, &order, 0))
        return 0;
    fleet->raw_fleet_record[98U] = plord[3];
    fleet->raw_fleet_record[99U] = 0U;
    if (!adapter->refresh_fleet(adapter->context, &refreshed_view, player))
        return 0;
    return 1;
}
