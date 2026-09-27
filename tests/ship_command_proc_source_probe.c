#include "core.h"
#include <stdio.h>

typedef struct ProbeTrace {
    StarsI16 actions;
    StarsI16 last_action;
    StarsI16 last_value;
} ProbeTrace;

static void probe_action(void *opaque, StarsI16 action, StarsI16 value)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    ++trace->actions;
    trace->last_action = action;
    trace->last_value = value;
}

static StarsI32 probe_selection(void *opaque, StarsU16 control)
{
    (void)opaque;
    if (control == 114U) return 1L;
    if (control == 120U) return 1L;
    if (control == 121U) return 7L;
    if (control == 122U) return 4L;
    return 0L;
}

static StarsI32 probe_edit(void *opaque, StarsU16 control)
{
    (void)opaque; (void)control;
    return 5000L;
}

static StarsU32 probe_lparam(StarsU16 control, StarsU16 notify)
{
    return (StarsU32)control | ((StarsU32)notify << 16);
}

int main(void)
{
    StarsShipCommandState state;
    StarsShipCommandOps ops;
    StarsShipCommandInput input;
    ProbeTrace trace;
    StarsI16 failures;
    StarsI16 i;

    for (i = 0; i < STARS_SHIP_COMMAND_BUTTONS; ++i)
        state.button[i] = (StarsU16)(200U + i);
    state.ship_dropdown = 110U; state.ship_list = 111U;
    state.component_list = 112U; state.battle_dropdown = 113U;
    state.report_combo = 114U; state.order_dropdown[0] = 120U;
    state.order_dropdown[1] = 121U; state.order_dropdown[2] = 122U;
    state.order_edit = 123U; state.fleet_id = 8; state.planet_id = 9;
    state.order_kind = 0; state.report_flags = 0U;
    state.order_values[0] = 0; state.order_values[1] = 0;
    state.order_values[2] = 0; trace.actions = 0;
    ops.action = probe_action; ops.selection = probe_selection;
    ops.edit_value = probe_edit;
    input.frame_window = 100U; input.mine_window = 101U;
    input.scanner_window = 102U; input.message_window = 103U;
    input.player = 2U; input.state = &state; input.ops = &ops;
    input.context = &trace;

    stars_ship_command_proc(100U, 0U, probe_lparam(204U, 0U), &input);
    stars_ship_command_proc(100U, 0U, probe_lparam(110U, 1U), &input);
    stars_ship_command_proc(100U, 0U, probe_lparam(111U, 1U), &input);
    stars_ship_command_proc(100U, 0U, probe_lparam(120U, 1U), &input);
    stars_ship_command_proc(100U, 0U, probe_lparam(121U, 1U), &input);
    stars_ship_command_proc(100U, 0U, probe_lparam(123U,
        STARS_SHIP_COMMAND_NOTIFY_EDIT), &input);
    stars_ship_command_proc(100U, 0U, probe_lparam(114U, 0U), &input);

    failures = 0;
    failures += trace.actions < 7;
    failures += state.order_kind != 1;
    failures += state.order_values[0] != 4000;
    failures += (state.report_flags & 0x0200U) == 0U;
    failures += trace.last_action != STARS_SHIP_ACTION_UPDATE_REPORT;
    printf("SHIP_COMMAND_CHECKS=5 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
