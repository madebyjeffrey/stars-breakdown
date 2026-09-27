#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace {
    StarsI16 actions[32];
    StarsI16 resolved;
    StarsI16 text_reads;
    StarsI16 text_sets;
    StarsI16 queued_checks;
    StarsI16 tracked;
    StarsI16 default_calls;
} ProbeTrace;

static void probe_action(void *opaque, StarsU16 action, StarsI16 first,
    StarsI16 second)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)first;
    (void)second;
    if (action < 32U)
        ++trace->actions[action];
}

static StarsI16 probe_selection(void *opaque, StarsU16 control)
{ (void)opaque; (void)control; return 1; }

static StarsI16 probe_resolve(void *opaque, StarsI16 mode, StarsI16 selection,
    StarsSlotDialogState *state)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)mode;
    (void)selection;
    ++trace->resolved;
    state->design_empty = 0;
    state->design_exists = 0;
    state->design_queued = 0;
    state->design_index = 1;
    state->hull_first_part = 1;
    state->part_mask = 0x19ff;
    return 1;
}

static StarsI16 probe_tutorial(void *opaque, StarsI16 step)
{ (void)opaque; (void)step; return 1; }

static StarsI16 probe_queued(void *opaque, StarsU16 window,
    const StarsSlotDialogState *state, StarsI16 edit)
{ ProbeTrace *trace = (ProbeTrace *)opaque; (void)window; (void)state; (void)edit; ++trace->queued_checks; return 1; }

static StarsI16 probe_get_text(void *opaque, StarsU16 control, StarsU8 *text,
    StarsU16 length)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)control;
    ++trace->text_reads;
    if (length > 4U) {
        text[0] = 'N'; text[1] = 'e'; text[2] = 'w'; text[3] = 0;
    }
    return 3;
}

static StarsI16 probe_fits(void *opaque, const StarsU8 *text, StarsI16 width)
{ (void)opaque; (void)text; (void)width; return 0; }

static void probe_set_text(void *opaque, StarsU16 control, const StarsU8 *text)
{ ProbeTrace *trace = (ProbeTrace *)opaque; (void)control; (void)text; ++trace->text_sets; }

static StarsI16 probe_track(void *opaque, StarsU16 window, StarsI16 x,
    StarsI16 y, StarsI16 wparam, StarsI16 right_button)
{ ProbeTrace *trace = (ProbeTrace *)opaque; (void)window; (void)x; (void)y; (void)wparam; (void)right_button; ++trace->tracked; return 1; }

static StarsI32 probe_default(void *opaque, StarsU16 window, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{ ProbeTrace *trace = (ProbeTrace *)opaque; (void)window; (void)message; (void)wparam; (void)lparam; ++trace->default_calls; return 77; }

static StarsU32 command_lparam(StarsU16 control, StarsU16 notify)
{ return (StarsU32)control | ((StarsU32)notify << 16); }

int main(void)
{
    StarsSlotDialogState state;
    StarsSlotDialogOps ops;
    StarsSlotDialogInput input;
    ProbeTrace trace;
    StarsI32 result;
    StarsI16 failures;

    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    state.parts_control = STARS_SLOT_CONTROL_PARTS;
    state.designs_control = STARS_SLOT_CONTROL_DESIGNS;
    state.edit_control = STARS_SLOT_CONTROL_EDIT;
    state.part_mask = 0x19ff;
    state.starbase_part_mask = 0x0a3c;
    state.tutorial_active = 1;
    ops.action = probe_action;
    ops.selection = probe_selection;
    ops.resolve_design = probe_resolve;
    ops.tutorial_allowed = probe_tutorial;
    ops.check_queued = probe_queued;
    ops.get_text = probe_get_text;
    ops.fits_text = probe_fits;
    ops.set_text = probe_set_text;
    ops.track_mouse = probe_track;
    ops.default_proc = probe_default;
    input.state = &state;
    input.ops = &ops;
    input.context = &trace;

    result = stars_slot_dlg(&input, 10, STARS_SLOT_WM_INIT_DIALOG, 0, 0);
    failures = result != 1 || !state.dialog_open || state.mode != 0 ||
        trace.actions[STARS_SLOT_ACTION_INIT] != 1;
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 0,
        command_lparam(STARS_SLOT_CONTROL_STARBASE + 1U, STARS_SLOT_COMMAND_CLICK));
    failures += state.starbase_mode != 1;
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 0,
        command_lparam(STARS_SLOT_CONTROL_BUILD_MODE_LAST, STARS_SLOT_COMMAND_CLICK));
    failures += state.mode != 3;
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 0,
        command_lparam(STARS_SLOT_CONTROL_DESIGNS, STARS_SLOT_COMMAND_CHANGE));
    failures += trace.resolved < 1;
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 0,
        command_lparam(STARS_SLOT_CONTROL_EDIT, STARS_SLOT_COMMAND_EDIT));
    failures += trace.text_reads != 1 || trace.text_sets != 1 ||
        state.class_name[0] != 'N';
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 0,
        command_lparam(STARS_SLOT_CONTROL_DELETE, STARS_SLOT_COMMAND_CLICK));
    failures += trace.actions[STARS_SLOT_ACTION_DELETE_DESIGN] != 1;
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 0,
        command_lparam(STARS_SLOT_CONTROL_DESIGNS, STARS_SLOT_COMMAND_CHANGE));
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 0,
        command_lparam(STARS_SLOT_CONTROL_COPY, STARS_SLOT_COMMAND_CLICK));
    failures += state.mode != STARS_SLOT_MODE_COPY || !state.hull_copy;
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 0,
        command_lparam(STARS_SLOT_CONTROL_ACCEPT_EDIT, STARS_SLOT_COMMAND_CLICK));
    failures += trace.actions[STARS_SLOT_ACTION_ENTER_COPY] < 2;
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 1, 0);
    failures += state.mode != 0 || state.hull_copy != 0;
    result = stars_slot_dlg(&input, 10, 0x0201, 0, command_lparam(4, 5));
    failures += result != 1 || trace.tracked != 1;
    result = stars_slot_dlg(&input, 10, 0x0450, 0, 0);
    failures += result != 77 || trace.default_calls != 1;
    stars_slot_dlg(&input, 10, STARS_SLOT_WM_COMMAND, 2, 0);
    failures += state.dialog_open != 0 || trace.actions[STARS_SLOT_ACTION_CLOSE] != 1;
    {
        FILE *report;
        report = fopen("C:\\SLOTDLG\\SDR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "SLOT_DLG_CHECKS=10 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("SLOT_DLG_CHECKS=10 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
