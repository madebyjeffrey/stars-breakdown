#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace {
    StarsI16 actions;
    StarsI16 last_action;
    StarsI32 last_value;
} ProbeTrace;

static void probe_action(void *opaque, StarsI16 action, StarsI32 value)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    ++trace->actions; trace->last_action = action; trace->last_value = value;
}

static StarsI32 probe_drag(void *opaque, StarsI16 region, StarsI32 current)
{
    (void)opaque; (void)current;
    if (region == STARS_CLICK_REGION_FUEL_TRANSFER) return -5L;
    if (region == STARS_CLICK_REGION_ORDER_KIND) return 20L;
    return 7L;
}

static StarsI16 probe_popup(void *opaque, StarsI16 region, StarsI16 x,
    StarsI16 y)
{
    (void)opaque; (void)region; (void)x; (void)y;
    return 9;
}

static void probe_rect(StarsRect *rect, StarsI16 left)
{
    rect->left = left; rect->top = 0; rect->right = (StarsI16)(left + 10);
    rect->bottom = 10;
}

int main(void)
{
    StarsClickShipOrdersState state;
    StarsClickShipOrdersOps ops;
    StarsClickShipOrdersInput input;
    ProbeTrace trace;
    StarsPoint point;
    StarsI16 failures;
    StarsI16 i;

    memset(&state, 0, sizeof state);
    for (i = 0; i < STARS_CLICK_SHIP_REGIONS; ++i)
        probe_rect(&state.reference[i], (StarsI16)(i * 20));
    state.selected_object = 2; state.selected_planet = 4;
    state.selected_fleet = 8; state.order_kind = 7;
    state.fuel = 20L; state.mass_warp = 3;
    trace.actions = 0;
    ops.action = probe_action; ops.drag = probe_drag; ops.popup = probe_popup;
    input.state = &state; input.ops = &ops; input.context = &trace;

    point.x = 60; point.y = 5;
    stars_click_ship_orders(point, 0, 0, 0, &input);
    point.x = 0; point.y = 5;
    stars_click_ship_orders(point, 0, 1, 0, &input);
    stars_click_ship_orders(point, 0, 0, 0, &input);
    point.x = 40; point.y = 5;
    stars_click_ship_orders(point, 0, 0, 0, &input);
    point.x = 240; point.y = 5;
    stars_click_ship_orders(point, 0, 0, 1, &input);

    failures = 0;
    failures += state.fuel != 0L;
    failures += state.order_values[0] != 15;
    failures += state.order_kind != 9;
    failures += trace.actions < 6;
    failures += trace.last_action != STARS_CLICK_ACTION_SET_ORDER_KIND;
    printf("CLICK_SHIP_CHECKS=5 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
