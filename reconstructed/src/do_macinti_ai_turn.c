#include "core.h"
#include <string.h>

static StarsU16 macinti_word(const StarsU8 *raw)
{
    return (StarsU16)((StarsU16)raw[0] | ((StarsU16)raw[1] << 8));
}

static void macinti_bind_planet(const StarsMacintiTurnPlanet *source,
    StarsMacintiTurnPlanet *view)
{
    const StarsU8 *raw;
    *view = *source;
    raw = source->raw_record;
    if (!raw)
        return;
    view->id = (StarsI16)macinti_word(raw);
    view->owner = macinti_word(raw + 2) == 0xffffU ? -1 :
        (StarsI16)macinti_word(raw + 2);
    view->starbase_flags = macinti_word(raw + 4) & 0x0200U;
    view->packet_strength = macinti_word(raw + 18) & 0x0fffU;
    view->mineral_concentration[0] = raw[9];
    view->mineral_concentration[1] = raw[10];
    view->mineral_concentration[2] = raw[11];
}

/* The bound view is transient; the caller-owned 124-byte record remains the
 * authority for owner, position, orders and all sixteen signed 16-bit ship
 * counts. A present first PLORD order supplies its low kind nibble. */
static void macinti_bind_fleet(const StarsMacintiTurnFleet *source,
    StarsMacintiTurnFleet *view)
{
    const StarsU8 *raw;
    StarsI16 slot;
    *view = *source;
    raw = source->raw_record;
    if (!raw)
        return;
    view->id = (StarsI16)macinti_word(raw);
    view->owner = (StarsI16)macinti_word(raw + 2);
    view->planet_id = (StarsI16)macinti_word(raw + 6);
    view->point.x = (StarsI16)macinti_word(raw + 8);
    view->point.y = (StarsI16)macinti_word(raw + 10);
    view->cord = (StarsI16)macinti_word(raw + 98);
    view->plan = raw[96];
    for (slot = 0; slot < 16; ++slot)
        view->ship_counts[slot] = (StarsI16)macinti_word(
            raw + 12 + slot * 2);
    if (source->raw_plord && source->raw_plord_size >= 22U &&
            source->raw_plord[3] > 0U)
        view->order_kind = (StarsI16)(
            macinti_word(source->raw_plord + 10) & 0x000fU);
}

static StarsI32 macinti_delta_word(StarsU16 from, StarsU16 to)
{
    StarsU16 value;
    value = (StarsU16)(from - to);
    return value < 0x8000U ? (StarsI32)value :
        (StarsI32)value - 65536L;
}

/* Original 10a0:1a7d-1b2a: every fleet, not only this AI's, may lose a
 * distant second task-8 waypoint. The x/y subtraction wraps to 16 bits
 * before signed extension; the squared sum is signed 32-bit. */
static StarsI16 macinti_far_second_waypoint(
    const StarsMacintiTurnFleet *fleet)
{
    StarsI32 dx;
    StarsI32 dy;
    StarsU32 distance_squared;
    const StarsU8 *raw;
    const StarsU8 *plord;
    raw = fleet->raw_record;
    plord = fleet->raw_plord;
    if (macinti_word(raw + 98) <= 1U ||
            ((macinti_word(plord + 28) >> 8) & 0x000fU) != 8U)
        return 0;
    dx = macinti_delta_word(macinti_word(raw + 8),
        macinti_word(plord + 22));
    dy = macinti_delta_word(macinti_word(raw + 10),
        macinti_word(plord + 24));
    distance_squared = (StarsU32)dx * (StarsU32)dx +
        (StarsU32)dy * (StarsU32)dy;
    return distance_squared <= 0x7fffffffUL &&
        distance_squared > 40000UL;
}

/* Original 10a0:1db2-1dea and 2154-21b0 update the selected PLORD,
 * rather than merely changing the transient fleet policy's order kind. */
static StarsI16 macinti_set_selected_order_kind(
    const StarsMacintiAiTurnOps *ops, StarsMacintiTurnFleet *fleet,
    StarsU16 new_kind)
{
    StarsMacintiSelectedFleet selected;
    StarsU16 kind;
    selected.fleet_record = 0;
    selected.fleet_record_size = 0;
    selected.plord = 0;
    selected.plord_size = 0;
    if (!ops->select_fleet(ops->context, fleet, &selected) ||
            !selected.fleet_record || selected.fleet_record_size < 124U ||
            macinti_word(selected.fleet_record) != (StarsU16)fleet->id ||
            !selected.plord || selected.plord_size < 22U)
        return 0;
    kind = macinti_word(selected.plord + 10);
    kind = (StarsU16)((kind & 0xfff0U) | new_kind);
    selected.plord[10] = (StarsU8)kind;
    selected.plord[11] = (StarsU8)(kind >> 8);
    if (!ops->refresh_fleet(ops->context, fleet, &selected))
        return 0;
    fleet->raw_record = selected.fleet_record;
    fleet->raw_record_size = selected.fleet_record_size;
    fleet->raw_plord = selected.plord;
    fleet->raw_plord_size = selected.plord_size;
    return 1;
}

static StarsI16 macinti_truncate_selected_order(
    const StarsMacintiAiTurnOps *ops, StarsMacintiTurnFleet *fleet)
{
    StarsMacintiSelectedFleet selected;
    selected.fleet_record = 0;
    selected.fleet_record_size = 0;
    selected.plord = 0;
    selected.plord_size = 0;
    if (!ops->select_fleet(ops->context, fleet, &selected) ||
            !selected.fleet_record || selected.fleet_record_size < 124U ||
            macinti_word(selected.fleet_record) != (StarsU16)fleet->id ||
            !selected.plord || selected.plord_size < 22U)
        return 0;
    selected.fleet_record[98] = 1U;
    selected.fleet_record[99] = 0U;
    selected.plord[3] = 1U;
    if (!ops->refresh_fleet(ops->context, fleet, &selected))
        return 0;
    fleet->raw_record = selected.fleet_record;
    fleet->raw_record_size = selected.fleet_record_size;
    fleet->raw_plord = selected.plord;
    fleet->raw_plord_size = selected.plord_size;
    return 1;
}

static StarsI16 macinti_planet_index(const StarsMacintiAiTurnInput *input,
    StarsI16 id)
{
    StarsI16 index;
    for (index = 0; index < input->planet_count; ++index)
        if (input->planets[index].id == id)
            return index;
    return -1;
}

static StarsI16 macinti_selected_planet_index(
    const StarsMacintiAiTurnInput *input, StarsI16 selected)
{
    StarsI16 index;
    StarsI16 found;
    const StarsU8 *pointer;
    if (!input->selected_ai_planet_pointers)
        return macinti_planet_index(input,
            input->selected_ai_planet_ids[selected]);
    pointer = input->selected_ai_planet_pointers + (StarsU16)selected * 4U;
    if ((pointer[0] | pointer[1] | pointer[2] | pointer[3]) == 0U)
        return -1;
    found = -1;
    for (index = 0; index < input->planet_count; ++index)
        if (memcmp(input->planets[index].guest_far_pointer, pointer, 4U) == 0) {
            if (found >= 0)
                return -1;
            found = index;
        }
    return found;
}

/* Original 10a0:0f5b/0ffc route into attack production. Colonizer queue
 * work itself is not performed here; the route distinguishes early-turn
 * colonizer-only from the later colonizer-then-attack fallthrough. */
StarsI16 stars_macinti_attack_outer_route(
    const StarsMacintiAttackOuterInput *input, StarsI16 *route)
{
    const StarsU8 *raw;
    StarsI16 draw;
    StarsI16 slot;
    StarsI16 high;
    StarsI16 build;
    if (!input || !route || !input->planet ||
            !input->planet->raw_record ||
            input->planet->raw_record_size < 56U ||
            !input->random_value ||
            !input->should_planet_build_colonizer)
        return 0;
    raw = input->planet->raw_record;
    if (macinti_word(raw) != (StarsU16)input->planet->id)
        return 0;
    if (!input->should_build_colonizers ||
            (input->colonizer_count > 40 &&
             (input->turn > 120 || input->colonizer_count > 100))) {
        draw = input->random_value(input->context, 100);
        if (draw < 0 || draw >= 100)
            return 0;
        if (draw >= 8) {
            *route = STARS_MACINTI_OUTER_ATTACK;
            return 1;
        }
    }
    if (input->colonizer_count > 49 && input->turn > 120) {
        *route = STARS_MACINTI_OUTER_ATTACK;
        return 1;
    }
    for (slot = 0; slot < 3; ++slot) {
        high = (StarsI16)macinti_word(raw + 30 + slot * 4);
        if (high < 0 ||
                (high == 0 && macinti_word(raw + 28 + slot * 4) < 30U)) {
            *route = STARS_MACINTI_OUTER_SKIP;
            return 1;
        }
    }
    build = input->should_planet_build_colonizer(input->context,
        input->planet);
    if (build != 0 && build != 1)
        return 0;
    *route = !build ? STARS_MACINTI_OUTER_ATTACK :
        input->turn > 4 ? STARS_MACINTI_OUTER_COLONIZER_THEN_ATTACK :
            STARS_MACINTI_OUTER_COLONIZER_ONLY;
    return 1;
}

/* Inner attack-production choice at 10a0:1153-1307. The preceding
 * colonizer/Random(100) branch is intentionally supplied by the caller;
 * this helper does not claim to reconstruct that outer gate. */
StarsI16 stars_macinti_should_queue_attack(
    const StarsMacintiAttackQueueInput *input, StarsI16 *should_queue)
{
    const StarsU8 *planet_raw;
    const StarsU8 *fleet_raw;
    StarsMacintiTurnFleet fleet_view;
    StarsI32 mine_count;
    StarsI16 index;
    StarsI16 draw;
    StarsI16 score;
    if (!input || !should_queue || !input->planet ||
            !input->planet->raw_record ||
            input->planet->raw_record_size < 56U ||
            input->fleet_count < 0 ||
            (input->fleet_count && !input->fleets) ||
            !input->random_value)
        return 0;
    planet_raw = input->planet->raw_record;
    if (macinti_word(planet_raw) != (StarsU16)input->planet->id)
        return 0;
    /* Reject malformed fleet tables before consuming any random draws. */
    for (index = 0; index < input->fleet_count; ++index)
        if (!input->fleets[index].raw_record ||
                input->fleets[index].raw_record_size < 124U ||
                macinti_word(input->fleets[index].raw_record) !=
                    (StarsU16)input->fleets[index].id)
            return 0;
    *should_queue = 0;
    if (!input->outer_branch_enabled ||
            (input->attack_design != 14 && input->attack_design != 15) ||
            input->attack_pressure >= 60 ||
            input->design_existing >= 5000UL)
        return 1;
    draw = input->random_value(input->context, 2);
    if (draw < 0 || draw >= 2)
        return 0;
    if (draw)
        return 1;
    mine_count = 0;
    for (index = 0; index < input->fleet_count; ++index) {
        fleet_raw = input->fleets[index].raw_record;
        if (macinti_word(fleet_raw + 6) == macinti_word(planet_raw) &&
                (StarsI16)macinti_word(fleet_raw + 12 +
                    input->attack_design * 2) > 0 &&
                (StarsI16)macinti_word(fleet_raw + 2) == input->player_id) {
            if (!input->mine_from_fleet)
                return 0;
            macinti_bind_fleet(&input->fleets[index], &fleet_view);
            if (!input->mine_from_fleet(input->context, &fleet_view,
                    &mine_count))
                return 0;
            break;
        }
    }
    if (mine_count >= 1001L)
        return 1;
    score = (StarsI16)(3 * ((StarsI16)planet_raw[9] +
        (StarsI16)planet_raw[10] + (StarsI16)planet_raw[11]));
    if (mine_count < (StarsI32)score && score > 150) {
        *should_queue = 1;
        return 1;
    }
    if (input->attack_pressure < 30) {
        draw = input->random_value(input->context, 10);
        if (draw < 0 || draw >= 10)
            return 0;
        *should_queue = draw != 0;
    }
    return 1;
}

/* Original 10a0:0807-0826 queues one colonizer, then (after turn four)
 * possibly a second and third. The 32-bit multiply wraps before signed
 * threshold comparisons. Only the first two queues increment iStack_8. */
static StarsI16 macinti_colonizer_queue_count(
    const StarsMacintiAttackProductionInput *input)
{
    const StarsU8 *raw;
    StarsU32 population;
    StarsU32 product;
    raw = input->outer.planet->raw_record;
    if (input->outer.turn <= 4)
        return 1;
    population = (StarsU32)macinti_word(raw + 40) |
        ((StarsU32)macinti_word(raw + 42) << 16);
    product = population * (StarsU32)(StarsI32)input->growth_percent;
    if (product > 0x7fffffffUL || product <= 2300UL ||
            input->resources_at_planet <= 35 ||
            input->colonizer_ability <= 0)
        return 1;
    if (product > 3600UL && input->resources_at_planet > 50 &&
            input->colonizer_ability > 1)
        return 3;
    return 2;
}

/* Bounded 10a0:0f5b-1307 service sequence. It deliberately excludes the
 * earlier design/resource branches and the later support/freighter queues.
 * A queue-service failure leaves counters at their original pre-call point. */
StarsI16 stars_macinti_run_attack_production(
    const StarsMacintiAttackProductionInput *input,
    StarsMacintiAttackProductionResult *result)
{
    StarsMacintiAttackQueueInput attack;
    StarsMacintiProductionPlan plan;
    StarsI16 route;
    StarsI16 colonizer_queues;
    StarsI16 queue_index;
    StarsI16 should_queue;
    if (!input || !result || !input->queue_production ||
            !input->outer.planet || !input->attack.planet ||
            !input->outer.random_value ||
            !input->outer.should_planet_build_colonizer ||
            input->outer.planet->id != input->attack.planet->id ||
            input->outer.planet->raw_record !=
                input->attack.planet->raw_record ||
            !input->outer.planet->raw_record ||
            input->outer.planet->raw_record_size < 56U ||
            input->attack.planet->raw_record_size < 56U ||
            macinti_word(input->outer.planet->raw_record) !=
                (StarsU16)input->outer.planet->id ||
            input->attack.random_value != input->outer.random_value ||
            input->attack.context != input->outer.context ||
            input->attack.fleet_count < 0 ||
            (input->attack.fleet_count && !input->attack.fleets) ||
            input->mission_design < -1 || input->mission_design >= 16 ||
            input->mission_count < 0 || input->player_planet_count < 0 ||
            input->colonizer_design < 0 || input->colonizer_design >= 16 ||
            input->outer.colonizer_count < 0)
        return 0;
    for (queue_index = 0; queue_index < input->attack.fleet_count;
            ++queue_index)
        if (!input->attack.fleets[queue_index].raw_record ||
                input->attack.fleets[queue_index].raw_record_size < 124U ||
                macinti_word(input->attack.fleets[queue_index].raw_record) !=
                    (StarsU16)input->attack.fleets[queue_index].id)
            return 0;
    result->outer_route = STARS_MACINTI_OUTER_SKIP;
    result->mission_queued = 0;
    result->mission_count = input->mission_count;
    result->colonizer_queues = 0;
    result->attack_queued = 0;
    result->colonizer_count = input->outer.colonizer_count;
    result->attack_pressure = input->attack.attack_pressure;
    /* Original 10a0:0f5b: this queue precedes Random(100) and does not
     * advance attack pressure. The planet cap is integer cPlanet / 4. */
    if (input->mission_design != -1 && result->mission_count < 64 &&
            result->mission_count < input->player_planet_count / 4) {
        StarsI16 draw;
        draw = input->outer.random_value(input->outer.context, 3);
        if (draw < 0 || draw >= 3)
            return 0;
        if (draw == 0) {
            plan.action = STARS_MACINTI_PRODUCTION_QUEUE;
            plan.design = input->mission_design;
            plan.quantity = 1;
            plan.object_group = 2;
            plan.queue_mode = 1;
            plan.finish_mode = 0;
            plan.reason = 0;
            plan.attack_pressure_increment = 0;
            ++result->mission_count;
            if (!input->queue_production(input->queue_context,
                    input->outer.planet, &plan))
                return 0;
            result->mission_queued = 1;
        }
    }
    if (!stars_macinti_attack_outer_route(&input->outer, &route))
        return 0;
    result->outer_route = route;
    if (route == STARS_MACINTI_OUTER_SKIP)
        return 1;
    if (route == STARS_MACINTI_OUTER_COLONIZER_ONLY ||
            route == STARS_MACINTI_OUTER_COLONIZER_THEN_ATTACK) {
        colonizer_queues = macinti_colonizer_queue_count(input);
        if (result->colonizer_count > 32767 -
                (colonizer_queues > 1 ? 2 : 1))
            return 0;
        plan.action = STARS_MACINTI_PRODUCTION_QUEUE;
        plan.design = input->colonizer_design;
        plan.quantity = 1;
        plan.object_group = 2;
        plan.queue_mode = 1;
        plan.finish_mode = 0;
        plan.reason = 0;
        plan.attack_pressure_increment = 0;
        for (queue_index = 0; queue_index < colonizer_queues;
                ++queue_index) {
            if (queue_index < 2)
                ++result->colonizer_count;
            if (!input->queue_production(input->queue_context,
                    input->outer.planet, &plan))
                return 0;
            ++result->colonizer_queues;
        }
        if (route == STARS_MACINTI_OUTER_COLONIZER_ONLY)
            return 1;
    }
    attack = input->attack;
    attack.outer_branch_enabled = 1;
    attack.attack_pressure = result->attack_pressure;
    if (!stars_macinti_should_queue_attack(&attack, &should_queue))
        return 0;
    if (!should_queue)
        return 1;
    if (result->attack_pressure == 32767)
        return 0;
    plan.action = STARS_MACINTI_PRODUCTION_QUEUE;
    plan.design = attack.attack_design;
    plan.quantity = 1;
    plan.object_group = 2;
    plan.queue_mode = 1;
    plan.finish_mode = 0;
    plan.reason = 0;
    plan.attack_pressure_increment = 1;
    ++result->attack_pressure;
    if (!input->queue_production(input->queue_context,
            input->outer.planet, &plan))
        return 0;
    result->attack_queued = 1;
    return 1;
}

/* 10a0:1ded-1f15: return -1 on failed service, 1 when the original
 * advances to the next fleet, or 0 to continue at the later order gate. */
static StarsI16 macinti_attack_design_followup(
    const StarsMacintiAiTurnInput *input,
    const StarsMacintiAiTurnOps *ops,
    const StarsMacintiTurnFleet *fleet)
{
    StarsI16 joined;
    StarsI16 draw;
    StarsI16 planet_index;
    StarsU16 concentration;
    const StarsU8 *raw;
    if (input->first_sweep_attack_pressure > 48) {
        StarsI16 try_buddy;
        try_buddy = input->first_sweep_attack_pressure > 58;
        if (!try_buddy) {
            draw = ops->random_value(ops->context, 3);
            if (draw < 0 || draw >= 3)
                return -1;
            try_buddy = draw != 0;
        }
        if (try_buddy) {
            joined = 0;
            if (!ops->find_buddy(ops->context, fleet, 14, 15,
                    72L, 108L, &joined))
                return -1;
            if (joined)
                return 1;
        }
    }
    planet_index = fleet->planet_id == -1 ? -1 :
        macinti_planet_index(input, fleet->planet_id);
    if (planet_index >= 0) {
        raw = input->planets[planet_index].raw_record;
        if ((StarsI16)macinti_word(raw + 2) == input->player_id) {
            concentration = (StarsU16)(raw[9] + raw[10] + raw[11]);
            if (concentration < 30U) {
                if (!ops->retarget_miner_live(ops->context, fleet))
                    return -1;
                return 1;
            }
            if (concentration < 60U) {
                draw = ops->random_value(ops->context, 3);
                if (draw < 0 || draw >= 3)
                    return -1;
                if (draw == 0) {
                    if (!ops->retarget_miner_live(ops->context, fleet))
                        return -1;
                    return 1;
                }
            }
        }
    }
    draw = ops->random_value(ops->context, 10);
    if (draw < 0 || draw >= 10)
        return -1;
    if (draw == 0 &&
            !ops->retarget_miner_live(ops->context, fleet))
        return -1;
    return 0;
}

/* Original 10a0:09a3-0a05 selected-planet action gate. rgwtMin[3] is a
 * 32-bit quantity at offset 40; its high word is tested as signed, while
 * the zero-high low word is compared unsigned with 200. */
static StarsI16 macinti_planet_action_gate(
    const StarsMacintiTurnPlanet *planet, StarsI16 turn)
{
    const StarsU8 *raw;
    StarsI16 mineral_high;
    StarsU16 starbase_kind;
    raw = planet->raw_record;
    mineral_high = (StarsI16)macinti_word(raw + 42);
    starbase_kind = macinti_word(raw + 44) & 0x000fU;
    return (macinti_word(raw + 4) & 0x0200U) != 0U &&
        mineral_high >= 0 &&
        (mineral_high > 0 || macinti_word(raw + 40) >= 200U) &&
        starbase_kind != 0U &&
        (starbase_kind != 1U || turn < 26);
}

static StarsI16 macinti_prod_queue_valid(const StarsU8 *raw,
    StarsU16 bytes)
{
    return raw && bytes >= 4U &&
        (StarsU32)bytes >= 4UL + (StarsU32)raw[3] * 4UL;
}

/* Original 10a0:0aa3-0b00: type 2, design below 16 aborts the current
 * production pass with FinishProduction(0). The live post-init allocation
 * supersedes the planet's earlier lpplprod pointer. */
static StarsI16 macinti_prod_queue_bypass(const StarsU8 *raw)
{
    StarsU16 index;
    StarsU32 item;
    for (index = 0; index < raw[3]; ++index) {
        const StarsU8 *entry = raw + 4U + index * 4U;
        item = (StarsU32)entry[0] | ((StarsU32)entry[1] << 8) |
            ((StarsU32)entry[2] << 16) | ((StarsU32)entry[3] << 24);
        if (((item >> 17) & 7UL) == 2UL &&
                ((item >> 10) & 0x7fUL) < 16UL)
            return 1;
    }
    return 0;
}

/* The original pre-planet fleet sweep marks AI byte 14 for the target of a
 * qualifying second MOVE order. The target and kind are in the second
 * 18-byte PLORD record after its four-byte header. */
static StarsI16 macinti_fleet_marker_target(
    const StarsMacintiAiTurnInput *input,
    const StarsMacintiTurnFleet *fleet, StarsI16 mission_design,
    StarsI16 *target)
{
    const StarsU8 *raw;
    const StarsU8 *plord;
    StarsU16 flags;
    *target = -1;
    raw = fleet->raw_record;
    if (macinti_word(raw + 2) != (StarsU16)input->player_id ||
            mission_design == -1 ||
            (macinti_word(raw + 32) == 0U &&
             macinti_word(raw + 34) == 0U) ||
            macinti_word(raw + 6) != 0xffffU ||
            macinti_word(raw + 98) <= 1U ||
            (StarsI16)macinti_word(raw + 90) < 0 ||
            (macinti_word(raw + 90) == 0U &&
             macinti_word(raw + 88) == 0U))
        return 1;
    plord = fleet->raw_plord;
    if (!plord || fleet->raw_plord_size < 30U || plord[3] < 2U)
        return 0;
    flags = macinti_word(plord + 28);
    if (((flags >> 8) & 0x000fU) != 1U)
        return 1;
    *target = (StarsI16)macinti_word(plord + 26);
    return *target >= 0 && (StarsU16)*target < input->game_planet_max;
}

/* Typed semantic reconstruction of the design-maintenance and dispatch
 * prefix of DoMacintiAiTurn, NE 21:0008-2A05.  The original then scans the
 * live planet/fleet tables and chooses individual tasks; those table-owned
 * decisions remain explicit callbacks until their dependent AI procedures are
 * reconstructed. */

static StarsI16 macinti_armada_potency(StarsI16 turn)
{
    StarsI16 value;

    value = 6;
    if (turn > 130)
        value = (StarsI16)((turn - 120) / 20 + 6);
    if (value > STARS_MACINTI_ARMADA_MAX)
        value = STARS_MACINTI_ARMADA_MAX;
    return value;
}

static StarsI16 macinti_support_potency(StarsI16 turn)
{
    StarsI16 value;

    value = 6;
    if (turn > STARS_MACINTI_SUPPORT_RAMP_TURN)
        value = (StarsI16)((turn - 100) / 22 + 6);
    if (value > STARS_MACINTI_SUPPORT_MAX)
        value = STARS_MACINTI_SUPPORT_MAX;
    return value;
}

static StarsI16 macinti_production_is_affordable(
    const StarsMacintiTurnPlanet *planet)
{
    StarsI16 resource;

    for (resource = 0; resource < 4; ++resource)
        if (planet->resources_available[resource] <
                planet->production_queue_cost[resource])
            return 0;
    return 1;
}

static StarsI16 macinti_production_queue_available(
    const StarsMacintiTurnPlanet *planet)
{
    if (planet->production_queue_limit == 0)
        return 1;
    return planet->production_queue_count <
        planet->production_queue_limit;
}

/* The raw body obtains resources after subtracting the existing queue cost,
 * then subtracts each channel of GetTrueHullCost for every requested hull.
 * Keep the subtraction ordered and accept an exact zero remainder. */
static StarsI16 macinti_hull_batch_is_affordable(
    const StarsMacintiTurnPlanet *planet, StarsI16 batch)
{
    StarsI32 remaining[4];
    StarsI16 resource;
    StarsI16 hull;

    if (batch < 0)
        return 0;
    for (resource = 0; resource < 4; ++resource) {
        remaining[resource] = planet->resources_available[resource] -
            planet->production_queue_cost[resource];
        if (remaining[resource] < 0)
            return 0;
    }
    for (hull = 0; hull < batch; ++hull) {
        for (resource = 0; resource < 4; ++resource) {
            remaining[resource] -= planet->hull_cost[resource];
            if (remaining[resource] < 0)
                return 0;
        }
    }
    return 1;
}

static StarsI16 macinti_merge(const StarsMacintiAiTurnOps *ops,
    StarsI16 mask)
{
    return ops->merge_shdefs ? ops->merge_shdefs(ops->context, mask) : 1;
}

static StarsU16 macinti_mineral_score(
    const StarsMacintiTurnPlanet *planet)
{
    StarsU16 score;
    StarsI16 mineral;

    score = 0;
    for (mineral = 0; mineral < 3; ++mineral) {
        if (planet->mineral_concentration[mineral] < 0x43)
            score = (StarsU16)(score +
                planet->mineral_concentration[mineral] / 2);
        else
            score = (StarsU16)(score + 0x4b);
    }
    return score > 127 ? 127 : score;
}

static StarsI16 macinti_fleet_sum(const StarsMacintiTurnFleet *fleet,
    StarsI16 first, StarsI16 last)
{
    StarsI16 slot;
    StarsI16 total;

    total = 0;
    for (slot = first; slot <= last; ++slot)
        total = (StarsI16)(total + fleet->ship_counts[slot]);
    return total;
}

static void macinti_planet_policy(const StarsMacintiTurnPlanet *planet,
    StarsI16 player, StarsMacintiPlanetPolicy *policy)
{
    StarsI16 threat;

    policy->mineral_score = macinti_mineral_score(planet);
    policy->owned = planet->owner == player;
    policy->unowned = planet->owner == -1;
    policy->hostile = planet->owner >= 0 && planet->owner != player;
    policy->has_starbase = planet->starbase_flags != 0;
    policy->viable_minerals = policy->mineral_score >= 0x1e;
    policy->production_queue_available =
        macinti_production_queue_available(planet);
    policy->production_affordable =
        macinti_production_is_affordable(planet);
    policy->production_batch = planet->production_batch;
    policy->production_batch_affordable =
        planet->production_batch == 0 ||
        macinti_hull_batch_is_affordable(planet, planet->production_batch);
    policy->threat_level = 0;
    policy->threat_marked = 0;
    if (policy->hostile) {
        threat = (StarsI16)(planet->packet_strength /
            STARS_MACINTI_THREAT_STEP + 1);
        if (threat > STARS_MACINTI_THREAT_MAX)
            threat = STARS_MACINTI_THREAT_MAX;
        if ((planet->starbase_flags & STARS_MACINTI_STARBASE_FLAG) != 0U)
            ++threat;
        policy->threat_level = threat;
        policy->threat_marked = 1;
    }
}

static void macinti_fleet_policy(const StarsMacintiTurnFleet *fleet,
    StarsMacintiFleetPolicy *policy)
{
    policy->colonizer_ships = fleet->ship_counts[0];
    policy->transport_ships = fleet->ship_counts[1];
    policy->armed_ships = macinti_fleet_sum(fleet, 2, 9);
    policy->mission_design_ships = (StarsI16)(fleet->ship_counts[10] +
        fleet->ship_counts[11]);
    policy->attack_design_ships = (StarsI16)(fleet->ship_counts[0xe] +
        fleet->ship_counts[0xf]);
    policy->support_design_ships = (StarsI16)(fleet->ship_counts[0xc] +
        fleet->ship_counts[0xd]);
    policy->total_ships = (StarsI16)(policy->colonizer_ships +
        policy->transport_ships + policy->armed_ships +
        policy->mission_design_ships +
        policy->attack_design_ships + policy->support_design_ships);
    policy->needs_order_repair = fleet->cord < 2 &&
        policy->total_ships != 0 && fleet->order_kind == 0;
    if (policy->needs_order_repair)
        policy->recommended_task = STARS_MACINTI_TASK_REPAIR_ORDER;
    else if (policy->armed_ships > 0)
        policy->recommended_task = STARS_MACINTI_TASK_ATTACK;
    else if (policy->transport_ships > 0)
        policy->recommended_task = STARS_MACINTI_TASK_TRANSPORT;
    else if (policy->colonizer_ships > 0)
        policy->recommended_task = STARS_MACINTI_TASK_COLONIZE;
    else
        policy->recommended_task = STARS_MACINTI_TASK_IDLE;
}

static StarsI16 macinti_dispatch_production(
    const StarsMacintiAiTurnOps *ops,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy,
    StarsI16 turn,
    StarsI16 *queued,
    StarsI16 *finished,
    StarsI16 *rejected,
    StarsI16 attack_design,
    StarsI16 derive_attack_pressure,
    StarsI16 *attack_pressure,
    StarsI16 enforce_typed_affordability)
{
    StarsMacintiProductionPlan plan;

    if (!ops->plan_production) {
        if (ops->choose_planet_production &&
                !ops->choose_planet_production(ops->context, planet,
                policy, turn))
            return 0;
        return 1;
    }
    plan.action = STARS_MACINTI_PRODUCTION_NONE;
    plan.design = -1;
    plan.quantity = 0;
    plan.object_group = 0;
    plan.queue_mode = 1;
    plan.finish_mode = 0;
    plan.reason = 0;
    plan.attack_pressure_increment = 0;
    if (!ops->plan_production(ops->context, planet, policy, turn, &plan))
        return 0;
    if (plan.attack_pressure_increment != 0 &&
            (plan.attack_pressure_increment != 1 ||
             !derive_attack_pressure || !attack_pressure ||
             attack_design < 14 || attack_design > 15 || plan.action !=
                 STARS_MACINTI_PRODUCTION_QUEUE ||
             plan.design != attack_design || plan.quantity != 1 ||
             plan.object_group != 2 || plan.queue_mode != 1 ||
             *attack_pressure == 32767))
        return 0;
    if (plan.action == STARS_MACINTI_PRODUCTION_NONE)
        return 1;
    if (plan.action == STARS_MACINTI_PRODUCTION_FINISH) {
        if (!ops->finish_production ||
                !ops->finish_production(ops->context, planet,
                plan.finish_mode))
            return 0;
        ++*finished;
        return 1;
    }
    if (plan.action != STARS_MACINTI_PRODUCTION_QUEUE ||
            plan.quantity <= 0 ||
            (enforce_typed_affordability &&
             !macinti_hull_batch_is_affordable(planet, plan.quantity))) {
        ++*rejected;
        return 1;
    }
    if (!ops->queue_production)
        return 0;
    if (plan.attack_pressure_increment)
        ++*attack_pressure;
    if (!ops->queue_production(ops->context, planet, &plan))
        return 0;
    ++*queued;
    return 1;
}

typedef struct MacintiBoundAttackQueueContext {
    StarsMacintiAiTurnInput *input;
    const StarsMacintiAiTurnOps *ops;
    const StarsMacintiAttackProductionResult *result;
} MacintiBoundAttackQueueContext;

static StarsI16 macinti_bound_attack_queue(void *opaque,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiProductionPlan *plan)
{
    MacintiBoundAttackQueueContext *bridge;
    bridge = (MacintiBoundAttackQueueContext *)opaque;
    bridge->input->first_sweep_mission_count =
        bridge->result->mission_count;
    bridge->input->colonizer_count = bridge->result->colonizer_count;
    bridge->input->first_sweep_attack_pressure =
        bridge->result->attack_pressure;
    return bridge->ops->queue_production(bridge->ops->context, planet,
        plan);
}

/* Opt-in bridge from the live selected-planet pass into the reconstructed
 * 10a0:0f5b-1307 slice. Other production branches remain an explicit
 * applicability assertion of build_attack_production. */
static StarsI16 macinti_dispatch_bound_attack(
    StarsMacintiAiTurnInput *input, const StarsMacintiAiTurnOps *ops,
    const StarsMacintiTurnPlanet *planet,
    const StarsMacintiPlanetPolicy *policy, StarsI16 attack_design,
    StarsI16 mission_design)
{
    StarsMacintiAttackProductionInput slice;
    StarsMacintiAttackProductionResult result;
    MacintiBoundAttackQueueContext bridge;
    StarsI32 queued_total;
    StarsI16 success;
    memset(&slice, 0, sizeof slice);
    if (!ops->build_attack_production(ops->context, planet, policy,
            input->turn, &slice))
        return 0;
    slice.outer.planet = planet;
    slice.outer.turn = input->turn;
    slice.outer.colonizer_count = input->colonizer_count;
    slice.outer.random_value = ops->random_value;
    slice.outer.context = ops->context;
    slice.attack.planet = planet;
    slice.attack.fleets = input->fleets;
    slice.attack.fleet_count = input->fleet_count;
    slice.attack.player_id = input->player_id;
    slice.attack.attack_design = attack_design;
    slice.attack.attack_pressure = input->first_sweep_attack_pressure;
    slice.attack.random_value = ops->random_value;
    slice.attack.context = ops->context;
    slice.mission_design = mission_design;
    slice.mission_count = input->first_sweep_mission_count;
    bridge.input = input;
    bridge.ops = ops;
    bridge.result = &result;
    slice.queue_production = macinti_bound_attack_queue;
    slice.queue_context = &bridge;
    result.outer_route = -1;
    success = stars_macinti_run_attack_production(&slice, &result);
    if (result.outer_route != -1) {
        queued_total = (StarsI32)input->typed_production_queued +
            result.mission_queued + result.colonizer_queues +
            result.attack_queued;
        if (queued_total > 32767L)
            return 0;
        input->typed_production_queued = (StarsI16)queued_total;
        input->first_sweep_mission_count = result.mission_count;
        input->colonizer_count = result.colonizer_count;
        input->first_sweep_attack_pressure = result.attack_pressure;
    }
    return success;
}

static StarsI16 macinti_dispatch_fleet_action(
    const StarsMacintiAiTurnOps *ops,
    const StarsMacintiTurnFleet *fleet,
    const StarsMacintiFleetPolicy *policy,
    StarsI16 turn,
    StarsI16 *actions,
    StarsI16 *rejected)
{
    StarsMacintiFleetPlan plan;
    StarsMacintiFleetAction action;

    if (!ops->plan_fleet_action)
        return 1;
    plan.action = STARS_MACINTI_FLEET_ACTION_NONE;
    plan.target_planet = -1;
    plan.order_kind = 0;
    plan.append = 0;
    plan.reason = 0;
    if (!ops->plan_fleet_action(ops->context, fleet, policy, turn, &plan))
        return 0;
    if (plan.action == STARS_MACINTI_FLEET_ACTION_NONE)
        return 1;
    action = 0;
    switch (plan.action) {
    case STARS_MACINTI_FLEET_ACTION_REPAIR_ORDER:
        action = ops->repair_order;
        break;
    case STARS_MACINTI_FLEET_ACTION_RETARGET_MINER:
        action = ops->retarget_miner;
        break;
    case STARS_MACINTI_FLEET_ACTION_JOIN_BUDDY:
        action = ops->join_buddy;
        break;
    case STARS_MACINTI_FLEET_ACTION_COLONIZE:
        action = ops->colonize;
        break;
    case STARS_MACINTI_FLEET_ACTION_WORMHOLE:
        action = ops->goto_wormhole;
        break;
    case STARS_MACINTI_FLEET_ACTION_MOVE_STARBASE:
        action = ops->move_starbase;
        break;
    case STARS_MACINTI_FLEET_ACTION_ATTACK:
        action = ops->attack;
        break;
    case STARS_MACINTI_FLEET_ACTION_TARGET_ARMADA:
        action = ops->target_armada;
        break;
    case STARS_MACINTI_FLEET_ACTION_TARGET_FREIGHTER:
        action = ops->target_freighter;
        break;
    case STARS_MACINTI_FLEET_ACTION_CLEAR_TASK:
        action = ops->clear_task;
        break;
    default:
        break;
    }
    if (!action)
        action = ops->apply_fleet_action;
    if (!action) {
        ++*rejected;
        return 1;
    }
    if (!action(ops->context, fleet, &plan))
        return 0;
    ++*actions;
    return 1;
}

StarsI16 stars_do_macinti_ai_turn(StarsMacintiAiTurnInput *input)
{
    const StarsMacintiAiTurnOps *ops;
    StarsI16 starter_missing;
    StarsI16 starter_order;
    StarsI16 design_end;
    StarsI16 budget;
    StarsI16 selected_design;
    StarsI16 i;
    StarsI16 mission_design;
    StarsI16 attack_design;
    StarsI16 selected_count;
    StarsU8 split_mask[16];
    StarsI16 masks[5];
    StarsMacintiPlanetPolicy planet_policy;
    StarsMacintiFleetPolicy fleet_policy;

    if (!input || !input->ops || input->turn < 0 ||
            input->planet_count < 0 || input->fleet_count < 0 ||
            (input->planet_count > 0 && !input->planets) ||
            (input->fleet_count > 0 && !input->fleets))
        return 0;
    ops = input->ops;
    mission_design = -1;
    attack_design = -1;
    selected_count = input->selected_ai_planet_count;
    if (input->derive_attack_pressure_from_plans &&
            (!input->planet_table_bound || !ops->check_design_status ||
             (!ops->plan_production &&
              !input->bound_attack_production)))
        return 0;
    if (input->bound_attack_production &&
            (!input->planet_table_bound ||
             !input->derive_attack_pressure_from_plans ||
             !ops->build_attack_production || !ops->queue_production ||
             !ops->finish_production || !ops->random_value))
        return 0;
    if (input->planet_table_bound) {
        if (input->selected_ai_planet_pointers && selected_count > 0 &&
                (StarsU32)selected_count * 4UL <=
                    input->selected_ai_planet_pointers_size) {
            for (i = 0; i < selected_count; ++i) {
                const StarsU8 *pointer;
                pointer = input->selected_ai_planet_pointers +
                    (StarsU16)i * 4U;
                if ((pointer[0] | pointer[1] | pointer[2] | pointer[3]) == 0U) {
                    selected_count = i;
                    break;
                }
            }
        }
        if (ops->route_transport_fleet &&
                (!ops->is_ai_transport || !ops->set_transport_plan))
            return 0;
        if (ops->is_ai_attack &&
                (!ops->is_ai_transport || !ops->clear_current_task ||
                 !ops->select_fleet || !ops->refresh_fleet))
            return 0;
        if (ops->find_buddy &&
                (!ops->random_value || !ops->retarget_miner_live ||
                 (!input->first_sweep_attack_pressure_bound &&
                  !input->derive_attack_pressure_from_plans) ||
                 (input->first_sweep_attack_pressure_bound &&
                  input->first_sweep_attack_pressure < 0)))
            return 0;
        if (!input->raw_ai_planet_table ||
                input->game_planet_max > 4095U ||
                input->raw_ai_planet_record_count < input->game_planet_max ||
                input->selected_ai_planet_count < 0 ||
                (input->selected_ai_planet_pointers &&
                 (StarsU32)input->selected_ai_planet_count * 4UL >
                     input->selected_ai_planet_pointers_size) ||
                (input->selected_ai_planet_count > 0 &&
                 !input->selected_ai_planet_pointers &&
                 !input->selected_ai_planet_ids))
            return 0;
        for (i = 0; i < input->planet_count; ++i) {
            const StarsMacintiTurnPlanet *planet = &input->planets[i];
            if (!planet->raw_record || planet->raw_record_size < 56U ||
                    planet->id < 0 ||
                    (StarsU16)planet->id >= input->game_planet_max ||
                    macinti_word(planet->raw_record) !=
                        (StarsU16)planet->id)
                return 0;
        }
        for (i = 0; i < input->fleet_count; ++i) {
            const StarsMacintiTurnFleet *fleet = &input->fleets[i];
            StarsI16 slot;
            StarsI16 cord;
            if (!fleet->raw_record || fleet->raw_record_size < 124U ||
                    macinti_word(fleet->raw_record) != (StarsU16)fleet->id ||
                    (StarsI16)macinti_word(fleet->raw_record + 98) < 0)
                return 0;
            cord = (StarsI16)macinti_word(fleet->raw_record + 98);
            if (cord > 1 && (!fleet->raw_plord ||
                    fleet->raw_plord_size < 40U ||
                    fleet->raw_plord[3] < 2U))
                return 0;
            if (cord > 1 && macinti_far_second_waypoint(fleet) &&
                    (!ops->select_fleet || !ops->refresh_fleet))
                return 0;
            if ((StarsU16)input->turn <= 10U &&
                    macinti_word(fleet->raw_record + 2) ==
                        (StarsU16)input->player_id &&
                    (macinti_word(fleet->raw_record + 12) > 0U ||
                     macinti_word(fleet->raw_record + 16) != 0U) &&
                    (!fleet->raw_plord || fleet->raw_plord_size < 22U ||
                     !ops->select_fleet || !ops->refresh_fleet))
                return 0;
            for (slot = 0; slot < 16; ++slot)
                if ((StarsI16)macinti_word(fleet->raw_record + 12 +
                        slot * 2) < 0)
                    return 0;
            if (macinti_word(fleet->raw_record + 2) ==
                    (StarsU16)input->player_id &&
                    macinti_word(fleet->raw_record + 98) > 0U &&
                    (!fleet->raw_plord || fleet->raw_plord_size < 22U ||
                     fleet->raw_plord[3] == 0U))
                return 0;
            if (macinti_word(fleet->raw_record + 2) ==
                    (StarsU16)input->player_id && cord == 1 &&
                    ((StarsI16)macinti_word(fleet->raw_record + 12) < 1 ||
                     (StarsU16)input->turn < 41U) &&
                    (macinti_word(fleet->raw_record + 40) > 0U ||
                     macinti_word(fleet->raw_record + 42) > 0U) &&
                    (macinti_word(fleet->raw_plord + 10) & 0x000fU) != 3U &&
                    (!ops->select_fleet || !ops->refresh_fleet))
                return 0;
        }
        for (i = 0; i < selected_count; ++i) {
            StarsI16 index;
            index = macinti_selected_planet_index(input, i);
            if (index < 0 || input->planets[index].id < 0 ||
                    (StarsU16)input->planets[index].id >=
                        input->game_planet_max)
                return 0;
        }
        for (i = 0; i < selected_count; ++i) {
            const StarsMacintiTurnPlanet *planet = &input->planets[
                macinti_selected_planet_index(input, i)];
            const StarsU8 *raw = planet->raw_record;
            if (!macinti_planet_action_gate(planet, input->turn))
                continue;
            if (!ops->select_planet || !ops->init_production ||
                    !ops->current_production_queue ||
                    !ops->finish_production)
                return 0;
            if ((raw[52] | raw[53] | raw[54] | raw[55]) != 0U &&
                    !macinti_prod_queue_valid(
                        planet->raw_existing_prod_queue,
                        planet->raw_existing_prod_queue_size))
                return 0;
        }
    }
    starter_missing = input->starter_missing;
    starter_order = starter_missing ? 7 : 6;
    if (input->turn < STARS_MACINTI_STARTER_TURN || starter_missing) {
        if (ops->create_starter && !ops->create_starter(ops->context))
            return 0;
        starter_missing = 0;
    }
    masks[0] = starter_missing ? (StarsI16)0x037c : (StarsI16)0x03fc;
    masks[1] = 1;
    masks[2] = (StarsI16)-0x4000;
    masks[3] = 0x3000;
    masks[4] = 0;
    for (i = 0; i < 4; ++i)
        if (!macinti_merge(ops, masks[i]))
            return 0;

    input->armada_potency[0] = (StarsU8)macinti_armada_potency(input->turn);
    input->armada_potency[1] = (StarsU8)(input->armada_potency[0] / 2);
    input->armada_potency[2] = (StarsU8)macinti_support_potency(input->turn);
    input->armada_potency[3] = (StarsU8)(input->armada_potency[2] / 2 - 1);
    if (input->armada_potency[3] >= 4)
        input->armada_potency[3] = 3;
    input->starter_order = starter_order;

    budget = input->turn < 120 ? 50 :
        (input->turn < 200 ? 70 : 100);
    if (ops->check_design_status) {
        for (i = 0; i < 16; ++i)
            input->split_status_mask[i] = 0;
        selected_design = -1;
        if (!ops->check_design_status(ops->context, 12, 13, budget,
                &selected_design, input->split_status_mask))
            return 0;
        if (ops->design_status_info) {
            StarsI16 active;
            StarsI16 hull_kind;

            for (i = 12; i < 14; ++i) {
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
        design_end = 15;
        selected_design = -1;
        if (!ops->check_design_status(ops->context, 14, design_end,
                5000, &selected_design, input->split_status_mask))
            return 0;
        if (selected_design != -1 &&
                (selected_design < 14 || selected_design > 15))
            return 0;
        attack_design = selected_design;
        if (!ops->check_design_status(ops->context, 10, 11, budget,
                    &selected_design, input->split_status_mask))
            return 0;
        mission_design = selected_design;
        if (!ops->check_design_status(ops->context, 8, 9, budget,
                    &selected_design, input->split_status_mask) ||
                !ops->check_design_status(ops->context, 2, 4, budget,
                    &selected_design, input->split_status_mask) ||
                !ops->check_design_status(ops->context, 5,
                    (StarsI16)(7 - starter_missing), budget, &selected_design,
                    input->split_status_mask))
            return 0;
    }
    if (input->turn > STARS_MACINTI_SPLIT_TURN && ops->split_shdefs) {
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
        split_mask[14] = 2;
        split_mask[15] = 2;
        if (!ops->split_shdefs(ops->context, split_mask))
            return 0;
        for (i = 0; i < 16; ++i)
            split_mask[i] = 0;
        split_mask[10] = 2;
        split_mask[11] = 2;
        if (!ops->split_shdefs(ops->context, split_mask))
            return 0;
    }
    if (ops->ensure_shdefs && !ops->ensure_shdefs(ops->context))
        return 0;
    if (ops->ensure_starbase_designs &&
            !ops->ensure_starbase_designs(ops->context))
        return 0;
    if (ops->colonizer_decision &&
            !ops->colonizer_decision(ops->context, &input->colonizer_count))
        return 0;
    input->first_sweep_attack_census = 0;
    input->first_sweep_attack_census_bound = 0;
    input->first_sweep_mission_count = 0;
    if (input->planet_table_bound && ops->check_design_status) {
        input->first_sweep_attack_census_bound = 1;
        if (mission_design != -1) {
            for (i = 0; i < input->fleet_count; ++i) {
                const StarsU8 *raw = input->fleets[i].raw_record;
                if ((StarsI16)macinti_word(raw + 2) == input->player_id &&
                        (macinti_word(raw + 32) != 0U ||
                         macinti_word(raw + 34) != 0U))
                    ++input->first_sweep_mission_count;
            }
        }
        if (attack_design != -1) {
            for (i = 0; i < input->fleet_count; ++i) {
                const StarsU8 *raw = input->fleets[i].raw_record;
                if ((StarsI16)macinti_word(raw + 2) == input->player_id &&
                        (macinti_word(raw + 40) != 0U ||
                         macinti_word(raw + 42) != 0U))
                    ++input->first_sweep_attack_census;
            }
        }
        if (input->derive_attack_pressure_from_plans) {
            input->first_sweep_attack_pressure =
                input->first_sweep_attack_census;
            input->first_sweep_attack_pressure_bound = 1;
        } else if (ops->find_buddy &&
                input->first_sweep_attack_pressure <
                    input->first_sweep_attack_census) {
            return 0;
        }
    }
    if (input->planet_table_bound) {
        for (i = 0; i < input->fleet_count; ++i) {
            StarsI16 target;
            if (!macinti_fleet_marker_target(input, &input->fleets[i],
                    mission_design, &target))
                return 0;
        }
    }
    if (ops->visit_planets && !ops->visit_planets(ops->context))
        return 0;
    if (ops->visit_fleets && !ops->visit_fleets(ops->context))
        return 0;
    input->typed_planets_visited = 0;
    input->typed_fleets_visited = 0;
    input->typed_armed_fleets = 0;
    input->typed_colonizer_fleets = 0;
    input->typed_mission_fleets = 0;
    input->typed_support_design_fleets = 0;
    input->typed_attack_design_fleets = 0;
    input->typed_production_candidates = 0;
    input->typed_production_queued = 0;
    input->typed_production_finished = 0;
    input->typed_production_rejected = 0;
    input->typed_fleet_actions = 0;
    input->typed_fleet_actions_rejected = 0;
    input->typed_transport_routes = 0;
    input->typed_fleet_finalizations = 0;
    input->basic_tasks_run = 0;
    input->production_queue_filled = 0;
    if (input->planet_table_bound) {
        StarsU16 slot;
        for (slot = 0; slot < input->game_planet_max; ++slot) {
            input->raw_ai_planet_table[slot * 16U + 14U] = 0;
            input->raw_ai_planet_table[slot * 16U + 13U] = 0;
        }
        for (i = 0; i < input->fleet_count; ++i) {
            StarsI16 target;
            if (!macinti_fleet_marker_target(input, &input->fleets[i],
                    mission_design, &target))
                return 0;
            if (target >= 0)
                input->raw_ai_planet_table[(StarsU16)target * 16U + 14U] |= 1U;
        }
        for (i = 0; i < selected_count; ++i) {
            StarsI16 selected_index;
            selected_index = macinti_selected_planet_index(input, i);
            input->raw_ai_planet_table[
                (StarsU16)input->planets[selected_index].id * 16U + 13U] = 1;
        }
        for (i = 0; i < input->planet_count; ++i) {
            StarsMacintiTurnPlanet view;
            StarsMacintiPlanetPolicy policy;
            macinti_bind_planet(&input->planets[i], &view);
            if (view.owner == input->player_id || view.owner == -1)
                continue;
            macinti_planet_policy(&view, input->player_id, &policy);
            input->raw_ai_planet_table[(StarsU16)view.id * 16U + 10U] =
                (StarsU8)policy.threat_level;
            input->raw_ai_planet_table[(StarsU16)view.id * 16U + 9U] = 1;
        }
    }
    if (input->planet_table_bound || ops->visit_planet_typed) {
        StarsI16 visit_count;
        visit_count = input->planet_table_bound ?
            selected_count : input->planet_count;
        for (i = 0; i < visit_count; ++i) {
            StarsI16 index;
            StarsMacintiTurnPlanet view;
            const StarsMacintiTurnPlanet *planet;
            index = input->planet_table_bound ?
                macinti_selected_planet_index(input, i) : i;
            if (input->planet_table_bound) {
                macinti_bind_planet(&input->planets[index], &view);
                planet = &view;
            } else {
                planet = &input->planets[index];
            }
            macinti_planet_policy(planet, input->player_id,
                &planet_policy);
            if (ops->visit_planet_typed &&
                    !ops->visit_planet_typed(ops->context, planet,
                    &planet_policy, input->turn))
                return 0;
            ++input->typed_planets_visited;
            if (input->planet_table_bound &&
                    macinti_planet_action_gate(planet, input->turn)) {
                const StarsU8 *current;
                StarsU16 bytes;
                if (!ops->select_planet(ops->context, planet))
                    return 0;
                if ((planet->raw_record[52] | planet->raw_record[53] |
                        planet->raw_record[54] | planet->raw_record[55]) !=
                        0U && planet->raw_existing_prod_queue[3] >= 24U)
                    continue;
                if (!ops->init_production(ops->context, planet))
                    return 0;
                current = 0;
                bytes = 0;
                if (!ops->current_production_queue(ops->context, planet,
                        &current, &bytes) ||
                        !macinti_prod_queue_valid(current, bytes))
                    return 0;
                if (macinti_prod_queue_bypass(current)) {
                    if (!ops->finish_production(ops->context, planet, 0))
                        return 0;
                    ++input->typed_production_finished;
                    continue;
                }
            }
            /* The packed branch has already applied its original starbase,
             * population and live PLPROD gates. Generic typed budget/limit
             * fields are not additional predicates in 10a0:09a3-1a02. */
            if (input->planet_table_bound ?
                    macinti_planet_action_gate(planet, input->turn) :
                    (planet_policy.owned &&
                     planet_policy.production_queue_available &&
                     planet_policy.production_affordable &&
                     planet_policy.production_batch_affordable)) {
                StarsI16 queued_before;
                StarsI16 finished_before;
                queued_before = input->typed_production_queued;
                finished_before = input->typed_production_finished;
                ++input->typed_production_candidates;
                if (input->bound_attack_production) {
                    if (!macinti_dispatch_bound_attack(input, ops, planet,
                            &planet_policy, attack_design, mission_design))
                        return 0;
                } else if (!macinti_dispatch_production(ops, planet,
                            &planet_policy, input->turn,
                            &input->typed_production_queued,
                            &input->typed_production_finished,
                            &input->typed_production_rejected,
                            attack_design,
                            input->derive_attack_pressure_from_plans,
                            &input->first_sweep_attack_pressure,
                            !input->planet_table_bound))
                    return 0;
                /* Original 10a0:1a02 finishes the selected live planet
                 * even when no item was queued. A delegated early finish
                 * action has already called this service. */
                if (input->planet_table_bound &&
                        input->typed_production_finished == finished_before) {
                    if (!ops->finish_production(ops->context, planet,
                            input->typed_production_queued !=
                                queued_before))
                        return 0;
                    ++input->typed_production_finished;
                }
            }
        }
    }
    if (input->planet_table_bound || ops->visit_fleet_typed) {
        for (i = 0; i < input->fleet_count; ++i) {
            StarsMacintiTurnFleet view;
            StarsMacintiTurnFleet source;
            const StarsMacintiTurnFleet *fleet;
            if (input->planet_table_bound) {
                source = input->fleets[i];
                macinti_bind_fleet(&source, &view);
                source = view;
                if (macinti_far_second_waypoint(&source)) {
                    if (!macinti_truncate_selected_order(ops, &source))
                        return 0;
                }
                macinti_bind_fleet(&source, &view);
                source = view;
                fleet = &view;
            } else {
                fleet = &input->fleets[i];
            }
            if (fleet->owner != input->player_id)
                continue;
            if (input->planet_table_bound && fleet->cord == 1 &&
                    (fleet->ship_counts[0] < 1 ||
                     (StarsU16)input->turn < 41U) &&
                    (fleet->ship_counts[14] > 0 ||
                     fleet->ship_counts[15] > 0) &&
                    fleet->order_kind != 3) {
                if (!macinti_set_selected_order_kind(ops, &source, 3U))
                    return 0;
                continue;
            }
            if (input->planet_table_bound && ops->find_buddy &&
                    fleet->cord == 1 && fleet->order_kind == 3 &&
                    (fleet->ship_counts[0] < 1 ||
                     (StarsU16)input->turn < 41U) &&
                    (fleet->ship_counts[14] > 0 ||
                     fleet->ship_counts[15] > 0)) {
                StarsI16 followup;
                followup = macinti_attack_design_followup(input, ops,
                    fleet);
                if (followup < 0)
                    return 0;
                if (followup > 0)
                    continue;
            }
            if (input->planet_table_bound && ops->is_ai_attack &&
                    (fleet->ship_counts[0] < 1 ||
                     (StarsU16)input->turn < 41U) &&
                    !(fleet->cord == 1 &&
                      (fleet->ship_counts[14] > 0 ||
                       fleet->ship_counts[15] > 0))) {
                StarsI16 is_attack;
                StarsI16 is_transport;
                StarsI16 target_id;
                StarsI16 target_index;
                is_attack = 0;
                if (!ops->is_ai_attack(ops->context, fleet, &is_attack))
                    return 0;
                if (!is_attack) {
                    is_transport = 0;
                    if (!ops->is_ai_transport(ops->context, fleet,
                            &is_transport))
                        return 0;
                    if (is_transport) {
                        target_id = -1;
                        if (fleet->cord < 2 || fleet->order_kind != 0)
                            target_id = fleet->planet_id;
                        else if (((macinti_word(source.raw_plord + 28) >>
                                  8) & 0x000fU) == 1U)
                            target_id = (StarsI16)macinti_word(
                                source.raw_plord + 26);
                        if (target_id != -1) {
                            target_index = macinti_planet_index(input,
                                target_id);
                            if ((target_index < 0 ||
                                 (StarsI16)macinti_word(input->planets[
                                     target_index].raw_record + 2) !=
                                     input->player_id) &&
                                    macinti_word(source.raw_record + 88) ==
                                        0U &&
                                    macinti_word(source.raw_record + 90) ==
                                        0U) {
                                if (!macinti_truncate_selected_order(ops,
                                        &source))
                                    return 0;
                                macinti_bind_fleet(&source, &view);
                                fleet = &view;
                                if (!ops->clear_current_task(ops->context,
                                        fleet, 0))
                                    return 0;
                            }
                        }
                    }
                }
            }
            if (input->planet_table_bound &&
                    (StarsU16)input->turn <= 10U &&
                    (fleet->ship_counts[0] > 0 ||
                     fleet->ship_counts[2] != 0)) {
                if (!macinti_set_selected_order_kind(ops, &source, 5U))
                    return 0;
                continue;
            }
            macinti_fleet_policy(fleet, &fleet_policy);
            if (fleet_policy.armed_ships > 0)
                ++input->typed_armed_fleets;
            if (fleet_policy.colonizer_ships > 0)
                ++input->typed_colonizer_fleets;
            if (fleet_policy.mission_design_ships > 0)
                ++input->typed_mission_fleets;
            if (fleet_policy.support_design_ships > 0)
                ++input->typed_support_design_fleets;
            if (fleet_policy.attack_design_ships > 0)
                ++input->typed_attack_design_fleets;
            if (fleet_policy.needs_order_repair &&
                    fleet->raw_plord &&
                    !stars_ai_repair_first_order(fleet->raw_plord))
                return 0;
            if (ops->choose_fleet_task &&
                    !ops->choose_fleet_task(ops->context, fleet,
                    &fleet_policy, input->turn))
                return 0;
            if (!macinti_dispatch_fleet_action(ops, fleet,
                    &fleet_policy, input->turn,
                    &input->typed_fleet_actions,
                    &input->typed_fleet_actions_rejected))
                return 0;
            if (ops->visit_fleet_typed &&
                    !ops->visit_fleet_typed(ops->context, fleet,
                    &fleet_policy, input->armada_potency))
                return 0;
            ++input->typed_fleets_visited;
        }
    }
    if (ops->route_transport_fleet) {
        for (i = 0; i < input->fleet_count; ++i) {
            StarsMacintiTurnFleet view;
            const StarsMacintiTurnFleet *fleet;
            StarsI16 is_transport;
            if (input->planet_table_bound) {
                macinti_bind_fleet(&input->fleets[i], &view);
                fleet = &view;
            } else {
                fleet = &input->fleets[i];
            }
            if (fleet->owner != input->player_id)
                continue;
            if (input->planet_table_bound) {
                if (fleet->cord >= 2)
                    continue;
                is_transport = 0;
                if (!ops->is_ai_transport(ops->context, fleet,
                        &is_transport))
                    return 0;
                if (!is_transport)
                    continue;
                if (fleet->plan != 4U) {
                    if (!ops->set_transport_plan(ops->context, fleet, 4U))
                        return 0;
                    view.plan = 4U;
                }
            } else if (fleet->ship_counts[1] == 0) {
                continue;
            }
            macinti_fleet_policy(fleet, &fleet_policy);
            if (!ops->route_transport_fleet(ops->context, fleet,
                    &fleet_policy, input->armada_potency))
                return 0;
            ++input->typed_transport_routes;
        }
    }
    if (ops->finalize_fleet) {
        for (i = 0; i < input->fleet_count; ++i) {
            StarsMacintiTurnFleet view;
            const StarsMacintiTurnFleet *fleet;
            if (input->planet_table_bound) {
                macinti_bind_fleet(&input->fleets[i], &view);
                fleet = &view;
            } else {
                fleet = &input->fleets[i];
            }
            if (fleet->owner != input->player_id)
                continue;
            macinti_fleet_policy(fleet, &fleet_policy);
            if (!ops->finalize_fleet(ops->context, fleet,
                    &fleet_policy, input->armada_potency))
                return 0;
            ++input->typed_fleet_finalizations;
        }
    }
    if (ops->handle_basic_tasks) {
        if (!ops->handle_basic_tasks(ops->context))
            return 0;
        input->basic_tasks_run = 1;
    }
    if (ops->fill_production_queue) {
        if (!ops->fill_production_queue(ops->context))
            return 0;
        input->production_queue_filled = 1;
    }
    return 1;
}
