#include "core.h"
#include <stdio.h>

typedef struct CargoState {
    StarsI32 available;
    int calls;
    StarsI16 kind[3];
    StarsI16 id[3];
    StarsI16 item[3];
    StarsI32 delta[3];
} CargoState;

static StarsI32 change_cargo(void *context, StarsI16 kind, StarsI16 id,
    StarsI16 item, StarsI32 delta)
{
    CargoState *state = (CargoState *)context;
    int slot = state->calls;
    if (slot < 3) {
        state->kind[slot] = kind; state->id[slot] = id;
        state->item[slot] = item; state->delta[slot] = delta;
    }
    ++state->calls;
    return delta == 0L ? state->available : 0L;
}

int main(void)
{
    CargoState state;
    StarsI16 result;
    int failures = 0;

    state.available = 3L; state.calls = 0;
    result = stars_xfer_ai_troopers(4, 9, 5, change_cargo, &state);
    if (result != 3 || state.calls != 3 || state.kind[0] != 2 ||
        state.id[0] != 4 || state.delta[0] != 0L || state.kind[1] != 2 ||
        state.id[1] != 4 || state.delta[1] != -3L || state.kind[2] != 1 ||
        state.id[2] != 9 || state.delta[2] != 3L)
        ++failures;

    state.available = 8L; state.calls = 0;
    result = stars_xfer_ai_troopers(4, 9, 5, change_cargo, &state);
    if (result != 5 || state.calls != 3 || state.delta[1] != -5L ||
        state.delta[2] != 5L)
        ++failures;
    state.calls = 0;
    result = stars_xfer_ai_troopers(4, 9, 0, change_cargo, &state);
    if (result != 0 || state.calls != 0)
        ++failures;
    printf("XFER_AI_TROOPERS_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
