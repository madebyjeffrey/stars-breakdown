#include "core.h"

/* NE 12:8074-8175. Determine whether the nearest visible scan object is the
 * selected fleet or one of its waypoints. */
StarsI16 stars_near_a_waypoint(StarsNearWaypointInput *input)
{
    StarsPoint point;
    StarsPoint nearest_point;
    StarsU16 flags;
    StarsU16 i;
    if (!input || input->selected_kind != 2U || !input->nearest) return 0;
    point = input->query_point;
    if (!input->logical_input && input->scan_to_logical)
        input->scan_to_logical(input->context, &point);
    if (!input->nearest(input->context, point, 0x4fU, &nearest_point, &flags) ||
        (flags & 4U) == 0U) return 0;
    if (nearest_point.x != input->selected_point.x ||
        nearest_point.y != input->selected_point.y) return 1;
    if (!input->waypoints) return 0;
    for (i = 1U; i < input->selected_waypoint_count; ++i)
        if (input->waypoints[i].x == nearest_point.x &&
            input->waypoints[i].y == nearest_point.y) return 1;
    return 0;
}
