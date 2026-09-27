/* Semantic reconstruction of NE 10:A38C-C7A2 (FRunLogRecord).
 *
 * The executable's routine is large because every memory-log record mutates
 * a different DGROUP object.  This source keeps the common part together - the
 * record grammar and its validation - and makes the old object/heap operations
 * typed callbacks.  It is therefore ordinary C source, not a byte emitter;
 * an application adapter can connect each callback to its real fleet, planet,
 * design, or player table.
 */
#include "core.h"
#include <string.h>

#define LOG_MAX_CARGO_ITEMS 5U
#define LOG_MAX_DESIGNS 26U
#define LOG_MAX_ORDER_BYTES 18U
#define LOG_MAX_MERGE_FLEETS 256U
#define LOG_MAX_RELATION_BYTES 256U
#define LOG_MAX_PLAYER_BYTES 26U

static StarsU16 log_word(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] |
        ((StarsU16)data[(StarsU16)(offset + 1U)] << 8));
}

static StarsU32 log_long(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU32)log_word(data, offset) |
        ((StarsU32)log_word(data, (StarsU16)(offset + 2U)) << 16);
}

static StarsI16 log_call_void(StarsRunLogVoid callback, void *context,
    const void *value)
{
    return callback ? callback(context, value) : 1;
}

static StarsI16 log_call_order(StarsRunLogOrderOp callback, void *context,
    const StarsRunLogOrder *value)
{
    return callback ? callback(context, value) : 1;
}

static StarsI16 log_call_fleet_flag(StarsRunLogFleetFlagOp callback,
    void *context, const StarsRunLogFleetFlag *value)
{
    return callback ? callback(context, value) : 1;
}

static StarsI16 log_call_order_flag(StarsRunLogOrderFlagOp callback,
    void *context, const StarsRunLogOrderFlag *value)
{
    return callback ? callback(context, value) : 1;
}

/* The three transfer record widths are part of the log format, not a choice
 * made by the receiving object: type 1 stores signed bytes, type 2 signed
 * words, and type 0x19 signed longs. */
static StarsI16 log_decode_transfer(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsRunLogTransfer value;
    StarsU16 mask;
    StarsU16 cursor;
    StarsU16 item;
    StarsU16 width;
    StarsU32 raw;
    if (!input->payload || input->payload_size < 6U) return 0;
    value.source_id = log_word(input->payload, 0U);
    value.target_id = log_word(input->payload, 2U);
    value.source_kind = (StarsU8)(input->payload[4] & 0x0fU);
    value.target_kind = (StarsU8)(input->payload[4] >> 4);
    mask = input->payload[5];
    cursor = 6U;
    width = input->record_type == STARS_LOG_RT_TRANSFER_BYTE ? 1U :
        (input->record_type == STARS_LOG_RT_TRANSFER_WORD ? 2U : 4U);
    for (item = 0U; item < LOG_MAX_CARGO_ITEMS; ++item) {
        value.delta[item] = 0;
        if ((mask & (StarsU16)(1U << item)) != 0U) {
            if ((StarsU16)(cursor + width) > input->payload_size) return 0;
            if (width == 1U) {
                value.delta[item] = (StarsI32)(signed char)
                    input->payload[cursor];
            } else if (width == 2U) {
                value.delta[item] = (StarsI32)(StarsI16)
                    log_word(input->payload, cursor);
            } else {
                raw = log_long(input->payload, cursor);
                value.delta[item] = (StarsI32)raw;
            }
            cursor = (StarsU16)(cursor + width);
        }
    }
    return log_call_void(ops ? ops->transfer : 0, context, &value);
}

static StarsI16 log_decode_order(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsRunLogOrder value;
    StarsU16 size;
    if (!input->payload || input->payload_size < 4U) return 0;
    size = (StarsU16)(input->payload_size - 4U);
    if (size > LOG_MAX_ORDER_BYTES) return 0;
    value.fleet_id = log_word(input->payload, 0U);
    value.index = log_word(input->payload, 2U);
    value.operation = STARS_LOG_ORDER_REPLACE;
    value.delete_following = 0;
    if (input->record_type == STARS_LOG_RT_DELETE_ORDER) {
        value.operation = STARS_LOG_ORDER_DELETE;
        value.delete_following = (StarsI16)((value.index & 0x8000U) != 0U);
        value.index = (StarsU16)(value.index & 0x7fffU);
        if (size != 0U) return 0;
    } else {
        if (input->record_type == STARS_LOG_RT_INSERT_ORDER)
            value.operation = STARS_LOG_ORDER_INSERT;
        memcpy(value.data, input->payload + 4U, size);
    }
    value.data_size = size;
    return log_call_order(ops ? ops->order : 0, context, &value);
}

static StarsI16 log_decode_fleet_flags(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    if (input->record_type == STARS_LOG_RT_FLEET_FLAG) {
        StarsRunLogFleetFlag value;
        if (!input->payload || input->payload_size < 4U) return 0;
        value.fleet_id = log_word(input->payload, 0U);
        value.value = (StarsU16)(log_word(input->payload, 2U) & 1U);
        return log_call_fleet_flag(ops ? ops->fleet_flag : 0, context,
            &value);
    } else {
        StarsRunLogOrderFlag value;
        if (!input->payload || input->payload_size < 6U) return 0;
        value.fleet_id = log_word(input->payload, 0U);
        value.index = log_word(input->payload, 2U);
        value.value = (StarsU16)(log_word(input->payload, 4U) & 0x000fU);
        if (value.index > 9U) return 0;
        return log_call_order_flag(ops ? ops->order_flag : 0, context,
            &value);
    }
}

static StarsI16 log_decode_balance(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsRunLogTransferBalance value;
    StarsU32 mask;
    StarsU16 cursor;
    StarsU16 item;
    if (!input->payload || input->payload_size < 7U) return 0;
    value.first_fleet = log_word(input->payload, 0U);
    value.second_fleet = log_word(input->payload, 2U);
    /* The packed transfer-balance record places its presence mask at the
     * odd offset five; this is why the original decompiler showed a mix of
     * byte and word accesses in this case. */
    mask = log_word(input->payload, 5U);
    value.mask = (StarsU16)mask;
    cursor = 7U;
    for (item = 0U; item < 16U; ++item) {
        value.delta[item] = 0;
        if ((mask & (1UL << item)) != 0UL) {
            if ((StarsU16)(cursor + 2U) > input->payload_size) return 0;
            value.delta[item] = (StarsI16)log_word(input->payload, cursor);
            cursor = (StarsU16)(cursor + 2U);
        }
    }
    return ops && ops->transfer_balance ?
        ops->transfer_balance(context, &value) : 1;
}

static StarsI16 log_decode_group(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsU16 fleets[LOG_MAX_MERGE_FLEETS];
    StarsRunLogFleetGroup value;
    StarsU16 count;
    StarsU16 i;
    if (!input->payload || input->payload_size < 2U ||
            (input->payload_size & 1U) != 0U) return 0;
    count = (StarsU16)(input->payload_size / 2U);
    if (count > LOG_MAX_MERGE_FLEETS) return 0;
    value.base_fleet = log_word(input->payload, 0U);
    value.fleets = fleets;
    value.fleet_count = count;
    for (i = 0U; i < count; ++i)
        fleets[i] = log_word(input->payload, (StarsU16)(i * 2U));
    if (input->record_type == STARS_LOG_RT_SPLIT_FLEET)
        return ops && ops->split_fleet ? ops->split_fleet(context, &value) : 1;
    return ops && ops->merge_fleet ? ops->merge_fleet(context, &value) : 1;
}

static StarsI16 log_decode_design(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsRunLogDesignChange value;
    StarsU16 header;
    if (!input->payload || input->payload_size < 2U) return 0;
    header = log_word(input->payload, 0U);
    value.player = (StarsU16)((header >> 4) & 0x0fU);
    value.slot = (StarsU16)((header >> 8) & 0x1fU);
    value.action = (StarsU16)(header & 0x000fU);
    if (value.player != input->player || value.slot >= LOG_MAX_DESIGNS ||
            (value.action != 0U && value.action != 1U)) return 0;
    value.design_payload = input->payload + 2U;
    value.design_payload_size = (StarsU16)(input->payload_size - 2U);
    return ops && ops->design ? ops->design(context, &value) : 1;
}

static StarsI16 log_decode_production(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsRunLogProduction value;
    if (!input->payload || input->payload_size < 2U ||
            ((input->payload_size - 2U) & 3U) != 0U) return 0;
    value.planet_id = log_word(input->payload, 0U);
    value.entries = input->payload + 2U;
    value.entry_count = (StarsU16)((input->payload_size - 2U) / 4U);
    return ops && ops->production ? ops->production(context, &value) : 1;
}

static StarsI16 log_decode_battle_plan(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsRunLogBattlePlan value;
    StarsU16 header;
    StarsU16 flags;
    if (!input->payload || input->payload_size < 4U) return 0;
    header = log_word(input->payload, 0U);
    flags = log_word(input->payload, 2U);
    value.player = (StarsU16)(header & 0x000fU);
    value.plan_index = (StarsU16)((header >> 4) & 0x000fU);
    value.deleted = (StarsU16)((header >> 14) & 1U);
    value.payload = input->payload;
    value.payload_size = input->payload_size;
    /* Non-deletion records carry the two packed tactic nibbles. */
    if (value.player != input->player && value.deleted == 0U) return 0;
    if (!value.deleted && (((header >> 8) & 0x0fU) > 6U ||
            (flags & 0x000fU) > 8U || ((flags >> 4) & 0x0fU) > 8U)) return 0;
    return ops && ops->battle_plan ? ops->battle_plan(context, &value) : 1;
}

static StarsI16 log_decode_research(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsRunLogResearch value;
    if (!input->payload || input->payload_size < 2U ||
            input->payload[0] > 100U || (input->payload[1] & 0x0fU) >= 6U ||
            ((input->payload[1] >> 4) & 0x0fU) >= 8U) return 0;
    value.research_percent = input->payload[0];
    value.technology = input->payload[1];
    return ops && ops->research ? ops->research(context, &value) : 1;
}

static StarsI16 log_decode_scan(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsRunLogPlanetScan value;
    StarsU32 packed;
    if (!input->payload || input->payload_size < 6U) return 0;
    value.planet_id = log_word(input->payload, 0U);
    packed = log_long(input->payload, 2U);
    value.remote_terraform = (StarsU16)(packed & 1UL);
    value.scanner_range = (StarsU16)((packed >> 1) & 0x03ffUL);
    value.scanner_mines = (StarsU16)((packed >> 11) & 0x000fUL);
    value.scanner_radius = (StarsU16)((packed >> 15) & 0x03ffUL);
    return ops && ops->planet_scan ? ops->planet_scan(context, &value) : 1;
}

/* NE 10:A38C-C7A2. Apply a single runtime-log record.  The switch mirrors
 * the executable's dispatch, but its cases now say what they change: an
 * order record edits an order array, type 0x1d replaces production entries,
 * and type 0x2c supplies a fleet-name encoding. */
StarsI16 stars_run_log_record(const StarsRunLogRecordInput *input,
    const StarsRunLogRecordOps *ops, void *context)
{
    StarsRunLogSalt salt;
    StarsRunLogRelations relations;
    StarsRunLogFleetPlan fleet_plan;
    StarsRunLogThing thing;
    StarsRunLogFleetName name;
    StarsRunLogPlayerBytes bytes;
    if (!input) return 0;
    switch (input->record_type) {
    case STARS_LOG_RT_NONE:
        return 1;
    case STARS_LOG_RT_TRANSFER_BYTE:
    case STARS_LOG_RT_TRANSFER_WORD:
    case STARS_LOG_RT_TRANSFER_LONG:
        return log_decode_transfer(input, ops, context);
    case STARS_LOG_RT_DELETE_ORDER:
    case STARS_LOG_RT_INSERT_ORDER:
    case STARS_LOG_RT_REPLACE_ORDER:
        return log_decode_order(input, ops, context);
    case STARS_LOG_RT_FLEET_FLAG:
    case STARS_LOG_RT_ORDER_FLAG:
        return log_decode_fleet_flags(input, ops, context);
    case STARS_LOG_RT_TRANSFER_BALANCE:
        return log_decode_balance(input, ops, context);
    case STARS_LOG_RT_SPLIT_FLEET:
    case STARS_LOG_RT_MERGE_FLEET:
        return log_decode_group(input, ops, context);
    case STARS_LOG_RT_SHIP_DESIGN:
        return log_decode_design(input, ops, context);
    case STARS_LOG_RT_PRODUCTION:
        return log_decode_production(input, ops, context);
    case STARS_LOG_RT_BATTLE_PLAN:
        return log_decode_battle_plan(input, ops, context);
    case STARS_LOG_RT_RESEARCH:
        return log_decode_research(input, ops, context);
    case STARS_LOG_RT_PLANET_SCAN:
        return log_decode_scan(input, ops, context);
    case STARS_LOG_RT_PLAYER_SALT:
        if ((input->game_flags & 2U) == 0U) return 1;
        if (!input->payload || input->payload_size < 4U) return 0;
        salt.value = log_long(input->payload, 0U);
        return ops && ops->salt ? ops->salt(context, &salt) : 1;
    case STARS_LOG_RT_RELATIONS:
        if (!input->payload || input->payload_size < input->player_count ||
                input->player_count > LOG_MAX_RELATION_BYTES) return 0;
        relations.data = input->payload;
        relations.player_count = input->player_count;
        return ops && ops->relations ? ops->relations(context, &relations) : 1;
    case STARS_LOG_RT_FLEET_PLAN:
        if (!input->payload || input->payload_size < 4U) return 0;
        fleet_plan.fleet_id = log_word(input->payload, 0U);
        fleet_plan.plan_index = log_word(input->payload, 2U);
        return ops && ops->fleet_plan ?
            ops->fleet_plan(context, &fleet_plan) : 1;
    case STARS_LOG_RT_THING:
        if (!input->payload || input->payload_size < 4U) return 0;
        thing.thing_id = log_word(input->payload, 0U);
        thing.owner = (StarsU8)log_word(input->payload, 2U);
        return ops && ops->thing ? ops->thing(context, &thing) : 1;
    case STARS_LOG_RT_FLEET_NAME:
        if (!input->payload || input->payload_size < 6U) return 0;
        name.fleet_id = log_word(input->payload, 0U);
        name.encoding_size = input->payload[4];
        name.encoding = input->payload + 5U;
        if ((StarsU16)(5U + name.encoding_size) > input->payload_size)
            return 0;
        if (ops && ops->fleet_name)
            return ops->fleet_name(context, &name);
        return 1;
    case STARS_LOG_RT_PLAYER_SETTINGS:
        if ((input->game_flags & 2U) == 0U) return 1;
        if (!input->payload || input->payload_size > LOG_MAX_PLAYER_BYTES)
            return 0;
        bytes.data = input->payload;
        bytes.size = input->payload_size;
        return ops && ops->player_settings ?
            ops->player_settings(context, &bytes) : 1;
    default:
        /* Unknown records were intentionally ignored by the original
         * switch.  Keeping that behavior lets newer logs pass through an
         * older reader without inventing a state mutation. */
        return 1;
    }
}
