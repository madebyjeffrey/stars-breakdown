#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace {
    StarsI16 posts, paints, scrolls, adds, measures, popups, mains, cursors;
    StarsI16 last_command, last_dx, last_dy, last_cursor;
    StarsI16 scroll_x, scroll_y;
} ProbeTrace;

static StarsU16 probe_begin(void *opaque, StarsU16 window, StarsRect *rect)
{
    (void)opaque; (void)window; rect->left = 0; rect->top = 0;
    rect->right = 100; rect->bottom = 100; return 9;
}
static void probe_end(void *opaque, StarsU16 window, StarsU16 dc, StarsRect *rect)
{ (void)opaque; (void)window; (void)dc; (void)rect; }
static void probe_rect(void *opaque, StarsU16 window, StarsRect *rect)
{ (void)opaque; (void)window; rect->left = 0; rect->top = 0; rect->right = 100; rect->bottom = 100; }
static void probe_draw(void *opaque, StarsU16 dc, const StarsRect *rect)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)rect; ++t->paints; }
static void probe_post(void *opaque, StarsU16 window, StarsI16 command)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; ++t->posts; t->last_command = command; }
static void probe_focus(void *opaque, StarsU16 window)
{ (void)opaque; (void)window; }
static void probe_cursor_pos(void *opaque, StarsPoint *point)
{ (void)opaque; point->x = 5; point->y = 5; }
static void probe_map(void *opaque, StarsU16 window, StarsPoint *point)
{ (void)opaque; (void)window; (void)point; }
static StarsI16 probe_in(void *opaque, const StarsRect *rect, StarsPoint point)
{ (void)opaque; return point.x >= (StarsI16)rect->left && point.x < (StarsI16)rect->right &&
    point.y >= (StarsI16)rect->top && point.y < (StarsI16)rect->bottom; }
static StarsU16 probe_load(void *opaque, StarsU16 id)
{ (void)opaque; return id; }
static void probe_set_cursor(void *opaque, StarsU16 cursor)
{ ProbeTrace *t = (ProbeTrace *)opaque; ++t->cursors; t->last_cursor = cursor; }
static StarsU16 probe_key(void *opaque, StarsU16 key)
{ (void)opaque; (void)key; return 0; }
static void probe_cross(void *opaque, StarsU16 window, StarsPoint point)
{ (void)opaque; (void)window; (void)point; }
static void probe_set_scroll(void *opaque, StarsU16 window, StarsI16 bar, StarsI16 value)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; if (bar) t->scroll_y = value; else t->scroll_x = value; }
static StarsI16 probe_get_scroll(void *opaque, StarsU16 window, StarsI16 bar)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; return bar ? t->scroll_y : t->scroll_x; }
static void probe_scroll(void *opaque, StarsI16 dx, StarsI16 dy)
{ ProbeTrace *t = (ProbeTrace *)opaque; ++t->scrolls; t->last_dx = dx; t->last_dy = dy; }
static void probe_transform(void *opaque, StarsPoint *point)
{ (void)opaque; (void)point; }
static void probe_nearest(void *opaque, StarsPoint point, StarsI16 radius, StarsScanSelection *selection)
{ (void)opaque; (void)radius; selection->x = point.x; selection->y = point.y;
    selection->grobj = 1; selection->grobj_full = 1; selection->planet = 7; }
static StarsI16 probe_near(void *opaque, StarsPoint point, StarsI16 mode)
{ (void)opaque; (void)point; (void)mode; return 0; }
static StarsI16 probe_drag(void *opaque, StarsPoint point)
{ (void)opaque; (void)point; return 0; }
static void probe_add(void *opaque, StarsPoint point, const StarsScanSelection *selection)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)point; (void)selection; ++t->adds; }
static void probe_measure(void *opaque, const StarsScanSelection *selection, StarsPoint point)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)selection; (void)point; ++t->measures; }
static StarsI16 probe_popup(void *opaque, StarsU16 window, StarsPoint point,
    const StarsScanSelection *selection, StarsScannerPopupChoice *choice)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; (void)point; (void)selection;
    ++t->popups; choice->kind = 1; choice->id = 7; choice->owner = 0; return 0; }
static void probe_popup_below(void *opaque, StarsU16 window, StarsPoint point,
    const StarsScanSelection *selection)
{ (void)opaque; (void)window; (void)point; (void)selection; }
static void probe_change_scan(void *opaque, const StarsScanSelection *selection, StarsI16 mode)
{ (void)opaque; (void)selection; (void)mode; }
static StarsI16 probe_next(void *opaque, const StarsScanSelection *selection, StarsI16 only_ours)
{ (void)opaque; (void)selection; (void)only_ours; return 1; }
static StarsI16 probe_can(void *opaque, StarsI16 kind, StarsI16 id, StarsI16 owner)
{ (void)opaque; (void)kind; (void)id; (void)owner; return 1; }
static void probe_main(void *opaque, StarsI16 kind, StarsI16 id)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)kind; (void)id; ++t->mains; }
static void probe_redraw(void *opaque, StarsI16 erase, StarsI16 selected)
{ (void)opaque; (void)erase; (void)selected; }
static void probe_toggle(void *opaque, StarsI16 id, StarsI16 route)
{ (void)opaque; (void)id; (void)route; }
static StarsI16 probe_warp(void *opaque)
{ (void)opaque; return 0; }
static StarsI32 probe_default(void *opaque, StarsU16 window, StarsU16 msg,
    StarsU16 wparam, StarsU32 lparam)
{ (void)opaque; (void)window; (void)msg; (void)wparam; (void)lparam; return 44L; }

int main(void)
{
    StarsScannerWndOps ops;
    StarsScannerWndState state;
    ProbeTrace trace;
    StarsI16 failures;

    memset(&ops, 0, sizeof ops); memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    ops.begin_paint = probe_begin; ops.end_paint = probe_end;
    ops.get_client_rect = probe_rect; ops.draw_scrollbar = probe_draw;
    ops.draw_scanner = probe_draw; ops.post_command = probe_post;
    ops.set_focus = probe_focus; ops.get_cursor_pos = probe_cursor_pos;
    ops.screen_to_client = probe_map; ops.point_in_rect = probe_in;
    ops.load_cursor = probe_load; ops.set_cursor = probe_set_cursor;
    ops.get_async_key_state = probe_key; ops.draw_crosshair = probe_cross;
    ops.set_scroll_pos = probe_set_scroll; ops.get_scroll_pos = probe_get_scroll;
    ops.scroll_scanner = probe_scroll; ops.logical_to_scan = probe_transform;
    ops.scan_to_logical = probe_transform; ops.find_nearest = probe_nearest;
    ops.near_waypoint = probe_near; ops.drag_waypoint = probe_drag;
    ops.add_waypoint = probe_add; ops.measuring_tape = probe_measure;
    ops.popup_menu = probe_popup; ops.popup_below = probe_popup_below;
    ops.change_scan_selection = probe_change_scan; ops.next_object_here = probe_next;
    ops.can_select = probe_can; ops.change_main_object = probe_main;
    ops.redraw_scan_selection = probe_redraw; ops.toggle_planet_route = probe_toggle;
    ops.warp_from_planet = probe_warp; ops.default_proc = probe_default;
    state.frame_window = 10; state.scanner_window = 11; state.zoom = 0;
    state.scan_increment = 4; state.scan_page = 12; state.scrollbar_height = 10;
    state.fleets_present = 1; state.selected_kind = 2; state.selected_point.x = 5;
    state.selected_point.y = 5; state.ops = &ops; state.context = &trace;

    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_CREATE, 0, 0, 0);
    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_SIZE, 0, 0, 0);
    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_PAINT, 0, 0, 0);
    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_CHAR, '-', 0, 0);
    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_HSCROLL, 1, 0, 0);
    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_SETCURSOR, 0, 0, 0);
    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_LBUTTONDOWN, 4, 0, 0x00050005UL);
    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_MBUTTONDOWN, 0, 0, 0x00050005UL);
    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_RBUTTONDOWN, 0, 0, 0x00050005UL);
    state.selected_kind = 1;
    stars_scanner_wnd_proc(&state, STARS_SCANNER_WM_LBUTTONDOWN, 0, 0, 0x00050005UL);
    failures = 0;
    failures += state.x_scan_top != 1004;
    failures += trace.posts < 2;
    failures += trace.paints < 2;
    failures += trace.scrolls != 1;
    failures += trace.adds != 1;
    failures += trace.measures != 1;
    failures += trace.popups != 1;
    failures += trace.mains != 1;
    {
        FILE *report;
        report = fopen("C:\\SCWND\\SWR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "SCANNER_WND_CHECKS=8 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("SCANNER_WND_CHECKS=8 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
