#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 moved;
    StarsI16 mines;
    StarsI16 arrived;
    StarsI16 tracked;
    StarsI16 killed_waypoints;
    StarsI16 messages;
} Trace;

static StarsI16 get_order(void *p, const StarsMoveFleet *fleet,
    StarsMoveFleetOrder *order)
{
    (void)p;
    order->kind = 2;
    order->target_kind = 1;
    order->target_id = fleet->id + 10;
    order->target_x = fleet->target_x;
    order->target_y = fleet->target_y;
    order->colonist_cargo = 0;
    return 1;
}
static StarsI32 distance_value(void *p, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{ (void)p; return (StarsI32)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); }
static StarsI32 fuel_use(void *p, const StarsMoveFleet *fleet,
    const StarsMoveFleetOrder *order, StarsI32 distance)
{ (void)p; (void)fleet; (void)order; return distance; }
static StarsI32 fuel_gain(void *p, const StarsMoveFleet *fleet, StarsI32 amount)
{ (void)p; (void)fleet; return amount > 3 ? 3 : amount; }
static void cargo(void *p, StarsMoveFleet *fleet, StarsI16 kind, StarsI32 amount)
{ Trace *t = (Trace *)p; (void)fleet; (void)kind; if (amount > 0) ++t->arrived; }
static StarsI16 mines(void *p, StarsMoveFleet *fleet, StarsI32 *movement)
{ Trace *t = (Trace *)p; (void)fleet; ++t->mines; if (*movement > 1) *movement -= 1; return 0; }
static StarsI16 nearest(void *p, StarsI16 x, StarsI16 y)
{ (void)p; (void)x; (void)y; return 77; }
static void message(void *p, StarsI16 owner, StarsI16 code, StarsI16 id,
    StarsI16 a, StarsI16 b, StarsI16 c)
{ Trace *t = (Trace *)p; (void)owner; (void)code; (void)id; (void)a; (void)b; (void)c; ++t->messages; }
static void no_track(void *p, StarsMoveFleet *fleet)
{ Trace *t = (Trace *)p; (void)fleet; ++t->tracked; }
static void kill_waypoints(void *p)
{ Trace *t = (Trace *)p; ++t->killed_waypoints; }

int main(void)
{
    StarsMoveFleet fleet;
    StarsMoveFleetsOps ops;
    StarsMoveFleetsInput input;
    Trace trace;

    memset(&fleet, 0, sizeof fleet);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleet.id = 1; fleet.active = 1; fleet.target_x = 4; fleet.target_y = 0;
    fleet.fuel = 100; fleet.fuel_capacity = 100; fleet.has_ram_scoop = 1;
    ops.context = &trace; ops.get_order = get_order;
    ops.distance = distance_value; ops.fuel_use = fuel_use;
    ops.fuel_gain = fuel_gain; ops.change_cargo = cargo;
    ops.travel_mines = mines; ops.nearest_object = nearest;
    ops.send_message = message; ops.no_auto_track = no_track;
    ops.kill_waypoints = kill_waypoints;
    input.fleets = &fleet; input.fleet_count = 1; input.ops = &ops;
    stars_move_fleets(&input);
    if (fleet.x != 4 || fleet.target_id != 11 || trace.mines == 0 ||
            trace.tracked == 0 || trace.killed_waypoints != 1) return 1;
    puts("MOVE_FLEETS_SOURCE=PASS");
    return 0;
}
