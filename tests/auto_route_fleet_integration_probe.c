#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedFleet {
    StarsU8 raw[124];
    StarsU8 orders_raw[40];
    StarsAutoRouteOrder typed_orders[2];
    StarsAutoRoutePlanet target;
    StarsI16 gate_calls;
    StarsI16 fuel_calls;
    StarsI16 allow_gate;
    StarsI16 damage_value;
    StarsI32 last_gate_cargo;
    StarsI32 last_gate_distance;
    StarsI16 ideal_warp_value;
} PackedFleet;

static StarsU16 get_word(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU16)(p[offset] | ((StarsU16)p[offset + 1U] << 8));
}

static void put_word(StarsU8 *p, StarsU16 offset, StarsU16 value)
{
    p[offset] = (StarsU8)value;
    p[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU32 get_long(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU32)get_word(p, offset) |
        ((StarsU32)get_word(p, (StarsU16)(offset + 2U)) << 16);
}

static void put_long(StarsU8 *p, StarsU16 offset, StarsU32 value)
{
    put_word(p, offset, (StarsU16)value);
    put_word(p, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

static void decode_orders(PackedFleet *packed)
{
    StarsI16 index;
    StarsU16 offset;
    memset(packed->typed_orders, 0, sizeof packed->typed_orders);
    for (index = 0; index < 2; ++index) {
        offset = (StarsU16)(4U + (StarsU16)index * 18U);
        packed->typed_orders[index].x = (StarsI16)get_word(
            packed->orders_raw, offset);
        packed->typed_orders[index].y = (StarsI16)get_word(
            packed->orders_raw, (StarsU16)(offset + 2U));
        packed->typed_orders[index].target_id = (StarsI16)get_word(
            packed->orders_raw, (StarsU16)(offset + 4U));
        packed->typed_orders[index].flags = get_word(
            packed->orders_raw, (StarsU16)(offset + 6U));
    }
}

static void encode_orders(PackedFleet *packed)
{
    StarsU16 offset;
    offset = 22U;
    put_word(packed->orders_raw, offset,
        (StarsU16)packed->typed_orders[1].x);
    put_word(packed->orders_raw, (StarsU16)(offset + 2U),
        (StarsU16)packed->typed_orders[1].y);
    put_word(packed->orders_raw, (StarsU16)(offset + 4U),
        (StarsU16)packed->typed_orders[1].target_id);
    put_word(packed->orders_raw, (StarsU16)(offset + 6U),
        packed->typed_orders[1].flags);
    put_word(packed->raw, 98U, 2U);
}

static StarsI16 ideal_warp(void *context, const StarsAutoRouteFleet *fleet,
    StarsI16 order)
{
    (void)context;
    (void)fleet;
    (void)order;
    return ((PackedFleet *)context)->ideal_warp_value;
}

static StarsI32 distance(void *context, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    (void)context;
    (void)y1;
    (void)y2;
    return x2 - x1;
}

static StarsAutoRoutePlanet *find_planet(void *context, StarsI16 id)
{
    PackedFleet *packed = (PackedFleet *)context;
    return id == packed->target.id ? &packed->target :
        (StarsAutoRoutePlanet *)0;
}

static StarsI16 gate_damage(void *context, StarsI16 source_gate,
    StarsI16 target_gate, StarsI32 distance_value, StarsI32 cargo,
    StarsI16 *damage)
{
    PackedFleet *packed = (PackedFleet *)context;
    (void)source_gate;
    (void)target_gate;
    (void)cargo;
    ++packed->gate_calls;
    packed->last_gate_cargo = cargo;
    packed->last_gate_distance = distance_value;
    *damage = packed->damage_value;
    return packed->allow_gate;
}

static StarsI32 estimate_fuel(void *context, const StarsAutoRouteFleet *fleet,
    StarsI16 segment, StarsI16 warp, StarsI32 distance_value,
    StarsI16 max_cargo)
{
    PackedFleet *packed = (PackedFleet *)context;
    (void)fleet;
    (void)segment;
    (void)max_cargo;
    ++packed->fuel_calls;
    return distance_value < 0 ? 1000L / warp : distance_value / warp;
}

static void initialize(PackedFleet *packed, StarsAutoRouteFleet *fleet,
    StarsAutoRoutePlanet *source, StarsAutoRouteOps *ops,
    StarsAutoRouteInput *input)
{
    memset(packed, 0, sizeof *packed);
    put_word(packed->raw, 0, 21U);
    put_word(packed->raw, 2, 2U);
    put_word(packed->raw, 8, 0U);
    put_word(packed->raw, 10, 0U);
    put_long(packed->raw, 76, 0UL);
    packed->orders_raw[2] = 2U;
    packed->orders_raw[3] = 2U;
    packed->target.id = 3;
    packed->target.owner = 2;
    packed->target.x = 100;
    packed->target.y = 0;
    packed->target.status_flags = 0x0200U;
    packed->target.stargate = 7;
    packed->target.hull_cargo_max = 1;
    memset(fleet, 0, sizeof *fleet);
    memset(source, 0, sizeof *source);
    memset(ops, 0, sizeof *ops);
    fleet->owner = (StarsI16)get_word(packed->raw, 2);
    fleet->x = (StarsI16)get_word(packed->raw, 8);
    fleet->y = (StarsI16)get_word(packed->raw, 10);
    fleet->fuel_capacity = 1000L;
    fleet->cargo_hull_capacity[0] = (StarsI16)get_long(packed->raw, 12);
    fleet->orders = packed->typed_orders;
    source->owner = 2;
    source->route_target_id = 3;
    source->status_flags = 0x0200U;
    source->stargate = 6;
    ops->ideal_warp = ideal_warp;
    ops->distance = distance;
    ops->find_planet = find_planet;
    ops->gate_damage = gate_damage;
    ops->estimate_fuel = estimate_fuel;
    ops->context = packed;
    input->fleet = fleet;
    input->source = source;
    input->ops = ops;
    decode_orders(packed);
    packed->ideal_warp_value = 5;
}

/* Independent transcription of the raw AutoRouteFleet control flow.  The
 * matrix below compares the reconstructed source with this oracle across
 * route-helper and resource branches rather than merely repeating the same
 * callback calls. */
static void reference_auto_route(StarsAutoRouteInput *input)
{
    StarsAutoRouteFleet *fleet = input->fleet;
    const StarsAutoRoutePlanet *source = input->source;
    const StarsAutoRouteOps *ops = input->ops;
    StarsAutoRouteOrder *order = &fleet->orders[1];
    StarsAutoRoutePlanet *target;
    StarsI16 warp;
    StarsI16 lower;
    StarsI16 slot;
    StarsI16 damage;
    StarsI32 distance_value;
    StarsI32 fuel;
    StarsI32 lower_fuel;
    StarsI32 max_cargo = 0;
    StarsI16 has_minerals = 0;

    fleet->cord = 2;
    fleet->orders[0].flags = 0;
    order->flags = (StarsU16)(8U | (1U << 8));
    target = ops->find_planet ? ops->find_planet(ops->context,
        source->route_target_id) : (StarsAutoRoutePlanet *)0;
    if (!target)
        return;
    order->target_id = target->id;
    order->x = target->x;
    order->y = target->y;
    order->flags = (StarsU16)(order->flags | 0x1000U);
    warp = ops->ideal_warp ? ops->ideal_warp(ops->context, fleet, 0) : 0;
    distance_value = ops->distance ? ops->distance(ops->context, fleet->x,
        fleet->y, order->x, order->y) :
        (StarsI32)(order->x > fleet->x ? order->x - fleet->x :
            fleet->x - order->x) +
        (StarsI32)(order->y > fleet->y ? order->y - fleet->y :
            fleet->y - order->y);
    for (slot = 0; slot < 4; ++slot)
        if (fleet->mineral_weights[slot] != 0)
            has_minerals = 1;
    if (!has_minerals && source->owner == target->owner &&
            (source->status_flags & 0x0200U) != 0U &&
            (target->status_flags & 0x0200U) != 0U &&
            source->stargate != -1 && target->stargate != -1) {
        StarsI16 gate_ok = 0;
        for (slot = 0; slot < 16; ++slot)
            if (fleet->cargo[slot] != 0 &&
                    fleet->cargo_hull_capacity[slot] > max_cargo)
                max_cargo = fleet->cargo_hull_capacity[slot];
        damage = 0;
        if (ops->gate_damage)
            gate_ok = ops->gate_damage(ops->context, source->stargate,
                target->stargate, distance_value, max_cargo, &damage);
        if (gate_ok && damage == 0)
            warp = 11;
        if (warp < 9 && target->hull_cargo_max != 0) {
            for (slot = 9; slot > 0; --slot) {
                fuel = ops->estimate_fuel ? ops->estimate_fuel(
                    ops->context, fleet, 0, slot, -1, 1) : -1;
                if (distance_value <= fuel)
                    break;
            }
            warp = slot;
        }
    }
    if (warp > 0 && warp < 11) {
        fuel = distance_value / ((StarsI32)warp * warp);
        lower = warp;
        while (lower >= 3) {
            --lower;
            if (lower <= 0)
                break;
            lower_fuel = distance_value / ((StarsI32)lower * lower);
            if (lower_fuel > fuel) {
                ++lower;
                break;
            }
            fuel = lower_fuel;
        }
        warp = lower;
        while (warp > 0) {
            fuel = ops->estimate_fuel ? ops->estimate_fuel(ops->context,
                fleet, 0, warp, distance_value, 0) : distance_value;
            if (fuel <= fleet->fuel_capacity || warp == 1)
                break;
            --warp;
        }
    }
    order->flags = (StarsU16)(order->flags & (StarsU16)~0x00f0U);
    order->flags = (StarsU16)(order->flags | ((StarsU16)warp << 4));
}

int main(void)
{
    PackedFleet packed;
    StarsAutoRouteFleet fleet;
    StarsAutoRoutePlanet source;
    StarsAutoRouteOps ops;
    StarsAutoRouteInput input;
    int failures = 0;
    int checks = 0;

#define CHECK(condition) do { \
    ++checks; \
    if (!(condition)) \
        ++failures; \
} while (0)

    memset(&packed, 0, sizeof packed);
    initialize(&packed, &fleet, &source, &ops, &input);
    packed.allow_gate = 1;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(get_word(packed.raw, 98) == 2U &&
        get_word(packed.orders_raw, 22U + 4U) == 3U &&
        ((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 11U &&
        packed.gate_calls == 1 && packed.fuel_calls == 0);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.allow_gate = 0;
    fleet.fuel_capacity = 10L;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 1U &&
        packed.fuel_calls != 0 && packed.gate_calls == 1);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.allow_gate = 1;
    put_long(packed.raw, 76, 1UL);
    fleet.mineral_weights[0] = (StarsI32)get_long(packed.raw, 76);
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(packed.gate_calls == 0 &&
        ((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 5U);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.allow_gate = 1;
    source.owner = 3;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(packed.gate_calls == 0 &&
        ((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 5U);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.allow_gate = 1;
    packed.damage_value = 4;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(packed.gate_calls == 1 &&
        ((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 8U);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.target.id = 99;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(get_word(packed.raw, 98) == 2U &&
        get_word(packed.orders_raw, 22U + 4U) == 0U &&
        get_word(packed.orders_raw, 22U + 6U) == 0x0108U &&
        packed.gate_calls == 0 && packed.fuel_calls == 0);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.ideal_warp_value = 0;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 8U &&
        packed.gate_calls == 1);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.allow_gate = 1;
    put_long(packed.raw, 12, 77UL);
    fleet.cargo_hull_capacity[0] = 77;
    fleet.cargo[0] = 1;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(packed.gate_calls == 1 && packed.last_gate_cargo == 77L &&
        ((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 11U);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.allow_gate = 1;
    ops.ideal_warp = (StarsAutoRouteIdealWarp)0;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(packed.gate_calls == 1 &&
        ((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 11U);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.allow_gate = 1;
    ops.distance = (StarsAutoRouteDistance)0;
    packed.target.y = 20;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(packed.gate_calls == 1 && packed.last_gate_distance == 120L);

    initialize(&packed, &fleet, &source, &ops, &input);
    source.status_flags = 0;
    source.stargate = -1;
    packed.target.status_flags = 0;
    packed.target.stargate = -1;
    fleet.fuel_capacity = 1L;
    ops.estimate_fuel = (StarsAutoRouteEstimateFuel)0;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(packed.gate_calls == 0 && packed.fuel_calls == 0 &&
        ((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 1U);

    initialize(&packed, &fleet, &source, &ops, &input);
    packed.allow_gate = 1;
    fleet.mineral_weights[0] = 0L;
    fleet.mineral_weights[1] = 0L;
    fleet.mineral_weights[2] = 0L;
    fleet.mineral_weights[3] = 1L;
    stars_auto_route_fleet(&input);
    encode_orders(&packed);
    CHECK(packed.gate_calls == 0 &&
        ((get_word(packed.orders_raw, 22U + 6U) >> 4) & 0x0fU) == 5U);

    /* Route differential matrix: vary helper availability, gate/resource
     * predicates, mineral slots, target geometry, and fuel capacity. */
    {
        StarsI16 route_case;
        for (route_case = 0; route_case < 12; ++route_case) {
            PackedFleet ref_packed;
            PackedFleet actual_packed;
            StarsAutoRouteFleet ref_fleet;
            StarsAutoRouteFleet actual_fleet;
            StarsAutoRoutePlanet ref_source;
            StarsAutoRoutePlanet actual_source;
            StarsAutoRouteOps ref_ops;
            StarsAutoRouteOps actual_ops;
            StarsAutoRouteInput ref_input;
            StarsAutoRouteInput actual_input;
            StarsU16 ref_flags;
            StarsU16 actual_flags;

            initialize(&ref_packed, &ref_fleet, &ref_source, &ref_ops,
                &ref_input);
            initialize(&actual_packed, &actual_fleet, &actual_source,
                &actual_ops, &actual_input);
            ref_packed.allow_gate = actual_packed.allow_gate =
                (StarsI16)(route_case & 1);
            ref_packed.damage_value = actual_packed.damage_value =
                (StarsI16)((route_case & 2) ? 3 : 0);
            ref_packed.target.y = actual_packed.target.y =
                (StarsI16)((route_case & 4) ? 20 : 0);
            ref_fleet.fuel_capacity = actual_fleet.fuel_capacity =
                (StarsI32)((route_case & 8) ? 10 : 1000);
            ref_fleet.mineral_weights[route_case % 4] =
                actual_fleet.mineral_weights[route_case % 4] =
                (StarsI32)((route_case & 8) ? 1 : 0);
            ref_fleet.cargo[route_case % 3] =
                actual_fleet.cargo[route_case % 3] = 1;
            ref_fleet.cargo_hull_capacity[route_case % 3] =
                actual_fleet.cargo_hull_capacity[route_case % 3] =
                (StarsI32)(30 + route_case);
            if (route_case & 1) {
                ref_ops.distance = (StarsAutoRouteDistance)0;
                actual_ops.distance = (StarsAutoRouteDistance)0;
            }
            if (route_case & 2) {
                ref_ops.ideal_warp = (StarsAutoRouteIdealWarp)0;
                actual_ops.ideal_warp = (StarsAutoRouteIdealWarp)0;
            }
            if (route_case & 4) {
                ref_ops.estimate_fuel = (StarsAutoRouteEstimateFuel)0;
                actual_ops.estimate_fuel = (StarsAutoRouteEstimateFuel)0;
            }
            reference_auto_route(&ref_input);
            stars_auto_route_fleet(&actual_input);
            ref_flags = (StarsU16)(ref_fleet.orders[1].flags & 0x10f0U);
            actual_flags = (StarsU16)(actual_fleet.orders[1].flags & 0x10f0U);
            CHECK(ref_fleet.cord == actual_fleet.cord &&
                ref_fleet.orders[1].target_id ==
                    actual_fleet.orders[1].target_id &&
                ref_fleet.orders[1].x == actual_fleet.orders[1].x &&
                ref_fleet.orders[1].y == actual_fleet.orders[1].y &&
                ref_flags == actual_flags &&
                ref_packed.gate_calls == actual_packed.gate_calls &&
                ref_packed.fuel_calls == actual_packed.fuel_calls &&
                ref_packed.last_gate_cargo ==
                    actual_packed.last_gate_cargo &&
                ref_packed.last_gate_distance ==
                    actual_packed.last_gate_distance);
        }
    }

    if (checks != 24)
        ++failures;
    if (failures == 0)
        puts("AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS");
    else
        puts("AUTO_ROUTE_FLEET_INTEGRATION_CHECKS=24 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}
