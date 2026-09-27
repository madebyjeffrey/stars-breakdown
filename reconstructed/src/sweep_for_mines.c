#include "core.h"

#define SWEEP_THING_KIND 0
#define SWEEP_FLEET_HIDDEN 0x0400
#define SWEEP_MESSAGE_FLEET 0xc2
#define SWEEP_MESSAGE_PLANET 0xf4
#define SWEEP_MESSAGE_TARGET 0xbe

static StarsI32 sweep_amount(StarsI32 power, StarsI16 mine_type,
    StarsI32 remaining, StarsI32 distance)
{
    StarsI32 amount;
    StarsI32 safe_amount;

    amount = power;
    if (mine_type == 2)
        amount /= 3L;
    if (amount < 2L)
        amount = 2L;
    safe_amount = remaining - distance + 1L;
    if (safe_amount < amount)
        amount = safe_amount;
    if (amount > remaining)
        amount = remaining;
    return amount;
}

static void sweep_mine(StarsSweepMinesInput *input,
    StarsSweepMinesThing *thing, StarsI16 sweeper, StarsI16 source_id,
    StarsI16 source_message, StarsI32 power, StarsI32 distance)
{
    StarsI32 amount;
    if (!input->ops.send_message || power <= 0L ||
            !input->ops.can_attack(input->context, sweeper, thing->owner))
        return;
    amount = sweep_amount(power, thing->mine_type,
        thing->remaining_strength, distance);
    if (amount <= 0L)
        return;
    input->ops.send_message(input->context, sweeper, source_message,
        source_id, amount, thing->mine_type);
    input->ops.send_message(input->context, thing->owner,
        SWEEP_MESSAGE_TARGET, source_id, amount, thing->mine_type);
    thing->remaining_strength -= amount;
    if (thing->remaining_strength <= 0L) {
        input->ops.free_thing(input->context, thing);
        return;
    }
    thing->owner_mask |= (StarsU32)1UL << (sweeper & 31);
}

static StarsI32 sweep_distance(StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    StarsI32 dx;
    StarsI32 dy;
    dx = (StarsI32)x1 - x2;
    dy = (StarsI32)y1 - y2;
    return dx * dx + dy * dy;
}

/* NE 24:76A4-7E4E. Apply fleet and planetary mine-sweeper power to enemy
 * minefields within range. Type-2 minefields absorb one third of the power,
 * every sweep removes at least two strength units, and the original leaves
 * one unit of clearance from the minefield's detection radius. */
void stars_sweep_for_mines(StarsSweepMinesInput *input)
{
    StarsU16 fleet_index;
    StarsU16 planet_index;
    StarsU16 thing_index;
    StarsI32 power;
    StarsI32 distance;
    StarsSweepMinesFleet *fleet;
    StarsSweepMinesPlanet *planet;
    StarsSweepMinesThing *thing;

    if (!input || !input->things || !input->ops.can_attack ||
            !input->ops.free_thing)
        return;
    for (fleet_index = 0; fleet_index < input->fleet_count; ++fleet_index) {
        fleet = &input->fleets[fleet_index];
        power = input->ops.fleet_power ?
            input->ops.fleet_power(input->context, fleet) : 0L;
        if (power <= 0L || (fleet->status_flags & SWEEP_FLEET_HIDDEN) != 0)
            continue;
        for (thing_index = 0; thing_index < input->thing_count;
                ++thing_index) {
            thing = &input->things[thing_index];
            if (!thing->active || thing->kind != SWEEP_THING_KIND ||
                    thing->owner == fleet->player)
                continue;
            if (!input->ops.can_attack(input->context, fleet->player,
                    thing->owner))
                continue;
            distance = sweep_distance(fleet->x, fleet->y, thing->x, thing->y);
            if (distance > thing->detection_radius)
                continue;
            sweep_mine(input, thing, fleet->player, fleet->id,
                SWEEP_MESSAGE_FLEET, power, distance);
        }
    }
    for (planet_index = 0; planet_index < input->planet_count; ++planet_index) {
        planet = &input->planets[planet_index];
        if (planet->player == -1 || !input->ops.planet_power)
            continue;
        power = input->ops.planet_power(input->context, planet);
        if (power <= 0L)
            continue;
        for (thing_index = 0; thing_index < input->thing_count;
                ++thing_index) {
            thing = &input->things[thing_index];
            if (!thing->active || thing->kind != SWEEP_THING_KIND ||
                    thing->owner == planet->player ||
                    !input->ops.can_attack(input->context, planet->player,
                        thing->owner))
                continue;
            distance = sweep_distance(planet->x, planet->y,
                thing->x, thing->y);
            if (distance > thing->detection_radius)
                continue;
            sweep_mine(input, thing, planet->player, planet->id,
                SWEEP_MESSAGE_PLANET, power, distance);
        }
    }
}
