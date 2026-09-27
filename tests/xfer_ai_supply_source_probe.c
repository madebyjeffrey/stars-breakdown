#include "core.h"
#include <stdio.h>

typedef struct SupplyState {
    StarsI32 available;
    StarsI32 accepted;
    int calls;
    StarsI16 group[3];
    StarsI16 id[3];
    StarsI16 supply[3];
    StarsI32 delta[3];
} SupplyState;

static StarsI32 change_cargo(void *context, StarsI16 group, StarsI16 id,
    StarsI16 supply, StarsI32 delta)
{
    SupplyState *state = (SupplyState *)context;
    int slot = state->calls++;
    if (slot < 3) {
        state->group[slot] = group; state->id[slot] = id;
        state->supply[slot] = supply; state->delta[slot] = delta;
    }
    if (delta == 0L) return state->available;
    return state->accepted;
}

int main(void)
{
    SupplyState state;
    StarsI16 result;
    int failures = 0;

    state.available = 8L; state.accepted = 3L; state.calls = 0;
    result = stars_xfer_ai_supply(1, 10, 2, 20, 4, 5, change_cargo, &state);
    if (result != 3 || state.calls != 3 || state.group[0] != 1 ||
        state.id[0] != 10 || state.delta[0] != 0L || state.group[1] != 2 ||
        state.id[1] != 20 || state.delta[1] != 5L || state.group[2] != 1 ||
        state.id[2] != 10 || state.delta[2] != -3L)
        ++failures;

    state.available = 2L; state.accepted = 2L; state.calls = 0;
    result = stars_xfer_ai_supply(1, 10, 2, 20, 4, -5, change_cargo, &state);
    if (result != 2 || state.calls != 3 || state.group[0] != 2 ||
        state.id[0] != 20 || state.delta[0] != 0L || state.group[1] != 1 ||
        state.id[1] != 10 || state.delta[1] != 2L || state.group[2] != 2 ||
        state.id[2] != 20 || state.delta[2] != -2L)
        ++failures;
    printf("XFER_AI_SUPPLY_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
