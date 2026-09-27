#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct DestroyState {
    int ships;
    int packets;
    const StarsU8 *planet;
} DestroyState;

static void kill_ships(void *context, const StarsU8 *planet)
{
    DestroyState *state = (DestroyState *)context;
    ++state->ships;
    state->planet = planet;
}

static void kill_packets(void *context, const StarsU8 *planet)
{
    DestroyState *state = (DestroyState *)context;
    ++state->packets;
    state->planet = planet;
}

static void put_word(StarsU8 *p, StarsU16 value)
{
    p[0] = (StarsU8)value; p[1] = (StarsU8)(value >> 8);
}

int main(void)
{
    StarsU8 planets[3 * 0x38];
    StarsDestroyIshdefSbOps ops;
    DestroyState state;
    int failures = 0;

    memset(planets, 0, sizeof(planets));
    put_word(planets + 2, 4U); put_word(planets + 4, 0x0200U);
    put_word(planets + 0x2c, 7U);
    put_word(planets + 0x38 + 2, 4U);
    put_word(planets + 0x38 + 4, 0x0200U);
    put_word(planets + 0x38 + 0x2c, 8U);
    put_word(planets + 2 * 0x38 + 2, 5U);
    put_word(planets + 2 * 0x38 + 4, 0x0200U);
    put_word(planets + 2 * 0x38 + 0x2c, 7U);
    ops.kill_ships = kill_ships;
    ops.kill_mass_packets = kill_packets;
    memset(&state, 0, sizeof(state));
    stars_destroy_all_ishdef_sb(planets, 3U, 7, 4, &ops, &state);
    if (state.ships != 1 || state.packets != 1 ||
        state.planet != planets ||
        (planets[4] & 0x02U) != 0U ||
        (planets[0x38 + 4] & 0x02U) != 0U ||
        (planets[2 * 0x38 + 4] & 0x02U) != 0U)
        ++failures;
    printf("DESTROY_ALL_ISHDEF_SB_CHECKS=3 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
