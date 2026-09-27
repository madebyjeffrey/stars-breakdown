#include "core.h"
#include <string.h>

static StarsI16 waypoint_same_point(const StarsPoint *a, const StarsPoint *b)
{
    return a->x == b->x && a->y == b->y;
}

static StarsU16 waypoint_payload(const StarsAddWayPointInput *input,
    const StarsWayPointScan *scan)
{
    if (scan->grobj == 1U)
        return (StarsU16)scan->idpl;
    if (scan->grobj == 2U && input->fleet_ids && scan->ifl >= 0 &&
            (StarsU16)scan->ifl < input->fleet_count)
        return input->fleet_ids[(StarsU16)scan->ifl];
    if (scan->grobj == 4U)
        return (StarsU16)scan->iwp;
    if (scan->grobj == 8U && input->thing_ids && scan->ith >= 0 &&
            (StarsU16)scan->ith < input->thing_count)
        return input->thing_ids[(StarsU16)scan->ith];
    return 0U;
}

/* NE 0C:7504-7A18.  Insert a scan target immediately after the active
 * waypoint, preserving the original 18-byte order record and its packed
 * task/warp/object fields. */
StarsI16 stars_add_waypoint(StarsAddWayPointInput *input, StarsPoint point,
    StarsWayPointScan *scan)
{
    StarsPoint saved[3];
    StarsRect bounds;
    StarsMoveOrder *inserted;
    StarsU16 dc;
    StarsU16 insert_index;
    StarsU16 i;
    StarsU16 warp;
    StarsI16 saved_count;
    StarsI16 threshold;
    StarsI16 far_target;
    StarsU16 payload;
    StarsI32 dx;
    StarsI32 dy;
    StarsI32 distance;
    StarsI32 limit;

    if (!input || !scan)
        return 0;
    if (input->order_count == 0x57U) {
        if (input->message_beep)
            input->message_beep(input->context, 0x40U);
        if (input->alert_limit)
            input->alert_limit(input->context, 0x516U, 0x10);
        return 0;
    }
    saved_count = 0;
    if ((input->scan_flags & 0x80U) != 0U) {
        saved[0] = point;
        saved[1] = scan->point;
        saved_count = 2;
        if (input->active_waypoint < (StarsI16)input->order_count - 1) {
            saved[2] = input->orders[(StarsU16)(input->active_waypoint + 1)].point;
            saved_count = 3;
        }
    }
    far_target = 0;
    threshold = input->scan_to_point ?
        input->scan_to_point(input->context, 0x14) : 0;
    dx = (StarsI32)point.x - (StarsI32)scan->point.x;
    dy = (StarsI32)point.y - (StarsI32)scan->point.y;
    distance = dx * dx + dy * dy;
    limit = (StarsI32)threshold * (StarsI32)threshold;
    if (distance > limit) {
        scan->grobj_full = 4U;
        scan->grobj = 4U;
        scan->idpl = -1;
        scan->ifl = -1;
        scan->iwp = (StarsI16)(input->active_waypoint + 1);
        scan->point = point;
        far_target = 1;
    }
    if (!input->orders || input->active_waypoint < 0 ||
            (StarsU16)input->active_waypoint >= input->order_count)
        return 0;
    insert_index = (StarsU16)(input->active_waypoint + 1);
    if (waypoint_same_point(&scan->point,
            &input->orders[(StarsU16)input->active_waypoint].point))
        return 0;
    if (insert_index < input->order_count &&
            waypoint_same_point(&scan->point,
                &input->orders[insert_index].point))
        return 0;
    dc = input->get_dc ? input->get_dc(input->context,
        input->scanner_window) : 0;
    if (input->draw_ship_path)
        input->draw_ship_path(input->context, dc, 0);
    if (input->order_count == input->order_capacity) {
        if (!input->reallocate_orders ||
                !input->reallocate_orders(input->context, &input->orders,
                    (StarsU16)(input->order_count + 3U))) {
            if (input->release_dc)
                input->release_dc(input->context, input->scanner_window, dc);
            return 0;
        }
        input->order_capacity = (StarsU16)(input->order_count + 3U);
    }
    if (insert_index < input->order_count)
        memmove(&input->orders[insert_index + 1U],
            &input->orders[insert_index],
            (size_t)(input->order_count - insert_index) *
                sizeof(StarsMoveOrder));
    inserted = &input->orders[insert_index];
    if (insert_index != 0U)
        *inserted = input->orders[insert_index - 1U];
    else
        memset(inserted, 0, sizeof(*inserted));
    inserted->point = scan->point;
    payload = waypoint_payload(input, scan);
    inserted->id = (StarsI16)payload;
    inserted->flags = (StarsU16)((inserted->flags & 0xf0ffU) |
        ((scan->grobj & 0x000fU) << 8));
    ++input->order_count;
    warp = input->warp_best ? input->warp_best(input->context,
        input->scanner_window, inserted) : 0U;
    inserted->flags = (StarsU16)((inserted->flags & 0xff0fU) |
        ((warp & 0x000fU) << 4));
    scan->grobj = 4U;
    scan->grobj_full = (StarsU16)(scan->grobj_full | 4U);
    scan->iwp = (StarsI16)insert_index;
    if (input->redraw_selection)
        input->redraw_selection(input->context, 0, 0);
    if (input->lookup_fleet)
        input->lookup_fleet(input->context);
    if (insert_index != 0U &&
            (input->orders[insert_index - 1U].flags & 0x000fU) == 6U &&
            input->orders[insert_index - 1U].reserved[0] == 5U) {
        memset(input->orders[insert_index - 1U].reserved, 0, 10U);
        input->orders[insert_index - 1U].flags = (StarsU16)
            (input->orders[insert_index - 1U].flags & 0xfff0U);
        if (input->lookup_fleet)
            input->lookup_fleet(input->context);
    }
    if (input->change_selection)
        input->change_selection(input->context, scan, 1);
    if (input->release_dc)
        input->release_dc(input->context, input->scanner_window, dc);
    if (far_target && saved_count != 0) {
        for (i = 0; i < (StarsU16)saved_count; ++i)
            if (input->logical_to_scan)
                input->logical_to_scan(input->context, &saved[i]);
        if (input->bound_points)
            input->bound_points(input->context, &bounds, saved, saved_count);
        if (input->invalidate_rect)
            input->invalidate_rect(input->context, input->scanner_window,
                &bounds, 0);
    }
    return 1;
}
