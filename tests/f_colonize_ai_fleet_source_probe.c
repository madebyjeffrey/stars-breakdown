#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ColonizeState {
    int changed;
    StarsI16 selection;
    int moved;
    StarsI16 move_arg;
    StarsMoveOrder order;
} ColonizeState;

static void change_selection(void *context, StarsI16 kind, StarsI16 selection)
{
    ColonizeState *state = (ColonizeState *)context;
    state->changed = kind; state->selection = selection;
}

static StarsU16 find_warp(void *context, const StarsU8 *fleet, StarsI16 mode)
{
    (void)context; (void)fleet; (void)mode;
    return 0x0017U;
}

static StarsI16 move_fleet(void *context, const StarsU8 *fleet,
    const StarsMoveOrder *order, StarsI16 arg)
{
    ColonizeState *state = (ColonizeState *)context;
    (void)fleet;
    ++state->moved; state->move_arg = arg;
    state->order = *order;
    return 1;
}

int main(void)
{
    StarsU8 fleet[8];
    StarsPoint planets[3];
    ColonizeState state;
    StarsI16 result;
    int failures = 0;

    memset(fleet, 0, sizeof(fleet));
    fleet[0] = 0x34; fleet[1] = 0x12;
    planets[2].x = 300; planets[2].y = -40;
    memset(&state, 0, sizeof(state));
    result = stars_f_colonize_ai_fleet(fleet, planets, 2, change_selection,
        find_warp, move_fleet, &state);
    if (result != 1 || state.changed != 2 || state.selection != 0x1234 ||
        state.moved != 1 || state.move_arg != 0 ||
        state.order.point.x != 300 || state.order.point.y != -40 ||
        state.order.flags != 0x1172U ||
        (fleet[5] & 0x80U) == 0U)
        ++failures;
    printf("F_COLONIZE_AI_FLEET_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
