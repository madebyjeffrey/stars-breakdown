#include "core.h"
#include <stdio.h>

typedef struct ToolbarState {
    int selected;
    int realized;
    int drawn;
    StarsPoint last_point;
    StarsI16 last_button;
    StarsI16 last_down;
} ToolbarState;

static void select_palette(void *context, StarsU16 hdc, StarsU16 palette)
{
    ToolbarState *state = (ToolbarState *)context;
    if (hdc == 7U && palette == 9U) ++state->selected;
}

static void realize_palette(void *context, StarsU16 hdc)
{
    ToolbarState *state = (ToolbarState *)context;
    if (hdc == 7U) ++state->realized;
}

static StarsI16 button_down(void *context, StarsI16 button)
{
    (void)context;
    return (StarsI16)(button == 3);
}

static void draw_button(void *context, StarsU16 hdc, StarsPoint point,
    StarsI16 button, StarsI16 down)
{
    ToolbarState *state = (ToolbarState *)context;
    if (hdc == 7U) {
        ++state->drawn;
        state->last_point = point;
        state->last_button = button;
        state->last_down = down;
    }
}

int main(void)
{
    static const StarsI16 buttons[] = {-1, 2, 3, -2, 4};
    StarsDrawToolbarOps ops;
    ToolbarState state;
    int failures = 0;

    state.selected = 0; state.realized = 0; state.drawn = 0;
    state.last_point.x = 0; state.last_point.y = 0;
    state.last_button = 0; state.last_down = 0;
    ops.select_palette = select_palette;
    ops.realize_palette = realize_palette;
    ops.button_down = button_down;
    ops.draw_button = draw_button;
    stars_draw_toolbar(7U, 9U, buttons, 5U, 15, &ops, &state);
    if (state.selected != 1 || state.realized != 1 || state.drawn != 3 ||
        state.last_button != 4 || state.last_down != 0 ||
        state.last_point.x != 70 || state.last_point.y != 4)
        ++failures;

    printf("DRAW_TOOLBAR_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
