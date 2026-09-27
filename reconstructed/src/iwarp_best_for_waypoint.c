#include "core.h"

/* NE 12:7A18-8074. Fuel callbacks receive a trial warp for the current
 * waypoint; adapters must leave the original packed order unchanged. */
StarsI16 stars_iwarp_best_for_waypoint(StarsWarpBestState *state)
{
    StarsI16 base, ai, warp, flat, limit_ai, owner, planet, found;
    StarsI16 distance, candidate, travel;
    StarsI32 fuel, capacity;
    const StarsWarpBestOps *ops;
    if (!state || !state->ops) return 0;
    ops = state->ops;
    base = ops->ideal_warp ? ops->ideal_warp(state->context, 0) : 9;
    ai = state->ai_mode && ops->ideal_warp ?
        ops->ideal_warp(state->context, 1) : base;
    if (state->order_index < 1) return base;
    flat = (StarsI16)(state->order_kind == 2 || state->order_kind == 5 ||
        state->colony_hull != 0);
    limit_ai = (StarsI16)(!flat && state->ai_mode != 0);
    if (limit_ai) flat = 1;
    warp = base;
    if (base < 9) {
        owner = -1; planet = -1;
        found = ops->nearest_planet ? ops->nearest_planet(state->context,
            state->order_point, &owner, &planet) : 0;
        if (flat || (found && (owner == -1 || owner == state->current_player))) {
            if (limit_ai && found && owner == state->current_player &&
                    state->destination_disables_ai_limit) limit_ai = 0;
            warp = 9;
        } else {
            if (state->order_index > 1 && state->previous_warp > base &&
                    state->previous_warp < 11) warp = state->previous_warp;
            fuel = ops->fuel_to_waypoint ? ops->fuel_to_waypoint(
                state->context, state->order_warp) : 0;
            capacity = ops->fleet_stat ? ops->fleet_stat(state->context, 1) : 0;
            if (fuel >= capacity / 10L || state->fuel_on_board < capacity * 7L / 10L)
                goto optimize;
            ++warp;
        }
        while (warp > base) {
            fuel = ops->fuel_to_waypoint ?
                ops->fuel_to_waypoint(state->context, warp) : 0;
            if (fuel <= state->fuel_on_board &&
                    ((found && ops->has_cargo_hull &&
                      ops->has_cargo_hull(state->context, planet)) ||
                     fuel <= state->fuel_on_board / 2L || flat)) break;
            --warp;
        }
    }
    if (limit_ai && ai < warp) warp = ai;
optimize:
    if (warp > 1 && state->target_kind != 2) {
        distance = ops->distance_term ? ops->distance_term(state->context,
            state->previous_point, state->order_point) : 0;
        travel = (StarsI16)(((StarsI32)distance + warp * warp - 1) / (warp * warp));
        while (warp > 2) {
            candidate = (StarsI16)(warp - 1);
            if (travel != (StarsI16)(((StarsI32)distance + candidate * candidate - 1) /
                    (candidate * candidate))) break;
            warp = candidate;
        }
    }
    if (ops->can_use_gate && ops->can_use_gate(state->context,
            state->previous_point, state->order_point) == 1) warp = 11;
    if (warp > 11) warp = 9;
    return warp;
}
