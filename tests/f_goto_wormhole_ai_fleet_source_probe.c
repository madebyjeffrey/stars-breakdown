#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct WormholeState {
    int changed;
    StarsI16 selection;
    StarsMoveOrder order;
} WormholeState;

static void change_selection(void *context, StarsI16 kind, StarsI16 selection)
{
    WormholeState *state = (WormholeState *)context;
    state->changed = kind; state->selection = selection;
}

static StarsU16 find_warp(void *context, const StarsU8 *fleet, StarsI16 mode)
{
    (void)context; (void)fleet; (void)mode;
    return 0x0005U;
}

static StarsI16 move_fleet(void *context, const StarsU8 *fleet,
    const StarsMoveOrder *order, StarsI16 arg)
{
    WormholeState *state = (WormholeState *)context;
    (void)fleet; (void)arg;
    state->order = *order;
    return 1;
}

int main(void)
{
    StarsU8 fleet[8];
    StarsU8 wormhole[8];
    WormholeState state;
    StarsI16 result;
    int failures = 0;

    memset(fleet, 0, sizeof(fleet));
    memset(wormhole, 0, sizeof(wormhole));
    fleet[0] = 0x78; fleet[1] = 0x56;
    wormhole[0] = 0x34; wormhole[1] = 0x12;
    wormhole[2] = 0x2c; wormhole[3] = 0x01;
    wormhole[4] = 0xd8; wormhole[5] = 0xff;
    memset(&state, 0, sizeof(state));
    result = stars_f_goto_wormhole_ai_fleet(fleet, wormhole,
        change_selection, find_warp, move_fleet, &state);
    if (result != 1 || state.changed != 2 || state.selection != 0x5678 ||
        state.order.id != 0x1234 || state.order.point.x != 300 ||
        state.order.point.y != -40 || state.order.flags != 0x0850U)
        ++failures;
    printf("F_GOTO_WORMHOLE_AI_FLEET_CHECKS=1 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
