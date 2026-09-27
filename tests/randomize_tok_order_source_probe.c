#include "core.h"
#include <stdio.h>

typedef struct RandomState {
    StarsI16 values[4];
    StarsI16 counts[4];
    int calls;
} RandomState;

static StarsI16 random_value(void *context, StarsI16 count)
{
    RandomState *state = (RandomState *)context;
    state->counts[state->calls] = count;
    return state->values[state->calls++];
}

int main(void)
{
    StarsToken tokens[4];
    RandomState state;
    int i;
    int failures = 0;

    for (i = 0; i < 4; ++i) {
        int j;
        for (j = 0; j < 29; ++j) tokens[i].raw[j] = (StarsU8)(i + 10);
    }
    state.values[0] = 3; state.values[1] = 0;
    state.values[2] = 0; state.values[3] = 0; state.calls = 0;
    stars_randomize_tok_order(tokens, 4U, random_value, &state);
    if (state.calls != 4 || state.counts[0] != 4 || state.counts[1] != 3 ||
        state.counts[2] != 2 || state.counts[3] != 1 ||
        tokens[0].raw[0] != 13U || tokens[1].raw[0] != 11U ||
        tokens[2].raw[0] != 12U || tokens[3].raw[0] != 10U ||
        tokens[0].raw[28] != 13U || tokens[3].raw[28] != 10U)
        ++failures;
    printf("RANDOMIZE_TOK_ORDER_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
