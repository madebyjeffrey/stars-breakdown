#include "core.h"
#include <stdio.h>
#include <string.h>

/* Retained selector-3 planet 24 after the AI supply transfer. The host
 * coordinator checks the full file streams; this packet carries its exact
 * queue and resource boundary through the same source functions in Win16. */
typedef struct RototillNativeProduction {
    StarsI16 failures;
    StarsI16 adds;
    StarsI16 builds;
    StarsI16 resource_calls;
    StarsI16 population_calls;
    StarsI16 research_calls;
    StarsI16 cost_calls;
    StarsI16 max_population_calls;
    StarsI16 random_event_calls;
    StarsI16 selection_calls;
    StarsI16 initialize_calls;
    StarsI16 fill_calls;
    StarsI16 finish_calls;
    StarsU16 queued_quantity;
    StarsU8 raw_planet[56];
    StarsProduceItem item;
    StarsProducePlanet planet;
} RototillNativeProduction;

/* Player-1 race bytes from the retained turn-zero CLEAN.HST type-6 record.
 * The planet has environment 62/33/61, exactly at the race center. */
static const StarsU8 native_race_stats[16] = {
    10, 10, 10, 10, 10, 5, 9, 0, 1, 1, 1, 1, 1, 1, 1, 0
};
static const StarsU8 native_race_environment[9] = {
    62, 33, 61, 32, 6, 26, 92, 60, 96
};
static const StarsU32 native_race_bits = 0x80002045UL;

/* planet.c's unrelated innate-race generator refers to this table service.
 * The retained production path must never enter it. */
StarsI16 stars_get_race_trait(const StarsU8 *data, StarsU16 player,
    StarsI16 index)
{
    (void)data; (void)player; (void)index;
    return 0;
}

static void native_word(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static void native_dword(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    native_word(raw, offset, (StarsU16)value);
    native_word(raw, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

static StarsI16 native_race_stat(void *opaque, StarsI16 player, StarsI16 stat)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    if (player != 1 || stat < 0 || stat >= 16) {
        ++run->failures;
        return 0;
    }
    return (StarsI16)(signed char)native_race_stats[stat];
}

static StarsI16 native_race_grbit(void *opaque, StarsI16 player, StarsI16 bit)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    if (player != 1 || bit < 0 || bit >= 32) {
        ++run->failures;
        return 0;
    }
    return (StarsI16)((native_race_bits >> bit) & 1UL);
}

static StarsI16 native_desirability(void *opaque, const StarsU8 *planet,
    StarsI16 player)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    StarsI16 axis;
    if (planet != run->raw_planet || player != 1) ++run->failures;
    for (axis = 0; axis < 3; ++axis)
        if (planet[12 + axis] != native_race_environment[axis])
            ++run->failures;
    return 100;
}

static StarsI32 native_max_population(void *opaque, StarsU16 planet_id,
    StarsI16 player)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    StarsCalcPlanetMaxPopInput input;
    ++run->max_population_calls;
    if (planet_id != 24U || player != 1) ++run->failures;
    memset(&input, 0, sizeof input);
    input.planet_owner = 1;
    input.player = player;
    input.race_stat = native_race_stat(opaque, player, 14);
    input.race_grbit = native_race_grbit(opaque, player, 9);
    input.desirability = native_desirability(opaque, run->raw_planet, player);
    return stars_calc_planet_max_pop(&input);
}

static StarsI16 native_real_resources(RototillNativeProduction *run)
{
    StarsCResourcesAtPlanetOps ops;
    StarsI16 result;
    memset(&ops, 0, sizeof ops);
    ops.calc_max_population = native_max_population;
    ops.planet_desirability = native_desirability;
    ops.get_player_stat = native_race_stat;
    ops.factory_calc_max_population = native_max_population;
    ops.factory_get_player_stat = native_race_stat;
    result = stars_cresources_at_planet(run->raw_planet, 1, &ops, run);
    if (result != 32) ++run->failures;
    return result;
}

static void native_compute_costs(RototillNativeProduction *run,
    StarsU16 kind, StarsU16 variant, StarsU16 quantity,
    StarsI32 costs[4])
{
    StarsProductionCostInput input;
    StarsProductionCostOps ops;
    StarsU32 result[4];
    StarsI16 lane;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    input.player = 1;
    input.planet_player = 1;
    input.item_kind = kind;
    input.item_variant = variant;
    input.quantity = quantity;
    input.only_one = 1;
    ops.race_stat = native_race_stat;
    ops.race_grbit = native_race_grbit;
    input.ops = &ops;
    input.context = run;
    stars_get_production_costs(&input, result);
    for (lane = 0; lane < 4; ++lane) costs[lane] = (StarsI32)result[lane];
    if (costs[0] != 0L || costs[1] != 0L ||
            costs[2] != 3L || costs[3] != 10L)
        ++run->failures;
    ++run->cost_calls;
}

static void native_costs(void *opaque, StarsU32 packed, StarsI32 costs[4])
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    if (packed != 0x00021c01UL) ++run->failures;
    native_compute_costs(run, (StarsU16)((packed >> 10) & 127UL),
        (StarsU16)((packed >> 17) & 7UL), (StarsU16)(packed & 1023UL),
        costs);
}

static StarsI16 native_add(void *opaque, StarsI16 kind,
    StarsU16 quantity, StarsI16 now, StarsI16 flags)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    ++run->adds;
    if (kind != 7 || quantity != 3U || now != 1 || flags != 1 ||
            run->adds != 1)
        ++run->failures;
    run->queued_quantity = quantity;
    return 1;
}

static void native_selection(void *opaque, StarsI16 kind, StarsI16 id)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    ++run->selection_calls;
    if (kind != 1 || id != 24 || run->selection_calls != 1)
        ++run->failures;
}

static void native_initialize(void *opaque, StarsU8 *scratch, StarsU16 size)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    ++run->initialize_calls;
    if (!scratch || size != 256U || run->selection_calls != 1 ||
            run->initialize_calls != 1)
        ++run->failures;
}

static void native_estimate(void *opaque, const StarsU8 *planet,
    StarsI32 minerals[3], StarsI16 player, StarsI16 mode)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    if (planet != run->raw_planet || player != -1 || mode != 0)
        ++run->failures;
    minerals[0] = minerals[1] = minerals[2] = 0L;
}

static StarsI16 native_available_resources(void *opaque,
    const StarsU8 *planet, StarsI16 player)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    if (planet != run->raw_planet || player != 1 ||
            planet[40] != 225U || planet[41] != 0U)
        ++run->failures;
    return native_real_resources(run);
}

static StarsI16 native_ai_research(void *opaque, StarsI16 player)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    if (player != 1) ++run->failures;
    return 0;
}

static StarsI16 native_fill(void *opaque, const StarsU8 *planet)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    StarsGetResourcesAvailableOps resources;
    StarsFillProdMinesAndFactoriesInput fill;
    static const StarsU32 catalog[1] = {0x00021c01UL};
    ++run->fill_calls;
    if (planet != run->raw_planet || run->initialize_calls != 1 ||
            run->fill_calls != 1)
        ++run->failures;
    memset(&resources, 0, sizeof resources);
    memset(&fill, 0, sizeof fill);
    resources.estimate_minerals = native_estimate;
    resources.resources_at_planet = native_available_resources;
    resources.research_percent = native_ai_research;
    stars_get_resources_available(planet, fill.available, 1, 0UL,
        &resources, run);
    if (fill.available[0] != 538L || fill.available[1] != 964L ||
            fill.available[2] != 506L || fill.available[3] != 32L)
        ++run->failures;
    fill.max_mines = 22;
    fill.mines_operating = 10;
    fill.max_factories = 22;
    fill.factories_operating = 10;
    fill.global_queue_items = catalog;
    fill.global_queue_count = 1U;
    fill.get_costs = native_costs;
    fill.add_item = native_add;
    fill.context = run;
    return stars_fill_prod_mines_and_factories(&fill);
}

static void native_finish(void *opaque, StarsI16 changed)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    ++run->finish_calls;
    if (changed != 1 || run->adds != 1 || run->finish_calls != 1)
        ++run->failures;
}

static StarsI32 native_resources(void *opaque,
    const StarsProducePlanet *planet, StarsI16 owner)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    ++run->resource_calls;
    if (planet != &run->planet || owner != 1 ||
            planet->id != 24 || planet->factory_count != 10 ||
            planet->research_percent != 15)
        ++run->failures;
    return native_real_resources(run);
}

static StarsI16 native_build_costs(void *opaque,
    const StarsProducePlanet *planet, const StarsProduceItem *item,
    StarsI32 costs[4])
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    if (planet != &run->planet || item != &run->item ||
            item->type != 7 || item->object_kind != 1)
        ++run->failures;
    native_compute_costs(run, (StarsU16)item->type,
        (StarsU16)item->object_kind, 1U, costs);
    return 1;
}

static StarsI16 native_build_object(void *opaque,
    StarsProducePlanet *planet, StarsI16 group, StarsI16 kind,
    StarsI16 quantity, StarsI32 minerals[3])
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    (void)minerals;
    if (planet != &run->planet || group != 1 || kind != 7 ||
            quantity != 1 || planet->factory_count >= 12)
        ++run->failures;
    ++planet->factory_count;
    ++run->builds;
    return 1;
}

static StarsI16 native_build_item(void *opaque,
    StarsProducePlanet *planet, StarsProduceItem *item,
    StarsProduceItem *partial, StarsI32 resources[4], StarsI16 *status)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    StarsBuildProductionInput input;
    StarsBuildProductionOps ops;
    StarsI16 lane, built;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    ops.costs = native_build_costs;
    ops.build_object = native_build_object;
    ops.context = run;
    input.planet = planet;
    input.item = item;
    input.partial = partial;
    input.status = status;
    input.ops = &ops;
    for (lane = 0; lane < 4; ++lane) input.resources[lane] = resources[lane];
    built = stars_build_production_item(&input);
    for (lane = 0; lane < 4; ++lane) resources[lane] = input.resources[lane];
    return built;
}

static void native_population(void *opaque, StarsProducePlanet *planet)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    if (planet != &run->planet) ++run->failures;
    ++run->population_calls;
}

static void native_research(void *opaque, StarsI16 mode)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    if (mode != 1) ++run->failures;
    ++run->research_calls;
}

static void native_random_events(void *opaque)
{
    RototillNativeProduction *run = (RototillNativeProduction *)opaque;
    ++run->random_event_calls;
}

int rototill_native_production_probe(void)
{
    RototillNativeProduction run;
    StarsProductionQueueOps queue_ops;
    const StarsU8 *planets[1];
    StarsProduceOps ops;
    StarsProduceInput input;
    memset(&run, 0, sizeof run);
    memset(&queue_ops, 0, sizeof queue_ops);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);

    /* AI phase: 225 colonists and zero research leave 32 resources. */
    native_word(run.raw_planet, 0U, 24U);
    native_word(run.raw_planet, 2U, 1U);
    native_word(run.raw_planet, 4U, 11143U);
    memcpy(run.raw_planet + 12U, native_race_environment, 3U);
    native_dword(run.raw_planet, 20U, 10UL << 20);
    native_dword(run.raw_planet, 28U, 538UL);
    native_dword(run.raw_planet, 32U, 964UL);
    native_dword(run.raw_planet, 36U, 506UL);
    native_dword(run.raw_planet, 40U, 225UL);
    planets[0] = run.raw_planet;
    queue_ops.change_selection = native_selection;
    queue_ops.initialize = native_initialize;
    queue_ops.fill = native_fill;
    queue_ops.finish = native_finish;
    stars_fill_production_queue(planets, 1U, &queue_ops, &run);
    if (run.selection_calls != 1 || run.initialize_calls != 1 ||
            run.fill_calls != 1 || run.finish_calls != 1 ||
            run.adds != 1 || run.queued_quantity != 3U)
        ++run.failures;

    /* FGenerateTurn reloads HST's 15% research before Produce. Its mining
     * pass enters this boundary with mineral stocks 539/965/506. */
    run.item.type = 7;
    run.item.object_kind = 1;
    run.item.quantity = (StarsI16)run.queued_quantity;
    run.item.active = 1;
    run.planet.id = 24;
    run.planet.owner = 1;
    run.planet.research_percent = 15;
    run.planet.factory_count = 10;
    run.planet.mine_count = 10;
    run.planet.mineral[0] = 539L;
    run.planet.mineral[1] = 965L;
    run.planet.mineral[2] = 506L;
    run.planet.items = &run.item;
    run.planet.item_count = 1U;
    run.planet.item_capacity = 1U;
    ops.resources_at_planet = native_resources;
    ops.build_item_with_resources = native_build_item;
    ops.update_populations = native_population;
    ops.update_research = native_research;
    ops.random_events = native_random_events;
    ops.context = &run;
    input.planets = &run.planet;
    input.planet_count = 1U;
    input.ops = &ops;
    stars_produce(&input);
    if (run.builds != 2 || run.planet.factory_count != 12 ||
            run.item.quantity != 1 || run.item.progress != 89 ||
            run.planet.mineral[0] != 539L ||
            run.planet.mineral[1] != 965L ||
            run.planet.mineral[2] != 498L ||
            run.planet.last_year_resources != 4L ||
            run.resource_calls != 1 || run.max_population_calls < 2 ||
            run.cost_calls < 2 || run.population_calls != 1 ||
            run.research_calls != 1 || run.random_event_calls != 1)
        ++run.failures;
    return run.failures;
}

#ifndef TARGET_NATIVE
int main(void)
{
    int failures = rototill_native_production_probe();
    printf("ROTOTILL_NATIVE_PRODUCTION_FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
#endif
