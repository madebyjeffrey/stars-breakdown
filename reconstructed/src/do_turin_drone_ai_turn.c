#include "core.h"
#include <string.h>

/* Typed semantic reconstruction of DoTurinDroneAiTurn, NE 18:3670-58B9.
 * The fleet records and production tables remain behind callbacks, but the
 * phase order is explicit: research setup, design maintenance, planet
 * production, fleet dispatch, basic AI tasks, and queue finalization. */

#define TURIN_ARMADA_START 3
#define TURIN_ARMADA_RAMP 130
#define TURIN_ARMADA_MAX 50
#define TURIN_SUPPORT_RAMP 115
#define TURIN_SUPPORT_MAX 12
#define TURIN_SPLIT_TURN 60
#define TURIN_RESOURCE_COUNT 4
#define TURIN_HULL_BATCH 5

static StarsU16 turin_packed_u16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)((StarsU16)raw[offset] |
        ((StarsU16)raw[offset + 1U] << 8));
}

static StarsI32 turin_packed_i32(const StarsU8 *raw, StarsU16 offset)
{
    StarsU32 value;

    value = (StarsU32)turin_packed_u16(raw, offset) |
        ((StarsU32)turin_packed_u16(raw,
        (StarsU16)(offset + 2U)) << 16);
    return (StarsI32)value;
}

StarsI16 stars_turin_planet_from_packed(const StarsU8 *raw, StarsU16 size,
    StarsTurinTurnPlanet *out)
{
    StarsI16 index;

    if (!raw || !out || size < 56U)
        return 0;
    memset(out, 0, sizeof *out);
    out->id = (StarsI16)turin_packed_u16(raw, 0U);
    out->owner = (StarsI16)turin_packed_u16(raw, 2U);
    out->visibility_flags = raw[4];
    out->packed_flags = turin_packed_u16(raw, 4U);
    for (index = 0; index < 3; ++index)
        out->mineral_concentration[index] = raw[9 + index];
    for (index = 0; index < 4; ++index)
        out->current_minerals[index] = turin_packed_i32(raw,
            (StarsU16)(28U + (StarsU16)index * 4U));
    return 1;
}

StarsI16 stars_turin_fleet_from_packed(const StarsU8 *raw, StarsU16 size,
    StarsI16 order_kind, StarsU8 *raw_plord, StarsTurinTurnFleet *out)
{
    StarsI16 index;

    if (!raw || !out || size < 124U)
        return 0;
    memset(out, 0, sizeof *out);
    out->id = (StarsI16)turin_packed_u16(raw, 0U);
    out->owner = (StarsI16)turin_packed_u16(raw, 2U);
    out->planet_id = (StarsI16)turin_packed_u16(raw, 6U);
    out->cord = (StarsI16)turin_packed_u16(raw, 98U);
    out->order_kind = order_kind;
    out->raw_plord = raw_plord;
    for (index = 0; index < 16; ++index)
        out->ship_counts[index] = (StarsI16)turin_packed_u16(raw,
            (StarsU16)(12U + (StarsU16)index * 2U));
    for (index = 0; index < 5; ++index)
        out->current_cargo[index] = turin_packed_i32(raw,
            (StarsU16)(76U + (StarsU16)index * 4U));
    return 1;
}

StarsI16 stars_turin_ship_queue_present(const StarsU8 *raw, StarsU16 size,
    StarsI16 *has_ship)
{
    StarsU16 index;
    StarsU16 count;
    StarsU16 offset;
    StarsU32 item;

    if (!raw || !has_ship || size < 4U)
        return 0;
    count = raw[3];
    if (size < (StarsU16)(4U + count * 4U))
        return 0;
    *has_ship = 0;
    for (index = 0; index < count; ++index) {
        offset = (StarsU16)(4U + index * 4U);
        item = (StarsU32)turin_packed_u16(raw, offset) |
            ((StarsU32)turin_packed_u16(raw,
            (StarsU16)(offset + 2U)) << 16);
        if (((item >> 17) & 7UL) == 2UL &&
                ((item >> 10) & 0x7fUL) < 16UL) {
            *has_ship = 1;
            break;
        }
    }
    return 1;
}

static StarsI16 turin_potent_rob_war_fleet(
    const StarsTurinTurnFleet *fleet, StarsU8 threshold)
{
    StarsU16 strength;
    StarsI16 index;

    strength = 0U;
    for (index = 2; index <= 5; ++index)
        strength = (StarsU16)(strength +
            (StarsU16)fleet->ship_counts[index]);
    for (index = 6; index <= 7; ++index)
        strength = (StarsU16)(strength +
            (StarsU16)((StarsU16)fleet->ship_counts[index] * 2U));
    return (StarsI16)((StarsI16)strength >= (StarsI16)threshold);
}

static StarsI16 turin_armada_potency(StarsI16 turn)
{
    StarsI16 value;

    value = TURIN_ARMADA_START;
    if (turn > TURIN_ARMADA_RAMP)
        value = (StarsI16)((turn - 120) / 20 + TURIN_ARMADA_START);
    if (value > TURIN_ARMADA_MAX)
        value = TURIN_ARMADA_MAX;
    return value;
}

static StarsI16 turin_support_potency(StarsI16 turn)
{
    StarsI16 value;

    value = 6;
    if (turn > TURIN_SUPPORT_RAMP)
        value = (StarsI16)((turn - 100) / 22 + 6);
    if (value > TURIN_SUPPORT_MAX)
        value = TURIN_SUPPORT_MAX;
    return value;
}

static StarsI16 turin_merge(const StarsTurinDroneAiTurnOps *ops,
    StarsI16 mask)
{
    if (ops->merge_needed && !ops->merge_needed(ops->context, mask))
        return 1;
    return ops->merge_shdefs ? ops->merge_shdefs(ops->context, mask) : 1;
}

static StarsI16 turin_queue_request(const StarsTurinDroneAiTurnOps *ops,
    const StarsTurinTurnPlanet *planet, StarsI16 design, StarsI16 quantity,
    StarsTurinDroneAiTurnInput *input, StarsI16 *queued_this_planet)
{
    if (ops->queue_production_full) {
        if (!ops->queue_production_full(ops->context, planet, design,
                quantity, 2, 1))
            return 0;
    } else if (quantity == 1 && ops->queue_production) {
        if (!ops->queue_production(ops->context, planet, design, 1))
            return 0;
    } else {
        return 0;
    }
    if (!*queued_this_planet) {
        ++input->production_queued;
        *queued_this_planet = 1;
    }
    input->production_hulls_queued = (StarsI16)
        (input->production_hulls_queued + quantity);
    return 1;
}

static StarsI16 turin_queue_one(const StarsTurinDroneAiTurnOps *ops,
    const StarsTurinTurnPlanet *planet, StarsI16 design,
    StarsTurinDroneAiTurnInput *input, StarsI16 *queued_this_planet)
{
    return turin_queue_request(ops, planet, design, 1, input,
        queued_this_planet);
}

static StarsI16 turin_queue_hulls(const StarsTurinDroneAiTurnOps *ops,
    const StarsTurinTurnPlanet *planet, StarsI16 design,
    const StarsI32 hull_cost[4], StarsU32 queue_cost[4],
    StarsTurinDroneAiTurnInput *input, StarsI16 *queued_this_planet,
    StarsI16 *exhausted)
{
    StarsU32 remaining[TURIN_RESOURCE_COUNT];
    StarsI16 resource;
    StarsI16 hull;

    *exhausted = 0;
    for (resource = 0; resource < TURIN_RESOURCE_COUNT; ++resource) {
        remaining[resource] = (StarsU32)planet->resources_available[resource] -
            queue_cost[resource];
        if ((remaining[resource] & 0x80000000UL) != 0U) {
            *exhausted = 1;
            return 1;
        }
    }
    for (hull = 0; hull < TURIN_HULL_BATCH; ++hull) {
        for (resource = 0; resource < TURIN_RESOURCE_COUNT;
             ++resource) {
            remaining[resource] -= (StarsU32)hull_cost[resource];
            if ((remaining[resource] & 0x80000000UL) != 0U) {
                *exhausted = 1;
                return 1;
            }
        }
        if (!turin_queue_one(ops, planet, design, input,
                queued_this_planet))
            return 0;
        for (resource = 0; resource < TURIN_RESOURCE_COUNT; ++resource)
            queue_cost[resource] += (StarsU32)hull_cost[resource];
    }
    return 1;
}

static StarsI16 turin_fleet_sum(const StarsTurinTurnFleet *fleet,
    StarsI16 first, StarsI16 last)
{
    StarsI16 slot;
    StarsI16 total;

    total = 0;
    for (slot = first; slot <= last; ++slot)
        total = (StarsI16)(total + fleet->ship_counts[slot]);
    return total;
}

/* These bands are the ship-role boundaries used by the raw fleet loops:
 * colonizers, transports, ordinary armed designs, support designs, and
 * attack designs. The first-order repair has a narrower raw slot predicate. */
static void turin_fleet_policy(const StarsTurinTurnFleet *fleet,
    StarsTurinFleetPolicy *policy)
{
    StarsU16 first_kind;

    first_kind = fleet->raw_plord ?
        (StarsU16)(fleet->raw_plord[10] & 0x0fU) :
        (StarsU16)fleet->order_kind;
    policy->colonizer_ships = fleet->ship_counts[0];
    policy->transport_ships = fleet->ship_counts[1];
    policy->armed_ships = turin_fleet_sum(fleet, 2, 9);
    policy->support_design_ships = (StarsI16)(fleet->ship_counts[0xc] +
        fleet->ship_counts[0xd]);
    policy->attack_design_ships = (StarsI16)(fleet->ship_counts[0xe] +
        fleet->ship_counts[0xf]);
    policy->total_ships = (StarsI16)(policy->colonizer_ships +
        policy->transport_ships + policy->armed_ships +
        policy->support_design_ships + policy->attack_design_ships);
    policy->needs_order_repair = fleet->ship_counts[2] == 0 &&
        fleet->ship_counts[3] == 0 && fleet->cord < 2 &&
        fleet->ship_counts[1] == 0 && fleet->ship_counts[8] == 0 &&
        fleet->ship_counts[9] == 0 && fleet->ship_counts[13] == 0 &&
        fleet->ship_counts[14] == 0 && fleet->ship_counts[0] == 0 &&
        fleet->ship_counts[10] == 0 && fleet->ship_counts[11] == 0 &&
        fleet->ship_counts[12] != 0 && fleet->cord == 1 &&
        first_kind == 0U;
}

StarsI16 stars_turin_plan_local_transport(
    const StarsTurinTurnFleet *fleet, const StarsTurinTurnPlanet *planet,
    StarsI16 player_id, StarsTurinFleetPlan *plan)
{
    if (!fleet || !planet || !plan)
        return 0;
    plan->action = STARS_TURIN_FLEET_ACTION_NONE;
    plan->target_planet = -1;
    plan->order_kind = 0;
    plan->append = 0;
    plan->reason = 0;
    /* Bind the unambiguous raw local-supply path. Other planet/fleet states
     * remain for the route adapter because the fallback branches are partial. */
    if (fleet->owner != player_id || fleet->planet_id < 0 ||
            fleet->planet_id != planet->id ||
            planet->owner != player_id ||
            planet->current_minerals[3] < 1L)
        return 1;
    plan->action = STARS_TURIN_FLEET_ACTION_TRANSPORT;
    plan->reason = 2;
    return 1;
}

static StarsI16 turin_apply_fleet_action(
    const StarsTurinDroneAiTurnOps *ops,
    const StarsTurinTurnFleet *fleet,
    const StarsTurinFleetPlan *plan)
{
    StarsTurinFleetAction action;

    action = 0;
    switch (plan->action) {
    case STARS_TURIN_FLEET_ACTION_CLEAR_TASK:
        action = ops->clear_task;
        break;
    case STARS_TURIN_FLEET_ACTION_MOVE:
        action = ops->move;
        break;
    case STARS_TURIN_FLEET_ACTION_COLONIZE:
        action = ops->colonize;
        break;
    case STARS_TURIN_FLEET_ACTION_WORMHOLE:
        action = ops->goto_wormhole;
        break;
    case STARS_TURIN_FLEET_ACTION_MOVE_STARBASE:
        action = ops->move_starbase;
        break;
    case STARS_TURIN_FLEET_ACTION_ATTACK:
        action = ops->attack;
        break;
    case STARS_TURIN_FLEET_ACTION_TARGET_ARMADA:
        action = ops->target_armada;
        break;
    case STARS_TURIN_FLEET_ACTION_TRANSPORT:
        action = ops->transport;
        break;
    case STARS_TURIN_FLEET_ACTION_TARGET_FREIGHTER:
        action = ops->target_freighter;
        break;
    default:
        break;
    }
    if (!action)
        action = ops->apply_fleet_action;
    if (!action)
        return 0;
    return action(ops->context, fleet, plan);
}

static StarsI16 turin_mark_planet(StarsTurinDroneAiTurnInput *input,
    StarsTurinTurnPlanet *planet, StarsI16 *production_eligible)
{
    StarsI16 value;
    StarsU8 *ai_record;

    value = 0;
    *production_eligible = 0;
    ai_record = input->raw_ai_planet ?
        input->raw_ai_planet + (StarsU32)(StarsU16)planet->id * 16UL : 0;
    if (ai_record)
        ai_record[9] = 1U;
    if (planet->owner == -1 && planet->visibility_flags > 2) {
        /* The first pass already counted viable worlds. The second pass
         * marks visible unowned worlds without repeating that service. */
        ++input->unowned_opt_marked_count;
        if (planet->mineral_concentration[0] < 0x43)
            value = planet->mineral_concentration[0] / 2;
        else
            value = 0x4b;
        if (planet->mineral_concentration[1] < 0x43)
            value = (StarsI16)(value + planet->mineral_concentration[1] / 2);
        else
            value = (StarsI16)(value + 0x4b);
        if (planet->mineral_concentration[2] < 0x43)
            value = (StarsI16)(value + planet->mineral_concentration[2] / 2);
        else
            value = (StarsI16)(value + 0x4b);
        if (value > 127)
            value = 127;
        if (ai_record)
            ai_record[1] = (StarsU8)value;
        ++input->unowned_good_count;
    }
    if (planet->owner == input->player_id) {
        if (input->ops->planet_desirability)
            value = input->ops->planet_desirability(input->ops->context,
                planet, input->player_id);
        else
            value = planet->desirability;
        if (value < 0) {
            ++input->owned_bad_count;
            if (ai_record)
                ai_record[2] = 1U;
        } else
            *production_eligible = 1;
    }
    if (planet->owner != input->player_id && planet->owner != -1) {
        if (!input->ops->planet_opt)
            return 0;
        if (ai_record)
            ai_record[10] = (StarsU8)(((planet->packed_flags >> 9) & 1U) + 1U);
        value = input->ops->planet_opt(input->ops->context, planet,
            input->player_id);
        if (value > 0) {
            ++input->enemy_viable_count;
            if (ai_record)
                ai_record[3] = (StarsU8)value;
        }
    }
    if (input->ops->visit_planet &&
            !input->ops->visit_planet(input->ops->context, planet, value))
        return 0;
    return 1;
}

StarsI16 stars_do_turin_drone_ai_turn(StarsTurinDroneAiTurnInput *input)
{
    const StarsTurinDroneAiTurnOps *ops;
    StarsI16 budget;
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
    StarsI16 designs[8];
    StarsI16 i;
    StarsI16 role;
    StarsI16 role_index;
    StarsI16 queued_this_planet;
    StarsI16 support_queued;
    StarsI16 exhausted;
    StarsI16 production_eligible;
    StarsU32 queue_cost[4];
    StarsU32 limit;
    StarsI32 seed_remaining;
    static const StarsI16 production_roles[4] = {3, 0, 7, 5};
    StarsTurinFleetPlan fleet_plan;
    StarsTurinFleetPolicy fleet_policy;
    StarsU8 split_mask[16];

    if (!input || !input->ops || input->turn < 0 ||
            input->planet_count < 0 ||
            (input->planet_count > 0 && !input->planets) ||
            input->fleet_count < 0 ||
            (input->fleet_count > 0 && !input->fleets))
        return 0;
    ops = input->ops;
    if (input->raw_ai_planet) {
        for (i = 0; i < input->planet_count; ++i)
            if (input->planets[i].id < 0 ||
                    (StarsU32)(StarsU16)input->planets[i].id * 16UL +
                    16UL > input->raw_ai_planet_size)
                return 0;
    }
    for (i = 0; i < 8; ++i)
        designs[i] = -1;
    input->unowned_good_count = 0;
    input->owned_bad_count = 0;
    input->enemy_viable_count = 0;
    input->unowned_viable_count = 0;
    input->unowned_opt_marked_count = 0;
    input->research_order = -1;
    input->latest_starbase_design = -1;
    research_order = designs[7];
    latest_starbase_design = -1;
    if (ops->ensure_ai) {
        research_order = ops->ensure_ai(ops->context, 15,
            &latest_starbase_design);
        if (research_order < 0)
            return 0;
    }
    input->research_order = research_order;
    input->latest_starbase_design = latest_starbase_design;
    if (!turin_merge(ops, (StarsI16)-0x1f10) ||
            !turin_merge(ops, 0x1000) || !turin_merge(ops, 0x0c00) ||
            !turin_merge(ops, 0x000c))
        return 0;
    input->armada_potency[0] = (StarsU8)turin_armada_potency(input->turn);
    input->armada_potency[1] = (StarsU8)(input->armada_potency[0] / 2);
    input->armada_potency[2] = (StarsU8)turin_support_potency(input->turn);
    input->armada_potency[3] = (StarsU8)(input->armada_potency[2] / 2 - 1);
    if (input->armada_potency[3] >= 4)
        input->armada_potency[3] = 3;
    budget = input->turn < 120 ? 50 :
        (input->turn < 200 ? 70 : 100);
    if (ops->check_design_status) {
        for (i = 0; i < 16; ++i)
            input->split_status_mask[i] = 0;
        if (!ops->check_design_status(ops->context, 6, 7, budget,
                &designs[0], input->split_status_mask) ||
            !ops->check_design_status(ops->context, 8, 9, budget,
                &designs[1], input->split_status_mask) ||
            !ops->check_design_status(ops->context, 13, 14, budget,
                &designs[2], input->split_status_mask) ||
            !ops->check_design_status(ops->context, 4, 5, budget,
                &designs[3], input->split_status_mask) ||
            !ops->check_design_status(ops->context, 12, 12, budget,
                &designs[4], input->split_status_mask) ||
            !ops->check_design_status(ops->context, 15, 15, budget,
                &designs[5], input->split_status_mask) ||
            (input->player_tech_level_valid &&
             input->player_tech_level < 7 ?
                (designs[6] = -1, 0) :
                !ops->check_design_status(ops->context, 2, 3, budget,
                    &designs[6], input->split_status_mask)) ||
            !ops->check_design_status(ops->context, 10, 11, budget,
                &designs[7], input->split_status_mask))
            return 0;
    }
    for (i = 0; i < 8; ++i)
        input->design_selection[i] = designs[i];
    if (input->turn > TURIN_SPLIT_TURN && ops->split_shdefs) {
        for (i = 0; i < 16; ++i)
            split_mask[i] = input->split_status_mask[i];
        if (!ops->split_shdefs(ops->context, split_mask))
            return 0;
    }
    if (ops->ensure_shdefs &&
            !ops->ensure_shdefs(ops->context, research_order))
        return 0;
    input->production_queued = 0;
    input->production_hulls_queued = 0;
    /* Original first pass sees every unowned world before production at
     * any owned planet; the second pass below remains in planet order. */
    for (i = 0; i < input->planet_count; ++i)
        if (input->planets[i].owner == -1 &&
                input->planets[i].visibility_flags > 2 &&
                ops->planet_opt &&
                ops->planet_opt(ops->context, &input->planets[i],
                input->player_id) > 0)
            ++input->unowned_viable_count;
    for (i = 0; i < input->planet_count; ++i)
        if (!turin_mark_planet(input, &input->planets[i],
                &production_eligible))
            return 0;
        else if (input->planets[i].owner == input->player_id &&
                production_eligible &&
                input->planets[i].production_slot_open) {
            if (ops->prepare_production &&
                    !ops->prepare_production(ops->context,
                    &input->planets[i], input->turn))
                return 0;
            if (input->production_policy_valid &&
                    !ops->queue_production && !ops->queue_production_full)
                return 0;
            if (input->design_1_policy_valid &&
                    !ops->queue_production && !ops->queue_production_full)
                return 0;
            if (input->minelayer_policy_valid &&
                    input->design_12_eligible &&
                    (!ops->random_value || !ops->queue_production_full))
                return 0;
            queued_this_planet = 0;
            support_queued = 0;
            if (!input->planets[i].production_queue_has_ship &&
                    (ops->queue_production || ops->queue_production_full)) {
                if (input->turn == 0) {
                    seed_remaining = input->plan_max;
                    while (seed_remaining > 0) {
                        if (!turin_queue_one(ops, &input->planets[i], 0,
                                input, &queued_this_planet))
                            return 0;
                        seed_remaining -= seed_remaining < 0xbf ? 30 : 100;
                    }
                } else if (input->base_design_inactive &&
                        input->base_design_hull_id == 5) {
                    limit = input->plan_max / 4U;
                    if (limit > 0x20U)
                        limit = 0x20U;
                    if (input->base_design_existing <= 0xffffUL &&
                            input->base_design_existing < limit &&
                            input->base_design_built / 10U <
                            input->base_design_existing &&
                            !turin_queue_one(ops, &input->planets[i], 0,
                            input, &queued_this_planet))
                        return 0;
                }
                if (input->production_policy_valid &&
                        input->player_tech_2 > 4 && designs[1] >= 0) {
                    StarsI16 desired;
                    StarsI16 expanded;
                    StarsI16 should_queue;

                    desired = (StarsI16)(input->ai_planet_scale * 2);
                    if (desired < input->player_planet_count / 10)
                        desired = (StarsI16)
                            (input->player_planet_count / 10);
                    expanded = (StarsI16)((StarsI32)desired * 10L / 7L);
                    should_queue = input->design_8_9_existing_total < desired;
                    if (!should_queue &&
                            input->design_8_9_existing_total < expanded) {
                        if (!ops->random_value)
                            return 0;
                        should_queue = ops->random_value(ops->context, 4) == 0;
                    }
                    if (should_queue)
                        if (!turin_queue_one(ops, &input->planets[i],
                                designs[1], input, &queued_this_planet))
                            return 0;
                }
                if (input->design_1_policy_valid &&
                        (input->unowned_viable_count != 0 ||
                        input->enemy_viable_count != 0) &&
                        input->design_1_existing < 2UL) {
                    for (role = 0; role < 4; ++role)
                        if (!turin_queue_one(ops, &input->planets[i], 1,
                                input, &queued_this_planet))
                            return 0;
                }
                if (input->minelayer_policy_valid &&
                        input->design_12_eligible) {
                    StarsI16 local_minelayers;
                    StarsI16 fleet_index;

                    if (ops->random_value(ops->context, 3) == 0) {
                        local_minelayers = 0;
                        for (fleet_index = 0;
                                fleet_index < input->fleet_count;
                                ++fleet_index)
                            if (input->fleets[fleet_index].planet_id ==
                                    input->planets[i].id &&
                                    input->fleets[fleet_index].owner ==
                                    input->player_id &&
                                    input->fleets[fleet_index].ship_counts[12]
                                    > 0) {
                                local_minelayers = input->fleets[
                                    fleet_index].ship_counts[12];
                                break;
                            }
                        if ((local_minelayers < 10 ||
                                (local_minelayers < 17 &&
                                ops->random_value(ops->context, 8) == 0)) &&
                                ops->random_value(ops->context,
                                (StarsI16)(local_minelayers * 2 + 1)) == 0)
                            if (!turin_queue_request(ops,
                                    &input->planets[i], 12, 3, input,
                                    &queued_this_planet))
                                return 0;
                    }
                }
                if (input->support_policy_valid && designs[2] >= 0) {
                    StarsI16 fleet_index;

                    if (!ops->queue_production_full)
                        return 0;
                    for (fleet_index = 0;
                            fleet_index < input->fleet_count;
                            ++fleet_index) {
                        const StarsTurinTurnFleet *fleet;

                        fleet = &input->fleets[fleet_index];
                        if (fleet->planet_id != input->planets[i].id ||
                                fleet->owner != input->player_id ||
                                !(ops->potent_rob_war_fleet ?
                                ops->potent_rob_war_fleet(ops->context,
                                fleet, 2) :
                                turin_potent_rob_war_fleet(fleet,
                                input->armada_potency[0])))
                            continue;
                        if ((StarsI16)(fleet->ship_counts[13] +
                                fleet->ship_counts[14]) >=
                                (StarsI16)input->armada_potency[2]) {
                            if (!turin_queue_request(ops,
                                    &input->planets[i], designs[2], 4,
                                    input, &queued_this_planet))
                                return 0;
                            support_queued = 1;
                        }
                        break;
                    }
                }
                if (!support_queued) {
                    for (role = 0; role < 4; ++role)
                        queue_cost[role] = (StarsU32)
                            input->planets[i].production_queue_cost[role];
                    if (!input->production_designs_valid) {
                        if (designs[3] >= 0 && !turin_queue_hulls(ops,
                                &input->planets[i], designs[3],
                                input->planets[i].hull_cost, queue_cost,
                                input, &queued_this_planet, &exhausted))
                            return 0;
                    } else {
                        for (role_index = 0; role_index < 4; ++role_index) {
                            role = production_roles[role_index];
                            if (designs[role] < 0 ||
                                    (role == 5 &&
                                    !input->design_15_inactive))
                                continue;
                            limit = role == 3 ?
                                (StarsU32)(input->plan_max / 24U + 4U) :
                                (role == 7 ?
                                (StarsU32)(input->plan_max / 4U + 12U) :
                                (StarsU32)(input->plan_max / 12U + 8U));
                            if (input->design_existing[role] > 0xffffUL ||
                                    input->design_existing[role] >= limit)
                                continue;
                            if (!turin_queue_hulls(ops, &input->planets[i],
                                    designs[role],
                                    input->design_hull_cost[role],
                                    queue_cost, input, &queued_this_planet,
                                    &exhausted))
                                return 0;
                            if (exhausted)
                                break;
                        }
                    }
                }
            }
            if (ops->finish_production &&
                    !ops->finish_production(ops->context,
                    &input->planets[i], queued_this_planet))
                return 0;
        }
    if (ops->prepare_fleets &&
            !ops->prepare_fleets(ops->context, input->turn))
        return 0;
    for (i = 0; i < input->fleet_count; ++i) {
        if (input->fleets[i].owner != input->player_id)
            continue;
        turin_fleet_policy(&input->fleets[i], &fleet_policy);
        if (fleet_policy.needs_order_repair &&
                input->fleets[i].raw_plord &&
                !stars_ai_repair_first_order(
                input->fleets[i].raw_plord))
            return 0;
        if (fleet_policy.needs_order_repair && ops->repair_order &&
                !ops->repair_order(ops->context, &input->fleets[i],
                &fleet_policy, input->armada_potency))
            return 0;
        /* The raw second fleet pass routes turn-zero design-2/3 miners to
         * LAB_1088_5135, which changes the first packed order's kind to 5. */
        if (input->turn == 0 &&
                (input->fleets[i].ship_counts[2] != 0 ||
                 input->fleets[i].ship_counts[3] != 0)) {
            if (input->fleets[i].raw_plord &&
                    !stars_ai_set_first_order_kind(
                    input->fleets[i].raw_plord, 5U))
                return 0;
        } else if (ops->visit_fleet_typed && ops->plan_fleet_action) {
            fleet_plan.action = STARS_TURIN_FLEET_ACTION_NONE;
            fleet_plan.target_planet = -1;
            fleet_plan.order_kind = 0;
            fleet_plan.append = 0;
            fleet_plan.reason = 0;
            if (!ops->plan_fleet_action(ops->context,
                    &input->fleets[i], &fleet_policy, input->turn,
                    &fleet_plan))
                return 0;
            if (fleet_plan.action != STARS_TURIN_FLEET_ACTION_NONE &&
                    !turin_apply_fleet_action(ops,
                    &input->fleets[i], &fleet_plan))
                return 0;
        }
        if (ops->visit_fleet_typed &&
                !ops->visit_fleet_typed(ops->context, &input->fleets[i],
                &fleet_policy, input->armada_potency))
            return 0;
    }
    if (ops->visit_fleets && !ops->visit_fleets(ops->context, input->player_id))
        return 0;
    if (ops->handle_tasks && !ops->handle_tasks(ops->context,
            research_order, latest_starbase_design))
        return 0;
    if (ops->fill_queue && !ops->fill_queue(ops->context))
        return 0;
    return 1;
}
