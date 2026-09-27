#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct KeepProbe {
    int calls;
    StarsI16 speed;
    StarsI16 threat_count;
} KeepProbe;

static StarsI16 is_transport(void *context, const StarsU8 *fleet)
{
    (void)context;
    return fleet[0] == 1U;
}

static void set_speed(void *context, const StarsU8 *fleet, StarsI16 speed,
    StarsI16 threat_count, const StarsU8 *const *threats)
{
    KeepProbe *probe = (KeepProbe *)context;
    (void)fleet;
    (void)threats;
    ++probe->calls; probe->speed = speed; probe->threat_count = threat_count;
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    StarsU16 bits = (StarsU16)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
}

int main(void)
{
    StarsU8 hostile[18];
    StarsU8 friendly_thing[18];
    StarsU8 fleet[124];
    StarsU8 short_fleet[124];
    const StarsU8 *things[2];
    const StarsU8 *fleets[2];
    StarsKeepFleetsMovingOps ops;
    KeepProbe probe;
    int failures = 0;

    memset(hostile, 0, sizeof(hostile));
    memset(friendly_thing, 0, sizeof(friendly_thing));
    memset(fleet, 0, sizeof(fleet));
    memset(short_fleet, 0, sizeof(short_fleet));
    put_word(hostile, 0, (StarsI16)(3 << 9));
    put_word(friendly_thing, 0, (StarsI16)(4 << 9));
    put_word(fleet, 2, 4); put_word(fleet, 98, 2); fleet[0] = 1;
    put_word(short_fleet, 2, 4); put_word(short_fleet, 98, 1);
    things[0] = hostile; things[1] = friendly_thing;
    fleets[0] = fleet; fleets[1] = short_fleet;
    probe.calls = 0; probe.speed = 0; probe.threat_count = 0;
    ops.is_transport = is_transport; ops.set_speed = set_speed;
    stars_keep_fleets_moving(things, 2, 4, 5, fleets, 2, &ops, &probe);
    if (probe.calls != 1 || probe.speed != 30 || probe.threat_count != 1)
        ++failures;
    printf("KEEP_FLEETS_MOVING_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
