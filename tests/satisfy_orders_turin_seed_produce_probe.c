#include "core.h"
#include "satisfy_orders_turin_seed_produce_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Trace {
    StarsProduceItem queue[4];
    StarsI16 queue_calls;
    StarsI16 finish_calls;
    StarsI16 production_changed;
    StarsI16 planet_desirability;
    StarsI16 build_calls;
    StarsI16 object_calls;
    StarsI16 mining_draw[3];
    StarsI16 mining_draw_count;
    StarsI16 mining_failures;
    StarsI32 mineral_stock[3];
} Trace;

static StarsI16 mining_random(void *context, StarsI16 limit)
{
    Trace *trace;
    StarsI16 index;

    trace = (Trace *)context;
    index = trace->mining_draw_count++;
    if (limit != 100 || index < 0 || index >= 3) {
        ++trace->mining_failures;
        return 99;
    }
    return trace->mining_draw[index];
}

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
        fprintf(stderr, "SO-E3 Turin seed/Produce check failed at line %d\n",
            line);
    }
}

#define check(condition) check_at((condition), __LINE__)

static StarsI16 raw_production_slot_open(StarsU16 planet_flags,
    StarsU32 population)
{
    return (StarsI16)(((planet_flags & 0x0200U) != 0U) &&
        population >= 200UL);
}

static StarsI16 planet_desirability(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 player)
{
    Trace *trace;
    StarsU8 data[65536];
    StarsI16 result;

    trace = (Trace *)context;
    if (planet->id != E3_PLANET_ID || player != E3_PLANET_OWNER)
        return -1;
    memset(data, 0, sizeof data);
    memcpy(data + 12U, E3_PLANET_ENVIRONMENT, 3U);
    memcpy(data + 0x59a2U + 192U + 16U,
        E3_PLAYER_ENVIRONMENT, 9U);
    result = stars_planet_desirability(data, 0U, player);
    trace->planet_desirability = result;
    return result;
}

static StarsI16 queue_production(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 design, StarsI16 quantity)
{
    Trace *trace;
    StarsProduceItem *item;

    trace = (Trace *)context;
    if (planet->id != E3_PLANET_ID || planet->owner != E3_PLANET_OWNER ||
            design != 0 ||
            quantity != 1 || trace->queue_calls >= 4)
        return 0;
    item = &trace->queue[trace->queue_calls];
    memset(item, 0, sizeof *item);
    item->type = design;
    item->object_kind = 2;
    item->quantity = quantity;
    item->active = 1;
    ++trace->queue_calls;
    return 1;
}

static StarsI16 finish_production(void *context,
    const StarsTurinTurnPlanet *planet, StarsI16 changed)
{
    Trace *trace;

    trace = (Trace *)context;
    if (planet->id != E3_PLANET_ID || planet->owner != E3_PLANET_OWNER)
        return 0;
    ++trace->finish_calls;
    trace->production_changed = changed;
    return 1;
}

static StarsI32 resources_at_planet(void *context,
    const StarsProducePlanet *planet, StarsI16 owner)
{
    (void)context;
    (void)owner;
    return planet->resources;
}

static StarsI16 max_capacity(void *context,
    const StarsProducePlanet *planet, StarsI16 type, StarsI16 operable)
{
    (void)context;
    (void)planet;
    (void)type;
    (void)operable;
    return 3;
}

static StarsI16 costs(void *context, const StarsProducePlanet *planet,
    const StarsProduceItem *item, StarsI32 result[4])
{
    StarsI16 index;
    (void)context;
    (void)planet;
    (void)item;
    for (index = 0; index < 4; ++index)
        result[index] = E3_PRODUCTION_COSTS[index];
    return 1;
}

static StarsI16 build_capacity(void *context,
    const StarsProducePlanet *planet, StarsI16 type, StarsI16 operable)
{
    (void)context;
    (void)planet;
    (void)type;
    (void)operable;
    return 3;
}

static StarsI16 build_object(void *context, StarsProducePlanet *planet,
    StarsI16 object_kind, StarsI16 type, StarsI16 count,
    StarsI32 minerals[3])
{
    Trace *trace;

    (void)planet;
    (void)object_kind;
    (void)type;
    (void)count;
    (void)minerals;
    trace = (Trace *)context;
    ++trace->object_calls;
    return 1;
}

static StarsI16 build_item(void *context, StarsProducePlanet *planet,
    StarsProduceItem *item, StarsProduceItem *partial,
    StarsI32 resources[4], StarsI16 *status)
{
    Trace *trace;
    StarsBuildProductionOps build_ops;
    StarsBuildProductionInput build_input;
    StarsI16 index;
    StarsI16 built;

    trace = (Trace *)context;
    memset(&build_ops, 0, sizeof build_ops);
    memset(&build_input, 0, sizeof build_input);
    build_ops.costs = costs;
    build_ops.capacity = build_capacity;
    build_ops.build_object = build_object;
    build_ops.context = context;
    build_input.planet = planet;
    build_input.item = item;
    build_input.partial = partial;
    build_input.status = status;
    build_input.ops = &build_ops;
    for (index = 0; index < 4; ++index)
        build_input.resources[index] = resources[index];
    built = stars_build_production_item(&build_input);
    for (index = 0; index < 4; ++index)
        resources[index] = build_input.resources[index];
    ++trace->build_calls;
    return built;
}

int main(int argc, char **argv)
{
    Trace trace;
    StarsTurinDroneAiTurnOps turin_ops;
    StarsTurinDroneAiTurnInput turin_input;
    StarsTurinTurnPlanet turin_planet;
    StarsMineralMinedOps mining_ops;
    StarsMineralMinedInput mining_input;
    StarsProduceOps produce_ops;
    StarsProduceInput produce_input;
    StarsProducePlanet produce_planet;
    StarsI16 result;

    if (argc != 4)
        return 2;
    memset(&trace, 0, sizeof trace);
    memset(&turin_ops, 0, sizeof turin_ops);
    memset(&turin_input, 0, sizeof turin_input);
    memset(&turin_planet, 0, sizeof turin_planet);
    memset(&produce_ops, 0, sizeof produce_ops);
    memset(&produce_input, 0, sizeof produce_input);
    memset(&produce_planet, 0, sizeof produce_planet);
    memset(&mining_ops, 0, sizeof mining_ops);
    memset(&mining_input, 0, sizeof mining_input);
    trace.mining_draw[0] = (StarsI16)atoi(argv[1]);
    trace.mining_draw[1] = (StarsI16)atoi(argv[2]);
    trace.mining_draw[2] = (StarsI16)atoi(argv[3]);
    checks = failures = 0;

    turin_ops.queue_production = queue_production;
    turin_ops.finish_production = finish_production;
    turin_ops.planet_desirability = planet_desirability;
    turin_ops.context = &trace;
    turin_planet.id = E3_PLANET_ID;
    turin_planet.owner = E3_PLANET_OWNER;
    turin_planet.production_slot_open = raw_production_slot_open(
        E3_PLANET_FLAGS, E3_PLANET_POPULATION);
    turin_input.turn = 0;
    turin_input.player_id = E3_PLANET_OWNER;
    turin_input.planet_count = 1;
    turin_input.planets = &turin_planet;
    turin_input.plan_max = E3_MAP_POINT_COUNT;
    turin_input.ops = &turin_ops;

    check(turin_planet.production_slot_open == 1);
    result = stars_do_turin_drone_ai_turn(&turin_input);
    check(result == 1);
    check(trace.planet_desirability == 100);
    check(trace.queue_calls == 2 && turin_input.production_hulls_queued == 2);
    check(trace.finish_calls == 1 && trace.production_changed == 1);
    check(trace.queue[0].type == 0 && trace.queue[0].object_kind == 2 &&
        trace.queue[0].quantity == 1 && trace.queue[0].progress == 0);
    check(trace.queue[1].type == 0 && trace.queue[1].object_kind == 2 &&
        trace.queue[1].quantity == 1 && trace.queue[1].progress == 0);

    /* The task-5 merge/scrap slice pins these retained post-scrap stocks.
     * Mine through current source, then feed those exact results to Produce. */
    memcpy(trace.mineral_stock, E3_POST_TASK5_MINERALS,
        sizeof trace.mineral_stock);
    mining_input.player = E3_PLANET_OWNER;
    mining_input.special_race = E3_MINE_SPECIAL_RACE;
    mining_input.efficiency_tenths = E3_MINE_EFFICIENCY_TENTHS;
    mining_input.operating_mines = E3_MINE_OPERATING_MINES;
    mining_input.c_mines = -1;
    memcpy(mining_input.concentration, E3_MINE_CONCENTRATION,
        sizeof mining_input.concentration);
    memcpy(mining_input.percent_level, E3_MINE_PERCENT_LEVEL,
        sizeof mining_input.percent_level);
    mining_input.random_rounding = 1;
    mining_input.apply = 1;
    mining_input.mineral_weight = trace.mineral_stock;
    mining_input.ops = &mining_ops;
    mining_ops.random = mining_random;
    mining_ops.context = &trace;
    stars_est_minerals_mined(&mining_input);
    check(trace.mining_draw_count == 3 && trace.mining_failures == 0);

    produce_planet.id = E3_PLANET_ID;
    produce_planet.owner = E3_PLANET_OWNER;
    produce_planet.resources = 32;
    produce_planet.research_percent = 15;
    produce_planet.mineral[0] = trace.mineral_stock[0];
    produce_planet.mineral[1] = trace.mineral_stock[1];
    produce_planet.mineral[2] = trace.mineral_stock[2];
    produce_planet.items = trace.queue;
    produce_planet.item_count = trace.queue_calls;
    produce_planet.item_capacity = 4;
    produce_ops.context = &trace;
    produce_ops.resources_at_planet = resources_at_planet;
    produce_ops.max_capacity = max_capacity;
    produce_ops.build_item_with_resources = build_item;
    produce_input.planets = &produce_planet;
    produce_input.planet_count = 1;
    produce_input.ops = &produce_ops;
    stars_produce(&produce_input);

    check(trace.build_calls == 1 && trace.object_calls == 0);
    check(produce_planet.item_count == 2 &&
        trace.queue[0].progress == 81 && trace.queue[1].progress == 0);
    check(produce_planet.mineral[0] == 592 &&
        produce_planet.mineral[1] ==
            (trace.mining_draw[1] < 70 ? 964 : 963) &&
        produce_planet.mineral[2] == 520);
    check(produce_planet.resources == 32 &&
        produce_planet.last_year_resources == 4);

    printf("SOE3_TURIN_SEED_PRODUCE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    printf("TURIN_PLANET24_DESIRABILITY=%d\n",
        trace.planet_desirability);
    printf("MINE=%ld,%ld,%ld STOCK=%ld,%ld,%ld LEVELS=%u,%u,%u "
        "CONC=%u,%u,%u DRAWS=%d FAILURES=%d\n",
        (long)mining_input.output[0], (long)mining_input.output[1],
        (long)mining_input.output[2], (long)trace.mineral_stock[0],
        (long)trace.mineral_stock[1], (long)trace.mineral_stock[2],
        (unsigned)mining_input.percent_level[0],
        (unsigned)mining_input.percent_level[1],
        (unsigned)mining_input.percent_level[2],
        (unsigned)mining_input.concentration[0],
        (unsigned)mining_input.concentration[1],
        (unsigned)mining_input.concentration[2],
        (int)trace.mining_draw_count, (int)trace.mining_failures);
    printf("PRODUCE_MINERALS=%ld,%ld,%ld ITEMS=%u "
        "ITEM0=%d,%d,%d,%d ITEM1=%d,%d,%d,%d\n",
        (long)produce_planet.mineral[0], (long)produce_planet.mineral[1],
        (long)produce_planet.mineral[2], (unsigned)produce_planet.item_count,
        trace.queue[0].type, trace.queue[0].object_kind,
        trace.queue[0].quantity, trace.queue[0].progress,
        trace.queue[1].type, trace.queue[1].object_kind,
        trace.queue[1].quantity, trace.queue[1].progress);
    return failures == 0 ? 0 : 1;
}
