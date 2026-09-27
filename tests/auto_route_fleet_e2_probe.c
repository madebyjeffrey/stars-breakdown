#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct AutoRouteE2State {
    StarsU8 fleet_raw[124];
    StarsU8 designs[16][147];
    StarsU8 hulls[16][143];
    StarsU8 engines[16][78];
    StarsAutoRouteOrder route_orders[2];
    StarsAutoRoutePlanet source;
    StarsAutoRoutePlanet target;
    StarsStargateArmor gates[16];
    StarsAutoRouteFleet route_fleet;
    StarsEstFuelFleet fuel_fleet;
    StarsAutoRouteOps route_ops;
    StarsStargateDamageInput gate_input;
    StarsMoveFleet move_fleet;
    StarsMoveFleetsOps move_ops;
    StarsI16 ideal_warp;
    StarsI16 move_order_reads;
    StarsI16 move_fuel_calls;
    StarsI16 route_range_calls;
    StarsI16 route_cost_calls;
    StarsI32 move_last_fuel;
    StarsI32 gate_calls;
} AutoRouteE2State;

static StarsU16 get_word(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU16)(p[offset] | ((StarsU16)p[offset + 1U] << 8));
}

static StarsI32 get_long(const StarsU8 *p, StarsU16 offset)
{
    return (StarsI32)((StarsU32)get_word(p, offset) |
        ((StarsU32)get_word(p, (StarsU16)(offset + 2U)) << 16));
}

static void put_word(StarsU8 *p, StarsU16 offset, StarsU16 value)
{
    p[offset] = (StarsU8)value;
    p[offset + 1U] = (StarsU8)(value >> 8);
}

static void put_long(StarsU8 *p, StarsU16 offset, StarsI32 value)
{
    StarsU32 u;

    u = (StarsU32)value;
    put_word(p, offset, (StarsU16)u);
    put_word(p, (StarsU16)(offset + 2U), (StarsU16)(u >> 16));
}

static StarsI32 absolute_i32(StarsI32 value)
{
    return value < 0 ? -value : value;
}

static StarsI32 route_distance(StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    return absolute_i32((StarsI32)x2 - x1) +
        absolute_i32((StarsI32)y2 - y1);
}

static void seed_state(AutoRouteE2State *state)
{
    StarsI16 slot;
    StarsI16 warp;

    memset(state, 0, sizeof *state);
    put_word(state->fleet_raw, 2U, 2U);
    put_word(state->fleet_raw, 8U, 0U);
    put_word(state->fleet_raw, 10U, 0U);
    put_word(state->fleet_raw, 12U, 2U);
    put_long(state->fleet_raw, 44U, 10000L);
    put_long(state->fleet_raw, 76U, 0L);
    put_long(state->fleet_raw, 80U, 0L);
    put_long(state->fleet_raw, 84U, 0L);
    put_long(state->fleet_raw, 88U, 0L);

    for (slot = 0; slot < 16; ++slot) {
        put_word(state->designs[slot], 0U, 1U);
        put_long(state->designs[slot], 40U, 1000L);
        put_word(state->designs[slot], 60U, 10U);
        state->designs[slot][122] = 1U;
        put_word(state->hulls[1], 52U, 50U);
        for (warp = 0; warp < 12; ++warp)
            put_word(state->engines[10], (StarsU16)(54U + warp * 2U),
                (StarsU16)(50U + warp * 10U));
    }

    state->source.id = 1;
    state->source.owner = 2;
    state->source.x = 0;
    state->source.y = 0;
    state->source.route_target_id = 3;
    state->source.status_flags = 0x0200U;
    state->source.stargate = 1;
    state->target.id = 3;
    state->target.owner = 2;
    state->target.x = 100;
    state->target.y = 20;
    state->target.status_flags = 0x0200U;
    state->target.stargate = 2;
    state->target.hull_cargo_max = 1;
    state->gates[1].fuel_max = 1000;
    state->gates[1].defense = 1000;
    state->gates[2].fuel_max = 1000;
    state->gates[2].defense = 1000;
    state->ideal_warp = 8;
}

static void load_typed(AutoRouteE2State *state)
{
    StarsI16 slot;
    StarsI16 warp;
    StarsU16 design_id;
    StarsU16 engine_id;

    memset(&state->route_fleet, 0, sizeof state->route_fleet);
    memset(&state->fuel_fleet, 0, sizeof state->fuel_fleet);
    state->route_fleet.owner = (StarsI16)get_word(state->fleet_raw, 2U);
    state->route_fleet.x = (StarsI16)get_word(state->fleet_raw, 8U);
    state->route_fleet.y = (StarsI16)get_word(state->fleet_raw, 10U);
    state->route_fleet.fuel_capacity = 5000L;
    state->route_fleet.orders = state->route_orders;
    state->fuel_fleet.player = state->route_fleet.owner;
    state->fuel_fleet.destination = state->target.id;
    state->fuel_fleet.cargo_weight = get_long(state->fleet_raw, 44U);
    for (slot = 0; slot < 4; ++slot) {
        state->fuel_fleet.mineral_weight[slot] = get_long(state->fleet_raw,
            (StarsU16)(76U + slot * 4U));
        state->route_fleet.mineral_weights[slot] =
            state->fuel_fleet.mineral_weight[slot];
    }
    for (slot = 0; slot < 16; ++slot) {
        design_id = get_word(state->designs[slot], 0U);
        engine_id = get_word(state->designs[slot], 60U);
        state->route_fleet.cargo[slot] = get_word(state->fleet_raw,
            (StarsU16)(12U + slot * 2U));
        state->route_fleet.cargo_hull_capacity[slot] =
            (StarsI32)get_word(state->hulls[design_id & 15U], 52U);
        state->fuel_fleet.slots[slot].cargo_count =
            (StarsU32)state->route_fleet.cargo[slot];
        state->fuel_fleet.slots[slot].ship_stat =
            state->route_fleet.cargo_hull_capacity[slot];
        state->fuel_fleet.slots[slot].hull_fuel_weight =
            get_long(state->designs[slot], 40U);
        state->fuel_fleet.slots[slot].engine_available =
            state->designs[slot][122] != 0U && engine_id != 0U;
        state->fuel_fleet.slots[slot].engine_id = (StarsI16)engine_id;
        for (warp = 0; warp < 12; ++warp)
            state->fuel_fleet.slots[slot].fuel_used[warp] =
                get_word(state->engines[engine_id & 15U],
                    (StarsU16)(54U + warp * 2U));
    }
}

static StarsI16 gate_lookup(void *context, StarsI16 id,
    StarsStargateArmor *armor)
{
    AutoRouteE2State *state = (AutoRouteE2State *)context;

    if (id < 0 || id >= 16)
        return 0;
    *armor = state->gates[id];
    return armor->fuel_max != 0 || armor->defense != 0;
}

static StarsI16 find_ideal_warp(void *context,
    const StarsAutoRouteFleet *fleet, StarsI16 order)
{
    AutoRouteE2State *state = (AutoRouteE2State *)context;
    (void)fleet;
    (void)order;
    return state->ideal_warp;
}

static StarsI32 find_distance(void *context, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    (void)context;
    return route_distance(x1, y1, x2, y2);
}

static StarsAutoRoutePlanet *find_planet(void *context, StarsI16 id)
{
    AutoRouteE2State *state = (AutoRouteE2State *)context;
    return id == state->target.id ? &state->target :
        (StarsAutoRoutePlanet *)0;
}

static StarsI16 real_gate_damage(void *context, StarsI16 source,
    StarsI16 destination, StarsI32 distance, StarsI32 weight,
    StarsI16 *damage)
{
    AutoRouteE2State *state = (AutoRouteE2State *)context;

    ++state->gate_calls;
    return stars_calc_stargate_damage(&state->gate_input, source,
        destination, (StarsI16)distance, (StarsI16)weight, damage);
}

static StarsI32 real_fuel(void *context, const StarsAutoRouteFleet *fleet,
    StarsI16 segment, StarsI16 warp, StarsI32 distance, StarsI16 range_only)
{
    AutoRouteE2State *state = (AutoRouteE2State *)context;
    StarsEstFuelInput input;
    (void)fleet;
    (void)segment;

    memset(&input, 0, sizeof input);
    input.fleet = &state->fuel_fleet;
    input.warp = warp;
    input.travel = distance;
    input.range_only = range_only;
    if (range_only)
        ++state->route_range_calls;
    else
        ++state->route_cost_calls;
    return stars_est_fuel_use(&input);
}

static StarsI32 oracle_fuel(const StarsEstFuelFleet *fleet, StarsI16 warp,
    StarsI32 distance, StarsI16 range_only)
{
    StarsI32 values[16];
    StarsI32 remaining;
    StarsI32 total;
    StarsI32 current;
    StarsI32 next;
    StarsI32 capacity;
    StarsI32 used;
    StarsI32 weight;
    StarsI16 slot;
    StarsI16 found;

    remaining = 0;
    for (slot = 0; slot < 4; ++slot)
        remaining += fleet->mineral_weight[slot];
    for (slot = 0; slot < 16; ++slot)
        values[slot] = fleet->slots[slot].engine_available ?
            fleet->slots[slot].fuel_used[warp] : 99999L;
    current = 0;
    total = 0;
    for (;;) {
        next = 100000L;
        found = 0;
        for (slot = 0; slot < 16; ++slot) {
            if (fleet->slots[slot].cargo_count == 0U)
                continue;
            if (values[slot] == current) {
                capacity = (StarsI32)fleet->slots[slot].cargo_count *
                    fleet->slots[slot].ship_stat;
                used = capacity < remaining ? capacity : remaining;
                if (used < 0)
                    used = 0;
                remaining -= used;
                weight = (StarsI32)fleet->slots[slot].cargo_count *
                    fleet->slots[slot].hull_fuel_weight + used;
                total += (StarsI32)(((double)weight * values[slot] *
                    (range_only ? 1000L : distance)) / 2000.0);
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
        return (StarsI32)(((double)fleet->cargo_weight * 1000.0) /
            (double)total);
    return fleet->cargo_weight / (total / 1000L);
}

static StarsI16 oracle_gate_damage(const AutoRouteE2State *state,
    StarsI16 source, StarsI16 destination, StarsI16 distance,
    StarsI16 weight, StarsI16 *damage)
{
    StarsI32 range;
    StarsI32 scale;
    StarsI32 factor;
    StarsStargateArmor a;
    StarsStargateArmor b;

    a = state->gates[source];
    b = state->gates[destination];
    range = a.fuel_max == -1 ? 8000L : a.fuel_max;
    if (range * 5L < distance)
        return -1;
    if ((a.defense > 0 && (StarsI32)a.defense * 5L < weight) ||
            (b.defense > 0 && (StarsI32)b.defense * 5L < weight))
        return -2;
    scale = 10000L;
    if (distance > range) {
        factor = (range * 5L - distance) * 2500L / range;
        if (factor < 1L) {
            *damage = 100;
            return 0;
        }
        scale = scale * factor / 10000L;
    }
    if (a.defense > 0 && a.defense < weight) {
        factor = ((StarsI32)a.defense * 5L - weight) * 2500L /
            a.defense;
        if (factor < 1L) {
            *damage = 100;
            return 0;
        }
        scale = scale * factor / 10000L;
    }
    if (b.defense > 0 && b.defense < weight) {
        factor = ((StarsI32)b.defense * 5L - weight) * 2500L /
            b.defense;
        if (factor < 1L) {
            *damage = 100;
            return 0;
        }
        scale = scale * factor / 10000L;
    }
    *damage = (StarsI16)((10000L - scale) / 100L);
    return 1;
}

static void oracle_route(AutoRouteE2State *state, StarsI16 *warp_out)
{
    StarsI16 warp;
    StarsI16 lower;
    StarsI16 slot;
    StarsI16 damage;
    StarsI32 distance;
    StarsI32 fuel;
    StarsI32 lower_fuel;
    StarsI32 max_cargo;
    StarsI16 has_minerals;

    warp = state->ideal_warp;
    distance = route_distance(state->route_fleet.x, state->route_fleet.y,
        state->target.x, state->target.y);
    has_minerals = 0;
    for (slot = 0; slot < 4; ++slot)
        if (state->fuel_fleet.mineral_weight[slot] != 0)
            has_minerals = 1;
    if (!has_minerals && state->source.owner == state->target.owner &&
            (state->source.status_flags & 0x0200U) != 0U &&
            (state->target.status_flags & 0x0200U) != 0U) {
        max_cargo = 0;
        for (slot = 0; slot < 16; ++slot)
            if (state->route_fleet.cargo[slot] != 0 &&
                    state->route_fleet.cargo_hull_capacity[slot] > max_cargo)
                max_cargo = state->route_fleet.cargo_hull_capacity[slot];
        damage = 0;
        if (oracle_gate_damage(state, state->source.stargate,
                state->target.stargate, (StarsI16)distance,
                (StarsI16)max_cargo, &damage) == 1 && damage == 0)
            warp = 11;
        if (warp < 9 && state->target.hull_cargo_max != 0) {
            for (slot = 9; slot > 0; --slot) {
                fuel = oracle_fuel(&state->fuel_fleet, slot, -1L, 1);
                if (distance <= fuel)
                    break;
            }
            warp = slot;
        }
    }
    if (warp > 0 && warp < 11) {
        fuel = distance / ((StarsI32)warp * warp);
        lower = warp;
        while (lower >= 3) {
            --lower;
            if (lower <= 0)
                break;
            lower_fuel = distance / ((StarsI32)lower * lower);
            if (lower_fuel > fuel) {
                ++lower;
                break;
            }
            fuel = lower_fuel;
        }
        warp = lower;
        while (warp > 0) {
            fuel = oracle_fuel(&state->fuel_fleet, warp, distance, 0);
            if (fuel <= state->route_fleet.fuel_capacity || warp == 1)
                break;
            --warp;
        }
    }
    *warp_out = warp;
}

static StarsI16 get_move_order(void *context, const StarsMoveFleet *fleet,
    StarsMoveFleetOrder *order)
{
    AutoRouteE2State *state = (AutoRouteE2State *)context;
    StarsU16 flags;
    (void)fleet;

    if (state->move_order_reads != 0)
        return 0;
    ++state->move_order_reads;
    flags = state->route_orders[1].flags;
    memset(order, 0, sizeof *order);
    order->kind = 2;
    order->target_kind = 1;
    order->target_id = state->route_orders[1].target_id;
    order->target_x = state->route_orders[1].x;
    order->target_y = state->route_orders[1].y;
    order->warp_level = (StarsI16)((flags >> 4) & 0x000fU);
    return 1;
}

static StarsI32 move_distance(void *context, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    (void)context;
    return route_distance(x1, y1, x2, y2);
}

static StarsI32 move_fuel(void *context, const StarsMoveFleet *fleet,
    const StarsMoveFleetOrder *order, StarsI32 distance)
{
    AutoRouteE2State *state = (AutoRouteE2State *)context;
    StarsEstFuelInput input;
    (void)fleet;

    memset(&input, 0, sizeof input);
    input.fleet = &state->fuel_fleet;
    input.warp = order->warp_level;
    input.travel = distance;
    ++state->move_fuel_calls;
    state->move_last_fuel = stars_est_fuel_use(&input);
    return state->move_last_fuel;
}

static void initialize_turn(AutoRouteE2State *state)
{
    memset(&state->route_orders, 0, sizeof state->route_orders);
    memset(&state->move_fleet, 0, sizeof state->move_fleet);
    state->move_order_reads = 0;
    state->move_fuel_calls = 0;
    state->route_range_calls = 0;
    state->route_cost_calls = 0;
    state->move_last_fuel = 0;
    state->gate_calls = 0;
    load_typed(state);
    state->route_ops.ideal_warp = find_ideal_warp;
    state->route_ops.distance = find_distance;
    state->route_ops.find_planet = find_planet;
    state->route_ops.gate_damage = real_gate_damage;
    state->route_ops.estimate_fuel = real_fuel;
    state->route_ops.context = state;
    state->gate_input.lookup = gate_lookup;
    state->gate_input.context = state;
    state->source.route_target_id = state->target.id;
    state->move_fleet.id = 41;
    state->move_fleet.owner = state->route_fleet.owner;
    state->move_fleet.x = state->route_fleet.x;
    state->move_fleet.y = state->route_fleet.y;
    state->move_fleet.active = 1;
    state->move_fleet.fuel = 5000L;
    state->move_fleet.fuel_capacity = 5000L;
    state->move_ops.get_order = get_move_order;
    state->move_ops.distance = move_distance;
    state->move_ops.fuel_use = move_fuel;
    state->move_ops.context = state;
}

static int run_case(AutoRouteE2State *state, StarsI16 minerals,
    StarsI16 gate_damage_case, StarsI16 fuel_limited, int *checks)
{
    StarsI16 expected_warp;
    StarsI16 actual_warp;
    StarsI16 expected_damage;
    StarsI16 actual_damage;
    StarsI16 expected_gate_result;
    StarsI16 actual_gate_result;
    StarsI16 expected_gate_calls;
    StarsI16 slot;
    StarsI32 distance;
    StarsI32 expected_fuel;
    StarsI32 expected_x;
    StarsI32 expected_y;
    StarsI32 expected_fleet_fuel;
    StarsAutoRouteInput route_input;
    StarsMoveFleetsInput move_input;
    int failures;

    initialize_turn(state);
    state->gates[1].fuel_max = 1000;
    state->gates[1].defense = 1000;
    state->gates[2].fuel_max = 1000;
    state->gates[2].defense = 1000;
    if (minerals)
        state->route_fleet.mineral_weights[3] =
            state->fuel_fleet.mineral_weight[3] = 1L;
    if (gate_damage_case)
        state->gates[2].defense = 10;
    if (fuel_limited)
        state->route_fleet.fuel_capacity = 1L;
    expected_gate_calls = minerals ? 0 : 1;
    distance = route_distance(state->route_fleet.x, state->route_fleet.y,
        state->target.x, state->target.y);

    expected_gate_result = oracle_gate_damage(state, 1, 2,
        (StarsI16)distance, 50, &expected_damage);
    actual_gate_result = stars_calc_stargate_damage(&state->gate_input, 1, 2,
        (StarsI16)distance, 50, &actual_damage);
    failures = 0;
    ++*checks;
    if (expected_gate_result != actual_gate_result ||
            expected_damage != actual_damage) {
        ++failures;
    }

    oracle_route(state, &expected_warp);
    route_input.fleet = &state->route_fleet;
    route_input.source = &state->source;
    route_input.ops = &state->route_ops;
    stars_auto_route_fleet(&route_input);
    actual_warp = (StarsI16)((state->route_orders[1].flags >> 4) & 0x000fU);
    ++*checks;
    if (actual_warp != expected_warp || state->route_fleet.cord != 2 ||
            state->route_orders[1].target_id != state->target.id) {
        ++failures;
    }

    for (slot = 0; slot < 16; ++slot) {
        ++*checks;
        if (state->route_fleet.cargo_hull_capacity[slot] !=
                (StarsI32)get_word(state->hulls[1], 52U))
            ++failures;
    }
    ++*checks;
    if ((minerals == 0 && gate_damage_case == 0 &&
            state->route_range_calls != 0) ||
            (minerals != 0 && state->route_cost_calls == 0) ||
            (gate_damage_case != 0 && fuel_limited &&
                state->route_range_calls == 0))
        ++failures;
    ++*checks;
    if (state->gate_calls != expected_gate_calls) {
        ++failures;
    }
    expected_fuel = oracle_fuel(&state->fuel_fleet, actual_warp, distance, 0);
    expected_x = state->target.x;
    expected_y = state->target.y;
    expected_fleet_fuel = 5000L - expected_fuel;
    move_input.fleets = &state->move_fleet;
    move_input.fleet_count = 1;
    move_input.post_movement = 0;
    move_input.ops = &state->move_ops;
    stars_move_fleets(&move_input);
    ++*checks;
    if (state->move_fleet.x != expected_x || state->move_fleet.y != expected_y ||
            state->move_fleet.target_id != state->target.id ||
            state->move_fleet.fuel != expected_fleet_fuel ||
            state->move_fuel_calls != 1) {
        ++failures;
    }
    if (fuel_limited && expected_fuel <= 1L)
        ++failures;
    return failures;
}

int main(void)
{
    AutoRouteE2State state;
    StarsEstFuelInput fuel_input;
    StarsI32 expected;
    StarsI32 actual;
    int failures;
    int checks;

    failures = 0;
    checks = 0;
    seed_state(&state);
    load_typed(&state);
    memset(&fuel_input, 0, sizeof fuel_input);
    fuel_input.fleet = &state.fuel_fleet;
    fuel_input.warp = 8;
    fuel_input.travel = 120;
    expected = oracle_fuel(&state.fuel_fleet, 8, 120, 0);
    actual = stars_est_fuel_use(&fuel_input);
    ++checks;
    if (expected != actual)
        ++failures;
    fuel_input.range_only = 1;
    fuel_input.travel = -1L;
    expected = oracle_fuel(&state.fuel_fleet, 8, -1L, 1);
    actual = stars_est_fuel_use(&fuel_input);
    ++checks;
    if (expected != actual)
        ++failures;

    if (run_case(&state, 0, 0, 0, &checks) != 0)
        ++failures;
    if (run_case(&state, 1, 0, 0, &checks) != 0)
        ++failures;
    if (run_case(&state, 0, 1, 0, &checks) != 0)
        ++failures;
    if (run_case(&state, 0, 1, 1, &checks) != 0)
        ++failures;

    if (failures == 0) {
#ifdef INTEGRATION_NATIVE
        puts("AUTO_ROUTE_FLEET_E2_CHECKS=86 FAILURES=0 RESULT=PASS");
#else
        printf("AUTO_ROUTE_FLEET_E2_CHECKS=%d FAILURES=0 RESULT=PASS\n",
            checks);
#endif
    } else {
#ifdef INTEGRATION_NATIVE
        puts("AUTO_ROUTE_FLEET_E2_CHECKS=86 FAILURES=1 RESULT=FAIL");
#else
        printf("AUTO_ROUTE_FLEET_E2_CHECKS=%d FAILURES=%d RESULT=FAIL\n",
            checks, failures);
#endif
    }
    return failures != 0;
}
