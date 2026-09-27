/* Genuine C reconstructions of the compact AI enumeration helpers. */
#include "core.h"
#include <math.h>
#include <string.h>

/* NE 12:5F32. Far planet pointers are represented by their complete
 * offset:selector values, so the original identity test remains exact. */
StarsI16 stars_enum_calc_miner_dest(StarsU32 source_planet,
    StarsU32 test_planet, const StarsU8 *ai_planets, StarsU16 test_id,
    StarsAiRandom random_value, void *context)
{
    StarsU8 value;
    StarsI16 random_number;
    if (source_planet == test_planet || !ai_planets) return 0;
    value = ai_planets[(StarsU16)(test_id * 0x10U + 1U)];
    if (value == 0U) return 0;
    random_number = random_value ? random_value(context, 100) : 0;
    if ((value & 0x80U) != 0U && random_number > 0x18) return 0;
    return (StarsI16)value;
}

/* NE 12:5FC8. The AI record must be enabled at offset ten; the selected
 * player's race stat fourteen then rejects the fully advanced value eight. */
StarsI16 stars_enum_calc_colonist_drop(StarsU32 source_planet,
    StarsU32 test_planet, const StarsU8 *ai_planets, StarsU16 test_id,
    StarsI16 player, StarsAiRaceStat get_race_stat, void *context)
{
    StarsU8 value;
    StarsU8 enabled;
    if (source_planet == test_planet || !ai_planets) return 0;
    value = ai_planets[(StarsU16)(test_id * 0x10U + 3U)];
    enabled = ai_planets[(StarsU16)(test_id * 0x10U + 10U)];
    if (enabled != 1U || value == 0U) return 0;
    if (get_race_stat && get_race_stat(context, player, 0x0e) == 8)
        return 0;
    return (StarsI16)value;
}

/* NE 16:3F00.  The source planet is an unused callback parameter. */
StarsI16 stars_enum_pick_up(const StarsEnumPickupPlanet *source_planet,
    const StarsEnumPickupPlanet *test_planet, StarsI16 player)
{
    (void)source_planet;
    if (!test_planet || test_planet->player != player)
        return 0;
    if (test_planet->pickup_count < 0)
        return 0;
    if (test_planet->pickup_count == 0 &&
        test_planet->mineral_threshold <= (StarsU16)0x0898U)
        return 0;
    if ((test_planet->overlap_flags & (StarsU16)0x0200U) == 0U)
        return 0;
    return 1;
}

/* NE 19:9C66. Walk the 18-byte thing records and replace each unowned
 * minefield's count with the square of its rounded square root. */
void stars_increase_ai_minefield_sizes(StarsAiMineThing *things,
    StarsU16 count)
{
    StarsU16 index;
    StarsU16 flags;
    StarsI32 mines;
    StarsI32 rounded;

    if (!things)
        return;
    for (index = 0; index < count; ++index) {
        flags = (StarsU16)(things[index].raw[0] |
            ((StarsU16)things[index].raw[1] << 8));
        if (((flags >> 13) & 7U) == 0U) {
            mines = (StarsI32)((StarsU32)things[index].raw[6] |
                ((StarsU32)things[index].raw[7] << 8) |
                ((StarsU32)things[index].raw[8] << 16) |
                ((StarsU32)things[index].raw[9] << 24));
            rounded = (StarsI32)(sqrt((double)mines) + 0.5);
            mines = rounded * rounded;
            things[index].raw[6] = (StarsU8)mines;
            things[index].raw[7] = (StarsU8)(mines >> 8);
            things[index].raw[8] = (StarsU8)(mines >> 16);
            things[index].raw[9] = (StarsU8)(mines >> 24);
        }
    }
}

/* NE 19:0BC2. Limit the requested transfer by the source's available
 * troopers, then remove and add exactly the accepted quantity. */
StarsI16 stars_xfer_ai_troopers(StarsI16 source, StarsI16 destination,
    StarsI16 quantity, StarsAiChangeCargo change_cargo, void *context)
{
    StarsI32 available;

    if (quantity == 0 || !change_cargo)
        return 0;
    available = change_cargo(context, 2, source, 3, 0L);
    if (available < (StarsI32)quantity)
        quantity = (StarsI16)available;
    if (quantity == 0)
        return 0;
    change_cargo(context, 2, source, 3, -(StarsI32)quantity);
    change_cargo(context, 1, destination, 3, (StarsI32)quantity);
    return quantity;
}

/* NE 19:0AD0. Reverse negative requests, clamp by source availability, then
 * debit the source only by the amount the destination accepts. */
StarsI16 stars_xfer_ai_supply(StarsI16 source_group, StarsI16 source,
    StarsI16 destination_group, StarsI16 destination, StarsI16 supply,
    StarsI16 quantity, StarsAiChangeCargo change_cargo, void *context)
{
    StarsI32 available;
    StarsI32 accepted;
    StarsI16 swap_group;
    StarsI16 swap_id;

    if (quantity == 0 || !change_cargo)
        return 0;
    if (quantity < 0) {
        swap_group = source_group; source_group = destination_group;
        destination_group = swap_group;
        swap_id = source; source = destination; destination = swap_id;
        quantity = (StarsI16)-quantity;
    }
    available = change_cargo(context, source_group, source, supply, 0L);
    if (available < (StarsI32)quantity)
        quantity = (StarsI16)available;
    if (quantity == 0)
        return 0;
    accepted = change_cargo(context, destination_group, destination, supply,
        (StarsI32)quantity);
    if (accepted != 0L)
        change_cargo(context, source_group, source, supply, -accepted);
    return (StarsI16)accepted;
}

static StarsU32 ai_long_at(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU32)bytes[offset] | ((StarsU32)bytes[offset + 1U] << 8) |
        ((StarsU32)bytes[offset + 2U] << 16) |
        ((StarsU32)bytes[offset + 3U] << 24);
}

/* NE 19:56D0. Estimate mined minerals, add the planet's stored mineral
 * weights, and publish resources after the research withholding rule. The
 * selection flag is supplied explicitly because the original reads it from
 * the global planet-selection record. */
void stars_get_resources_available(const StarsU8 *planet, StarsI32 *resources,
    StarsI16 player, StarsU32 selection_flags,
    const StarsGetResourcesAvailableOps *ops, void *context)
{
    StarsI16 index;
    StarsI16 available_word;
    StarsI16 percent;
    StarsI32 available;
    StarsI32 withholding;

    if (!planet || !resources)
        return;
    if (ops && ops->estimate_minerals)
        ops->estimate_minerals(context, planet, resources, (StarsI16)-1, 0);
    else {
        resources[0] = 0L;
        resources[1] = 0L;
        resources[2] = 0L;
    }
    for (index = 0; index < 3; ++index)
        resources[index] = (StarsI32)((StarsU32)resources[index] +
            ai_long_at(planet, (StarsU16)(0x1cU + index * 4U)));

    available_word = (ops && ops->resources_at_planet) ?
        ops->resources_at_planet(context, planet, player) : 0;
    available = (StarsI32)available_word;
    if ((selection_flags & 0x00800000UL) == 0UL &&
            ops && ops->research_percent) {
        percent = ops->research_percent(context, player);
        withholding = (StarsI32)(((StarsI32)available_word *
            (StarsI32)percent) / 100L);
        available = (StarsI32)((StarsU32)available -
            (StarsU32)withholding);
    }
    resources[3] = available;
}

/* NE 19:57C0. Clear the four production-cost totals, then sum the costs of
 * each queued four-byte production item. The queue adapter represents the
 * original far pointer and its header count without exposing that ABI. */
void stars_get_prod_q_cost(const StarsU8 *planet, StarsI32 *costs,
    StarsI16 player, const StarsGetProdQCostOps *ops, void *context)
{
    StarsU16 count;
    StarsU16 index;
    StarsU16 cost_index;
    const StarsU8 *queue;
    const StarsU8 *item;
    StarsI32 item_costs[4];

    if (!costs)
        return;
    for (cost_index = 0; cost_index < 4; ++cost_index)
        costs[cost_index] = 0L;
    queue = (planet && ops && ops->get_queue) ?
        ops->get_queue(context, planet, &count) : 0;
    if (!queue || !ops || !ops->get_costs)
        return;
    for (index = 0; index < count; ++index) {
        item = queue + 4U + index * 4U;
        ops->get_costs(context, planet, item, item_costs, player, 0);
        for (cost_index = 0; cost_index < 4; ++cost_index)
            costs[cost_index] = (StarsI32)((StarsU32)costs[cost_index] +
                (StarsU32)item_costs[cost_index]);
    }
}

static StarsU32 ai_point_distance(StarsPoint first, StarsPoint second)
{
    StarsI32 dx = (StarsI32)first.x - (StarsI32)second.x;
    StarsI32 dy = (StarsI32)first.y - (StarsI32)second.y;
    return (StarsU32)(dx * dx + dy * dy);
}

/* NE 19:5BAE. Select the nearest fleet accepted by the enumerator. A null
 * candidate with a zero selector is the original far-pointer-list sentinel. */
const StarsU8 *stars_lpfl_find_closest_enum(const StarsU8 *source,
    const StarsU8 *const *candidates, const StarsU16 *selectors,
    StarsU16 count, StarsU16 *selected_selector,
    const StarsAiEnumOps *ops, void *context)
{
    StarsPoint source_point;
    StarsPoint candidate_point;
    const StarsU8 *best = 0;
    StarsU32 best_distance = 10000000UL;
    StarsU16 best_selector = 0;
    StarsU16 index;
    StarsU16 selector;

    if (selected_selector) *selected_selector = 0;
    if (!source || !candidates || !ops || !ops->get_point ||
            !ops->predicate)
        return 0;
    ops->get_point(context, source, &source_point);
    for (index = 0; index < count; ++index) {
        selector = selectors ? selectors[index] : 0;
        if (!candidates[index] && selector == 0U)
            break;
        ops->get_point(context, candidates[index], &candidate_point);
        if (ai_point_distance(source_point, candidate_point) < best_distance &&
                ops->predicate(context, source, candidates[index]) != 0) {
            best = candidates[index];
            best_selector = selector;
            best_distance = ai_point_distance(source_point, candidate_point);
        }
    }
    if (selected_selector) *selected_selector = best_selector;
    return best;
}

/* NE 19:5CD4. Select the nearest planet accepted by the enumerator. */
const StarsU8 *stars_lppl_find_closest_enum(const StarsU8 *source,
    const StarsU8 *const *candidates, StarsU16 count,
    const StarsAiEnumOps *ops, void *context)
{
    StarsPoint source_point;
    StarsPoint candidate_point;
    const StarsU8 *best = 0;
    StarsU32 best_distance = 10000000UL;
    StarsU32 distance;
    StarsU16 index;

    if (!source || !candidates || !ops || !ops->get_point ||
            !ops->predicate)
        return 0;
    ops->get_point(context, source, &source_point);
    for (index = 0; index < count; ++index) {
        if (!candidates[index])
            continue;
        ops->get_point(context, candidates[index], &candidate_point);
        distance = ai_point_distance(source_point, candidate_point);
        if (distance < best_distance &&
                ops->predicate(context, source, candidates[index]) != 0) {
            best = candidates[index];
            best_distance = distance;
        }
    }
    return best;
}

/* NE 19:5E06. Choose the highest predicate score, breaking equal scores by
 * the nearest point; the original starts with a score threshold of one. */
const StarsU8 *stars_lppl_find_best_enum(const StarsU8 *source,
    const StarsU8 *const *candidates, StarsU16 count,
    const StarsAiEnumOps *ops, void *context)
{
    StarsPoint source_point;
    StarsPoint candidate_point;
    const StarsU8 *best = 0;
    StarsU32 best_distance = 10000000UL;
    StarsU32 distance;
    StarsI16 best_score = 1;
    StarsI16 score;
    StarsU16 index;

    if (!source || !candidates || !ops || !ops->get_point ||
            !ops->predicate)
        return 0;
    ops->get_point(context, source, &source_point);
    for (index = 0; index < count; ++index) {
        if (!candidates[index])
            continue;
        ops->get_point(context, candidates[index], &candidate_point);
        distance = ai_point_distance(source_point, candidate_point);
        score = ops->predicate(context, source, candidates[index]);
        if (best_score < score ||
                (score == best_score && distance < best_distance)) {
            best = candidates[index];
            best_score = score;
            best_distance = distance;
        }
    }
    return best;
}

/* NE 19:5F54. Reservoir-sample all planets inside the inclusive radius. If
 * starbases are to be avoided, repeat the sampling up to the same two
 * retries as the original before accepting the final selected id. */
StarsI16 stars_id_random_planet_nearby(StarsPoint center, StarsI16 distance,
    StarsI16 avoid_starbases, const StarsPoint *points,
    const StarsU16 *planet_flags, StarsU16 count, StarsAiRandom random_value,
    void *context)
{
    StarsU32 radius;
    StarsU32 candidate_distance;
    StarsI32 dx;
    StarsI32 dy;
    StarsI16 retries;
    StarsI16 selected;
    StarsI16 seen;
    StarsI16 random_number;
    StarsU16 index;

    if (!points || !random_value)
        return (StarsI16)-1;
    retries = avoid_starbases ? 2 : 0;
    radius = (StarsU32)((StarsI32)distance * (StarsI32)distance);
    for (;;) {
        seen = 1;
        selected = (StarsI16)-1;
        for (index = 0; index < count; ++index) {
            dx = (StarsI32)center.x - (StarsI32)points[index].x;
            dy = (StarsI32)center.y - (StarsI32)points[index].y;
            candidate_distance = (StarsU32)(dx * dx + dy * dy);
            if (candidate_distance <= radius) {
                random_number = random_value(context, seen);
                ++seen;
                if (random_number == 0)
                    selected = (StarsI16)index;
            }
        }
        if (selected < 0 || retries < 1)
            return selected;
        --retries;
        if (!planet_flags || (planet_flags[(StarsU16)selected] & 0x0200U) == 0U)
            return selected;
    }
}

/* NE 19:6896. Mark owned planets targeted by an enemy fleet carrying at
 * least one hull in the attack category range 16..19. */
void stars_mark_planets_under_attack(const StarsU8 *const *fleets,
    StarsU16 count, StarsI16 player,
    const StarsMarkPlanetsUnderAttackOps *ops, void *context)
{
    const StarsU8 *fleet;
    const StarsU8 *planet;
    StarsI16 fleet_player;
    StarsI16 planet_id;
    StarsI16 design;
    StarsI16 hull_category;
    StarsI16 has_attacker;
    StarsU16 fleet_index;
    StarsI16 ship_count;

    if (!fleets || !ops || !ops->hull_category || !ops->lookup_planet ||
            !ops->mark_planet)
        return;
    for (fleet_index = 0; fleet_index < count; ++fleet_index) {
        fleet = fleets[fleet_index];
        if (!fleet)
            break;
        fleet_player = (StarsI16)(fleet[2] |
            ((StarsU16)fleet[3] << 8));
        planet_id = (StarsI16)(fleet[6] |
            ((StarsU16)fleet[7] << 8));
        if (fleet_player == player || planet_id == (StarsI16)-1)
            continue;
        has_attacker = 0;
        for (design = 0; design < 16; ++design) {
            ship_count = (StarsI16)(fleet[12U + design * 2U] |
                ((StarsU16)fleet[13U + design * 2U] << 8));
            if (ship_count < 1)
                continue;
            hull_category = ops->hull_category(context, fleet_player,
                (StarsU16)design);
            if (hull_category >= 16 && hull_category <= 19) {
                has_attacker = 1;
                break;
            }
        }
        if (!has_attacker)
            continue;
        planet = ops->lookup_planet(context, planet_id);
        if (planet && (StarsI16)(planet[2] |
                ((StarsU16)planet[3] << 8)) == player)
            ops->mark_planet(context, (StarsI16)(planet[0] |
                ((StarsU16)planet[1] << 8)));
    }
}

/* NE 19:6E04. Find the nearest owned starbase to a fleet order point. The
 * optional big-base predicate captures the original fourth mineral/resource
 * test without coupling this source adapter to the global planet tables. */
StarsI16 stars_idpl_find_closest_starbase(StarsPoint source,
    const StarsU8 *const *planets, const StarsPoint *points, StarsU16 count,
    StarsI16 player, StarsI16 big_ones, StarsAiBigStarbase is_big,
    void *context)
{
    const StarsU8 *best = 0;
    StarsU32 best_distance = 10000000UL;
    StarsU32 distance;
    StarsI32 dx;
    StarsI32 dy;
    StarsU16 index;
    StarsI16 owner;

    if (!planets || !points)
        return (StarsI16)-1;
    for (index = 0; index < count; ++index) {
        if (!planets[index])
            continue;
        owner = (StarsI16)(planets[index][2] |
            ((StarsU16)planets[index][3] << 8));
        if (owner != player)
            continue;
        if ((StarsU16)(planets[index][4] |
                ((StarsU16)planets[index][5] << 8)) & 0x0200U) {
            if (big_ones && (!is_big || !is_big(context, planets[index])))
                continue;
            dx = (StarsI32)source.x - (StarsI32)points[index].x;
            dy = (StarsI32)source.y - (StarsI32)points[index].y;
            distance = (StarsU32)(dx * dx + dy * dy);
            if (distance < best_distance) {
                best = planets[index];
                best_distance = distance;
            }
        }
    }
    return best ? (StarsI16)(best[0] | ((StarsU16)best[1] << 8)) :
        (StarsI16)-1;
}

/* NE 19:12E6. Rank nearby wormholes by accessibility and move cost, retain
 * the nearest candidate on equal scores, then apply the original final
 * percentage roll. Thing records are the documented 18-byte layout. */
const StarsU8 *stars_lpth_worm_find(StarsPoint source, StarsU32 distance_limit,
    StarsI16 player, const StarsU8 *const *things, StarsU16 count,
    StarsGetWormholeMovePercent move_percent, StarsAiRandom random_value,
    void *context)
{
    const StarsU8 *best = 0;
    StarsU32 best_distance = 0x00004240UL;
    StarsU32 distance;
    StarsU32 limit;
    StarsI32 dx;
    StarsI32 dy;
    StarsI16 best_score = 0;
    StarsI16 score;
    StarsI16 index;
    StarsU16 flags;
    StarsU16 visited;
    StarsPoint point;
    StarsI16 random_number;

    if (!things || !random_value)
        return 0;
    limit = distance_limit * 4UL;
    for (index = 0; (StarsU16)index < count; ++index) {
        if (!things[(StarsU16)index])
            continue;
        flags = (StarsU16)(things[(StarsU16)index][0] |
            ((StarsU16)things[(StarsU16)index][1] << 8));
        if ((flags >> 13) != 2U)
            continue;
        point.x = (StarsI16)(things[(StarsU16)index][2] |
            ((StarsU16)things[(StarsU16)index][3] << 8));
        point.y = (StarsI16)(things[(StarsU16)index][4] |
            ((StarsU16)things[(StarsU16)index][5] << 8));
        dx = (StarsI32)source.x - (StarsI32)point.x;
        dy = (StarsI32)source.y - (StarsI32)point.y;
        distance = (StarsU32)(dx * dx + dy * dy);
        if (distance > limit)
            continue;
        visited = (StarsU16)(things[(StarsU16)index][10] |
            ((StarsU16)things[(StarsU16)index][11] << 8));
        if ((visited & (StarsU16)(1U << (player & 0x0f))) == 0U) {
            score = distance_limit < distance ? 50 : 90;
        } else {
            score = move_percent ? move_percent(context,
                things[(StarsU16)index]) : 0;
            score = (StarsI16)(0x46 - score * 10);
        }
        if (best_score < score ||
                (best_score == score && distance <= best_distance)) {
            best = things[(StarsU16)index];
            best_score = score;
            best_distance = distance;
        }
    }
    if (!best)
        return 0;
    random_number = random_value(context, 100);
    if (best_score <= random_number)
        return 0;
    return best;
}

/* NE 19:15A4. Use the AI planet-status byte to find the nearest unknown
 * planet, optionally replacing that result with a wormhole on the original
 * five-percent diversion path. */
StarsI16 stars_id_nearest_unknown_planet_strided(StarsPoint source,
    const StarsU8 *status, StarsU16 status_stride, StarsU16 status_offset,
    const StarsPoint *points, StarsU16 count, StarsI16 fleet_has_planet,
    StarsI16 status_ready, StarsPrepareUnknownPlanetStatus prepare_status,
    StarsFindAiWormhole find_wormhole, StarsAiRandom random_value,
    void *context, const StarsU8 **wormhole)
{
    StarsU32 best_distance = 99999999UL;
    StarsU32 distance;
    StarsI32 dx;
    StarsI32 dy;
    StarsI16 best = -1;
    StarsI16 index;
    StarsPoint point;

    if (wormhole) *wormhole = 0;
    if (!status || !points || status_stride == 0U ||
            status_offset >= status_stride)
        return (StarsI16)-1;
    if (!status_ready) {
        if (prepare_status && status_stride == 1U && status_offset == 0U)
            prepare_status(context, (StarsU8 *)status, count);
        else
            return (StarsI16)-1;
    }
    for (index = 0; (StarsU16)index < count; ++index) {
        if (status[(StarsU16)index * status_stride + status_offset] != 0x10U)
            continue;
        point = points[(StarsU16)index];
        dx = (StarsI32)source.x - (StarsI32)point.x;
        dy = (StarsI32)source.y - (StarsI32)point.y;
        distance = (StarsU32)(dx * dx + dy * dy);
        if (distance < best_distance) {
            best = index;
            best_distance = distance;
        }
    }
    if (wormhole && fleet_has_planet && random_value &&
            random_value(context, 100) < 5 && find_wormhole) {
        *wormhole = find_wormhole(context, source, best_distance);
        if (*wormhole)
            best = -1;
    }
    return best;
}

StarsI16 stars_id_nearest_unknown_planet(StarsPoint source, StarsU8 *status,
    const StarsPoint *points, StarsU16 count, StarsI16 fleet_has_planet,
    StarsI16 status_ready, StarsPrepareUnknownPlanetStatus prepare_status,
    StarsFindAiWormhole find_wormhole, StarsAiRandom random_value,
    void *context, const StarsU8 **wormhole)
{
    return stars_id_nearest_unknown_planet_strided(source, status, 1U, 0U,
        points, count, fleet_has_planet, status_ready, prepare_status,
        find_wormhole, random_value, context, wormhole);
}

/* NE 19:3D0A. Update the selected fleet's order list, replacing its first
 * order or appending after the current tail as requested. Equal endpoints
 * collapse the list back to one order before the final fleet refresh. */
StarsI16 stars_f_move_ai_fleet(const StarsU8 *fleet,
    const StarsMoveOrder *order, StarsI16 append,
    StarsMoveAiFleetState *state, void *context)
{
    StarsU16 order_index;
    StarsI16 fleet_id;

    if (!fleet || !order || !state || !state->orders ||
            state->capacity == 0U || state->count >= state->capacity)
        return 0;
    fleet_id = (StarsI16)(fleet[0] | ((StarsU16)fleet[1] << 8));
    if (state->select_fleet)
        state->select_fleet(context, 2, fleet_id);
    if (append && state->count > 0U)
        order_index = (StarsU16)(state->count - 1U);
    else
        order_index = 0U;
    if (state->count > 0U &&
            state->orders[order_index].point.x == order->point.x &&
            state->orders[order_index].point.y == order->point.y) {
        if (state->count > 1U) {
            state->count = 1U;
            if (state->refresh_fleet)
                state->refresh_fleet(context);
        }
    } else {
        ++order_index;
    }
    state->orders[order_index] = *order;
    state->count = (StarsU16)(order_index + 1U);
    return state->refresh_fleet ? state->refresh_fleet(context) : 0;
}

static void ai_put_prod_record(StarsU8 *record, StarsU16 item,
    StarsU16 quantity, StarsU16 object_group)
{
    StarsU32 packed = ((StarsU32)quantity & 0x03ffUL) |
        (((StarsU32)item & 0x007fUL) << 10) |
        (((StarsU32)object_group & 7UL) << 17);
    record[0] = (StarsU8)packed;
    record[1] = (StarsU8)(packed >> 8);
    record[2] = (StarsU8)(packed >> 16);
    record[3] = (StarsU8)(packed >> 24);
}

/* NE 19:3E50. Add a packed production item in front, at the tail, or after
 * clearing the queue, with the temporary-list clamp used by AI initialization. */
void stars_add_item_to_queue(StarsAiProductionQueue *selected,
    StarsAiProductionQueue *global, StarsU16 item, StarsU16 quantity,
    StarsU16 object_group, StarsI16 mode,
    const StarsAddItemToQueueOps *ops, void *context)
{
    StarsAiProductionQueue temporary;
    StarsU8 temporary_records[64 * 4];
    StarsU16 limit;
    StarsU16 insert_at;
    StarsI16 initialized = 0;

    if (selected && selected->count > 200U)
        return;
    if (!global) {
        memset(temporary_records, 0, sizeof(temporary_records));
        temporary.records = temporary_records;
        temporary.count = 0;
        temporary.capacity = 64;
        global = &temporary;
        initialized = 1;
        if (ops && ops->initialize)
            ops->initialize(context, temporary_records);
        limit = (ops && ops->item_capacity) ?
            ops->item_capacity(context, item, object_group) : 0U;
        if (quantity > limit)
            quantity = limit;
    }
    if (quantity == 0U) {
        if (initialized && ops && ops->finish)
            ops->finish(context, 0);
        return;
    }
    if (mode != 2 && global->count >= global->capacity) {
        if (initialized && ops && ops->finish)
            ops->finish(context, 0);
        return;
    }
    if (mode == 0) {
        memmove(global->records + 4U, global->records,
            (size_t)global->count * 4U);
        insert_at = 0;
    } else if (mode == 1) {
        insert_at = global->count;
    } else if (mode == 2) {
        global->count = 0;
        insert_at = 0;
    } else {
        if (initialized && ops && ops->finish)
            ops->finish(context, 0);
        return;
    }
    ai_put_prod_record(global->records + insert_at * 4U, item, quantity,
        object_group);
    ++global->count;
    if (initialized && ops && ops->finish)
        ops->finish(context, 1);
}

/* NE 19:425A. Update AI research state from the ordered requirement bytes,
 * selecting the next incomplete field or the least-developed field when all
 * supplied requirements are met. */
StarsI16 stars_iro_ensure_ai(StarsU8 *requirements, StarsI16 requirement_count,
    StarsI16 *latest_starbase, StarsI16 percent, StarsIroAiState *state,
    const StarsIroAiOps *ops, void *context)
{
    StarsI16 index;
    StarsI16 tech_index;
    StarsI16 level;
    StarsI16 minimum;
    StarsU8 next_tech;

    if (!requirements || !state)
        return (StarsI16)-1;
    if (latest_starbase) {
        if (ops && ops->ensure_designs)
            ops->ensure_designs(context);
        *latest_starbase = (ops && ops->latest_starbase) ?
            ops->latest_starbase(context) : (StarsI16)-1;
        if (ops && ops->validate_history)
            ops->validate_history(context);
    }
    state->percent_research = (StarsI16)(signed char)percent;
    for (index = 0; index < 6 && state->tech[index] > 0x17U; ++index)
        ;
    if (index == 6) {
        state->percent_research = 0;
        if (ops && ops->write_research)
            ops->write_research(context, state->current_tech,
                state->percent_research);
    }
    for (index = 0; index < requirement_count; ++index) {
        tech_index = (StarsI16)(requirements[index] >> 5);
        level = (StarsI16)(requirements[index] & 0x1fU);
        if (tech_index < 0 || tech_index >= 6 ||
                state->tech[tech_index] < (StarsU8)level)
            break;
    }
    if (index >= requirement_count) {
        minimum = 0;
        for (tech_index = 1; tech_index < 6; ++tech_index) {
            if (state->tech[tech_index] < state->tech[minimum])
                minimum = tech_index;
        }
        if ((state->current_tech & 0x0fU) != (StarsU8)minimum) {
            state->current_tech = (StarsU8)((state->current_tech & 0xf0U) |
                (StarsU8)minimum);
            /* The original low-nibble comparison against 0x1A is
             * unreachable for a four-bit field. */
            if (ops && ops->write_research)
                ops->write_research(context, state->current_tech,
                    state->percent_research);
        }
        return 0x39e;
    }
    state->current_tech = (StarsU8)((state->current_tech & 0xf0U) |
        (StarsU8)tech_index);
    if (index < requirement_count - 1 &&
            state->tech[tech_index] + 1U ==
            (StarsU8)(requirements[index] & 0x1fU)) {
        next_tech = (StarsU8)(requirements[index + 1] >> 5);
        state->current_tech = (StarsU8)((state->current_tech & 0x0fU) |
            (StarsU8)(next_tech << 4));
    }
    if (ops && ops->write_research)
        ops->write_research(context, state->current_tech,
            state->percent_research);
    return index;
}

/* NE 19:45CA. Gather hostile ordinary things, using -1 when the original
 * 100-entry local list overflows, then set the ideal speed of eligible own
 * fleets according to the race mode and transport classification. */
void stars_keep_fleets_moving(const StarsU8 *const *things, StarsU16 thing_count,
    StarsI16 player, StarsI16 turn, const StarsU8 *const *fleets,
    StarsU16 fleet_count, const StarsKeepFleetsMovingOps *ops, void *context)
{
    const StarsU8 *threats[100];
    const StarsU8 *fleet;
    StarsU16 threat_count = 0;
    StarsU16 thing_index;
    StarsU16 fleet_index;
    StarsU16 flags;
    StarsI16 owner;
    StarsI16 speed;
    StarsU16 cord;

    if (!fleets || !ops || !ops->set_speed)
        return;
    if (things) {
        for (thing_index = 0; thing_index < thing_count; ++thing_index) {
            if (!things[thing_index])
                continue;
            flags = (StarsU16)(things[thing_index][0] |
                ((StarsU16)things[thing_index][1] << 8));
            owner = (StarsI16)((flags >> 9) & 0x0fU);
            if ((flags >> 13) != 0U || owner == player ||
                    things[thing_index][12] == 2U)
                continue;
            if (threat_count == 100U) {
                threat_count = (StarsU16)-1;
                break;
            }
            threats[threat_count++] = things[thing_index];
        }
    }
    for (fleet_index = 0; fleet_index < fleet_count; ++fleet_index) {
        fleet = fleets[fleet_index];
        if (!fleet)
            break;
        owner = (StarsI16)(fleet[2] | ((StarsU16)fleet[3] << 8));
        cord = (StarsU16)(fleet[98] | ((StarsU16)fleet[99] << 8));
        if (owner != player || cord <= 1U)
            continue;
        if (turn > 4 || turn < 0) {
            speed = -1;
            if (ops->is_transport && ops->is_transport(context, fleet))
                speed = 30;
        } else {
            speed = 16;
        }
        ops->set_speed(context, fleet, speed, (StarsI16)threat_count,
            threats);
    }
}

/* NE 19:9F30. Before turn sixty the source is always allowed to build a
 * colonizer; afterward, use the nearest unmarked planet and the original
 * distance gates with their two independent coin flips. */
StarsI16 stars_f_should_planet_build_colonizer(StarsPoint source,
    StarsI16 turn, const StarsPoint *planet_points,
    const StarsU8 *planet_status, StarsU16 planet_count,
    StarsAiRandom random_value, void *context)
{
    StarsU32 nearest = 10000000UL;
    StarsU32 distance;
    StarsI32 dx;
    StarsI32 dy;
    StarsI16 random_number;
    StarsU16 index;

    if (turn < 60)
        return 1;
    if (!planet_points || !planet_status)
        return 0;
    for (index = 0; index < planet_count; ++index) {
        if (planet_status[index * 0x10U + 0x0dU] == 0U) {
            dx = (StarsI32)source.x - (StarsI32)planet_points[index].x;
            dy = (StarsI32)source.y - (StarsI32)planet_points[index].y;
            distance = (StarsU32)(dx * dx + dy * dy);
            if (distance < nearest)
                nearest = distance;
        }
    }
    if (nearest >= 0x1de85UL)
        return 0;
    if (nearest >= 0x15f91UL) {
        random_number = random_value ? random_value(context, 2) : 0;
        if (random_number == 0)
            return 0;
    }
    if (nearest >= 0xf425UL) {
        random_number = random_value ? random_value(context, 2) : 0;
        if (random_number == 0)
            return 0;
    }
    return 1;
}

/* NE 19:A082. Collect the current player's planets and Fisher-Yates shuffle
 * them unless the game-data flag requests stable order. */
void stars_init_random_planet_list(const StarsU8 *const *planets,
    StarsU16 planet_count, StarsI16 player, StarsU16 stable_order,
    const StarsU8 **out_planets, StarsU16 *out_count,
    StarsAiRandom random_value, void *context)
{
    StarsU16 count = 0;
    StarsU16 index;
    StarsU16 swap_index;
    const StarsU8 *temporary;
    if (out_count)
        *out_count = 0;
    if (!planets || !out_planets || !out_count)
        return;
    for (index = 0; index < planet_count; ++index) {
        if (planets[index] &&
                (StarsI16)(planets[index][2] |
                ((StarsU16)planets[index][3] << 8)) == player)
            out_planets[count++] = planets[index];
    }
    if (!stable_order && random_value) {
        for (index = 0; index + 1U < count; ++index) {
            swap_index = (StarsU16)(index +
                (StarsU16)random_value(context, (StarsI16)(count - index)));
            if (swap_index >= count)
                swap_index = (StarsU16)(count - 1U);
            temporary = out_planets[index];
            out_planets[index] = out_planets[swap_index];
            out_planets[swap_index] = temporary;
        }
    }
    *out_count = count;
}

/* NE 19:43E. Walk the packed AI-part alternatives, trying each requested
 * technology level from highest to lowest until the part lookup succeeds. */
StarsI16 stars_f_get_ai_part(StarsU16 part_index, const StarsU8 *part_counts,
    StarsU16 packed_count, const StarsU16 *packed_parts,
    StarsLookupAiPart lookup_part, StarsAiPart *result, void *context)
{
    StarsU16 offset = 0;
    StarsU16 remaining;
    StarsU16 word;
    StarsU16 level;
    StarsAiPart candidate;

    if (!part_counts || !packed_parts || !lookup_part || !result)
        return 0;
    for (level = 0; level < part_index; ++level)
        offset = (StarsU16)(offset + part_counts[level]);
    remaining = part_counts[part_index];
    while (remaining != 0U) {
        --remaining;
        if (offset >= packed_count)
            return 0;
        word = packed_parts[offset++];
        candidate.tech_group = (StarsU16)(1U << (word & 0x000fU));
        candidate.attributes = (StarsU16)((word >> 4) & 0x001fU);
        for (level = (StarsU16)((word >> 9) & 0x000fU); level != 0U;
                --level) {
            if (lookup_part(context, candidate.tech_group,
                    candidate.attributes, &candidate) != 0) {
                *result = candidate;
                return 1;
            }
            candidate.attributes = (StarsU16)((candidate.attributes - 1U) &
                0x001fU);
        }
    }
    return 0;
}

static void ai_shdef_name_defaults(StarsU16 hull_id, StarsI16 *name_base,
    StarsI16 *name_length)
{
    StarsU32 hull_bit = 0UL;

    if (hull_id < 32U)
        hull_bit = 1UL << hull_id;
    if ((hull_bit & 0x780UL) != 0UL) {
        *name_base = 0x3de;
        *name_length = 0x10;
    } else if ((hull_bit & 0x40UL) != 0UL) {
        *name_base = 0x422;
        *name_length = 0x10;
    } else if ((hull_bit & 0x70UL) != 0UL) {
        *name_base = 0x3d4;
        *name_length = 10;
    } else if ((hull_bit & 0xf0000UL) != 0UL) {
        *name_base = 0x3ee;
        *name_length = 0xc;
    } else if ((hull_bit & 0xfUL) != 0UL) {
        *name_base = 0x40a;
        *name_length = 8;
    } else if ((hull_bit & 0x1f00000UL) != 0UL) {
        *name_base = 0x3fa;
        *name_length = 8;
    } else if ((hull_bit & 0x3800UL) != 0UL) {
        *name_base = 0x412;
        *name_length = 8;
    } else if ((hull_bit & 0xc000UL) != 0UL) {
        *name_base = 0x402;
        *name_length = 8;
    } else {
        *name_base = 0x41a;
        *name_length = 8;
    }
}

/* NE 19:12C. Build a complete AI ship design from a hull and the requested
 * part alternatives. The callbacks stand in for the original hull/part
 * tables and publication routines while keeping this implementation in C. */
StarsI16 stars_f_create_ai_shdef(StarsI16 design_index, StarsU16 hull_id,
    const StarsU8 *part_counts, StarsLookupAiHull lookup_hull,
    StarsLookupAiShdefPart lookup_part, StarsNameAiShdef name_shdef,
    StarsPublishAiShdef publish_shdef, void *context)
{
    StarsAiShdefWork work;
    StarsAiPart part;
    StarsU16 index;
    StarsU16 total_parts;
    StarsI16 name_base;
    StarsI16 name_length;

    if (!part_counts || !lookup_hull || !lookup_part || !publish_shdef)
        return 0;
    work.hull_id = hull_id;
    work.part_count = 0;
    work.flags = 0;
    work.name_base = 0;
    work.name_length = 0;
    work.bitmap_id = 0;
    if (lookup_hull(context, (StarsU16)(design_index < 0 ? 0x0400U : 0x4000U),
            hull_id, &work) == 0)
        return 0;
    total_parts = work.part_count;
    if (total_parts > 64U)
        total_parts = 64U;
    work.part_count = 0;
    for (index = 0; index < total_parts; ++index) {
        if (lookup_part(context, (StarsU16)part_counts[index], &part) == 0)
            return 0;
        work.part_groups[index] = part.tech_group;
        work.part_attributes[index] = part.attributes;
        work.part_ids[index] = (StarsU16)(part.component ?
            (part.component[0] | ((StarsU16)part.component[1] << 8)) : 0U);
        ++work.part_count;
    }
    work.flags = (StarsU16)(work.flags & 0xfdffU);
    if (design_index < 0)
        return publish_shdef(context, -1, &work);
    work.flags = (StarsU16)((work.flags & 0x83ffU) |
        ((StarsU16)design_index & 0x001fU) << 10);
    ai_shdef_name_defaults(hull_id, &name_base, &name_length);
    work.name_base = name_base;
    work.name_length = name_length;
    if (name_shdef)
        name_shdef(context, &work, name_base, name_length, work.bitmap_id);
    return publish_shdef(context, design_index, &work);
}

/* NE 19:55A. Choose an unused bitmap from the four-image family and then
 * retry generated class names, falling back to the original numbered form
 * after twenty collisions. */
void stars_pick_ai_name_and_bitmap(StarsAiShdefWork *work, StarsI16 name_start,
    StarsI16 name_count, StarsI16 bitmap_start, const StarsPickAiNameOps *ops,
    void *context)
{
    StarsU8 bitmap_used[4];
    StarsU16 index;
    StarsI16 attempt;
    StarsI16 name_number;
    StarsI16 random_number;
    StarsI16 bitmap;
    StarsU16 design_class;

    if (!work || !ops || !ops->random_value || !ops->get_name)
        return;
    for (index = 0; index < 4U; ++index)
        bitmap_used[index] = 0U;
    design_class = (StarsU16)((work->flags >> 10) & 0x001fU);
    for (index = 0; index < 4U; ++index)
        if (ops->bitmap_in_use &&
                ops->bitmap_in_use(context, (StarsU16)(bitmap_start + index),
                work->hull_id) != 0)
            bitmap_used[index] = 1U;
    bitmap = 0;
    while (bitmap < 4 && bitmap_used[(StarsU16)bitmap] != 0U)
        ++bitmap;
    if (bitmap == 4)
        bitmap = ops->random_value(context, 4);
    if (bitmap < 0 || bitmap > 3)
        bitmap = 0;
    work->bitmap_id = (StarsI16)(bitmap_start + bitmap);
    for (attempt = 0; attempt <= 20; ++attempt) {
        random_number = ops->random_value(context, name_count);
        if (random_number < 0)
            random_number = 0;
        name_number = (StarsI16)(name_start + random_number);
        if (ops->get_name(context, name_number, work->name,
                (StarsU16)sizeof(work->name)) == 0)
            return;
        if (ops->name_in_use == 0 ||
                ops->name_in_use(context, design_class, work->name) == 0) {
            return;
        }
    }
    random_number = ops->random_value(context, 100);
    name_number = ops->random_value(context, name_count);
    if (name_number < 0)
        name_number = 0;
    if (ops->format_name)
        ops->format_name(context, work->name, (StarsU16)sizeof(work->name),
            (StarsI16)(name_start + name_number), random_number);
}

/* NE 19:8A2. Recalculate a design, reconcile its active state with the
 * previous slot, adjust the owning player's design count, then store and log
 * the new record. */
StarsI16 stars_f_change_ai_shdef(StarsAiShdefWork *work, StarsI16 design_index,
    StarsU16 turn, const StarsChangeAiShdefOps *ops, void *context)
{
    StarsU16 old_flags;
    StarsU16 new_active;
    StarsU16 old_active;
    StarsI16 slot;
    StarsI16 delta;

    if (!work || !ops || !ops->old_flags || !ops->set_flags ||
            !ops->change_count || !ops->update_cost || !ops->log_change)
        return 0;
    work->flags = (StarsU16)((work->flags & 0x83ffU) |
        ((StarsU16)design_index & 0x001fU) << 10);
    work->turn = turn;
    work->existing = 0UL;
    work->built = 0UL;
    slot = design_index < 16 ? design_index : (StarsI16)(design_index - 16);
    old_flags = ops->old_flags(context, slot);
    ops->update_cost(context, work);
    new_active = (StarsU16)(work->flags & 0x0200U);
    old_active = (StarsU16)(old_flags & 0x0200U);
    if (new_active == old_active) {
        if (new_active == 0U) {
            ops->set_flags(context, slot, (StarsU16)(old_flags | 0x0200U));
            ops->log_change(context, slot, work);
        }
    } else {
        delta = new_active == 0U ? 1 : -1;
        ops->change_count(context, ops->player_design_count, delta);
    }
    ops->log_change(context, slot, work);
    return 1;
}

/* NE 19:1792. Add mine production for planets whose existing production is
 * blocked, while respecting resource availability, research withholding,
 * operable-mine capacity, and the production queue's timing guard. */
void stars_add_mines_to_blocked_queues(StarsAiBlockedMinePlanet *const *planets,
    StarsU16 planet_count, const StarsAddBlockedMinesOps *ops, void *context)
{
    StarsU16 index;
    StarsAiBlockedMinePlanet *planet;
    StarsI32 resources;
    StarsI32 capacity;
    StarsI16 amount;

    if (!planets || !ops || !ops->resources_at_planet ||
            !ops->add_queue_item || !ops->mark_queue)
        return;
    for (index = 0; index < planet_count; ++index) {
        planet = planets[index];
        if (!planet)
            break;
        if ((planet->production_flags & 0x0007U) == 1U &&
                ((planet->production_flags >> 10) & 0x007fU) != 8U &&
                ((planet->production_flags >> 10) & 0x007fU) != 3U &&
                ((planet->production_flags >> 10) & 0x007fU) != 11U &&
                ((planet->production_flags >> 10) & 0x007fU) != 12U)
            continue;
        if (planet->current_eta == 1)
            continue;
        resources = (StarsI32)ops->resources_at_planet(context, planet->id);
        if (resources < 0 || planet->mine_cost <= 0)
            continue;
        capacity = (StarsI32)planet->max_mines - planet->existing_mines;
        if (capacity < 0)
            capacity = 0;
        amount = (StarsI16)(resources / planet->mine_cost);
        if ((StarsI32)amount > capacity)
            amount = (StarsI16)capacity;
        if (amount < 1) {
            ops->add_queue_item(context, planet->id, 3, 1);
            ops->mark_queue(context, planet->id, 700);
            continue;
        }
        ops->add_queue_item(context, planet->id, 8, amount);
        if (planet->queued_eta < 0)
            planet->queued_eta = 700;
        if (planet->current_eta <= planet->queued_eta ||
                planet->queued_eta >= 700)
            ops->mark_queue(context, planet->id, planet->queued_eta);
    }
}

/* NE 19:1FFE. Select the nearest valid enemy fleet, otherwise fall back to a
 * nearby enemy planet or an unclaimed planet, then issue the movement order.
 * Fleet and planet linked lists are represented as explicit arrays here. */
StarsI16 stars_id_target_attack(const StarsAiTargetFleet *self,
    const StarsAiTargetFleet *attackers, StarsU16 attacker_count,
    const StarsAiTargetFleet *enemies, StarsU16 enemy_count, StarsI16 only_humans,
    const StarsAiTargetPlanet *planets, StarsU16 planet_count, StarsI16 owner,
    const StarsTargetAttackOps *ops, void *context)
{
    StarsU16 index;
    StarsU16 best_id = 0xffffU;
    StarsPoint best_point;
    StarsU32 best_distance = 0xffffffffUL;
    StarsU32 distance;
    StarsI32 own_power = 0;
    StarsI32 supporting_power;
    StarsI16 dx;
    StarsI16 dy;

    if (!self || !ops || !ops->move_target)
        return -1;
    own_power = self->combat_power;
    for (index = 0; index < attacker_count; ++index)
        if (attackers[index].in_transit && attackers[index].combat_power > 0)
            own_power += attackers[index].combat_power;
    for (index = 0; index < enemy_count; ++index) {
        if (only_humans && enemies[index].human == 0U)
            continue;
        supporting_power = 0;
        if (attackers) {
            StarsU16 other;
            for (other = 0; other < attacker_count; ++other)
                if (attackers[other].planet_id == enemies[index].planet_id)
                    supporting_power += attackers[other].combat_power;
        }
        if (supporting_power > 0 && own_power <= supporting_power * 5L &&
                ops->random_value && ops->random_value(context, 15) == 0)
            continue;
        dx = (StarsI16)(enemies[index].point.x - self->point.x);
        dy = (StarsI16)(enemies[index].point.y - self->point.y);
        distance = (StarsU32)((StarsI32)dx * dx + (StarsI32)dy * dy);
        if (distance < best_distance) {
            best_distance = distance;
            best_id = enemies[index].id;
            best_point = enemies[index].point;
        }
    }
    if (best_id == 0xffffU || best_distance >= 0x7e90UL) {
        best_id = 0xffffU;
        best_distance = 0xffffffffUL;
        for (index = 0; index < planet_count; ++index) {
            if (planets[index].owner == owner || planets[index].has_attack_fleet)
                continue;
            dx = (StarsI16)(planets[index].point.x - self->point.x);
            dy = (StarsI16)(planets[index].point.y - self->point.y);
            distance = (StarsU32)((StarsI32)dx * dx + (StarsI32)dy * dy);
            if (distance < best_distance) {
                best_distance = distance;
                best_id = planets[index].id;
                best_point = planets[index].point;
            }
        }
    }
    if (best_id == 0xffffU)
        return -1;
    if (ops->move_target(context, best_id, best_point,
            best_distance < 0x7e90UL ? 0x0200U : 0x0100U) != 0)
        return 0;
    if (ops->retreat_to_base && own_power > 0 && best_distance >= 0x7e90UL)
        ops->retreat_to_base(context, self->id);
    return -1;
}

/* NE 19:6A7E. Convert available resources into defensive queue quantities,
 * cap them by the available global production item, and finish only when an
 * item was actually queued. */
void stars_quick_build_defenses(const StarsAiQuickDefensePlanet *planet,
    const StarsQuickDefenseOps *ops, void *context)
{
    StarsI32 weakest;
    StarsI32 available;
    StarsI32 standard;
    StarsI32 armor;
    StarsI16 index;

    if (!planet || !ops || !ops->add_queue_item || !ops->finish_production)
        return;
    if (planet->production_threshold <= 0)
        return;
    weakest = planet->resources[0] / 5;
    for (index = 1; index < 3; ++index)
        if (planet->resources[index] / 5 < weakest)
            weakest = planet->resources[index] / 5;
    available = planet->production_threshold / 10;
    if (available > 5)
        available -= available / 6;
    standard = available;
    if (standard > planet->current_mine_capacity)
        standard = planet->current_mine_capacity;
    armor = 0;
    if (weakest < standard) {
        armor = (standard - weakest) / 6;
        standard = weakest + armor;
    }
    if (standard > planet->defense_capacity)
        standard = planet->defense_capacity;
    armor *= 5;
    if (standard > 0)
        ops->add_queue_item(context, planet->id, 9, (StarsI16)standard);
    if (armor > 0)
        ops->add_queue_item(context, planet->id, 11, (StarsI16)armor);
    ops->finish_production(context, planet->id,
        (StarsI16)((standard > 0 || armor > 0) ? 1 : 0));
}

/* NE 19:76E4. Maintain the compact starbase status array, retaining the
 * newest active design in each early family and creating missing designs
 * before assigning late-family readiness states. */
void stars_ensure_macinti_starbase_designs(StarsU8 *status, StarsU16 turn,
    const StarsAiStarbaseDesign *designs, StarsU16 design_count,
    StarsU32 cost_a, StarsU32 cost_b, StarsEnsureMacintiDesign ensure_design,
    void *context)
{
    StarsU16 index;
    StarsI16 best;
    if (!status || !designs || !ensure_design || design_count < 10U)
        return;
    status[0] = 0U;
    for (index = 1; index < 4U; ++index) {
        if (designs[index].active == 0U) {
            ensure_design(context, (StarsI16)index,
                index < 3U ? (StarsI16)1 : (StarsI16)2);
            status[index] = designs[index].active ? 0U : 1U;
        } else if (turn - designs[index].age < 35U) {
            status[index] = 0U;
        } else {
            status[index] = 3U;
        }
    }
    best = -1;
    for (index = 1; index < 4U; ++index)
        if (status[index] > 1U && (best < 0 ||
                status[index] > status[(StarsU16)best] ||
                (status[index] == status[(StarsU16)best] &&
                 designs[index].age < designs[(StarsU16)best].age)))
            best = (StarsI16)index;
    for (index = 1; index < 4U; ++index)
        if (status[index] > 1U && (StarsI16)index != best)
            status[index] = 0U;
    for (index = 4; index < 10U; ++index)
        status[index] = designs[index].active ? 1U : 0U;
    if (cost_a < cost_b) {
        status[4] = 2U; status[5] = 2U; status[6] = 2U;
    } else {
        status[7] = 2U; status[8] = 2U; status[9] = 2U;
    }
}

/* NE 19:882A. Decide whether a starbase should advance or recycle and queue
 * the next design in its family when the randomized age/capacity gates pass. */
StarsI16 stars_f_upgrade_ai_starbase(StarsAiUpgradeStarbase *starbase,
    StarsI16 latest_design, StarsI16 turn, StarsU16 game_family,
    const StarsUpgradeStarbaseOps *ops, void *context)
{
    StarsI16 next;
    StarsI16 index;
    StarsU16 minimum;
    (void)turn;
    (void)game_family;
    if (!starbase || !ops || !ops->random_value || !ops->queue_upgrade ||
            latest_design < 0)
        return 0;
    if (starbase->recycle_state == 3U)
        return 0;
    if (starbase->recycle_state == 2U && ops->random_value(context, 100) > 9)
        return 0;
    minimum = starbase->family;
    if (starbase->family < 4U) {
        next = (StarsI16)(latest_design + 1);
        while (next <= 8 && starbase->recycle_state != 0U)
            ++next;
    } else {
        next = (StarsI16)(starbase->family + 3U);
        if (next > 9)
            next = (StarsI16)(starbase->family - 3U);
    }
    if (next < (StarsI16)minimum)
        next = (StarsI16)minimum;
    if (starbase->recycle_state == 0U &&
            ops->random_value(context, 100) >= 6)
        return 0;
    for (index = 0; index < 3; ++index)
        if (starbase->resource_values[index] < 200U)
            return 0;
    return ops->queue_upgrade(context, (StarsI16)(next + 16));
}

/* NE 19:8524. Queue the latest starbase design on eligible owned planets,
 * selecting the cyber-base path when requested and avoiding duplicate history
 * entries. */
void stars_queue_ai_starbase(const StarsAiQueueStarbasePlanet *planets,
    StarsU16 planet_count, StarsI16 owner, StarsI16 latest_design,
    const StarsQueueAiStarbaseOps *ops, void *context)
{
    StarsU16 index;
    StarsI16 design;
    if (!planets || !ops || !ops->is_cyber || !ops->queue_starbase ||
            latest_design < 0)
        return;
    for (index = 0; index < planet_count; ++index) {
        if (planets[index].owner != owner || planets[index].history_present)
            continue;
        if (planets[index].mining_level < 0 ||
                (planets[index].mining_level == 0 &&
                 planets[index].mineral_total <= 79U))
            continue;
        if (ops->is_cyber(context, planets[index].id) != 0)
            design = latest_design;
        else
            design = (StarsI16)(latest_design +
                (planets[index].starbase_family > 3U ? 1 : 0));
        if (ops->queue_starbase(context, planets[index].id, design) != 0)
            break;
    }
}

/* NE 19:61DE. Scouts may pursue the nearest eligible enemy when armed;
 * otherwise they select the nearest unexplored planet and publish its target
 * marker for later movement. */
StarsI16 stars_id_target_scout(const StarsAiTargetFleet *self,
    const StarsAiTargetFleet *enemies, StarsU16 enemy_count, StarsI16 only_humans,
    const StarsAiTargetPlanet *unknown, StarsU16 unknown_count,
    StarsU16 *worm_id, const StarsScoutTargetOps *ops, void *context)
{
    StarsU16 index;
    StarsU16 best_index = 0;
    StarsU16 best = 0xffffU;
    StarsU32 best_distance = 0xffffffffUL;
    StarsU32 distance;
    StarsI16 dx;
    StarsI16 dy;
    StarsU16 flags;
    if (worm_id)
        *worm_id = 0xffffU;
    if (!self || !ops || !ops->move_target)
        return -1;
    if (ops->has_teeth && ops->has_teeth(context, self) != 0) {
        for (index = 0; index < enemy_count; ++index) {
            if (only_humans && enemies[index].human == 0U)
                continue;
            dx = (StarsI16)(enemies[index].point.x - self->point.x);
            dy = (StarsI16)(enemies[index].point.y - self->point.y);
            distance = (StarsU32)((StarsI32)dx * dx + (StarsI32)dy * dy);
            if (distance < best_distance) {
                best_distance = distance;
                best = enemies[index].id;
                best_index = index;
            }
        }
        if (best != 0xffffU && best_distance < 0x7e90UL) {
            flags = 0x0200U;
            return ops->move_target(context, best, enemies[best_index].point,
                flags) != 0 ? 0 : -1;
        }
    }
    best = 0xffffU;
    best_distance = 0xffffffffUL;
    for (index = 0; index < unknown_count; ++index) {
        dx = (StarsI16)(unknown[index].point.x - self->point.x);
        dy = (StarsI16)(unknown[index].point.y - self->point.y);
        distance = (StarsU32)((StarsI32)dx * dx + (StarsI32)dy * dy);
        if (distance < best_distance) {
            best_distance = distance;
            best = index;
        }
    }
    if (best == 0xffffU)
        return -1;
    if (worm_id)
        *worm_id = unknown[best].id;
    return ops->move_target(context, unknown[best].id, unknown[best].point,
        0x0100U) != 0 ? 0 : -1;
}

/* NE 19:7DD6. Spend a sufficiently large resource reserve on a nearby
 * strategic starbase target, using reservoir sampling for equally eligible
 * enemy planets and then queueing the associated construction items. */
StarsI16 stars_f_ai_fling(const StarsAiFlingPlanet *planets, StarsU16 count,
    StarsPoint origin, StarsU16 turn, StarsI16 owner, const StarsI32 *resources,
    StarsU16 warp_range, const StarsAiFlingOps *ops, void *context)
{
    StarsU16 index;
    StarsU16 chosen = 0xffffU;
    StarsU16 found = 0;
    StarsU32 range_squared;
    StarsU32 distance;
    StarsI16 dx;
    StarsI16 dy;
    StarsI32 total;
    StarsI16 queued;
    if (!planets || !resources || !ops || !ops->random_value ||
            !ops->queue_item)
        return 0;
    total = resources[0] + resources[1] + resources[2];
    if (total < 0 || total <= 3000L)
        return 0;
    range_squared = (StarsU32)warp_range * warp_range;
    for (index = 0; index < count; ++index) {
        if (planets[index].owner < 0 || planets[index].owner == owner ||
                turn > (StarsU16)(planets[index].turn_seen + 2U))
            continue;
        if ((planets[index].flags & 0x0001U) == 0U)
            continue;
        dx = (StarsI16)(planets[index].point.x - origin.x);
        dy = (StarsI16)(planets[index].point.y - origin.y);
        distance = (StarsU32)((StarsI32)dx * dx + (StarsI32)dy * dy);
        if (distance > range_squared)
            continue;
        ++found;
        if (chosen == 0xffffU ||
                ops->random_value(context, (StarsI16)found) == 0)
            chosen = index;
    }
    if (chosen == 0xffffU)
        return 0;
    queued = ops->queue_item(context, 16, 80);
    if (queued == 0)
        return 0;
    for (index = 0; index < 3U; ++index)
        if (resources[index] > 500L)
            ops->queue_item(context, (StarsI16)(14 + index),
                (StarsI16)(resources[index] / 200L > 25L ? 25L :
                resources[index] / 200L));
    return 1;
}

/* NE 19:95A4. Run the basic AI task pipeline in its original order, allowing
 * each production action to claim a planet before the next action runs. */
void stars_handle_basic_ai_tasks(const StarsAiBasicPlanet *planets,
    StarsU16 planet_count, StarsI16 owner, StarsI16 starbase_design,
    const StarsHandleBasicOps *ops, void *context)
{
    StarsU16 index;
    StarsI16 did_work;
    if (!planets || !ops)
        return;
    if (ops->keep_fleets_moving)
        ops->keep_fleets_moving(context);
    if (ops->queue_starbases)
        ops->queue_starbases(context, starbase_design);
    for (index = 0; index < planet_count; ++index) {
        if (planets[index].owner != owner)
            continue;
        if (planets[index].research_level < 1 &&
                planets[index].mineral_total <= 39U &&
                planets[index].ai_marker == 0U)
            continue;
        did_work = 0;
        if (ops->upgrade_starbase)
            did_work = ops->upgrade_starbase(context, planets[index].id);
        if (!did_work && ops->fling)
            did_work = ops->fling(context, planets[index].id);
        if (!did_work && ops->scanner)
            did_work = ops->scanner(context, planets[index].id);
        if (!did_work && ops->defenses)
            did_work = ops->defenses(context, planets[index].id);
        if (!did_work && ops->terraforming)
            did_work = ops->terraforming(context, planets[index].id);
        if (ops->finish_production)
            ops->finish_production(context, planets[index].id, did_work);
    }
    if (ops->fix_planets_under_attack)
        ops->fix_planets_under_attack(context);
    if (ops->add_mines_blocked)
        ops->add_mines_blocked(context);
}

/* NE 19:286C. Select a useful planet for a freighter by combining cargo
 * fullness with the caller's target score, then publish a planet movement
 * order through the typed movement adapter. */
StarsI16 stars_id_target_freighter(const StarsAiTargetFleet *freighter,
    const StarsAiTargetPlanet *planets, StarsU16 planet_count, StarsU16 home_id,
    StarsI16 cargo_free, StarsI16 percent_full,
    const StarsFreighterTargetOps *ops, void *context, StarsU16 *target_id)
{
    StarsU16 index;
    StarsU16 best = 0xffffU;
    StarsI32 best_score = 0L;
    StarsI32 score;
    if (target_id)
        *target_id = 0xffffU;
    if (!freighter || !planets || !ops || !ops->score_target ||
            !ops->move_target || cargo_free < 1)
        return -1;
    for (index = 0; index < planet_count; ++index) {
        if (planets[index].id == freighter->planet_id ||
                planets[index].id == home_id || planets[index].owner < 0 ||
                planets[index].has_attack_fleet)
            continue;
        score = ops->score_target(context, &planets[index], percent_full);
        if (best == 0xffffU || score > best_score) {
            best = index;
            best_score = score;
        }
    }
    if (best == 0xffffU || best_score <= 0L)
        return -1;
    if (target_id)
        *target_id = planets[best].id;
    return ops->move_target(context, planets[best].id, planets[best].point,
        0x0100U) != 0 ? (StarsI16)planets[best].id : -1;
}

static StarsI16 ai_history_find(const StarsAiHistoryEntry *entries,
    StarsU16 count, StarsU16 planet_id)
{
    StarsU16 index;
    for (index = 0; index < count; ++index)
        if (entries[index].planet_id == planet_id)
            return (StarsI16)index;
    return -1;
}

/* NE 19:4CF0. Compact and sanitize history, add qualifying owned starbases
 * and mineral worlds, and attach each unrepresented transport to its nearest
 * available history slot. */
void stars_validate_starbase_history(StarsAiHistoryEntry *entries,
    StarsU16 *entry_count, StarsU16 turn, StarsI16 owner,
    StarsU16 capacity, const StarsAiHistoryPlanet *planets,
    StarsU16 planet_count, const StarsAiHistoryTransport *transports,
    StarsU16 transport_count, const StarsValidateHistoryOps *ops, void *context)
{
    StarsU16 read_index;
    StarsU16 write_index;
    StarsU16 index;
    StarsU16 resource;
    StarsI16 found;
    StarsU16 total;
    if (!entries || !entry_count || !planets || !ops || !ops->is_transport ||
            turn <= 19U || capacity == 0U)
        return;
    total = *entry_count;
    if (total > capacity)
        total = capacity;
    write_index = 0;
    for (read_index = 0; read_index < total; ++read_index) {
        found = -1;
        for (index = 0; index < planet_count; ++index)
            if (planets[index].id == entries[read_index].planet_id &&
                    planets[index].owner == owner) {
                found = 1;
                break;
            }
        if (found < 0)
            continue;
        if (entries[read_index].level < 0 || entries[read_index].level > 8)
            entries[read_index].level = 0;
        if (write_index != read_index)
            entries[write_index] = entries[read_index];
        ++write_index;
    }
    total = write_index;
    for (index = 0; index < planet_count && total < capacity; ++index) {
        if (planets[index].owner != owner)
            continue;
        if (!planets[index].active_starbase && planets[index].population < 20U)
            continue;
        if (ai_history_find(entries, total, planets[index].id) >= 0)
            continue;
        for (resource = 0; resource < 3U; ++resource)
            if (planets[index].concentrations[resource] *
                    planets[index].concentrations[resource] * 4U +
                    planets[index].stored[resource] > 6999U)
                break;
        if (resource == 3U || planets[index].active_starbase) {
            entries[total].planet_id = planets[index].id;
            entries[total].level = 0;
            entries[total].fleet_count = 0;
            ++total;
        }
    }
    for (index = 0; index < transport_count; ++index) {
        if (!ops->is_transport(context, transports[index].id))
            continue;
        found = -1;
        for (write_index = 0; write_index < total; ++write_index)
            if (entries[write_index].planet_id == transports[index].destination &&
                    entries[write_index].fleet_count < 8U) {
                found = (StarsI16)write_index;
                break;
            }
        if (found >= 0) {
            entries[(StarsU16)found].fleets[
                entries[(StarsU16)found].fleet_count++] = transports[index].id;
        }
    }
    *entry_count = total;
}

/* NE 19:1D9E. Choose a minefield-safe fuel warp, or a threat-dependent
 * speed, and publish it only when it differs from the current order. */
void stars_set_ai_fleet_ideal_speed(const StarsU8 *fleet,
    StarsI16 fuel_max, StarsI16 minefield_count,
    const StarsU8 *const *minefields, StarsU16 minefield_capacity,
    StarsI16 player, const StarsSetAiFleetIdealSpeedOps *ops, void *context)
{
    const StarsU8 *field = 0;
    StarsMoveOrder order;
    StarsI16 speed;
    StarsI16 random_number;
    StarsI16 race_stat;
    StarsU16 index;
    StarsU16 flags;

    (void)fuel_max;
    if (!fleet || !ops || !ops->set_speed)
        return;
    if (minefield_count != 0 && ops->fleet_in_field) {
        if (minefield_count < 0) {
            for (index = 0; index < minefield_capacity; ++index) {
                field = minefields ? minefields[index] : 0;
                if (!field)
                    break;
                flags = (StarsU16)(field[0] |
                    ((StarsU16)field[1] << 8));
                if (((flags >> 13) & 7U) == 0U &&
                        ((flags >> 9) & 0x0fU) != (StarsU16)player &&
                        field[12] != 2U &&
                        ops->fleet_in_field(context, fleet, field) != 0) {
                    break;
                }
                field = 0;
            }
        } else {
            for (index = 0; index < (StarsU16)minefield_count &&
                    index < minefield_capacity; ++index) {
                field = minefields ? minefields[index] : 0;
                if (field && ops->fleet_in_field(context, fleet, field) != 0)
                    break;
                field = 0;
            }
        }
    }
    if (field) {
        if (field[12] == 1U)
            speed = 6;
        else if (field[12] == 2U) {
            speed = 5;
            random_number = ops->random_value ?
                ops->random_value(context, 10) : 0;
            if (random_number > 6)
                speed = 6;
        } else {
            speed = 4;
            random_number = ops->random_value ?
                ops->random_value(context, 10) : 0;
            if (random_number > 3)
                speed = 5;
        }
        race_stat = ops->race_stat ? ops->race_stat(context, player, 0x0e) : 0;
        if (race_stat == 1)
            ++speed;
    } else {
        memset(&order, 0, sizeof(order));
        speed = ops->best_warp ?
            (StarsI16)ops->best_warp(context, fleet, &order) : 0;
    }
    if (!ops->current_speed ||
            speed != ops->current_speed(context, fleet))
            ops->set_speed(context, fleet, (StarsU16)speed);
}

/* NE 19:9D18. Find the nearest friendly fleet carrying a hull in the
 * requested design range, then join it when the strict or probabilistic
 * distance gate succeeds. */
StarsI16 stars_f_find_buddy_and_join_up(const StarsU8 *fleet,
    StarsI16 design_low, StarsI16 design_high, StarsI32 max_distance,
    StarsI32 fallback_distance, const StarsU8 *const *candidates,
    const StarsPoint *candidate_points, StarsU16 candidate_count,
    StarsI16 player, const StarsFindBuddyOps *ops, void *context)
{
    const StarsU8 *best = 0;
    StarsU32 best_distance = 9999999UL;
    StarsU32 distance;
    StarsU32 limit;
    StarsU16 index;
    StarsU16 best_index = 0;
    StarsI16 design;
    StarsMoveOrder order;
    StarsPoint candidate_point;

    if (!fleet || !candidates || !candidate_points || !ops ||
            !ops->distance || !ops->clear_task || !ops->move_order)
        return 0;
    for (index = 0; index < candidate_count; ++index) {
        if (!candidates[index])
            break;
        if ((StarsI16)(candidates[index][2] |
                ((StarsU16)candidates[index][3] << 8)) != player ||
                candidates[index] == fleet)
            continue;
        for (design = design_low; design <= design_high; ++design) {
            if (design >= 0 && design < 16 &&
                    (StarsI16)(candidates[index][12U +
                    (StarsU16)design * 2U] |
                    ((StarsU16)candidates[index][13U +
                    (StarsU16)design * 2U] << 8)) != 0)
                break;
        }
        if (design > design_high)
            continue;
        distance = ops->distance(context, fleet, candidates[index]);
        if (distance < best_distance) {
            best = candidates[index];
            best_index = index;
            best_distance = distance;
        }
    }
    if (!best)
        return 0;
    limit = (StarsU32)((StarsI32)max_distance *
        (StarsI32)max_distance);
    if (best_distance > limit) {
        limit = (StarsU32)((StarsI32)fallback_distance *
            (StarsI32)fallback_distance);
        if (best_distance > limit || !ops->random_value ||
                ops->random_value(context, 2) == 0)
            return 0;
    }
    candidate_point = candidate_points[best_index];
    memset(&order, 0, sizeof(order));
    order.point = candidate_point;
    order.id = (StarsI16)(best[0] | ((StarsU16)best[1] << 8));
    order.flags = 0x1260U;
    ops->clear_task(context, fleet, 1);
    ops->move_order(context, fleet, &order, 0);
    return 1;
}

/* NE 19:476A. Count friendly fleets that contain an available colonizer
 * design, then apply the map-capacity and near-term production gates. */
StarsI16 stars_f_should_we_build_colonizers(StarsI16 player, StarsI16 turn,
    StarsI16 mode, StarsI16 map_size, StarsI16 plan_max,
    const StarsU8 *const *fleets, StarsU16 fleet_count,
    const StarsU8 *design_available, const StarsU16 *hull_types,
    const StarsU32 *built_counts, const StarsU8 *player_flags,
    const StarsI16 *player_colonies, StarsU16 player_count,
    StarsAiRandom random_value, void *context, StarsI16 *colonizer_count)
{
    StarsU8 colonizer_design[16];
    StarsI16 first_design = 16;
    StarsI16 last_design = -1;
    StarsI16 fleet_colonizers = 0;
    StarsI16 other_colonies = 0;
    StarsU32 built = 0UL;
    StarsU32 demand;
    StarsI16 design;
    StarsU16 fleet_index;
    StarsU16 index;
    StarsU16 flags;
    const StarsU8 *fleet;

    if (colonizer_count)
        *colonizer_count = 0;
    if (mode == 0 && (turn & 1) != 0)
        return 0;
    if (turn < 30)
        return 1;
    if (!fleets || !design_available || !hull_types || !built_counts)
        return 0;
    memset(colonizer_design, 0, sizeof(colonizer_design));
    for (design = 0; design < 16; ++design) {
        if (design_available[design] &&
                (hull_types[design] == 14U || hull_types[design] == 15U)) {
            colonizer_design[design] = 1;
            if (design < first_design)
                first_design = design;
            if (design > last_design)
                last_design = design;
        }
    }
    if (last_design < 0)
        return 0;
    for (fleet_index = 0; fleet_index < fleet_count; ++fleet_index) {
        fleet = fleets[fleet_index];
        if (!fleet)
            break;
        if ((StarsI16)(fleet[2] | ((StarsU16)fleet[3] << 8)) != player)
            continue;
        for (design = first_design; design <= last_design; ++design) {
            if (colonizer_design[design] &&
                    (StarsI16)(fleet[12U + (StarsU16)design * 2U] |
                    ((StarsU16)fleet[13U + (StarsU16)design * 2U] << 8)) > 0) {
                ++fleet_colonizers;
                break;
            }
        }
    }
    if (colonizer_count)
        *colonizer_count = fleet_colonizers;
    if ((StarsI16)(map_size * 20 + 10) < fleet_colonizers)
        return 0;
    if (player_flags && player_colonies) {
        for (index = 0; index < player_count; ++index) {
            flags = player_flags[index];
            if ((flags & 0x0100U) != 0U)
                other_colonies = (StarsI16)(other_colonies +
                    player_colonies[index]);
        }
    }
    if ((StarsI16)((plan_max * 4) / 5) <
            (StarsI16)(fleet_colonizers + other_colonies))
        return 0;
    for (design = first_design; design <= last_design; ++design)
        if (colonizer_design[design])
            built = (StarsU32)(built + built_counts[design]);
    demand = (StarsU32)(fleet_colonizers + other_colonies);
    if ((built >= demand && built - demand < 26UL) ||
            (fleet_colonizers < 5 && random_value &&
            random_value(context, 2) == 0))
        return 1;
    return 0;
}

/* NE 19:7014. Prefer the closest non-owned inhabited planet within the
 * supplied enemy range; otherwise ask the scan enumerator for the nearest
 * eligible object and send the fleet there. */
void stars_move_to_nearest_planet_or_enemy(const StarsU8 *fleet,
    StarsI16 current_planet, StarsPoint order_point, StarsI32 enemy_range,
    const StarsU8 *const *planets, const StarsPoint *planet_points,
    StarsU16 planet_count, StarsI16 player,
    const StarsMoveNearestPlanetOps *ops, void *context)
{
    StarsU32 best_distance = 10000000UL;
    StarsU32 distance;
    StarsU32 range;
    StarsI32 dx;
    StarsI32 dy;
    StarsI16 target = -1;
    StarsI16 found;
    StarsU16 index;
    StarsMoveOrder order;
    if (!fleet || !planets || !planet_points || !ops || !ops->move_fleet)
        return;
    for (index = 0; index < planet_count; ++index) {
        if (!planets[index])
            continue;
        if ((StarsI16)(planets[index][2] |
                ((StarsU16)planets[index][3] << 8)) == player ||
                (StarsI16)(planets[index][2] |
                ((StarsU16)planets[index][3] << 8)) < 0)
            continue;
        dx = (StarsI32)order_point.x - planet_points[index].x;
        dy = (StarsI32)order_point.y - planet_points[index].y;
        distance = (StarsU32)(dx * dx + dy * dy);
        if (distance < best_distance) {
            best_distance = distance;
            target = (StarsI16)(planets[index][0] |
                ((StarsU16)planets[index][1] << 8));
        }
    }
    range = (StarsU32)(enemy_range * enemy_range);
    if (target < 0 || best_distance > range) {
        if (!ops->find_nearest ||
                ops->find_nearest(context, order_point, &found) == 0)
            return;
        target = found;
    }
    if (target == current_planet)
        return;
    memset(&order, 0, sizeof(order));
    order.id = target;
    for (index = 0; index < planet_count; ++index) {
        if (planets[index] &&
                (StarsI16)(planets[index][0] |
                ((StarsU16)planets[index][1] << 8)) == target) {
            order.point = planet_points[index];
            break;
        }
    }
    order.flags = 0x1140U;
    ops->move_fleet(context, fleet, &order, 0);
}

/* NE 19:7222. Ensure the fixed AI starbase design families exist, then
 * rotate through the mature design slots once their forty-turn age gate has
 * elapsed and the corresponding production slots are empty. */
void stars_ensure_ai_starbase_designs(StarsI16 turn,
    const StarsU8 *capabilities, const StarsAiStarbaseDesign *designs,
    const StarsCreateAiStarbase create_starbase, void *context)
{
    static const StarsI16 even_slots[] = { 0, 2, 4, 5, 7, 9 };
    static const StarsI16 odd_slots[] = { 1, 3, 6, 8 };
    StarsI16 selected;
    StarsI16 start;
    StarsI16 slot;
    StarsU16 index;
    StarsU16 count;
    StarsU16 blocked;

    if (!capabilities || !designs || !create_starbase)
        return;
    if (capabilities[0]) {
        create_starbase(context, 2, 2);
        create_starbase(context, 4, 3);
    }
    if (capabilities[1])
        create_starbase(context, 1, 1);
    if (capabilities[2])
        create_starbase(context, 3, 2);
    if (turn <= 49)
        return;
    selected = -1;
    for (index = 0; index < sizeof(even_slots) / sizeof(even_slots[0]);
            ++index) {
        slot = even_slots[index];
        if (!designs[slot].active && (selected < 0 ||
                designs[slot].age >= designs[selected].age))
            selected = slot;
    }
    if (selected >= 0 && designs[selected].age + 40U <= (StarsU16)turn) {
        start = selected < 5 ? 5 : 0;
        blocked = 0;
        for (slot = start; slot < start + 5; slot = (StarsI16)(slot + 2))
            if (!designs[slot].active &&
                    (designs[slot].cost_a != 0UL ||
                    designs[slot].cost_b != 0UL))
                blocked = 1;
        if (!blocked) {
            create_starbase(context, start, 1);
            create_starbase(context, (StarsI16)(start + 2), 2);
            create_starbase(context, (StarsI16)(start + 4), 3);
        }
    }
    selected = -1;
    count = (StarsU16)(sizeof(odd_slots) / sizeof(odd_slots[0]));
    for (index = 0; index < count; ++index) {
        slot = odd_slots[index];
        if (!designs[slot].active && (selected < 0 ||
                designs[slot].age >= designs[selected].age))
            selected = slot;
    }
    if (selected < 0 || designs[selected].age + 40U > (StarsU16)turn)
        return;
    start = selected < 5 ? 6 : 1;
    for (slot = start; slot <= start + 2; slot = (StarsI16)(slot + 2))
        if (!designs[slot].active &&
                (designs[slot].cost_a != 0UL || designs[slot].cost_b != 0UL))
            return;
    create_starbase(context, start, 1);
    create_starbase(context, (StarsI16)(start + 2), 2);
}

/* NE 19:9B10. Select the newest active design, total its existing ships,
 * recycle empty designs past the age threshold, and flag occupied stale
 * designs for the caller's follow-up handling. */
StarsI16 stars_check_ai_shdef_status(StarsI16 design_begin,
    StarsI16 design_end, StarsU16 recycle_period, StarsI16 *latest,
    StarsU8 *old_designs, StarsI16 game_turn,
    const StarsAiShdefStatus *designs, StarsRecycleAiShdef recycle_shdef,
    void *context)
{
    StarsU32 total = 0UL;
    StarsI16 index;
    StarsU16 age;

    if (latest)
        *latest = -1;
    if (!designs)
        return 0;
    for (index = design_begin; index <= design_end; ++index) {
        if (designs[index].active)
            continue;
        total = (StarsU32)(total + designs[index].existing);
        if (!latest || *latest < 0 ||
                designs[*latest].turn < designs[index].turn)
            if (latest)
                *latest = index;
        age = (StarsU16)(game_turn - designs[index].turn);
        if (recycle_period < age) {
            if (designs[index].existing == 0UL) {
                if (recycle_shdef)
                    recycle_shdef(context, index);
            } else if (old_designs) {
                old_designs[index] = 1;
            }
        }
    }
    if (total > 32000UL)
        total = 32000UL;
    return (StarsI16)total;
}

/* NE 19:939A. Queue up to four defense units when the planet's mineral
 * reserve supports a larger operable minefield and no matching item is
 * already pending. */
StarsI16 stars_f_queue_ai_defenses(StarsI32 population,
    StarsU32 current_defenses, StarsU16 global_quantity,
    StarsI16 global_type, StarsI16 defense_type,
    StarsAiQueueHasType has_type, StarsAiQueueAddItem add_item,
    void *context)
{
    StarsU32 target_defenses;
    StarsU16 quantity;

    if (population <= 1599L)
        return 0;
    target_defenses = (StarsU32)population / 80UL;
    if (target_defenses <= current_defenses || !global_quantity ||
            global_type != defense_type ||
            (has_type && has_type(context, defense_type) != 0) || !add_item)
        return 0;
    quantity = global_quantity > 4U ? 4U : global_quantity;
    add_item(context, defense_type, (StarsI16)quantity);
    return 1;
}

/* NE 19:90D6. Select the last eligible scanner design, reject duplicate
 * pending work, verify all four resource totals, and add one scanner item. */
StarsI16 stars_f_queue_ai_scanner(const StarsAiProductionCandidate *pending,
    StarsU16 pending_count, StarsAiProductionCandidate *global_items,
    StarsU16 global_count, const StarsI32 *resources, StarsI32 *resource_cost,
    StarsAiQueueAddItem add_item, void *context)
{
    StarsI16 index;
    StarsI16 cost_index;
    const StarsAiProductionCandidate *candidate = 0;
    StarsI32 available;

    if (!global_items || !resources || !resource_cost || !add_item)
        return 0;
    for (index = 0; index < (StarsI16)pending_count; ++index)
        if (pending[index].category == 1U &&
                pending[index].subtype > 17U &&
                pending[index].subtype < 27U)
            return 0;
    for (index = (StarsI16)global_count - 1; index >= 0; --index) {
        if (global_items[index].category == 1U &&
                global_items[index].subtype > 17U &&
                global_items[index].subtype < 27U &&
                global_items[index].quantity != 0U) {
            candidate = &global_items[index];
            break;
        }
    }
    if (!candidate)
        return 0;
    for (cost_index = 0; cost_index < 4; ++cost_index) {
        available = resources[cost_index] - resource_cost[cost_index];
        if (candidate->costs[cost_index] > available)
            return 0;
    }
    add_item(context, candidate->queue_type, 1);
    for (cost_index = 0; cost_index < 4; ++cost_index)
        resource_cost[cost_index] = (StarsI32)(resource_cost[cost_index] +
            candidate->costs[cost_index]);
    global_items[index].quantity = 0;
    return 1;
}

/* NE 19:8D28. Queue the last global terraforming item, cap its quantity at
 * four, clear that global item, and add its calculated costs to the totals. */
StarsI16 stars_f_queue_ai_terraforming(StarsI16 race_mode,
    StarsI32 primary_resource, StarsU16 secondary_resource,
    const StarsAiProductionCandidate *pending, StarsU16 pending_count,
    StarsAiProductionCandidate *global_items, StarsU16 global_count,
    StarsI32 *resource_cost, StarsAiQueueAddItem add_item, void *context)
{
    StarsI16 index;
    StarsI16 cost_index;
    StarsU16 quantity;
    StarsAiProductionCandidate *candidate = 0;

    if (race_mode == 4 || primary_resource < 0 ||
            (primary_resource == 0 && secondary_resource <= 199U) ||
            !global_items || !resource_cost || !add_item)
        return 0;
    for (index = 0; index < (StarsI16)pending_count; ++index)
        if (pending[index].category == 1U &&
                pending[index].subtype == 12U)
            return 0;
    for (index = (StarsI16)global_count - 1; index >= 0; --index)
        if (global_items[index].category == 1U &&
                global_items[index].subtype == 12U &&
                global_items[index].quantity != 0U) {
            candidate = &global_items[index];
            break;
        }
    if (!candidate)
        return 0;
    quantity = candidate->quantity > 4U ? 4U : candidate->quantity;
    add_item(context, candidate->queue_type, (StarsI16)quantity);
    for (cost_index = 0; cost_index < 4; ++cost_index)
        resource_cost[cost_index] = (StarsI32)(resource_cost[cost_index] +
            candidate->costs[cost_index] * (StarsI32)quantity);
    candidate->quantity = 0;
    return 1;
}

/* NE 19:98D8. Repeatedly split a friendly non-battle fleet that contains
 * both selected and unselected hull designs; selected counts are transferred
 * to the split callback and cleared from the source fleet. */
void stars_split_out_shdefs(StarsU8 *selected_designs,
    StarsU8 *const *fleets, StarsU16 fleet_count, StarsI16 player,
    StarsU16 max_fleets, StarsSplitAiFleet split_fleet, void *context)
{
    StarsU16 selected_counts[16];
    StarsU16 fleet_index;
    StarsU16 design;
    StarsI16 fleet_player;
    StarsU16 flags;
    StarsU16 has_selected;
    StarsU16 has_unselected;
    StarsU8 *fleet;

    if (!selected_designs || !fleets || !split_fleet)
        return;
    for (fleet_index = 0; fleet_index < fleet_count; ++fleet_index) {
        fleet = fleets[fleet_index];
        if (!fleet)
            break;
        fleet_player = (StarsI16)(fleet[2] |
            ((StarsU16)fleet[3] << 8));
        flags = (StarsU16)(fleet[4] |
            ((StarsU16)fleet[5] << 8));
        if (fleet_player != player || (flags & 0x0400U) != 0U)
            continue;
        has_selected = 0;
        has_unselected = 0;
        for (design = 0; design < 16; ++design) {
            if ((StarsU16)(fleet[12U + design * 2U] |
                    ((StarsU16)fleet[13U + design * 2U] << 8)) == 0U)
                continue;
            if (selected_designs[design])
                has_selected = 1;
            else
                has_unselected = 1;
        }
        if (!has_selected || !has_unselected)
            continue;
        if (max_fleets != 0U && max_fleets <= fleet_index)
            return;
        for (design = 0; design < 16; ++design) {
            selected_counts[design] = selected_designs[design] ?
                (StarsU16)(fleet[12U + design * 2U] |
                ((StarsU16)fleet[13U + design * 2U] << 8)) : 0U;
            if (selected_counts[design]) {
                fleet[12U + design * 2U] = 0;
                fleet[13U + design * 2U] = 0;
            }
        }
        split_fleet(context, fleet, selected_counts);
        return;
    }
}

/* NE 19:0E3E. Prepare the AI planet-status map, mark already-targeted
 * planets, select the nearest colonizable unknown, and optionally divert to
 * a wormhole from an owned planet before returning the selected id. */
StarsI16 stars_id_nearest_colonizable_planet(StarsPoint source,
    StarsI16 source_planet, StarsI16 race_mode, StarsI16 turn,
    StarsI16 marked, StarsI16 player, StarsU8 *status,
    const StarsI16 *owners, const StarsPoint *points, StarsU16 planet_count,
    const StarsI16 *blocked_targets, StarsU16 blocked_count,
    const StarsNearestColonizableOps *ops, void *context,
    const StarsU8 **wormhole)
{
    StarsU32 best_distance = 99999999UL;
    StarsU32 distance;
    StarsI32 dx;
    StarsI32 dy;
    StarsI16 result = -1;
    StarsI16 value;
    StarsU16 index;

    if (wormhole)
        *wormhole = 0;
    if (!status || !owners || !points)
        return -1;
    if (!marked || race_mode == 0 || race_mode == 5) {
        for (index = 0; index < planet_count; ++index) {
            if (owners[index] == -2) {
                /* No loaded planet record: retain the mode default. */
                status[index] = (race_mode == 0 || race_mode == 5) ? 0U : 16U;
            } else if (owners[index] < 0) {
                value = (race_mode == 0 || race_mode == 5) ? 0 :
                    (ops && ops->planet_opt_value ?
                    ops->planet_opt_value(context, (StarsI16)index) : 0);
                status[index] = value >= 0 ? 0U : 8U;
            } else if (owners[index] == player) {
                status[index] = 1U;
            } else {
                status[index] = 2U;
            }
        }
        for (index = 0; index < blocked_count; ++index)
            if (blocked_targets[index] >= 0 &&
                    (StarsU16)blocked_targets[index] < planet_count)
                status[(StarsU16)blocked_targets[index]] = 4U;
    }
    for (index = 0; index < planet_count; ++index) {
        if (status[index] != 0U)
            continue;
        dx = (StarsI32)source.x - (StarsI32)points[index].x;
        dy = (StarsI32)source.y - (StarsI32)points[index].y;
        distance = (StarsU32)(dx * dx + dy * dy);
        if (distance < best_distance) {
            best_distance = distance;
            result = (StarsI16)index;
        }
    }
    if (wormhole && source_planet >= 0 &&
            (StarsU16)source_planet < planet_count && turn < 120 &&
            owners[(StarsU16)source_planet] == player && ops &&
            ops->find_wormhole) {
        *wormhole = ops->find_wormhole(context, source, best_distance);
        if (*wormhole)
            result = -1;
    }
    return result;
}

/* NE 19:395A. Rank nearby hostile freighters by useful cargo and transfer
 * cargo immediately when one is co-located with the requesting fleet. */
StarsI16 stars_f_salvage_target_freighter(const StarsAiFreighterThing *things,
    StarsU16 thing_count, StarsPoint fleet_point, StarsI16 fleet_id,
    StarsI16 needy, StarsI16 worst_resource, StarsI16 percent_full,
    StarsI32 cargo_max, StarsI32 score_best, StarsI16 best_id,
    StarsI16 *best_index, StarsTransferAiSupply transfer_supply,
    void *context)
{
    StarsI16 found = 0;
    StarsI16 index;
    StarsI16 resource;
    StarsI32 need;
    StarsI32 dx;
    StarsI32 dy;
    StarsI32 distance;
    StarsI32 scale;
    StarsI32 score;
    StarsI16 percent;
    const StarsAiFreighterThing *thing;

    if (!things || cargo_max <= 0)
        return 0;
    for (index = 0; index < (StarsI16)thing_count; ++index) {
        thing = &things[index];
        if (((thing->flags >> 13) & 7U) != 1U ||
                thing->destination_class != 0U)
            continue;
        dx = (StarsI32)fleet_point.x - thing->point.x;
        dy = (StarsI32)fleet_point.y - thing->point.y;
        if (dx == 0 && dy == 0) {
            if (needy != 0 && worst_resource >= 0 && worst_resource < 3 &&
                    thing->cargo[worst_resource] != 0 && transfer_supply)
                transfer_supply(context, thing->id, fleet_id,
                    worst_resource, thing->cargo[worst_resource]);
            if (needy != 2 && transfer_supply)
                for (resource = 0; resource < 3; ++resource)
                    if (thing->cargo[resource] != 0)
                        transfer_supply(context, thing->id, fleet_id,
                            resource, thing->cargo[resource]);
            found = 1;
            continue;
        }
        if (dx > 200 || dx < -200 || dy > 200 || dy < -200)
            continue;
        need = 0;
        if (needy == 2 && worst_resource >= 0 && worst_resource < 3)
            need = thing->cargo[worst_resource];
        else
            for (resource = 0; resource < 3; ++resource)
                if (needy == 0 || resource == worst_resource)
                    need += thing->cargo[resource];
                else
                    need += thing->cargo[resource] / 2;
        if (need <= 0)
            continue;
        percent = (StarsI16)((need * 100L) / cargo_max);
        if ((StarsI16)(100 - percent_full) < percent)
            percent = (StarsI16)(100 - percent_full);
        distance = dx * dx + dy * dy;
        scale = (StarsI32)(sqrt((double)distance) + 0.5);
        scale = (scale + 24L) / 25L;
        if (scale < 1)
            scale = 1;
        score = ((StarsI32)percent * 100L) / scale;
        if (score_best < score) {
            score_best = score;
            best_id = (StarsI16)thing->id;
            if (best_index)
                *best_index = index;
        }
    }
    (void)best_id;
    return found;
}

/* NE 19:58BE. Merge eligible friendly fleets that carry a selected hull and
 * occupy the same planet and coordinates, retaining the original mine-field
 * sentinel exclusion. */
void stars_merge_all_shdefs(StarsU16 design_mask, StarsAiShdefFleet *fleets,
    StarsU16 fleet_count, StarsI16 player, StarsMergeAiFleet merge_fleet,
    void *context)
{
    StarsU16 index;
    StarsU16 other;
    StarsU16 design;
    StarsU16 has_selected;
    if (!fleets || !merge_fleet)
        return;
    for (index = 0; index < fleet_count; ++index) {
        if (fleets[index].player != player || fleets[index].mine_value == 4000)
            continue;
        has_selected = 0;
        for (design = 0; design < 16; ++design)
            if ((design_mask & (StarsU16)(1U << design)) != 0U &&
                    fleets[index].ship_counts[design] != 0U) {
                has_selected = 1;
                break;
            }
        if (!has_selected)
            continue;
        for (other = (StarsU16)(index + 1U); other < fleet_count; ++other) {
            if (fleets[other].player == player &&
                    fleets[other].mine_value != 4000 &&
                    fleets[other].planet == fleets[index].planet &&
                    fleets[other].point.x == fleets[index].point.x &&
                    fleets[other].point.y == fleets[index].point.y) {
                merge_fleet(context, &fleets[index], &fleets[other]);
                break;
            }
        }
    }
}

/* NE 19:7BCA. Resolve the default starbase family, create its working
 * design, reduce component technology for lower levels, and publish it as
 * the corresponding AI starbase design. */
StarsI16 stars_f_create_ai_starbase(StarsI16 design_index, StarsI16 level,
    StarsI16 starbase_family, StarsI16 starbase_index, StarsI16 player_mode,
    StarsAiStarbaseWork *work, StarsCreateAiShdefWork create_shdef,
    StarsChangeAiShdefWork change_shdef,
    const StarsPickAiNameOps *name_ops, void *context)
{
    StarsAiShdefWork name_work;
    StarsU16 index;
    StarsU16 tech;

    if (!work || !create_shdef || !change_shdef || !name_ops)
        return 0;
    if (starbase_family < 0) {
        if (design_index == 1 || design_index == 3 || design_index == 6 ||
                design_index == 8) {
            starbase_family = 12;
            starbase_index = 0;
            if (level == 2)
                level = 3;
        } else {
            starbase_family = 0;
            starbase_index = 2;
        }
    }
    if (create_shdef(context, -1, starbase_index, starbase_family, work) == 0)
        return 0;
    if (level < 3) {
        for (index = 0; index < work->component_count && index < 64U;
                ++index) {
            tech = work->tech_levels[index];
            if (tech < 4U) {
                if (level < 2 && player_mode != 4 &&
                        (work->groups[index] == 0x0200U || tech > 1U)) {
                    if (tech > 0U)
                        --tech;
                }
            } else {
                tech = (StarsU16)(tech >> (3U - (StarsU16)level));
            }
            work->tech_levels[index] = tech;
        }
    }
    work->flags = (StarsU16)((work->flags & 0x83ffU) |
        (((StarsU16)(design_index + 16) & 0x001fU) << 10));
    memset(&name_work, 0, sizeof(name_work));
    name_work.hull_id = work->hull_id;
    name_work.flags = work->flags;
    name_work.bitmap_id = work->bitmap_id;
    memcpy(name_work.name, work->name, sizeof(name_work.name));
    stars_pick_ai_name_and_bitmap(&name_work, 0x432, 0x0d,
        work->bitmap_id, name_ops, context);
    work->bitmap_id = name_work.bitmap_id;
    memcpy(work->name, name_work.name, sizeof(work->name));
    change_shdef(context, (StarsI16)(design_index + 16), work);
    return 1;
}

/* NE 19:14DE. Sum each nonempty design's signed hull power times its fleet
 * count, stopping as soon as the original 32-bit accumulator turns negative. */
StarsU32 stars_ul_fleet_power(const StarsU8 *fleet,
    StarsLookupAiFleetHull lookup, void *context)
{
    StarsU32 total = 0UL;
    StarsU16 design;
    StarsI16 count;
    StarsI16 player;
    const StarsU8 *hull;
    StarsI32 contribution;

    if (!fleet || !lookup)
        return 0UL;
    player = (StarsI16)(fleet[2] | ((StarsU16)fleet[3] << 8));
    for (design = 0; design < 16U; ++design) {
        count = (StarsI16)(fleet[12U + design * 2U] |
            ((StarsU16)fleet[13U + design * 2U] << 8));
        if (count != 0) {
            hull = lookup(context, player, design);
            if (!hull)
                continue;
            contribution = (StarsI32)ai_long_at(hull, 0x87U) * count;
            total += (StarsU32)contribution;
            if ((total & 0x80000000UL) != 0UL)
                return total;
        }
    }
    return total;
}

/* NE 22:45DE. A planet is eligible when it is desirable, has enough stored
 * resources, and is not already a starbase; production initialization precedes
 * choosing the latest starbase design by mineral concentrations. */
StarsI16 stars_i_build_cyber_starbase(const StarsU8 *planet,
    StarsI16 player, StarsPlanetDesirability desirability,
    StarsChangeAiMainObject change_selection, StarsInitProduction init_production,
    StarsLatestStarbase latest_of, StarsLatestStarbase latest, void *context)
{
    StarsU16 flags;
    StarsI32 resources;
    StarsU8 production[260];
    StarsI16 value;

    if (!planet || !desirability)
        return (StarsI16)-1;
    flags = (StarsU16)(planet[4] | ((StarsU16)planet[5] << 8));
    resources = (StarsI32)((StarsU32)planet[0x28] |
        ((StarsU32)planet[0x29] << 8) | ((StarsU32)planet[0x2a] << 16) |
        ((StarsU32)planet[0x2b] << 24));
    value = desirability(context, planet, player);
    if ((flags & 0x0200U) != 0U || value < 15 || resources <= 499L)
        return (StarsI16)-1;
    if (change_selection)
        change_selection(context, 1, (StarsI16)(planet[0] |
            ((StarsU16)planet[1] << 8)));
    memset(production, 0, sizeof(production));
    if (init_production)
        init_production(context, production);
    if (planet[9] < 16U || planet[10] < 16U || planet[11] < 16U)
        return latest_of ? latest_of(context) : (StarsI16)-1;
    return latest ? latest(context) : (StarsI16)-1;
}

/* NE 19:0C78. Build a colonization order from the selected planet, encode the
 * ideal warp in its order flags, and mark the fleet when movement succeeds. */
StarsI16 stars_f_colonize_ai_fleet(StarsU8 *fleet, const StarsPoint *planets,
    StarsI16 planet, StarsChangeAiMainObject change_selection,
    StarsFindIdealWarp find_warp, StarsMoveAiFleet move_fleet, void *context)
{
    StarsMoveOrder order;
    StarsU16 warp;
    StarsU16 fleet_flags;

    if (!fleet || !planets || planet < 0 || !move_fleet)
        return 0;
    if (change_selection)
        change_selection(context, 2, (StarsI16)(fleet[0] |
            ((StarsU16)fleet[1] << 8)));
    memset(&order, 0, sizeof(order));
    order.point = planets[planet];
    order.id = planet;
    order.flags = 0x1102U;
    warp = find_warp ? find_warp(context, fleet, 1) : 0U;
    order.flags = (StarsU16)((order.flags & 0xff0fU) |
        ((warp & 0x000fU) << 4));
    if (move_fleet(context, fleet, &order, 0) == 0)
        return 0;
    fleet_flags = (StarsU16)(fleet[4] | ((StarsU16)fleet[5] << 8));
    fleet_flags = (StarsU16)((fleet_flags & 0x7fffU) | 0x8000U);
    fleet[4] = (StarsU8)fleet_flags;
    fleet[5] = (StarsU8)(fleet_flags >> 8);
    return 1;
}

/* NE 19:0D5C. Build the wormhole order from the thing's point and id; its
 * order type is zero, with only the ideal-warp nibble added afterward. */
StarsI16 stars_f_goto_wormhole_ai_fleet(StarsU8 *fleet,
    const StarsU8 *wormhole, StarsChangeAiMainObject change_selection,
    StarsFindIdealWarp find_warp, StarsMoveAiFleet move_fleet, void *context)
{
    StarsMoveOrder order;
    StarsU16 warp;
    StarsU16 fleet_flags;

    if (!fleet || !wormhole || !move_fleet)
        return 0;
    if (change_selection)
        change_selection(context, 2, (StarsI16)(fleet[0] |
            ((StarsU16)fleet[1] << 8)));
    memset(&order, 0, sizeof(order));
    order.point.x = (StarsI16)(wormhole[2] |
        ((StarsU16)wormhole[3] << 8));
    order.point.y = (StarsI16)(wormhole[4] |
        ((StarsU16)wormhole[5] << 8));
    order.id = (StarsI16)(wormhole[0] |
        ((StarsU16)wormhole[1] << 8));
    order.flags = 0x0800U;
    warp = find_warp ? find_warp(context, fleet, 1) : 0U;
    order.flags = (StarsU16)((order.flags & 0xff0fU) |
        ((warp & 0x000fU) << 4));
    if (move_fleet(context, fleet, &order, 0) == 0)
        return 0;
    fleet_flags = (StarsU16)(fleet[4] | ((StarsU16)fleet[5] << 8));
    fleet_flags = (StarsU16)((fleet_flags & 0x7fffU) | 0x8000U);
    fleet[4] = (StarsU8)fleet_flags;
    fleet[5] = (StarsU8)(fleet_flags >> 8);
    return 1;
}

/* NE 14:0000.  The original uses two four-element automatic long arrays as
 * scratch space for HandleBasicAiTasks; their contents are owned by that
 * callback and do not escape this procedure. */
void stars_do_maid_ai_turn(StarsI16 turn, void *production, StarsI16 *marked_planets,
    const StarsMaidAiOps *ops, void *context)
{
    StarsI16 ai_level;
    StarsI32 scratch_a[4];
    StarsI32 scratch_b[4];
    ai_level = (turn < 0x14) ? 0 : 0x0f;
    ai_level = (ops && ops->ensure_ai) ?
        ops->ensure_ai(context, ai_level) : ai_level;
    if (marked_planets) *marked_planets = 0;
    if (ops && ops->handle_tasks)
        ops->handle_tasks(context, ai_level, production, (StarsI16)-1,
            scratch_a, scratch_b);
    if (ops && ops->fill_queue)
        ops->fill_queue(context);
}

/* NE 14:012E.  The caller supplies the original first armada-potency byte;
 * fleet records expose the original ship-count array beginning at byte 12. */
StarsI16 stars_potent_is_war_fleet(const StarsU16 *ship_counts,
    StarsI16 potency, StarsU16 first_armada_threshold)
{
    StarsU32 strength;
    StarsI16 index;
    strength = 0;
    for (index = 11; index <= 12; ++index)
        strength += ship_counts[index];
    for (index = 9; index <= 10; ++index)
        strength += (StarsU32)ship_counts[index] * 2U;
    if (potency < 2)
        return 1;
    return strength >= (StarsU32)(first_armada_threshold & 0x00ffU);
}

static StarsU16 ai_word_at(const StarsU8 *record, StarsU16 offset)
{
    return (StarsU16)(record[offset] | ((StarsU16)record[offset + 1U] << 8));
}

/* NE 19:8458.  The original player pointer is selected by idPlayer before
 * entering this helper; the reconstructed interface receives that record. */
StarsI16 stars_ishdef_ai_sb_latest_of(const StarsU8 *record)
{
    if ((ai_word_at(record, 0x03edU) & 0x0200U) == 0U &&
        ai_word_at(record, 0x0110U) < ai_word_at(record, 0x03efU))
        return 6;
    return 1;
}

/* NE 19:84BE. */
StarsI16 stars_ishdef_ai_sb_latest(const StarsU8 *record)
{
    if ((ai_word_at(record, 0x035aU) & 0x0200U) == 0U &&
        ai_word_at(record, 0x007dU) < ai_word_at(record, 0x035cU))
        return 5;
    return 0;
}

/* NE 18:31BC.  Robotic armada potency weights designs 2..5 normally and
 * designs 6..7 twice; the caller supplies the first armada threshold byte. */
StarsI16 stars_potent_rob_war_fleet(const StarsU16 *ship_counts,
    StarsI16 potency, StarsU16 first_armada_threshold)
{
    StarsU32 strength;
    StarsI16 index;

    if (!ship_counts)
        return 0;
    strength = 0U;
    for (index = 2; index <= 5; ++index)
        strength += ship_counts[index];
    for (index = 6; index <= 7; ++index)
        strength += (StarsU32)ship_counts[index] * 2U;
    if (potency < 2)
        return 1;
    return strength >= (StarsU32)(first_armada_threshold & 0x00ffU);
}

/* NE 19:1CF6.  The field check uses strict squared-distance comparison; the
 * coordinate subtraction is the original signed 16-bit operation. */
StarsI16 stars_f_fleet_in_field(StarsPoint fleet, StarsPoint thing,
    StarsU32 radius_squared)
{
    StarsU32 distance;

    distance = stars_distance_squared(fleet.x, fleet.y, thing.x, thing.y);
    return (StarsI16)(distance < radius_squared);
}

static StarsI16 ai_special_design_allowed(StarsAiDesignSpecial special,
    void *context, StarsU16 player, StarsU16 index)
{
    return special && special(context, player, index) != 0;
}

/* NE 19:4C08.  The kind ranges identify ordinary transport hulls; kind 31
 * uses the large-weight branch and still requires a usable special stat. */
StarsI16 stars_f_is_ai_transport(const StarsI16 *ship_counts,
    const StarsI16 *hull_kinds, StarsU16 count, StarsU16 player,
    StarsGetAiDesignWeight get_weight, StarsAiDesignSpecial special,
    void *context)
{
    StarsU16 index;
    StarsI16 kind;

    if (!ship_counts || !hull_kinds)
        return 0;
    for (index = 0; index < count; ++index) {
        if (ship_counts[index] > 0) {
            kind = hull_kinds[index];
            if ((kind >= 0 && kind < 4) || (kind > 10 && kind < 14))
                return 1;
            if (kind == 31 && get_weight &&
                get_weight(context, player, index) > 499 &&
                ai_special_design_allowed(special, context, player, index))
                return 1;
        }
    }
    return 0;
}

/* NE 19:4A72.  Attack hull kinds are 6..10, with kind 5 and the light
 * 29/31 branches gated by the recovered special-stat predicate. */
StarsI16 stars_f_is_ai_attack(const StarsI16 *ship_counts,
    const StarsI16 *hull_kinds, StarsU16 count, StarsU16 player,
    StarsGetAiDesignWeight get_weight, StarsAiDesignSpecial special,
    void *context)
{
    StarsU16 index;
    StarsI16 kind;

    if (!ship_counts || !hull_kinds)
        return 0;
    for (index = 0; index < count; ++index) {
        if (ship_counts[index] > 0) {
            kind = hull_kinds[index];
            if (kind > 5 && kind < 11)
                return 1;
            if (kind == 5 &&
                ai_special_design_allowed(special, context, player, index))
                return 1;
            if ((kind == 29 || kind == 31) && get_weight &&
                get_weight(context, player, index) < 500 &&
                ai_special_design_allowed(special, context, player, index))
                return 1;
        }
    }
    return 0;
}

/* NE 19:4B9A.  The two arrays are the fleet rgcsh[16] counts and the
 * corresponding hull technology values from the original hull table. */
StarsI16 stars_is_turin_drone_ai_attack(const StarsI16 *ship_counts,
    const StarsI16 *hull_tech)
{
    StarsI16 index;
    for (index = 0; index < 16; ++index) {
        if (ship_counts[index] > 0 && hull_tech[index] >= 4 &&
            hull_tech[index] <= 10)
            return 1;
    }
    return 0;
}

/* NE 19:6152.  The fleet overlap word selects the owner hull table through
 * bits 9..12; the lookup callback supplies the corresponding hull record. */
StarsI16 stars_fleet_might_have_teeth(const StarsI16 *ship_counts,
    StarsU16 overlap_flags, StarsLookupFleetHull lookup, void *context)
{
    StarsU16 owner;
    StarsU16 index;
    StarsU16 component_count;
    const StarsHullComponent *components;
    owner = (StarsU16)((overlap_flags >> 9) & 0x0fU);
    for (index = 0; index < 16; ++index) {
        if (ship_counts[index] != 0 && lookup) {
            components = lookup(context, owner, index, &component_count);
            if (components && stars_hull_has_teeth(components,
                    component_count) != 0)
                return 1;
        }
    }
    return 0;
}

/* NE 19:6F7E.  The original builds an ORDER for the closest starbase and
 * submits it as a replacement order (fAppend == 0). */
StarsI16 stars_f_move_to_nearest_starbase(const StarsU8 *fleet,
    StarsI16 big_starbase, const StarsMoveToStarbaseOps *ops, void *context)
{
    StarsMoveOrder order;
    StarsI16 starbase;

    if (!ops || !ops->find_closest || !ops->move_fleet)
        return 0;
    starbase = ops->find_closest(context, fleet, big_starbase);
    if (starbase == (StarsI16)-1)
        return 0;
    order.point.x = 0;
    order.point.y = 0;
    order.id = starbase;
    order.flags = 0x1040U;
    if (ops->get_point)
        ops->get_point(context, starbase, &order.point);
    return ops->move_fleet(context, fleet, &order, 0);
}

/* NE 19:69E6.  The AI planet list is a far-pointer array terminated by a
 * null entry; each corresponding 16-byte AI record uses byte eight as the
 * under-attack marker. */
void stars_fix_planets_under_attack(const StarsU8 *const *planets,
    StarsU16 planet_limit, const StarsU8 *ai_planets, void *production,
    StarsQuickBuildDefenses quick_build, void *context)
{
    StarsU16 index;
    const StarsU8 *planet;
    StarsU16 id;

    if (!planets || !ai_planets || !quick_build)
        return;
    for (index = 0; index < planet_limit; ++index) {
        planet = planets[index];
        if (!planet)
            break;
        id = (StarsU16)(planet[0] | ((StarsU16)planet[1] << 8));
        if (ai_planets[(StarsU16)(id * 0x10U + 8U)] != 0U)
            quick_build(context, planet, production);
    }
}

static StarsU32 ai_dword_at(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU32)bytes[offset] |
        ((StarsU32)bytes[offset + 1U] << 8) |
        ((StarsU32)bytes[offset + 2U] << 16) |
        ((StarsU32)bytes[offset + 3U] << 24);
}

/* NE 22:3D00.  Planet and AI records remain byte records here because the
 * original resource union uses deliberately unaligned long accesses. */
StarsI16 stars_f_enum_drop_off_stage1(const StarsU8 *source,
    const StarsU8 *test, StarsU16 planet_count, StarsI16 player,
    const StarsU8 *ai_data, const StarsDropoffOps *ops, void *context)
{
    StarsU16 source_id;
    StarsU16 test_id;
    StarsU16 status_offset;
    StarsU32 status;
    StarsU32 minerals;
    StarsI32 resource_value;
    StarsPoint source_point;
    StarsPoint test_point;
    StarsU32 distance;

    if (!source || !test || !ai_data || !ops || !ops->get_point)
        return 0;
    source_id = (StarsU16)(source[0] | ((StarsU16)source[1] << 8));
    test_id = (StarsU16)(test[0] | ((StarsU16)test[1] << 8));
    status_offset = (StarsU16)(planet_count * 2U + 2U + test_id * 2U);
    status = (ai_dword_at(ai_data, status_offset) >> 3) & 3U;
    if (status == 3U || status != 0U ||
        (StarsI16)(test[2] | ((StarsU16)test[3] << 8)) != player)
        return 0;
    resource_value = (StarsI32)ai_dword_at(test, 42U);
    minerals = ai_dword_at(test, 31U);
    if (resource_value >= 1 &&
        (resource_value >= 0 || minerals >= 200U))
        return 0;
    ops->get_point(context, source_id, &source_point);
    ops->get_point(context, test_id, &test_point);
    distance = stars_distance_squared(source_point.x, source_point.y,
        test_point.x, test_point.y);
    return distance < 0x27101UL ? 1 : 0;
}

/* NE 22:3DFE.  Stage two permits status values zero through two and adds
 * 0xD2 per status level to the mineral total before applying its test. */
StarsI16 stars_f_enum_drop_off_stage2(const StarsU8 *source,
    const StarsU8 *test, StarsU16 planet_count, StarsI16 player,
    const StarsU8 *ai_data, const StarsDropoffOps *ops, void *context)
{
    StarsU16 source_id;
    StarsU16 test_id;
    StarsU16 status_offset;
    StarsU32 status;
    StarsU32 minerals;
    StarsU32 added;
    StarsU32 adjusted_minerals;
    StarsI32 resource_value;
    StarsPoint source_point;
    StarsPoint test_point;
    StarsU32 distance;

    if (!source || !test || !ai_data || !ops || !ops->get_point)
        return 0;
    source_id = (StarsU16)(source[0] | ((StarsU16)source[1] << 8));
    test_id = (StarsU16)(test[0] | ((StarsU16)test[1] << 8));
    status_offset = (StarsU16)(planet_count * 2U + 2U + test_id * 2U);
    status = (ai_dword_at(ai_data, status_offset) >> 3) & 3U;
    if (status == 3U ||
        (StarsI16)(test[2] | ((StarsU16)test[3] << 8)) != player)
        return 0;
    added = status * 0xd2UL;
    minerals = ai_dword_at(test, 31U);
    adjusted_minerals = minerals + added;
    resource_value = (StarsI32)ai_dword_at(test, 42U);
    if (adjusted_minerals < minerals)
        ++resource_value;
    if (resource_value >= 1 ||
        (resource_value >= 0 && adjusted_minerals >= 1000U))
        return 0;
    ops->get_point(context, source_id, &source_point);
    ops->get_point(context, test_id, &test_point);
    distance = stars_distance_squared(source_point.x, source_point.y,
        test_point.x, test_point.y);
    return distance <= 160000UL ? 1 : 0;
}
