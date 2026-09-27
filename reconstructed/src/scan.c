/* Genuine C reconstruction of the scan-waypoint selection helper. */
#include "core.h"

/* NE 12:0EFC.  Zoom values are passed explicitly; the original reads the
 * global iScanZoom before applying these integer scale factors. */
StarsI16 stars_pt_to_scan(StarsI16 distance, StarsI16 zoom)
{
    switch (zoom) {
    case 1: return (StarsI16)(distance * 5 >> 2);
    case 2: return (StarsI16)(distance * 3 >> 1);
    case 3: return (StarsI16)(distance << 1);
    case 4: return (StarsI16)(distance << 2);
    case -4: return (StarsI16)(distance >> 2);
    case -3: return (StarsI16)(distance * 3 >> 3);
    case -2: return (StarsI16)(distance >> 1);
    case -1: return (StarsI16)(distance * 3 >> 2);
    default: return distance;
    }
}

/* NE 12:0FC2. */
StarsI16 stars_scan_to_pt(StarsI16 distance, StarsI16 zoom)
{
    switch (zoom) {
    case 1: return (StarsI16)((distance << 2) / 5);
    case 2: return (StarsI16)((distance << 1) / 3);
    case 3: return (StarsI16)(distance >> 1);
    case 4: return (StarsI16)(distance >> 2);
    case -4: return (StarsI16)(distance << 2);
    case -3: return (StarsI16)((distance << 3) / 3);
    case -2: return (StarsI16)(distance << 1);
    case -1: return (StarsI16)((distance << 2) / 3);
    default: return distance;
    }
}

static StarsI16 scan_word_at(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsI16)(bytes[offset] | ((StarsU16)bytes[offset + 1U] << 8));
}

StarsI16 stars_set_scan_waypoint(StarsI16 new_waypoint,
    StarsI16 active_waypoint, const StarsU8 *order_records,
    StarsU16 order_count, const StarsScanWaypointOps *ops, void *context)
{
    StarsScanSelection selection;
    StarsU16 offset;
    if (new_waypoint == active_waypoint)
        return new_waypoint;
    if (!order_records || new_waypoint < 0 ||
        (StarsU16)new_waypoint >= order_count)
        return new_waypoint;
    offset = (StarsU16)new_waypoint * 0x12U;
    selection.x = scan_word_at(order_records, offset + 4U);
    selection.y = scan_word_at(order_records, offset + 6U);
    selection.grobj = 0;
    selection.grobj_full = 0;
    selection.planet = 0;
    selection.fleet = 0;
    selection.waypoint = new_waypoint;
    selection.thing = 0;
    if (ops && ops->find_nearest)
        ops->find_nearest(context, selection.x, selection.y, 4, &selection);
    selection.waypoint = new_waypoint;
    if (ops && ops->change_selection)
        ops->change_selection(context, &selection, 1);
    return new_waypoint;
}
