#include "core.h"
#include <string.h>

#define AUTOMITRON_RESOURCE_COUNT 4
#define AUTOMITRON_HULL_BATCH 5
#define AUTOMITRON_PLANET_SCANNED 0x0200U

/* Typed semantic reconstruction of the design-policy, planet/fleet, and
 * finalization phases of DoAutomitronAiTurn, NE 20:01E0-1937.  The original
 * continues into resource-budgeted queue construction and detailed fleet
 * routing; those operations remain callback boundaries here. The packed
 * first-order repair is implemented directly in the owned-fleet loop. */

static StarsI16 automitron_armada_potency(StarsI16 turn)
{
    StarsI16 value;

    value = 3;
    if (turn > 130)
        value = (StarsI16)((turn - 120) / 20 + 3);
    if (value > 50)
        value = 50;
    return value;
}

static StarsI16 automitron_support_potency(StarsI16 turn)
{
    StarsI16 value;

    value = 6;
    if (turn > 115)
        value = (StarsI16)((turn - 100) / 22 + 6);
    if (value > 12)
        value = 12;
    return value;
}

static StarsI16 automitron_can_afford(
    const StarsAutomitronTurnPlanet *planet, StarsI16 hulls)
{
    StarsI32 remaining[AUTOMITRON_RESOURCE_COUNT];
    StarsI16 resource;
    StarsI16 hull;

    for (resource = 0; resource < AUTOMITRON_RESOURCE_COUNT; ++resource) {
        remaining[resource] = planet->resources_available[resource] -
            planet->production_queue_cost[resource];
        if (remaining[resource] < 0)
            return 0;
    }
    for (hull = 0; hull < hulls; ++hull) {
        for (resource = 0; resource < AUTOMITRON_RESOURCE_COUNT;
             ++resource) {
            remaining[resource] -= planet->hull_cost[resource];
            if (remaining[resource] < 0)
                return 0;
        }
    }
    return 1;
}

static StarsI16 automitron_production_entry(
    const StarsAutomitronTurnPlanet *planet, StarsI16 player_id,
    StarsI16 desirability)
{
    if (planet->owner != player_id && planet->owner != -1)
        return 0;
    if (planet->owner == player_id && desirability < 0)
        return 0;
    if ((planet->visibility_flags & AUTOMITRON_PLANET_SCANNED) == 0U)
        return 0;
    if (planet->mineral_rate > 0)
        return 1;
    return planet->mineral_rate >= 0 && planet->mineral_concentration > 0x5db;
}

/* PLPROD has a four-byte header (iprodMac at byte 3) followed by four-byte
 * _prod entries. Original NE 20:~05F0 bypasses all construction branches
 * when (entry >> 17 & 7) == 2 and (entry >> 10 & 0x7f) >= 16. */
static StarsI16 automitron_queue_gate(
    const StarsAutomitronTurnPlanet *planet, StarsI16 fallback)
{
    const StarsU8 *raw;
    StarsU16 index;
    StarsU16 count;
    StarsU32 item;

    raw = planet->raw_prod_queue;
    if (!raw)
        return fallback;
    count = raw[3];
    for (index = 0; index < count; ++index) {
        raw = planet->raw_prod_queue + 4U + index * 4U;
        item = (StarsU32)raw[0] | ((StarsU32)raw[1] << 8) |
            ((StarsU32)raw[2] << 16) | ((StarsU32)raw[3] << 24);
        if (((item >> 17) & 7UL) == 2UL &&
                ((item >> 10) & 0x7fUL) >= 16UL)
            return 0;
    }
    return 1;
}

static StarsU8 *automitron_ai_planet_record(
    const StarsAutomitronAiTurnInput *input,
    const StarsAutomitronTurnPlanet *planet)
{
    if (planet->raw_ai_planet)
        return planet->raw_ai_planet;
    if (input->raw_ai_planet_table && planet->id >= 0 &&
            planet->id < 4096 &&
            (StarsU16)planet->id < input->raw_ai_planet_record_count)
        return input->raw_ai_planet_table + (StarsU16)planet->id * 16U;
    return 0;
}

/* Original NE 20:~0800-0905: first matching owned fleet at this planet,
 * then Random(3), optionally Random(8), then Random(2*csh6+1).  The queue
 * eligibility and design flags are live caller bindings, not guessed from
 * the reduced planet record. */
static StarsI16 automitron_seed_colonizer(
    const StarsAutomitronAiTurnInput *input,
    const StarsAutomitronTurnPlanet *planet, StarsI16 queue_eligible)
{
    const StarsAutomitronAiTurnOps *ops;
    StarsI16 count;
    StarsI16 index;

    ops = input->ops;
    if (!input->colonizer_seed_inputs_bound || !queue_eligible ||
            (input->colonizer_design_flags & 0x0200U) != 0U ||
            !ops->random || !ops->add_raw_queue_item)
        return 0;
    if (ops->random(ops->context, 3) != 0)
        return 0;
    count = 0;
    for (index = 0; index < input->fleet_count; ++index) {
        const StarsAutomitronTurnFleet *fleet;
        fleet = &input->fleets[index];
        if (fleet->owner == input->player_id &&
                fleet->planet_id == planet->id &&
                fleet->ship_counts[6] != 0U) {
            count = (StarsI16)fleet->ship_counts[6];
            break;
        }
    }
    if (count >= 17)
        return 0;
    if (count >= 10 && ops->random(ops->context, 8) != 0)
        return 0;
    if (ops->random(ops->context, (StarsI16)(count * 2 + 1)) != 0)
        return 0;
    if (!ops->add_raw_queue_item(ops->context, planet, 6, 3, 2, 1))
        return -1;
    return 1;
}

/* Original NE 20:~0670-07F0: before the stochastic colonizer seed, queue a
 * support design according to AI-data/owned-planet pressure and technology,
 * then a first colonizer design if viable unowned planets exist. The return
 * value is a two-bit mask of successfully queued items, or -1 on failure. */
static StarsI16 automitron_queue_early(
    const StarsAutomitronAiTurnInput *input,
    const StarsAutomitronTurnPlanet *planet, StarsI16 design_support,
    StarsI16 support_status, StarsI16 queue_eligible)
{
    const StarsAutomitronAiTurnOps *ops;
    StarsI16 pressure;
    StarsI16 queued;
    StarsI16 queue_support;

    ops = input->ops;
    if (!input->early_queue_inputs_bound || !queue_eligible ||
            !ops->add_raw_queue_item ||
            input->ai_data_group_count < 0 ||
            input->ai_data_group_count > 1638 ||
            input->owned_planet_count < 0)
        return 0;
    pressure = (StarsI16)(input->owned_planet_count / 10);
    if ((StarsI16)(input->ai_data_group_count * 2) >= pressure)
        pressure = (StarsI16)(input->ai_data_group_count * 2);
    queued = 0;
    queue_support = 0;
    if (input->tech_level_two > 4 && design_support >= 0) {
        if (support_status < pressure)
            queue_support = 1;
        else if (support_status < (StarsI16)((pressure * 10) / 7) &&
                ops->random && ops->random(ops->context, 4) == 0)
            queue_support = 1;
    }
    if (queue_support) {
        if (!ops->add_raw_queue_item(ops->context, planet,
                design_support, 1, 2, 1))
            return -1;
        queued = 1;
    }
    if (input->unowned_viable_count > 0 && !input->design_one_exists &&
            input->turn > 10) {
        if (!ops->add_raw_queue_item(ops->context, planet, 1, 1, 2, 1))
            return -1;
        queued = (StarsI16)(queued | 2);
    }
    return queued;
}

/* Original NE 20:~0910-09B0: scan fleets in roster order until the first
 * potent owned fleet at this planet. Only that fleet decides whether the
 * four-ship support item is queued; a weaker first potent fleet does not
 * cause a search for a later, stronger one. */
static StarsI16 automitron_queue_support(
    const StarsAutomitronAiTurnInput *input,
    const StarsAutomitronTurnPlanet *planet, StarsI16 design_escort,
    StarsI16 queue_eligible)
{
    const StarsAutomitronAiTurnOps *ops;
    StarsI16 index;

    ops = input->ops;
    if (!queue_eligible || design_escort < 0 ||
            !ops->is_potent_war_fleet || !ops->add_raw_queue_item)
        return 0;
    for (index = 0; index < input->fleet_count; ++index) {
        const StarsAutomitronTurnFleet *fleet;
        fleet = &input->fleets[index];
        if (fleet->owner != input->player_id ||
                fleet->planet_id != planet->id)
            continue;
        if (!ops->is_potent_war_fleet(ops->context, fleet, 2))
            continue;
        if ((StarsI16)(fleet->ship_counts[2] + fleet->ship_counts[3]) <
                (StarsI16)input->armada_potency[2])
            return 0;
        if (!ops->add_raw_queue_item(ops->context, planet,
                design_escort, 4, 2, 1))
            return -1;
        return 1;
    }
    return 0;
}

/* Original NE 20:~09D0-0D21: each of two design families obtains a fresh
 * resource/queue-cost snapshot, then repeats GetTrueHullCost and AddItemToQueue
 * up to five times. A deficit stops the current batch and skips the second
 * family when it occurs in the first. */
static StarsI16 automitron_queue_hull_batch(
    const StarsAutomitronAiTurnInput *input,
    const StarsAutomitronTurnPlanet *planet, StarsI16 design,
    StarsI32 existing, StarsI16 divisor, StarsI16 extra,
    StarsI16 *depleted)
{
    const StarsAutomitronAiTurnOps *ops;
    StarsI32 available[4];
    StarsI32 queue_cost[4];
    StarsI32 remaining[4];
    StarsI32 hull_cost[4];
    StarsI16 resource;
    StarsI16 queued;
    StarsI16 hull;

    ops = input->ops;
    *depleted = 0;
    if (design < 0 ||
            existing >= (StarsI32)(input->game_planet_max / divisor + extra))
        return 0;
    if (!ops->get_production_budget(ops->context, planet, available,
            queue_cost))
        return -1;
    for (resource = 0; resource < 4; ++resource) {
        remaining[resource] = available[resource] - queue_cost[resource];
        if (remaining[resource] < 0) {
            *depleted = 1;
            return 0;
        }
    }
    queued = 0;
    for (hull = 0; hull < 5; ++hull) {
        if (!ops->get_hull_cost(ops->context, input->player_id,
                design, hull_cost))
            return -1;
        for (resource = 0; resource < 4; ++resource) {
            remaining[resource] -= hull_cost[resource];
            if (remaining[resource] < 0) {
                *depleted = 1;
                return queued;
            }
        }
        if (!ops->add_raw_queue_item(ops->context, planet, design, 1, 2, 1))
            return -1;
        ++queued;
    }
    return queued;
}

static StarsI16 automitron_queue_hull_batches(
    const StarsAutomitronAiTurnInput *input,
    const StarsAutomitronTurnPlanet *planet, StarsI16 design_is,
    StarsI16 design_attack, StarsI16 queue_eligible)
{
    const StarsAutomitronAiTurnOps *ops;
    StarsI16 first;
    StarsI16 second;
    StarsI16 depleted;

    ops = input->ops;
    if (!queue_eligible)
        return 0;
    if (input->game_planet_max < 0 ||
            input->is_design_existing < 0 ||
            input->attack_design_existing < 0 ||
            !ops->get_production_budget || !ops->get_hull_cost ||
            !ops->add_raw_queue_item)
        return -1;
    first = automitron_queue_hull_batch(input, planet, design_is,
        input->is_design_existing, 12, 8, &depleted);
    if (first < 0 || depleted)
        return first;
    second = automitron_queue_hull_batch(input, planet, design_attack,
        input->attack_design_existing, 24, 4, &depleted);
    if (second < 0)
        return -1;
    return (StarsI16)(first + second);
}

/* Original Automitron second fleet sweep, NE 20:~0D50-11EA.  The first
 * scout branch sends low-fuel scouts to task 5 before target selection. */
static StarsI16 automitron_low_fuel_scout(
    const StarsAutomitronAiTurnInput *input,
    const StarsAutomitronTurnFleet *fleet)
{
    const StarsAutomitronAiTurnOps *ops;

    ops = input->ops;
    if (!input->scout_fuel_gate_bound || fleet->cord >= 2 ||
            fleet->ship_counts[6] != 0U ||
            fleet->ship_counts[1] != 0U ||
            fleet->is_ai_transport ||
            fleet->ship_counts[2] != 0U ||
            fleet->ship_counts[3] != 0U ||
            fleet->ship_counts[0] == 0U ||
            input->hull0_first_slot_byte >= 10U || fleet->fuel >= 2)
        return 0;
    if (!fleet->raw_plord || fleet->raw_plord_bytes <
            STARS_AI_PLORD_HEADER_SIZE + STARS_AI_PLORD_ORDER_SIZE ||
            !ops->refresh_packed_order ||
            !stars_ai_set_first_order_kind(fleet->raw_plord, 5U))
        return -1;
    return ops->refresh_packed_order(ops->context, fleet) ? 1 : -1;
}

static void automitron_fleet_policy(const StarsAutomitronTurnFleet *fleet,
    StarsAutomitronFleetPolicy *policy)
{
    StarsI16 slot;

    policy->total_ships = 0;
    policy->slot_1_ships = fleet->ship_counts[1];
    policy->slot_2_3_ships = 0;
    policy->slot_6_ships = fleet->ship_counts[6];
    policy->slot_9_10_ships = 0;
    for (slot = 0; slot < 16; ++slot) {
        policy->total_ships = (StarsI16)(policy->total_ships +
            fleet->ship_counts[slot]);
        if (slot == 2 || slot == 3)
            policy->slot_2_3_ships = (StarsI16)(policy->slot_2_3_ships +
                fleet->ship_counts[slot]);
        if (slot == 9 || slot == 10)
            policy->slot_9_10_ships = (StarsI16)(policy->slot_9_10_ships +
                fleet->ship_counts[slot]);
    }
    /* Raw DoAutomitronAiTurn gates this repair on rgcsh[6] at fleet +0x18. */
    policy->needs_order_repair = policy->slot_6_ships != 0 &&
        fleet->cord == 1 && fleet->order_kind == 0;
}

static StarsI16 automitron_check_design_status(
    const StarsAutomitronAiTurnOps *ops, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    if (ops->check_design_status_ex)
        return ops->check_design_status_ex(ops->context, first, last,
            budget, selected, mask);
    if (ops->check_design_status)
        return ops->check_design_status(ops->context, first, last, budget,
            selected);
    return 1;
}

static StarsI16 automitron_apply_fleet_action(
    const StarsAutomitronAiTurnOps *ops,
    const StarsAutomitronTurnFleet *fleet,
    const StarsAutomitronFleetPlan *plan)
{
    StarsAutomitronFleetAction action;

    action = 0;
    switch (plan->action) {
    case STARS_AUTOMITRON_FLEET_ACTION_CLEAR_TASK:
        action = ops->clear_task;
        break;
    case STARS_AUTOMITRON_FLEET_ACTION_MOVE:
        action = ops->move;
        break;
    case STARS_AUTOMITRON_FLEET_ACTION_COLONIZE:
        action = ops->colonize;
        break;
    case STARS_AUTOMITRON_FLEET_ACTION_WORMHOLE:
        action = ops->goto_wormhole;
        break;
    case STARS_AUTOMITRON_FLEET_ACTION_MOVE_STARBASE:
        action = ops->move_starbase;
        break;
    case STARS_AUTOMITRON_FLEET_ACTION_ATTACK:
        action = ops->attack;
        break;
    case STARS_AUTOMITRON_FLEET_ACTION_TARGET_ARMADA:
        action = ops->target_armada;
        break;
    case STARS_AUTOMITRON_FLEET_ACTION_TRANSPORT:
        action = ops->transport;
        break;
    case STARS_AUTOMITRON_FLEET_ACTION_TARGET_FREIGHTER:
        action = ops->target_freighter;
        break;
    default:
        break;
    }
    if (!action)
        action = ops->apply_fleet_action;
    if (action)
        return action(ops->context, fleet, plan);
    if ((plan->action == STARS_AUTOMITRON_FLEET_ACTION_MOVE ||
            plan->action == STARS_AUTOMITRON_FLEET_ACTION_COLONIZE) &&
            plan->packed_order_valid && fleet->raw_plord &&
            fleet->raw_plord_bytes >=
                STARS_AI_PLORD_HEADER_SIZE + STARS_AI_PLORD_ORDER_SIZE &&
            ops->refresh_packed_order) {
        StarsU16 capacity;
        capacity = (StarsU16)((fleet->raw_plord_bytes -
            STARS_AI_PLORD_HEADER_SIZE) / STARS_AI_PLORD_ORDER_SIZE);
        if (!stars_ai_move_order_to_plord(fleet->raw_plord, capacity,
                &plan->packed_order, plan->append))
            return 0;
        return ops->refresh_packed_order(ops->context, fleet);
    }
    return 0;
}

StarsI16 stars_do_automitron_ai_turn(StarsAutomitronAiTurnInput *input)
{
    const StarsAutomitronAiTurnOps *ops;
    StarsI16 budget;
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
    StarsI16 research_percent;
    StarsI16 selected;
    StarsI16 design_is;
    StarsI16 design_support;
    StarsI16 design_escort;
    StarsI16 design_attack;
    StarsI16 support_status;
    StarsI16 queued;
    StarsI16 i;
    StarsU8 split_mask[16];
    StarsAutomitronFleetPolicy fleet_policy;
    StarsAutomitronFleetPlan fleet_plan;

    if (!input || !input->ops || input->turn < 0 ||
            input->planet_count < 0 || input->fleet_count < 0 ||
            (input->planet_count > 0 && !input->planets) ||
            (input->fleet_count > 0 && !input->fleets))
        return 0;
    ops = input->ops;
    input->unowned_viable_count = 0;
    input->owned_bad_count = 0;
    input->research_order = -1;
    input->latest_starbase_design = -1;
    research_order = -1;
    latest_starbase_design = -1;
    research_percent = input->turn < 10 ? 0 : 20;
    if (ops->ensure_ai) {
        research_order = ops->ensure_ai(ops->context, research_percent,
            &latest_starbase_design);
        if (research_order < 0)
            return 0;
        input->research_order = research_order;
        input->latest_starbase_design = latest_starbase_design;
        if (ops->ensure_shdefs && !ops->ensure_shdefs(ops->context,
                research_order))
            return 0;
    }
    if (input->turn < 51) {
        if (input->turn > 30 && ops->merge_shdefs &&
                !ops->merge_shdefs(ops->context, 0x4000))
            return 0;
    } else if (ops->merge_shdefs &&
            (!ops->merge_shdefs(ops->context, 0x1e0c) ||
             !ops->merge_shdefs(ops->context, 0x0040) ||
             !ops->merge_shdefs(ops->context, 0x4000)))
        return 0;
    /* Validate the full table binding before changing any planet record. */
    for (i = 0; i < input->planet_count; ++i) {
        const StarsAutomitronTurnPlanet *planet = &input->planets[i];
        if (planet->raw_ai_planet && planet->raw_ai_planet_size < 16U)
            return 0;
        if (planet->raw_prod_queue &&
                (planet->raw_prod_queue_size < 4U ||
                 planet->raw_prod_queue_size <
                    (StarsU16)(4U + (StarsU16)planet->raw_prod_queue[3] * 4U)))
            return 0;
        if (!planet->raw_ai_planet && input->raw_ai_planet_table &&
                (planet->id < 0 || planet->id >= 4096 ||
                 (StarsU16)planet->id >= input->raw_ai_planet_record_count))
            return 0;
    }
    input->armada_potency[0] = (StarsU8)automitron_armada_potency(input->turn);
    input->armada_potency[1] = (StarsU8)(input->armada_potency[0] / 2);
    input->armada_potency[2] = (StarsU8)automitron_support_potency(input->turn);
    input->armada_potency[3] = (StarsU8)(input->armada_potency[2] / 2 - 1);
    if (input->armada_potency[3] >= 4)
        input->armada_potency[3] = 3;
    budget = input->turn < 120 ? 50 :
        (input->turn < 200 ? 70 : 100);
    design_is = -1;
    design_support = -1;
    design_escort = -1;
    design_attack = -1;
    support_status = 0;
    selected = -1;
    if (ops->check_design_status_ex || ops->check_design_status) {
        for (i = 0; i < 16; ++i)
            split_mask[i] = 0;
        /* The original calls all four services. Their return values are
         * status/count data, not Boolean success flags; the second value
         * drives the early support-design pressure gate. */
        automitron_check_design_status(ops, 11, 12, budget,
            &design_is, split_mask);
        support_status = automitron_check_design_status(ops, 4, 5, budget,
            &design_support, split_mask);
        automitron_check_design_status(ops, 2, 3, budget,
            &design_escort, split_mask);
        automitron_check_design_status(ops, 9, 10, budget,
            &design_attack, split_mask);
        selected = design_attack;
    }
    if (input->turn > 60 && ops->split_shdefs) {
        if (!ops->split_shdefs(ops->context, split_mask))
            return 0;
    }
    if (!ops->ensure_ai && ops->ensure_shdefs &&
            !ops->ensure_shdefs(ops->context, selected))
        return 0;
    /* Original NE 20:~04E0 completes this census before entering the
     * mutation/production pass, so the first visit sees the final count. */
    for (i = 0; i < input->planet_count; ++i) {
        const StarsAutomitronTurnPlanet *planet = &input->planets[i];
        if (planet->owner == -1 && planet->planet_class > 2) {
            StarsI16 value;
            value = ops->pct_planet_opt_value ?
                ops->pct_planet_opt_value(ops->context, planet,
                    input->player_id) : planet->optimal_value;
            if (value > 0)
                ++input->unowned_viable_count;
        }
    }
    input->production_queued = 0;
    input->production_hulls_queued = 0;
    input->colonizer_seed_queued = 0;
    input->support_design_queued = 0;
    input->tech_support_queued = 0;
    input->initial_colonizer_queued = 0;
    for (i = 0; i < input->planet_count; ++i) {
        const StarsAutomitronTurnPlanet *planet = &input->planets[i];
        StarsU8 *ai_record;
        StarsI16 early_result;
        StarsI16 seed_result;
        StarsI16 support_result;
        StarsI16 raw_queue_open;
        StarsI16 seed_queue_open;
        StarsI16 support_queue_open;
        StarsI16 production_entry;
        StarsI16 changed;
        StarsI16 desirability;
        StarsI16 optimal_value;
        desirability = planet->desirability;
        optimal_value = planet->optimal_value;
        ai_record = automitron_ai_planet_record(input, planet);
        if (ai_record)
            ai_record[9] = 1;
        if (planet->owner == input->player_id &&
                ops->pct_planet_desirability)
            desirability = ops->pct_planet_desirability(ops->context,
                planet, input->player_id);
        if (planet->owner != input->player_id && planet->owner != -1) {
            if (ai_record)
                ai_record[10] = (StarsU8)(
                    ((planet->visibility_flags >> 9) & 1U) + 1U);
            if (ops->pct_planet_opt_value)
                optimal_value = ops->pct_planet_opt_value(ops->context,
                    planet, input->player_id);
        }
        if (ai_record) {
            /* Original vlpbAiPlanet uses a 16-byte record indexed by the
             * planet ID: visit marker, bad-owned marker, and foreign-planet
             * status/optimal-value markers. */
            if (planet->owner == input->player_id &&
                    desirability < 0)
                ai_record[2] = 1;
            else if (planet->owner != input->player_id &&
                    planet->owner != -1) {
                if (optimal_value > 0)
                    ai_record[3] = 1;
            }
        }
        if (planet->owner == input->player_id && desirability < 0)
            ++input->owned_bad_count;
        if (ops->visit_planet && !ops->visit_planet(ops->context, planet,
                budget))
            return 0;
        production_entry = automitron_production_entry(planet,
            input->player_id, desirability);
        if (!production_entry)
            continue;
        if (ops->init_production &&
                !ops->init_production(ops->context, planet))
            return 0;
        /* The original scans the queue once after InitProduction. Retain
         * that answer across later AddItemToQueue calls in this pass. */
        raw_queue_open = automitron_queue_gate(planet, 1);
        if (planet->raw_prod_queue && !raw_queue_open) {
            if (ops->finish_production &&
                    !ops->finish_production(ops->context, planet, 0))
                return 0;
            continue;
        }
        seed_queue_open = planet->raw_prod_queue ? raw_queue_open :
            planet->colonizer_queue_eligible;
        support_queue_open = planet->raw_prod_queue ? raw_queue_open :
            planet->support_queue_eligible;
        early_result = automitron_queue_early(input, planet,
            design_support, support_status, support_queue_open);
        if (early_result < 0)
            return 0;
        if ((early_result & 1) != 0)
            ++input->tech_support_queued;
        if ((early_result & 2) != 0)
            ++input->initial_colonizer_queued;
        seed_result = automitron_seed_colonizer(input, planet,
            seed_queue_open);
        if (seed_result < 0)
            return 0;
        input->colonizer_seed_queued = (StarsI16)(
            input->colonizer_seed_queued + seed_result);
        support_result = automitron_queue_support(input, planet,
            design_escort, support_queue_open);
        if (support_result < 0)
            return 0;
        input->support_design_queued = (StarsI16)(
            input->support_design_queued + support_result);
        queued = support_result > 0 ? 4 : 0;
        if (support_result == 0 && input->hull_batch_inputs_bound) {
            queued = automitron_queue_hull_batches(input, planet,
                design_is, design_attack, support_queue_open);
            if (queued < 0)
                return 0;
        } else if (support_result == 0 && !input->hull_batch_inputs_bound &&
                ops->queue_production && raw_queue_open &&
                planet->production_slot_open && design_is >= 0 &&
                automitron_can_afford(planet, AUTOMITRON_HULL_BATCH)) {
            if (!ops->queue_production(ops->context, planet, design_is,
                    AUTOMITRON_HULL_BATCH))
                return 0;
            queued = AUTOMITRON_HULL_BATCH;
        }
        input->production_hulls_queued = (StarsI16)(
            input->production_hulls_queued + queued);
        changed = early_result != 0 || seed_result != 0 ||
            support_result != 0 || queued != 0;
        if (changed)
            ++input->production_queued;
        if (ops->finish_production &&
                !ops->finish_production(ops->context, planet, changed))
            return 0;
    }
    /* The original first owned-fleet sweep clears _fleet.overlap_4 bit 15
     * for every fleet before its later route/order sweep.  Keep that global
     * ordering even though this reduced source does not yet build the two
     * attack/transport linked lists in the first sweep. */
    for (i = 0; i < input->fleet_count; ++i) {
        if (input->fleets[i].owner == input->player_id &&
                input->fleets[i].raw_fleet_record &&
                input->fleets[i].raw_fleet_size < 124U)
            return 0;
    }
    for (i = 0; i < input->fleet_count; ++i) {
        if (input->fleets[i].owner == input->player_id &&
                input->fleets[i].raw_fleet_record)
            input->fleets[i].raw_fleet_record[5] = (StarsU8)(
                input->fleets[i].raw_fleet_record[5] & 0x7fU);
    }
    for (i = 0; i < input->fleet_count; ++i) {
        StarsI16 scout_result;
        if (input->fleets[i].owner != input->player_id)
            continue;
        scout_result = automitron_low_fuel_scout(input, &input->fleets[i]);
        if (scout_result < 0)
            return 0;
        if (scout_result > 0)
            continue;
        automitron_fleet_policy(&input->fleets[i], &fleet_policy);
        if (fleet_policy.needs_order_repair &&
                input->fleets[i].raw_plord &&
                !stars_ai_repair_first_order(
                input->fleets[i].raw_plord))
            return 0;
        if (fleet_policy.needs_order_repair && ops->repair_order &&
                !ops->repair_order(ops->context, &input->fleets[i],
                &fleet_policy, input->armada_potency))
            return 0;
        if (ops->visit_fleet_typed) {
            if (ops->plan_fleet_action) {
                memset(&fleet_plan, 0, sizeof fleet_plan);
                fleet_plan.target_planet = -1;
                if (!ops->plan_fleet_action(ops->context,
                        &input->fleets[i], &fleet_policy, input->turn,
                        &fleet_plan))
                    return 0;
                if (fleet_plan.action != STARS_AUTOMITRON_FLEET_ACTION_NONE &&
                        !automitron_apply_fleet_action(ops,
                        &input->fleets[i], &fleet_plan))
                    return 0;
            }
            if (!ops->visit_fleet_typed(ops->context, &input->fleets[i],
                    &fleet_policy, input->armada_potency[0]))
                return 0;
        } else if (ops->visit_fleet && !ops->visit_fleet(ops->context,
                &input->fleets[i], input->armada_potency[0]))
            return 0;
    }
    if (ops->run_basic_tasks && !ops->run_basic_tasks(ops->context,
            research_order, latest_starbase_design, input->player_id))
        return 0;
    if (ops->fill_production_queue &&
            !ops->fill_production_queue(ops->context))
        return 0;
    return 1;
}
