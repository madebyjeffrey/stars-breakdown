#include "core.h"

enum {
    CYBER_FREIGHTER_PICKUP = 0,
    CYBER_FREIGHTER_DROPOFF_STAGE1 = 1,
    CYBER_FREIGHTER_DROPOFF_STAGE2 = 2,
    CYBER_FREIGHTER_CARGO_TYPE = 2,
    CYBER_FREIGHTER_RESOURCE_TYPE = 3,
    CYBER_FREIGHTER_PLANET_STARBASE_FLAG = 9,
    CYBER_FREIGHTER_PLANET_PROGRESS_SHIFT = 1,
    CYBER_FREIGHTER_DROPOFF_PROGRESS_SHIFT = 3,
    CYBER_FREIGHTER_PROGRESS_MASK = 3,
    CYBER_FREIGHTER_ORDER_PLANET = 1
};

#define CYBER_FREIGHTER_LOW_STOCK 2001L

static void cyber_freighter_advance(StarsU8 *flags, StarsI16 shift)
{
    StarsU8 old_value;
    if (!flags)
        return;
    old_value = (StarsU8)((*flags >> shift) & CYBER_FREIGHTER_PROGRESS_MASK);
    if (old_value < CYBER_FREIGHTER_PROGRESS_MASK) {
        *flags = (StarsU8)(*flags &
            (StarsU8)~(CYBER_FREIGHTER_PROGRESS_MASK << shift));
        *flags = (StarsU8)(*flags | ((old_value + 1U) << shift));
    }
}

/* Genuine semantic reconstruction of DoCyberFreighter, NE 16:37B0-3D00.
 *
 * The Cyber freighter first determines whether the planet is a source or a
 * sink, transferring supply when the current planet is owned by the AI.  On
 * other planets it uses the fleet's available cargo and special-planet rules
 * to choose pickup versus drop-off.  Drop-off attempts are recorded in two
 * two-bit progress fields so repeated turns can fall back from stage one to
 * stage two.  Enemy-owned ordinary planets are handled by preserving the
 * current planet order and sending the fleet to a nearby starbase.  Callbacks
 * stand in for the original lookup, transfer, enumeration, and movement
 * services while the resource-independent policy remains visible in C. */
void stars_do_cyber_freighter(StarsCyberFreighterFleet *fleet,
    StarsCyberFreighterPlanState *plan_state, StarsI16 player,
    const StarsDoCyberFreighterOps *ops)
{
    StarsCyberFreighterPlanet current;
    StarsCyberFreighterPlanet destination;
    StarsI16 carrying;
    StarsI16 target_kind;
    StarsI16 is_special;
    StarsI16 has_destination;
    StarsU16 warp;
    StarsU16 order_flags;

    if (!fleet || !plan_state || !ops || !ops->lookup_planet)
        return;

    if (!ops->lookup_planet(ops->context, fleet->planet_id, &current)) {
        /* A freighter in open space heads for the nearest planet-like object
         * when the object enumerator can provide one. */
        if (!ops->find_nearest || !ops->find_nearest(ops->context,
                fleet->point, &destination))
            return;
        current = destination;
        carrying = 0;
    } else if (current.owner == player) {
        /* Owned planets alternate between taking and supplying the AI
         * freighter according to their mineral reserve. */
        if (current.mining_rate < 0 ||
                (current.mining_rate < 1 &&
                 current.mineral_stock < CYBER_FREIGHTER_LOW_STOCK)) {
            if (ops->transfer_supply)
                ops->transfer_supply(ops->context, player, current.id,
                    CYBER_FREIGHTER_CARGO_TYPE,
                    CYBER_FREIGHTER_RESOURCE_TYPE, -1000L);
            if (ops->refresh_fleet)
                ops->refresh_fleet(ops->context, fleet->id);
            carrying = 0;
        } else {
            if (ops->transfer_supply)
                ops->transfer_supply(ops->context, player, current.id,
                    CYBER_FREIGHTER_CARGO_TYPE,
                    CYBER_FREIGHTER_RESOURCE_TYPE, 1000L);
            if (ops->refresh_fleet)
                ops->refresh_fleet(ops->context, fleet->id);
            carrying = 1;
        }
    } else {
        is_special = (StarsI16)(current.owner == -1 ||
            (ops->race_stat && ops->race_stat(ops->context, current.owner) == 8) ||
            (current.status_flags & (1U << CYBER_FREIGHTER_PLANET_STARBASE_FLAG)));
        if (is_special) {
            carrying = (StarsI16)(fleet->available_cargo >= 1 ||
                fleet->cargo_by_mineral[2] != 0);
        } else {
            /* Enemy ordinary planets are not serviced directly. */
            if (ops->move_to_starbase)
                ops->move_to_starbase(ops->context, fleet);
            carrying = 0;
        }
    }

    target_kind = carrying ? CYBER_FREIGHTER_DROPOFF_STAGE1 :
        CYBER_FREIGHTER_PICKUP;
    has_destination = ops->find_target && ops->find_target(ops->context,
        &current, target_kind, &destination);
    if (carrying && !has_destination)
        has_destination = ops->find_target && ops->find_target(ops->context,
            &current, CYBER_FREIGHTER_DROPOFF_STAGE2, &destination);

    if (!has_destination) {
        if (carrying && current.owner == player) {
            if (ops->transfer_supply)
                ops->transfer_supply(ops->context, player, current.id,
                    CYBER_FREIGHTER_CARGO_TYPE,
                    CYBER_FREIGHTER_RESOURCE_TYPE, -1000L);
            if (ops->refresh_fleet)
                ops->refresh_fleet(ops->context, fleet->id);
            if ((plan_state->flags >> CYBER_FREIGHTER_PLANET_PROGRESS_SHIFT &
                    CYBER_FREIGHTER_PROGRESS_MASK) <
                    CYBER_FREIGHTER_PROGRESS_MASK)
                cyber_freighter_advance(&plan_state->flags,
                    CYBER_FREIGHTER_PLANET_PROGRESS_SHIFT);
        }
        return;
    }

    if (carrying &&
            (plan_state->flags >> CYBER_FREIGHTER_DROPOFF_PROGRESS_SHIFT &
            CYBER_FREIGHTER_PROGRESS_MASK) < CYBER_FREIGHTER_PROGRESS_MASK)
        cyber_freighter_advance(&plan_state->flags,
            CYBER_FREIGHTER_DROPOFF_PROGRESS_SHIFT);
    order_flags = (StarsU16)(0x1100U | (carrying ? 0U : 1U));
    warp = ops->ideal_warp ? ops->ideal_warp(ops->context, fleet, 1) : 0;
    if (ops->move_fleet && ops->move_fleet(ops->context, fleet,
            CYBER_FREIGHTER_ORDER_PLANET, destination.id, destination.point,
            (StarsU16)(order_flags | ((warp & 0x0fU) << 4))) != 0)
        fleet->moving_flags = (StarsU16)(fleet->moving_flags | 0x8000U);
}
