#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI16 race(void *p, StarsI16 owner, StarsI16 stat)
{ (void)owner; (void)stat; return *(StarsI16 *)p; }

int main(void)
{
    StarsHealFleet fleet;
    StarsHealPlanet planet;
    StarsHealInput input;
    StarsI16 ability;
    unsigned checks;

    memset(&fleet, 0, sizeof fleet); memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input);
    ability = 0; checks = 0;
    fleet.player = 2; fleet.destination = 3;
    fleet.ship_presence[0] = 1;
    planet.id = 3; planet.owner = 2;
    input.fleets = &fleet; input.fleet_count = 1; input.planets = &planet;
    input.planet_count = 1; input.race_stat = race; input.context = &ability;
#define REPAIR(expected) do { \
    fleet.packed_ship_condition[0] = (200U << 7) | 7U; \
    stars_heal_ships(&input); \
    ++checks; \
    if (fleet.packed_ship_condition[0] != \
            (((200U - (expected)) << 7) | 7U)) return (int)checks; \
} while (0)
    REPAIR(5); /* A fleet that moved receives the minimum repair. */
    fleet.status_flags = 0x4000U; REPAIR(0);
    fleet.status_flags = 0x0400U; REPAIR(0);
    fleet.status_flags = 0x2000U; fleet.destination = -1; REPAIR(10);
    fleet.destination = 3; planet.owner = -1; REPAIR(15);
    planet.owner = 1; REPAIR(15);
    planet.owner = 2; REPAIR(25);
    planet.starbase_present = 1; REPAIR(40);
    planet.starbase_has_cargo = 1; REPAIR(100);
    planet.starbase_blocked = 1; REPAIR(25);
    ability = 4; REPAIR(50);
    fleet.hull_kind[0] = 25; REPAIR(75);
    fleet.ship_presence[1] = 1; fleet.hull_kind[1] = 26; REPAIR(100);
    fleet.hull_kind[0] = 26; fleet.hull_kind[1] = 25; REPAIR(100);
    fleet.ship_presence[0] = 0; fleet.ship_presence[1] = 0; REPAIR(50);
    fleet.packed_ship_condition[0] = (50U << 7) | 7U;
    stars_heal_ships(&input);
    if (fleet.packed_ship_condition[0] != 0) return 16;
    fleet.packed_ship_condition[0] = 7U;
    stars_heal_ships(&input);
    if (fleet.packed_ship_condition[0] != 0) return 17;
    planet.starbase_damage = 100; planet.starbase_blocked = 0;
    planet.starbase_present = 0; stars_heal_ships(&input);
    if (planet.starbase_damage != 100) return 18;
    planet.starbase_present = 1; planet.starbase_blocked = 1;
    stars_heal_ships(&input);
    if (planet.starbase_damage != 100) return 19;
    planet.starbase_blocked = 0; stars_heal_ships(&input);
    if (planet.starbase_damage != 25) return 20;
    stars_heal_ships(&input);
    if (planet.starbase_damage != 0) return 21;
    ability = 0; planet.starbase_damage = 50; stars_heal_ships(&input);
    if (planet.starbase_damage != 0) return 22;
    puts("HEAL_SHIPS_SOURCE=PASS");
    return 0;
}
