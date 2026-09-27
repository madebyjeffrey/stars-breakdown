#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct FuelTest { StarsI32 capacity; int friendly; int queries; } FuelTest;
static StarsI32 capacity(void *c, const StarsFuelFleet *f)
{ FuelTest *t = (FuelTest *)c; (void)f; ++t->queries; return t->capacity; }
static StarsI16 can_refuel(void *c, const StarsFuelFleet *f)
{ (void)f; return (StarsI16)((FuelTest *)c)->friendly; }
static void set_fuel(void *c, StarsFuelFleet *f, StarsI32 value)
{ (void)c; f->fuel = value; }
int main(void)
{
    StarsFuelFleet fleet;
    StarsFuelInput input;
    FuelTest test;
    memset(&fleet, 0, sizeof fleet); memset(&input, 0, sizeof input);
    memset(&test, 0, sizeof test);
    test.capacity = 500L;
    fleet.fuel = 10L; fleet.base_fuel = 10L; fleet.ship_counts[0] = 1;
    fleet.destination = -1;
    fleet.designs[0].fuel_component = 1L;
    fleet.designs[0].hull_kind = 0x19;
    input.fleets = &fleet; input.fleet_count = 1; input.context = &test;
    input.ops.current_fuel = capacity; input.ops.can_refuel = can_refuel;
    input.ops.set_fuel = set_fuel;
    stars_fuel_fleets(&input);
    if (fleet.fuel != 260L || test.queries != 1) return 1;
    fleet.ship_counts[0] = 3; stars_fuel_fleets(&input);
    if (fleet.fuel != 500L) return 2;
    fleet.designs[0].hull_kind = 0x1a; fleet.ship_counts[0] = 1;
    fleet.designs[0].fuel_component = 0; stars_fuel_fleets(&input);
    if (fleet.fuel != 210L) return 3;
    fleet.designs[0].hull_kind = 0; fleet.designs[0].fuel_component = 2;
    stars_fuel_fleets(&input);
    if (fleet.fuel != 110L) return 4;
    fleet.designs[0].fuel_component = 0; test.queries = 0;
    stars_fuel_fleets(&input);
    if (fleet.fuel != 110L || test.queries) return 5;
    fleet.destination = 3; test.friendly = 1; stars_fuel_fleets(&input);
    if (fleet.fuel != 500L) return 6;
    fleet.fuel = 10L; fleet.status_flags = 0x0400U; test.queries = 0;
    stars_fuel_fleets(&input);
    if (fleet.fuel != 10L || test.queries) return 7;
    fleet.status_flags = 7U; test.friendly = 0;
    stars_fuel_fleets(&input);
    if (fleet.fuel != 10L || test.queries) return 8;
    puts("FUEL_FLEETS_SOURCE=PASS");
    return 0;
}
