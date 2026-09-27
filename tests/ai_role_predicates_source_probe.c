#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct RoleState {
    StarsI16 weight[16];
    StarsI16 special[16];
} RoleState;

static StarsI16 weight(void *context, StarsU16 player, StarsU16 index)
{
    RoleState *state = (RoleState *)context;
    (void)player;
    return state->weight[index];
}

static StarsI16 special(void *context, StarsU16 player, StarsU16 index)
{
    RoleState *state = (RoleState *)context;
    (void)player;
    return state->special[index];
}

int main(void)
{
    StarsI16 counts[16];
    StarsI16 kinds[16];
    RoleState state;
    int failures = 0;

    memset(counts, 0, sizeof counts);
    memset(kinds, 0, sizeof kinds);
    memset(&state, 0, sizeof state);
    counts[0] = 1;
    kinds[0] = 2;
    if (!stars_f_is_ai_transport(counts, kinds, 16U, 1U, weight,
            special, &state) || stars_f_is_ai_attack(counts, kinds, 16U,
            1U, weight, special, &state))
        ++failures;

    memset(counts, 0, sizeof counts);
    kinds[3] = 8;
    counts[3] = 1;
    if (stars_f_is_ai_transport(counts, kinds, 16U, 1U, weight,
            special, &state) != 0 ||
        stars_f_is_ai_attack(counts, kinds, 16U, 1U, weight, special,
            &state) != 1)
        ++failures;

    memset(counts, 0, sizeof counts);
    kinds[5] = 31;
    counts[5] = 1;
    state.weight[5] = 500;
    state.special[5] = 1;
    if (stars_f_is_ai_transport(counts, kinds, 16U, 1U, weight,
            special, &state) != 1 ||
        stars_f_is_ai_attack(counts, kinds, 16U, 1U, weight, special,
            &state) != 0)
        ++failures;

    state.weight[5] = 499;
    if (stars_f_is_ai_transport(counts, kinds, 16U, 1U, weight,
            special, &state) != 0 ||
        stars_f_is_ai_attack(counts, kinds, 16U, 1U, weight, special,
            &state) != 1)
        ++failures;

    printf("AI_ROLE_PREDICATE_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
