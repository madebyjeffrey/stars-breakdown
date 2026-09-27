#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace {
    StarsI16 moves, previews, bars, warped, changed, released, invalidated;
    StarsI16 delete_prompt, last_kind, last_id;
    StarsPoint next_mouse;
} ProbeTrace;

static void probe_transform(void *opaque, StarsPoint *point)
{ (void)opaque; (void)point; }
static StarsI16 probe_move(void *opaque, StarsPoint *point)
{ ProbeTrace *t = (ProbeTrace *)opaque; if (t->moves++) return 0;
    *point = t->next_mouse; return 1; }
static StarsI16 probe_near(void *opaque, StarsPoint point, StarsI16 mode)
{ (void)opaque; (void)point; (void)mode; return 0; }
static void probe_find(void *opaque, StarsPoint point, StarsI16 radius,
    StarsScanSelection *selection)
{ (void)opaque; (void)radius; selection->x = point.x; selection->y = point.y;
    selection->grobj = 2; selection->grobj_full = 2; selection->fleet = 4; }
static void probe_xor(void *opaque, StarsU16 dc, const StarsPoint *points,
    StarsI16 count)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)points; (void)count; ++t->previews; }
static void probe_bar(void *opaque, StarsU16 dc, const StarsPoint *point,
    const StarsScanSelection *selection)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)point; (void)selection; ++t->bars; }
static StarsU16 probe_dc(void *opaque, StarsU16 window)
{ (void)opaque; (void)window; return 12; }
static void probe_release_dc(void *opaque, StarsU16 window, StarsU16 dc)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; (void)dc; ++t->released; }
static void probe_capture(void *opaque, StarsU16 window)
{ (void)opaque; (void)window; }
static void probe_release_capture(void *opaque)
{ (void)opaque; }
static void probe_cursor(void *opaque, StarsU16 cursor)
{ (void)opaque; (void)cursor; }
static void probe_rect(void *opaque, StarsU16 window, StarsRect *rect)
{ (void)opaque; (void)window; rect->left = 0; rect->top = 0; rect->right = 100; rect->bottom = 100; }
static StarsI16 probe_confirm(void *opaque, StarsI16 which)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)which; ++t->delete_prompt; return 0; }
static void probe_delete(void *opaque, StarsI16 which)
{ (void)opaque; (void)which; }
static void probe_redraw(void *opaque, StarsI16 erase, StarsI16 selected)
{ (void)opaque; (void)erase; (void)selected; }
static void probe_line(void *opaque, StarsU16 dc, StarsPoint from, StarsPoint to)
{ (void)opaque; (void)dc; (void)from; (void)to; }
static StarsU16 probe_warp(void *opaque, StarsI16 kind, const StarsPoint *point)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)point; ++t->warped; t->last_kind = kind; return 2; }
static void probe_fleet(void *opaque)
{ (void)opaque; }
static void probe_change(void *opaque, const StarsScanSelection *selection, StarsI16 mode)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)mode; ++t->changed; t->last_id = selection->fleet; }
static void probe_bound(void *opaque, StarsRect *rect, const StarsPoint *points, StarsI16 count)
{ (void)opaque; (void)rect; (void)points; (void)count; }
static void probe_draw(void *opaque, StarsU16 dc, const StarsRect *rect)
{ (void)opaque; (void)dc; (void)rect; }
static void probe_invalidate(void *opaque, StarsU16 window)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; ++t->invalidated; }
static void probe_title(void *opaque, StarsU16 window)
{ (void)opaque; (void)window; }

int main(void)
{
    StarsWaypointDragOps ops;
    StarsWaypointDragState state;
    ProbeTrace trace;
    StarsPoint point;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops); memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.next_mouse.x = 30; trace.next_mouse.y = 35;
    ops.logical_to_scan = probe_transform; ops.scan_to_logical = probe_transform;
    ops.get_mouse_move = probe_move; ops.near_waypoint = probe_near;
    ops.find_nearest = probe_find; ops.draw_xor_lines = probe_xor;
    ops.draw_bar = probe_bar; ops.get_dc = probe_dc; ops.release_dc = probe_release_dc;
    ops.set_capture = probe_capture; ops.release_capture = probe_release_capture;
    ops.set_cursor = probe_cursor; ops.get_client_rect = probe_rect;
    ops.confirm_delete = probe_confirm; ops.delete_waypoint = probe_delete;
    ops.redraw_scan_selection = probe_redraw; ops.draw_starbase_line = probe_line;
    ops.warp_best = probe_warp; ops.refresh_fleet = probe_fleet;
    ops.change_scan_selection = probe_change; ops.bound_points = probe_bound;
    ops.draw_scanner = probe_draw; ops.invalidate_mine = probe_invalidate;
    ops.set_mineral_title = probe_title;
    state.scanner_window = 11; state.mine_window = 12; state.order_count = 2;
    state.active_waypoint = 0; state.order_points[0].x = 5; state.order_points[0].y = 5;
    state.order_points[1].x = 20; state.order_points[1].y = 20;
    state.ops = &ops; state.context = &trace;
    point.x = 5; point.y = 5;
    failures = stars_handle_waypoint_drag(point, &state) != 1;
    failures += state.order_points[0].x != 30 || state.order_points[0].y != 35;
    failures += trace.warped != 1 || trace.changed != 1;
    failures += trace.released != 1 || trace.invalidated != 1;
    failures += trace.previews < 2 || trace.bars < 2;
    {
        FILE *report;
        report = fopen("C:\\WPDG\\WDR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "WAYPOINT_DRAG_CHECKS=5 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("WAYPOINT_DRAG_CHECKS=5 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
