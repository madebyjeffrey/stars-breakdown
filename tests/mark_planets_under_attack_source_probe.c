#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct AttackProbe {
    StarsU8 *planet_one;
    StarsU8 *planet_two;
    int marked;
    StarsI16 marked_id;
} AttackProbe;

static StarsI16 hull_category(void *context, StarsI16 player,
    StarsU16 design)
{
    (void)context;
    (void)player;
    return design == 1U ? 16 : 15;
}

static const StarsU8 *lookup_planet(void *context, StarsI16 id)
{
    AttackProbe *probe = (AttackProbe *)context;
    if (id == 1) return probe->planet_one;
    if (id == 2) return probe->planet_two;
    return 0;
}

static void mark_planet(void *context, StarsI16 id)
{
    AttackProbe *probe = (AttackProbe *)context;
    ++probe->marked;
    probe->marked_id = id;
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsI16 value)
{
    StarsU16 bits = (StarsU16)value;
    bytes[offset] = (StarsU8)bits;
    bytes[offset + 1U] = (StarsU8)(bits >> 8);
}

int main(void)
{
    StarsU8 own_fleet[124];
    StarsU8 attacking_fleet[124];
    StarsU8 harmless_fleet[124];
    StarsU8 no_target_fleet[124];
    StarsU8 planet_one[56];
    StarsU8 planet_two[56];
    const StarsU8 *fleets[5];
    StarsMarkPlanetsUnderAttackOps ops;
    AttackProbe probe;
    int failures = 0;

    memset(own_fleet, 0, sizeof(own_fleet));
    memset(attacking_fleet, 0, sizeof(attacking_fleet));
    memset(harmless_fleet, 0, sizeof(harmless_fleet));
    memset(no_target_fleet, 0, sizeof(no_target_fleet));
    memset(planet_one, 0, sizeof(planet_one));
    memset(planet_two, 0, sizeof(planet_two));
    put_word(own_fleet, 2, 4); put_word(own_fleet, 6, 1);
    put_word(own_fleet, 12, 1);
    put_word(attacking_fleet, 2, 3); put_word(attacking_fleet, 6, 1);
    put_word(attacking_fleet, 14, 1);
    put_word(harmless_fleet, 2, 3); put_word(harmless_fleet, 6, 2);
    put_word(harmless_fleet, 12, 1);
    put_word(no_target_fleet, 2, 3); put_word(no_target_fleet, 6, -1);
    put_word(no_target_fleet, 14, 1);
    put_word(planet_one, 0, 7); put_word(planet_one, 2, 4);
    put_word(planet_two, 0, 8); put_word(planet_two, 2, 5);
    fleets[0] = own_fleet; fleets[1] = attacking_fleet;
    fleets[2] = harmless_fleet; fleets[3] = no_target_fleet; fleets[4] = 0;
    probe.planet_one = planet_one; probe.planet_two = planet_two;
    probe.marked = 0; probe.marked_id = -1;
    ops.hull_category = hull_category;
    ops.lookup_planet = lookup_planet;
    ops.mark_planet = mark_planet;
    stars_mark_planets_under_attack(fleets, 5, 4, &ops, &probe);
    if (probe.marked != 1 || probe.marked_id != 7)
        ++failures;
    printf("MARK_PLANETS_UNDER_ATTACK_CHECKS=1 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
