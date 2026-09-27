#include "core.h"

#define FUEL_FLEET_DEAD 0x0400
#define FUEL_HULL_TANK 0x19
#define FUEL_HULL_TANK_LARGE 0x1a

/* NE 23:2EFA-32CE. Refill a fleet at a qualifying friendly starbase;
 * otherwise add Anti-Matter Generator and fuel-transport production to
 * current fuel, capped at the source fleet capacity. */
void stars_fuel_fleets(StarsFuelInput *input)
{
    StarsU16 fleet_index;
    StarsU16 ship_index;
    StarsFuelFleet *fleet;
    StarsI32 capacity;
    StarsI32 generator_count;
    StarsI32 tank_fuel;
    StarsI32 ceiling;

    if (!input || !input->fleets || !input->ops.current_fuel ||
            !input->ops.set_fuel)
        return;
    for (fleet_index = 0; fleet_index < input->fleet_count; ++fleet_index) {
        fleet = &input->fleets[fleet_index];
        if ((fleet->status_flags & FUEL_FLEET_DEAD) != 0)
            continue;
        if (fleet->destination != -1 && input->ops.can_refuel &&
                input->ops.can_refuel(input->context, fleet)) {
            input->ops.set_fuel(input->context, fleet,
                input->ops.current_fuel(input->context, fleet));
            continue;
        }
        generator_count = 0L;
        tank_fuel = 0L;
        for (ship_index = 0; ship_index < 16; ++ship_index) {
            if (fleet->ship_counts[ship_index] == 0)
                continue;
            generator_count += (StarsI32)fleet->ship_counts[ship_index] *
                fleet->designs[ship_index].fuel_component;
            if (fleet->designs[ship_index].hull_kind == FUEL_HULL_TANK ||
                    fleet->designs[ship_index].hull_kind == FUEL_HULL_TANK_LARGE)
                tank_fuel += (StarsI32)fleet->ship_counts[ship_index] * 200L;
        }
        if (generator_count == 0L && tank_fuel == 0L)
            continue;
        ceiling = generator_count * 50L + tank_fuel + fleet->base_fuel;
        capacity = input->ops.current_fuel(input->context, fleet);
        if (capacity < ceiling)
            ceiling = capacity;
        input->ops.set_fuel(input->context, fleet, ceiling);
    }
}
