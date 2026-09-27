#include "core.h"

#define HEAL_STATUS_DEAD 0x0400
#define HEAL_STATUS_NO_HEAL 0x4000
#define HEAL_STATUS_HERE_ALL_TURN 0x2000
#define HEAL_RACE_STAT 0x000e
#define HEAL_STARBASE_RATE 50
#define HEAL_STARBASE_BONUS 75

static StarsI16 heal_rate(const StarsHealInput *input,
    const StarsHealFleet *fleet)
{
    StarsI16 rate;
    StarsI16 bonus;
    StarsU16 index;
    const StarsHealPlanet *planet;

    rate = 5;
    if ((fleet->status_flags & HEAL_STATUS_HERE_ALL_TURN) != 0) {
        rate = fleet->destination < 0 ? 10 : 15;
        for (index = 0; fleet->destination >= 0 &&
                index < input->planet_count; ++index) {
            planet = &input->planets[index];
            if (planet->id != fleet->destination)
                continue;
            if (planet->owner == fleet->player) {
                rate = 25;
                if (planet->starbase_present && !planet->starbase_blocked)
                    rate = planet->starbase_has_cargo ? 100 : 40;
            }
            break;
        }
    }
    if (input->race_stat && input->race_stat(input->context,
            fleet->player, HEAL_RACE_STAT) == 4)
        rate = (StarsI16)(rate * 2);
    /* Repair ships add a fleet-wide bonus after the race multiplier. */
    bonus = 0;
    for (index = 0; index < 16; ++index) {
        if (!fleet->ship_presence[index])
            continue;
        if (fleet->hull_kind[index] == 26)
            bonus = 50;
        else if (bonus < 5 && fleet->hull_kind[index] == 25)
            bonus = 25;
    }
    return (StarsI16)(rate + bonus);
}

void stars_heal_ships(StarsHealInput *input)
{
    StarsHealFleet *fleet;
    StarsHealPlanet *planet;
    StarsI16 index;
    StarsI16 ship;
    StarsI16 rate;
    StarsU16 condition;

    if (!input)
        return;
    for (index = 0; index < (StarsI16)input->fleet_count; ++index) {
        fleet = &input->fleets[index];
        if ((fleet->status_flags & (HEAL_STATUS_DEAD | HEAL_STATUS_NO_HEAL)) != 0)
            continue;
        rate = heal_rate(input, fleet);
        for (ship = 0; ship < 16; ++ship) {
            condition = fleet->packed_ship_condition[ship];
            if (condition == 0)
                continue;
            if ((StarsU16)rate >= (condition >> 7))
                fleet->packed_ship_condition[ship] = 0;
            else
                fleet->packed_ship_condition[ship] =
                    (condition & 0x7f) | (StarsU16)(((condition >> 7) - rate) << 7);
        }
    }
    for (index = 0; index < (StarsI16)input->planet_count; ++index) {
        planet = &input->planets[index];
        if (!planet->starbase_present || planet->starbase_blocked ||
                planet->starbase_damage == 0)
            continue;
        rate = HEAL_STARBASE_RATE;
        if (input->race_stat && input->race_stat(input->context,
                planet->owner, HEAL_RACE_STAT) == 4)
            rate = HEAL_STARBASE_BONUS;
        if (rate >= (StarsI16)planet->starbase_damage)
            planet->starbase_damage = 0;
        else
            planet->starbase_damage = (StarsU16)(planet->starbase_damage - rate);
    }
}
