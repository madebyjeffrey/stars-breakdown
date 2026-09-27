#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct CyberState {
    int changed;
    StarsI16 selection;
    int initialized;
    StarsI16 result_of;
    StarsI16 result_latest;
} CyberState;

static StarsI16 desirable(void *context, const StarsU8 *planet, StarsI16 player)
{ (void)context; (void)planet; (void)player; return 15; }

static void change_selection(void *context, StarsI16 kind, StarsI16 id)
{ CyberState *s = (CyberState *)context; s->changed = kind; s->selection = id; }

static void init_production(void *context, void *production)
{ CyberState *s = (CyberState *)context; (void)production; ++s->initialized; }

static StarsI16 latest_of(void *context)
{ CyberState *s = (CyberState *)context; return s->result_of; }

static StarsI16 latest(void *context)
{ CyberState *s = (CyberState *)context; return s->result_latest; }

int main(void)
{
    StarsU8 planet[64];
    CyberState state;
    StarsI16 result;
    int failures = 0;

    memset(planet, 0, sizeof(planet));
    planet[0] = 0x34; planet[1] = 0x12;
    planet[0x28] = 0xf4; planet[0x29] = 0x01;
    planet[9] = 15; planet[10] = 16; planet[11] = 16;
    state.changed = 0; state.selection = 0; state.initialized = 0;
    state.result_of = 6; state.result_latest = 7;
    result = stars_i_build_cyber_starbase(planet, 3, desirable,
        change_selection, init_production, latest_of, latest, &state);
    if (result != 6 || state.changed != 1 || state.selection != 0x1234 ||
        state.initialized != 1)
        ++failures;
    planet[9] = 16;
    result = stars_i_build_cyber_starbase(planet, 3, desirable,
        change_selection, init_production, latest_of, latest, &state);
    if (result != 7)
        ++failures;
    planet[4] = 0x00; planet[5] = 0x02;
    result = stars_i_build_cyber_starbase(planet, 3, desirable,
        change_selection, init_production, latest_of, latest, &state);
    if (result != -1)
        ++failures;
    printf("I_BUILD_CYBER_STARBASE_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
