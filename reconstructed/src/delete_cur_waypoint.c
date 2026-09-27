#include <string.h>
#include "core.h"

static StarsI16 order_word(const StarsU8 *records, StarsU16 index,
    StarsU16 word)
{
    StarsU16 offset;
    offset = (StarsU16)((StarsU32)index * 18UL + (StarsU32)word);
    return (StarsI16)((StarsU16)records[offset] |
        ((StarsU16)records[offset + 1U] << 8));
}

static StarsPoint order_point(const StarsU8 *records, StarsU16 index)
{
    StarsPoint point;
    point.x = order_word(records, index, 4U);
    point.y = order_word(records, index, 6U);
    return point;
}

static void remove_order(StarsDeleteCurWayPointInput *input,
    StarsU16 index, StarsU16 count)
{
    StarsU16 move_count;
    move_count = (StarsU16)(count - index - 1U);
    memmove(input->records + (StarsU32)index * 18UL,
        input->records + (StarsU32)(index + 1U) * 18UL,
        (size_t)move_count * 18U);
}

/* Genuine C reconstruction of DeleteCurWayPoint, NE 0B:9B08-9E27. */
void stars_delete_cur_waypoint(StarsDeleteCurWayPointInput *input,
    StarsI16 backup)
{
    StarsPoint points[3];
    StarsScanSelection scan;
    StarsRect rect;
    StarsPoint point;
    StarsI16 point_count;
    StarsI16 next_active;
    StarsU16 move_count;

    if (!input || !input->records || !input->iord_mac)
        return;
    if (input->cord < 2U || input->active_waypoint == 0) {
        if (input->ops && input->ops->message_beep)
            input->ops->message_beep(input->context, 0x40U);
        return;
    }
    point_count = 0;
    if ((input->scan_flags & 0x80U) != 0U) {
        points[point_count++] = order_point(input->records,
            (StarsU16)input->active_waypoint);
        points[point_count++] = order_point(input->records,
            (StarsU16)(input->active_waypoint - 1));
        if (input->active_waypoint < (StarsI16)(input->cord - 1U))
            points[point_count++] = order_point(input->records,
                (StarsU16)(input->active_waypoint + 1));
    }
    if (input->ops && input->ops->redraw_scan_selection)
        input->ops->redraw_scan_selection(input->context, 0U, 0U);
    remove_order(input, (StarsU16)input->active_waypoint, input->cord);
    --input->cord;
    --*input->iord_mac;

    next_active = (StarsI16)(input->active_waypoint - 1);
    if (next_active < (StarsI16)(input->cord - 1U) &&
        order_word(input->records, (StarsU16)next_active, 4U) ==
            order_word(input->records, (StarsU16)input->active_waypoint, 4U) &&
        order_word(input->records, (StarsU16)next_active, 6U) ==
            order_word(input->records, (StarsU16)input->active_waypoint, 6U)) {
        move_count = (StarsU16)(input->cord - (StarsU16)next_active - 2U);
        memmove(input->records + (StarsU32)input->active_waypoint * 18UL,
            input->records + (StarsU32)(input->active_waypoint + 1) * 18UL,
            (size_t)move_count * 18U);
        input->active_waypoint = next_active;
        --input->cord;
        --*input->iord_mac;
    }
    else {
        input->active_waypoint = next_active;
    }
    if (backup == 0 && input->active_waypoint < (StarsI16)(input->cord - 1U))
        ++input->active_waypoint;
    if (input->ops && input->ops->redraw_scan_selection)
        input->ops->redraw_scan_selection(input->context, 0U, 0U);
    if (input->ops && input->ops->lookup_fleet)
        input->ops->lookup_fleet(input->context, -1, input->fleet);
    point = order_point(input->records, (StarsU16)input->active_waypoint);
    scan.x = point.x;
    scan.y = point.y;
    scan.grobj = 0;
    scan.grobj_full = 0;
    scan.planet = 0;
    scan.fleet = 0;
    scan.waypoint = 0;
    scan.thing = 0;
    if (input->ops && input->ops->find_nearest)
        input->ops->find_nearest(input->context, point, 0x8fU, &scan);
    input->active_waypoint = -2;
    if (input->ops && input->ops->change_selection)
        input->ops->change_selection(input->context, &scan, 1);
    if ((input->scan_flags & 0x80U) != 0U) {
        StarsI16 i;
        for (i = 0; i < point_count; ++i)
            if (input->ops && input->ops->logical_to_scan)
                input->ops->logical_to_scan(input->context, &points[i]);
        if (input->ops && input->ops->bound_points)
            input->ops->bound_points(input->context, &rect, points, point_count);
        if (input->ops && input->ops->invalidate_rect)
            input->ops->invalidate_rect(input->context, input->scanner_window,
                &rect, 1);
    }
}
