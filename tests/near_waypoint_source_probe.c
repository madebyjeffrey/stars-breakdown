#include <stdio.h>
#include "core.h"

typedef struct Trace { StarsPoint result; StarsU16 flags; StarsI16 converted; } Trace;
static StarsI16 nearest(void *context, StarsPoint query, StarsU16 kind,
    StarsPoint *result, StarsU16 *flags)
{ Trace *t=(Trace *)context; (void)query; (void)kind; *result=t->result; *flags=t->flags; return 1; }
static void logical(void *context, StarsPoint *point)
{ Trace *t=(Trace *)context; ++t->converted; point->x+=1; }
int main(void)
{
    StarsNearWaypointInput input; StarsPoint waypoints[3]; Trace trace;
    input.selected_kind=2U; input.logical_input=1; input.selected_point.x=4;
    input.selected_point.y=5; input.query_point.x=0; input.query_point.y=0;
    input.selected_waypoint_count=3U; input.waypoints=waypoints;
    input.nearest=nearest; input.scan_to_logical=logical; input.context=&trace;
    trace.result.x=4; trace.result.y=5; trace.flags=4U; trace.converted=0;
    waypoints[1].x=4; waypoints[1].y=5;
    if (!stars_near_a_waypoint(&input)) return 1;
    trace.result.x=8; if (!stars_near_a_waypoint(&input)) return 2;
    trace.flags=0; if (stars_near_a_waypoint(&input)) return 3;
    input.selected_kind=1U; if (stars_near_a_waypoint(&input)) return 4;
    input.selected_kind=2U; trace.flags=4U; input.logical_input=0;
    trace.result.x=4; trace.result.y=5; if (!stars_near_a_waypoint(&input) || trace.converted != 1) return 5;
    puts("NEAR_WAYPOINT_SOURCE=PASS");
    return 0;
}
