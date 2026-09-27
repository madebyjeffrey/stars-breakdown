#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsKillUsedWaypointFleet *fleet;
    StarsI16 calls;
    StarsI16 failures;
} Trace;

static const char *resource_text(StarsI16 message)
{
    if (message == 0x0137)
        return "Ultimate Recycling";
    if (message == 0x004e)
        return "Annual resources from all planets:  ";
    return (const char *)0;
}

static void delete_waypoint(void *context, StarsKillUsedWaypointFleet *fleet,
    StarsI16 index, StarsI16 recycle)
{
    Trace *trace = (Trace *)context;
    (void)index;
    (void)recycle;
    trace->fleet = fleet;
    if (fleet->cord > 1)
        --fleet->cord;
}

static void send_message_ex(void *context, StarsI16 owner, StarsI16 message,
    StarsU16 status_word, StarsI16 fleet_id)
{
    Trace *trace = (Trace *)context;
    const char *text = resource_text(message);
    ++trace->calls;
    if (owner != 3 || fleet_id != 1 || status_word != 0x8001U ||
        text == (const char *)0 ||
        ((message == 0x0137 && strcmp(text, "Ultimate Recycling") != 0) ||
         (message == 0x004e && strcmp(text,
             "Annual resources from all planets:  ") != 0)))
        ++trace->failures;
}

int main(void)
{
    StarsKillUsedWaypointOrder orders[16];
    StarsKillUsedWaypointFleet fleet;
    StarsKillUsedWaypointOps ops;
    StarsKillUsedWaypointsInput input;
    Trace trace;

    memset(orders, 0, sizeof orders);
    memset(&fleet, 0, sizeof fleet);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    fleet.id = 1;
    fleet.owner = 3;
    fleet.cord = 2;
    fleet.status_flags = (StarsI16)0x8000;
    fleet.orders = orders;
    ops.delete_waypoint = delete_waypoint;
    ops.send_message_ex = send_message_ex;
    ops.context = &trace;
    input.fleets = &fleet;
    input.fleet_count = 1;
    input.ops = &ops;
    stars_kill_used_waypoints(&input);
    if (trace.calls != 1 || trace.failures != 0 ||
        resource_text(0x0137) == (const char *)0)
        return 1;

    memset(&trace, 0, sizeof trace);
    fleet.status_flags = 0;
    fleet.cord = 2;
    fleet.x = 10;
    fleet.y = 20;
    orders[2].flags = 9;
    stars_kill_used_waypoints(&input);
    if (trace.calls != 1 || trace.failures != 0 ||
        resource_text(0x004e) == (const char *)0)
        return 2;

    puts("KILL_USED_WAYPOINTS_RESOURCE_CHECKS=2 FAILURES=0 RESULT=PASS");
    return 0;
}
