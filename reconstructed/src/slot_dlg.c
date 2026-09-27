#include "core.h"
#include <string.h>

static StarsU16 slot_low_word(StarsU32 value)
{ return (StarsU16)value; }

static StarsU16 slot_high_word(StarsU32 value)
{ return (StarsU16)(value >> 16); }

static void slot_action(const StarsSlotDialogInput *input, StarsU16 action,
    StarsI16 first, StarsI16 second)
{
    if (input->ops && input->ops->action)
        input->ops->action(input->context, action, first, second);
}

static StarsI16 slot_tutorial(const StarsSlotDialogInput *input,
    StarsI16 step)
{
    if (!input->state->tutorial_active)
        return 1;
    if (!input->ops || !input->ops->tutorial_allowed)
        return 0;
    return input->ops->tutorial_allowed(input->context, step);
}

static void slot_clear_hull(StarsSlotDialogState *state)
{
    state->hull_valid = 0;
    state->hull_first_part = 0;
    state->hull_copy = 0;
    state->design_valid = 0;
    state->design_empty = 1;
}

static void slot_refresh_selection(StarsSlotDialogInput *input)
{
    StarsSlotDialogState *state;
    StarsI16 selection;

    state = input->state;
    selection = -1;
    if (input->ops && input->ops->selection)
        selection = input->ops->selection(input->context,
            state->designs_control);
    state->selection = selection;
    state->design_valid = 0;
    if (selection >= 0 && input->ops && input->ops->resolve_design)
        state->design_valid = input->ops->resolve_design(input->context,
            state->mode, selection, state);
    if (selection >= 0)
        slot_action(input, STARS_SLOT_ACTION_SET_SELECTION, selection, 0);
    if (state->mode == 3 || state->mode == STARS_SLOT_MODE_COPY)
        slot_action(input, STARS_SLOT_ACTION_FILL_PARTS,
            state->part_mask, state->starbase_mode);
    slot_action(input, STARS_SLOT_ACTION_UPDATE_GLOBALS, state->selection, 0);
    slot_action(input, STARS_SLOT_ACTION_INVALIDATE, 0, 0);
}

static void slot_select_starbase(StarsSlotDialogInput *input,
    StarsU16 control)
{
    StarsSlotDialogState *state;

    state = input->state;
    state->starbase_mode = (StarsI16)(control - STARS_SLOT_CONTROL_STARBASE);
    slot_clear_hull(state);
    slot_action(input, STARS_SLOT_ACTION_FILL_PARTS,
        state->starbase_mode ? state->starbase_part_mask : state->part_mask,
        state->starbase_mode);
    slot_action(input, STARS_SLOT_ACTION_FILL_DESIGNS, state->mode,
        state->starbase_mode);
    slot_refresh_selection(input);
}

static void slot_select_mode(StarsSlotDialogInput *input, StarsU16 control)
{
    StarsSlotDialogState *state;

    state = input->state;
    state->mode = (StarsI16)(control - STARS_SLOT_CONTROL_BUILD_MODE);
    slot_clear_hull(state);
    slot_action(input, STARS_SLOT_ACTION_UPDATE_GLOBALS, state->mode, 0);
    slot_action(input, STARS_SLOT_ACTION_FILL_DESIGNS, state->mode,
        state->starbase_mode);
    slot_action(input, STARS_SLOT_ACTION_INVALIDATE, state->mode, 0);
    slot_action(input, STARS_SLOT_ACTION_SHOW_CONTROLS, state->mode,
        state->mode == 3);
    if (control != STARS_SLOT_CONTROL_BUILD_MODE_LAST)
        slot_refresh_selection(input);
    else if (state->tutorial_active)
        slot_action(input, STARS_SLOT_ACTION_ADVANCE_TUTOR, 0, 0);
}

static void slot_edit_change(StarsSlotDialogInput *input)
{
    StarsU8 text[32];
    StarsI16 count;

    if (input->state->edit_guard)
        return;
    input->state->edit_guard = 1;
    memset(text, 0, sizeof text);
    count = 0;
    if (input->ops && input->ops->get_text)
        count = input->ops->get_text(input->context,
            input->state->edit_control, text, (StarsU16)sizeof text);
    if (input->ops && input->ops->fits_text &&
        input->ops->fits_text(input->context, text, 0xa0) == 0) {
        if (input->ops->set_text)
            input->ops->set_text(input->context, input->state->edit_control,
                text);
    }
    if (count < 0)
        count = 0;
    if (count >= (StarsI16)sizeof input->state->class_name)
        count = (StarsI16)sizeof input->state->class_name - 1;
    memcpy(input->state->class_name, text, (size_t)count);
    input->state->class_name[count] = 0;
    slot_action(input, STARS_SLOT_ACTION_REDRAW_HULL, 0x100, 0);
    input->state->edit_guard = 0;
    if (input->state->tutorial_active)
        slot_action(input, STARS_SLOT_ACTION_ADVANCE_TUTOR, 0, 0);
}

static void slot_delete_design(StarsSlotDialogInput *input)
{
    StarsSlotDialogState *state;
    StarsI16 selection;

    state = input->state;
    if (!slot_tutorial(input, 0))
        return;
    selection = state->selection;
    if (input->ops && input->ops->selection)
        selection = input->ops->selection(input->context,
            state->designs_control);
    if (selection < 0 || !state->design_valid)
        return;
    if (state->starbase_mode && selection == 0 && state->race_special)
        return;
    if (input->ops && input->ops->check_queued &&
        !input->ops->check_queued(input->context, state->hwnd, state, 0))
        return;
    slot_action(input, STARS_SLOT_ACTION_DELETE_DESIGN, selection,
        state->starbase_mode);
    state->design_valid = 0;
    state->hull_valid = 0;
    slot_action(input, STARS_SLOT_ACTION_LOG_DESIGN, selection, 0);
    slot_action(input, STARS_SLOT_ACTION_FILL_DESIGNS, state->mode,
        state->starbase_mode);
    slot_action(input, STARS_SLOT_ACTION_ENABLE_CONTROLS, 0, 0);
    slot_action(input, STARS_SLOT_ACTION_UPDATE_GLOBALS, 0, 0);
    slot_action(input, STARS_SLOT_ACTION_INVALIDATE, 0, 0);
}

static void slot_copy_design(StarsSlotDialogInput *input)
{
    StarsSlotDialogState *state;
    StarsI16 selection;

    state = input->state;
    if (!slot_tutorial(input, 1))
        return;
    selection = state->selection;
    if (input->ops && input->ops->selection)
        selection = input->ops->selection(input->context,
            state->designs_control);
    if (selection < 0 || !state->design_valid)
        return;
    if (input->ops && input->ops->check_queued &&
        !input->ops->check_queued(input->context, state->hwnd, state, 1))
        return;
    state->mode = STARS_SLOT_MODE_COPY;
    state->hull_copy = 1;
    state->hull_valid = 1;
    slot_action(input, STARS_SLOT_ACTION_ENTER_COPY, selection,
        state->starbase_mode);
    slot_action(input, STARS_SLOT_ACTION_UPDATE_GLOBALS, selection, 0);
    slot_action(input, STARS_SLOT_ACTION_INVALIDATE, 0, 0);
}

static void slot_accept_design(StarsSlotDialogInput *input)
{
    StarsSlotDialogState *state;
    StarsI16 selection;

    state = input->state;
    if (!slot_tutorial(input, 2))
        return;
    selection = state->selection;
    if (input->ops && input->ops->selection)
        selection = input->ops->selection(input->context,
            state->designs_control);
    if (selection < 0 || !state->design_valid)
        return;
    if (input->ops && input->ops->check_queued &&
        !input->ops->check_queued(input->context, state->hwnd, state, 1))
        return;
    state->mode = STARS_SLOT_MODE_COPY;
    state->hull_valid = 1;
    slot_action(input, STARS_SLOT_ACTION_ENTER_COPY, selection,
        state->starbase_mode);
    slot_action(input, STARS_SLOT_ACTION_SHOW_CONTROLS, 0, 0);
    slot_action(input, STARS_SLOT_ACTION_FILL_PARTS,
        state->starbase_mode ? 0x0a3c : 0x19ff, state->starbase_mode);
    slot_action(input, STARS_SLOT_ACTION_SET_EDIT_TEXT, 0, 0);
    if (state->tutorial_active)
        slot_action(input, STARS_SLOT_ACTION_ADVANCE_TUTOR, 0, 0);
}

static void slot_finish_edit(StarsSlotDialogInput *input, StarsU16 result)
{
    StarsSlotDialogState *state;

    state = input->state;
    if (state->mode != STARS_SLOT_MODE_COPY) {
        slot_action(input, STARS_SLOT_ACTION_SET_SELECTION, -2, 0);
        slot_action(input, STARS_SLOT_ACTION_CLOSE, result == 1, 0);
        state->dialog_open = 0;
        if (state->tutorial_active)
            slot_action(input, STARS_SLOT_ACTION_ADVANCE_TUTOR, 0, 0);
        return;
    }
    if (result == 1) {
        if (!state->starbase_mode && !state->hull_first_part)
            return;
        if (!slot_tutorial(input, 3))
            return;
        slot_action(input, STARS_SLOT_ACTION_COMMIT_EDIT,
            state->design_index, state->starbase_mode);
        slot_action(input, STARS_SLOT_ACTION_LOG_DESIGN,
            state->design_index, 0);
    }
    else if (!slot_tutorial(input, 4)) {
        return;
    }
    if (result == 2)
        slot_action(input, STARS_SLOT_ACTION_CANCEL_EDIT,
            state->design_index, state->starbase_mode);
    slot_action(input, STARS_SLOT_ACTION_INVALIDATE, 0, 0);
    slot_action(input, STARS_SLOT_ACTION_SHOW_CONTROLS, 1, 0);
    slot_action(input, STARS_SLOT_ACTION_FILL_PARTS,
        state->starbase_mode ? state->starbase_part_mask : state->part_mask,
        state->starbase_mode);
    state->mode = 0;
    state->hull_copy = 0;
    state->hull_valid = 0;
    slot_select_mode(input, STARS_SLOT_CONTROL_BUILD_MODE);
}

/* Genuine C reconstruction of SlotDlg, NE 1A:0550-264F.  The message
 * dispatcher and build-mode transitions are retained here; packed hull/design
 * records and USER/GDI effects enter through named callbacks. */
StarsI32 stars_slot_dlg(StarsSlotDialogInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsSlotDialogState *state;
    StarsU16 control;
    StarsU16 notify;
    StarsI16 result;

    if (!input || !input->state)
        return 0;
    state = input->state;
    if (message == STARS_SLOT_WM_PAINT) {
        slot_action(input, STARS_SLOT_ACTION_PAINT, hwnd, state->mode);
        return 1;
    }
    if (message == STARS_SLOT_WM_ERASE_BACKGROUND) {
        slot_action(input, STARS_SLOT_ACTION_ERASE, hwnd, wparam);
        return 1;
    }
    if (message == STARS_SLOT_WM_CONTROL_COLOR) {
        slot_action(input, STARS_SLOT_ACTION_CONTROL_COLOR, hwnd, wparam);
        if (input->ops && input->ops->default_proc)
            return input->ops->default_proc(input->context, hwnd, message,
                wparam, lparam);
        return 1;
    }
    if (message == STARS_SLOT_WM_SETCURSOR) {
        slot_action(input, STARS_SLOT_ACTION_CONTROL_COLOR, hwnd,
            (StarsI16)slot_low_word(lparam));
        return 1;
    }
    if (message == STARS_SLOT_WM_DRAW_ITEM) {
        slot_action(input, STARS_SLOT_ACTION_DRAW_ITEM, hwnd,
            (StarsI16)slot_low_word(lparam));
        return 1;
    }
    if (message == STARS_SLOT_WM_MEASURE_ITEM) {
        slot_action(input, STARS_SLOT_ACTION_MEASURE_ITEM, hwnd, 0x42);
        return 1;
    }
    if (message == STARS_SLOT_WM_INIT_DIALOG) {
        state->hwnd = hwnd;
        state->dialog_open = 1;
        state->mode = 0;
        state->starbase_mode = 0;
        state->hull_copy = 0;
        slot_action(input, STARS_SLOT_ACTION_INIT, hwnd, 0);
        slot_action(input, STARS_SLOT_ACTION_UPDATE_GLOBALS, 0, 0);
        slot_action(input, STARS_SLOT_ACTION_FILL_PARTS, state->part_mask, 0);
        slot_action(input, STARS_SLOT_ACTION_FILL_DESIGNS, 0, 0);
        slot_action(input, STARS_SLOT_ACTION_SET_SELECTION, 0, 0);
        return 1;
    }
    if (message == STARS_SLOT_WM_COMMAND) {
        control = slot_low_word(lparam);
        notify = slot_high_word(lparam);
        if (notify == STARS_SLOT_COMMAND_CLICK &&
            control >= STARS_SLOT_CONTROL_STARBASE && control <
            STARS_SLOT_CONTROL_STARBASE + 2U) {
            slot_select_starbase(input, control);
            return 0;
        }
        if (notify == STARS_SLOT_COMMAND_CLICK &&
            control >= STARS_SLOT_CONTROL_BUILD_MODE && control <=
            STARS_SLOT_CONTROL_BUILD_MODE_LAST) {
            slot_select_mode(input, control);
            return 0;
        }
        if (control == STARS_SLOT_CONTROL_EDIT &&
            notify == STARS_SLOT_COMMAND_EDIT) {
            slot_edit_change(input);
            return 0;
        }
        if (control == STARS_SLOT_CONTROL_DESIGNS &&
            notify == STARS_SLOT_COMMAND_CHANGE) {
            slot_refresh_selection(input);
            return 0;
        }
        if (control == STARS_SLOT_CONTROL_PARTS &&
            notify == STARS_SLOT_COMMAND_CHANGE) {
            slot_action(input, STARS_SLOT_ACTION_SET_SELECTION, -1, 0);
            return 0;
        }
        if (control == STARS_SLOT_CONTROL_DELETE &&
            notify == STARS_SLOT_COMMAND_CLICK) {
            slot_delete_design(input);
            return 0;
        }
        if (control == STARS_SLOT_CONTROL_COPY &&
            notify == STARS_SLOT_COMMAND_CLICK) {
            slot_copy_design(input);
            return 0;
        }
        if (control == STARS_SLOT_CONTROL_ACCEPT_EDIT &&
            notify == STARS_SLOT_COMMAND_CLICK) {
            slot_accept_design(input);
            return 0;
        }
        if ((wparam == 1 || wparam == 2) && notify == STARS_SLOT_COMMAND_CLICK) {
            slot_finish_edit(input, wparam);
            return 0;
        }
        if (wparam == 0x0076U && notify == STARS_SLOT_COMMAND_CLICK) {
            slot_action(input, STARS_SLOT_ACTION_HELP, state->mode,
                state->help_file);
            return 1;
        }
    }
    if (message == 0x0201U || message == 0x0203U || message == 0x0204U) {
        if (input->ops && input->ops->track_mouse) {
            result = input->ops->track_mouse(input->context, hwnd,
                (StarsI16)slot_low_word(lparam),
                (StarsI16)slot_high_word(lparam), wparam,
                message == 0x0204U);
            return result;
        }
    }
    if (input->ops && input->ops->default_proc)
        return input->ops->default_proc(input->context, hwnd, message,
            wparam, lparam);
    return 0;
}
