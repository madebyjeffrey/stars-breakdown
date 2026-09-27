#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct LookupState {
    StarsU8 normal[0x93 * 4];
    StarsU8 alternate[0x93 * 4];
    StarsU16 player;
    StarsU16 index;
    StarsU16 alternate_table;
    StarsU16 selector;
    int calls;
} LookupState;

static const StarsU8 *lookup(void *context, StarsU16 player,
    StarsU16 index, StarsU16 alternate, StarsU16 *selector)
{
    LookupState *state = (LookupState *)context;
    state->player = player;
    state->index = index;
    state->alternate_table = alternate;
    state->selector = alternate ? 0x2468U : 0x1357U;
    *selector = state->selector;
    ++state->calls;
    return (alternate ? state->alternate : state->normal) + index * 0x93U;
}

int main(void)
{
    StarsToken token;
    LookupState state;
    const StarsU8 *result;
    int failures = 0;

    memset(&token, 0, sizeof token);
    memset(&state, 0, sizeof state);
    token.raw[2] = 3;
    token.raw[4] = 5;
    result = stars_lpshdef_from_tok(&token, lookup, &state);
    if (result != state.normal + 5 * 0x93U || state.player != 3U ||
        state.index != 5U || state.alternate_table != 0U ||
        state.selector != 0x1357U || state.calls != 1)
        ++failures;

    token.raw[2] = 7;
    token.raw[4] = 0x12U;
    result = stars_lpshdef_from_tok(&token, lookup, &state);
    if (result != state.alternate + 2 * 0x93U || state.player != 7U ||
        state.index != 2U || state.alternate_table != 1U ||
        state.selector != 0x2468U || state.calls != 2)
        ++failures;

    if (sizeof token != 29U)
        ++failures;
    printf("LPSHDEF_FROM_TOK_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
