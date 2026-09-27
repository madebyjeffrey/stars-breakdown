#include "core.h"

/* NE 12:8176-8AF6. Track a scanner waypoint under capture, preview the
 * nearest object, and commit the selected order record. The original uses
 * several far DGROUP tables and GDI objects; those services are callbacks so
 * this source expresses the drag state and order mutation directly. */

#define STARS_WAYPOINT_CURSOR_CLOSE_GRAB 4
#define STARS_WAYPOINT_STARBASE_FLAG 0x0080U

static StarsI16 waypoint_same(StarsPoint left, StarsPoint right)
{
    return left.x == right.x && left.y == right.y;
}

static StarsI16 waypoint_id(const StarsScanSelection *selection)
{
    if (selection->grobj == 1) return selection->planet;
    if (selection->grobj == 2) return selection->fleet;
    if (selection->grobj == 8) return selection->thing;
    return selection->waypoint;
}

static void waypoint_preview(const StarsWaypointDragState *state,
    StarsU16 dc, StarsPoint *points, StarsI16 line_count,
    const StarsScanSelection *selection)
{
    if (state->ops && state->ops->draw_xor_lines)
        state->ops->draw_xor_lines(state->context, dc, points, line_count);
    if (state->ops && state->ops->draw_bar)
        state->ops->draw_bar(state->context, dc, &points[line_count - 1],
            selection);
}

StarsI16 stars_handle_waypoint_drag(StarsPoint point,
    StarsWaypointDragState *state)
{
    StarsRect rect;
    StarsPoint preview[4];
    StarsPoint mouse;
    StarsPoint logical;
    StarsScanSelection nearest;
    StarsU16 dc;
    StarsU16 redraw_dc;
    StarsI16 line_count;
    StarsI16 moved;
    StarsI16 dragging;
    StarsI16 index;
    StarsI16 id;
    StarsI16 duplicate;
    StarsI16 confirmation;

    if (!state || !state->ops) return 0;
    if (state->active_waypoint < 0 ||
        state->active_waypoint >= state->order_count ||
        state->active_waypoint >= STARS_WAYPOINT_DRAG_MAX_ORDERS) return 0;
    index = state->active_waypoint;
    line_count = index == state->order_count - 1 ? 3 : 4;
    for (confirmation = 0; confirmation < line_count; ++confirmation)
        preview[confirmation] = state->order_points[index];
    preview[0] = state->order_points[index > 0 ? index - 1 : index];
    preview[1] = state->order_points[index];
    if (line_count > 3) preview[3] = state->order_points[index + 1];
    if (state->ops->logical_to_scan) {
        for (confirmation = 0; confirmation < line_count; ++confirmation)
            state->ops->logical_to_scan(state->context, &preview[confirmation]);
        state->ops->logical_to_scan(state->context, &point);
    }
    if (state->ops->get_client_rect)
        state->ops->get_client_rect(state->context, state->scanner_window, &rect);
    else {
        rect.left = 0; rect.top = 0; rect.right = 0xffffU; rect.bottom = 0xffffU;
    }
    rect.bottom = (StarsU16)((StarsI16)rect.bottom - state->scrollbar_height);
    dc = state->ops->get_dc ? state->ops->get_dc(state->context,
        state->scanner_window) : 0;
    if (state->ops->set_capture) state->ops->set_capture(state->context,
        state->scanner_window);
    mouse = point; moved = 0; dragging = 1;
    while (dragging && state->ops->get_mouse_move &&
        state->ops->get_mouse_move(state->context, &mouse)) {
        if (mouse.x > (StarsI16)rect.right) mouse.x = (StarsI16)rect.right;
        if (mouse.y > (StarsI16)rect.bottom) mouse.y = (StarsI16)rect.bottom;
        if (mouse.x < 0) mouse.x = 0;
        if (mouse.y < 0) mouse.y = 0;
        if (mouse.x == point.x && mouse.y == point.y) continue;
        if (!moved && state->ops->near_waypoint &&
            state->ops->near_waypoint(state->context, mouse, 0)) {
            point = mouse;
            continue;
        }
        moved = 1;
        logical = mouse;
        if (state->ops->scan_to_logical)
            state->ops->scan_to_logical(state->context, &logical);
        nearest.grobj = 0; nearest.grobj_full = 0;
        nearest.x = logical.x; nearest.y = logical.y;
        nearest.planet = -1; nearest.fleet = -1;
        nearest.waypoint = index; nearest.thing = -1;
        if (state->ops->find_nearest)
            state->ops->find_nearest(state->context, logical,
                (state->scan_flags & STARS_SCANNER_SCAN_ADD) ? 0x8f : 0x4f,
                &nearest);
        if (state->ops->draw_xor_lines)
            state->ops->draw_xor_lines(state->context, dc, preview, line_count);
        preview[line_count - 1] = logical;
        if (state->ops->logical_to_scan)
            state->ops->logical_to_scan(state->context,
                &preview[line_count - 1]);
        waypoint_preview(state, dc, preview, line_count, &nearest);
        point = mouse;
    }
    if (moved) {
        logical = preview[line_count - 1];
        if (state->ops->scan_to_logical)
            state->ops->scan_to_logical(state->context, &logical);
        duplicate = waypoint_same(logical, state->order_points[index > 0 ?
            index - 1 : index]);
        if (!duplicate && index < state->order_count - 1)
            duplicate = waypoint_same(logical, state->order_points[index + 1]);
        confirmation = 0;
        if (duplicate && state->ops->confirm_delete)
            confirmation = state->ops->confirm_delete(state->context,
                index == state->order_count - 1 ? 0 : 1);
        if (duplicate && confirmation && state->ops->delete_waypoint)
            state->ops->delete_waypoint(state->context,
                index == state->order_count - 1 ? 0 : 1);
        if (!duplicate || !confirmation) {
            id = waypoint_id(&nearest);
            if (state->ops->redraw_scan_selection)
                state->ops->redraw_scan_selection(state->context, 0, 0);
            state->order_points[index] = logical;
            state->order_kinds[index] = nearest.grobj;
            state->order_ids[index] = id;
            if (state->ops->warp_best)
                state->ops->warp_best(state->context, nearest.grobj, &logical);
            if (state->ops->refresh_fleet)
                state->ops->refresh_fleet(state->context);
            nearest.waypoint = index;
            nearest.grobj_full = (StarsI16)(nearest.grobj_full | 4);
            if (state->ops->change_scan_selection)
                state->ops->change_scan_selection(state->context, &nearest, 1);
        }
    }
    if (state->ops && state->ops->draw_bar)
        state->ops->draw_bar(state->context, dc, (StarsPoint *)0, (StarsScanSelection *)0);
    if (state->ops->release_capture) state->ops->release_capture(state->context);
    if (state->ops->set_cursor) state->ops->set_cursor(state->context,
        STARS_WAYPOINT_CURSOR_CLOSE_GRAB);
    if (state->ops->invalidate_mine)
        state->ops->invalidate_mine(state->context, state->mine_window);
    if (state->ops->set_mineral_title)
        state->ops->set_mineral_title(state->context, state->mine_window);
    if (state->ops->release_dc)
        state->ops->release_dc(state->context, state->scanner_window, dc);
    if (moved && (state->scan_flags & STARS_WAYPOINT_STARBASE_FLAG) != 0 &&
        state->ops->draw_scanner) {
        if (state->ops->bound_points)
            state->ops->bound_points(state->context, &rect, preview, line_count);
        redraw_dc = state->ops->get_dc ? state->ops->get_dc(state->context,
            state->scanner_window) : 0;
        state->ops->draw_scanner(state->context, redraw_dc, &rect);
        if (state->ops->release_dc)
            state->ops->release_dc(state->context, state->scanner_window,
                redraw_dc);
    }
    return moved;
}
