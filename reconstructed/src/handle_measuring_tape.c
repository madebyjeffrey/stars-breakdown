#include "core.h"
#include <stdlib.h>

static StarsI16 measure_clamp(StarsI16 value, StarsI16 maximum)
{
    if (value < 0)
        return 0;
    if (value > maximum)
        return maximum;
    return value;
}

static StarsI16 measure_distance_changed(const StarsPoint *a,
    const StarsPoint *b)
{
    return abs((int)a->x - (int)b->x) > 2 ||
        abs((int)a->y - (int)b->y) > 2;
}

static StarsI16 measure_id(const StarsMeasureTapeInput *input,
    const StarsWayPointScan *scan)
{
    if (scan->grobj == 1U)
        return scan->idpl;
    if (scan->grobj == 2U && input->fleet_ids && scan->ifl >= 0 &&
            (StarsU16)scan->ifl < input->fleet_count)
        return (StarsI16)input->fleet_ids[(StarsU16)scan->ifl];
    if (scan->grobj == 8U && input->thing_ids && scan->ith >= 0 &&
            (StarsU16)scan->ith < input->thing_count)
        return (StarsI16)input->thing_ids[(StarsU16)scan->ith];
    return scan->iwp;
}

/* NE 0C:9974-9CF9.  Track the measuring-tape endpoint in scanner
 * coordinates, snap it to a nearby object, and publish the live distance
 * marker until the mouse-move service ends the drag. */
StarsI16 stars_handle_measuring_tape(StarsMeasureTapeInput *input,
    StarsWayPointScan *initial_scan, StarsPoint point)
{
    StarsPoint initial_logical;
    StarsPoint start_scan;
    StarsPoint endpoint_scan;
    StarsPoint endpoint_logical;
    StarsWayPointScan nearest;
    StarsMeasureBar bar;
    StarsU16 dc;
    StarsU16 old_pen;
    StarsU16 old_rop;
    StarsI16 dragging;
    StarsI16 max_y;
    StarsU16 mask;
    StarsU8 no_object[20];

    if (!input || !initial_scan || !input->mouse_move ||
            !input->get_dc || !input->select_pen || !input->set_rop)
        return 0;
    initial_logical = initial_scan->point;
    start_scan = initial_scan->point;
    if (input->logical_to_scan)
        input->logical_to_scan(input->context, &start_scan);
    max_y = (StarsI16)(input->client_bottom - input->scrollbar_height);
    dc = input->get_dc(input->context, input->scanner_window);
    if (input->capture)
        input->capture(input->context, input->scanner_window);
    old_pen = input->select_pen(input->context, dc, 0);
    old_rop = input->set_rop(input->context, dc, 7);
    endpoint_scan = point;
    if (input->logical_to_scan)
        input->logical_to_scan(input->context, &endpoint_scan);
    dragging = 1;
    for (;;) {
        if (!input->mouse_move(input->context, &endpoint_scan)) {
            if (!dragging && input->move_to && input->line_to) {
                input->move_to(input->context, dc, start_scan.x, start_scan.y);
                input->line_to(input->context, dc,
                    point.x, point.y);
            }
            input->set_rop(input->context, dc, old_rop);
            input->select_pen(input->context, dc, old_pen);
            if (input->draw_bar)
                input->draw_bar(input->context, dc, 0);
            if (input->release_capture)
                input->release_capture(input->context);
            if (input->release_dc)
                input->release_dc(input->context, input->scanner_window, dc);
            return (StarsI16)(dragging == 0);
        }
        endpoint_scan.x = measure_clamp(endpoint_scan.x,
            input->client_right);
        endpoint_scan.y = measure_clamp(endpoint_scan.y, max_y);
        endpoint_logical = endpoint_scan;
        if (input->scan_to_logical)
            input->scan_to_logical(input->context, &endpoint_logical);
        mask = input->shift_down && input->shift_down(input->context) ?
            0x8fU : 0x4fU;
        nearest = *initial_scan;
        nearest.grobj = 0U;
        nearest.grobj_full = 0U;
        nearest.idpl = -1;
        nearest.ifl = -1;
        nearest.iwp = -1;
        nearest.ith = -1;
        nearest.point = endpoint_logical;
        if (input->find_nearest && input->find_nearest(input->context,
                endpoint_scan, mask, &nearest))
            endpoint_logical = nearest.point;
        if (initial_logical.x == endpoint_logical.x &&
                initial_logical.y == endpoint_logical.y)
            continue;
        endpoint_scan = endpoint_logical;
        if (input->logical_to_scan)
            input->logical_to_scan(input->context, &endpoint_scan);
        if (dragging && measure_distance_changed(&initial_logical,
                &endpoint_logical))
            dragging = 0;
        if (dragging)
            continue;
        if (input->move_to && input->line_to) {
            input->move_to(input->context, dc, start_scan.x, start_scan.y);
            input->line_to(input->context, dc,
                endpoint_scan.x, endpoint_scan.y);
        }
        bar.point = endpoint_logical;
        bar.kind = nearest.grobj;
        if (nearest.grobj == 0U) {
            bar.id = -1;
            if (input->no_object_text)
                input->no_object_text(input->context, no_object,
                    (StarsU16)sizeof(no_object));
            else
                no_object[0] = 0;
            bar.label = no_object;
        } else {
            bar.id = measure_id(input, &nearest);
            bar.label = 0;
        }
        if (input->draw_bar)
            input->draw_bar(input->context, dc, &bar);
        point = endpoint_scan;
        initial_logical = endpoint_logical;
    }
}
