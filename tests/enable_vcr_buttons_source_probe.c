#include "core.h"
#include <stdio.h>

typedef struct VcrState {
    int enabled;
    int focused;
    StarsU16 focus_control;
} VcrState;

static StarsU16 get_item(void *context, StarsU16 dialog, StarsU16 id)
{
    (void)context;
    return (StarsU16)(dialog + id);
}

static void enable(void *context, StarsU16 control, StarsU16 enabled)
{
    VcrState *state = (VcrState *)context;
    if (enabled) ++state->enabled;
    (void)control;
}

static void set_focus(void *context, StarsU16 control)
{
    VcrState *state = (VcrState *)context;
    ++state->focused;
    state->focus_control = control;
}

int main(void)
{
    StarsEnableVcrButtonOps ops;
    VcrState state;
    int failures = 0;

    ops.get_item = get_item;
    ops.enable = enable;
    ops.set_focus = set_focus;
    state.enabled = 0; state.focused = 0; state.focus_control = 0;
    stars_enable_vcr_buttons(10U, -1, 4, &ops, &state);
    if (state.enabled != 3 || state.focused != 1 ||
        state.focus_control != 0xadu)
        ++failures;

    state.enabled = 0; state.focused = 0; state.focus_control = 0;
    stars_enable_vcr_buttons(10U, 4, 4, &ops, &state);
    if (state.enabled != 2 || state.focused != 1 ||
        state.focus_control != 11U)
        ++failures;

    printf("ENABLE_VCR_BUTTONS_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
