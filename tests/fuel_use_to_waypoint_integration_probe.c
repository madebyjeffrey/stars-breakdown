#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedWaypointState {
    StarsU8 fleet[124];
    StarsU8 orders[54];
    StarsU8 planet[56];
    StarsU8 hulls[16][143];
    StarsU8 designs[16][147];
} PackedWaypointState;

typedef struct Trace {
    StarsI16 distances;
    StarsI16 estimates;
    StarsI16 planets;
    StarsI16 distance_index;
    StarsI32 distance_values[4];
    StarsI32 scoop_value;
    StarsI16 planet_hull_cargo;
} Trace;

static PackedWaypointState *g_state;
static Trace *g_trace;

static StarsU16 get_word(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU16)(p[offset] | ((StarsU16)p[offset + 1U] << 8));
}

static void put_word(StarsU8 *p, StarsU16 offset, StarsU16 value)
{
    p[offset] = (StarsU8)value;
    p[offset + 1U] = (StarsU8)(value >> 8);
}

static void reset_trace(Trace *trace)
{
    memset(trace, 0, sizeof *trace);
    trace->distance_values[0] = 6L;
    trace->distance_values[1] = 6L;
    trace->distance_values[2] = 6L;
    trace->distance_values[3] = 6L;
    trace->planet_hull_cargo = 0;
}

static StarsI32 distance(void *context, StarsI16 x0, StarsI16 y0,
    StarsI16 x1, StarsI16 y1)
{
    Trace *trace = g_trace;
    (void)context;
    (void)x0; (void)y0; (void)x1; (void)y1;
    ++trace->distances;
    if (trace->distance_index >= 4)
        return trace->distance_values[3];
    return trace->distance_values[trace->distance_index++];
}

static StarsI32 estimate(void *context, const StarsFuelWaypointFleet *fleet,
    StarsI16 segment, StarsI16 warp, StarsI32 parameter, StarsI16 range_only)
{
    (void)context;
    (void)fleet; (void)segment; (void)warp; (void)range_only;
    ++g_trace->estimates;
    return parameter < 0L ? 30L : parameter;
}

static StarsI32 ram_scoop(void *context, const StarsFuelWaypointFleet *fleet,
    StarsI16 warp, StarsI32 distance_value)
{
    (void)context;
    (void)fleet; (void)warp; (void)distance_value;
    return g_trace->scoop_value;
}

static StarsI16 cargo_hull(void *context, const StarsFuelWaypointFleet *fleet,
    StarsI16 slot)
{
    StarsU16 hull_id;
    (void)context;
    (void)fleet;
    if (slot < 0 || slot >= 16)
        return 0;
    hull_id = get_word(g_state->designs[slot], 0);
    return hull_id == 0x19U || hull_id == 0x1aU ?
        (StarsI16)hull_id : 0;
}

static StarsI16 planet_info(void *context, StarsI16 planet_id,
    StarsI16 *owner, StarsU16 *flags, StarsI16 *hull_cargo)
{
    (void)context;
    (void)planet_id;
    ++g_trace->planets;
    *owner = (StarsI16)get_word(g_state->planet, 2);
    *flags = get_word(g_state->planet, 4);
    *hull_cargo = (StarsI16)get_word(g_state->hulls[
        get_word(g_state->planet, 44) & 0x000fU], 52);
    g_trace->planet_hull_cargo = *hull_cargo;
    return 1;
}

static void seed_state(PackedWaypointState *state)
{
    StarsI16 i;
    memset(state, 0, sizeof *state);
    put_word(state->fleet, 2, 3);
    put_word(state->fleet, 12, 1);
    put_word(state->orders, 0, 10);
    put_word(state->orders, 2, 20);
    put_word(state->orders, 18, 30);
    put_word(state->orders, 20, 40);
    put_word(state->orders, 22, 7);
    put_word(state->orders, 24, 0x0120U);
    put_word(state->orders, 36, 50);
    put_word(state->orders, 38, 60);
    put_word(state->orders, 42, 0x0020U);
    put_word(state->planet, 2, 3);
    put_word(state->planet, 4, 0x0200U);
    put_word(state->planet, 44, 0);
    for (i = 0; i < 16; ++i)
        put_word(state->hulls[i], 52, 50);
    put_word(state->designs[0], 0, 0x19U);
}

static void load_orders(const PackedWaypointState *state,
    StarsFuelWaypointOrder *orders)
{
    StarsI16 i;
    for (i = 0; i < 3; ++i) {
        orders[i].x = (StarsI16)get_word(state->orders,
            (StarsU16)(i * 18));
        orders[i].y = (StarsI16)get_word(state->orders,
            (StarsU16)(i * 18 + 2));
        orders[i].target_id = (StarsI16)get_word(state->orders,
            (StarsU16)(i * 18 + 4));
        orders[i].flags = get_word(state->orders,
            (StarsU16)(i * 18 + 6));
    }
}

static void load_fleet(const PackedWaypointState *state,
    StarsFuelWaypointOrder *orders, StarsFuelWaypointFleet *fleet)
{
    StarsI16 slot;
    memset(fleet, 0, sizeof *fleet);
    fleet->owner = (StarsI16)get_word(state->fleet, 2);
    for (slot = 0; slot < 16; ++slot)
        fleet->cargo[slot] = (StarsI32)get_word(state->fleet,
            (StarsU16)(12U + (StarsU16)slot * 2U));
    fleet->orders = orders;
}

/* Independent transcription of the original 000B:A9F4 loop.  This is kept
 * separate from the reconstructed implementation so the route matrix below
 * compares the source against the raw control/data flow rather than merely
 * exercising the same helper sequence twice. */
static StarsI32 reference_waypoint_fuel(
    const StarsFuelUseToWaypointInput *input)
{
    const StarsFuelWaypointFleet *fleet = input->fleet;
    const StarsFuelWaypointOps *ops = input->ops;
    StarsI32 total = 0;
    StarsI32 greatest = 0;
    StarsI16 segment;

    for (segment = 0; segment < input->waypoint_count; ++segment) {
        const StarsFuelWaypointOrder *current = &fleet->orders[segment];
        const StarsFuelWaypointOrder *next = &fleet->orders[segment + 1];
        StarsI16 warp = (StarsI16)((next->flags >> 4) & 0x000fU);
        StarsI32 distance_value = 0;
        StarsI16 turns;
        StarsI32 base;
        StarsI32 segment_total;

        if (warp == 0 || warp > 10) {
            turns = 1;
            base = 0;
        } else {
            distance_value = ops->distance(ops->context, current->x,
                current->y, next->x, next->y);
            turns = (StarsI16)(distance_value / warp);
            base = ops->estimate_fuel(ops->context, fleet, segment, warp,
                -1L, 0);
        }
        segment_total = base;
        if (turns > 1) {
            StarsI32 per_warp = ops->estimate_fuel(ops->context, fleet,
                segment, warp, (StarsI32)warp * warp, 0);
            StarsI32 remainder = ops->estimate_fuel(ops->context, fleet,
                segment, warp, distance_value -
                (StarsI32)warp * warp * (turns - 1), 0);
            StarsI32 ram = ops->ram_scoop_gain(ops->context, fleet, warp,
                (StarsI32)warp * warp);
            StarsI16 slot;

            segment_total = per_warp * (turns - 1) + remainder;
            if (segment_total < base)
                segment_total = base;
            for (slot = 0; slot < 16; ++slot) {
                if (fleet->cargo[slot] != 0) {
                    StarsI16 hull = ops->cargo_hull_type(ops->context,
                        fleet, slot);
                    if (hull == 0x19 || hull == 0x1a)
                        ram += fleet->cargo[slot] * 200L;
                }
            }
            if (ram > 0 && ram < per_warp) {
                StarsI32 adjusted = per_warp +
                    (per_warp - ram) * (turns - 1);
                if (adjusted < segment_total)
                    segment_total = adjusted;
            }
        }
        total += segment_total;
        if (greatest < total)
            greatest = total;
        if (((next->flags >> 8) & 0x000fU) == 1U && ops->planet_info) {
            StarsI16 owner = 0;
            StarsU16 flags = 0;
            StarsI16 hull_cargo = 0;
            if (ops->planet_info(ops->context, next->target_id, &owner,
                    &flags, &hull_cargo) && owner == fleet->owner &&
                    (flags & 0x0200U) != 0U && hull_cargo != 0)
                total = 0;
        }
    }
    return greatest;
}

int main(void)
{
    PackedWaypointState state;
    StarsFuelWaypointOrder orders[3];
    StarsFuelWaypointFleet fleet;
    StarsFuelWaypointOps ops;
    StarsFuelUseToWaypointInput input;
    Trace trace;
    StarsI32 result;
    int checks = 0;
    int failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)

    g_state = &state;
    g_trace = &trace;
    memset(&ops, 0, sizeof ops);
    ops.distance = distance;
    ops.estimate_fuel = estimate;
    ops.ram_scoop_gain = ram_scoop;
    ops.cargo_hull_type = cargo_hull;
    ops.planet_info = planet_info;
    ops.context = 0;

    seed_state(&state);
    load_orders(&state, orders);
    load_fleet(&state, orders, &fleet);
    reset_trace(&trace);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.waypoint_count = 1;
    input.max_cargo = 1;
    input.ops = &ops;
    result = stars_fuel_use_to_waypoint(&input);
    CHECK(result == 38L && fleet.owner == 3 && fleet.cargo[0] == 1L &&
        orders[1].target_id == 7 && (orders[1].flags & 0x0120U) == 0x0120U &&
        trace.distances == 1 && trace.estimates == 3 && trace.planets == 1);
    CHECK(get_word(state.fleet, 12) == 1U &&
        get_word(state.orders, 24) == 0x0120U &&
        get_word(state.planet, 2) == 3U &&
        get_word(state.planet, 4) == 0x0200U &&
        get_word(state.hulls[0], 52) == 50U);

    seed_state(&state);
    put_word(state.orders, 24, 0x0020U);
    load_orders(&state, orders);
    load_fleet(&state, orders, &fleet);
    reset_trace(&trace);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.waypoint_count = 2;
    input.ops = &ops;
    result = stars_fuel_use_to_waypoint(&input);
    CHECK(result == 76L && trace.distances == 2 && trace.estimates == 6 &&
        trace.planets == 0);

    seed_state(&state);
    put_word(state.orders, 24, 0);
    load_orders(&state, orders);
    load_fleet(&state, orders, &fleet);
    reset_trace(&trace);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.waypoint_count = 1;
    input.ops = &ops;
    result = stars_fuel_use_to_waypoint(&input);
    CHECK(result == 0L && trace.distances == 0 && trace.estimates == 0 &&
        trace.planets == 0);

    seed_state(&state);
    put_word(state.planet, 2, 4);
    load_orders(&state, orders);
    load_fleet(&state, orders, &fleet);
    reset_trace(&trace);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.waypoint_count = 1;
    input.ops = &ops;
    result = stars_fuel_use_to_waypoint(&input);
    CHECK(result == 38L && trace.planets == 1);

    seed_state(&state);
    load_orders(&state, orders);
    load_fleet(&state, orders, &fleet);
    reset_trace(&trace);
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.waypoint_count = 1;
    input.ops = &ops;
    result = stars_fuel_use_to_waypoint(&input);
    CHECK(result == 38L && trace.planets == 1 &&
        get_word(state.planet, 2) == 3U);

    seed_state(&state);
    put_word(state.designs[0], 0, 0);
    put_word(state.planet, 4, 0);
    load_orders(&state, orders);
    load_fleet(&state, orders, &fleet);
    fleet.cargo[0] = 0;
    reset_trace(&trace);
    trace.scoop_value = 2L;
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.waypoint_count = 1;
    input.ops = &ops;
    result = stars_fuel_use_to_waypoint(&input);
    CHECK(result == 8L && trace.estimates == 3 && trace.planets == 1);

    seed_state(&state);
    put_word(state.planet, 4, 0);
    load_orders(&state, orders);
    load_fleet(&state, orders, &fleet);
    reset_trace(&trace);
    trace.distance_values[0] = 6L;
    trace.distance_values[1] = 12L;
    memset(&input, 0, sizeof input);
    input.fleet = &fleet;
    input.waypoint_count = 2;
    input.ops = &ops;
    result = stars_fuel_use_to_waypoint(&input);
    CHECK(result == 88L && trace.distances == 2 && trace.estimates == 6);

    CHECK(sizeof state.fleet == 124 && sizeof state.orders == 54 &&
        sizeof state.planet == 56 && sizeof state.hulls == 16 * 143 &&
        sizeof state.designs[0] == 147);
    CHECK(get_word(state.orders, 36) == 50U &&
        get_word(state.orders, 38) == 60U &&
        get_word(state.orders, 42) == 0x0020U &&
        orders[2].flags == 0x0020U);
    CHECK(fleet.owner == 3 && fleet.cargo[0] == 1L &&
        get_word(state.planet, 2) == 3U &&
        get_word(state.hulls[0], 52) == 50U);

    /* Differential route matrix: the reference is an independent
     * transcription of the original loop.  Vary the route length, warp
     * sequence, distance sequence, cargo-hull slots, ram-scoop gain, and
     * planet-reset boundary so every segment feeds the next one. */
    {
        StarsI16 route_case;
        StarsI32 expected;
        for (route_case = 0; route_case < 12; ++route_case) {
            StarsI16 segment;
            seed_state(&state);
            put_word(state.designs[0], 0,
                (StarsU16)((route_case & 1) ? 0x1aU : 0x19U));
            put_word(state.planet, 2,
                (StarsU16)((route_case & 2) ? 4U : 3U));
            put_word(state.planet, 4,
                (StarsU16)((route_case & 4) ? 0U : 0x0200U));
            put_word(state.hulls[0], 52,
                (StarsU16)((route_case & 8) ? 1U : 50U));
            trace.scoop_value = (StarsI32)(route_case % 5);
            load_orders(&state, orders);
            load_fleet(&state, orders, &fleet);
            for (segment = 1; segment < 3; ++segment) {
                put_word(state.orders, (StarsU16)(segment * 18),
                    (StarsU16)(20 + segment * 17 + route_case));
                put_word(state.orders, (StarsU16)(segment * 18 + 2),
                    (StarsU16)(40 + segment * 11));
                put_word(state.orders, (StarsU16)(segment * 18 + 6),
                    (StarsU16)(((route_case + segment) % 12) << 4));
                if (segment == 2)
                    put_word(state.orders, 42,
                        (StarsU16)(0x0100U | (((route_case + 1) & 1) << 4)));
            }
            load_orders(&state, orders);
            load_fleet(&state, orders, &fleet);
            reset_trace(&trace);
            trace.scoop_value = (StarsI32)(route_case % 5);
            memset(&input, 0, sizeof input);
            input.fleet = &fleet;
            input.waypoint_count = (StarsI16)(1 + (route_case % 2));
            input.ops = &ops;
            expected = reference_waypoint_fuel(&input);
            reset_trace(&trace);
            CHECK(stars_fuel_use_to_waypoint(&input) == expected);
        }
    }

    /* Full packed ship/waypoint boundary: load every fleet count, every
     * design hull selector, and the planet-selected hull definition. */
    {
        StarsI16 slot;
        StarsI16 mapping_ok;
        StarsI32 expected;
        seed_state(&state);
        put_word(state.planet, 44, 7U);
        for (slot = 0; slot < 16; ++slot) {
            put_word(state.fleet, (StarsU16)(12U + (StarsU16)slot * 2U),
                (StarsU16)(slot + 1));
            put_word(state.designs[slot], 0,
                (StarsU16)((slot % 3 == 0) ? 0x19U :
                    ((slot % 3 == 1) ? 0x1aU : 0x08U)));
            put_word(state.hulls[slot], 52, (StarsU16)(20 + slot));
        }
        put_word(state.hulls[7], 52, 77U);
        load_orders(&state, orders);
        load_fleet(&state, orders, &fleet);
        reset_trace(&trace);
        memset(&input, 0, sizeof input);
        input.fleet = &fleet;
        input.waypoint_count = 2;
        input.ops = &ops;
        expected = reference_waypoint_fuel(&input);
        reset_trace(&trace);
        result = stars_fuel_use_to_waypoint(&input);
        mapping_ok = 1;
        for (slot = 0; slot < 16; ++slot)
            if (fleet.cargo[slot] != (StarsI32)(slot + 1))
                mapping_ok = 0;
        CHECK(result == expected && trace.distances == 2 &&
            trace.estimates == 6 && trace.planets == 1 &&
            trace.planet_hull_cargo == 77);
        CHECK(mapping_ok && get_word(state.fleet, 12) == 1U &&
            get_word(state.fleet, 42) == 16U);
        CHECK(get_word(state.planet, 44) == 7U &&
            get_word(state.hulls[7], 52) == 77U &&
            get_word(state.designs[1], 0) == 0x1aU);
        CHECK(orders[1].flags == 0x0120U && orders[2].flags == 0x0020U &&
            orders[2].target_id == 0);
    }

    /* Twelve differential and four full-boundary checks were added. */
    if (checks != 27)
        ++failures;

    if (failures == 0)
        puts("FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=27 FAILURES=0 RESULT=PASS");
    else
        puts("FUEL_USE_TO_WAYPOINT_INTEGRATION_CHECKS=27 FAILURES=1 RESULT=FAIL");
    return failures != 0;
#undef CHECK
}
