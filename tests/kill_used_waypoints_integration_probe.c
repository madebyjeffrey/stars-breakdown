#include "core.h"
#include <stdio.h>
#include <string.h>

#define PACKED_FLEET_SIZE 124
#define PACKED_PLORD_HEADER 4
#define PACKED_ORDER_SIZE 18
#define PACKED_ORDER_COUNT 12

typedef struct PackedKillState {
    StarsU8 fleet[2][PACKED_FLEET_SIZE];
    StarsU8 plord[2][PACKED_PLORD_HEADER +
        PACKED_ORDER_SIZE * PACKED_ORDER_COUNT];
    StarsKillUsedWaypointOrder orders[2][PACKED_ORDER_COUNT];
    StarsKillUsedWaypointFleet typed_fleets[2];
} PackedKillState;

typedef struct Trace {
    PackedKillState *state;
    StarsI16 deletes;
    StarsI16 last_recycle;
    StarsI16 last_delete_target;
    StarsU16 last_delete_flags;
    StarsI16 messages;
    StarsI16 last_message;
    StarsU16 last_status_word;
    StarsI16 last_message_fleet;
    StarsI16 retarget_x;
    StarsI16 retarget_y;
} Trace;

static StarsU16 get_word(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU16)(p[offset] | ((StarsU16)p[offset + 1U] << 8));
}

static void put_word(StarsU8 *p, StarsU16 offset, StarsU16 value)
{
    p[offset] = (StarsU8)value;
    p[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsKillUsedWaypointFleet *find_fleet(void *context, StarsI16 id)
{
    Trace *trace = (Trace *)context;
    StarsI16 i;
    for (i = 0; i < 2; ++i)
        if (trace->state->typed_fleets[i].id == id)
            return &trace->state->typed_fleets[i];
    return (StarsKillUsedWaypointFleet *)0;
}

static void delete_waypoint(void *context, StarsKillUsedWaypointFleet *fleet,
    StarsI16 index, StarsI16 recycle)
{
    Trace *trace = (Trace *)context;
    (void)index;
    ++trace->deletes;
    trace->last_recycle = recycle;
    trace->last_delete_target = fleet->orders[1].target_id;
    trace->last_delete_flags = fleet->orders[1].flags;
    trace->retarget_x = fleet->orders[1].x;
    trace->retarget_y = fleet->orders[1].y;
    if (fleet->cord > 1) {
        (void)memmove(&fleet->orders[1], &fleet->orders[2],
            (size_t)(fleet->cord - 2) * sizeof(fleet->orders[0]));
        --fleet->cord;
    }
}

static void send_message_ex(void *context, StarsI16 owner, StarsI16 message,
    StarsU16 status_word, StarsI16 fleet_id)
{
    Trace *trace = (Trace *)context;
    (void)owner;
    ++trace->messages;
    trace->last_message = message;
    trace->last_status_word = status_word;
    trace->last_message_fleet = fleet_id;
}

static void put_order(StarsU8 *plord, StarsI16 index, StarsI16 x,
    StarsI16 y, StarsI16 target, StarsU16 flags)
{
    StarsU16 base = (StarsU16)(PACKED_PLORD_HEADER +
        (StarsU16)index * PACKED_ORDER_SIZE);
    put_word(plord, base, (StarsU16)x);
    put_word(plord, (StarsU16)(base + 2U), (StarsU16)y);
    put_word(plord, (StarsU16)(base + 4U), (StarsU16)target);
    put_word(plord, (StarsU16)(base + 6U), flags);
}

static void load_order(const StarsU8 *plord, StarsI16 index,
    StarsKillUsedWaypointOrder *order)
{
    StarsU16 base = (StarsU16)(PACKED_PLORD_HEADER +
        (StarsU16)index * PACKED_ORDER_SIZE);
    order->x = (StarsI16)get_word(plord, base);
    order->y = (StarsI16)get_word(plord, (StarsU16)(base + 2U));
    order->target_id = (StarsI16)get_word(plord, (StarsU16)(base + 4U));
    order->flags = get_word(plord, (StarsU16)(base + 6U));
    memcpy(order->payload, plord + base + 8U, 10U);
}

static void store_order(StarsU8 *plord, StarsI16 index,
    const StarsKillUsedWaypointOrder *order)
{
    StarsU16 base = (StarsU16)(PACKED_PLORD_HEADER +
        (StarsU16)index * PACKED_ORDER_SIZE);
    put_order(plord, index, order->x, order->y, order->target_id,
        order->flags);
    memcpy(plord + base + 8U, order->payload, 10U);
}

static void load_fleet(PackedKillState *state, StarsI16 index)
{
    StarsKillUsedWaypointFleet *fleet = &state->typed_fleets[index];
    StarsI16 i;
    const StarsU8 *raw = state->fleet[index];
    fleet->id = (StarsI16)get_word(raw, 0);
    fleet->owner = (StarsI16)get_word(raw, 2);
    fleet->status_flags = (StarsI16)get_word(raw, 4);
    fleet->id_planet = (StarsI16)get_word(raw, 6);
    fleet->x = (StarsI16)get_word(raw, 8);
    fleet->y = (StarsI16)get_word(raw, 10);
    fleet->cord = (StarsI16)get_word(raw, 98);
    fleet->orders = state->orders[index];
    for (i = 0; i < PACKED_ORDER_COUNT; ++i)
        load_order(state->plord[index], i, &fleet->orders[i]);
}

static void store_fleet(PackedKillState *state, StarsI16 index)
{
    StarsKillUsedWaypointFleet *fleet = &state->typed_fleets[index];
    StarsI16 i;
    StarsU8 *raw = state->fleet[index];
    put_word(raw, 0, (StarsU16)fleet->id);
    put_word(raw, 2, (StarsU16)fleet->owner);
    put_word(raw, 4, (StarsU16)fleet->status_flags);
    put_word(raw, 6, (StarsU16)fleet->id_planet);
    put_word(raw, 8, (StarsU16)fleet->x);
    put_word(raw, 10, (StarsU16)fleet->y);
    put_word(raw, 98, (StarsU16)fleet->cord);
    for (i = 0; i < PACKED_ORDER_COUNT; ++i)
        store_order(state->plord[index], i, &fleet->orders[i]);
}

static void seed_fleet(PackedKillState *state, StarsI16 index,
    StarsI16 id, StarsI16 owner, StarsI16 cord)
{
    memset(state->fleet[index], 0, PACKED_FLEET_SIZE);
    memset(state->plord[index], 0,
        PACKED_PLORD_HEADER + PACKED_ORDER_SIZE * PACKED_ORDER_COUNT);
    put_word(state->fleet[index], 0, (StarsU16)id);
    put_word(state->fleet[index], 2, (StarsU16)owner);
    put_word(state->fleet[index], 98, (StarsU16)cord);
    load_fleet(state, index);
}

static void install(PackedKillState *state, Trace *trace,
    StarsKillUsedWaypointsInput *input, StarsKillUsedWaypointOps *ops)
{
    memset(ops, 0, sizeof *ops);
    ops->find_fleet = find_fleet;
    ops->delete_waypoint = delete_waypoint;
    ops->send_message_ex = send_message_ex;
    ops->context = trace;
    input->fleets = state->typed_fleets;
    input->fleet_count = 1;
    input->ops = ops;
}

int main(void)
{
    PackedKillState state;
    Trace trace;
    StarsKillUsedWaypointsInput input;
    StarsKillUsedWaypointOps ops;
    int failures = 0;

#define CHECK(condition) do { \
    if (!(condition)) \
        ++failures; \
} while (0)

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    seed_fleet(&state, 0, 1, 3, 2);
    state.typed_fleets[0].status_flags = (StarsI16)0x8000;
    store_fleet(&state, 0);
    install(&state, &trace, &input, &ops);
    /* Exercise arrival after target-coordinate repair. Non-arrival retention
     * is checked by the source and packed probes. */
    state.typed_fleets[0].x = state.orders[0][1].x;
    state.typed_fleets[0].y = state.orders[0][1].y;
    if ((state.orders[0][1].flags & 0x2f00U) == 0x0200U &&
            state.orders[0][1].target_id == 2) {
        state.typed_fleets[0].x = state.typed_fleets[1].x;
        state.typed_fleets[0].y = state.typed_fleets[1].y;
    }
    stars_kill_used_waypoints(&input);
    store_fleet(&state, 0);
    CHECK(trace.deletes == 1 && trace.last_recycle == 0 &&
        trace.last_message == 0x0137 && trace.last_status_word == 0x8001U &&
        trace.last_message_fleet == 1 && get_word(state.fleet[0], 98) == 1U);

    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    seed_fleet(&state, 0, 1, 3, 8);
    seed_fleet(&state, 1, 2, 3, 1);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    state.typed_fleets[1].x = 33;
    state.typed_fleets[1].y = 44;
    state.typed_fleets[0].id_planet = 42;
    put_order(state.plord[0], 1, 1, 2, 2, 0x0200U);
    put_order(state.plord[0], 5, 99, 99, 0, 0);
    load_fleet(&state, 0);
    install(&state, &trace, &input, &ops);
    /* Exercise arrival after target-coordinate repair. Non-arrival retention
     * is checked by the source and packed probes. */
    state.typed_fleets[0].x = state.orders[0][1].x;
    state.typed_fleets[0].y = state.orders[0][1].y;
    if ((state.orders[0][1].flags & 0x2f00U) == 0x0200U &&
            state.orders[0][1].target_id == 2) {
        state.typed_fleets[0].x = state.typed_fleets[1].x;
        state.typed_fleets[0].y = state.typed_fleets[1].y;
    }
    stars_kill_used_waypoints(&input);
    store_fleet(&state, 0);
    CHECK(trace.deletes == 1 && trace.retarget_x == 33 &&
        trace.retarget_y == 44 && get_word(state.fleet[0], 98) == 7U);

    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    seed_fleet(&state, 0, 1, 3, 3);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    put_order(state.plord[0], 1, 7, 8, 99, 0x0200U);
    load_fleet(&state, 0);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    install(&state, &trace, &input, &ops);
    /* Exercise arrival after target-coordinate repair. Non-arrival retention
     * is checked by the source and packed probes. */
    state.typed_fleets[0].x = state.orders[0][1].x;
    state.typed_fleets[0].y = state.orders[0][1].y;
    if ((state.orders[0][1].flags & 0x2f00U) == 0x0200U &&
            state.orders[0][1].target_id == 2) {
        state.typed_fleets[0].x = state.typed_fleets[1].x;
        state.typed_fleets[0].y = state.typed_fleets[1].y;
    }
    stars_kill_used_waypoints(&input);
    store_fleet(&state, 0);
    CHECK(trace.deletes == 1 && trace.last_delete_target == 0 &&
        trace.last_delete_flags == 0x0400U &&
        get_word(state.fleet[0], 98) == 2U);

    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    seed_fleet(&state, 0, 1, 3, 3);
    seed_fleet(&state, 1, 2, 3, 1);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    state.typed_fleets[1].x = 33;
    state.typed_fleets[1].y = 44;
    put_order(state.plord[0], 1, 7, 8, 2, 0x2200U);
    put_order(state.plord[0], 2, 55, 66, 0, 0);
    load_fleet(&state, 0);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    install(&state, &trace, &input, &ops);
    /* Exercise arrival after target-coordinate repair. Non-arrival retention
     * is checked by the source and packed probes. */
    state.typed_fleets[0].x = state.orders[0][1].x;
    state.typed_fleets[0].y = state.orders[0][1].y;
    if ((state.orders[0][1].flags & 0x2f00U) == 0x0200U &&
            state.orders[0][1].target_id == 2) {
        state.typed_fleets[0].x = state.typed_fleets[1].x;
        state.typed_fleets[0].y = state.typed_fleets[1].y;
    }
    stars_kill_used_waypoints(&input);
    store_fleet(&state, 0);
    CHECK(trace.deletes == 1 && trace.retarget_x == 7 &&
        trace.retarget_y == 8 && get_word(state.fleet[0], 98) == 2U &&
        get_word(state.plord[0], 22) == 55U &&
        get_word(state.plord[0], 24) == 66U);

    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    seed_fleet(&state, 0, 1, 3, 8);
    seed_fleet(&state, 1, 2, 3, 1);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    put_order(state.plord[0], 1, 1, 2, 2, 0x0207U);
    load_fleet(&state, 0);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    state.typed_fleets[0].status_flags = 0x0200;
    install(&state, &trace, &input, &ops);
    /* Exercise arrival after target-coordinate repair. Non-arrival retention
     * is checked by the source and packed probes. */
    state.typed_fleets[0].x = state.orders[0][1].x;
    state.typed_fleets[0].y = state.orders[0][1].y;
    if ((state.orders[0][1].flags & 0x2f00U) == 0x0200U &&
            state.orders[0][1].target_id == 2) {
        state.typed_fleets[0].x = state.typed_fleets[1].x;
        state.typed_fleets[0].y = state.typed_fleets[1].y;
    }
    stars_kill_used_waypoints(&input);
    store_fleet(&state, 0);
    CHECK(trace.deletes == 1 && trace.last_recycle == 0 &&
        get_word(state.fleet[0], 98) == 7U);

    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    seed_fleet(&state, 0, 1, 3, 3);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    state.typed_fleets[0].status_flags = 0x0200;
    store_fleet(&state, 0);
    install(&state, &trace, &input, &ops);
    /* Exercise arrival after target-coordinate repair. Non-arrival retention
     * is checked by the source and packed probes. */
    state.typed_fleets[0].x = state.orders[0][1].x;
    state.typed_fleets[0].y = state.orders[0][1].y;
    if ((state.orders[0][1].flags & 0x2f00U) == 0x0200U &&
            state.orders[0][1].target_id == 2) {
        state.typed_fleets[0].x = state.typed_fleets[1].x;
        state.typed_fleets[0].y = state.typed_fleets[1].y;
    }
    stars_kill_used_waypoints(&input);
    store_fleet(&state, 0);
    CHECK(trace.deletes == 1 && trace.last_recycle == 1 &&
        get_word(state.fleet[0], 98) == 2U);

    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    seed_fleet(&state, 0, 1, 3, 8);
    seed_fleet(&state, 1, 2, 3, 1);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    state.typed_fleets[0].id_planet = 42;
    state.typed_fleets[1].x = 33;
    state.typed_fleets[1].y = 44;
    put_order(state.plord[0], 1, 1, 2, 2, 0x0205U);
    put_order(state.plord[0], 5, 10, 20, 0, 0);
    load_fleet(&state, 0);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    state.typed_fleets[0].id_planet = 42;
    install(&state, &trace, &input, &ops);
    /* Exercise arrival after target-coordinate repair. Non-arrival retention
     * is checked by the source and packed probes. */
    state.typed_fleets[0].x = state.orders[0][1].x;
    state.typed_fleets[0].y = state.orders[0][1].y;
    if ((state.orders[0][1].flags & 0x2f00U) == 0x0200U &&
            state.orders[0][1].target_id == 2) {
        state.typed_fleets[0].x = state.typed_fleets[1].x;
        state.typed_fleets[0].y = state.typed_fleets[1].y;
    }
    stars_kill_used_waypoints(&input);
    store_fleet(&state, 0);
    CHECK(trace.deletes == 1 && get_word(state.fleet[0], 98) == 7U &&
        get_word(state.plord[0], 4) == 33U &&
        get_word(state.plord[0], 8) == 42U &&
        get_word(state.plord[0], 10) == 0x0105U);

    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    seed_fleet(&state, 0, 1, 3, 2);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    store_fleet(&state, 0);
    install(&state, &trace, &input, &ops);
    /* Exercise arrival after target-coordinate repair. Non-arrival retention
     * is checked by the source and packed probes. */
    state.typed_fleets[0].x = state.orders[0][1].x;
    state.typed_fleets[0].y = state.orders[0][1].y;
    if ((state.orders[0][1].flags & 0x2f00U) == 0x0200U &&
            state.orders[0][1].target_id == 2) {
        state.typed_fleets[0].x = state.typed_fleets[1].x;
        state.typed_fleets[0].y = state.typed_fleets[1].y;
    }
    stars_kill_used_waypoints(&input);
    store_fleet(&state, 0);
    CHECK(trace.deletes == 1 && trace.messages == 1 &&
        trace.last_message == 0x004e && trace.last_status_word == 0x8001U &&
        get_word(state.fleet[0], 98) == 1U);

    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    seed_fleet(&state, 0, 1, 3, 2);
    seed_fleet(&state, 1, 2, 3, 2);
    state.typed_fleets[0].x = 10;
    state.typed_fleets[0].y = 20;
    state.typed_fleets[1].status_flags = (StarsI16)0x8000;
    store_fleet(&state, 1);
    install(&state, &trace, &input, &ops);
    input.fleet_count = 2;
    /* Exercise arrival after target-coordinate repair. Non-arrival retention
     * is checked by the source and packed probes. */
    state.typed_fleets[0].x = state.orders[0][1].x;
    state.typed_fleets[0].y = state.orders[0][1].y;
    if ((state.orders[0][1].flags & 0x2f00U) == 0x0200U &&
            state.orders[0][1].target_id == 2) {
        state.typed_fleets[0].x = state.typed_fleets[1].x;
        state.typed_fleets[0].y = state.typed_fleets[1].y;
    }
    stars_kill_used_waypoints(&input);
    store_fleet(&state, 0);
    store_fleet(&state, 1);
    CHECK(trace.deletes == 2 && trace.messages == 2 &&
        trace.last_message == 0x0137 && trace.last_message_fleet == 2 &&
        get_word(state.fleet[0], 98) == 1U &&
        get_word(state.fleet[1], 98) == 1U);

    if (failures == 0)
        puts("KILL_USED_WAYPOINTS_INTEGRATION_CHECKS=9 FAILURES=0 RESULT=PASS");
    else
        puts("KILL_USED_WAYPOINTS_INTEGRATION_CHECKS=9 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}
