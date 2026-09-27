#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 events;
    StarsU16 failures;
} Trace;

static void event(Trace *trace)
{
    ++trace->events;
}

static StarsI16 transfer(void *opaque, const void *opaque_value)
{
    const StarsRunLogTransfer *value =
        (const StarsRunLogTransfer *)opaque_value;
    Trace *trace = (Trace *)opaque;
    if (value->source_id != 0x1111U || value->target_id != 0x2222U)
        ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 order(void *opaque, const StarsRunLogOrder *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->fleet_id != 7U || value->operation > STARS_LOG_ORDER_REPLACE)
        ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 fleet_flag(void *opaque, const StarsRunLogFleetFlag *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->fleet_id != 7U || value->value != 1U) ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 order_flag(void *opaque, const StarsRunLogOrderFlag *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->fleet_id != 7U || value->index != 2U || value->value != 3U)
        ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 balance(void *opaque,
    const StarsRunLogTransferBalance *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->first_fleet != 7U || value->second_fleet != 8U)
        ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 group(void *opaque, const StarsRunLogFleetGroup *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->base_fleet != 7U || value->fleet_count == 0U)
        ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 design(void *opaque, const StarsRunLogDesignChange *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->player != 2U || value->slot != 3U || value->action != 1U)
        ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 production(void *opaque,
    const StarsRunLogProduction *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->planet_id != 9U || value->entry_count != 1U)
        ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 battle_plan(void *opaque,
    const StarsRunLogBattlePlan *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->player != 2U || value->plan_index != 1U || value->deleted)
        ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 research(void *opaque, const StarsRunLogResearch *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->research_percent != 50U || value->technology != 0x32U)
        ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 planet_scan(void *opaque,
    const StarsRunLogPlanetScan *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->planet_id != 9U || value->remote_terraform != 1U ||
            value->scanner_range != 3U || value->scanner_mines != 2U ||
            value->scanner_radius != 4U) ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 salt(void *opaque, const StarsRunLogSalt *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->value != 0x44332211UL) ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 relations(void *opaque, const StarsRunLogRelations *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->player_count != 3U || value->data[0] != 1U) ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 fleet_plan(void *opaque, const StarsRunLogFleetPlan *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->fleet_id != 7U || value->plan_index != 4U) ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 thing(void *opaque, const StarsRunLogThing *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->thing_id != 9U || value->owner != 4U) ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 fleet_name(void *opaque, const StarsRunLogFleetName *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->fleet_id != 7U || value->encoding_size != 1U ||
            value->encoding[0] != 0xa5U) ++trace->failures;
    event(trace);
    return 1;
}

static StarsI16 player_settings(void *opaque,
    const StarsRunLogPlayerBytes *value)
{
    Trace *trace = (Trace *)opaque;
    if (value->size != 3U || value->data[0] != 0x55U) ++trace->failures;
    event(trace);
    return 1;
}

static void put_word(StarsU8 *data, StarsU16 value)
{
    data[0] = (StarsU8)value;
    data[1] = (StarsU8)(value >> 8);
}

static void put_long(StarsU8 *data, StarsU32 value)
{
    put_word(data, (StarsU16)value);
    put_word(data + 2U, (StarsU16)(value >> 16));
}

static StarsI16 run(StarsU16 type, StarsU8 *payload, StarsU16 size,
    const StarsRunLogRecordOps *ops, Trace *trace)
{
    StarsRunLogRecordInput input;
    input.record_type = type;
    input.payload_size = size;
    input.payload = payload;
    input.player = 2U;
    input.player_count = 3U;
    input.game_flags = 2U;
    {
        StarsI16 result = stars_run_log_record(&input, ops, trace);
        return result;
    }
}

int main(void)
{
    Trace trace;
    StarsRunLogRecordOps ops;
    StarsRunLogRecordInput empty_input;
    StarsU8 payload[64];
    StarsU16 i;
    memset(&trace, 0, sizeof trace);
    memset(&ops, 0, sizeof ops);
    ops.transfer = transfer;
    ops.order = order;
    ops.fleet_flag = fleet_flag;
    ops.order_flag = order_flag;
    ops.transfer_balance = balance;
    ops.split_fleet = group;
    ops.merge_fleet = group;
    ops.design = design;
    ops.production = production;
    ops.battle_plan = battle_plan;
    ops.research = research;
    ops.planet_scan = planet_scan;
    ops.salt = salt;
    ops.relations = relations;
    ops.fleet_plan = fleet_plan;
    ops.thing = thing;
    ops.fleet_name = fleet_name;
    ops.player_settings = player_settings;

    memset(payload, 0, sizeof payload);
    put_word(payload, 0x1111U); put_word(payload + 2U, 0x2222U);
    payload[4] = 0x12U; payload[5] = 3U;
    payload[6] = 0xfeU; put_word(payload + 7U, 0x1234U);
    if (!run(STARS_LOG_RT_TRANSFER_BYTE, payload, 9U, &ops, &trace) ||
            !run(STARS_LOG_RT_TRANSFER_WORD, payload, 10U, &ops, &trace))
        ++trace.failures;
    put_long(payload + 6U, 0xfffffffeUL);
    if (!run(STARS_LOG_RT_TRANSFER_LONG, payload, 14U, &ops, &trace))
        ++trace.failures;

    put_word(payload, 7U); put_word(payload + 2U, 2U);
    if (!run(STARS_LOG_RT_DELETE_ORDER, payload, 4U, &ops, &trace))
        ++trace.failures;
    payload[4] = 0xabu; payload[5] = 0xcdu;
    if (!run(STARS_LOG_RT_INSERT_ORDER, payload, 6U, &ops, &trace) ||
            !run(STARS_LOG_RT_REPLACE_ORDER, payload, 6U, &ops, &trace))
        ++trace.failures;
    put_word(payload + 2U, 1U); put_word(payload + 4U, 1U);
    if (!run(STARS_LOG_RT_FLEET_FLAG, payload, 6U, &ops, &trace))
        ++trace.failures;
    put_word(payload + 2U, 2U); put_word(payload + 4U, 3U);
    if (!run(STARS_LOG_RT_ORDER_FLAG, payload, 6U, &ops, &trace))
        ++trace.failures;

    put_word(payload, 7U); put_word(payload + 2U, 8U);
    put_long(payload + 5U, 1UL);
    if (!run(STARS_LOG_RT_TRANSFER_BALANCE, payload, 9U, &ops, &trace) ||
            !run(STARS_LOG_RT_SPLIT_FLEET, payload, 2U, &ops, &trace) ||
            !run(STARS_LOG_RT_MERGE_FLEET, payload, 4U, &ops, &trace))
        ++trace.failures;

    put_word(payload, (StarsU16)((3U << 8) | (2U << 4) | 1U));
    if (!run(STARS_LOG_RT_SHIP_DESIGN, payload, 2U, &ops, &trace))
        ++trace.failures;
    put_word(payload, 9U); put_word(payload + 2U, 1U);
    memset(payload + 4U, 0, 4U);
    if (!run(STARS_LOG_RT_PRODUCTION, payload, 6U, &ops, &trace))
        ++trace.failures;
    put_word(payload, (StarsU16)((1U << 4) | 2U));
    put_word(payload + 2U, 0U);
    if (!run(STARS_LOG_RT_BATTLE_PLAN, payload, 4U, &ops, &trace))
        ++trace.failures;
    payload[0] = 50U; payload[1] = 0x32U;
    if (!run(STARS_LOG_RT_RESEARCH, payload, 2U, &ops, &trace))
        ++trace.failures;
    put_word(payload, 9U);
    put_long(payload + 2U, 1UL | (3UL << 1) | (2UL << 11) |
        (4UL << 15));
    if (!run(STARS_LOG_RT_PLANET_SCAN, payload, 6U, &ops, &trace))
        ++trace.failures;
    put_long(payload, 0x44332211UL);
    if (!run(STARS_LOG_RT_PLAYER_SALT, payload, 4U, &ops, &trace))
        ++trace.failures;
    payload[0] = 1U; payload[1] = 2U; payload[2] = 3U;
    if (!run(STARS_LOG_RT_RELATIONS, payload, 3U, &ops, &trace))
        ++trace.failures;
    put_word(payload, 7U); put_word(payload + 2U, 4U);
    if (!run(STARS_LOG_RT_FLEET_PLAN, payload, 4U, &ops, &trace))
        ++trace.failures;
    put_word(payload, 9U); put_word(payload + 2U, 4U);
    if (!run(STARS_LOG_RT_THING, payload, 4U, &ops, &trace))
        ++trace.failures;
    put_word(payload, 7U); payload[4] = 1U; payload[5] = 0xa5U;
    if (!run(STARS_LOG_RT_FLEET_NAME, payload, 6U, &ops, &trace))
        ++trace.failures;
    payload[0] = 0x55U; payload[1] = 0x66U; payload[2] = 0x77U;
    if (!run(STARS_LOG_RT_PLAYER_SETTINGS, payload, 3U, &ops, &trace))
        ++trace.failures;

    memset(&empty_input, 0, sizeof empty_input);
    empty_input.record_type = STARS_LOG_RT_NONE;
    empty_input.player = 2U;
    empty_input.player_count = 3U;
    if (stars_run_log_record(&empty_input, &ops, &trace) != 1)
        ++trace.failures;
    {
        StarsU8 empty_balance[7];
        memset(empty_balance, 0, sizeof empty_balance);
        empty_balance[4] = 0x22U;
        ops.transfer_balance = 0;
        if (!run(STARS_LOG_RT_TRANSFER_BALANCE, empty_balance,
                sizeof empty_balance, &ops, &trace)) ++trace.failures;
    }
    i = trace.events;
#ifndef TARGET_NATIVE
    printf("RUN_LOG_CHECKS=%u FAILURES=%u RESULT=%s\n", (unsigned)i,
        (unsigned)trace.failures, trace.failures ? "FAIL" : "PASS");
#endif
    return trace.failures ? 1 : 0;
}
