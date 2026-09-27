#include "core.h"
#include <string.h>

#define TURIN_SCOUT_ORDER_TARGET_PLANET 0x0100U
#define TURIN_SCOUT_ORDER_MOVE 0x1000U
#define TURIN_SCOUT_STATUS_UNKNOWN 0x10U

StarsI16 stars_turin_plan_scout_planet(void *opaque,
    const StarsTurinTurnFleet *fleet, const StarsTurinFleetPolicy *policy,
    StarsI16 turn, StarsTurinFleetPlan *plan)
{
    StarsTurinScoutAdapter *adapter;
    const StarsU8 *wormhole;
    StarsPoint source;
    StarsI16 target;
    StarsI16 warp;

    (void)policy;
    (void)turn;
    adapter = (StarsTurinScoutAdapter *)opaque;
    if (!adapter || !fleet || !plan)
        return 0;
    plan->action = STARS_TURIN_FLEET_ACTION_NONE;
    plan->target_planet = -1;
    plan->order_kind = 0;
    plan->append = 0;
    plan->reason = 0;
    if (adapter->enemy_candidate_count != 0U)
        return 1;
    if (!adapter->fleet_has_teeth ||
            !adapter->fleet_has_teeth(adapter->context, fleet))
        return 1;
    if (!adapter->planet_points || !adapter->ai_planet_records ||
            !adapter->fleet_point || !adapter->ideal_warp ||
            !adapter->random_value || adapter->planet_count == 0U ||
            adapter->ai_planet_record_stride == 0U ||
            adapter->unknown_status_offset >=
                adapter->ai_planet_record_stride ||
            (StarsU32)adapter->planet_count *
                adapter->ai_planet_record_stride >
                adapter->ai_planet_records_size ||
            !adapter->status_ready)
        return 0;
    if (!adapter->fleet_point(adapter->context, fleet, &source))
        return 0;
    wormhole = 0;
    target = stars_id_nearest_unknown_planet_strided(source,
        adapter->ai_planet_records, adapter->ai_planet_record_stride,
        adapter->unknown_status_offset, adapter->planet_points,
        adapter->planet_count, (StarsI16)(fleet->planet_id >= 0),
        adapter->status_ready, 0, adapter->find_wormhole,
        adapter->random_value, adapter->context, &wormhole);
    if (wormhole)
        return 0;
    if (target < 0 || (StarsU16)target >= adapter->planet_count ||
            target == fleet->planet_id)
        return 1;
    warp = adapter->ideal_warp(adapter->context, fleet, target,
        adapter->planet_points[(StarsU16)target]);
    if (warp < 0 || warp > 15)
        return 0;
    plan->action = STARS_TURIN_FLEET_ACTION_MOVE;
    plan->target_planet = target;
    plan->order_kind = (StarsI16)(TURIN_SCOUT_ORDER_MOVE |
        TURIN_SCOUT_ORDER_TARGET_PLANET | ((StarsU16)warp << 4));
    plan->append = 0;
    plan->reason = 1;
    return 1;
}

StarsI16 stars_turin_apply_scout_planet(void *opaque,
    const StarsTurinTurnFleet *fleet, const StarsTurinFleetPlan *plan)
{
    const StarsTurinScoutAdapter *adapter;
    StarsMoveOrder order;

    adapter = (const StarsTurinScoutAdapter *)opaque;
    if (!adapter || !fleet || !plan ||
            plan->action != STARS_TURIN_FLEET_ACTION_MOVE ||
            plan->target_planet < 0 ||
            (StarsU16)plan->target_planet >= adapter->planet_count ||
            !adapter->planet_points || !fleet->raw_plord ||
            adapter->order_capacity == 0U)
        return 0;
    memset(&order, 0, sizeof order);
    order.point = adapter->planet_points[(StarsU16)plan->target_planet];
    order.id = plan->target_planet;
    order.flags = (StarsU16)plan->order_kind;
    return stars_ai_move_order_to_plord(fleet->raw_plord,
        adapter->order_capacity, &order, plan->append);
}
