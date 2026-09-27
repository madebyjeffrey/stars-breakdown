#include "core.h"

/* Typed semantic reconstruction of DoCyberAiTurn, NE 22:002A-1A77.  The
 * packet and freighter services and the packed-order repair/write seams are
 * explicit typed boundaries so their 16-bit storage and callback order remain
 * testable at the caller. */

#define CYBER_ARMADA_MAX 50
#define CYBER_SUPPORT_MAX 12
#define CYBER_SPLIT_TURN 80
#define CYBER_RESOURCE_COUNT 4
#define CYBER_PLANET_SCANNED 0x0200U
#define CYBER_STARBASE_FLAG 0x0200U
#define CYBER_THREAT_STEP 250U
#define CYBER_THREAT_MAX 6

static StarsI16 cyber_armada_potency(StarsI16 turn)
{
    StarsI16 value;

    value = 3;
    if (turn > 130)
        value = (StarsI16)((turn - 120) / 20 + 3);
    if (value > CYBER_ARMADA_MAX)
        value = CYBER_ARMADA_MAX;
    return value;
}

static StarsI16 cyber_support_potency(StarsI16 turn)
{
    StarsI16 value;

    value = 6;
    if (turn > 115)
        value = (StarsI16)((turn - 100) / 22 + 6);
    if (value > CYBER_SUPPORT_MAX)
        value = CYBER_SUPPORT_MAX;
    return value;
}

static StarsI16 cyber_budget(StarsI16 turn)
{
    if (turn < 120)
        return 50;
    if (turn < 200)
        return 70;
    if (turn < 400)
        return 100;
    return 300;
}

static StarsI16 cyber_packet_pressure(StarsI16 turn)
{
    StarsI16 value;

    value = 1;
    if (turn > 50)
        value = (StarsI16)((turn - 50) / 10 + 1);
    if (turn > 100)
        value = (StarsI16)(value + ((turn - 100) / 10) * (turn / 100));
    return value;
}

static StarsU16 cyber_low_minerals(const StarsCyberTurnPlanet *planet)
{
    StarsU16 mask;
    StarsU32 threshold;
    StarsI16 i;

    mask = 0;
    threshold = (planet->habitat_kind == 1 || planet->habitat_kind == 3 ||
            planet->habitat_kind == 6 || planet->habitat_kind == 8) ? 10 :
        1000;
    for (i = 0; i < 3; ++i)
        if (planet->mineral_rates[i] < 1 &&
                (planet->mineral_rates[i] < 0 ||
                 planet->minerals[i] < threshold))
            mask = (StarsU16)(mask | (StarsU16)(1 << (i + 8)));
    return mask;
}

static StarsI16 cyber_production_is_affordable(
    const StarsCyberTurnPlanet *planet)
{
    StarsI16 resource;

    for (resource = 0; resource < CYBER_RESOURCE_COUNT; ++resource)
        if (planet->resources_available[resource] <
                planet->production_queue_cost[resource])
            return 0;
    return 1;
}

static StarsI16 cyber_hostile_threat(const StarsCyberTurnPlanet *planet)
{
    StarsI16 threat;

    threat = (StarsI16)(planet->packet_strength / CYBER_THREAT_STEP + 1);
    if (threat > CYBER_THREAT_MAX)
        threat = CYBER_THREAT_MAX;
    if ((planet->starbase_flags & CYBER_STARBASE_FLAG) != 0U)
        ++threat;
    return threat;
}

static StarsI16 cyber_fleet_sum(const StarsCyberTurnFleet *fleet,
    StarsI16 first, StarsI16 last)
{
    StarsI16 slot;
    StarsI16 total;

    total = 0;
    for (slot = first; slot <= last; ++slot)
        total = (StarsI16)(total + fleet->ship_counts[slot]);
    return total;
}

static void cyber_fleet_policy(const StarsCyberTurnFleet *fleet,
    StarsCyberFleetPolicy *policy)
{
    policy->colonizer_ships = fleet->ship_counts[0];
    policy->transport_ships = fleet->ship_counts[1];
    policy->armed_ships = cyber_fleet_sum(fleet, 2, 9);
    /* The raw routine scans the auxiliary design slots together with the
     * support slots when deciding whether a fleet needs a special order. */
    policy->auxiliary_design_ships = (StarsI16)(fleet->ship_counts[10] +
        fleet->ship_counts[11]);
    policy->support_design_ships = (StarsI16)(fleet->ship_counts[0xc] +
        fleet->ship_counts[0xd]);
    policy->attack_design_ships = (StarsI16)(fleet->ship_counts[0xe] +
        fleet->ship_counts[0xf]);
    policy->total_ships = (StarsI16)(policy->colonizer_ships +
        policy->transport_ships + policy->armed_ships +
        policy->auxiliary_design_ships +
        policy->support_design_ships + policy->attack_design_ships);
    policy->needs_order_repair = fleet->cord < 2 &&
        policy->total_ships != 0 && fleet->order_kind == 0;
}

static StarsI16 cyber_apply_fleet_action(
    const StarsCyberAiTurnOps *ops,
    const StarsCyberTurnFleet *fleet,
    const StarsCyberFleetPlan *plan)
{
    StarsCyberFleetAction action;

    action = 0;
    switch (plan->action) {
    case STARS_CYBER_FLEET_ACTION_CLEAR_TASK:
        action = ops->clear_task;
        break;
    case STARS_CYBER_FLEET_ACTION_MOVE:
        action = ops->move;
        break;
    case STARS_CYBER_FLEET_ACTION_COLONIZE:
        action = ops->colonize;
        break;
    case STARS_CYBER_FLEET_ACTION_WORMHOLE:
        action = ops->goto_wormhole;
        break;
    case STARS_CYBER_FLEET_ACTION_MOVE_STARBASE:
        action = ops->move_starbase;
        break;
    case STARS_CYBER_FLEET_ACTION_ATTACK:
        action = ops->attack;
        break;
    case STARS_CYBER_FLEET_ACTION_TARGET_ARMADA:
        action = ops->target_armada;
        break;
    case STARS_CYBER_FLEET_ACTION_TRANSPORT:
        action = ops->transport;
        break;
    case STARS_CYBER_FLEET_ACTION_TARGET_FREIGHTER:
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

/* NE 22:~111C: after FColonizeAiFleet succeeds, a route with a second
 * order marks that order's planet in vlpbAiPlanet[id][15]. */
static StarsI16 cyber_mark_colonizer_second_order(
    const StarsCyberAiTurnInput *input, const StarsCyberTurnFleet *fleet)
{
    StarsI16 target;
    const StarsU8 *raw;

    if (!input->raw_ai_planet_table)
        return 1;
    raw = fleet->raw_plord;
    if (!raw || fleet->raw_plord_bytes < 4U)
        return 0;
    if (raw[3] < 2U)
        return 1;
    if (fleet->raw_plord_bytes < 40U ||
            input->raw_ai_planet_record_count > 4095U)
        return 0;
    target = (StarsI16)((StarsU16)raw[26] |
        ((StarsU16)raw[27] << 8));
    if (target < 0 ||
            (StarsU16)target >= input->raw_ai_planet_record_count)
        return 0;
    input->raw_ai_planet_table[(StarsU16)target * 16U + 15U] = 4U;
    return 1;
}

static StarsI16 cyber_check_design_status(const StarsCyberAiTurnOps *ops,
    StarsI16 first, StarsI16 last, StarsI16 budget, StarsI16 *selected,
    StarsU8 status_mask[16])
{
    if (ops->check_design_status_ex)
        return ops->check_design_status_ex(ops->context, first, last,
            budget, selected, status_mask);
    if (ops->check_design_status)
        return ops->check_design_status(ops->context, first, last, budget,
            selected);
    return 1;
}

StarsI16 stars_do_cyber_ai_turn(StarsCyberAiTurnInput *input)
{
    const StarsCyberAiTurnOps *ops;
    StarsI16 budget;
    StarsI16 selected;
    StarsI16 research_order;
    StarsI16 latest_starbase_design;
    StarsI16 i;
    StarsU8 split_mask[16];
    StarsU16 low_mask;
    StarsCyberFleetPlan fleet_plan;
    StarsCyberFleetPolicy fleet_policy;

    if (!input || !input->ops || input->turn < 0 ||
            input->planet_count < 0 || input->fleet_count < 0 ||
            (input->planet_count > 0 && !input->planets) ||
            (input->fleet_count > 0 && !input->fleets))
        return 0;
    ops = input->ops;
    input->packet_pressure = cyber_packet_pressure(input->turn);
    input->low_mineral_planets = 0;
    input->production_affordable_count = 0;
    input->production_blocked_count = 0;
    input->research_order = -1;
    input->latest_starbase_design = -1;
    research_order = -1;
    latest_starbase_design = -1;
    selected = -1;
    if (ops->ensure_ai) {
        research_order = ops->ensure_ai(ops->context, 17,
            &latest_starbase_design);
        if (research_order < 0)
            return 0;
        input->research_order = research_order;
        input->latest_starbase_design = latest_starbase_design;
        if (ops->ensure_shdefs && !ops->ensure_shdefs(ops->context,
                research_order))
            return 0;
    }
    if (!ops->merge_shdefs || !ops->merge_shdefs(ops->context, 1) ||
            !ops->merge_shdefs(ops->context, 0x30) ||
            !ops->merge_shdefs(ops->context, (StarsI16)-0x4000) ||
            !ops->merge_shdefs(ops->context, 0x03c0) ||
            !ops->merge_shdefs(ops->context, 0x3c00))
        return 0;
    input->armada_potency[0] = (StarsU8)cyber_armada_potency(input->turn);
    input->armada_potency[1] = (StarsU8)(input->armada_potency[0] / 2);
    input->armada_potency[2] = (StarsU8)cyber_support_potency(input->turn);
    input->armada_potency[3] = (StarsU8)(input->armada_potency[2] / 2 - 1);
    if (input->armada_potency[3] >= 4)
        input->armada_potency[3] = 3;
    budget = cyber_budget(input->turn);
    for (i = 0; i < 16; ++i)
        split_mask[i] = 0;
    if (ops->check_design_status || ops->check_design_status_ex) {
        if (!cyber_check_design_status(ops, 4, 5, budget, &selected,
                split_mask))
            return 0;
        if (selected >= 0 && selected < 16)
            split_mask[selected] = 0;
        if (!cyber_check_design_status(ops, 14, 15, budget, &selected,
                split_mask))
            return 0;
        if (selected >= 0 && selected < 16)
            split_mask[selected] = 0;
        if (!cyber_check_design_status(ops, 2, 3, budget, &selected,
                split_mask))
            return 0;
        if (selected >= 0 && selected < 16)
            split_mask[selected] = 0;
    }
    if (input->turn > CYBER_SPLIT_TURN && ops->split_shdefs) {
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
        split_mask[2] = 2;
        split_mask[3] = 2;
        if (!ops->split_shdefs(ops->context, split_mask))
            return 0;
    }
    if (!ops->ensure_ai && ops->ensure_shdefs &&
            !ops->ensure_shdefs(ops->context, selected))
        return 0;
    for (i = 0; i < input->planet_count; ++i) {
        const StarsCyberTurnPlanet *planet = &input->planets[i];
        if (planet->owner == input->player_id &&
                (planet->visibility_flags & CYBER_PLANET_SCANNED) != 0U)
            low_mask = cyber_low_minerals(planet);
        else
            low_mask = 0;
        if (low_mask != 0)
            ++input->low_mineral_planets;
        if (ops->mark_planet && !ops->mark_planet(ops->context, planet,
                low_mask, input->player_id))
            return 0;
        if (planet->owner != input->player_id && planet->owner != -1 &&
                ops->mark_hostile_planet &&
                !ops->mark_hostile_planet(ops->context, planet,
                    input->player_id, cyber_hostile_threat(planet)))
            return 0;
        if (planet->owner == input->player_id) {
            if (!cyber_production_is_affordable(planet)) {
                ++input->production_blocked_count;
            } else {
                ++input->production_affordable_count;
                if (ops->visit_production &&
                        !ops->visit_production(ops->context, planet, budget))
                    return 0;
            }
        }
    }
    for (i = 0; i < input->fleet_count; ++i) {
        if (input->fleets[i].owner == input->player_id &&
                ops->visit_fleet_typed) {
            cyber_fleet_policy(&input->fleets[i], &fleet_policy);
            if (fleet_policy.needs_order_repair &&
                    input->fleets[i].raw_plord &&
                    !stars_ai_repair_first_order(
                    input->fleets[i].raw_plord))
                return 0;
            if (fleet_policy.needs_order_repair && ops->repair_order &&
                    !ops->repair_order(ops->context, &input->fleets[i],
                    &fleet_policy, input->armada_potency[0]))
                return 0;
            if (ops->run_freighter_ex &&
                    !ops->run_freighter_ex(ops->context, &input->fleets[i],
                    &fleet_policy, input->armada_potency[0]))
                return 0;
            if (ops->plan_fleet_action) {
                fleet_plan.action = STARS_CYBER_FLEET_ACTION_NONE;
                fleet_plan.target_planet = -1;
                fleet_plan.order_kind = 0;
                fleet_plan.append = 0;
                fleet_plan.reason = 0;
                if (!ops->plan_fleet_action(ops->context,
                        &input->fleets[i], &fleet_policy,
                        input->armada_potency[0], &fleet_plan))
                    return 0;
                if (fleet_plan.action != STARS_CYBER_FLEET_ACTION_NONE) {
                    if (!cyber_apply_fleet_action(ops,
                            &input->fleets[i], &fleet_plan))
                        return 0;
                    if (fleet_plan.action ==
                            STARS_CYBER_FLEET_ACTION_COLONIZE &&
                            !cyber_mark_colonizer_second_order(input,
                            &input->fleets[i]))
                        return 0;
                }
            }
            if (!ops->visit_fleet_typed(ops->context, &input->fleets[i],
                    &fleet_policy, input->armada_potency[0]))
                return 0;
        }
        if (input->fleets[i].owner == input->player_id && ops->visit_fleet &&
                !ops->visit_fleet(ops->context,
                &input->fleets[i], input->armada_potency[0]))
                return 0;
    }
    if (ops->run_basic_tasks_with_research) {
        if (!ops->run_basic_tasks_with_research(ops->context,
                research_order, latest_starbase_design, input->player_id))
            return 0;
    } else if (ops->run_basic_tasks && !ops->run_basic_tasks(ops->context,
            input->player_id))
            return 0;
    if (ops->run_packets_ex) {
        if (!ops->run_packets_ex(ops->context, input->player_id,
                input->planets, input->planet_count,
                input->packet_pressure))
            return 0;
    } else if (ops->run_packets &&
            !ops->run_packets(ops->context, input->player_id))
        return 0;
    if (ops->fill_production_queue &&
            !ops->fill_production_queue(ops->context))
        return 0;
    return 1;
}
