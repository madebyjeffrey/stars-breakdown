#include "core.h"

#define BREED_COLONISTS_RACE_STAT 0x0e
#define BREED_COLONISTS_MESSAGE_CARGO 0xfb
#define BREED_COLONISTS_MESSAGE_PLANET 0x158

/* NE 24:7E4E-80FD. Give fleets belonging to races with the transit-breeding
 * trait their growth bonus. The bonus is fixed-point colonist cargo divided
 * by 200; if less than one colonist is produced, the original one-in-three
 * random gate decides whether to defer it. Untransferred growth is deposited
 * at the fleet's owned destination planet. */
void stars_breed_colonists_in_transit(StarsTransitBreedInput *input)
{
    StarsU8 breeder[16];
    StarsI16 player;
    StarsU16 index;
    StarsI16 stat;
    StarsI16 random_value;
    StarsI32 growth;
    StarsI32 transferred;
    StarsI32 remainder;
    StarsTransitBreedFleet *fleet;
    StarsTransitBreedPlanet *planet;

    if (!input || !input->fleets || !input->ops.race_stat)
        return;
    for (player = 0; player < input->player_count && player < 16; ++player) {
        stat = input->ops.race_stat(input->context, player,
            BREED_COLONISTS_RACE_STAT);
        breeder[player] = (StarsU8)(stat == 4);
    }
    if (player == 0)
        return;
    for (player = 0; player < input->player_count && player < 16; ++player)
        if (breeder[player])
            break;
    if (player == input->player_count || player == 16)
        return;

    for (index = 0; index < input->fleet_count; ++index) {
        fleet = &input->fleets[index];
        if ((fleet->status_flags & 0x0400) != 0 || fleet->player < 0 ||
                fleet->player >= input->player_count || fleet->player >= 16 ||
                !breeder[fleet->player] || fleet->colonists == 0UL)
            continue;
        growth = ((StarsI32)fleet->colonists *
            (StarsI32)fleet->growth_rate) / 200L;
        if (growth < 1L) {
            random_value = input->ops.random ?
                input->ops.random(input->context, 3) : 1;
            if (random_value != 0)
                continue;
            growth = 1L;
        }
        transferred = input->ops.transfer_colonists ?
            input->ops.transfer_colonists(input->context, fleet, growth) : 0L;
        if (transferred > 0L && input->ops.send_message)
            input->ops.send_message(input->context, fleet->player,
                BREED_COLONISTS_MESSAGE_CARGO,
                (StarsU16)(fleet->id | 0x8000), fleet->id, 0,
                (StarsI16)transferred, 0, 0);
        if (transferred >= growth || fleet->destination == -1 ||
                !input->ops.planet_from_id)
            continue;
        planet = input->ops.planet_from_id(input->context, fleet->destination);
        if (!planet || planet->player != fleet->player)
            continue;
        remainder = growth - transferred;
        planet->population += (StarsU32)remainder;
        if (input->ops.send_message)
            input->ops.send_message(input->context, fleet->player,
                BREED_COLONISTS_MESSAGE_PLANET,
                (StarsU16)(fleet->id | 0x8000), fleet->id,
                (StarsU16)planet->id, (StarsI16)remainder,
                (StarsI16)(remainder >> 16), 0);
    }
}
