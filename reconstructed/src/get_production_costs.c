#include "core.h"

#define PRODUCTION_GROUP_SPECIAL 0x8000U
#define PRODUCTION_GROUP_HULL 0x0000U
#define PRODUCTION_KIND_MINERAL 0U
#define PRODUCTION_KIND_SCANNER 1U
#define PRODUCTION_KIND_ARMOR 2U
#define PRODUCTION_KIND_DEFENSE 3U
#define PRODUCTION_KIND_ENGINE 4U
#define PRODUCTION_KIND_FUEL 5U
#define PRODUCTION_KIND_WEAPON 6U
#define PRODUCTION_KIND_TRUE_PART 0x0dU
#define PRODUCTION_KIND_FIRST_SPECIAL 0x0eU
#define PRODUCTION_KIND_LAST_SPECIAL 0x10U
#define PRODUCTION_KIND_FIRST_HULL_PART 0x11U
#define PRODUCTION_KIND_LAST_HULL_PART 0x1bU
#define PRODUCTION_RACE_STAT_ENVIRONMENT 0x0e
#define PRODUCTION_RACE_STAT_MINERAL 5
#define PRODUCTION_RACE_STAT_RESOURCE 2
#define PRODUCTION_RACE_STAT_COST_REDUCTION 0x0e
#define PRODUCTION_GRBIT_ALCHEMY 0x1f
#define PRODUCTION_GRBIT_DEFENSE 6
#define PRODUCTION_GRBIT_FUEL 1
#define PRODUCTION_GRBIT_COST_REDUCTION 3
#define PRODUCTION_GRBIT_ADVANCED_TECH 0x0c
#define PRODUCTION_PLANET_STARBASE 0x0200U
#define PRODUCTION_HULL_HIDDEN 0x0200U
#define PRODUCTION_PART_COSTS 4
#define PRODUCTION_TECH_LOW 6
#define PRODUCTION_TECH_HIGH 7
#define PRODUCTION_TECH_SPECIAL 3
#define PRODUCTION_TECH_ALCHEMY 4
#define PRODUCTION_TECH_COST_REDUCTION 8
#define PRODUCTION_PERCENT_SPECIAL 5
#define PRODUCTION_PERCENT_SAME_COMPONENT 8
#define PRODUCTION_PERCENT_DIFFERENT_COMPONENT 7
#define PRODUCTION_PERCENT_COMPLEMENT 10
#define PRODUCTION_PERCENT_REMAINING 2

static void zero_costs(StarsU32 costs[4])
{
    StarsI16 i;
    for (i = 0; i < PRODUCTION_PART_COSTS; ++i) costs[i] = 0UL;
}

static StarsI16 race_stat(const StarsProductionCostInput *input,
    StarsI16 player, StarsI16 stat)
{
    if (!input->ops || !input->ops->race_stat) return 0;
    return input->ops->race_stat(input->context, player, stat);
}

static StarsI16 race_grbit(const StarsProductionCostInput *input,
    StarsI16 player, StarsI16 bit)
{
    if (!input->ops || !input->ops->race_grbit) return 0;
    return input->ops->race_grbit(input->context, player, bit);
}

static void set_u16_costs(StarsU32 costs[4], const StarsU16 source[4])
{
    StarsI16 i;
    for (i = 0; i < PRODUCTION_PART_COSTS; ++i) costs[i] = source[i];
}

static void scale_costs(StarsU32 costs[4], StarsU16 quantity)
{
    StarsI16 i;
    for (i = 0; i < PRODUCTION_PART_COSTS; ++i)
        costs[i] = costs[i] * (StarsU32)quantity;
}

static void production_part_cost(const StarsProductionCostInput *input,
    StarsU16 group, StarsU16 subtype, StarsU32 costs[4])
{
    StarsTruePart part;
    StarsU16 part_costs[4];
    StarsI16 i;
    zero_costs(costs);
    if (!input->ops || !input->ops->lookup_part || !input->ops->part_cost)
        return;
    for (i = 0; i < PRODUCTION_PART_COSTS; ++i) part_costs[i] = 0U;
    if (input->ops->lookup_part(input->context, group, subtype, &part) == 0)
        return;
    input->ops->part_cost(input->context, input->player, &part, part_costs);
    set_u16_costs(costs, part_costs);
}

static StarsU32 scaled_component_cost(StarsU16 cost, StarsU16 level)
{
    return (StarsU32)cost * (StarsU32)level;
}

static StarsU32 subtract_component(StarsU32 remaining, StarsU32 old_cost,
    StarsU32 new_cost, StarsI16 same_component)
{
    StarsU32 difference;
    StarsU32 usable;
    StarsU32 minimum;
    if (same_component) {
        difference = new_cost >= old_cost ? new_cost - old_cost : 0UL;
    } else {
        minimum = (new_cost * (StarsU32)PRODUCTION_PERCENT_REMAINING) /
            (StarsU32)PRODUCTION_PERCENT_COMPLEMENT;
        difference = new_cost >= (old_cost *
            (StarsU32)PRODUCTION_PERCENT_DIFFERENT_COMPONENT) /
            (StarsU32)PRODUCTION_PERCENT_COMPLEMENT ?
            new_cost - (old_cost * (StarsU32)PRODUCTION_PERCENT_DIFFERENT_COMPONENT) /
            (StarsU32)PRODUCTION_PERCENT_COMPLEMENT : minimum;
        if (difference < minimum) difference = minimum;
    }
    usable = new_cost >= difference ? new_cost - difference : 0UL;
    if (usable > remaining) usable = remaining;
    return remaining - usable;
}

static void adjust_for_existing_starbase(const StarsProductionCostInput *input,
    const StarsProductionHull *candidate, StarsU32 costs[4])
{
    const StarsProductionHull *current;
    StarsTruePart old_part;
    StarsTruePart new_part;
    StarsU16 old_cost[4];
    StarsU16 new_cost[4];
    StarsU32 old_scaled;
    StarsU32 new_scaled;
    StarsU32 difference;
    StarsU32 half;
    StarsU32 usable;
    StarsI16 i;
    StarsI16 slot;
    StarsI16 same;

    current = input->planet_hull;
    if (!current || !candidate) return;
    if (current->hull_id != candidate->hull_id) {
        for (i = 0; i < PRODUCTION_PART_COSTS; ++i) {
            half = costs[i] / 2UL;
            difference = current->base_costs[i] / 2UL;
            usable = costs[i] >= difference ? costs[i] - difference : 0UL;
            costs[i] = usable > half ? usable : half;
        }
        return;
    }

    for (i = 0; i < PRODUCTION_PART_COSTS; ++i) {
        costs[i] = costs[i] >= current->base_costs[i] ?
            costs[i] - current->base_costs[i] : 0UL;
    }
    for (slot = 0; slot < (StarsI16)candidate->component_count &&
            slot < STARS_PRODUCTION_HULL_COMPONENTS; ++slot) {
        const StarsProductionHullComponent *old_component;
        const StarsProductionHullComponent *new_component;
        old_component = &current->components[slot];
        new_component = &candidate->components[slot];
        if (!old_component->present || !new_component->present) continue;
        if (!input->ops || !input->ops->lookup_part ||
                !input->ops->part_cost) continue;
        if (input->ops->lookup_part(input->context, old_component->group,
                old_component->subtype, &old_part) == 0) continue;
        if (input->ops->lookup_part(input->context, new_component->group,
                new_component->subtype, &new_part) == 0) continue;
        input->ops->part_cost(input->context, input->player, &old_part,
            old_cost);
        input->ops->part_cost(input->context, input->player, &new_part,
            new_cost);
        same = (StarsI16)(old_component->group == new_component->group &&
            old_component->subtype == new_component->subtype);
        for (i = 0; i < PRODUCTION_PART_COSTS; ++i) {
            old_scaled = scaled_component_cost(old_cost[i],
                old_component->technology_level);
            new_scaled = scaled_component_cost(new_cost[i],
                new_component->technology_level);
            costs[i] = subtract_component(costs[i], old_scaled, new_scaled,
                same);
        }
    }
}

static void special_costs(const StarsProductionCostInput *input,
    StarsU32 costs[4])
{
    StarsI16 i;
    StarsI16 stat;
    StarsU16 kind;
    zero_costs(costs);
    kind = input->item_kind;
    if (kind == PRODUCTION_KIND_MINERAL || kind == 8U) {
        costs[3] = (StarsU32)(StarsI16)race_stat(input, input->player,
            PRODUCTION_RACE_STAT_MINERAL);
        return;
    }
    if (kind == PRODUCTION_KIND_SCANNER || kind == 7U) {
        i = race_grbit(input, input->player, PRODUCTION_GRBIT_ALCHEMY);
        if ((input->game_flags & 0x0800U) != 0U) {
            for (i = 0; i < 4; ++i) costs[i] = (StarsU32)(2 -
                race_grbit(input, input->player, PRODUCTION_GRBIT_ALCHEMY));
        } else {
            costs[2] = (StarsU32)(4 - i);
        }
        costs[3] = (StarsU32)(StarsI16)race_stat(input, input->player,
            PRODUCTION_RACE_STAT_RESOURCE);
        return;
    }
    if (kind == PRODUCTION_KIND_ARMOR || kind == 9U) {
        production_part_cost(input, PRODUCTION_GROUP_SPECIAL, 9U, costs);
        stat = race_stat(input, input->player,
            PRODUCTION_RACE_STAT_COST_REDUCTION);
        if (stat == PRODUCTION_TECH_ALCHEMY)
            for (i = 0; i < 4; ++i) costs[i] = costs[i] * 3UL / 5UL;
        return;
    }
    if (kind == PRODUCTION_KIND_DEFENSE || kind == 0x0bU) {
        costs[3] = (StarsU32)(race_grbit(input, input->player,
            PRODUCTION_GRBIT_DEFENSE) ? 25 : 100);
        if (race_stat(input, input->planet_player,
                PRODUCTION_RACE_STAT_COST_REDUCTION) == PRODUCTION_TECH_SPECIAL)
            costs[3] /= 2UL;
        return;
    }
    if (kind == PRODUCTION_KIND_ENGINE || kind == PRODUCTION_KIND_FUEL ||
            kind == 0x0cU) {
        costs[3] = (StarsU32)(race_grbit(input, input->player,
            PRODUCTION_GRBIT_FUEL) ? 70 : 100);
        if (race_stat(input, input->planet_player,
                PRODUCTION_RACE_STAT_COST_REDUCTION) == PRODUCTION_TECH_SPECIAL)
            costs[3] /= 2UL;
        return;
    }
    if (kind == PRODUCTION_KIND_WEAPON || kind == 0x0aU) {
        stat = race_stat(input, input->planet_player,
            PRODUCTION_RACE_STAT_COST_REDUCTION);
        costs[0] = costs[1] = costs[2] = stat == PRODUCTION_TECH_LOW ? 25UL :
            stat == PRODUCTION_TECH_HIGH ? 48UL : 44UL;
        costs[3] = stat == PRODUCTION_TECH_LOW ? 5UL : 10UL;
        return;
    }
    if (kind == PRODUCTION_KIND_TRUE_PART) {
        production_part_cost(input, PRODUCTION_GROUP_SPECIAL, 0x0eU, costs);
        return;
    }
    if (kind >= PRODUCTION_KIND_FIRST_SPECIAL &&
            kind <= PRODUCTION_KIND_LAST_SPECIAL) {
        stat = race_stat(input, input->planet_player,
            PRODUCTION_RACE_STAT_COST_REDUCTION);
        costs[kind - PRODUCTION_KIND_FIRST_SPECIAL] =
            stat == PRODUCTION_TECH_LOW ? 70UL : stat == PRODUCTION_TECH_HIGH ?
            120UL : 110UL;
        costs[3] = stat == PRODUCTION_TECH_LOW ? 5UL : 10UL;
        return;
    }
    if (kind >= PRODUCTION_KIND_FIRST_HULL_PART &&
            kind <= PRODUCTION_KIND_LAST_HULL_PART) {
        if (kind == PRODUCTION_KIND_LAST_HULL_PART) kind =
            PRODUCTION_KIND_FIRST_HULL_PART + 1U;
        production_part_cost(input, PRODUCTION_GROUP_SPECIAL,
            (StarsU16)(kind - PRODUCTION_KIND_FIRST_HULL_PART), costs);
    }
}

void stars_get_production_costs(const StarsProductionCostInput *input,
    StarsU32 costs[4])
{
    StarsProductionHull hull;
    StarsI16 special;
    StarsI16 stat;
    StarsI16 i;
    StarsU32 hull_costs[4];

    if (!input || !costs) return;
    zero_costs(costs);
    if (input->item_variant != 2U) {
        special_costs(input, costs);
    } else {
        special = (StarsI16)(input->item_kind >= 0x10U);
        if (input->ops && input->ops->lookup_hull &&
                input->ops->lookup_hull(input->context, input->player,
                    (StarsU16)(input->item_kind - (special ? 0x10U : 0U)),
                    special, &hull) != 0) {
            if ((hull.hidden_flags & PRODUCTION_HULL_HIDDEN) != 0U) {
                zero_costs(costs);
                return;
            }
            for (i = 0; i < PRODUCTION_PART_COSTS; ++i)
                hull_costs[i] = hull.base_costs[i];
            if (special && (input->planet_flags & PRODUCTION_PLANET_STARBASE))
                adjust_for_existing_starbase(input, &hull, hull_costs);
            stat = race_stat(input, input->player,
                PRODUCTION_RACE_STAT_COST_REDUCTION);
            if (special && (race_grbit(input, input->player,
                    PRODUCTION_GRBIT_COST_REDUCTION) != 0 || stat == 8)) {
                for (i = 0; i < PRODUCTION_PART_COSTS; ++i)
                    hull_costs[i] -= hull_costs[i] / 5UL;
            }
            if (special) {
                for (i = 0; i < PRODUCTION_PART_COSTS; ++i)
                    costs[i] = (hull_costs[i] + 1UL) / 2UL;
            } else {
                for (i = 0; i < PRODUCTION_PART_COSTS; ++i)
                    costs[i] = hull_costs[i];
            }
        }
    }
    if (!input->only_one) scale_costs(costs, input->quantity);
}
