#include "core.h"

#define MAC_FREIGHTER_NO_PLANET (-1)
#define MAC_FREIGHTER_RANGE 0x9c41UL
#define MAC_FREIGHTER_TRANSFER_DIVISOR 20L
#define MAC_FREIGHTER_DEFICIT_DIVISOR 5L
#define MAC_FREIGHTER_CAPACITY_GATE 25
#define MAC_FREIGHTER_MIN_STOCK 1000L
#define MAC_FREIGHTER_MIN_SOURCE_STOCK 2500L
#define MAC_FREIGHTER_MIN_DEFICIT_STOCK 200L
#define MAC_FREIGHTER_MAX_DISTANCE_PENALTY 6
#define MAC_FREIGHTER_MIN_GAIN 5L
#define MAC_FREIGHTER_STRONG_GAIN 10L
#define MAC_FREIGHTER_LATE_GAIN 15L
#define MAC_FREIGHTER_EARLY_TURN 81U
#define MAC_FREIGHTER_LATE_TURN 161U
#define MAC_FREIGHTER_ORDER_FLAGS 0x1141U

static StarsU32 mac_freighter_distance(StarsPoint a, StarsPoint b)
{
    StarsI32 dx = (StarsI32)a.x - b.x;
    StarsI32 dy = (StarsI32)a.y - b.y;
    return (StarsU32)(dx * dx + dy * dy);
}

static StarsI32 mac_freighter_available(const StarsMacFreighterFleet *fleet,
    StarsI32 source_stock, StarsI32 divisor)
{
    StarsI32 available = fleet->fleet_capacity;
    StarsI16 index;
    for (index = 0; index < 3; ++index)
        available -= fleet->cargo[index];
    if (available > source_stock / divisor)
        available = source_stock / divisor;
    if (available < 0)
        return 0;
    return available;
}

static StarsI16 mac_freighter_loss_percent(StarsU32 distance)
{
    if (distance < 0x9c5UL)
        return 3;
    if (distance < 0x2711UL)
        return 6;
    if (distance < 0x57e5UL)
        return 9;
    return MAC_FREIGHTER_MAX_DISTANCE_PENALTY * 2;
}

/* Genuine semantic reconstruction of IdTargetMacFreighter, NE 21:3524-3E39.
 *
 * A Macinti freighter first attempts to export a fraction of the current
 * planet's surplus to the nearby AI planet that benefits most after distance
 * loss.  If that fails, it identifies the first mineral in deficit and looks
 * for the nearby planet with the lowest concentration of that mineral.  As a
 * final fallback it scores nearby planets by capacity, reducing the score for
 * longer trips, and emits a resource-delivery order.  The resource callback
 * represents CResourcesAtPlanet and the transfer/move callbacks represent the
 * original order-writing services. */
StarsI16 stars_id_target_mac_freighter(StarsMacFreighterFleet *fleet,
    StarsI16 player, StarsU16 game_turn, const StarsMacFreighterPlanet *planets,
    StarsU16 planet_count, const StarsIdTargetMacFreighterOps *ops)
{
    const StarsMacFreighterPlanet *source = (const StarsMacFreighterPlanet *)0;
    const StarsMacFreighterPlanet *best = (const StarsMacFreighterPlanet *)0;
    StarsI32 transfer = 0;
    StarsI32 best_gain = 0;
    StarsI32 source_before;
    StarsI32 source_after;
    StarsI32 target_before;
    StarsI32 target_after;
    StarsI32 gain;
    StarsI32 available;
    StarsI16 mineral = -1;
    StarsI16 index;
    StarsU32 distance;
    StarsI16 penalty;
    StarsI16 best_capacity = -32768;

    if (!fleet || !planets || !ops || !ops->move)
        return -1;
    if (fleet->planet_id != MAC_FREIGHTER_NO_PLANET) {
        for (index = 0; (StarsU16)index < planet_count; ++index)
            if (planets[index].id == (StarsU16)fleet->planet_id) {
                source = &planets[index];
                break;
            }
    }

    /* Phase one: ship a twentieth of a capacious source reserve to the
     * closest useful AI planet.  Resource callbacks are sampled before and
     * after a hypothetical transfer, matching the original benefit test. */
    if (source && source->owner == player &&
            source->capacity_percent > MAC_FREIGHTER_CAPACITY_GATE &&
            source->minerals[2] > MAC_FREIGHTER_MIN_STOCK - 1L) {
        transfer = mac_freighter_available(fleet, source->minerals[2],
            MAC_FREIGHTER_TRANSFER_DIVISOR);
        if (transfer > 0) {
            source_before = ops->resources ? ops->resources(ops->context,
                source) : 0;
            for (index = 0; (StarsU16)index < planet_count; ++index) {
                if (&planets[index] == source || planets[index].ai_marker & 1U)
                    continue;
                distance = mac_freighter_distance(fleet->point,
                    planets[index].point);
                if (distance >= MAC_FREIGHTER_RANGE)
                    continue;
                penalty = mac_freighter_loss_percent(distance);
                target_before = ops->resources ? ops->resources(ops->context,
                    &planets[index]) : 0;
                target_after = target_before + transfer -
                    transfer * penalty / 100L;
                source_after = source_before;
                gain = (target_after - target_before) -
                    (source_before - source_after);
                if (!best || gain > best_gain) {
                    best = &planets[index];
                    best_gain = gain;
                }
            }
            if (best && best_gain >= MAC_FREIGHTER_MIN_GAIN &&
                    (best_gain >= MAC_FREIGHTER_STRONG_GAIN ||
                     game_turn < MAC_FREIGHTER_EARLY_TURN) &&
                    (best_gain >= MAC_FREIGHTER_LATE_GAIN ||
                     game_turn < MAC_FREIGHTER_LATE_TURN)) {
                if (ops->transfer_supply)
                    ops->transfer_supply(ops->context, source->id, player,
                        2, transfer);
                return ops->move(ops->context, fleet, 1U, best->id,
                    best->point) ? best->id : -1;
            }
        }
    }

    /* Phase two: find a mineral whose source planet is below a useful stock
     * level, then choose the nearby planet with the smallest concentration. */
    if (source) {
        for (index = 0; index < 3; ++index)
            if (source->minerals[index] > MAC_FREIGHTER_MIN_SOURCE_STOCK - 1L) {
                mineral = index;
                break;
            }
        if (mineral >= 0) {
            StarsI32 lowest = 0x7fffffffL;
            for (index = 0; (StarsU16)index < planet_count; ++index) {
                if (&planets[index] == source)
                    continue;
                distance = mac_freighter_distance(fleet->point,
                    planets[index].point);
                if (distance < MAC_FREIGHTER_RANGE &&
                        planets[index].minerals[mineral] < lowest) {
                    lowest = planets[index].minerals[mineral];
                    best = &planets[index];
                }
            }
            if (best && best->minerals[mineral] < 200L) {
                available = mac_freighter_available(fleet,
                    source->minerals[mineral], MAC_FREIGHTER_DEFICIT_DIVISOR);
                if (available > 0 && ops->transfer_supply)
                    ops->transfer_supply(ops->context, source->id, player,
                        mineral, available);
                if (available > 0)
                    return ops->move(ops->context, fleet, 1U, best->id,
                        best->point) ? best->id : -1;
            }
        }
    }

    /* Phase three: capacity is the fallback score, with distance penalties
     * of two, four, or six percentage points for progressively longer trips. */
    best = (const StarsMacFreighterPlanet *)0;
    for (index = 0; (StarsU16)index < planet_count; ++index) {
        if (&planets[index] == source)
            continue;
        distance = mac_freighter_distance(fleet->point, planets[index].point);
        if (distance >= MAC_FREIGHTER_RANGE)
            continue;
        penalty = distance < 0x2711UL ? (distance < 0x9c5UL ? 0 : 2) :
            (distance < 0x57e5UL ? 4 : 6);
        if (planets[index].capacity_percent - penalty > best_capacity) {
            best_capacity = (StarsI16)(planets[index].capacity_percent - penalty);
            best = &planets[index];
        }
    }
    if (!best)
        return -1;
    return ops->move(ops->context, fleet, 1U, best->id, best->point) ?
        best->id : -1;
}
