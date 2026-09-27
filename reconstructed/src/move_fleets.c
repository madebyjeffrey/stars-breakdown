#include "core.h"

#define MOVE_FLEET_MAX_PASSES 10
#define MOVE_FLEET_ORDER_MOVE 1
#define MOVE_FLEET_ORDER_SKIP 6
#define MOVE_FLEET_ORDER_PLANET 1
#define MOVE_FLEET_ORDER_WORMHOLE 2
#define MOVE_FLEET_STATUS_DEAD 0x0004
#define MOVE_FLEET_STATUS_MOVED 0x2000
#define MOVE_FLEET_STATUS_BLOCKED 0x0400
#define MOVE_FLEET_RAMSCOOP_STAT 0x000e

static StarsI32 fleet_abs(StarsI32 value)
{
    return value < 0 ? -value : value;
}

static StarsI32 fleet_distance(const StarsMoveFleetsOps *ops,
    const StarsMoveFleet *fleet, const StarsMoveFleetOrder *order)
{
    if (ops->distance)
        return ops->distance(ops->context, fleet->x, fleet->y,
            order->target_x, order->target_y);
    return fleet_abs((StarsI32)order->target_x - fleet->x) +
        fleet_abs((StarsI32)order->target_y - fleet->y);
}

static StarsI32 fleet_movement_limit(const StarsMoveFleetsOps *ops,
    const StarsMoveFleet *fleet, const StarsMoveFleetOrder *order,
    StarsI32 distance, StarsI16 pass)
{
    StarsI32 limit;

    if (ops->movement_limit)
        limit = ops->movement_limit(ops->context, fleet, order, distance,
            pass);
    else if (fleet->warp_level > 0)
        limit = (StarsI32)fleet->warp_level * fleet->warp_level;
    else
        limit = distance;
    return limit < 0 ? 0 : limit;
}

static void fleet_message(const StarsMoveFleetsOps *ops, const StarsMoveFleet *fleet,
    StarsI16 code, StarsI16 a, StarsI16 b, StarsI16 c)
{
    if (ops->send_message)
        ops->send_message(ops->context, fleet->owner, code, fleet->id, a, b, c);
}

static StarsI32 fleet_use_fuel(const StarsMoveFleetsOps *ops,
    const StarsMoveFleet *fleet, const StarsMoveFleetOrder *order,
    StarsI32 distance)
{
    if (ops->fuel_use)
        return ops->fuel_use(ops->context, fleet, order, distance);
    return distance;
}

static void fleet_set_position(StarsMoveFleet *fleet,
    const StarsMoveFleetOrder *order, StarsI32 amount, StarsI32 total)
{
    StarsI32 dx;
    StarsI32 dy;

    if (amount >= total || total <= 0) {
        fleet->x = order->target_x;
        fleet->y = order->target_y;
        fleet->target_id = order->target_kind == MOVE_FLEET_ORDER_PLANET ?
            order->target_id : -1;
        fleet->moving = 0;
        return;
    }
    dx = (StarsI32)order->target_x - fleet->x;
    dy = (StarsI32)order->target_y - fleet->y;
    fleet->x = (StarsI16)(fleet->x + (dx * amount) / total);
    fleet->y = (StarsI16)(fleet->y + (dy * amount) / total);
    fleet->target_id = -1;
    fleet->moving = 1;
}

static StarsI32 fleet_available_movement(const StarsMoveFleetsOps *ops,
    StarsMoveFleet *fleet, const StarsMoveFleetOrder *order,
    StarsI32 distance, StarsI32 movement_limit, StarsI32 fuel,
    StarsI16 pass)
{
    StarsI32 movement;
    StarsI32 gain;

    (void)order;
    if (fuel < 0) fuel = 0;
    if (pass > 0 && fleet->move_left > 0)
        movement = fleet->move_left;
    else
        movement = distance;
    if (movement > movement_limit)
        movement = movement_limit;
    if (fuel > 0 && fleet->fuel < fuel)
        return 0;
    if (fleet->has_ram_scoop && pass == 0 && ops->fuel_gain) {
        gain = ops->fuel_gain(ops->context, fleet, movement);
        if (gain > 0) {
            if (ops->change_cargo)
                ops->change_cargo(ops->context, fleet, 2, gain);
            fleet_message(ops, fleet, 0xf3,
                (StarsI16)(gain > 32767L ? 32767 : gain), 0, 0);
        }
    }
    if (ops->travel_mines && movement > 0 &&
        ops->travel_mines(ops->context, fleet, &movement))
        fleet->status |= MOVE_FLEET_STATUS_BLOCKED;
    if (movement < 0) movement = 0;
    if (movement > distance) movement = distance;
    if (fuel > 0 && fleet->fuel >= fuel)
        fleet->fuel -= fuel;
    else if (fuel > 0)
        fleet->fuel = 0;
    return movement;
}

static void fleet_arrival(const StarsMoveFleetsOps *ops,
    StarsMoveFleet *fleet, const StarsMoveFleetOrder *order,
    StarsI32 distance, StarsI16 pass)
{
    StarsI16 gate;

    if (order->target_kind == MOVE_FLEET_ORDER_WORMHOLE &&
        ops->can_jumpgate && ops->can_jumpgate(ops->context, fleet)) {
        gate = order->target_id;
        if (ops->stargate_jump && ops->stargate_jump(ops->context,
                fleet, gate, order->target_id, distance)) {
            fleet->status &= (StarsI16)~MOVE_FLEET_STATUS_MOVED;
            if (ops->no_auto_track)
                ops->no_auto_track(ops->context, fleet);
            return;
        }
    }
    fleet_set_position(fleet, order, distance, distance);
    if (ops->nearest_object && fleet->target_id < 0)
        fleet->target_id = ops->nearest_object(ops->context, fleet->x, fleet->y);
    if (ops->change_cargo && order->colonist_cargo != 0)
        ops->change_cargo(ops->context, fleet, 1, order->colonist_cargo);
    if (ops->transfer_cargo && pass > 0)
        ops->transfer_cargo(ops->context, fleet, fleet);
    if (ops->no_auto_track)
        ops->no_auto_track(ops->context, fleet);
}

static StarsI16 move_one_fleet(const StarsMoveFleetsOps *ops,
    StarsMoveFleet *fleet, StarsI16 pass)
{
    StarsMoveFleetOrder order;
    StarsI32 distance;
    StarsI32 movement_limit;
    StarsI32 travel;
    StarsI32 movement;
    StarsI32 fuel;
    StarsI16 race_value;

    if (!fleet->active || (fleet->status & MOVE_FLEET_STATUS_BLOCKED) != 0)
        return 0;
    if (!ops->get_order || !ops->get_order(ops->context, fleet, &order))
        return 0;
    if (order.kind == MOVE_FLEET_ORDER_MOVE || order.kind == MOVE_FLEET_ORDER_SKIP)
        return 0;
    if ((fleet->status & MOVE_FLEET_STATUS_DEAD) != 0 &&
        ops->random_value && ops->random_value(ops->context, 4) == 0) {
        fleet_message(ops, fleet, 0x102, -5, 0, 0);
        return 0;
    }
    if (pass == 0 && ops->race_grbit &&
        ops->race_grbit(ops->context, fleet->owner, 8) != 0 &&
        ops->random_value && ops->random_value(ops->context, 10) == 0) {
        fleet_message(ops, fleet, 0xf2, 0, 0, 0);
        return 0;
    }
    distance = fleet_distance(ops, fleet, &order);
    if (distance <= 0) {
        fleet_arrival(ops, fleet, &order, 0, pass);
        return 1;
    }
    movement_limit = fleet_movement_limit(ops, fleet, &order, distance, pass);
    travel = pass > 0 && fleet->move_left > 0 ? fleet->move_left : distance;
    if (travel > movement_limit)
        travel = movement_limit;
    if (travel <= 0)
        return 0;
    fuel = fleet_use_fuel(ops, fleet, &order, travel);
    if (fuel <= 0) fuel = travel;
    movement = fleet_available_movement(ops, fleet, &order, distance,
        movement_limit, fuel, pass);
    if (movement <= 0) {
        fleet_message(ops, fleet, 0xde, order.target_x, order.target_y, 0);
        return 0;
    }
    if (ops->race_stat && fleet->has_ram_scoop)
        race_value = ops->race_stat(ops->context, fleet->owner,
            MOVE_FLEET_RAMSCOOP_STAT);
    else
        race_value = 0;
    if (race_value < 0) race_value = 0;
    fleet_set_position(fleet, &order, movement, distance);
    fleet->move_left = distance - movement;
    fleet->status |= MOVE_FLEET_STATUS_MOVED;
    if (movement >= distance) {
        fleet_arrival(ops, fleet, &order, distance, pass);
        fleet->move_left = 0;
    } else if (fuel > fleet->fuel_capacity && fleet->fuel_capacity > 0) {
        fleet_message(ops, fleet, 0x27, fleet->warp_level, 0, 0);
    }
    return 1;
}

void stars_move_fleets(StarsMoveFleetsInput *input)
{
    const StarsMoveFleetsOps *ops;
    StarsI16 pass;
    StarsI16 index;
    StarsI16 moved;

    if (!input || !input->ops || !input->fleets)
        return;
    ops = input->ops;
    for (pass = 0; pass <= MOVE_FLEET_MAX_PASSES; ++pass) {
        moved = 0;
        for (index = 0; index < (StarsI16)input->fleet_count; ++index) {
            if (move_one_fleet(ops, &input->fleets[index], pass))
                moved = 1;
        }
        if (!moved)
            break;
    }
    if (ops->kill_waypoints)
        ops->kill_waypoints(ops->context);
}
