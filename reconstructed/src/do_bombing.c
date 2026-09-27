#include "core.h"

#define BOMB_FLEET_SKIP 0x0400U
#define BOMB_FLEET_DONE 0x1000U
#define BOMB_PLANET_STARBASE 0x0200U
#define BOMB_PER_THOUSAND 1000L
#define BOMB_ENVIRONMENT_CHANNELS 3
#define BOMB_EVENT_FLEET_ENVIRONMENT 0x012e
#define BOMB_EVENT_PLANET_ENVIRONMENT 0x017a
#define BOMB_EVENT_FLEET_EMPTY 0x008f
#define BOMB_EVENT_PLANET_EMPTY 0x0090
#define BOMB_EVENT_FLEET_POPULATION 0x0060
#define BOMB_EVENT_PLANET_POPULATION 0x006a
#define BOMB_EVENT_FLEET_POPULATION_MULTI 0x0166
#define BOMB_EVENT_PLANET_POPULATION_MULTI 0x0170
#define BOMB_EVENT_FLEET_BUILDING 99
#define BOMB_EVENT_PLANET_BUILDING 0x006d
#define BOMB_EVENT_FLEET_BUILDING_MULTI 0x0169
#define BOMB_EVENT_PLANET_BUILDING_MULTI 0x0173

static StarsDoBombingPlanet *bomb_planet(StarsDoBombingInput *input,
    StarsI16 id)
{
    StarsU16 i;
    for (i = 0; i < input->planet_count; ++i)
        if (input->planets[i].id == id) return &input->planets[i];
    return (StarsDoBombingPlanet *)0;
}

static StarsI32 bomb_scaled(StarsI32 value, StarsU16 per_thousand)
{
    if (value <= 0) return 0;
    return (StarsI32)(((StarsI32)value * (StarsI32)per_thousand) /
        BOMB_PER_THOUSAND);
}

static StarsI32 bomb_clamp(StarsI32 value, StarsI32 low, StarsI32 high)
{
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

static void bomb_change_environment(StarsDoBombingPlanet *planet,
    StarsI32 damage)
{
    StarsI16 channel;
    StarsI16 delta;
    if (damage <= 0) return;
    for (channel = 0; channel < BOMB_ENVIRONMENT_CHANNELS; ++channel) {
        delta = (StarsI16)(planet->environment[channel] -
            planet->original_environment[channel]);
        if (delta > 0) {
            if (delta > damage) delta = (StarsI16)damage;
            planet->environment[channel] = (signed char)(
                planet->environment[channel] - delta);
            damage -= delta;
        }
        if (damage <= 0) break;
    }
}

/* Genuine semantic reconstruction of DoBombing, NE 31:AEFA-BE93.
 * Each eligible fleet attacks an enemy non-starbase planet once.  The
 * original combines same-owner fleets through FCalcFleetBombDamage, applies
 * defense-adjusted population and infrastructure damage, consumes the
 * corresponding mineral/terraforming pools, reduces environmental penalties,
 * emits separate fleet/planet report messages, and removes a planet when all
 * colonists and minerals are gone.  The callbacks keep those original service
 * boundaries visible while the state updates remain ordinary C. */
void stars_do_bombing(StarsDoBombingInput *input)
{
    StarsU16 index;
    StarsDoBombingFleet *fleet;
    StarsDoBombingPlanet *planet;
    StarsDoBombingDamage damage;
    StarsU16 survival = 1000U;
    StarsU16 smart_survival = 1000U;
    StarsI32 population_loss;
    StarsI32 smart_loss;
    StarsI32 building_loss;
    StarsI32 environment_loss;
    StarsI16 fleet_event;
    StarsI16 planet_event;
    StarsI16 base_fleet_event;
    StarsI16 base_planet_event;
    StarsI16 amount;

    if (!input || !input->fleets || !input->planets || !input->calculate)
        return;
    for (index = 0; index < input->fleet_count; ++index) {
        fleet = &input->fleets[index];
        if ((fleet->flags & BOMB_FLEET_SKIP) ||
                (fleet->flags & BOMB_FLEET_DONE) || fleet->planet_id < 0)
            continue;
        planet = bomb_planet(input, fleet->planet_id);
        if (!planet || planet->owner < 0 || planet->owner == fleet->player ||
                (planet->flags & BOMB_PLANET_STARBASE)) continue;
        if (input->attack && !input->attack(input->context, fleet,
                planet->owner)) continue;
        damage.population = damage.minimum_population = 0;
        damage.smart_population = damage.buildings = damage.terraforming = 0;
        damage.multiple_fleets = 0;
        if (!input->calculate(input->context, fleet, &damage)) continue;
        fleet->flags = (StarsU16)(fleet->flags | BOMB_FLEET_DONE);
        survival = smart_survival = 1000U;
        if (input->survival)
            input->survival(input->context, planet, &survival,
                &smart_survival);
        population_loss = bomb_scaled(damage.population, survival);
        smart_loss = bomb_scaled(damage.smart_population, smart_survival);
        if (smart_loss > population_loss) population_loss = smart_loss;
        if (population_loss < damage.minimum_population)
            population_loss = damage.minimum_population;
        population_loss = bomb_clamp(population_loss, 0, planet->population);
        planet->population -= population_loss;
        building_loss = bomb_scaled(damage.buildings, survival);
        building_loss = bomb_clamp(building_loss, 0, planet->buildings);
        planet->buildings = (StarsI16)(planet->buildings - building_loss);
        environment_loss = bomb_scaled(damage.terraforming, survival);
        bomb_change_environment(planet, environment_loss);
        amount = (StarsI16)bomb_scaled(damage.buildings, survival);
        if (amount > 0 && planet->mineral_stock > 0) {
            if ((StarsI32)amount > planet->mineral_stock)
                amount = (StarsI16)planet->mineral_stock;
            planet->mineral_stock -= amount;
        }
        if (population_loss > 0) {
            base_fleet_event = damage.multiple_fleets ?
                BOMB_EVENT_FLEET_POPULATION_MULTI : BOMB_EVENT_FLEET_POPULATION;
            base_planet_event = damage.multiple_fleets ?
                BOMB_EVENT_PLANET_POPULATION_MULTI : BOMB_EVENT_PLANET_POPULATION;
        } else {
            base_fleet_event = BOMB_EVENT_FLEET_BUILDING;
            base_planet_event = BOMB_EVENT_PLANET_BUILDING;
        }
        if (planet->population == 0 && planet->mineral_stock == 0) {
            fleet_event = damage.multiple_fleets ? 0x017c : BOMB_EVENT_FLEET_EMPTY;
            planet_event = damage.multiple_fleets ? 0x017d : BOMB_EVENT_PLANET_EMPTY;
        } else {
            fleet_event = base_fleet_event;
            planet_event = base_planet_event;
        }
        if (input->send_message) {
            input->send_message(input->context, fleet->player, fleet_event,
                (StarsI16)(fleet->id | 0x8000), fleet->id, planet->id,
                (StarsI16)population_loss, (StarsI16)building_loss,
                (StarsI16)environment_loss, 0);
            input->send_message(input->context, planet->owner, planet_event,
                planet->id, fleet->id, planet->id, (StarsI16)population_loss,
                (StarsI16)building_loss, (StarsI16)environment_loss, 0);
        }
        (void)input->random;
        if (planet->population == 0 && planet->mineral_stock == 0 &&
                input->uninhabit)
            input->uninhabit(input->context, planet);
    }
}
