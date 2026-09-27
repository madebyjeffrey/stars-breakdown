#include "core.h"
#include <stdio.h>

static StarsI16 race_stat(void *context, StarsI16 owner)
{
    (void)context;
    return owner == 3 ? 8 : 4;
}

static StarsI16 warp(void *context, const StarsPacketAttackPlanet *planet,
    StarsI16 *gate)
{
    (void)context;
    *gate = planet->id == 1 ? 1 : 0;
    return planet->id == 1 ? 2 : 4;
}

static double distance_between(void *context, StarsU16 source, StarsU16 test)
{
    (void)context;
    return source == 0 && test == 1 ? 10.0 : 1000.0;
}

int main(void)
{
    StarsPacketAttackPlanet source = { 0, 1, 0, 0, 0 };
    StarsPacketAttackPlanet target = { 1, 2, 0, 0, 0x0010 };
    StarsU16 flags[2] = { 0, 0 };
    StarsFEnumPktAttackInput input;
    int failures = 0;

    input.source = &source;
    input.test = &target;
    input.player = 1;
    input.ai_planet_flags = flags;
    input.planet_count = 2;
    input.ai_packet_weight = 1;
    input.warp_distance_scale = 100.0;
    input.gated_distance_exponent = 1.0;
    input.ordinary_distance_exponent = 1.0;
    input.score_divisor = 1.0;
    input.race_stat = race_stat;
    input.warp = warp;
    input.distance = distance_between;
    input.context = 0;
    if (stars_f_enum_pkt_attack(&input) != 1)
        ++failures;

    target.owner = 3;
    if (stars_f_enum_pkt_attack(&input) != 0)
        ++failures;
    printf("F_ENUM_PKT_ATTACK_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
