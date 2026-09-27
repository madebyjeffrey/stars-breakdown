#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedFuelState {
    StarsU8 fleet[124];
    StarsU8 designs[16][147];
    StarsU8 hulls[16][143];
    StarsU8 engines[16][78];
} PackedFuelState;

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

static void seed_component(StarsU8 *design, StarsU16 item)
{
    put_word(design, 58, 0x1000U);
    put_word(design, 60, item);
    design[122] = 1;
}

static void seed_state(PackedFuelState *state)
{
    StarsI16 i;
    memset(state, 0, sizeof *state);
    put_word(state->fleet, 2, 3);
    put_word(state->fleet, 6, 9);
    put_long(state->fleet, 44, 1500L);
    put_long(state->fleet, 76, 1000L);
    put_long(state->fleet, 80, 500L);
    put_word(state->fleet, 12, 2);
    put_word(state->fleet, 14, 1);

    for (i = 0; i < 16; ++i)
        put_word(state->hulls[i], 52, 50);
    put_word(state->engines[10], 54U + 2U * 2U, 20);
    put_word(state->engines[3], 54U + 2U * 2U, 40);
    for (i = 0; i < 16; ++i) {
        put_word(state->designs[i], 0, 1);
        put_long(state->designs[i], 40, 0L);
    }
    put_long(state->designs[0], 40, 4L);
    seed_component(state->designs[0], 10);
    put_long(state->designs[1], 40, 2L);
    seed_component(state->designs[1], 3);
}

static void load_typed(const PackedFuelState *state, StarsEstFuelFleet *fleet)
{
    StarsI16 i;
    memset(fleet, 0, sizeof *fleet);
    fleet->player = (StarsI16)get_word(state->fleet, 2);
    fleet->destination = (StarsI16)get_word(state->fleet, 6);
    fleet->cargo_weight = get_long(state->fleet, 44);
    for (i = 0; i < 4; ++i)
        fleet->mineral_weight[i] = get_long(state->fleet,
            (StarsU16)(76U + (StarsU16)i * 4U));
    for (i = 0; i < STARS_EST_FUEL_SLOTS; ++i) {
        const StarsU8 *design = state->designs[i];
        StarsU16 item;
        StarsI16 warp;
        fleet->slots[i].cargo_count = get_word(state->fleet,
            (StarsU16)(12U + (StarsU16)i * 2U));
        item = get_word(design, 0);
        fleet->slots[i].ship_stat = (StarsI32)get_word(
            state->hulls[item & 15U], 52);
        fleet->slots[i].hull_fuel_weight = get_long(design, 40);
        item = get_word(design, 60);
        fleet->slots[i].engine_available = design[122] != 0 && item != 0;
        fleet->slots[i].engine_id = (StarsI16)(item & 0xffU);
        for (warp = 0; warp < STARS_EST_FUEL_WARPS; ++warp)
            fleet->slots[i].fuel_used[warp] = get_word(
                state->engines[fleet->slots[i].engine_id],
                (StarsU16)(54U + (StarsU16)warp * 2U));
    }
}

typedef struct FuelTrace {
    StarsI16 warp_calls;
    StarsI16 distance_calls;
    StarsI32 distance_value;
} FuelTrace;

static StarsI16 packed_warp(void *context, StarsI16 order,
    StarsI16 player)
{
    FuelTrace *trace = (FuelTrace *)context;
    (void)order;
    (void)player;
    ++trace->warp_calls;
    return 2;
}

static StarsI32 packed_distance(void *context, StarsI16 order,
    StarsI16 warp, StarsI16 player, StarsI16 destination)
{
    FuelTrace *trace = (FuelTrace *)context;
    (void)order;
    (void)warp;
    (void)player;
    (void)destination;
    ++trace->distance_calls;
    return trace->distance_value;
}

static StarsI32 reference_fuel_term(StarsI32 hull_fuel_weight,
    StarsU32 cargo_count, StarsI32 used, StarsI32 fuel, StarsI32 travel)
{
    double weight;
    double fuel_distance;
    weight = (double)hull_fuel_weight * (double)cargo_count +
        (double)used;
    fuel_distance = (double)fuel * (double)travel;
    return (StarsI32)(weight * fuel_distance / 2000.0);
}

static StarsI32 reference_fuel(const StarsEstFuelFleet *fleet,
    StarsI16 warp, StarsI32 travel, StarsI16 race_grbit,
    StarsI16 range_only)
{
    StarsI32 values[STARS_EST_FUEL_SLOTS];
    StarsI32 remaining;
    StarsI32 total;
    StarsI32 current;
    StarsI32 next;
    StarsI32 capacity;
    StarsI32 used;
    StarsI16 slot;
    StarsI16 found;

    for (slot = 0; slot < STARS_EST_FUEL_SLOTS; ++slot) {
        values[slot] = fleet->slots[slot].engine_available ?
            fleet->slots[slot].fuel_used[warp] : 99999L;
        if (race_grbit)
            values[slot] -= (values[slot] * 15L) / 100L;
    }
    remaining = 0;
    for (slot = 0; slot < 4; ++slot)
        remaining += fleet->mineral_weight[slot];
    total = 0;
    current = 0;
    for (;;) {
        next = 100000L;
        found = 0;
        for (slot = 0; slot < STARS_EST_FUEL_SLOTS; ++slot) {
            if (fleet->slots[slot].cargo_count == 0U)
                continue;
            if (values[slot] == current) {
                capacity = (StarsI32)fleet->slots[slot].cargo_count *
                    fleet->slots[slot].ship_stat;
                used = capacity < remaining ? capacity : remaining;
                if (used < 0)
                    used = 0;
                remaining -= used;
                total += reference_fuel_term(
                    fleet->slots[slot].hull_fuel_weight,
                    fleet->slots[slot].cargo_count, used, current,
                    travel);
            } else if (values[slot] > current && values[slot] < next) {
                next = values[slot];
                found = 1;
            }
        }
        if (!found)
            break;
        current = next;
    }
    if (!range_only)
        total += 9L;
    total /= 10L;
    if (!range_only)
        return total;
    if (total == 0)
        return 1000000000L;
    if (total < 100001L)
        return (StarsI32)((double)fleet->cargo_weight * 1000.0 /
            (double)total);
    return fleet->cargo_weight / (total / 1000L);
}

int main(void)
{
    PackedFuelState state;
    StarsEstFuelFleet fleet;
    StarsEstFuelInput input;
    StarsU16 flags;
    StarsI32 result;
    FuelTrace trace;
    StarsI16 warp;
    StarsI16 slot;
    StarsI16 distance_index;
    StarsI32 route_distances[4];
    int checks = 0;
    int failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    seed_state(&state);
    load_typed(&state, &fleet);
    memset(&input, 0, sizeof input);
    flags = 0;
    input.fleet = &fleet;
    input.warp = 2;
    input.travel = 100;
    input.race_grbit = 1;
    input.game_flags = &flags;
    result = stars_est_fuel_use(&input);
    CHECK(fleet.player == 3 && fleet.destination == 9 &&
        fleet.slots[0].cargo_count == 2U &&
        fleet.slots[0].fuel_used[2] == 20L &&
        fleet.slots[1].fuel_used[2] == 40L && result == 18L &&
        (flags & 0x0040U) != 0U);

    CHECK(get_word(state.fleet, 12) == 2U &&
        get_word(state.fleet, 14) == 1U &&
        get_long(state.fleet, 76) == 1000L &&
        get_long(state.fleet, 80) == 500L &&
        get_word(state.designs[0], 60) == 10U &&
        get_word(state.engines[10], 58) == 20U);

    seed_state(&state);
    load_typed(&state, &fleet);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.warp = 2;
    input.travel = 100;
    flags = 0;
    input.game_flags = &flags;
    result = stars_est_fuel_use(&input);
    CHECK(result == 22L && (flags & 0x0040U) != 0U);

    memset(&state, 0, sizeof state);
    load_typed(&state, &fleet);
    memset(&input, 0, sizeof input);
    flags = 0;
    input.fleet = &fleet;
    input.warp = 2;
    input.travel = 100;
    input.range_only = 1;
    input.game_flags = &flags;
    result = stars_est_fuel_use(&input);
    CHECK(result == 1000000000L && (flags & 0x0040U) == 0U);

    seed_state(&state);
    put_word(state.fleet, 14, 0);
    put_word(state.engines[10], 54U + 2U * 10U, 70);
    load_typed(&state, &fleet);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.warp = 10;
    input.travel = 100;
    result = stars_est_fuel_use(&input);
    CHECK(result == 38L && fleet.slots[0].engine_id == 10 &&
        fleet.slots[0].fuel_used[10] == 70L);

    flags = 0x0040U;
    input.game_flags = &flags;
    input.warp = 12;
    result = stars_est_fuel_use(&input);
    CHECK(result == 0L && (flags & 0x0040U) == 0U);

    seed_state(&state);
    load_typed(&state, &fleet);
    memset(&trace, 0, sizeof trace);
    trace.distance_value = 100;
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.order = 3;
    input.warp = -1;
    input.travel = -1L;
    input.race_grbit = 1;
    input.warp_from_order = packed_warp;
    input.distance = packed_distance;
    input.context = &trace;
    result = stars_est_fuel_use(&input);
    CHECK(result == 18L && trace.warp_calls == 1 &&
        trace.distance_calls == 1);

    memset(&state, 0, sizeof state);
    put_word(state.fleet, 2, 3);
    put_long(state.fleet, 44, 1000000L);
    put_long(state.fleet, 76, 1000000L);
    put_word(state.fleet, 12, 1000);
    put_word(state.hulls[1], 52, 1000);
    put_word(state.designs[0], 0, 1);
    put_long(state.designs[0], 40, 0L);
    seed_component(state.designs[0], 10);
    put_word(state.engines[10], 54U + 2U * 2U, 1000);
    load_typed(&state, &fleet);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.warp = 2;
    input.travel = 1000;
    input.race_grbit = 1;
    result = stars_est_fuel_use(&input);
    CHECK(result == 42500000L);

    seed_state(&state);
    put_word(state.designs[0], 60, 0);
    state.designs[0][122] = 0;
    load_typed(&state, &fleet);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.warp = 2;
    input.travel = 100;
    result = stars_est_fuel_use(&input);
    CHECK(result == 54010L && fleet.slots[0].engine_available == 0);

    seed_state(&state);
    load_typed(&state, &fleet);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.warp = 2;
    input.travel = 100;
    input.range_only = 1;
    fleet.cargo_weight = 2500L;
    result = stars_est_fuel_use(&input);
    CHECK(result == 119047L);

    /* Differential matrix: every packed design slot, hull capacity, engine
     * row, warp row, race discount, range mode, and route distance is varied
     * against an independent implementation of the original loop. */
    seed_state(&state);
    put_long(state.fleet, 44, 5000L);
    put_long(state.fleet, 76, 100L);
    put_long(state.fleet, 80, 200L);
    put_long(state.fleet, 84, 300L);
    put_long(state.fleet, 88, 400L);
    for (slot = 0; slot < 16; ++slot) {
        StarsI16 engine_id;
        engine_id = (StarsI16)((slot % 15) + 1);
        put_word(state.designs[slot], 0, (StarsU16)slot);
        put_long(state.designs[slot], 40, (StarsI32)(slot + 1));
        seed_component(state.designs[slot], (StarsU16)engine_id);
        put_word(state.fleet, (StarsU16)(12U + (StarsU16)slot * 2U),
            (StarsU16)((slot % 4) + 1));
        put_word(state.hulls[slot], 52,
            (StarsU16)(20U + (StarsU16)slot * 5U));
        for (warp = 0; warp < STARS_EST_FUEL_WARPS; ++warp)
            put_word(state.engines[engine_id],
                (StarsU16)(54U + (StarsU16)warp * 2U),
                (StarsU16)(7U + (StarsU16)engine_id * 3U +
                    (StarsU16)warp * 2U));
    }
    load_typed(&state, &fleet);
    {
        StarsI16 mapping_ok;
        mapping_ok = 1;
        for (slot = 0; slot < 16; ++slot) {
            if (fleet.slots[slot].engine_id !=
                    (StarsI16)((slot % 15) + 1) ||
                    fleet.slots[slot].ship_stat != 20L + slot * 5L ||
                    fleet.slots[slot].fuel_used[11] !=
                        7L + ((slot % 15) + 1) * 3L + 22L)
                mapping_ok = 0;
        }
        CHECK(mapping_ok);
    }
    for (warp = 0; warp < STARS_EST_FUEL_WARPS; ++warp) {
        StarsI16 flags_value;
        memset(&input, 0, sizeof input);
        input.fleet = &fleet;
        input.warp = warp;
        input.travel = 1L + (StarsI32)warp * 17L;
        input.race_grbit = (StarsI16)(warp & 1);
        input.range_only = (StarsI16)((warp % 3) == 0);
        flags_value = 0;
        input.game_flags = (StarsU16 *)&flags_value;
        result = stars_est_fuel_use(&input);
        CHECK(result == reference_fuel(&fleet, warp, input.travel,
            input.race_grbit, input.range_only));
    }
    route_distances[0] = 1L;
    route_distances[1] = 37L;
    route_distances[2] = 1000L;
    route_distances[3] = 50000L;
    for (distance_index = 0; distance_index < 4; ++distance_index) {
        memset(&trace, 0, sizeof trace);
        trace.distance_value = route_distances[distance_index];
        memset(&input, 0, sizeof input);
        input.fleet = &fleet;
        input.warp = (StarsI16)(distance_index + 2);
        input.travel = -1L;
        input.race_grbit = (StarsI16)(distance_index & 1);
        input.distance = packed_distance;
        input.context = &trace;
        result = stars_est_fuel_use(&input);
        CHECK(result == reference_fuel(&fleet, input.warp,
            route_distances[distance_index], input.race_grbit, 0) &&
            trace.distance_calls == 1);
    }

    CHECK(sizeof state.fleet == 124 && sizeof state.designs[0] == 147 &&
        sizeof state.hulls[0] == 143 && sizeof state.engines[0] == 78);
    CHECK(get_word(state.engines[10], 58) == 41U);
    if (checks != 29)
        ++failures;

    if (failures == 0)
        puts("EST_FUEL_USE_INTEGRATION_CHECKS=29 FAILURES=0 RESULT=PASS");
    else
        puts("EST_FUEL_USE_INTEGRATION_CHECKS=29 FAILURES=1 RESULT=FAIL");
    return failures != 0;
#undef CHECK
}
