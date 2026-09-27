#include "core.h"
#include <string.h>

#define KILL_WP_TARGET_FLEET 2U
#define KILL_WP_TARGET_MASK 0x0f00U
#define KILL_WP_KIND_MASK 0x000fU
#define KILL_WP_NO_MOVE 0x2000U
#define KILL_WP_INVALID 0x0400U
#define KILL_WP_PLANET 0x0100U
#define KILL_WP_STATUS_DELETED 0x8000U
#define KILL_WP_REUSE_FLAG 0x0200U
#define KILL_WP_PLANET_ORDER 7U
#define KILL_WP_PLANET_KIND 8U
#define KILL_WP_MESSAGE_USED 0x0137
#define KILL_WP_MESSAGE_EMPTY 0x004e

static StarsU16 kill_wp_target_kind(StarsU16 flags)
{
    return (StarsU16)((flags >> 8) & 0x000fU);
}

static void kill_wp_message(const StarsKillUsedWaypointOps *ops,
    const StarsKillUsedWaypointFleet *fleet, StarsI16 message)
{
    if (ops->send_message_ex)
        ops->send_message_ex(ops->context, fleet->owner, message,
            (StarsU16)(fleet->id | (StarsI16)0x8000), fleet->id);
    else if (ops->send_message)
        ops->send_message(ops->context, fleet->owner, message, fleet->id);
}

static StarsKillUsedWaypointFleet *kill_wp_find_fleet(
    const StarsKillUsedWaypointsInput *input, StarsI16 id)
{
    StarsU16 index;
    StarsKillUsedWaypointFleet *fleet;

    if (input->ops->find_fleet)
        return input->ops->find_fleet(input->ops->context, id);
    for (index = 0; index < input->fleet_count; ++index) {
        fleet = &input->fleets[index];
        if (fleet->id == id)
            return fleet;
    }
    return (StarsKillUsedWaypointFleet *)0;
}

static void kill_wp_delete(const StarsKillUsedWaypointsInput *input,
    StarsKillUsedWaypointFleet *fleet, StarsI16 recycle)
{
    if (input->ops->delete_waypoint) {
        input->ops->delete_waypoint(input->ops->context, fleet, 1, recycle);
    } else if (fleet->orders && fleet->cord > 1) {
        (void)memmove(&fleet->orders[1], &fleet->orders[2],
            (size_t)(fleet->cord - 2) * sizeof(fleet->orders[0]));
        --fleet->cord;
    }
}

static StarsI16 kill_wp_retarget_orders(const StarsKillUsedWaypointsInput *input,
    StarsKillUsedWaypointFleet *fleet)
{
    StarsI16 index;
    StarsKillUsedWaypointOrder *order;
    StarsKillUsedWaypointFleet *target;
    StarsU16 next_kind;

    for (index = 1; index < fleet->cord; ++index) {
        order = &fleet->orders[index];
        if (kill_wp_target_kind(order->flags) != KILL_WP_TARGET_FLEET)
            continue;
        target = kill_wp_find_fleet(input, order->target_id);
        if (!target) {
            order->flags = (StarsU16)((order->flags & 0xf0ffU) |
                KILL_WP_INVALID);
            order->target_id = 0;
        } else if ((order->flags & KILL_WP_NO_MOVE) == 0U) {
            order->x = target->x;
            order->y = target->y;
        }
    }
    if (fleet->x != fleet->orders[1].x || fleet->y != fleet->orders[1].y)
        return 0;
    /* Original copies nine WORDs from PLORD+22 to PLORD+4: one ORDER. */
    fleet->orders[0] = fleet->orders[1];
    next_kind = kill_wp_target_kind(fleet->orders[0].flags);
    if (next_kind == KILL_WP_TARGET_FLEET &&
        (fleet->orders[0].flags & KILL_WP_KIND_MASK) != 1U &&
        (fleet->orders[0].flags & KILL_WP_KIND_MASK) != 4U) {
        fleet->orders[0].flags = (StarsU16)(fleet->orders[0].flags & 0xf0ffU);
        if (fleet->id_planet == -1) {
            fleet->orders[0].flags = (StarsU16)(fleet->orders[0].flags |
                KILL_WP_INVALID);
            fleet->orders[0].target_id = 0;
        } else {
            fleet->orders[0].flags = (StarsU16)(fleet->orders[0].flags |
                KILL_WP_PLANET);
            fleet->orders[0].target_id = fleet->id_planet;
        }
    }
    return 1;
}

static void kill_wp_empty_message(const StarsKillUsedWaypointsInput *input,
    StarsKillUsedWaypointFleet *fleet)
{
    StarsU16 kind;
    StarsKillUsedWaypointPlanet *planet;

    kind = (StarsU16)(fleet->orders[0].flags & KILL_WP_KIND_MASK);
    if (kind == 1U || kind == 2U || kind == 3U || kind == 5U ||
        kind == 6U || kind == KILL_WP_PLANET_ORDER)
        return;
    if (kind == KILL_WP_PLANET_KIND && fleet->id_planet != -1 &&
        input->ops->find_planet) {
        planet = input->ops->find_planet(input->ops->context,
            fleet->id_planet);
        if (planet && planet->owner == fleet->owner &&
            (planet->status_flags & 0x03ffU) != 0U)
            return;
    }
    kill_wp_message(input->ops, fleet, KILL_WP_MESSAGE_EMPTY);
}

/* Genuine source reconstruction of KillUsedWaypoints, NE 11:189A-1D31. */
void stars_kill_used_waypoints(StarsKillUsedWaypointsInput *input)
{
    StarsU16 index;
    StarsI16 recycle;
    StarsKillUsedWaypointFleet *fleet;

    if (!input || !input->fleets || !input->ops)
        return;
    for (index = 0; index < input->fleet_count; ++index) {
        fleet = &input->fleets[index];
        if (!fleet->orders)
            return;
        if (fleet->cord <= 1)
            continue;
        if ((fleet->status_flags & KILL_WP_STATUS_DELETED) != 0U) {
            kill_wp_delete(input, fleet, 0);
            kill_wp_message(input->ops, fleet, KILL_WP_MESSAGE_USED);
            continue;
        }
        if (!kill_wp_retarget_orders(input, fleet)) continue;
        recycle = (StarsI16)(((fleet->orders[1].flags & KILL_WP_KIND_MASK) ==
            KILL_WP_PLANET_ORDER &&
            kill_wp_target_kind(fleet->orders[1].flags) ==
            KILL_WP_TARGET_FLEET) ? 0 :
            ((fleet->status_flags & KILL_WP_REUSE_FLAG) != 0U));
        kill_wp_delete(input, fleet, recycle);
        if (fleet->cord == 1)
            kill_wp_empty_message(input, fleet);
    }
}

typedef struct StarsKillUsedWaypointsPackedContext {
    const StarsKillUsedWaypointOps *original_ops;
    StarsKillUsedWaypointFleet *fleets;
    StarsU16 fleet_count;
} StarsKillUsedWaypointsPackedContext;

static StarsKillUsedWaypointFleet *kill_wp_packed_find_fleet(
    void *context, StarsI16 id)
{
    StarsKillUsedWaypointsPackedContext *packed;
    StarsU16 index;

    packed = (StarsKillUsedWaypointsPackedContext *)context;
    for (index = 0; index < packed->fleet_count; ++index) {
        if (packed->fleets[index].id == id)
            return &packed->fleets[index];
    }
    return (StarsKillUsedWaypointFleet *)0;
}

static StarsKillUsedWaypointPlanet *kill_wp_packed_find_planet(
    void *context, StarsI16 id)
{
    StarsKillUsedWaypointsPackedContext *packed;

    packed = (StarsKillUsedWaypointsPackedContext *)context;
    if (packed->original_ops->find_planet)
        return packed->original_ops->find_planet(
            packed->original_ops->context, id);
    return (StarsKillUsedWaypointPlanet *)0;
}

static void kill_wp_packed_delete(void *context,
    StarsKillUsedWaypointFleet *fleet, StarsI16 index, StarsI16 recycle)
{
    StarsKillUsedWaypointsPackedContext *packed;

    packed = (StarsKillUsedWaypointsPackedContext *)context;
    if (packed->original_ops->delete_waypoint)
        packed->original_ops->delete_waypoint(
            packed->original_ops->context, fleet, index, recycle);
    else if (fleet->orders && fleet->cord > 1) {
        (void)memmove(&fleet->orders[1], &fleet->orders[2],
            (size_t)(fleet->cord - 2) * sizeof(fleet->orders[0]));
        --fleet->cord;
    }
}

static void kill_wp_packed_message(void *context, StarsI16 owner,
    StarsI16 message, StarsI16 fleet_id)
{
    StarsKillUsedWaypointsPackedContext *packed;

    packed = (StarsKillUsedWaypointsPackedContext *)context;
    if (packed->original_ops->send_message)
        packed->original_ops->send_message(
            packed->original_ops->context, owner, message, fleet_id);
}

static void kill_wp_packed_message_ex(void *context, StarsI16 owner,
    StarsI16 message, StarsU16 status_word, StarsI16 fleet_id)
{
    StarsKillUsedWaypointsPackedContext *packed;

    packed = (StarsKillUsedWaypointsPackedContext *)context;
    if (packed->original_ops->send_message_ex)
        packed->original_ops->send_message_ex(
            packed->original_ops->context, owner, message,
            status_word, fleet_id);
    else if (packed->original_ops->send_message)
        packed->original_ops->send_message(
            packed->original_ops->context, owner, message, fleet_id);
}

static StarsU16 kill_wp_packed_get_word(const StarsU8 *raw,
    StarsU16 offset)
{
    return (StarsU16)(raw[offset] |
        ((StarsU16)raw[offset + 1U] << 8));
}

static void kill_wp_packed_put_word(StarsU8 *raw, StarsU16 offset,
    StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static int kill_wp_packed_ref(const StarsU8 *table, StarsU16 slot,
    StarsU16 *record_index)
{
    StarsU16 offset;
    StarsU16 segment;

    offset = kill_wp_packed_get_word(table,
        (StarsU16)(slot * STARS_KILL_WP_PACKED_FLEET_REF_SIZE));
    segment = kill_wp_packed_get_word(table,
        (StarsU16)(slot * STARS_KILL_WP_PACKED_FLEET_REF_SIZE + 2U));
    if (offset == 0U && segment == 0U)
        return 0;
    if (offset == 0U)
        return -1;
    *record_index = (StarsU16)(offset - 1U);
    return 1;
}

static int kill_wp_packed_load_fleet(
    StarsKillUsedWaypointsPackedInput *input, StarsU16 record_index,
    StarsU16 order_index, StarsKillUsedWaypointFleet *fleet,
    StarsKillUsedWaypointOrder *orders)
{
    const StarsU8 *raw_fleet;
    const StarsU8 *raw_orders;
    StarsU16 index;
    StarsU16 base;

    if (record_index >= input->fleet_record_count ||
        order_index >= input->plord_record_count)
        return 0;
    raw_fleet = input->fleet_records +
        (size_t)record_index * STARS_KILL_WP_PACKED_FLEET_SIZE;
    raw_orders = input->plord_records +
        (size_t)order_index * STARS_KILL_WP_PACKED_PLORD_SIZE;
    fleet->id = (StarsI16)kill_wp_packed_get_word(raw_fleet, 0U);
    fleet->owner = (StarsI16)kill_wp_packed_get_word(raw_fleet, 2U);
    fleet->status_flags = (StarsI16)kill_wp_packed_get_word(raw_fleet, 4U);
    fleet->id_planet = (StarsI16)kill_wp_packed_get_word(raw_fleet, 6U);
    fleet->x = (StarsI16)kill_wp_packed_get_word(raw_fleet, 8U);
    fleet->y = (StarsI16)kill_wp_packed_get_word(raw_fleet, 10U);
    fleet->cord = (StarsI16)kill_wp_packed_get_word(raw_fleet, 98U);
    fleet->orders = orders;
    for (index = 0U; index < STARS_KILL_WP_PACKED_ORDER_COUNT; ++index) {
        base = (StarsU16)(STARS_KILL_WP_PACKED_PLORD_HEADER_SIZE +
            index * STARS_KILL_WP_PACKED_ORDER_SIZE);
        orders[index].x = (StarsI16)kill_wp_packed_get_word(raw_orders, base);
        orders[index].y = (StarsI16)kill_wp_packed_get_word(
            raw_orders, (StarsU16)(base + 2U));
        orders[index].target_id = (StarsI16)kill_wp_packed_get_word(
            raw_orders, (StarsU16)(base + 4U));
        orders[index].flags = kill_wp_packed_get_word(
            raw_orders, (StarsU16)(base + 6U));
        memcpy(orders[index].payload, raw_orders + base + 8U, 10U);
    }
    return 1;
}

static void kill_wp_packed_store_fleet(
    const StarsKillUsedWaypointsPackedInput *input, StarsU16 record_index,
    StarsU16 order_index, const StarsKillUsedWaypointFleet *fleet)
{
    StarsU8 *raw_fleet;
    StarsU8 *raw_orders;
    StarsU16 index;
    StarsU16 base;

    raw_fleet = input->fleet_records +
        (size_t)record_index * STARS_KILL_WP_PACKED_FLEET_SIZE;
    raw_orders = input->plord_records +
        (size_t)order_index * STARS_KILL_WP_PACKED_PLORD_SIZE;
    kill_wp_packed_put_word(raw_fleet, 0U, (StarsU16)fleet->id);
    kill_wp_packed_put_word(raw_fleet, 2U, (StarsU16)fleet->owner);
    kill_wp_packed_put_word(raw_fleet, 4U, (StarsU16)fleet->status_flags);
    kill_wp_packed_put_word(raw_fleet, 6U, (StarsU16)fleet->id_planet);
    kill_wp_packed_put_word(raw_fleet, 8U, (StarsU16)fleet->x);
    kill_wp_packed_put_word(raw_fleet, 10U, (StarsU16)fleet->y);
    kill_wp_packed_put_word(raw_fleet, 98U, (StarsU16)fleet->cord);
    for (index = 0U; index < STARS_KILL_WP_PACKED_ORDER_COUNT; ++index) {
        base = (StarsU16)(STARS_KILL_WP_PACKED_PLORD_HEADER_SIZE +
            index * STARS_KILL_WP_PACKED_ORDER_SIZE);
        kill_wp_packed_put_word(raw_orders, base,
            (StarsU16)fleet->orders[index].x);
        kill_wp_packed_put_word(raw_orders, (StarsU16)(base + 2U),
            (StarsU16)fleet->orders[index].y);
        kill_wp_packed_put_word(raw_orders, (StarsU16)(base + 4U),
            (StarsU16)fleet->orders[index].target_id);
        kill_wp_packed_put_word(raw_orders, (StarsU16)(base + 6U),
            fleet->orders[index].flags);
        memcpy(raw_orders + base + 8U, fleet->orders[index].payload, 10U);
    }
}

void stars_kill_used_waypoints_packed(
    StarsKillUsedWaypointsPackedInput *input)
{
    StarsKillUsedWaypointsPackedContext context;
    StarsKillUsedWaypointOps packed_ops;
    StarsKillUsedWaypointsInput typed_input;
    StarsU16 slot;
    StarsU16 record_index;
    StarsU16 order_index;
    StarsU16 count;

    if (!input) return;
    input->enumerated_count = 0U;
    input->malformed = 0U;
    if (!input->fleet_table || !input->fleet_records ||
        !input->plord_records || !input->fleet_views ||
        !input->order_views || !input->ops || input->workspace_count == 0U)
        return;
    memset(&context, 0, sizeof context);
    context.original_ops = input->ops;
    context.fleets = input->fleet_views;
    memset(&packed_ops, 0, sizeof packed_ops);
    packed_ops.find_fleet = kill_wp_packed_find_fleet;
    packed_ops.find_planet = kill_wp_packed_find_planet;
    packed_ops.delete_waypoint = kill_wp_packed_delete;
    packed_ops.send_message = kill_wp_packed_message;
    packed_ops.send_message_ex = kill_wp_packed_message_ex;
    packed_ops.context = &context;
    count = 0U;
    for (slot = 0U; slot < input->fleet_table_count; ++slot) {
        if (kill_wp_packed_ref(input->fleet_table, slot,
                &record_index) == 0)
            break;
        if (kill_wp_packed_ref(input->fleet_table, slot,
                &record_index) < 0 || count >= input->workspace_count) {
            input->malformed = 1U;
            return;
        }
        order_index = kill_wp_packed_get_word(input->fleet_records +
            (size_t)record_index * STARS_KILL_WP_PACKED_FLEET_SIZE, 100U);
        if (order_index == 0U) {
            input->malformed = 1U;
            return;
        }
        --order_index;
        if (kill_wp_packed_load_fleet(input, record_index, order_index,
                &input->fleet_views[count],
                &input->order_views[(size_t)count *
                    STARS_KILL_WP_PACKED_ORDER_COUNT]) == 0) {
            input->malformed = 1U;
            return;
        }
        ++count;
    }
    context.fleet_count = count;
    typed_input.fleets = input->fleet_views;
    typed_input.fleet_count = count;
    typed_input.ops = &packed_ops;
    input->enumerated_count = count;
    stars_kill_used_waypoints(&typed_input);
    for (slot = 0U; slot < count; ++slot) {
        record_index = 0U;
        (void)kill_wp_packed_ref(input->fleet_table, slot, &record_index);
        order_index = kill_wp_packed_get_word(input->fleet_records +
            (size_t)record_index * STARS_KILL_WP_PACKED_FLEET_SIZE, 100U);
        --order_index;
        kill_wp_packed_store_fleet(input, record_index, order_index,
            &input->fleet_views[slot]);
    }
}
