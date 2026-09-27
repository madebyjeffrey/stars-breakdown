#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedPlanet {
    StarsU8 raw[56];
    StarsI32 weight[3];
} PackedPlanet;

typedef struct Trace {
    StarsI32 operating_mines;
    StarsI16 operating_calls;
    StarsI16 random_value;
    StarsI16 random_calls;
    StarsI32 fleet_values[3];
    StarsU8 fleet_raw[3][124];
    StarsU8 order_raw[3][4];
    StarsI16 packed_fleets;
    StarsI16 planet_id;
    StarsI16 planet_player;
    StarsI16 fleet_calls;
    StarsI16 consumed_calls;
    StarsI16 last_consumed;
} Trace;

static StarsU16 get_word(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU16)(p[offset] | ((StarsU16)p[offset + 1U] << 8));
}

static StarsI32 get_long(const StarsU8 *p, StarsU16 offset)
{
    StarsU32 value = (StarsU32)get_word(p, offset) |
        ((StarsU32)get_word(p, (StarsU16)(offset + 2U)) << 16);
    return (StarsI32)value;
}

static void put_word(StarsU8 *p, StarsU16 offset, StarsU16 value)
{
    p[offset] = (StarsU8)value;
    p[offset + 1U] = (StarsU8)(value >> 8);
}

static void put_long(StarsU8 *p, StarsU16 offset, StarsI32 value)
{
    StarsU32 u = (StarsU32)value;
    put_word(p, offset, (StarsU16)u);
    put_word(p, (StarsU16)(offset + 2U), (StarsU16)(u >> 16));
}

static StarsI32 multiply(void *context, StarsI32 left, StarsI32 right)
{
    (void)context;
    return left * right;
}

static StarsI32 divide_fixed(void *context, StarsI32 value, StarsI32 divisor)
{
    (void)context;
    return value / divisor;
}

static StarsI32 remainder_fixed(void *context, StarsI32 value,
    StarsI32 divisor)
{
    (void)context;
    return value % divisor;
}

static StarsI32 operating_mines(void *context)
{
    Trace *trace = (Trace *)context;
    ++trace->operating_calls;
    return trace->operating_mines;
}

static StarsI16 random_value(void *context, StarsI16 limit)
{
    Trace *trace = (Trace *)context;
    (void)limit;
    ++trace->random_calls;
    return trace->random_value;
}

static StarsI16 fleet_mines(void *context, StarsI16 index,
    StarsI32 *mines)
{
    Trace *trace = (Trace *)context;
    ++trace->fleet_calls;
    if (index < 0 || index >= 3)
        return 0;
    *mines = trace->fleet_values[index];
    if (trace->packed_fleets) {
        if (get_word(trace->fleet_raw[index], 6) !=
                (StarsU16)trace->planet_id ||
                trace->fleet_raw[index][2] != (StarsU8)trace->planet_player ||
                (get_word(trace->fleet_raw[index], 4) & 0x0400U) != 0U ||
                get_word(trace->fleet_raw[index], 98) >= 2U ||
                (get_word(trace->order_raw[index], 2) & 0x000fU) != 3U)
            *mines = 0L;
    }
    return 1;
}

static void fleet_consumed(void *context, StarsI16 index)
{
    Trace *trace = (Trace *)context;
    ++trace->consumed_calls;
    trace->last_consumed = index;
    if (trace->packed_fleets)
        put_word(trace->fleet_raw[index], 4,
            (StarsU16)(get_word(trace->fleet_raw[index], 4) & 0xdfffU));
}

static void install(StarsMineralMinedOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->fixed_multiply = multiply;
    ops->fixed_divide = divide_fixed;
    ops->fixed_remainder = remainder_fixed;
    ops->operating_mines = operating_mines;
    ops->random = random_value;
    ops->fleet_mines = fleet_mines;
    ops->fleet_consumed = fleet_consumed;
    ops->context = trace;
}

static void seed_planet(PackedPlanet *planet, StarsI16 player,
    StarsU8 concentration, StarsU8 percent)
{
    StarsI16 i;
    memset(planet, 0, sizeof *planet);
    put_word(planet->raw, 2, (StarsU16)player);
    for (i = 0; i < 3; ++i) {
        planet->raw[6U + (StarsU16)i] = percent;
        planet->raw[9U + (StarsU16)i] = concentration;
        put_long(planet->raw, (StarsU16)(28U + (StarsU16)i * 4U), 0L);
    }
}

static void load_input(PackedPlanet *planet, StarsMineralMinedInput *input,
    StarsMineralMinedOps *ops)
{
    StarsI16 i;
    memset(input, 0, sizeof *input);
    input->player = (StarsI16)get_word(planet->raw, 2);
    input->c_mines = 10;
    input->apply = 1;
    input->mineral_weight = planet->weight;
    input->force_concentration_floor =
        (get_word(planet->raw, 4) & 0x0400U) != 0U;
    input->ops = ops;
    for (i = 0; i < 3; ++i) {
        input->concentration[i] = planet->raw[9U + (StarsU16)i];
        input->percent_level[i] = planet->raw[6U + (StarsU16)i];
        planet->weight[i] = get_long(planet->raw,
            (StarsU16)(28U + (StarsU16)i * 4U));
    }
}

static void store_input(PackedPlanet *planet,
    const StarsMineralMinedInput *input)
{
    StarsI16 i;
    for (i = 0; i < 3; ++i) {
        planet->raw[9U + (StarsU16)i] = input->concentration[i];
        planet->raw[6U + (StarsU16)i] = input->percent_level[i];
        put_long(planet->raw, (StarsU16)(28U + (StarsU16)i * 4U),
            planet->weight[i]);
    }
}

int main(void)
{
    PackedPlanet planet;
    StarsMineralMinedInput input;
    StarsMineralMinedOps ops;
    Trace trace;
    int failures = 0;

#define CHECK(condition) do { \
    if (!(condition)) \
        ++failures; \
} while (0)

    seed_planet(&planet, 0, 50, 0);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    stars_est_minerals_mined(&input);
    store_input(&planet, &input);
    CHECK(input.output[0] == 5L && input.output[1] == 5L &&
        input.output[2] == 5L && get_long(planet.raw, 28) == 5L &&
        planet.raw[9] == 50U && planet.raw[6] == 250U);

    seed_planet(&planet, 0, 50, 100);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    stars_est_minerals_mined(&input);
    store_input(&planet, &input);
    CHECK(input.output[0] == 5L && get_long(planet.raw, 28) == 5L &&
        planet.raw[9] == 50U && planet.raw[6] == 94U &&
        get_word(planet.raw, 2) == 0U);

    seed_planet(&planet, -1, 50, 0);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.c_mines = -1;
    input.operating_mines = 0;
    stars_est_minerals_mined(&input);
    CHECK(input.output[0] == -1L && input.output[1] == -1L &&
        input.output[2] == -1L && trace.operating_calls == 0);

    seed_planet(&planet, 2, 50, 0);
    memset(&trace, 0, sizeof trace);
    trace.operating_mines = 20;
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.c_mines = -1;
    input.operating_mines = 1;
    input.efficiency_tenths = 5;
    stars_est_minerals_mined(&input);
    store_input(&planet, &input);
    CHECK(input.output[0] == 5L && trace.operating_calls == 1 &&
        get_long(planet.raw, 28) == 5L);

    seed_planet(&planet, 0, 55, 0);
    memset(&trace, 0, sizeof trace);
    trace.random_value = 0;
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.random_rounding = 1;
    input.apply = 0;
    stars_est_minerals_mined(&input);
    CHECK(input.output[0] == 6L && trace.random_calls == 3 &&
        get_long(planet.raw, 28) == 0L && planet.raw[9] == 55U);

    seed_planet(&planet, 0, 55, 0);
    memset(&trace, 0, sizeof trace);
    trace.random_value = 99;
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.random_rounding = 1;
    input.apply = 0;
    stars_est_minerals_mined(&input);
    CHECK(input.output[0] == 5L && trace.random_calls == 3);

    seed_planet(&planet, 2, 20, 0);
    memset(&trace, 0, sizeof trace);
    trace.operating_mines = 10;
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.c_mines = -1;
    input.special_race = 1;
    input.operating_mines = 1;
    input.force_concentration_floor = 1;
    input.apply = 0;
    stars_est_minerals_mined(&input);
    CHECK(input.output[0] == 3L && input.concentration[0] == 20U &&
        trace.operating_calls == 1);

    seed_planet(&planet, 2, 50, 0);
    memset(&trace, 0, sizeof trace);
    trace.operating_mines = 10;
    trace.fleet_values[0] = 4;
    trace.fleet_values[1] = 0;
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.c_mines = -1;
    input.special_race = 1;
    input.operating_mines = 1;
    input.fleet_count = 2;
    stars_est_minerals_mined(&input);
    CHECK(input.output[0] == 7L && trace.operating_calls == 1 &&
        trace.fleet_calls == 2 && trace.consumed_calls == 1 &&
        trace.last_consumed == 0);

    seed_planet(&planet, 2, 20, 0);
    put_word(planet.raw, 0, 77U);
    put_word(planet.raw, 4, 0x0400U);
    memset(&trace, 0, sizeof trace);
    trace.operating_mines = 10;
    trace.planet_id = 77;
    trace.planet_player = 2;
    trace.fleet_values[0] = 4;
    trace.fleet_values[1] = 6;
    trace.fleet_values[2] = 8;
    trace.packed_fleets = 1;
    put_word(trace.fleet_raw[0], 2, 2U);
    put_word(trace.fleet_raw[0], 6, 77U);
    put_word(trace.fleet_raw[0], 4, 0x2000U);
    put_word(trace.fleet_raw[0], 98, 1U);
    put_word(trace.order_raw[0], 2, 3U);
    put_word(trace.fleet_raw[1], 2, 2U);
    put_word(trace.fleet_raw[1], 6, 77U);
    put_word(trace.fleet_raw[1], 4, 0x0400U);
    put_word(trace.fleet_raw[1], 98, 1U);
    put_word(trace.order_raw[1], 2, 3U);
    put_word(trace.fleet_raw[2], 2, 2U);
    put_word(trace.fleet_raw[2], 6, 88U);
    put_word(trace.fleet_raw[2], 98, 1U);
    put_word(trace.order_raw[2], 2, 3U);
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.c_mines = -1;
    input.special_race = 1;
    input.operating_mines = 1;
    input.fleet_count = 3;
    stars_est_minerals_mined(&input);
    store_input(&planet, &input);
    CHECK(input.output[0] == 4L && get_long(planet.raw, 28) == 4L &&
        get_word(trace.fleet_raw[0], 4) == 0U &&
        get_word(trace.fleet_raw[1], 4) == 0x0400U &&
        trace.fleet_calls == 3 && trace.consumed_calls == 1 &&
        trace.last_consumed == 0 && input.force_concentration_floor);

    /* Populated ordinary-race turn: the original uses the pre-efficiency
     * product for concentration depletion, while the reported output uses
     * the race efficiency.  This is the differential that the prior edge
     * cases did not exercise. */
    seed_planet(&planet, 0, 80, 100);
    planet.raw[7] = 128U;
    planet.raw[8] = 0U;
    planet.raw[10] = 25U;
    planet.raw[11] = 4U;
    put_long(planet.raw, 28, 1000L);
    put_long(planet.raw, 32, 2000L);
    put_long(planet.raw, 36, 3000L);
    memset(&trace, 0, sizeof trace);
    trace.operating_mines = 17;
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.c_mines = -1;
    input.operating_mines = 1;
    input.efficiency_tenths = 5;
    input.apply = 1;
    stars_est_minerals_mined(&input);
    store_input(&planet, &input);
    CHECK(input.output[0] == 6L && input.output[1] == 2L &&
        input.output[2] == 0L && planet.raw[9] == 80U &&
        planet.raw[10] == 25U && planet.raw[11] == 4U &&
        planet.raw[6] == 78U && planet.raw[7] == 125U &&
        planet.raw[8] == 0U && get_long(planet.raw, 28) == 1006L &&
        get_long(planet.raw, 32) == 2002L &&
        get_long(planet.raw, 36) == 3000L && trace.operating_calls == 1);

    seed_planet(&planet, 0, 99, 200);
    planet.raw[7] = 1U;
    planet.raw[8] = 0U;
    planet.raw[10] = 24U;
    planet.raw[11] = 4U;
    put_long(planet.raw, 28, 10L);
    put_long(planet.raw, 32, 20L);
    put_long(planet.raw, 36, 30L);
    memset(&trace, 0, sizeof trace);
    trace.operating_mines = 33;
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.c_mines = -1;
    input.operating_mines = 1;
    input.efficiency_tenths = 7;
    input.apply = 1;
    stars_est_minerals_mined(&input);
    store_input(&planet, &input);
    CHECK(input.output[0] == 22L && input.output[1] == 5L &&
        input.output[2] == 0L && planet.raw[9] == 99U &&
        planet.raw[10] == 23U && planet.raw[11] == 4U &&
        planet.raw[6] == 134U && planet.raw[7] == 252U &&
        planet.raw[8] == 255U && get_long(planet.raw, 28) == 32L &&
        get_long(planet.raw, 32) == 25L &&
        get_long(planet.raw, 36) == 30L);

    seed_planet(&planet, 2, 60, 64);
    planet.raw[7] = 100U;
    planet.raw[8] = 0U;
    planet.raw[10] = 30U;
    planet.raw[11] = 10U;
    memset(&trace, 0, sizeof trace);
    trace.operating_mines = 12;
    trace.fleet_values[0] = 3L;
    trace.fleet_values[1] = 5L;
    install(&ops, &trace);
    load_input(&planet, &input, &ops);
    input.c_mines = -1;
    input.special_race = 1;
    input.operating_mines = 1;
    input.force_concentration_floor = 1;
    input.fleet_count = 2;
    input.apply = 1;
    stars_est_minerals_mined(&input);
    store_input(&planet, &input);
    CHECK(input.output[0] == 11L && input.output[1] == 4L &&
        input.output[2] == 4L && planet.raw[6] == 48U &&
        planet.raw[7] == 96U && planet.raw[8] == 253U &&
        planet.raw[9] == 60U && planet.raw[10] == 30U &&
        planet.raw[11] == 10U && trace.fleet_calls == 2 &&
        trace.consumed_calls == 2);

    input.ops = (const StarsMineralMinedOps *)0;
    stars_est_minerals_mined(&input);
    CHECK(input.output[0] == 11L);

    if (failures == 0)
        puts("EST_MINERALS_MINED_INTEGRATION_CHECKS=13 FAILURES=0 RESULT=PASS");
    else
        puts("EST_MINERALS_MINED_INTEGRATION_CHECKS=13 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}
