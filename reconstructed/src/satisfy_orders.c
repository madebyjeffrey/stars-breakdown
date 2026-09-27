#include "core.h"
#include <string.h>

#define SATISFY_FLEET_INACTIVE 0x0400U
#define SATISFY_TURN_CLEAR_MASK 0x0060U
#define SATISFY_TURN_ALREADY_DONE 0x0080U
#define SATISFY_PACKED_FLEET_ID 0U
#define SATISFY_PACKED_FLEET_OWNER 2U
#define SATISFY_PACKED_FLEET_FLAGS 4U
#define SATISFY_PACKED_FLEET_TURN_FLAGS 118U
#define SATISFY_PACKED_FLEET_PLANET 6U
#define SATISFY_PACKED_FLEET_SHIPS 12U
#define SATISFY_PACKED_FLEET_CARGO 76U
#define SATISFY_PACKED_ORDER_TARGET 4U
#define SATISFY_PACKED_ORDER_KIND 6U
#define SATISFY_PACKED_FLEET_BYTES 124U
#define SATISFY_PACKED_ORDER_BYTES 18U
#define SATISFY_TECH_POINT_NEGATIVE 0x013c
#define SATISFY_TECH_POINT_POSITIVE 0x013d
#define SATISFY_TECH_PLANET_NEGATIVE 0x013e
#define SATISFY_TECH_PLANET_POSITIVE 0x013f

/* Genuine C reconstruction of the resource-transfer core of SatisfyOrders,
 * NE 23:6798-9eb8.  The packed original has several object-specific branches;
 * this adapter gives those branches one typed transfer path while retaining
 * the turn engine's ownership of fleet, planet, and thing records. */

static StarsI32 satisfy_min(StarsI32 left, StarsI32 right)
{
    return left < right ? left : right;
}

static StarsI32 satisfy_available(const StarsSatisfyOrdersOps *ops,
    const StarsSatisfyFleet *fleet, StarsI16 resource)
{
    StarsI32 available;

    if (ops->available)
        available = ops->available(ops->context, fleet, resource);
    else
        available = fleet->cargo[resource];
    if (available < 0)
        available = 0;
    return available;
}

static StarsI32 satisfy_capacity(const StarsSatisfyFleet *fleet,
    StarsI16 resource, StarsI32 amount)
{
    if (fleet->capacity[resource] <= 0)
        return amount;
    if (amount > fleet->capacity[resource])
        return fleet->capacity[resource];
    return amount;
}

static StarsI32 satisfy_stat_percent_delta(StarsI32 stat,
    StarsI32 requested, StarsI32 current)
{
    StarsI32 derived;

    if (stat < 0)
        stat = 0;
    if (stat > 2000000L)
        stat = 2000000L;
    /* The original avoids a 32-bit intermediate overflow by switching from
     * (stat * requested) / 100 to (stat / 100) * requested. */
    if (stat < 65536L)
        derived = (StarsI32)(((StarsU32)stat * (StarsU32)requested) /
            100U);
    else
        derived = (stat / 100L) * requested;
    derived -= current;
    return derived > 0 ? derived : 0;
}

static StarsI16 satisfy_resource_allowed(const StarsSatisfyOrder *order,
    StarsI16 resource)
{
    /* The packed order loop excludes the fuel channel for planet and thing
     * destinations; only fleet and point destinations carry that channel. */
    if (resource == (StarsI16)STARS_SATISFY_RESOURCE_FUEL &&
            order->target_kind != (StarsI16)STARS_SATISFY_TARGET_FLEET &&
            order->target_kind != (StarsI16)STARS_SATISFY_TARGET_POINT)
        return 0;
    if (order->target_kind == (StarsI16)STARS_SATISFY_TARGET_THING &&
            resource >= (StarsI16)STARS_SATISFY_RESOURCE_COLONISTS)
        return 0;
    return 1;
}

static void satisfy_message(const StarsSatisfyOrdersOps *ops,
    const StarsSatisfyFleet *fleet, StarsI16 code, StarsI16 target,
    StarsI16 resource, StarsI32 amount)
{
    if (ops->send_message)
        ops->send_message(ops->context, fleet->owner, code, target,
            resource, amount);
    if (ops->message_layout)
        ops->message_layout(ops->context, fleet->owner, code, target,
            resource, amount, 0);
}

void stars_satisfy_raw_fuel_message(const StarsSatisfyFuelFrame *frame,
    StarsSatisfyRawMessage raw_message, void *context)
{
    StarsI16 payload[8];
    StarsI16 code;
    StarsI32 amount;
    StarsI16 i;

    if (!frame || !raw_message)
        return;
    for (i = 0; i < 8; ++i)
        payload[i] = 0;
    code = 0;
    amount = 0;
    if (frame->variant == STARS_SATISFY_FUEL_MESSAGE_FAILED_LOAD) {
        code = STARS_SATISFY_MESSAGE_FUEL_FAILED_LOAD;
        payload[0] = (StarsI16)((StarsU16)frame->fleet_id | 0x8000U);
        payload[1] = frame->fleet_id;
        payload[2] = frame->target_x;
        payload[3] = frame->target_word;
    } else if (frame->variant == STARS_SATISFY_FUEL_MESSAGE_CAPACITY) {
        code = STARS_SATISFY_MESSAGE_FUEL_CAPACITY;
        payload[0] = (StarsI16)((StarsU16)frame->fleet_id | 0x8000U);
        payload[1] = frame->fleet_id;
        payload[2] = (StarsI16)frame->fuel_stat;
        payload[3] = (StarsI16)frame->required_fuel;
        payload[4] = (StarsI16)frame->required_fuel;
    } else if (frame->variant == STARS_SATISFY_FUEL_MESSAGE_AVAILABLE) {
        code = STARS_SATISFY_MESSAGE_FUEL_AVAILABLE;
        payload[0] = (StarsI16)((StarsU16)frame->fleet_id | 0x8000U);
        payload[1] = frame->target_x;
        payload[2] = frame->target_word;
        payload[3] = frame->fleet_id;
        payload[4] = (StarsI16)(frame->required_fuel -
            frame->fleet_fuel);
    } else if (frame->variant == STARS_SATISFY_FUEL_MESSAGE_NONE &&
            frame->optimal_delta != 0) {
        code = frame->optimal_delta < 0 ?
            STARS_SATISFY_MESSAGE_TRANSFER :
            STARS_SATISFY_MESSAGE_FLEET_TRANSFER;
        amount = frame->optimal_delta < 0 ?
            (StarsI32)(0 - frame->optimal_delta) : frame->optimal_delta;
        payload[0] = (StarsI16)((StarsU16)frame->fleet_id | 0x8000U);
        payload[1] = frame->fleet_id;
        payload[2] = (StarsI16)amount;
        payload[3] = frame->target_word;
        payload[4] = STARS_SATISFY_RESOURCE_FUEL;
        payload[5] = frame->target_x;
        payload[6] = frame->target_id;
    }
    if (code != 0)
        raw_message(context, frame->recipient, code, payload);
}

void stars_satisfy_raw_failure_message(const StarsSatisfyFailureFrame *frame,
    StarsSatisfyRawMessage raw_message, StarsSatisfyRawMessage2 raw_message2,
    void *context)
{
    StarsI16 payload[8];
    StarsI16 code;
    StarsI16 payload1;
    StarsI16 payload2;
    StarsI16 payload3;
    StarsI16 i;

    if (!frame)
        return;
    code = 0;
    payload1 = 0;
    payload2 = 0;
    payload3 = 0;
    for (i = 0; i < 8; ++i)
        payload[i] = 0;
    payload1 = (StarsI16)((StarsU16)frame->fleet_id | 0x8000U);
    if (frame->variant == STARS_SATISFY_FAILURE_SET_AMOUNT ||
            frame->variant == STARS_SATISFY_FAILURE_SET_COLONISTS) {
        code = frame->variant == STARS_SATISFY_FAILURE_SET_COLONISTS ?
            STARS_SATISFY_MESSAGE_SET_COLONISTS :
            STARS_SATISFY_MESSAGE_SET_AMOUNT;
        payload[0] = payload1;
        payload[1] = frame->fleet_id;
        payload[2] = frame->resource;
        payload[3] = frame->requested;
        payload[5] = frame->target_x;
        payload[6] = frame->target_word;
        if (raw_message)
            raw_message(context, frame->recipient, code, payload);
        return;
    }
    if (frame->variant == STARS_SATISFY_FAILURE_COLONISTS_DEEP_SPACE) {
        code = STARS_SATISFY_MESSAGE_COLONISTS_DEEP_SPACE;
        payload1 = (StarsI16)((StarsU16)frame->fleet_id | 0x8000U);
        payload2 = frame->fleet_id;
        payload3 = 0;
    } else if (frame->variant == STARS_SATISFY_FAILURE_TRANSFER_FUTILE) {
        code = STARS_SATISFY_MESSAGE_TRANSFER_FUTILE;
        payload1 = (StarsI16)((StarsU16)frame->fleet_id | 0x8000U);
        payload2 = frame->fleet_id;
        payload3 = frame->thing_kind;
    }
    if (code != 0 && raw_message2)
        raw_message2(context, frame->recipient, code, payload1, payload2,
            payload3);
}

static void satisfy_raw_transfer_message(const StarsSatisfyOrdersOps *ops,
    const StarsSatisfyFleet *fleet, const StarsSatisfyOrder *order,
    StarsI16 result)
{
    StarsI16 source_owner;
    StarsI16 target_owner;
    StarsI16 target_id;
    StarsI16 source_word;
    StarsI16 target_word;

    if (!ops->raw_message2)
        return;
    source_owner = fleet->owner;
    target_owner = order->target_owner;
    target_id = order->message_target_id;
    source_word = (StarsI16)(fleet->message_word | 0x8000U);
    target_word = (StarsI16)order->message_target_word;
    if (result == STARS_SATISFY_PLAYER_TRANSFER_CREATED) {
        ops->raw_message2(ops->context, source_owner,
            STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_CREATED,
            (StarsI16)((StarsU16)target_id | 0x8000U),
            (StarsI16)fleet->message_word,
            (StarsI16)((StarsU16)target_owner | 0x30U));
        ops->raw_message2(ops->context, target_owner,
            STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_RECEIVED,
            (StarsI16)((StarsU16)target_id | 0x8000U),
            (StarsI16)((StarsU16)source_owner | 0x30U), target_word);
    } else if (result == STARS_SATISFY_PLAYER_TRANSFER_NO_DESIGN) {
        ops->raw_message2(ops->context, source_owner,
            STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_DESIGN,
            source_word, fleet->id,
            (StarsI16)((StarsU16)target_owner | 0x30U));
        /* The original uses a distinct recipient message (0x14b), even
         * though the older typed adapter exposed this as a no-ship pair. */
        ops->raw_message2(ops->context, target_owner, 0x014b,
            (StarsI16)-1,
            (StarsI16)((StarsU16)source_owner | 0x30U), 0);
    } else if (result == STARS_SATISFY_PLAYER_TRANSFER_NO_SHIP) {
        ops->raw_message2(ops->context, target_owner,
            STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_SHIP,
            source_word, fleet->id, 0);
    } else if (result == STARS_SATISFY_PLAYER_TRANSFER_BLOCKED) {
        ops->raw_message2(ops->context, target_owner,
            STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_BLOCKED,
            source_word,
            (StarsI16)((StarsU16)target_owner | 0x30U), 0);
    } else {
        ops->raw_message2(ops->context, target_owner,
            STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_INVALID,
            source_word, fleet->id, 0);
    }
}

static void satisfy_raw_auto_route_message(
    const StarsSatisfyOrdersOps *ops, const StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 code)
{
    StarsI16 payload[8];
    StarsU16 starbase;

    if (!ops->raw_message)
        return;
    starbase = (StarsU16)order->target_starbase_flags & 0x03ffU;
    payload[0] = (StarsI16)((StarsU16)fleet->id | 0x8000U);
    payload[1] = fleet->id;
    payload[2] = fleet->planet_id;
    payload[3] = (StarsI16)(starbase - 1U);
    payload[4] = 0;
    payload[5] = 0;
    payload[6] = 0;
    payload[7] = 0;
    ops->raw_message(ops->context, fleet->owner, code, payload);
}

static void satisfy_raw_mine_message(const StarsSatisfyOrdersOps *ops,
    const StarsSatisfyFleet *fleet, const StarsSatisfyOrder *order,
    StarsI16 code)
{
    StarsI16 payload1;
    StarsI16 payload2;
    StarsI16 payload3;

    if (!ops->raw_message2)
        return;
    payload1 = (StarsI16)((StarsU16)fleet->id | 0x8000U);
    payload2 = fleet->id;
    payload3 = code == STARS_SATISFY_MESSAGE_MINE_NO_PLANET ?
        0 : order->target_id;
    ops->raw_message2(ops->context, fleet->owner, code, payload1,
        payload2, payload3);
}

static void satisfy_raw_merge_message(const StarsSatisfyOrdersOps *ops,
    const StarsSatisfyFleet *fleet, const StarsSatisfyOrder *order,
    StarsI16 code)
{
    StarsI16 payload1;
    StarsI16 payload2;
    StarsI16 payload3;

    if (!ops->raw_message2)
        return;
    if (code == STARS_SATISFY_MESSAGE_MERGE_OK) {
        payload1 = (StarsI16)((StarsU16)order->target_id | 0x8000U);
        payload2 = (StarsI16)fleet->message_word;
        payload3 = order->target_id;
    } else {
        payload1 = (StarsI16)((StarsU16)fleet->id | 0x8000U);
        payload2 = fleet->id;
        payload3 = 0;
    }
    ops->raw_message2(ops->context, fleet->owner, code, payload1,
        payload2, payload3);
}

static StarsI16 satisfy_player_transfer_message(
    const StarsSatisfyOrdersOps *ops, StarsSatisfyFleet *fleet,
    const StarsSatisfyOrder *order, StarsI16 result)
{
    StarsI16 source_code;

    source_code = STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_INVALID;
    if (result == STARS_SATISFY_PLAYER_TRANSFER_NO_SHIP)
        source_code = STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_SHIP;
    else if (result == STARS_SATISFY_PLAYER_TRANSFER_NO_DESIGN)
        source_code = STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_DESIGN;
    else if (result == STARS_SATISFY_PLAYER_TRANSFER_CREATED)
        source_code = STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_CREATED;
    else if (result == STARS_SATISFY_PLAYER_TRANSFER_BLOCKED)
        source_code = STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_BLOCKED;

    satisfy_raw_transfer_message(ops, fleet, order, result);

    if (ops->transfer_player_message) {
        ops->transfer_player_message(ops->context, fleet->owner,
            source_code, fleet, order);
        if (ops->message_layout)
            ops->message_layout(ops->context, fleet->owner, source_code,
                order->target_id, order->kind, result,
                order->target_owner);
        if (result == STARS_SATISFY_PLAYER_TRANSFER_NO_DESIGN) {
            ops->transfer_player_message(ops->context, order->target_owner,
                STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_SHIP, fleet, order);
            if (ops->message_layout)
                ops->message_layout(ops->context, order->target_owner,
                    STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_NO_SHIP,
                    order->target_id, order->kind, result, fleet->owner);
        } else if (result == STARS_SATISFY_PLAYER_TRANSFER_CREATED) {
            ops->transfer_player_message(ops->context, order->target_owner,
                STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_RECEIVED, fleet, order);
            if (ops->message_layout)
                ops->message_layout(ops->context, order->target_owner,
                    STARS_SATISFY_MESSAGE_TRANSFER_PLAYER_RECEIVED,
                    order->target_id, order->kind, result, fleet->owner);
        }
        return 0;
    }
    return source_code;
}

static StarsI16 satisfy_transfer_message_code(
    const StarsSatisfyFleet *fleet, const StarsSatisfyOrder *order,
    StarsI16 resource)
{
    if (order->target_kind == STARS_SATISFY_TARGET_FLEET) {
        if (resource == STARS_SATISFY_RESOURCE_COLONISTS &&
                order->target_owner != fleet->owner)
            return STARS_SATISFY_MESSAGE_FOREIGN_FLEET_COLONISTS;
        return resource == STARS_SATISFY_RESOURCE_COLONISTS ?
            STARS_SATISFY_MESSAGE_FLEET_TRANSFER_COLONISTS :
            STARS_SATISFY_MESSAGE_FLEET_TRANSFER;
    }
    return resource == STARS_SATISFY_RESOURCE_COLONISTS ?
        STARS_SATISFY_MESSAGE_TRANSFER_COLONISTS :
        STARS_SATISFY_MESSAGE_TRANSFER;
}

static StarsI16 satisfy_shortage_message_code(
    const StarsSatisfyOrder *order)
{
    if (order->target_kind == STARS_SATISFY_TARGET_FLEET)
        return STARS_SATISFY_MESSAGE_FLEET_SHORTAGE;
    if (order->target_kind == STARS_SATISFY_TARGET_POINT)
        return STARS_SATISFY_MESSAGE_POINT_SHORTAGE;
    return STARS_SATISFY_MESSAGE_SHORTAGE;
}

static void satisfy_raw_shortage_message(const StarsSatisfyOrdersOps *ops,
    const StarsSatisfyFleet *fleet, const StarsSatisfyOrder *order,
    StarsI16 resource)
{
    StarsI16 code;

    if (!ops->raw_message2)
        return;
    if (order->target_kind == STARS_SATISFY_TARGET_PLANET)
        code = STARS_SATISFY_MESSAGE_SHORTAGE;
    else if (order->target_kind == STARS_SATISFY_TARGET_FLEET)
        code = STARS_SATISFY_MESSAGE_FLEET_SHORTAGE;
    else if (order->target_kind == STARS_SATISFY_TARGET_POINT)
        code = STARS_SATISFY_MESSAGE_POINT_SHORTAGE;
    else
        return;
    ops->raw_message2(ops->context, fleet->owner, code,
        (StarsI16)((StarsU16)fleet->id | 0x8000U), fleet->id, resource);
}

static void satisfy_raw_colonist_message(const StarsSatisfyOrdersOps *ops,
    const StarsSatisfyFleet *fleet, const StarsSatisfyOrder *order,
    StarsI16 code)
{
    if (!ops->raw_message2 ||
            (code != STARS_SATISFY_MESSAGE_COLONISTS_NO_UNOWNED_DROP &&
             code != STARS_SATISFY_MESSAGE_COLONISTS_RACE_BLOCKED &&
             code != STARS_SATISFY_MESSAGE_COLONISTS_PLANET_BLOCKED))
        return;
    ops->raw_message2(ops->context, fleet->owner, code,
        (StarsI16)((StarsU16)fleet->id | 0x8000U), fleet->id,
        order->target_id);
}

static void satisfy_raw_foreign_fleet_colonist(
    const StarsSatisfyOrdersOps *ops, const StarsSatisfyFleet *fleet)
{
    if (!ops->raw_message2)
        return;
    /* The original rejection carries the source fleet's current planet,
     * held in the turn-local planet view rather than the target order id. */
    ops->raw_message2(ops->context, fleet->owner,
        STARS_SATISFY_MESSAGE_FOREIGN_FLEET_COLONISTS,
        (StarsI16)((StarsU16)fleet->id | 0x8000U), fleet->id,
        fleet->planet_id);
}

static void satisfy_raw_resource_message(const StarsSatisfyOrdersOps *ops,
    const StarsSatisfyFleet *fleet, const StarsSatisfyOrder *order,
    StarsI16 code, StarsI16 resource, StarsI32 amount)
{
    StarsI16 payload[8];

    /* Technology transfers have a separate FSendPlrMsg frame below.  The
     * ordinary cargo message is still emitted through the typed adapter, but
     * must not be duplicated on the raw eight-word transport boundary. */
    if (!ops->raw_message || order->technology_flag != 0 ||
            (code != STARS_SATISFY_MESSAGE_TRANSFER &&
             code != STARS_SATISFY_MESSAGE_TRANSFER_COLONISTS &&
             code != STARS_SATISFY_MESSAGE_FLEET_TRANSFER &&
             code != STARS_SATISFY_MESSAGE_FLEET_TRANSFER_COLONISTS))
        return;
    payload[0] = (StarsI16)((StarsU16)fleet->id | 0x8000U);
    payload[1] = fleet->id;
    payload[2] = (StarsI16)amount;
    payload[3] = (StarsI16)order->message_target_word;
    payload[4] = resource;
    payload[5] = order->message_target_x;
    payload[6] = order->target_id;
    payload[7] = 0;
    ops->raw_message(ops->context, fleet->owner, code, payload);
}

static StarsI16 satisfy_colonist_drop_allowed(
    const StarsSatisfyFleet *fleet, const StarsSatisfyOrder *order,
    StarsI16 *rejection_code)
{
    *rejection_code = 0;
    if (order->target_kind != STARS_SATISFY_TARGET_PLANET)
        return 0;
    if (order->target_owner == fleet->owner)
        return 1;
    if (order->target_owner == -1 &&
            (order->target_planet_flags & 0x2000U) == 0U) {
        *rejection_code = STARS_SATISFY_MESSAGE_COLONISTS_NO_UNOWNED_DROP;
        return 0;
    }
    if (order->colonist_race_stat == 8) {
        *rejection_code = STARS_SATISFY_MESSAGE_COLONISTS_RACE_BLOCKED;
        return 0;
    }
    if ((order->target_planet_flags & 0x0200U) != 0U) {
        *rejection_code = STARS_SATISFY_MESSAGE_COLONISTS_PLANET_BLOCKED;
        return 0;
    }
    return 1;
}

static StarsU16 satisfy_packed_get_word(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1U] << 8));
}

static void satisfy_packed_put_word(StarsU8 *record, StarsU16 offset,
    StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU32 satisfy_packed_get_long(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU32)satisfy_packed_get_word(record, offset) |
        ((StarsU32)satisfy_packed_get_word(record,
            (StarsU16)(offset + 2U)) << 16);
}

static void satisfy_packed_put_long(StarsU8 *record, StarsU16 offset,
    StarsU32 value)
{
    satisfy_packed_put_word(record, offset, (StarsU16)value);
    satisfy_packed_put_word(record, (StarsU16)(offset + 2U),
        (StarsU16)(value >> 16));
}

static StarsU16 satisfy_scrap_fleet_word(const StarsU8 *fleet,
    const StarsSatisfyScrapPackedState *state)
{
    StarsU16 largest;
    StarsU16 count;
    StarsU16 primary;
    StarsU16 different;
    StarsI16 design;

    largest = 0;
    primary = 16U;
    different = 0;
    for (design = 0; design < 16; ++design) {
        count = satisfy_packed_get_word(fleet,
            (StarsU16)(SATISFY_PACKED_FLEET_SHIPS + design * 2));
        if (count == 0U)
            continue;
        ++different;
        if (count != largest && largest <= count) {
            StarsU16 hull;
            hull = satisfy_packed_get_word(state->design_records[design], 0U);
            primary = (StarsU16)design;
            largest = count;
            if (hull == 0x19U || hull == 0x1aU)
                --largest;
        }
    }
    count = (StarsU16)((primary << 9) |
        (satisfy_packed_get_word(fleet, SATISFY_PACKED_FLEET_ID) & 0x01ffU));
    if (different > 1U)
        count = (StarsU16)(count | 0x2000U);
    return count;
}

static const StarsU8 *satisfy_scrap_design_for_cost(
    const StarsSatisfyPackedInput *input, StarsI16 design,
    StarsU8 repriced[147])
{
    const StarsSatisfyScrapPackedState *state;
    const StarsU8 *original;
    state = input->scrap_state;
    original = state->design_records[design];
    if (!state->fleet_race_reprice)
        return original;
    memcpy(repriced, original, 147U);
    if (!input->ops->scrap_reprice(input->ops->context,
            original, repriced))
        return 0;
    return repriced;
}

/* Task-5 branch at original 10b0:8207-8e65. The caller supplies the live
 * SHDEF bank, optional PLANET record, and external salvage/message services. */
static StarsI16 satisfy_scrap_packed(StarsSatisfyPackedInput *input)
{
    StarsSatisfyScrapPackedState *state;
    StarsU8 *fleet;
    StarsU8 *planet;
    StarsU32 ore[3];
    StarsU32 resource_cost;
    StarsU32 count;
    StarsU32 cost;
    StarsU32 cargo;
    StarsU32 mineral_total;
    StarsU32 adjusted_resources;
    StarsU16 flags;
    StarsU16 planet_id;
    StarsI16 planet_owner;
    StarsI16 design;
    StarsI16 mineral;
    StarsI16 code;
    StarsI16 fleet_owner;
    StarsU16 fleet_word;
    StarsU16 message_object;
    StarsI16 technology_result;
    StarsI16 payload[8];
    StarsU8 repriced[147];

    state = input->scrap_state;
    fleet = input->fleet_record;
    if (!state || state->design_size < 147U)
        return 0;
    planet = state->planet_record;
    planet_id = satisfy_packed_get_word(fleet,
        SATISFY_PACKED_FLEET_PLANET);
    if (planet_id == 0xffffU) {
        if (!input->ops->scrap_salvage || !input->ops->raw_message2)
            return 0;
        planet_owner = -1;
        flags = 0;
    } else if (!planet || state->planet_size < 60U ||
            planet_id != satisfy_packed_get_word(planet, 0U)) {
        return 0;
    } else {
        planet_owner = (StarsI16)satisfy_packed_get_word(planet, 2U);
        flags = satisfy_packed_get_word(planet, 4U);
        if (!input->ops->raw_message)
            return 0;
        if (state->planet_race_scrap_bonus &&
                (!state->planet_resource_extra ||
                 !input->ops->scrap_planet_resources))
            return 0;
    }
    if (state->fleet_race_reprice && (!input->ops->scrap_reprice ||
            !state->game_flags))
        return 0;
    /* Every occupied design must be present before any planet mutation. */
    for (design = 0; design < 16; ++design) {
        if (satisfy_packed_get_word(fleet,
                (StarsU16)(SATISFY_PACKED_FLEET_SHIPS + design * 2)) != 0U &&
                !state->design_records[design])
            return 0;
    }
    if (input->ops->scrap_mark_techs) {
        StarsI16 fleet_owner_for_tech;
        fleet_owner_for_tech = (StarsI16)satisfy_packed_get_word(fleet,
            SATISFY_PACKED_FLEET_OWNER);
        for (design = 0; design < 16; ++design) {
            if (satisfy_packed_get_word(fleet,
                    (StarsU16)(SATISFY_PACKED_FLEET_SHIPS + design * 2))
                    != 0U)
                input->ops->scrap_mark_techs(input->ops->context,
                    state->design_records[design], fleet_owner_for_tech);
        }
    }
    /* SatisfyOrders sets gd.grBits2 bit 3 for the whole task-5 price pass
     * (10b0:8510 onward) and clears it before FRemovePlayerMessage
     * (10b0:8dxx). GetTruePartCost uses that transient bit to suppress its
     * ordinary technology-premium doubling while UpdateShdefCost reprices
     * the owner's design. */
    if (state->fleet_race_reprice)
        *state->game_flags = (StarsU16)((*state->game_flags &
            (StarsU16)~0x0008U) | 0x0008U);
    resource_cost = 0;
    mineral_total = 0;
    for (mineral = 0; mineral < 3; ++mineral) {
        ore[mineral] = 0;
        for (design = 0; design < 16; ++design) {
            const StarsU8 *ship;
            count = satisfy_packed_get_word(fleet,
                (StarsU16)(SATISFY_PACKED_FLEET_SHIPS + design * 2));
            if (count == 0U)
                continue;
            ship = satisfy_scrap_design_for_cost(input, design, repriced);
            if (!ship) {
                if (state->fleet_race_reprice)
                    *state->game_flags = (StarsU16)(*state->game_flags &
                        (StarsU16)~0x0008U);
                return 0;
            }
            cost = count * satisfy_packed_get_word(ship,
                (StarsU16)(0x2cU + mineral * 2));
            if ((satisfy_packed_get_word(ship, 0x7bU) & 0x8000U) != 0U)
                cost /= 4U;
            ore[mineral] += cost;
        }
        if (planet_id == 0xffffU) {
            ore[mineral] /= 3U;
        } else if ((flags & 0x0200U) != 0U) {
            if (state->planet_race_scrap_bonus)
                ore[mineral] = (ore[mineral] * 9U) / 10U;
            else
                ore[mineral] = (ore[mineral] * 4U) / 5U;
        } else if (state->planet_race_scrap_bonus) {
            ore[mineral] = (ore[mineral] * 9U) / 20U;
        } else {
            ore[mineral] /= 3U;
        }
        cargo = satisfy_packed_get_long(fleet,
            (StarsU16)(SATISFY_PACKED_FLEET_CARGO + mineral * 4));
        ore[mineral] += cargo;
        mineral_total += ore[mineral];
    }
    for (design = 0; design < 16; ++design) {
        const StarsU8 *ship;
        count = satisfy_packed_get_word(fleet,
            (StarsU16)(SATISFY_PACKED_FLEET_SHIPS + design * 2));
        if (count == 0U)
            continue;
        ship = satisfy_scrap_design_for_cost(input, design, repriced);
        if (!ship) {
            if (state->fleet_race_reprice)
                *state->game_flags = (StarsU16)(*state->game_flags &
                    (StarsU16)~0x0008U);
            return 0;
        }
        cost = count * satisfy_packed_get_word(ship, 0x2aU);
        if ((satisfy_packed_get_word(ship, 0x7bU) & 0x8000U) != 0U)
            cost /= 4U;
        resource_cost += cost;
    }
    if (state->fleet_race_reprice)
        *state->game_flags = (StarsU16)(*state->game_flags &
            (StarsU16)~0x0008U);
    fleet_owner = (StarsI16)satisfy_packed_get_word(fleet,
        SATISFY_PACKED_FLEET_OWNER);
    fleet_word = satisfy_scrap_fleet_word(fleet, state);
    if (planet_id == 0xffffU) {
        StarsI16 salvage_id;
        salvage_id = input->ops->scrap_salvage(input->ops->context,
            fleet, ore);
        if (salvage_id < 0)
            return 0;
        input->ops->raw_message2(input->ops->context, fleet_owner,
            0x5b, -6, salvage_id, (StarsI16)fleet_word);
    } else {
        /* The original credits minerals before selecting the optional
         * resource-conversion message path (10b0:86fc-88a9). */
        for (mineral = 0; mineral < 3; ++mineral) {
            StarsU16 offset;
            offset = (StarsU16)(0x1cU + mineral * 4);
            satisfy_packed_put_long(planet, offset,
                satisfy_packed_get_long(planet, offset) + ore[mineral]);
        }
        if (state->planet_race_scrap_bonus) {
            StarsU16 base_resources;
            if (resource_cost > 0xffffU)
                resource_cost = 0xffffU;
            *state->planet_resource_extra = (StarsU16)(
                *state->planet_resource_extra + (StarsU16)resource_cost);
            adjusted_resources = resource_cost;
            if (resource_cost != 0U && planet_owner != -1) {
                base_resources = input->ops->scrap_planet_resources(
                    input->ops->context, planet_owner, planet_id);
                adjusted_resources = (resource_cost * base_resources) /
                    (resource_cost + base_resources);
            }
        }
        code = (StarsI16)((state->planet_race_scrap_bonus ? 0x5c : 0x59) +
            ((flags & 0x0200U) != 0U ? 1 : 0));
        memset(payload, 0, sizeof payload);
        payload[0] = (StarsI16)planet_id;
        payload[1] = (StarsI16)fleet_word;
        payload[2] = (StarsI16)mineral_total;
        payload[3] = (StarsI16)(mineral_total >> 16);
        payload[4] = (StarsI16)planet_id;
        if (state->planet_race_scrap_bonus) {
            payload[5] = (StarsI16)adjusted_resources;
            payload[6] = (StarsI16)(adjusted_resources >> 16);
        }
        input->ops->raw_message(input->ops->context, fleet_owner,
            code, payload);
        message_object = planet_id;
        technology_result = 0;
        if ((flags & 0x0200U) != 0U && input->ops->scrap_learn)
            technology_result = input->ops->scrap_learn(
                input->ops->context, planet_owner, &message_object);
        payload[0] = (StarsI16)message_object;
        payload[1] = (StarsI16)satisfy_packed_get_word(fleet,
            SATISFY_PACKED_FLEET_ID);
        if (technology_result == 0) {
            code = (StarsI16)(state->planet_race_scrap_bonus ?
                code + 0xe6 : code + 0xe7);
        } else if (technology_result < 0) {
            code = state->planet_race_scrap_bonus ? 0x13e : 0x13c;
            payload[state->planet_race_scrap_bonus ? 7 : 5] =
                (StarsI16)(-(technology_result + 1));
        } else {
            code = state->planet_race_scrap_bonus ? 0x13f : 0x13d;
            payload[state->planet_race_scrap_bonus ? 7 : 5] =
                (StarsI16)(technology_result - 1);
        }
        input->ops->raw_message(input->ops->context, planet_owner,
            code, payload);
    }
    if (input->ops->scrap_remove_message)
        input->ops->scrap_remove_message(input->ops->context, fleet_owner,
            0x4e, (StarsU16)(satisfy_packed_get_word(fleet,
                SATISFY_PACKED_FLEET_ID) | 0x8000U));
    satisfy_packed_put_word(fleet, SATISFY_PACKED_FLEET_FLAGS,
        (StarsU16)(satisfy_packed_get_word(fleet,
            SATISFY_PACKED_FLEET_FLAGS) | SATISFY_FLEET_INACTIVE));
    /* The original transfers friendly colonists after removing the old
     * message and setting fDead (10b0:8df0-8e65). */
    if (planet_id != 0xffffU && planet_owner == fleet_owner)
        satisfy_packed_put_long(planet, 0x28U,
            satisfy_packed_get_long(planet, 0x28U) +
            satisfy_packed_get_long(fleet,
                (StarsU16)(SATISFY_PACKED_FLEET_CARGO + 12U)));
    satisfy_packed_put_word(input->order_record, SATISFY_PACKED_ORDER_KIND,
        (StarsU16)(satisfy_packed_get_word(input->order_record,
            SATISFY_PACKED_ORDER_KIND) & (StarsU16)~0x1000U));
    satisfy_packed_put_word(fleet, SATISFY_PACKED_FLEET_TURN_FLAGS,
        (StarsU16)(satisfy_packed_get_word(fleet,
            SATISFY_PACKED_FLEET_TURN_FLAGS) &
            (StarsU16)~SATISFY_TURN_CLEAR_MASK));
    return 1;
}

static StarsI16 satisfy_special_order(StarsSatisfyOrdersInput *input,
    StarsSatisfyFleet *fleet, StarsSatisfyOrder *order)
{
    const StarsSatisfyOrdersOps *ops;
    StarsI16 result;
    StarsI16 code;

    ops = input->ops;
    result = -1;
    code = 0;
    if (order->kind == STARS_SATISFY_ORDER_MINE && input->pass == 3 &&
            (fleet->flags & STARS_SATISFY_FLEET_CAN_MINE) != 0U) {
        if (!ops->mine)
            return 0;
        result = ops->mine(ops->context, fleet, order);
        if (result == STARS_SATISFY_MINE_NO_PLANET)
            code = STARS_SATISFY_MESSAGE_MINE_NO_PLANET;
        else if (result == STARS_SATISFY_MINE_NO_SOURCE)
            code = STARS_SATISFY_MESSAGE_MINE_EMPTY;
        else if (result == STARS_SATISFY_MINE_HOSTILE_PLANET)
            code = STARS_SATISFY_MESSAGE_MINE_BLOCKED;
        else if (result == STARS_SATISFY_MINE_SUCCESS)
            code = 0;
        else
            return 0;
        if (code != 0)
            satisfy_raw_mine_message(ops, fleet, order, code);
    } else if (order->kind == STARS_SATISFY_ORDER_MERGE &&
            (input->pass & 1) == 0) {
        if (!order->target_present || order->target_inactive) {
            result = -1;
            code = STARS_SATISFY_MESSAGE_MERGE_INVALID;
        } else if (order->target_owner != fleet->owner) {
            result = 0;
            code = STARS_SATISFY_MESSAGE_MERGE_ENEMY;
        } else {
            if (!ops->merge)
                return 0;
            result = ops->merge(ops->context, fleet, order);
            if (result > 0)
                code = STARS_SATISFY_MESSAGE_MERGE_OK;
            else if (result == 0)
                code = STARS_SATISFY_MESSAGE_MERGE_ENEMY;
            else
                code = STARS_SATISFY_MESSAGE_MERGE_INVALID;
        }
        satisfy_raw_merge_message(ops, fleet, order, code);
    /* The service owns the original scrap accounting and message effects.
     * A successful scrap retires the fleet; legacy battle_plan users retain
     * their earlier callback behavior until they migrate. */
    } else if (order->kind == STARS_SATISFY_ORDER_SCRAP &&
            input->pass == 1) {
        if (ops->scrap) {
            result = ops->scrap(ops->context, fleet, order);
            if (result < 0)
                return 0;
            fleet->flags = (StarsU16)(fleet->flags |
                SATISFY_FLEET_INACTIVE);
        } else if (ops->battle_plan) {
            result = ops->battle_plan(ops->context, fleet, order);
            if (result < 0)
                return 0;
        } else {
            return 0;
        }
    } else if (order->kind == STARS_SATISFY_ORDER_LAY_MINES &&
            input->pass == 3 &&
            ((fleet->flags & STARS_SATISFY_FLEET_CAN_MINE) != 0U ||
             input->lay_mines_race_stat == 5)) {
        if (!ops->lay_mines)
            return 0;
        result = ops->lay_mines(ops->context, fleet, order);
        code = result > 0 ? STARS_SATISFY_MESSAGE_LAY_MINES :
            STARS_SATISFY_MESSAGE_LAY_MINES_EMPTY;
        if (result > 0 && order->repeat_count != 0 &&
                order->repeat_count != 5)
            --order->repeat_count;
        if (result > 0 && order->repeat_count != 0) {
            satisfy_message(ops, fleet, code, order->target_id,
                order->kind, result);
            return 0;
        }
    } else if (order->kind == STARS_SATISFY_ORDER_AUTO_ROUTE &&
            order->source_cord == 1 && order->target_id != -1 &&
            order->target_owner == fleet->owner &&
            order->target_starbase_flags != 0) {
        if (ops->auto_route_at_starbase)
            result = ops->auto_route_at_starbase(ops->context, fleet, order);
        else if (ops->auto_route)
            result = ops->auto_route(ops->context, fleet, order);
        if (result < 0)
            return 0;
        code = order->auto_route_message_variant != 0 ?
            STARS_SATISFY_MESSAGE_AUTO_ROUTE :
            STARS_SATISFY_MESSAGE_AUTO_ROUTE_NO_PATH;
        satisfy_raw_auto_route_message(ops, fleet, order, code);
    } else if (order->kind == STARS_SATISFY_ORDER_AUTO_ROUTE &&
            input->pass == 4) {
        if (ops->auto_fleet_order)
            result = ops->auto_fleet_order(ops->context, fleet, order);
        else if (ops->auto_route)
            result = ops->auto_route(ops->context, fleet, order);
        if (result < 0)
            return 0;
    } else if (order->kind == STARS_SATISFY_ORDER_TRANSFER_PLAYER &&
            input->pass == 4) {
        if (!ops->transfer_player)
            return 0;
        result = ops->transfer_player(ops->context, fleet, order);
        if (result < 0)
            return 0;
        if (result == STARS_SATISFY_PLAYER_TRANSFER_CREATED &&
                ops->materialize_designs)
            ops->materialize_designs(ops->context, fleet, order, result);
        if (result == STARS_SATISFY_PLAYER_TRANSFER_CREATED &&
                ops->ship_copy)
            ops->ship_copy(ops->context, fleet, order, result);
        if (result == STARS_SATISFY_PLAYER_TRANSFER_CREATED &&
                ops->copy_fleet_state)
            ops->copy_fleet_state(ops->context, fleet, order, result);
        code = satisfy_player_transfer_message(ops, fleet, order, result);
    } else {
        return 0;
    }
    if (code != 0)
        satisfy_message(ops, fleet, code, order->target_id, order->kind, result);
    order->active = 0;
    return 1;
}

static StarsI16 satisfy_order(StarsSatisfyOrdersInput *input,
    StarsSatisfyFleet *fleet, StarsSatisfyOrder *order)
{
    const StarsSatisfyOrdersOps *ops;
    StarsI16 resource;
    StarsI16 complete;
    StarsI32 wanted;
    StarsI32 available;
    StarsI32 amount;
    StarsI32 accepted;
    StarsI32 target_capacity;
    StarsI16 colonist_rejection;
    StarsI16 resource_mode;
    StarsI32 moved[STARS_SATISFY_RESOURCE_COUNT];

    for (resource = 0; resource < STARS_SATISFY_RESOURCE_COUNT; ++resource)
        moved[resource] = 0;

    ops = input->ops;
    complete = 1;
    for (resource = 0; resource < STARS_SATISFY_RESOURCE_COUNT;
         ++resource) {
        wanted = order->requested[resource];
        if (wanted <= 0)
            continue;
        resource_mode = order->resource_mode[resource];
        if (resource_mode == STARS_SATISFY_RESOURCE_MODE_STAT_PERCENT ||
                resource_mode == STARS_SATISFY_RESOURCE_MODE_STAT_PERCENT_ALT) {
            if ((input->pass & 1) != 0) {
                complete = 0;
                continue;
            }
            if (!ops->fleet_stat) {
                complete = 0;
                continue;
            }
            wanted = satisfy_stat_percent_delta(
                ops->fleet_stat(ops->context, fleet,
                    resource == STARS_SATISFY_RESOURCE_FUEL ? 1 : 2),
                wanted, fleet->cargo[resource]);
            if (wanted <= 0) {
                complete = 0;
                continue;
            }
        }
        if (!satisfy_resource_allowed(order, resource)) {
            complete = 0;
            continue;
        }
        available = satisfy_available(ops, fleet, resource);
        amount = satisfy_min(wanted, available);
        amount = satisfy_capacity(fleet, resource, amount);
        if (ops->target_capacity) {
            target_capacity = ops->target_capacity(ops->context,
                order, resource);
            if (target_capacity < amount)
                amount = target_capacity;
        }
        if (amount <= 0) {
            complete = 0;
            /* The original defers resource-shortage notifications until
             * pass four; earlier passes only leave the order active. */
            if (input->pass == 4) {
                satisfy_raw_shortage_message(ops, fleet, order, resource);
                satisfy_message(ops, fleet,
                    satisfy_shortage_message_code(order), order->target_id,
                    resource, wanted);
            }
            continue;
        }
        if (resource == STARS_SATISFY_RESOURCE_COLONISTS &&
                order->target_kind == STARS_SATISFY_TARGET_FLEET &&
                order->target_owner != fleet->owner) {
            if (ops->raw_message2)
                satisfy_raw_foreign_fleet_colonist(ops, fleet);
            else
                satisfy_message(ops, fleet,
                    STARS_SATISFY_MESSAGE_FOREIGN_FLEET_COLONISTS,
                    order->target_id, resource, amount);
            order->active = 0;
            return 1;
        }
        accepted = amount;
        if (ops->transfer)
            accepted = ops->transfer(ops->context, fleet, order,
                resource, amount);
        if (accepted < 0)
            accepted = 0;
        if (accepted > amount)
            accepted = amount;
        if (accepted > 0) {
            order->requested[resource] -= accepted;
            moved[resource] += accepted;
            fleet->cargo[resource] -= accepted;
            if (fleet->cargo[resource] < 0)
                fleet->cargo[resource] = 0;
            if (resource == STARS_SATISFY_RESOURCE_COLONISTS &&
                    order->target_kind == STARS_SATISFY_TARGET_PLANET) {
                if (satisfy_colonist_drop_allowed(fleet, order,
                        &colonist_rejection)) {
                    if (ops->queue_colonist_drop)
                        ops->queue_colonist_drop(ops->context, fleet, order,
                            accepted);
                } else if (colonist_rejection != 0) {
                    satisfy_raw_colonist_message(ops, fleet, order,
                        colonist_rejection);
                }
                if (colonist_rejection != 0)
                    satisfy_message(ops, fleet, colonist_rejection,
                        order->target_id, resource, accepted);
            }
            satisfy_raw_resource_message(ops, fleet, order,
                satisfy_transfer_message_code(fleet, order, resource),
                resource, accepted);
            satisfy_message(ops, fleet,
                satisfy_transfer_message_code(fleet, order, resource),
                order->target_id, resource, accepted);
        }
        if (order->requested[resource] > 0)
            complete = 0;
    }
    if (ops->create_salvage && order->target_kind ==
            STARS_SATISFY_TARGET_POINT && order->target_id == -1) {
        StarsI16 has_moved;

        has_moved = 0;
        for (resource = 0; resource < STARS_SATISFY_RESOURCE_COUNT;
                ++resource)
            if (moved[resource] > 0)
                has_moved = 1;
        if (has_moved)
            ops->create_salvage(ops->context, fleet, order, moved);
    }
    if (order->technology_flag != 0 && ops->technology_binding) {
        StarsSatisfyTechnologyOutcome outcome;
        StarsI16 negative_code;
        StarsI16 positive_code;

        negative_code = order->target_kind == STARS_SATISFY_TARGET_POINT ?
            SATISFY_TECH_POINT_NEGATIVE : SATISFY_TECH_PLANET_NEGATIVE;
        positive_code = order->target_kind == STARS_SATISFY_TARGET_POINT ?
            SATISFY_TECH_POINT_POSITIVE : SATISFY_TECH_PLANET_POSITIVE;
        memset(&outcome, 0, sizeof outcome);
        outcome.amount = 0;
        outcome.raw_recipient = fleet->owner;
        for (resource = 0; resource < STARS_SATISFY_RESOURCE_COUNT;
                ++resource)
            outcome.amount += moved[resource];
        ops->technology_binding(ops->context, fleet, order,
            outcome.amount, negative_code, positive_code, &outcome);
        if (outcome.message_code != 0 && ops->raw_message)
            ops->raw_message(ops->context, outcome.raw_recipient,
                outcome.message_code, outcome.raw_payload);
    } else if (ops->technology_discovery && order->technology_flag != 0) {
        StarsI32 technology_amount;

        technology_amount = 0;
        for (resource = 0; resource < STARS_SATISFY_RESOURCE_COUNT;
                ++resource)
            technology_amount += moved[resource];
        if (technology_amount > 0)
            ops->technology_discovery(ops->context, fleet, order,
                technology_amount);
    }
    if (complete)
        order->active = 0;
    return (StarsI16)(order->active == 0);
}

void stars_satisfy_orders(StarsSatisfyOrdersInput *input)
{
    StarsSatisfyFleet *fleet;
    StarsSatisfyOrder *order;
    StarsI16 index;
    StarsI16 order_index;

    if (!input || !input->fleets || !input->ops || input->pass < 1 ||
            input->pass > 4)
        return;
    for (index = 0; index < (StarsI16)input->fleet_count; ++index) {
        fleet = &input->fleets[index];
        if (input->pass == 1)
            fleet->turn_flags = (StarsU16)(fleet->turn_flags &
                (StarsU16)~SATISFY_TURN_CLEAR_MASK);
        if ((fleet->flags & SATISFY_FLEET_INACTIVE) != 0U ||
                (fleet->turn_flags & SATISFY_TURN_ALREADY_DONE) != 0U)
            continue;
        for (order_index = 0; order_index < (StarsI16)fleet->order_count;
             ++order_index) {
            order = &fleet->orders[order_index];
            if (!order->active ||
                    (order->phase_mask != 0 &&
                     (order->phase_mask & (1 << input->pass)) == 0))
                continue;
            if (satisfy_special_order(input, fleet, order)) {
                if (input->ops->order_done)
                    input->ops->order_done(input->ops->context, fleet,
                        order, order_index);
                continue;
            }
            if (order->kind != STARS_SATISFY_ORDER_TRANSFER)
                continue;
            if (!input->ops->transfer)
                continue;
            if (satisfy_order(input, fleet, order) && input->ops->order_done)
                input->ops->order_done(input->ops->context, fleet, order,
                    order_index);
        }
    }
}

StarsI16 stars_satisfy_orders_packed(StarsSatisfyPackedInput *input)
{
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput typed;
    StarsU16 packed;
    StarsI16 index;

    if (!input || !input->fleet_record || !input->order_record ||
            input->fleet_size < SATISFY_PACKED_FLEET_BYTES ||
            input->order_size < SATISFY_PACKED_ORDER_BYTES || !input->ops)
        return 0;
    if (input->scrap_state && input->pass == 1 &&
            (satisfy_packed_get_word(input->order_record,
                SATISFY_PACKED_ORDER_KIND) & 0x100fU) == 0x1005U &&
            (satisfy_packed_get_word(input->fleet_record,
                SATISFY_PACKED_FLEET_FLAGS) & SATISFY_FLEET_INACTIVE) == 0U &&
            (satisfy_packed_get_word(input->fleet_record,
                SATISFY_PACKED_FLEET_TURN_FLAGS) &
                SATISFY_TURN_ALREADY_DONE) == 0U &&
            (input->phase_mask == 0 ||
                (input->phase_mask & (1 << input->pass)) != 0)) {
        StarsI16 done;
        done = satisfy_scrap_packed(input);
        if (done && input->ops->order_done) {
            StarsSatisfyFleet completed_fleet;
            StarsSatisfyOrder completed_order;
            memset(&completed_fleet, 0, sizeof completed_fleet);
            memset(&completed_order, 0, sizeof completed_order);
            completed_fleet.id = (StarsI16)satisfy_packed_get_word(
                input->fleet_record, SATISFY_PACKED_FLEET_ID);
            completed_fleet.owner = (StarsI16)satisfy_packed_get_word(
                input->fleet_record, SATISFY_PACKED_FLEET_OWNER);
            completed_fleet.flags = satisfy_packed_get_word(
                input->fleet_record, SATISFY_PACKED_FLEET_FLAGS);
            completed_order.kind = STARS_SATISFY_ORDER_SCRAP;
            completed_order.active = 0;
            input->ops->order_done(input->ops->context, &completed_fleet,
                &completed_order, 0);
        }
        return done;
    }
    memset(&fleet, 0, sizeof fleet);
    memset(&order, 0, sizeof order);
    fleet.id = (StarsI16)satisfy_packed_get_word(input->fleet_record,
        SATISFY_PACKED_FLEET_ID);
    fleet.owner = (StarsI16)satisfy_packed_get_word(input->fleet_record,
        SATISFY_PACKED_FLEET_OWNER);
    fleet.planet_id = input->planet_id;
    fleet.flags = satisfy_packed_get_word(input->fleet_record,
        SATISFY_PACKED_FLEET_FLAGS);
    fleet.turn_flags = satisfy_packed_get_word(input->fleet_record,
        SATISFY_PACKED_FLEET_TURN_FLAGS);
    for (index = 0; index < STARS_SATISFY_RESOURCE_COUNT; ++index) {
        fleet.cargo[index] = input->cargo[index];
        fleet.capacity[index] = input->capacity[index];
        order.requested[index] = input->requested[index];
        order.resource_mode[index] = input->resource_mode[index];
    }
    packed = satisfy_packed_get_word(input->order_record,
        SATISFY_PACKED_ORDER_KIND);
    order.kind = (StarsI16)(packed & 0x000fU);
    order.target_kind = (StarsI16)((packed >> 8) & 0x000fU);
    order.active = (StarsI16)((packed >> 12) & 1U);
    order.phase_mask = input->phase_mask;
    order.target_id = (StarsI16)satisfy_packed_get_word(input->order_record,
        SATISFY_PACKED_ORDER_TARGET);
    order.target_owner = input->target_owner;
    order.target_present = input->target_present;
    order.target_inactive = input->target_inactive;
    order.target_starbase_flags = input->target_starbase_flags;
    order.source_cord = input->source_cord;
    order.auto_route_message_variant = input->auto_route_message_variant;
    order.target_planet_flags = input->target_planet_flags;
    order.colonist_race_stat = input->colonist_race_stat;
    order.repeat_count = input->repeat_count;
    order.technology_flag = input->technology_flag;
    fleet.orders = &order;
    fleet.order_count = 1U;
    typed.fleets = &fleet;
    typed.fleet_count = 1U;
    typed.pass = input->pass;
    typed.lay_mines_race_stat = input->lay_mines_race_stat;
    typed.ops = input->ops;
    stars_satisfy_orders(&typed);
    for (index = 0; index < STARS_SATISFY_RESOURCE_COUNT; ++index) {
        input->cargo[index] = fleet.cargo[index];
        input->requested[index] = order.requested[index];
    }
    satisfy_packed_put_word(input->fleet_record,
        SATISFY_PACKED_FLEET_TURN_FLAGS, fleet.turn_flags);
    satisfy_packed_put_word(input->fleet_record,
        SATISFY_PACKED_FLEET_FLAGS, fleet.flags);
    packed = satisfy_packed_get_word(input->order_record,
        SATISFY_PACKED_ORDER_KIND);
    packed = (StarsU16)(packed & (StarsU16)~(StarsU16)(1U << 12));
    if (order.active)
        packed = (StarsU16)(packed | (StarsU16)(1U << 12));
    satisfy_packed_put_word(input->order_record, SATISFY_PACKED_ORDER_KIND,
        packed);
    return 1;
}
