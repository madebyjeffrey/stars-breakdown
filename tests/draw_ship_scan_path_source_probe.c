#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace {
    StarsI16 lines;
    StarsI16 modes;
    StarsI16 acquired;
    StarsI16 released;
    StarsI16 last_kind;
} ProbeTrace;

static StarsU16 probe_acquire(void *opaque, StarsU16 window)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)window; ++trace->acquired; return 77U;
}

static void probe_release(void *opaque, StarsU16 window, StarsU16 dc)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)window; (void)dc; ++trace->released;
}

static void probe_line(void *opaque, StarsU16 dc, StarsPoint from,
    StarsPoint to, StarsI16 kind)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)dc; (void)from; (void)to; ++trace->lines; trace->last_kind = kind;
}

static void probe_state(void *opaque, StarsU16 dc, StarsI16 value)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)dc; (void)value; ++trace->modes;
}

int main(void)
{
    StarsDrawShipScanPathState state;
    StarsDrawShipScanPathOps ops;
    StarsDrawShipScanPathInput input;
    ProbeTrace trace;
    StarsI16 failures;

    memset(&state, 0, sizeof state); memset(&trace, 0, sizeof trace);
    state.selected_kind = STARS_SCAN_PATH_KIND_FLEET;
    state.orders_visible = 0U; state.scanner_flags = STARS_SCAN_PATH_FLAG_STARBASE;
    state.transit_valid = 1; state.selected_point.x = 10;
    state.selected_point.y = 10; state.transit_point.x = 30;
    state.transit_point.y = 20; state.route_count = 3;
    state.route_points[0] = state.selected_point;
    state.route_points[1].x = 20; state.route_points[1].y = 20;
    state.route_points[2].x = 30; state.route_points[2].y = 10;
    ops.acquire_dc = probe_acquire; ops.release_dc = probe_release;
    ops.line = probe_line; ops.set_pen = probe_state; ops.set_mode = probe_state;
    input.scanner_window = 12U; input.scrollbar_height = 10;
    input.client_left = 0; input.client_top = 0; input.client_right = 100;
    input.client_bottom = 100; input.state = &state; input.ops = &ops;
    input.context = &trace;
    stars_draw_ship_scan_path(0U, 1, &input);
    failures = 0;
    failures += trace.lines < 3;
    failures += trace.acquired != 1;
    failures += trace.released != 1;
    failures += state.orders_visible != 1U;
    failures += trace.modes < 2;
    stars_draw_ship_scan_path(0U, 1, &input);
    failures += trace.acquired != 1;
    printf("SCAN_PATH_CHECKS=5 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
