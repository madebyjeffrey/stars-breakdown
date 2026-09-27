#include "core.h"
#include <stdio.h>

typedef struct MineProbe {
    int calls;
    const StarsU8 *base;
    StarsI16 last_player;
    StarsI16 last_flag;
} MineProbe;

static void estimate(void *context, const StarsU8 *planet, StarsI32 *scratch,
    StarsI16 player, StarsI16 flag)
{
    MineProbe *probe = (MineProbe *)context;
    ++probe->calls;
    probe->base = planet;
    probe->last_player = player;
    probe->last_flag = flag;
    scratch[0] = 1;
    scratch[1] = 2;
    scratch[2] = 3;
}

int main(void)
{
    StarsU8 planets[168];
    MineProbe probe;
    int failures = 0;
    probe.calls = 0;
    probe.base = 0;
    probe.last_player = 0;
    probe.last_flag = 0;
    stars_mine_minerals(planets, 3, estimate, &probe);
    if (probe.calls != 3 || probe.base != planets + 112 ||
        probe.last_player != -1 || probe.last_flag != 1)
        ++failures;
    stars_mine_minerals(planets, 0, estimate, &probe);
    if (probe.calls != 3)
        ++failures;
    printf("MINE_MINERALS_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
