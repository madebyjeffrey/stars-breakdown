#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 deletes;
    StarsI16 last_recycle;
    StarsI16 messages;
    StarsI16 last_message;
    StarsU16 last_status_word;
    StarsI16 last_message_fleet;
    StarsKillUsedWaypointFleet *fleets;
    StarsU16 fleet_count;
} Trace;

static StarsKillUsedWaypointFleet *find_fleet(void *context, StarsI16 id)
{
    Trace *trace;
    StarsU16 index;

    trace = (Trace *)context;
    for (index = 0; index < trace->fleet_count; ++index)
        if (trace->fleets[index].id == id)
            return &trace->fleets[index];
    return (StarsKillUsedWaypointFleet *)0;
}

static void delete_waypoint(void *context, StarsKillUsedWaypointFleet *fleet,
    StarsI16 index, StarsI16 recycle)
{
    Trace *trace;
    (void)fleet;
    (void)index;
    trace = (Trace *)context;
    ++trace->deletes;
    trace->last_recycle = recycle;
    if (fleet->cord == 2)
        --fleet->cord;
}

static void send_message(void *context, StarsI16 owner, StarsI16 message,
    StarsI16 fleet_id)
{
    Trace *trace;
    (void)owner;
    (void)fleet_id;
    trace = (Trace *)context;
    ++trace->messages;
    trace->last_message = message;
}

static void send_message_ex(void *context, StarsI16 owner, StarsI16 message,
    StarsU16 status_word, StarsI16 fleet_id)
{
    Trace *trace;
    (void)owner;
    trace = (Trace *)context;
    ++trace->messages;
    trace->last_message = message;
    trace->last_status_word = status_word;
    trace->last_message_fleet = fleet_id;
}

int main(void)
{
    StarsKillUsedWaypointOrder orders[16];
    StarsKillUsedWaypointFleet fleets[2];
    StarsKillUsedWaypointOps ops;
    StarsKillUsedWaypointsInput input;
    Trace trace;

    (void)memset(orders, 0, sizeof orders);
    (void)memset(fleets, 0, sizeof fleets);
    (void)memset(&ops, 0, sizeof ops);
    (void)memset(&trace, 0, sizeof trace);
    fleets[0].id = 1;
    fleets[0].owner = 3;
    fleets[0].cord = 2;
    fleets[0].status_flags = (StarsI16)0x8000;
    fleets[0].orders = orders;
    fleets[1].id = 2;
    fleets[1].x = 33;
    fleets[1].y = 44;
    trace.fleets = fleets;
    trace.fleet_count = 2;
    ops.find_fleet = find_fleet;
    ops.delete_waypoint = delete_waypoint;
    ops.send_message = send_message;
    ops.send_message_ex = send_message_ex;
    ops.context = &trace;
    input.fleets = fleets;
    input.fleet_count = 1;
    input.ops = &ops;

    stars_kill_used_waypoints(&input);
    if (trace.deletes != 1 || trace.last_recycle != 0 ||
        trace.messages != 1 || trace.last_message != 0x137 ||
        trace.last_status_word != 0x8001 || trace.last_message_fleet != 1)
        return 1;

    (void)memset(&trace, 0, sizeof trace);
    trace.fleets = fleets;
    trace.fleet_count = 2;
    fleets[0].status_flags = 0;
    fleets[0].cord = 8;
    fleets[0].x = 10;
    fleets[0].y = 20;
    fleets[0].id_planet = 42;
    orders[1].flags = 0x0200;
    orders[1].target_id = 2;
    orders[2].flags = 1;
    orders[5].x = 99;
    orders[5].y = 99;
    stars_kill_used_waypoints(&input);
    if (orders[1].x != 33 || orders[1].y != 44 ||
        trace.deletes != 0)
        return 2;

    (void)memset(&trace, 0, sizeof trace);
    trace.fleets = fleets;
    trace.fleet_count = 2;
    fleets[0].cord = 8;
    fleets[0].x = 10;
    fleets[0].y = 20;
    orders[1].flags = 0x0200;
    orders[1].target_id = 99;
    stars_kill_used_waypoints(&input);
    if ((orders[1].flags & 0x0f00U) != 0x0400U ||
        orders[1].target_id != 0 || trace.deletes != 0)
        return 5;

    (void)memset(&trace, 0, sizeof trace);
    trace.fleets = fleets;
    trace.fleet_count = 2;
    fleets[0].cord = 8;
    fleets[0].x = 10;
    fleets[0].y = 20;
    orders[1].flags = 0x2200;
    orders[1].target_id = 2;
    orders[1].x = 1;
    orders[1].y = 2;
    stars_kill_used_waypoints(&input);
    if (orders[1].x != 1 || orders[1].y != 2 || trace.deletes != 0)
        return 6;

    (void)memset(&trace, 0, sizeof trace);
    trace.fleets = fleets;
    trace.fleet_count = 2;
    fleets[0].cord = 8;
    fleets[0].x = 33;
    fleets[0].y = 44;
    orders[1].flags = 0x0203;
    orders[1].target_id = 2;
    orders[1].payload[9] = 0xa5U;
    stars_kill_used_waypoints(&input);
    if ((orders[0].flags & 0x0f00U) != 0x0100U ||
        orders[0].target_id != 42 || orders[0].payload[9] != 0xa5U ||
        trace.deletes != 1)
        return 3;

    (void)memset(&trace, 0, sizeof trace);
    trace.fleets = fleets;
    trace.fleet_count = 2;
    fleets[0].cord = 2;
    fleets[0].x = 1;
    fleets[0].y = 1;
    orders[1].flags = 0x0109;
    orders[1].x = 1;
    orders[1].y = 1;
    stars_kill_used_waypoints(&input);
    if (trace.messages != 1 || trace.last_message != 0x4e ||
        trace.deletes != 1 || fleets[0].cord != 1)
        return 4;
    puts("KILL_USED_WAYPOINTS_SOURCE=PASS");
    return 0;
}
