#include "core.h"

#define ROBOTOID_RESOURCE_COUNT 4
#define ROBOTOID_HULL_BATCH 5
#define ROBOTOID_PLANET_SCANNED 0x0200U

/* Typed semantic reconstruction of the setup, design maintenance, and
 * table-annotation phases of DoRobotoidAiTurn, NE 18:0312-1F33.  Production
 * accounting and individual fleet routing continue through explicit
 * callbacks until the shared order helpers are reconstructed. */

#define ROBOTOID_ARMADA_START 4
#define ROBOTOID_ARMADA_MAX 50
#define ROBOTOID_SPLIT_TURN 80
#define ROBOTOID_SUPPORT_MAX 12
#define ROBOTOID_STARBASE_FLAG 0x0200U
#define ROBOTOID_THREAT_STEP 250U
#define ROBOTOID_THREAT_MAX 6

static StarsI16 robotoid_armada_potency(StarsI16 turn)
{
    StarsI16 value;

    value = ROBOTOID_ARMADA_START;
    if (turn > 130)
        value = (StarsI16)((turn - 120) / 20 + ROBOTOID_ARMADA_START);
    if (value > ROBOTOID_ARMADA_MAX)
        value = ROBOTOID_ARMADA_MAX;
    return value;
}

static StarsI16 robotoid_support_potency(StarsI16 turn)
{
    StarsI16 value;

    value = 6;
    if (turn > 115)
        value = (StarsI16)((turn - 100) / 22 + 6);
    if (value > ROBOTOID_SUPPORT_MAX)
        value = ROBOTOID_SUPPORT_MAX;
    return value;
}

static StarsI16 robotoid_can_afford(
    const StarsRobotoidTurnPlanet *planet, StarsI16 hulls)
{
    StarsI32 remaining[ROBOTOID_RESOURCE_COUNT];
    StarsI16 resource;
    StarsI16 hull;

    for (resource = 0; resource < ROBOTOID_RESOURCE_COUNT; ++resource) {
        remaining[resource] = planet->resources_available[resource] -
            planet->production_queue_cost[resource];
        if (remaining[resource] < 0)
            return 0;
    }
    for (hull = 0; hull < hulls; ++hull) {
        for (resource = 0; resource < ROBOTOID_RESOURCE_COUNT;
             ++resource) {
            remaining[resource] -= planet->hull_cost[resource];
            if (remaining[resource] < 0)
                return 0;
        }
    }
    return 1;
}

static StarsI16 robotoid_production_entry(
    const StarsRobotoidTurnPlanet *planet, StarsI16 player_id)
{
    if (planet->owner != player_id)
        return 0;
    if ((planet->visibility_flags & ROBOTOID_PLANET_SCANNED) == 0U)
        return 0;
    if (planet->mineral_rate > 0)
        return 1;
    return planet->mineral_rate >= 0 && planet->mineral_concentration > 199;
}

static StarsI16 robotoid_hostile_threat(
    const StarsRobotoidTurnPlanet *planet)
{
    StarsI16 threat;

    threat = (StarsI16)(planet->packet_strength / ROBOTOID_THREAT_STEP + 1);
    if (threat > ROBOTOID_THREAT_MAX)
        threat = ROBOTOID_THREAT_MAX;
    if ((planet->starbase_flags & ROBOTOID_STARBASE_FLAG) != 0U)
        ++threat;
    return threat;
}

static StarsI16 robotoid_fleet_sum(const StarsRobotoidTurnFleet *fleet,
    StarsI16 first, StarsI16 last)
{
    StarsI16 slot;
    StarsI16 total;

    total = 0;
    for (slot = first; slot <= last; ++slot)
        total = (StarsI16)(total + fleet->ship_counts[slot]);
    return total;
}

static void robotoid_fleet_policy(const StarsRobotoidTurnFleet *fleet,
    StarsI16 turn, StarsRobotoidFleetPolicy *policy)
{
    policy->colonizer_ships = fleet->ship_counts[0];
    policy->transport_ships = fleet->ship_counts[1];
    policy->armed_ships = robotoid_fleet_sum(fleet, 2, 9);
    policy->support_design_ships = (StarsI16)(fleet->ship_counts[0xc] +
        fleet->ship_counts[0xd]);
    policy->attack_design_ships = (StarsI16)(fleet->ship_counts[0xe] +
        fleet->ship_counts[0xf]);
    policy->total_ships = (StarsI16)(policy->colonizer_ships +
        policy->transport_ships + policy->armed_ships +
        policy->support_design_ships + policy->attack_design_ships);
    if (policy->total_ships == 0 && fleet->warship_count != 0)
        policy->total_ships = fleet->warship_count;
    /* Raw repair is eligible only in the late-turn colonizer branch; route
     * selection below may still install an order and bypass the repair. */
    policy->needs_order_repair = fleet->ship_counts[0] != 0U &&
        turn >= 0x29 && fleet->cord == 1 && fleet->order_kind == 0;
}

static StarsI16 robotoid_apply_fleet_action(
    const StarsRobotoidAiTurnOps *ops,
    const StarsRobotoidTurnFleet *fleet,
    const StarsRobotoidFleetPlan *plan)
{
    StarsRobotoidFleetAction action;

    action = 0;
    switch (plan->action) {
    case STARS_ROBOTOID_FLEET_ACTION_CLEAR_TASK:
        action = ops->clear_task;
        break;
    case STARS_ROBOTOID_FLEET_ACTION_MOVE:
        action = ops->move;
        break;
    case STARS_ROBOTOID_FLEET_ACTION_COLONIZE:
        action = ops->colonize;
        break;
    case STARS_ROBOTOID_FLEET_ACTION_WORMHOLE:
        action = ops->goto_wormhole;
        break;
    case STARS_ROBOTOID_FLEET_ACTION_MOVE_STARBASE:
        action = ops->move_starbase;
        break;
    case STARS_ROBOTOID_FLEET_ACTION_ATTACK:
        action = ops->attack;
        break;
    case STARS_ROBOTOID_FLEET_ACTION_TARGET_ARMADA:
        action = ops->target_armada;
        break;
    case STARS_ROBOTOID_FLEET_ACTION_TRANSPORT:
        action = ops->transport;
        break;
    case STARS_ROBOTOID_FLEET_ACTION_TARGET_FREIGHTER:
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

StarsI16 stars_do_robotoid_ai_turn(StarsRobotoidAiTurnInput *input)
{
    const StarsRobotoidAiTurnOps *ops;
    StarsI16 budget;
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
    StarsI16 research_percent;
    StarsI16 selected;
    StarsI16 design_primary;
    StarsI16 i;
    StarsI16 queued;
    StarsRobotoidFleetPlan fleet_plan;
    StarsRobotoidFleetPolicy fleet_policy;
    StarsU8 split_mask[16];

    if (!input || !input->ops || input->turn < 0 ||
            input->planet_count < 0 || input->fleet_count < 0 ||
            (input->planet_count > 0 && !input->planets) ||
            (input->fleet_count > 0 && !input->fleets))
        return 0;
    ops = input->ops;
    input->war_fleet_count = 0;
    input->hostile_planet_count = 0;
    input->research_order = -1;
    input->latest_starbase_design = -1;
    research_order = -1;
    latest_starbase_design = -1;
    research_percent = input->turn < 10 ? 0 : 15;
    if (ops->ensure_ai) {
        research_order = ops->ensure_ai(ops->context, research_percent,
            &latest_starbase_design);
        if (research_order < 0)
            return 0;
    }
    input->research_order = research_order;
    input->latest_starbase_design = latest_starbase_design;
    if (input->turn > 50) {
        if ((ops->merge_shdefs && !ops->merge_shdefs(ops->context, 0x06fc)) ||
                (ops->merge_shdefs && !ops->merge_shdefs(ops->context, 1)) ||
                (ops->merge_shdefs &&
                    !ops->merge_shdefs(ops->context, (StarsI16)-0x4000)))
            return 0;
    }
    input->armada_potency[0] = (StarsU8)robotoid_armada_potency(input->turn);
    input->armada_potency[1] = (StarsU8)(input->armada_potency[0] / 2);
    input->armada_potency[2] = (StarsU8)robotoid_support_potency(input->turn);
    input->armada_potency[3] = (StarsU8)(input->armada_potency[2] / 2 - 1);
    if (input->armada_potency[3] >= 4)
        input->armada_potency[3] = 3;
    budget = input->turn < 120 ? 50 :
        (input->turn < 200 ? 70 : 100);
    design_primary = -1;
    if (ops->check_design_status) {
        for (i = 0; i < 16; ++i)
            input->split_status_mask[i] = 0;
        if (!ops->check_design_status(ops->context, 14, 15, budget,
                &design_primary, input->split_status_mask) ||
            !ops->check_design_status(ops->context, 11, 13, budget,
                &selected, input->split_status_mask) ||
            !ops->check_design_status(ops->context, 9, 10, budget,
                &selected, input->split_status_mask) ||
            !ops->check_design_status(ops->context, 2, 5, budget,
                &selected, input->split_status_mask) ||
            !ops->check_design_status(ops->context, 6, 7, (StarsI16)(budget * 3 / 2),
                &selected, input->split_status_mask))
            return 0;
        if (ops->design_status_info) {
            StarsI16 active;
            StarsI16 hull_kind;

            for (i = 14; i < 16; ++i) {
                if (input->split_status_mask[i] == 0)
                    continue;
                active = 1;
                hull_kind = 0;
                if (!ops->design_status_info(ops->context, i, &active,
                        &hull_kind))
                    return 0;
                if (!active && hull_kind == 0x1d)
                    input->split_status_mask[i] = 0;
            }
        }
    }
    if (input->turn > ROBOTOID_SPLIT_TURN && ops->split_shdefs) {
        for (i = 0; i < 16; ++i)
            split_mask[i] = input->split_status_mask[i];
        if (!ops->split_shdefs(ops->context, split_mask))
            return 0;
        for (i = 0; i < 16; ++i)
            split_mask[i] = 0;
        split_mask[0] = 2;
        if (!ops->split_shdefs(ops->context, split_mask))
            return 0;
        for (i = 0; i < 16; ++i)
            split_mask[i] = 0;
        split_mask[1] = 2;
        if (!ops->split_shdefs(ops->context, split_mask))
            return 0;
        for (i = 0; i < 16; ++i)
            split_mask[i] = 0;
        split_mask[11] = 2;
        split_mask[12] = 2;
        split_mask[13] = 2;
        if (!ops->split_shdefs(ops->context, split_mask))
            return 0;
    }
    if (ops->ensure_shdefs && !ops->ensure_shdefs(ops->context))
        return 0;
    input->production_queued = 0;
    input->production_hulls_queued = 0;
    for (i = 0; i < input->fleet_count; ++i) {
        if (input->fleets[i].owner == input->player_id &&
                design_primary >= 0 &&
                (input->fleets[i].ship_counts[14] != 0U ||
                 input->fleets[i].ship_counts[15] != 0U)) {
            if (!ops->count_war_fleet ||
                    ops->count_war_fleet(ops->context, &input->fleets[i],
                        input->armada_potency[2]))
                ++input->war_fleet_count;
        }
        if (ops->visit_fleet_typed) {
            if (input->fleets[i].owner != input->player_id)
                continue;
            robotoid_fleet_policy(&input->fleets[i], input->turn,
                &fleet_policy);
            fleet_plan.action = STARS_ROBOTOID_FLEET_ACTION_NONE;
            if (ops->plan_fleet_action) {
                fleet_plan.target_planet = -1;
                fleet_plan.order_kind = 0;
                fleet_plan.append = 0;
                fleet_plan.reason = 0;
                if (!ops->plan_fleet_action(ops->context,
                        &input->fleets[i], &fleet_policy, input->turn,
                        &fleet_plan))
                    return 0;
                if (fleet_plan.action != STARS_ROBOTOID_FLEET_ACTION_NONE &&
                        !robotoid_apply_fleet_action(ops,
                        &input->fleets[i], &fleet_plan))
                    return 0;
            }
            if (fleet_policy.needs_order_repair &&
                    fleet_plan.action == STARS_ROBOTOID_FLEET_ACTION_NONE) {
                if (input->fleets[i].raw_plord &&
                        !stars_ai_repair_first_order(
                        input->fleets[i].raw_plord))
                    return 0;
                if (ops->repair_order &&
                        !ops->repair_order(ops->context, &input->fleets[i],
                        &fleet_policy, input->armada_potency))
                    return 0;
            }
            if (!ops->visit_fleet_typed(ops->context, &input->fleets[i],
                    &fleet_policy, input->armada_potency))
                return 0;
        }
    }
    for (i = 0; i < input->planet_count; ++i) {
        const StarsRobotoidTurnPlanet *planet = &input->planets[i];
        if (planet->owner != input->player_id && planet->owner != -1) {
            StarsI16 threat_level;

            ++input->hostile_planet_count;
            threat_level = robotoid_hostile_threat(planet);
            if (ops->mark_planet_typed) {
                if (!ops->mark_planet_typed(ops->context, planet,
                        input->player_id, threat_level))
                    return 0;
            } else if (ops->mark_planet &&
                    !ops->mark_planet(ops->context, planet,
                    input->player_id))
                return 0;
        }
        if (robotoid_production_entry(planet, input->player_id) &&
                ops->run_production &&
                !ops->run_production(ops->context, planet, budget))
            return 0;
        queued = 0;
        if (ops->queue_production &&
                robotoid_production_entry(planet, input->player_id) &&
                planet->production_slot_open && design_primary >= 0 &&
                robotoid_can_afford(planet, ROBOTOID_HULL_BATCH)) {
            if (!ops->queue_production(ops->context, planet, design_primary,
                    ROBOTOID_HULL_BATCH))
                return 0;
            queued = ROBOTOID_HULL_BATCH;
        }
        input->production_hulls_queued = (StarsI16)(
            input->production_hulls_queued + queued);
        if (queued != 0)
            ++input->production_queued;
    }
    if (ops->run_fleets && !ops->run_fleets(ops->context, input->player_id))
        return 0;
    if (ops->run_basic_tasks_with_research) {
        if (!ops->run_basic_tasks_with_research(ops->context,
                research_order, latest_starbase_design, input->player_id))
            return 0;
    } else if (ops->run_basic_tasks && !ops->run_basic_tasks(ops->context,
            input->player_id))
            return 0;
    if (ops->fill_production_queue &&
            !ops->fill_production_queue(ops->context))
        return 0;
    return 1;
}
