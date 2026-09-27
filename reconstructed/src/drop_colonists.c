#include "core.h"

#define DROP_MESSAGE_PRIVATE 0x57
#define DROP_MESSAGE_REJECTED 0x58
#define DROP_MESSAGE_CLAIMED 7
#define DROP_MESSAGE_SHARED 8
#define DROP_MESSAGE_CONTESTED 9
#define DROP_MESSAGE_ARRIVED 0xc5
#define DROP_RACE_STAT 0x000e
#define DROP_RACE_GROWTH 2
#define DROP_RACE_RESEARCH 8

static void drop_message(const StarsDropColonistsOps *ops,
    StarsI16 player, StarsI16 code, StarsI16 planet, StarsI16 value)
{
    if (ops->send_message)
        ops->send_message(ops->context, player, code, planet, value, 0, 0);
}

static StarsI32 drop_survivors(const StarsDropColonistsOps *ops,
    const StarsColonistPlanet *planet, const StarsColonistDrop *drop)
{
    StarsI32 survivors;
    StarsI32 adjusted;

    survivors = drop->colonists;
    if (ops->survival) {
        adjusted = 0;
        if (ops->survival(ops->context, planet, drop->source_owner,
                survivors, &adjusted))
            survivors = adjusted;
    } else if (planet->climate_factor > 0) {
        survivors = (survivors * planet->climate_factor) / 100;
    }
    if (survivors < 0) survivors = 0;
    return survivors;
}

static StarsI16 drop_should_claim(const StarsDropColonistsOps *ops,
    const StarsColonistPlanet *planet, const StarsColonistDrop *drop)
{
    if (planet->owner >= 0)
        return 0;
    if (drop->private_drop)
        return 0;
    if (ops->race_stat && ops->race_stat(ops->context,
            drop->source_owner, DROP_RACE_STAT) == 8)
        return 1;
    return 1;
}

void stars_drop_colonists(StarsDropColonistsInput *input)
{
    const StarsDropColonistsOps *ops;
    StarsColonistDrop *drop;
    StarsColonistPlanet planet;
    StarsI32 survivors;
    StarsI16 index;
    StarsI16 owner;

    if (!input || !input->drops || !input->ops)
        return;
    ops = input->ops;
    for (index = 0; index < (StarsI16)input->drop_count; ++index) {
        drop = &input->drops[index];
        if (!drop->active || drop->target_planet < 0 || drop->colonists <= 0)
            continue;
        if (!ops->lookup_planet || !ops->lookup_planet(ops->context,
                drop->target_planet, &planet)) {
            drop->active = 0;
            continue;
        }
        survivors = drop_survivors(ops, &planet, drop);
        owner = planet.owner;
        if (drop->private_drop && owner >= 0 && owner != drop->source_owner) {
            drop_message(ops, drop->source_owner, DROP_MESSAGE_PRIVATE,
                planet.id, 0);
            drop->active = 0;
            continue;
        }
        if (survivors <= 0) {
            drop_message(ops, drop->source_owner, DROP_MESSAGE_REJECTED,
                planet.id, 0);
            drop->active = 0;
            continue;
        }
        if (planet.owner >= 0 && planet.capacity > 0 &&
                planet.population + survivors > planet.capacity) {
            if (ops->uninhabit)
                ops->uninhabit(ops->context, &planet);
            planet.owner = -1;
            owner = -1;
        }
        if (owner < 0 && drop_should_claim(ops, &planet, drop)) {
            owner = drop->source_owner;
            planet.owner = owner;
            if (ops->claim_planet)
                ops->claim_planet(ops->context, &planet, owner);
            if (ops->init_production)
                ops->init_production(ops->context, &planet, owner);
            if (ops->learn_research)
                ops->learn_research(ops->context, owner, planet.id);
            drop_message(ops, owner, DROP_MESSAGE_CLAIMED, planet.id,
                (StarsI16)survivors);
        } else if (owner >= 0 && owner != drop->source_owner) {
            drop_message(ops, owner, DROP_MESSAGE_CONTESTED, planet.id,
                (StarsI16)survivors);
        } else {
            drop_message(ops, drop->source_owner, DROP_MESSAGE_SHARED,
                planet.id, (StarsI16)survivors);
        }
        planet.population += survivors;
        planet.growth += survivors;
        if (ops->lookup_planet)
            ops->lookup_planet(ops->context, planet.id, &planet);
        drop_message(ops, drop->source_owner, DROP_MESSAGE_ARRIVED,
            planet.id, (StarsI16)survivors);
        drop->active = 0;
        drop->target_planet = -1;
    }
}
