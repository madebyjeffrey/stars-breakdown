#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct FixState {
    const StarsU8 *first;
    const StarsU8 *production;
    int calls;
} FixState;

static void quick_build(void *context, const StarsU8 *planet, void *production)
{
    FixState *state = (FixState *)context;
    ++state->calls;
    if (state->calls == 1)
        state->first = planet;
    state->production = (const StarsU8 *)production;
}

int main(void)
{
    StarsU8 planet_a[2];
    StarsU8 planet_b[2];
    StarsU8 ai_records[0x80];
    StarsU8 production[3];
    const StarsU8 *planets[4];
    FixState state;
    int failures = 0;

    memset(ai_records, 0, sizeof ai_records);
    memset(&state, 0, sizeof state);
    planet_a[0] = 3;
    planet_a[1] = 0;
    planet_b[0] = 7;
    planet_b[1] = 0;
    ai_records[3 * 0x10 + 8] = 1;
    planets[0] = planet_a;
    planets[1] = planet_b;
    planets[2] = 0;
    planets[3] = planet_a;
    stars_fix_planets_under_attack(planets, 4, ai_records, production,
        quick_build, &state);
    if (state.calls != 1 || state.first != planet_a ||
        state.production != production)
        ++failures;
    ai_records[7 * 0x10 + 8] = 1;
    stars_fix_planets_under_attack(planets, 2, ai_records, production,
        quick_build, &state);
    if (state.calls != 3)
        ++failures;
    printf("FIX_PLANETS_ATTACK_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
