#include "core.h"
#include <string.h>

static StarsU16 rototill_word(const StarsU8 *raw)
{
    return (StarsU16)((StarsU16)raw[0] | ((StarsU16)raw[1] << 8));
}

static void rototill_bind_fleet(const StarsRototillTurnFleet *source,
    StarsRototillTurnFleet *view)
{
    const StarsU8 *raw;
    StarsI16 index;
    *view = *source;
    raw = source->raw_fleet_record;
    if (!raw)
        return;
    view->id = (StarsI16)rototill_word(raw);
    view->owner = (StarsI16)rototill_word(raw + 2);
    view->planet_id = (StarsI16)rototill_word(raw + 6);
    view->cord = (StarsI16)rototill_word(raw + 98);
    for (index = 0; index < 16; ++index)
        view->ship_counts[index] = (StarsI16)rototill_word(
            raw + 12 + index * 2);
}

static void rototill_bind_planet(const StarsRototillAiTurnInput *input,
    const StarsRototillTurnPlanet *source, StarsRototillTurnPlanet *view);

/* Original NE 20:~2470 marks an unowned target in vlpbAiPlanet[id][1]
 * when the fleet has ships in slots 7/8 and at least one order.  With no
 * current planet, the second 18-byte PLORD order supplies its target ID. */
static StarsI16 rototill_unowned_marker_target(
    const StarsRototillAiTurnInput *input,
    const StarsRototillTurnFleet *fleet, StarsI16 *target,
    StarsI16 *occupied)
{
    StarsI16 i;
    StarsRototillTurnPlanet view;

    *target = -1;
    *occupied = 0;
    if (fleet->owner != input->player_id ||
            (fleet->ship_counts[7] == 0 && fleet->ship_counts[8] == 0) ||
            fleet->cord < 1)
        return 1;
    if (fleet->planet_id == -1) {
        if (fleet->cord < 2)
            return 1;
        if (!fleet->raw_plord || fleet->raw_plord_bytes < 40U ||
                fleet->raw_plord[3] < 2U)
            return 0;
        *target = (StarsI16)rototill_word(fleet->raw_plord + 26);
        return *target >= 0;
    }
    if (fleet->planet_id < 0)
        return 0;
    for (i = 0; i < input->planet_count; ++i) {
        rototill_bind_planet(input, &input->planets[i], &view);
        if (view.id == fleet->planet_id) {
            if (view.owner == -1)
                *target = fleet->planet_id;
            else
                *occupied = 1;
            return 1;
        }
    }
    return 0;
}

/* Callback views are transient: the packed record remains caller-owned. */
static void rototill_bind_planet(const StarsRototillAiTurnInput *input,
    const StarsRototillTurnPlanet *source, StarsRototillTurnPlanet *view)
{
    const StarsU8 *raw;
    *view = *source;
    raw = source->raw_planet_record;
    if (raw) {
        view->id = (StarsI16)rototill_word(raw);
        view->owner = rototill_word(raw + 2) == 0xffffU ? -1 :
            (StarsI16)rototill_word(raw + 2);
        view->planet_class = raw[4];
        view->visibility_flags = rototill_word(raw + 4);
        view->mineral_concentration[0] = raw[9];
        view->mineral_concentration[1] = raw[10];
        view->mineral_concentration[2] = raw[11];
        view->mineral_reserve = rototill_word(raw + 40);
        view->mineral_rate_high = (StarsI16)rototill_word(raw + 42);
    }
    if (!view->raw_ai_planet && input->raw_ai_planet_table) {
        view->raw_ai_planet = input->raw_ai_planet_table +
            (StarsU16)view->id * 16U;
        view->raw_ai_planet_size = 16U;
    }
}

/* Typed semantic reconstruction of the planet classification, dispatch, and
 * finalization phases of DoRototillAiTurn, NE 20:1E22-301F.  The original's queue and
 * fleet-order helpers are represented as typed callbacks while those shared
 * operations are connected. */

static StarsU16 rototill_mineral_score(const StarsRototillTurnPlanet *planet)
{
    StarsU16 score;
    StarsI16 i;

    score = 0;
    for (i = 0; i < 3; ++i) {
        if (planet->mineral_concentration[i] < 0x43)
            score = (StarsU16)(score + planet->mineral_concentration[i] / 2);
        else
            score = (StarsU16)(score + 0x4b);
    }
    if (score > 127)
        score = 127;
    return score;
}

static StarsI16 rototill_production_entry(
    const StarsRototillTurnPlanet *planet, StarsI16 player_id,
    StarsI16 desirability)
{
    if (planet->owner != player_id && planet->owner != -1)
        return 0;
    if (planet->owner == player_id && desirability < 0)
        return 0;
    return (planet->visibility_flags & 0x0200U) != 0U &&
        (planet->mineral_rate_high > 0 ||
         (planet->mineral_rate_high == 0 &&
          planet->mineral_reserve > 999U));
}

/* Original NE 20:~2230 uses the same four-byte PLPROD item layout as the
 * other AI turns, but bypasses when type 2 has design index >= 17. */
static StarsI16 rototill_queue_bypass(const StarsRototillTurnPlanet *planet)
{
    StarsU16 index;
    StarsU16 count;
    StarsU32 item;
    const StarsU8 *raw;

    raw = planet->raw_prod_queue;
    if (!raw)
        return 0;
    count = raw[3];
    for (index = 0; index < count; ++index) {
        raw = planet->raw_prod_queue + 4U + index * 4U;
        item = (StarsU32)raw[0] | ((StarsU32)raw[1] << 8) |
            ((StarsU32)raw[2] << 16) | ((StarsU32)raw[3] << 24);
        if (((item >> 17) & 7UL) == 2UL &&
                ((item >> 10) & 0x7fUL) >= 17UL)
            return 1;
    }
    return 0;
}

static StarsI16 rototill_refresh_prod_queue(
    const StarsRototillAiTurnOps *ops,
    const StarsRototillTurnPlanet *planet,
    StarsRototillTurnPlanet *active)
{
    const StarsU8 *queue;
    StarsU16 size;

    queue = 0;
    size = 0;
    if (!ops->current_prod_queue(ops->context, planet, &queue, &size) ||
            !queue || size < 4U ||
            size < (StarsU16)(4U + (StarsU16)queue[3] * 4U))
        return 0;
    *active = *planet;
    active->raw_prod_queue = queue;
    active->raw_prod_queue_size = size;
    return 1;
}

static StarsI16 rototill_fleet_sum(const StarsRototillTurnFleet *fleet,
    StarsI16 first, StarsI16 last)
{
    StarsI16 slot;
    StarsI16 total;

    total = 0;
    for (slot = first; slot <= last; ++slot)
        total = (StarsI16)(total + fleet->ship_counts[slot]);
    return total;
}

static void rototill_fleet_policy(const StarsRototillTurnFleet *fleet,
    StarsRototillFleetPolicy *policy)
{
    policy->colonizer_ships = fleet->ship_counts[0];
    policy->transport_ships = fleet->ship_counts[1];
    policy->armed_ships = rototill_fleet_sum(fleet, 2, 9);
    policy->total_ships = (StarsI16)(policy->colonizer_ships +
        policy->transport_ships + policy->armed_ships);
    policy->needs_order_repair = fleet->cord < 2 &&
        policy->total_ships != 0 && fleet->order_kind == 0;
}

/* NE 20:~24F5: a transport with a marked foreign destination replaces the
 * first or second complete order, then marks vlpbAiPlanet[id][3]. Returns
 * 1 when applied, 0 when this branch does not apply, -1 for bad binding. */
static StarsI16 rototill_transport_reroute(
    const StarsRototillAiTurnInput *input,
    const StarsRototillAiTurnOps *ops,
    const StarsRototillTurnFleet *fleet)
{
    StarsI16 is_transport;
    StarsI16 target;
    StarsI16 owner;
    StarsI16 i;
    StarsU16 offset;
    StarsU32 cargo;
    StarsMoveOrder order;
    StarsRototillTurnPlanet planet;
    StarsU8 *ai;
    const StarsU8 *raw;

    if (!input->fleet_transport_reroute_bound ||
            fleet->owner != input->player_id ||
            ((fleet->ship_counts[7] != 0 || fleet->ship_counts[8] != 0) &&
             fleet->cord >= 1))
        return 0;
    raw = fleet->raw_plord;
    if (!raw || fleet->raw_plord_bytes < 22U ||
            !fleet->raw_fleet_record)
        return -1;
    is_transport = ops->is_ai_transport(ops->context, fleet);
    if (is_transport < 0)
        return -1;
    if (!is_transport && fleet->ship_counts[1] == 0)
        return 0;
    target = -1;
    if (fleet->cord < 2)
        target = fleet->planet_id;
    else if (raw[3] >= 2U && fleet->raw_plord_bytes >= 40U &&
            (raw[29] & 0x0fU) == 1U)
        target = (StarsI16)rototill_word(raw + 26U);
    if (target == -1)
        return 0;
    if (target < 0 ||
            (StarsU16)target >= input->raw_ai_planet_record_count ||
            (StarsU16)target >= input->planet_point_count)
        return -1;
    ai = input->raw_ai_planet_table + (StarsU16)target * 16U;
    if (ai[3] == 0U)
        return 0;
    owner = -2;
    for (i = 0; i < input->planet_count; ++i) {
        rototill_bind_planet(input, &input->planets[i], &planet);
        if (planet.id == target) {
            owner = planet.owner;
            break;
        }
    }
    /* A missing planet has no safe race-stat lookup in this bounded view. */
    if (owner < 0 || owner == input->player_id)
        return 0;
    cargo = (StarsU32)fleet->raw_fleet_record[88U] |
        ((StarsU32)fleet->raw_fleet_record[89U] << 8) |
        ((StarsU32)fleet->raw_fleet_record[90U] << 16) |
        ((StarsU32)fleet->raw_fleet_record[91U] << 24);
    if ((StarsI32)cargo <= 0 ||
            ops->race_stat(ops->context, owner, 14) == 8)
        return 0;
    memset(&order, 0, sizeof order);
    order.point = input->planet_points[target];
    order.id = target;
    order.flags = 0x1101U;
    order.reserved[6] = 0U;
    order.reserved[7] = 0x20U;
    offset = rototill_word(raw + 8U) == (StarsU16)target ? 4U : 22U;
    if (fleet->raw_plord_bytes < offset + 18U)
        return -1;
    if (!ops->select_fleet(ops->context, fleet, input->player_id))
        return -1;
    memcpy(fleet->raw_plord + offset, &order, 18U);
    if (!ops->refresh_fleet(ops->context, fleet, input->player_id))
        return -1;
    ai[3] |= 0x80U;
    return 1;
}

/* Second fleet sweep, NE 20:~29D0. The owned selected-planet gate tests
 * minerals and starbase bit, while deep space short circuits both. A ship-1
 * first hull slot <=1 skips FMoveToNearestStarbase; either way, failed
 * starbase routing reaches the original first-order-kind-5 fallback. */
static StarsI16 rototill_first_kind_fallback(
    const StarsRototillAiTurnInput *input,
    const StarsRototillAiTurnOps *ops,
    const StarsRototillTurnFleet *fleet)
{
    StarsU8 *raw;
    StarsI16 i;
    StarsI16 owner;
    StarsI16 found;
    StarsU16 flags;
    StarsU32 planet_cargo;
    StarsI16 starbase_result;
    StarsRototillTurnPlanet planet;

    if ((!input->fleet_first_kind_deep_space_bound &&
         !input->fleet_first_kind_selected_planet_bound) ||
            fleet->owner != input->player_id ||
            fleet->ship_counts[7] != 0 || fleet->ship_counts[8] != 0 ||
            fleet->cord >= 2 || fleet->ship_counts[1] == 0 ||
            (fleet->planet_id == -1 ?
             !input->fleet_first_kind_deep_space_bound :
             !input->fleet_first_kind_selected_planet_bound))
        return 0;
    raw = fleet->raw_fleet_record;
    if (!raw || !fleet->raw_plord || fleet->raw_plord_bytes < 22U ||
            fleet->raw_plord[3] < 1U)
        return -1;
    owner = -1;
    flags = 0;
    planet_cargo = 0;
    if (fleet->planet_id != -1) {
        found = 0;
        for (i = 0; i < input->planet_count; ++i) {
            rototill_bind_planet(input, &input->planets[i], &planet);
            if (planet.id == fleet->planet_id) {
                if (!planet.raw_planet_record)
                    return -1;
                owner = planet.owner;
                flags = planet.visibility_flags;
                planet_cargo = (StarsU32)planet.raw_planet_record[40U] |
                    ((StarsU32)planet.raw_planet_record[41U] << 8) |
                    ((StarsU32)planet.raw_planet_record[42U] << 16) |
                    ((StarsU32)planet.raw_planet_record[43U] << 24);
                found = 1;
                break;
            }
        }
        if (!found)
            return -1;
    }
    if (fleet->planet_id != -1 && owner == input->player_id &&
            ((StarsI32)planet_cargo >= 50L ||
             (flags & 0x0200U) != 0U))
        return 0;
    if ((raw[88U] | raw[89U] | raw[90U] | raw[91U]) != 0U)
        return 0;
    if (!ops->select_fleet(ops->context, fleet, input->player_id))
        return -1;
    if (input->ship1_first_hull_slot_byte > 1U) {
        starbase_result = ops->move_nearest_starbase(ops->context,
            fleet, 0);
        if (starbase_result < 0)
            return -1;
        if (starbase_result > 0)
            return 1;
    }
    if (!ops->select_fleet(ops->context, fleet, input->player_id))
        return -1;
    fleet->raw_plord[10U] = (StarsU8)(
        (fleet->raw_plord[10U] & 0xf0U) | 5U);
    if (!ops->refresh_fleet(ops->context, fleet, input->player_id))
        return -1;
    return 1;
}

/* Second sweep scout branch, NE 20:~28D0. Target search is a separate
 * service, but these gates, selected-fleet order, and low-fuel task-5
 * fallback are owned by DoRototillAiTurn itself. */
static StarsI16 rototill_scout_dispatch(
    const StarsRototillAiTurnInput *input,
    const StarsRototillAiTurnOps *ops,
    const StarsRototillTurnFleet *fleet)
{
    const StarsU8 *raw;
    StarsU32 fuel;
    StarsI16 transport;

    if (!input->fleet_scout_bound || fleet->owner != input->player_id ||
            fleet->ship_counts[7] != 0 || fleet->ship_counts[8] != 0 ||
            fleet->cord >= 2 || fleet->ship_counts[1] != 0)
        return 0;
    transport = ops->is_ai_transport(ops->context, fleet);
    if (transport < 0)
        return -1;
    if (transport != 0 || fleet->ship_counts[13] != 0 ||
            fleet->ship_counts[14] != 0 || fleet->ship_counts[0] == 0)
        return 0;
    raw = fleet->raw_fleet_record;
    if (!raw || !fleet->raw_plord || fleet->raw_plord_bytes < 22U ||
            fleet->raw_plord[3] < 1U)
        return -1;
    if (!ops->select_fleet(ops->context, fleet, input->player_id))
        return -1;
    fuel = (StarsU32)raw[92U] | ((StarsU32)raw[93U] << 8) |
        ((StarsU32)raw[94U] << 16) | ((StarsU32)raw[95U] << 24);
    if (input->ship0_first_hull_slot_byte == 1U && fuel < 2UL) {
        if (!ops->select_fleet(ops->context, fleet, input->player_id))
            return -1;
        fleet->raw_plord[10U] = (StarsU8)(
            (fleet->raw_plord[10U] & 0xf0U) | 5U);
        if (!ops->refresh_fleet(ops->context, fleet, input->player_id))
            return -1;
        return 1;
    }
    if (!ops->target_scout(ops->context, fleet, input->player_id))
        return -1;
    return 1;
}

static StarsI16 rototill_colonizer_supply_dispatch(
    const StarsRototillAiTurnInput *input,
    const StarsRototillAiTurnOps *ops,
    const StarsRototillTurnFleet *fleet)
{
    StarsI16 index;
    StarsRototillTurnPlanet planet;
    StarsI16 target;
    if (!input->fleet_colonizer_supply_bound ||
            fleet->owner != input->player_id ||
            fleet->ship_counts[7] != 0 || fleet->ship_counts[8] != 0 ||
            fleet->cord >= 2 || fleet->ship_counts[1] == 0 ||
            fleet->planet_id < 0)
        return 0;
    for (index = 0; index < input->planet_count; ++index) {
        rototill_bind_planet(input, &input->planets[index], &planet);
        if (planet.id != fleet->planet_id)
            continue;
        if (planet.owner != input->player_id || !planet.raw_planet_record)
            return 0;
        if (!ops->select_fleet(ops->context, fleet, input->player_id))
            return -1;
        target = ops->nearest_colonizable(ops->context, fleet,
            input->player_id);
        if (target < -1 ||
                (target >= 0 &&
                 (!input->fleet_colonizer_marker_bound ||
                  (StarsU16)target >= input->raw_ai_planet_record_count ||
                  (StarsU16)target >= input->planet_point_count)))
            return -1;
        if (!ops->select_fleet(ops->context, fleet, input->player_id) ||
                !ops->transfer_supply(ops->context, fleet,
                    input->player_id) ||
                !ops->refresh_fleet(ops->context, fleet,
                    input->player_id))
            return -1;
        if (target >= 0) {
            ops->colonize_fleet(ops->context, fleet, target);
            input->raw_ai_planet_table[(StarsU16)target * 16U + 15U] = 4U;
        }
        return 1;
    }
    return -1;
}

StarsI16 stars_do_rototill_ai_turn(StarsRototillAiTurnInput *input)
{
    StarsI16 i;
    StarsU16 score;
    const StarsRototillAiTurnOps *ops;
    StarsRototillFleetPolicy fleet_policy;
    StarsI16 latest_starbase_design;
    StarsI16 research_percent;
    StarsI16 reroute;
    StarsI16 first_kind;
    StarsI16 scout;

    if (!input || !input->ops || input->turn < 0 ||
            input->planet_count < 0 || input->fleet_count < 0 ||
            (input->planet_count > 0 && !input->planets) ||
            (input->fleet_count > 0 && !input->fleets))
        return 0;
    ops = input->ops;
    input->current_order = -1;
    input->latest_starbase_design = -1;
    latest_starbase_design = -1;
    research_percent = input->turn < 20 ? 0 : 15;
    if (ops->ensure_ai) {
        input->current_order = ops->ensure_ai(ops->context, research_percent,
            &latest_starbase_design);
        if (input->current_order < 0)
            return 0;
        input->latest_starbase_design = latest_starbase_design;
    } else {
        input->current_order = research_percent;
    }
    input->unowned_viable_count = 0;
    input->owned_bad_count = 0;
    input->productive_count = 0;
    input->mineral_score = 0;
    input->production_items_added = 0;
    input->design_one_queued = 0;
    input->queue_bypass_count = 0;
    if (ops->ensure_shdefs && !ops->ensure_shdefs(ops->context,
            input->current_order))
        return 0;
    for (i = 0; i < input->planet_count; ++i) {
        const StarsRototillTurnPlanet *planet = &input->planets[i];
        if (planet->raw_planet_record &&
                (planet->raw_planet_record_size < 56U ||
                 rototill_word(planet->raw_planet_record) !=
                    (StarsU16)planet->id))
            return 0;
        if (planet->raw_ai_planet && planet->raw_ai_planet_size < 16U)
            return 0;
        if (!planet->raw_ai_planet && input->raw_ai_planet_table &&
                (planet->id < 0 ||
                 (StarsU16)planet->id >= input->raw_ai_planet_record_count ||
                 input->raw_ai_planet_record_count > 4095U))
            return 0;
        if (!ops->current_prod_queue && planet->raw_prod_queue &&
                (planet->raw_prod_queue_size < 4U ||
                 planet->raw_prod_queue_size <
                    (StarsU16)(4U +
                        (StarsU16)planet->raw_prod_queue[3] * 4U)))
            return 0;
    }
    for (i = 0; i < input->fleet_count; ++i) {
        const StarsRototillTurnFleet *fleet = &input->fleets[i];
        if (fleet->raw_fleet_record &&
                (fleet->raw_fleet_record_size < 124U ||
                 rototill_word(fleet->raw_fleet_record) !=
                    (StarsU16)fleet->id))
            return 0;
        if (fleet->raw_plord &&
                (fleet->raw_plord_bytes < 4U ||
                 fleet->raw_plord_bytes <
                    (StarsU16)(4U + (StarsU16)fleet->raw_plord[3] * 18U)))
            return 0;
    }
    if (input->fleet_unowned_marker_bound ||
            input->fleet_occupied_repair_bound ||
            input->fleet_transport_reroute_bound) {
        if (input->fleet_unowned_marker_bound &&
                (!input->raw_ai_planet_table ||
                 input->raw_ai_planet_record_count > 4095U))
            return 0;
        if (input->fleet_transport_reroute_bound &&
                (!input->raw_ai_planet_table ||
                 input->raw_ai_planet_record_count > 4095U ||
                 !input->planet_points || !ops->is_ai_transport ||
                 !ops->race_stat || !ops->select_fleet ||
                 !ops->refresh_fleet))
            return 0;
        for (i = 0; i < input->fleet_count; ++i) {
            StarsRototillTurnFleet view;
            StarsI16 target;
            StarsI16 occupied;
            StarsI16 reroute_target;
            rototill_bind_fleet(&input->fleets[i], &view);
            if (input->fleet_transport_reroute_bound &&
                    view.owner == input->player_id &&
                    ((view.ship_counts[7] == 0 &&
                      view.ship_counts[8] == 0) || view.cord < 1)) {
                if (!view.raw_fleet_record || !view.raw_plord ||
                        view.raw_plord_bytes < 22U)
                    return 0;
                reroute_target = -1;
                if (view.cord < 2)
                    reroute_target = view.planet_id;
                else if (view.raw_plord[3] >= 2U &&
                        view.raw_plord_bytes >= 40U &&
                        (view.raw_plord[29] & 0x0fU) == 1U)
                    reroute_target = (StarsI16)rototill_word(
                        view.raw_plord + 26U);
                if (reroute_target != -1 &&
                        (reroute_target < 0 ||
                         (StarsU16)reroute_target >=
                            input->raw_ai_planet_record_count ||
                         (StarsU16)reroute_target >=
                            input->planet_point_count))
                    return 0;
            }
            if (input->fleet_unowned_marker_bound ||
                    input->fleet_occupied_repair_bound) {
                if (!rototill_unowned_marker_target(input, &view, &target,
                        &occupied) ||
                        (input->fleet_unowned_marker_bound && target >= 0 &&
                         (StarsU16)target >=
                            input->raw_ai_planet_record_count))
                    return 0;
            } else {
                target = -1;
                occupied = 0;
            }
            if (occupied && input->fleet_occupied_repair_bound &&
                    (!view.raw_fleet_record || !view.raw_plord ||
                     !ops->select_fleet || !ops->refresh_fleet ||
                     !ops->clear_ai_current_task))
                return 0;
        }
    }
    if (input->fleet_first_kind_deep_space_bound ||
            input->fleet_first_kind_selected_planet_bound) {
        if (!ops->select_fleet || !ops->refresh_fleet ||
                (input->ship1_first_hull_slot_byte > 1U &&
                 !ops->move_nearest_starbase))
            return 0;
        for (i = 0; i < input->fleet_count; ++i) {
            StarsRototillTurnFleet view;
            const StarsU8 *raw;
            rototill_bind_fleet(&input->fleets[i], &view);
            if (view.owner != input->player_id ||
                    view.ship_counts[7] != 0 || view.ship_counts[8] != 0 ||
                    view.cord >= 2 || view.ship_counts[1] == 0 ||
                    (view.planet_id == -1 ?
                     !input->fleet_first_kind_deep_space_bound :
                     !input->fleet_first_kind_selected_planet_bound))
                continue;
            raw = view.raw_fleet_record;
            if (!raw || !view.raw_plord ||
                    view.raw_plord_bytes < 22U ||
                    view.raw_plord[3] < 1U)
                return 0;
            if (view.planet_id != -1) {
                StarsI16 j;
                StarsI16 found;
                found = 0;
                for (j = 0; j < input->planet_count; ++j) {
                    StarsRototillTurnPlanet planet;
                    rototill_bind_planet(input, &input->planets[j],
                        &planet);
                    if (planet.id == view.planet_id) {
                        if (!planet.raw_planet_record)
                            return 0;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    return 0;
            }
        }
    }
    if (input->fleet_scout_bound) {
        if (!ops->is_ai_transport || !ops->select_fleet ||
                !ops->refresh_fleet || !ops->target_scout)
            return 0;
        for (i = 0; i < input->fleet_count; ++i) {
            StarsRototillTurnFleet view;
            rototill_bind_fleet(&input->fleets[i], &view);
            if (view.owner == input->player_id &&
                    view.ship_counts[7] == 0 &&
                    view.ship_counts[8] == 0 && view.cord < 2 &&
                    view.ship_counts[1] == 0 &&
                    view.ship_counts[13] == 0 &&
                    view.ship_counts[14] == 0 &&
                    view.ship_counts[0] != 0 &&
                    (!view.raw_fleet_record || !view.raw_plord ||
                     view.raw_plord_bytes < 22U ||
                     view.raw_plord[3] < 1U))
                return 0;
        }
    }
    if ((input->fleet_colonizer_supply_bound &&
            (!ops->select_fleet || !ops->refresh_fleet ||
             !ops->nearest_colonizable || !ops->transfer_supply ||
             !input->raw_ai_planet_table || !input->planet_points)) ||
            (input->fleet_colonizer_marker_bound &&
             (!input->fleet_colonizer_supply_bound ||
              !ops->colonize_fleet)))
        return 0;
    if (input->production_inputs_bound && input->design_one_existing < 0)
        return 0;
    /* The original completes this census before any AI-planet write. */
    for (i = 0; i < input->planet_count; ++i) {
        StarsRototillTurnPlanet view;
        const StarsRototillTurnPlanet *planet;
        StarsI16 value;
        rototill_bind_planet(input, &input->planets[i], &view);
        planet = &view;
        if (planet->owner != -1 || planet->planet_class <= 2)
            continue;
        value = ops->pct_planet_opt_value ?
            ops->pct_planet_opt_value(ops->context, planet,
                input->player_id) : planet->optimal_value;
        if (value > 0) {
            ++input->unowned_viable_count;
            input->mineral_score = (StarsU16)(input->mineral_score +
                rototill_mineral_score(planet));
        }
    }
    for (i = 0; i < input->planet_count; ++i) {
        StarsRototillTurnPlanet view;
        StarsRototillTurnPlanet production_view;
        const StarsRototillTurnPlanet *planet;
        const StarsRototillTurnPlanet *active_planet;
        StarsI16 desirability;
        StarsI16 optimal_value;
        StarsI16 changed;
        StarsU8 *ai_record;

        rototill_bind_planet(input, &input->planets[i], &view);
        planet = &view;
        score = rototill_mineral_score(planet);
        desirability = planet->desirability;
        optimal_value = planet->optimal_value;
        ai_record = planet->raw_ai_planet;
        if (ai_record)
            ai_record[9] = 1;
        if (planet->owner == -1 && planet->planet_class > 2 && ai_record)
            ai_record[1] = (StarsU8)score;
        if (planet->owner == input->player_id &&
                ops->pct_planet_desirability)
            desirability = ops->pct_planet_desirability(ops->context,
                planet, input->player_id);
        if (planet->owner == input->player_id && desirability < 0) {
            ++input->owned_bad_count;
            if (ai_record)
                ai_record[2] = 1;
        }
        if (planet->owner != input->player_id && planet->owner != -1) {
            if (ai_record)
                ai_record[10] = (StarsU8)(
                    ((planet->visibility_flags >> 9) & 1U) + 1U);
            if (ops->pct_planet_opt_value)
                optimal_value = ops->pct_planet_opt_value(ops->context,
                    planet, input->player_id);
            if (optimal_value > 0 && ai_record)
                ai_record[3] = 1;
        }
        if (planet->owner == input->player_id && planet->productive)
            ++input->productive_count;
        if (ops->visit_planet && !ops->visit_planet(ops->context, planet,
                score, input->player_id))
            return 0;
        if (!input->production_inputs_bound ||
                !rototill_production_entry(planet, input->player_id,
                    desirability))
            continue;
        if (!planet->raw_prod_queue && !ops->current_prod_queue)
            return 0;
        if (ops->init_production &&
                !ops->init_production(ops->context, planet))
            return 0;
        active_planet = planet;
        if (ops->current_prod_queue) {
            if (!rototill_refresh_prod_queue(ops, planet,
                    &production_view))
                return 0;
            active_planet = &production_view;
        }
        if (rototill_queue_bypass(active_planet)) {
            ++input->queue_bypass_count;
            if (ops->finish_production &&
                    !ops->finish_production(ops->context,
                        active_planet, 0))
                return 0;
            continue;
        }
        changed = 0;
        if (input->turn == 0) {
            if (!ops->add_raw_queue_item ||
                    !ops->add_raw_queue_item(ops->context, active_planet,
                        0, 1, 2, 1))
                return 0;
            ++input->production_items_added;
            if (ops->current_prod_queue &&
                    !rototill_refresh_prod_queue(ops, planet,
                        &production_view))
                return 0;
            if (!ops->add_raw_queue_item(ops->context, active_planet,
                    0, 1, 2, 1))
                return 0;
            ++input->production_items_added;
            if (ops->current_prod_queue &&
                    !rototill_refresh_prod_queue(ops, planet,
                        &production_view))
                return 0;
            /* The original leaves sStack_78 zero in this opening branch. */
        } else if (!input->design_one_queued &&
                (input->design_one_existing == 0 ||
                 input->design_one_existing + 1L <
                    (StarsI32)input->unowned_viable_count)) {
            input->design_one_queued = 1;
            if (!ops->add_raw_queue_item ||
                    !ops->add_raw_queue_item(ops->context, active_planet,
                        1, 1, 2, 1))
                return 0;
            ++input->production_items_added;
            if (ops->current_prod_queue &&
                    !rototill_refresh_prod_queue(ops, planet,
                        &production_view))
                return 0;
            changed = 1;
        }
        if (ops->finish_production &&
                !ops->finish_production(ops->context,
                    active_planet, changed))
            return 0;
    }
    for (i = 0; i < input->fleet_count; ++i) {
        StarsRototillTurnFleet view;
        const StarsRototillTurnFleet *fleet;
        StarsI16 target;
        StarsI16 occupied;
        rototill_bind_fleet(&input->fleets[i], &view);
        fleet = &view;
        if (fleet->owner == input->player_id && fleet->raw_fleet_record)
            fleet->raw_fleet_record[5] &= 0x7fU;
        reroute = rototill_transport_reroute(input, ops, fleet);
        if (reroute < 0)
            return 0;
        if (reroute > 0)
            continue;
        occupied = 0;
        target = -1;
        if (input->fleet_unowned_marker_bound ||
                input->fleet_occupied_repair_bound)
            rototill_unowned_marker_target(input, fleet, &target,
                &occupied);
        if (input->fleet_unowned_marker_bound && target >= 0)
            input->raw_ai_planet_table[(StarsU16)target * 16U + 1U] |=
                0x80U;
        if (input->fleet_occupied_repair_bound && occupied) {
            if (!ops->select_fleet(ops->context, fleet,
                    input->player_id))
                return 0;
            fleet->raw_fleet_record[98] = 1;
            fleet->raw_fleet_record[99] = 0;
            fleet->raw_plord[3] = 1;
            view.cord = 1;
            if (!ops->refresh_fleet(ops->context, fleet,
                    input->player_id) ||
                    !ops->clear_ai_current_task(ops->context, fleet, 0))
                return 0;
            continue;
        }
        if (fleet->owner == input->player_id &&
                ops->visit_fleet_typed) {
            rototill_fleet_policy(fleet, &fleet_policy);
            if (fleet_policy.needs_order_repair && ops->repair_order &&
                    !ops->repair_order(ops->context, fleet,
                    input->player_id))
                return 0;
            if (!ops->visit_fleet_typed(ops->context, fleet,
                    &fleet_policy, input->player_id))
                return 0;
        }
        if (fleet->owner == input->player_id && ops->visit_fleet &&
                !ops->visit_fleet(ops->context,
                fleet, input->player_id))
            return 0;
    }
    for (i = 0; i < input->fleet_count; ++i) {
        StarsRototillTurnFleet view;
        rototill_bind_fleet(&input->fleets[i], &view);
        first_kind = rototill_first_kind_fallback(input, ops, &view);
        if (first_kind < 0)
            return 0;
        if (rototill_colonizer_supply_dispatch(input, ops, &view) < 0)
            return 0;
        scout = rototill_scout_dispatch(input, ops, &view);
        if (scout < 0)
            return 0;
    }
    if (ops->run_basic_tasks && !ops->run_basic_tasks(ops->context,
            input->current_order, latest_starbase_design, input->player_id))
        return 0;
    if (ops->fill_production_queue &&
            !ops->fill_production_queue(ops->context))
        return 0;
    return 1;
}
