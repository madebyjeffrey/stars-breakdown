#include <stdio.h>
#include <string.h>
#include "core.h"
#include "common.h"
#include "satisfy_orders_wormholes_turn_e3_fixture.h"

typedef struct WormholeTurnTrace {
    StarsMoveThing things[WORMHOLE_COUNT];
    StarsI16 random_index;
    StarsI16 validation_calls;
    StarsI16 waypoint_calls;
    StarsI16 failures;
} WormholeTurnTrace;

static StarsU16 get_u16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1U] << 8));
}

static void put_u16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsI16 random_value(void *opaque, StarsI16 limit)
{
    WormholeTurnTrace *trace = (WormholeTurnTrace *)opaque;
    StarsI16 index = trace->random_index;
    if (index >= WORMHOLE_DRAW_COUNT) {
        ++trace->failures;
        return 0;
    }
    if (limit != retained_draw_limits[index] ||
            retained_draws[index] < 0 || retained_draws[index] >= limit)
        ++trace->failures;
    ++trace->random_index;
    return retained_draws[index];
}

static StarsI16 wormhole_chance(void *opaque, const StarsMoveThing *thing)
{
    (void)opaque;
    return stars_pct_wormhole_moves(thing->packed_destination);
}

static StarsI16 validate_wormhole(void *opaque, StarsMoveThing *thing)
{
    WormholeTurnTrace *trace = (WormholeTurnTrace *)opaque;
    StarsWormholePoint other_things[WORMHOLE_COUNT - 1];
    StarsU16 index;
    StarsU16 count = 0;
    StarsU16 current;

    current = (StarsU16)(thing - trace->things);
    if (current >= WORMHOLE_COUNT) {
        ++trace->failures;
        return 0x0f;
    }
    for (index = 0; index < WORMHOLE_COUNT; ++index) {
        const StarsMoveThing *other;
        if (index == current) continue;
        other = &trace->things[index];
        other_things[count].x = other->x;
        other_things[count].y = other->y;
        other_things[count].identity = other->identity;
        other_things[count].flags = other->identity;
        ++count;
    }
    ++trace->validation_calls;
    return stars_validate_wormhole_position(thing->x, thing->y,
        thing->partner_identity, WORMHOLE_MAP_SIZE, other_things, count,
        retained_planets, WORMHOLE_PLANET_COUNT, retained_fleets,
        WORMHOLE_FLEET_COUNT);
}

static void validate_waypoints(void *opaque)
{
    WormholeTurnTrace *trace = (WormholeTurnTrace *)opaque;
    ++trace->waypoint_calls;
}

static void check(WormholeTurnTrace *trace, StarsI16 *checks,
    StarsI16 condition)
{
    ++*checks;
    if (!condition) ++trace->failures;
}

int main(void)
{
    WormholeTurnTrace trace;
    StarsMoveThingsOps ops;
    StarsMoveThingsInput input;
    StarsU8 output_record[18];
    StarsU16 index;
    StarsI16 checks = 0;

    memset(&trace, 0, sizeof trace);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    for (index = 0; index < WORMHOLE_COUNT; ++index) {
        StarsMoveThing *thing = &trace.things[index];
        const StarsU8 *raw = retained_input_things[index];
        StarsU16 identity = get_u16(raw, 0U);
        thing->kind = (StarsI16)(identity >> 13);
        thing->owner = (StarsI16)((identity >> 9) & 0x0fU);
        thing->identity = identity;
        thing->partner_identity = get_u16(raw, 12U);
        thing->x = (StarsI16)get_u16(raw, 2U);
        thing->y = (StarsI16)get_u16(raw, 4U);
        thing->packed_destination = get_u16(raw, 6U);
        thing->active = 1;
    }
    ops.random_value = random_value;
    ops.wormhole_chance = wormhole_chance;
    ops.validate_wormhole = validate_wormhole;
    ops.validate_waypoints = validate_waypoints;
    ops.context = &trace;
    input.things = trace.things;
    input.thing_count = WORMHOLE_COUNT;
    input.post_production = 1;
    input.map_size = WORMHOLE_MAP_SIZE;
    input.ops = &ops;
    stars_move_things(&input);

    check(&trace, &checks,
        trace.random_index == WORMHOLE_DRAW_COUNT);
    check(&trace, &checks,
        trace.validation_calls == WORMHOLE_COUNT);
    check(&trace, &checks, trace.waypoint_calls == 1);
    for (index = 0; index < WORMHOLE_COUNT; ++index) {
        memcpy(output_record, retained_input_things[index],
            sizeof output_record);
        put_u16(output_record, 2U, (StarsU16)trace.things[index].x);
        put_u16(output_record, 4U, (StarsU16)trace.things[index].y);
        put_u16(output_record, 6U,
            trace.things[index].packed_destination);
        check(&trace, &checks,
            trace.things[index].x == retained_expected_x[index]);
        check(&trace, &checks,
            trace.things[index].y == retained_expected_y[index]);
        check(&trace, &checks,
            trace.things[index].packed_destination ==
                retained_expected_destination[index]);
        check(&trace, &checks,
            memcmp(output_record, retained_output_things[index],
                sizeof output_record) == 0);
    }
    if (trace.failures == 0)
        puts("SATISFY_ORDERS_WORMHOLE_MOVE_CHECKS=19 FAILURES=0 RESULT=PASS");
    else
        printf("SATISFY_ORDERS_WORMHOLE_MOVE_CHECKS=%d FAILURES=%d RESULT=FAIL\n",
            (int)checks, (int)trace.failures);
    return trace.failures == 0 ? 0 : 1;
}
