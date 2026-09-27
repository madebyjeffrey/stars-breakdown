/* NE 10:6E1E-7095. PctPlanetDesirability.
 * Valid-domain integer equivalent of the original sqrt(sum/3)+0.9 rounding.
 * See analysis/reconstruction/habitability.md for limits and proof. */
#include "core.h"
#include "innate_fp.h"
#include <math.h>
#include <string.h>

/* NE 10:21F6-22CC. Probe up to five defense-part items beginning at item 9,
 * then leave the part at the last successful item (or item 9 when none was
 * found), preserving the lookup's returned component bytes. */
StarsI16 stars_get_best_defense_part(StarsU8 *part,
    StarsLookupDefensePart lookup, void *context)
{
    StarsU8 candidate[8];
    StarsI16 found = 0;
    StarsI16 item;
    StarsI16 result = 1;
    if (!part || !lookup) return 0;
    memset(candidate, 0, sizeof candidate);
    candidate[0] = 0;
    candidate[1] = 0x80U;
    candidate[2] = 9U;
    while (found < 5 && lookup(context, candidate) == 1) {
        ++found;
        candidate[2] = (StarsU8)(candidate[2] + 1U);
    }
    if (found < 1) result = 0;
    else --found;
    item = (StarsI16)(9 + found);
    candidate[2] = (StarsU8)item;
    lookup(context, candidate);
    memcpy(part, candidate, sizeof candidate);
    return result;
}

/* NE 10:7248-7303. Scale the planet's maximum population by the player's
 * mining trait, enforce the ten-unit floor, then disable mining for the
 * special race-stat value 8. */
StarsI16 stars_cmax_mines(const StarsU8 *planet, StarsI16 player,
    const StarsCMaxMinesOps *ops, void *context)
{
    StarsU16 planet_id;
    StarsI16 mining_trait;
    StarsI16 disable_stat;
    StarsI32 capacity;
    if (!planet || !ops || !ops->calc_max_population ||
            !ops->get_player_stat) return 0;
    planet_id = (StarsU16)(planet[0] | ((StarsU16)planet[1] << 8));
    capacity = ops->calc_max_population(context, planet_id, player);
    mining_trait = ops->get_player_stat(context, player, 6);
    capacity = (StarsI32)((capacity * (StarsI32)mining_trait) / 100L);
    if (capacity < 10L) capacity = 10L;
    disable_stat = ops->get_player_stat(context, player, 0x0e);
    if (disable_stat == 8) capacity = 0;
    return (StarsI16)capacity;
}

/* NE 10:6ACA-6B87. Convert the fourth mineral-weight value to a rounded
 * percentage of maximum population, retaining the zero-population path and
 * the original 999 ceiling. */
StarsI16 stars_pct_planet_capacity(const StarsU8 *planet, StarsI16 player,
    StarsCalcPlanetMaxPop calc_max_population, void *context)
{
    StarsU16 planet_id;
    StarsI32 max_population;
    StarsI32 mineral_weight;
    StarsI32 percentage;
    if (!planet || !calc_max_population) return 0;
    planet_id = (StarsU16)(planet[0] | ((StarsU16)planet[1] << 8));
    max_population = calc_max_population(context, planet_id, player);
    if (max_population < 1L) return 0;
    mineral_weight = (StarsI32)((StarsU32)planet[40] |
        ((StarsU32)planet[41] << 8) | ((StarsU32)planet[42] << 16) |
        ((StarsU32)planet[43] << 24));
    percentage = (StarsI32)((mineral_weight * 100L +
        max_population / 2L) / max_population);
    if (percentage > 999L) percentage = 999L;
    return (StarsI16)percentage;
}

/* NE 10:7304-73FC. Limit operable mines by both the scaled mineral weight
 * and CMaxMines, optionally including next year's population change. */
StarsI16 stars_cmax_operable_mines(const StarsU8 *planet, StarsI16 player,
    StarsI16 next_year, const StarsCMaxOperableMinesOps *ops, void *context)
{
    StarsI16 mining_trait;
    StarsI16 disable_stat;
    StarsI16 maximum;
    StarsI32 mineral_weight;
    StarsI32 population_change = 0;
    StarsCMaxMinesOps max_ops;
    if (!planet || !ops || !ops->max_mines.calc_max_population ||
            !ops->max_mines.get_player_stat) return 0;
    max_ops = ops->max_mines;
    maximum = stars_cmax_mines(planet, player, &max_ops, context);
    mineral_weight = (StarsI32)((StarsU32)planet[40] |
        ((StarsU32)planet[41] << 8) | ((StarsU32)planet[42] << 16) |
        ((StarsU32)planet[43] << 24));
    if (next_year && ops->change_population)
        population_change = ops->change_population(context, planet, 0);
    mineral_weight += population_change;
    mining_trait = ops->max_mines.get_player_stat(context, player, 6);
    mineral_weight = (StarsI32)((mineral_weight * mining_trait) / 100L);
    if (mineral_weight < (StarsI32)maximum) maximum = (StarsI16)mineral_weight;
    if (maximum < 1) maximum = 1;
    disable_stat = ops->max_mines.get_player_stat(context, player, 0x0e);
    if (disable_stat == 8) maximum = 0;
    return maximum;
}

/* NE 10:73FC-74BE. Report the mine count that can currently operate. */
StarsI16 stars_cmines_operating(const StarsU8 *planet,
    const StarsCMaxOperableMinesOps *ops, void *context)
{
    StarsI16 player;
    StarsI16 disable_stat;
    StarsI16 result;
    StarsU32 mineral_count;
    StarsI32 mineral_weight;

    if (!planet || !ops || !ops->max_mines.get_player_stat ||
            !ops->max_mines.calc_max_population) return 0;
    player = (StarsI16)((StarsU16)planet[2] |
        ((StarsU16)planet[3] << 8));
    if (player == (StarsI16)-1) return 0;
    disable_stat = ops->max_mines.get_player_stat(context, player, 0x0e);
    if (disable_stat == 8) {
        mineral_weight = (StarsI32)((StarsU32)planet[34] |
            ((StarsU32)planet[35] << 8) |
            ((StarsU32)planet[36] << 16) |
            ((StarsU32)planet[37] << 24));
        if (mineral_weight <= 0) return 0;
        return (StarsI16)sqrt((double)mineral_weight);
    }
    mineral_count = (StarsU32)((StarsU32)planet[20] |
        ((StarsU32)planet[21] << 8) |
        ((StarsU32)planet[22] << 16) |
        ((StarsU32)planet[23] << 24));
    mineral_count = (StarsU32)((mineral_count >> 8) & 0x0fffUL);
    result = stars_cmax_operable_mines(planet, player, 0, ops, context);
    if ((StarsI32)mineral_count < (StarsI32)result)
        result = (StarsI16)mineral_count;
    return result;
}

/* NE 10:755C-7618. Scale the planet's maximum population by the player's
 * factory trait, enforce the ten-unit floor, then apply the disable stat. */
StarsI16 stars_cmax_factories(const StarsU8 *planet, StarsI16 player,
    const StarsCMaxMinesOps *ops, void *context)
{
    StarsU16 planet_id;
    StarsI16 factory_trait;
    StarsI16 disable_stat;
    StarsI32 capacity;
    if (!planet || !ops || !ops->calc_max_population ||
            !ops->get_player_stat) return 0;
    planet_id = (StarsU16)(planet[0] | ((StarsU16)planet[1] << 8));
    capacity = ops->calc_max_population(context, planet_id, player);
    factory_trait = ops->get_player_stat(context, player, 3);
    capacity = (StarsI32)((capacity * (StarsI32)factory_trait) / 100L);
    if (capacity < 10L) capacity = 10L;
    disable_stat = ops->get_player_stat(context, player, 0x0e);
    if (disable_stat == 8) capacity = 0;
    return (StarsI16)capacity;
}

/* NE 10:7618-7710. Limit operable factories by the scaled mineral weight
 * and CMaxFactories, optionally including next year's population change. */
StarsI16 stars_cmax_operable_factories(const StarsU8 *planet,
    StarsI16 player, StarsI16 next_year,
    const StarsCMaxOperableFactoriesOps *ops, void *context)
{
    StarsI16 factory_trait;
    StarsI16 disable_stat;
    StarsI16 maximum;
    StarsI32 mineral_weight;
    StarsI32 population_change = 0;
    StarsCMaxMinesOps max_ops;
    if (!planet || !ops || !ops->max_factories.calc_max_population ||
            !ops->max_factories.get_player_stat) return 0;
    max_ops = ops->max_factories;
    maximum = stars_cmax_factories(planet, player, &max_ops, context);
    mineral_weight = (StarsI32)((StarsU32)planet[40] |
        ((StarsU32)planet[41] << 8) | ((StarsU32)planet[42] << 16) |
        ((StarsU32)planet[43] << 24));
    if (next_year && ops->change_population)
        population_change = ops->change_population(context, planet, 0);
    mineral_weight += population_change;
    factory_trait = ops->max_factories.get_player_stat(context, player, 3);
    mineral_weight = (StarsI32)((mineral_weight * factory_trait) / 100L);
    if (mineral_weight < (StarsI32)maximum) maximum = (StarsI16)mineral_weight;
    if (maximum < 1) maximum = 1;
    disable_stat = ops->max_factories.get_player_stat(context, player, 0x0e);
    if (disable_stat == 8) maximum = 0;
    return maximum;
}

/* NE 10:74BE-755C. Report the factories that can currently operate. */
StarsI16 stars_cfactories_operating(const StarsU8 *planet,
    const StarsCMaxOperableFactoriesOps *ops, void *context)
{
    StarsI16 player;
    StarsI16 disable_stat;
    StarsI16 result;
    StarsU32 factory_count;
    if (!planet || !ops || !ops->max_factories.get_player_stat ||
            !ops->max_factories.calc_max_population) return 0;
    player = (StarsI16)((StarsU16)planet[2] |
        ((StarsU16)planet[3] << 8));
    if (player == (StarsI16)-1) return 0;
    disable_stat = ops->max_factories.get_player_stat(context, player, 0x0e);
    if (disable_stat == 8) return 0;
    factory_count = (StarsU32)((StarsU32)planet[20] |
        ((StarsU32)planet[21] << 8) |
        ((StarsU32)planet[22] << 16) |
        ((StarsU32)planet[23] << 24));
    factory_count = (StarsU32)((factory_count >> 20) & 0x0fffUL);
    result = stars_cmax_operable_factories(planet, player, 0, ops, context);
    if ((StarsI32)factory_count < (StarsI32)result)
        result = (StarsI16)factory_count;
    return result;
}

/* NE 10:7710-77AE. Convert desirability to a defense capacity, clamp it to
 * the original ten-to-one-hundred range, then apply the disable stat. */
StarsI16 stars_cmax_defenses(const StarsU8 *planet, StarsI16 player,
    const StarsCMaxDefensesOps *ops, void *context)
{
    StarsI16 desirability;
    StarsI16 disable_stat;
    StarsI32 capacity;
    if (!planet || !ops || !ops->planet_desirability ||
            !ops->get_player_stat) return 0;
    desirability = ops->planet_desirability(context, planet, player);
    capacity = (StarsI32)desirability * 4L;
    if (capacity < 10L) capacity = 10L;
    if (capacity >= 101L) capacity = 100L;
    disable_stat = ops->get_player_stat(context, player, 0x0e);
    if (disable_stat == 8) capacity = 0;
    return (StarsI16)capacity;
}

/* NE 10:77AE-788E. Limit operable defenses by the rounded mineral-weight
 * quotient, optionally including next year's population change. */
StarsI16 stars_cmax_operable_defenses(const StarsU8 *planet,
    StarsI16 player, StarsI16 next_year,
    const StarsCMaxOperableDefensesOps *ops, void *context)
{
    StarsI16 maximum;
    StarsI16 disable_stat;
    StarsI32 mineral_weight;
    StarsI32 population_change = 0;
    StarsI32 capacity;
    StarsCMaxDefensesOps max_ops;
    if (!planet || !ops || !ops->max_defenses.planet_desirability ||
            !ops->max_defenses.get_player_stat) return 0;
    max_ops = ops->max_defenses;
    maximum = stars_cmax_defenses(planet, player, &max_ops, context);
    mineral_weight = (StarsI32)((StarsU32)planet[40] |
        ((StarsU32)planet[41] << 8) | ((StarsU32)planet[42] << 16) |
        ((StarsU32)planet[43] << 24));
    if (next_year && ops->change_population)
        population_change = ops->change_population(context, planet, 0);
    capacity = (mineral_weight + population_change + 24L) / 25L;
    if (capacity > 1000L) capacity = 1000L;
    if (capacity < (StarsI32)maximum) maximum = (StarsI16)capacity;
    disable_stat = ops->max_defenses.get_player_stat(context, player, 0x0e);
    if (disable_stat == 8) maximum = 0;
    return maximum;
}

/* NE 10:788E-7B0F. Combine population, mineral, factory and race factors
 * into the planet's available resources, retaining the special-race path. */
StarsI16 stars_cresources_at_planet(const StarsU8 *planet, StarsI16 player,
    const StarsCResourcesAtPlanetOps *ops, void *context)
{
    StarsI16 environment_factor;
    StarsI16 race_factor;
    StarsI16 factory_trait;
    StarsI16 disable_stat;
    StarsI16 desirability;
    StarsI16 factory_count;
    StarsI16 factory_capacity;
    StarsI16 result;
    StarsI32 mineral_weight;
    StarsI32 maximum_population;
    StarsI32 effective_weight;
    StarsI32 factory_resources;
    StarsCMaxOperableFactoriesOps factory_ops;
    double special_resources;

    if (!planet || !ops || !ops->calc_max_population ||
            !ops->planet_desirability || !ops->get_player_stat ||
            !ops->factory_calc_max_population ||
            !ops->factory_get_player_stat) return 0;
    mineral_weight = (StarsI32)((StarsU32)planet[40] |
        ((StarsU32)planet[41] << 8) | ((StarsU32)planet[42] << 16) |
        ((StarsU32)planet[43] << 24));
    if (mineral_weight == 0) return 0;
    environment_factor = ops->get_player_stat(context, player, 0);
    maximum_population = ops->calc_max_population(context,
        (StarsU16)(planet[0] | ((StarsU16)planet[1] << 8)), player);
    effective_weight = mineral_weight;
    if (maximum_population < effective_weight) {
        effective_weight = maximum_population +
            (effective_weight - maximum_population) / 2L;
        if (effective_weight > maximum_population * 2L)
            effective_weight = maximum_population * 2L;
    }
    disable_stat = ops->get_player_stat(context, player, 0x0e);
    if (disable_stat == 8) {
        if (!ops->get_player_byte || maximum_population == 0) return 0;
        race_factor = ops->get_player_byte(context, player, 0x1a);
        if (race_factor < 1) race_factor = 1;
        desirability = ops->planet_desirability(context, planet, player);
        if (desirability < 25) desirability = 25;
        special_resources = sqrt((double)effective_weight *
            (double)race_factor / (double)maximum_population);
        special_resources = special_resources * (double)desirability / 10.0;
        result = (StarsI16)(special_resources + 0.9);
    } else {
        if (environment_factor == 0) return 0;
        result = (StarsI16)(effective_weight / environment_factor);
        factory_ops.max_factories.calc_max_population =
            ops->factory_calc_max_population;
        factory_ops.max_factories.get_player_stat =
            ops->factory_get_player_stat;
        factory_ops.change_population = ops->factory_change_population;
        factory_capacity = stars_cmax_operable_factories(planet, player, 0,
            &factory_ops, context);
        factory_count = (StarsI16)((((StarsU32)planet[20] |
            ((StarsU32)planet[21] << 8) |
            ((StarsU32)planet[22] << 16) |
            ((StarsU32)planet[23] << 24)) >> 20) & 0x0fffU);
        if (factory_count > factory_capacity)
            factory_count = factory_capacity;
        factory_trait = ops->get_player_stat(context, player, 1);
        factory_resources = ((StarsI32)factory_count * factory_trait + 9L) /
            10L;
        result = (StarsI16)((StarsI32)result + factory_resources);
    }
    if (result == 0) result = 1;
    return result;
}

/* NE 10:7B10-7CF9. Select the strongest qualifying warp machine and report
 * whether a second machine ties it; fleet records are supplied by callbacks
 * because they are global far data in the original program. */
StarsI16 stars_iwarp_ma_from_planet(const StarsU8 *planet,
    StarsI16 current_player, StarsI16 *two,
    const StarsGetWarpMAItems get_items,
    const StarsGetWarpMARaceByte get_race_byte, void *context)
{
    StarsI16 owner;
    StarsI16 allowed;
    StarsI16 best = 0;
    StarsI16 tied = 0;
    StarsU16 machine_index;
    StarsU16 count;
    StarsU16 i;
    StarsI16 kind;
    StarsWarpMAItem items[256];
    StarsU16 owner_word;
    StarsU16 flag_word;
    if (two) *two = 0;
    if (!planet || !get_items) return 0;
    owner_word = (StarsU16)(planet[2] | ((StarsU16)planet[3] << 8));
    owner = (StarsI16)owner_word;
    flag_word = (StarsU16)(planet[4] | ((StarsU16)planet[5] << 8));
    if (owner == (StarsI16)-1 || (flag_word & 0x0200U) == 0)
        return 0;
    allowed = (StarsI16)(owner == current_player || current_player == -1);
    machine_index = (StarsU16)((StarsU32)planet[44] |
        ((StarsU32)planet[45] << 8) |
        ((StarsU32)planet[46] << 16) |
        ((StarsU32)planet[47] << 24));
    machine_index = (StarsU16)(machine_index & 0x000fU);
    if (!allowed && get_race_byte)
        allowed = (StarsI16)(get_race_byte(context, owner, machine_index,
            0x007bU) == 7);
    if (!allowed) return 0;
    count = get_items(context, owner, machine_index, items, 256U);
    if (count > 256U) count = 256U;
    for (i = 0; i < count; ++i) {
        if (items[i].identifier != 0x0200UL) continue;
        if ((items[i].flags >> 8) == 0) continue;
        kind = (StarsI16)(items[i].flags & 0x00ffU);
        if (kind <= 6 || kind >= 16) continue;
        kind = (StarsI16)(kind - 2);
        if (best < kind) {
            best = kind;
            tied = 0;
        } else if (best == kind) {
            tied = 1;
        }
    }
    if (two) *two = tied;
    return best;
}

/* NE 10:7CFA-7E99. Find the first qualifying stargate part and return its
 * fuel range, retaining the original unknown-part fallback of 10000. */
StarsI16 stars_stargate_range_from_planet(const StarsU8 *planet,
    StarsI16 player, StarsI16 stargate_index,
    const StarsGetStargateItems get_items,
    const StarsLookupStargateFuel lookup_fuel, void *context)
{
    StarsI16 owner = player;
    StarsU16 machine_index = (StarsU16)stargate_index;
    StarsU16 count;
    StarsU16 i;
    StarsU16 flag_word;
    StarsWarpMAItem items[256];
    StarsI16 kind;
    StarsI16 fuel_range;
    if (!get_items || !lookup_fuel) return 0;
    if (planet) {
        owner = (StarsI16)((StarsU16)planet[2] |
            ((StarsU16)planet[3] << 8));
        flag_word = (StarsU16)(planet[4] | ((StarsU16)planet[5] << 8));
        if (owner == (StarsI16)-1 || (flag_word & 0x0200U) == 0)
            return 0;
        machine_index = (StarsU16)((StarsU32)planet[44] |
            ((StarsU32)planet[45] << 8) |
            ((StarsU32)planet[46] << 16) |
            ((StarsU32)planet[47] << 24));
        machine_index = (StarsU16)(machine_index & 0x000fU);
    }
    count = get_items(context, owner, machine_index, items, 256U);
    if (count > 256U) count = 256U;
    for (i = 0; i < count; ++i) {
        if (items[i].identifier != 0x0200UL) continue;
        if ((items[i].flags >> 8) == 0) continue;
        kind = (StarsI16)(items[i].flags & 0x00ffU);
        if (kind >= 7) continue;
        fuel_range = lookup_fuel(context, items[i].identifier,
            items[i].flags);
        if (fuel_range != (StarsI16)-1) return fuel_range;
        return 10000;
    }
    return 0;
}

/* NE 10:7E9A-7F56. A production record is terraforming when its kind bits
 * equal one and its item code is one of the three terraforming codes. */
StarsI16 stars_fprod_is_terra(const StarsU8 *product)
{
    StarsU32 value;
    StarsU32 kind;
    StarsU32 code;
    if (!product) return 0;
    value = (StarsU32)product[0] | ((StarsU32)product[1] << 8) |
        ((StarsU32)product[2] << 16) | ((StarsU32)product[3] << 24);
    kind = (value >> 17) & 7UL;
    code = (value >> 10) & 0x7fUL;
    return (StarsI16)(kind == 1UL &&
        (code == 0x0cUL || code == 4UL || code == 5UL));
}

/* NE 10:7F56-8021. Sum the currently available terraforming deltas returned
 * by FCanTerraformLppl, preserving its three-axis sentinel conventions. */
StarsI16 stars_ipct_can_terraform(const StarsU8 *planet,
    StarsCanTerraform can_terraform, void *context)
{
    StarsI16 env_min[3];
    StarsI16 env_max[3];
    StarsI16 env_cost[3];
    StarsI16 current;
    StarsI16 result = 0;
    StarsI16 i;
    if (!planet || !can_terraform) return 0;
    if (!can_terraform(context, planet, env_min, env_max, env_cost, 1))
        return 0;
    for (i = 0; i < 3; ++i) {
        current = (StarsI16)planet[12 + i];
        if (env_min[i] != (StarsI16)-1)
            result = (StarsI16)(result + current - env_min[i]);
        if (env_max[i] != (StarsI16)-1)
            result = (StarsI16)(result + env_max[i] - current);
    }
    return result;
}

/* NE 10:8732-88BF. Reset an inhabited planet to its uninhabited state while
 * releasing its production record through the host-owned allocation hook. */
void stars_uninhabit_planet(StarsU8 *planet,
    const StarsUninhabitPlanetOps *ops, void *context)
{
    StarsI16 owner;
    StarsI16 i;
    StarsU16 flags;
    StarsU16 mineral_flags;
    StarsU16 fleet_flags;
    StarsU32 production;
    if (!planet) return;
    owner = (StarsI16)((StarsU16)planet[2] |
        ((StarsU16)planet[3] << 8));
    if (owner >= 0 && ops && ops->get_player_stat &&
            ops->get_player_stat(context, owner, 0x0e) == 3) {
        for (i = 0; i < 3; ++i) planet[12 + i] = planet[15 + i];
    }
    planet[2] = 0xffU;
    planet[3] = 0xffU;
    planet[40] = 0;
    planet[41] = 0;
    planet[42] = 0;
    planet[43] = 0;
    production = (StarsU32)planet[52] | ((StarsU32)planet[53] << 8) |
        ((StarsU32)planet[54] << 16) | ((StarsU32)planet[55] << 24);
    if (production != 0 && ops && ops->free_production)
        ops->free_production(context,
            (StarsI16)((StarsU16)planet[0] |
                ((StarsU16)planet[1] << 8)), production);
    planet[52] = 0;
    planet[53] = 0;
    planet[54] = 0;
    planet[55] = 0;
    flags = (StarsU16)(planet[4] | ((StarsU16)planet[5] << 8));
    flags = (StarsU16)(flags & 0xfdffU);
    planet[4] = (StarsU8)flags;
    planet[5] = (StarsU8)(flags >> 8);
    mineral_flags = (StarsU16)(planet[24] | ((StarsU16)planet[25] << 8));
    fleet_flags = (StarsU16)(planet[26] | ((StarsU16)planet[27] << 8));
    mineral_flags = (StarsU16)((mineral_flags & 0x0fffU) | 0xf000U);
    fleet_flags = (StarsU16)((fleet_flags & 0xff7eU) | 1U);
    planet[24] = (StarsU8)mineral_flags;
    planet[25] = (StarsU8)(mineral_flags >> 8);
    planet[26] = (StarsU8)fleet_flags;
    planet[27] = (StarsU8)(fleet_flags >> 8);
    planet[44] = 0;
    planet[45] = 0;
    planet[46] = 0;
    planet[47] = 0;
}

static StarsI16 cloak_section_points(StarsU16 group, StarsU16 subtype,
    StarsGetCloakPartPoints get_part_points, void *context)
{
    StarsI16 points = 0;
    if (group == 1U && subtype == 8U) points = 20;
    else if (group == 2U && subtype == 6U) points = 40;
    else if (group == 4U && subtype == 4U) points = 70;
    else if (group == 4U && subtype == 6U) points = 20;
    else if (group == 8U && subtype == 7U) points = 50;
    else if (group == 8U && subtype == 9U) points = 40;
    else if (group == 0x10U && subtype == 0x12U) points = 20;
    else if (group == 0x80U && subtype == 6U) points = 60;
    else if (group == 0x80U && subtype == 7U) points = 50;
    else if (group == 0x100U && subtype == 4U) points = 20;
    else if (group == 0x800U && subtype < 5U && get_part_points)
        points = get_part_points(context, group, subtype);
    return points;
}

/* NE 10:88C0-8B0D. Sum hull cloak points, derive the displayed percentage,
 * and report the steal threshold implied by cloak sections 5 and 14. */
StarsI16 stars_pct_cloak_from_huldef(const StarsU8 *hull, StarsI16 player,
    StarsI16 *percent_steal, const StarsCloakSection *sections,
    StarsU16 section_count, const StarsPctCloakOps *ops, void *context)
{
    StarsI16 subtype;
    StarsI16 i;
    StarsI16 points;
    StarsI32 total;
    StarsI16 result;
    if (percent_steal) *percent_steal = 0;
    if (!hull || !sections || !ops || !ops->get_race_grbit ||
            !ops->get_player_stat) return 0;
    if (player == (StarsI16)-1 ||
            (StarsI32)((StarsU16)hull[0] |
                ((StarsU16)hull[1] << 8)) < 0x20L ||
            ops->get_race_grbit(context, player, 3) == 0) {
        total = 0;
    } else {
        total = 40;
    }
    if (player != (StarsI16)-1 &&
            ops->get_player_stat(context, player, 0x0e) == 1)
        total += 300;
    for (i = 0; (StarsU16)i < section_count; ++i) {
        subtype = (StarsI16)(sections[i].flags & 0x00ffU);
        points = cloak_section_points(sections[i].group,
            (StarsU16)subtype, ops->get_part_points, context);
        if ((sections[i].flags >> 8) != 0)
            total += (StarsI32)points * (StarsI32)(sections[i].flags >> 8);
        if (sections[i].group == 2U && percent_steal) {
            if (subtype == 5 && *percent_steal < 70) *percent_steal = 70;
            else if (subtype == 14 && *percent_steal < 80)
                *percent_steal = 80;
        }
    }
    if (total == 0 || total > 25000) return 0;
    if (total < 101) result = (StarsI16)(total >> 1);
    else if (total < 301) result = (StarsI16)((total - 100) / 8L + 50);
    else if (total < 613) result = (StarsI16)((total - 300) / 24L + 75);
    else if (total < 1125) result = (StarsI16)((total - 612) / 64L + 88);
    else if (total < 1612) result = (StarsI16)(total - 612 > 767 ? 97 : 96);
    else result = 98;
    return result;
}

static StarsI16 env_byte(const StarsU8 *data, StarsU16 address)
{
    StarsU8 byte = data[address];
    return byte < 128U ? (StarsI16)byte : (StarsI16)((int)byte - 256);
}

static StarsI16 environment_desirability(const StarsU8 *data, const StarsI16 values[3], StarsI16 player_index)
{
    StarsU16 player = (StarsU16)(0x59a2U + (StarsU16)player_index * 192U);
    StarsU16 axis, rounded = 0;
    StarsI16 environment, center, low, high, distance, width, edge, score, red = 0;
    StarsU32 squares = 0, penalty = 10000UL, boundary;
    for (axis = 0; axis < 3U; ++axis) {
        environment = values[axis];
        center = env_byte(data, (StarsU16)(player + 16U + axis));
        low = env_byte(data, (StarsU16)(player + 19U + axis));
        high = env_byte(data, (StarsU16)(player + 22U + axis));
        if (high < 0) {
            squares += 10000UL;
        } else if (environment < low || environment > high) {
            distance = environment < low ? (StarsI16)(low - environment) : (StarsI16)(environment - high);
            red = (StarsI16)(red + (distance < 15 ? distance : 15));
        } else {
            distance = environment < center ? (StarsI16)(center - environment) : (StarsI16)(environment - center);
            width = environment < center ? (StarsI16)(center - low) : (StarsI16)(high - center);
            score = (StarsI16)(100 - distance * 100 / width);
            squares += (StarsU32)score * (StarsU32)score;
            edge = (StarsI16)(distance * 2 - width);
            if (edge > 0)
                penalty = penalty * (StarsU32)(width * 2 - edge) / (StarsU32)(width * 2);
        }
    }
    if (red) return (StarsI16)-red;
    /* The original biases sqrt(squares/3) upward by adding 0.9 then
     * truncating. Compare squares against each n+0.1 threshold exactly.
     * No equality is possible: 3*(10*n+1)^2 is never divisible by 100. */
    for (;;) {
        boundary = (StarsU32)(rounded * 10U + 1U);
        if (100UL * squares < 3UL * boundary * boundary) break;
        ++rounded;
    }
    return (StarsI16)((StarsU32)rounded * penalty / 10000UL);
}

StarsI16 stars_planet_desirability(const StarsU8 *data, StarsU16 planet, StarsI16 player_index)
{
    StarsI16 values[3];
    StarsU16 axis;
    for (axis = 0; axis < 3U; ++axis)
        values[axis] = env_byte(data, (StarsU16)(planet + 12U + axis));
    return environment_desirability(data, values, player_index);
}

static void copy_player_zero(StarsU8 *data, StarsU16 player)
{
    StarsU16 i;
    StarsU8 first, second;
    for (i = 0; i < 192U; i += 2U) {
        first = data[(StarsU16)(player + i)];
        second = data[(StarsU16)(player + i + 1U)];
        data[0x59a2U + i] = first;
        data[0x59a3U + i] = second;
    }
}

static StarsI16 innate_sample(StarsI16 start, StarsI16 span, StarsI16 count,
    StarsI16 index, StarsI16 center, StarsI16 phase, StarsI16 adjust, StarsI16 *delta)
{
    StarsI16 value = start;
    if (index && count >= 2) value = (StarsI16)(start + index * span / (count - 1));
    if (phase && center >= 0) {
        *delta = (StarsI16)(center - value);
        if (*delta >= -adjust && *delta <= adjust) *delta = 0;
        else if (*delta < 0) *delta = (StarsI16)(*delta + adjust);
        else *delta = (StarsI16)(*delta - adjust);
        value = (StarsI16)(center - *delta);
    }
    return value;
}

/* NE 29:4C6E-54D4. Host semantic adapter; see innate-habitability.md.
 * Explicit positive floating arithmetic preserves x87 operations and double
 * stores independently of the host floating-point precision. */
StarsI32 stars_innate_race_habitability(StarsU8 *data, StarsU16 player)
{
    StarsU8 saved[192];
    StarsI16 center[3], start[3], span[3], count[3], delta[3], values[3];
    StarsI16 phase, axis, x, y, z, low, high, end, adjust, trait, excess;
    StarsU16 i;
    StarsI32 desirability, inner;
    InnateFP total = innate_fp_integer(0), outer, row;
    static const StarsI16 weights[3] = {7, 5, 6};
    for (i = 0; i < 192U; ++i) saved[i] = data[0x59a2U + i];
    trait = stars_get_race_trait(data, player, 1);
    copy_player_zero(data, player);
    for (axis = 0; axis < 3; ++axis) delta[axis] = 0;
    for (phase = 0; phase < 3; ++phase) {
        adjust = phase == 0 ? 0 : (phase == 1 ? (trait ? 8 : 5) : (trait ? 17 : 15));
        for (axis = 0; axis < 3; ++axis) {
            center[axis] = env_byte(data, (StarsU16)(player + 16U + axis));
            low = env_byte(data, (StarsU16)(player + 19U + axis));
            high = env_byte(data, (StarsU16)(player + 22U + axis));
            if ((center[axis] < 0 || center[axis] > 100 || low < 0 || low > 100 || high < 0 || high > 100)
                && !(center[axis] == -1 && low == -1 && high == -1)) {
                data[(StarsU16)(player + 22U + axis)] = 255U;
                data[(StarsU16)(player + 19U + axis)] = 255U;
                data[(StarsU16)(player + 16U + axis)] = 255U;
                data[(StarsU16)(player + 84U)] |= 16U;
                center[axis] = low = high = -1;
                copy_player_zero(data, player);
            }
            if (center[axis] < 0) {
                start[axis] = 50; span[axis] = 11; count[axis] = 1;
            } else {
                start[axis] = (StarsI16)(low > adjust ? low - adjust : 0);
                end = (StarsI16)(high + adjust < 100 ? high + adjust : 100);
                span[axis] = (StarsI16)(end - start[axis]); count[axis] = 11;
            }
        }
        outer = innate_fp_integer(0);
        for (x = 0; x < count[0]; ++x) {
            values[0] = innate_sample(start[0], span[0], count[0], x, center[0], phase, adjust, &delta[0]);
            row = innate_fp_integer(0);
            for (y = 0; y < count[1]; ++y) {
                values[1] = innate_sample(start[1], span[1], count[1], y, center[1], phase, adjust, &delta[1]);
                inner = 0;
                for (z = 0; z < count[2]; ++z) {
                    values[2] = innate_sample(start[2], span[2], count[2], z, center[2], phase, adjust, &delta[2]);
                    desirability = environment_desirability(data, values, 0);
                    excess = (StarsI16)(delta[0] + delta[1] + delta[2] - adjust);
                    if (excess > 0) {
                        desirability -= excess;
                        if (desirability < 0) desirability = 0;
                    }
                    inner += desirability * desirability * weights[phase];
                }
                if (center[2] < 0) inner *= 11;
                else inner = inner * span[2] / 100;
                row = innate_fp_store(innate_fp_add(row, innate_fp_integer((StarsU32)inner)));
            }
            row = innate_fp_scale(row, center[1], span[1]);
            outer = innate_fp_store(innate_fp_add(outer, row));
        }
        outer = innate_fp_scale(outer, center[0], span[0]);
        total = innate_fp_store(innate_fp_add(total, outer));
    }
    if (player != 0x59a2U)
        for (i = 0; i < 192U; ++i) data[0x59a2U + i] = saved[i];
    return innate_fp_result(total);
}
