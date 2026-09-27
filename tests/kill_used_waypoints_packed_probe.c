#include <stdio.h>
#include <string.h>
#include "core.h"

#define FLEETS 3
#define TABLE_SLOTS 4

typedef struct Trace {
    StarsI16 deletes;
    StarsI16 messages;
    StarsI16 last_message;
    StarsI16 last_fleet;
} Trace;

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 get16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1U] << 8));
}

static void put_table_ref(StarsU8 *table, StarsI16 slot, StarsI16 record)
{
    StarsU16 offset = (StarsU16)(slot * 4);
    if (record < 0) {
        put16(table, offset, 0U);
        put16(table, (StarsU16)(offset + 2U), 0U);
    } else {
        put16(table, offset, (StarsU16)(record + 1));
        put16(table, (StarsU16)(offset + 2U), 1U);
    }
}

static void put_order(StarsU8 *raw, StarsI16 index, StarsI16 x,
    StarsI16 y, StarsI16 target, StarsU16 flags)
{
    StarsU16 base = (StarsU16)(4U + (StarsU16)index * 18U);
    put16(raw, base, (StarsU16)x);
    put16(raw, (StarsU16)(base + 2U), (StarsU16)y);
    put16(raw, (StarsU16)(base + 4U), (StarsU16)target);
    put16(raw, (StarsU16)(base + 6U), flags);
}

static void seed_fleet(StarsU8 *fleet, StarsU8 *plord, StarsI16 id,
    StarsI16 cord, StarsI16 order_index)
{
    memset(fleet, 0, 124U);
    memset(plord, 0, 220U);
    put16(fleet, 0U, (StarsU16)id);
    put16(fleet, 2U, 3U);
    put16(fleet, 98U, (StarsU16)cord);
    put16(fleet, 100U, (StarsU16)(order_index + 1));
}

static void delete_waypoint(void *context, StarsKillUsedWaypointFleet *fleet,
    StarsI16 index, StarsI16 recycle)
{
    Trace *trace = (Trace *)context;
    (void)index;
    (void)recycle;
    ++trace->deletes;
    if (fleet->cord > 1)
        --fleet->cord;
}

static void send_message_ex(void *context, StarsI16 owner, StarsI16 message,
    StarsU16 status_word, StarsI16 fleet_id)
{
    Trace *trace = (Trace *)context;
    (void)owner;
    (void)status_word;
    ++trace->messages;
    trace->last_message = message;
    trace->last_fleet = fleet_id;
}

int main(void)
{
    StarsU8 fleet_table[TABLE_SLOTS * 4];
    StarsU8 fleet_records[FLEETS * 124];
    StarsU8 plord_records[FLEETS * 220];
    StarsKillUsedWaypointFleet views[FLEETS];
    StarsKillUsedWaypointOrder orders[FLEETS * 12];
    StarsKillUsedWaypointOps ops;
    StarsKillUsedWaypointsPackedInput input;
    Trace trace;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { \
    ++checks; \
    if (!(condition)) ++failures; \
} while (0)

    memset(fleet_table, 0, sizeof fleet_table);
    memset(fleet_records, 0, sizeof fleet_records);
    memset(plord_records, 0, sizeof plord_records);
    memset(views, 0, sizeof views);
    memset(orders, 0, sizeof orders);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    seed_fleet(&fleet_records[0], &plord_records[0], 1, 2, 0);
    seed_fleet(&fleet_records[124], &plord_records[220], 2, 2, 1);
    seed_fleet(&fleet_records[248], &plord_records[440], 3, 2, 2);
    put16(&fleet_records[0], 4U, 0x8000U);
    put16(&fleet_records[124], 4U, 0x8000U);
    put16(&fleet_records[248], 4U, 0x8000U);
    put_table_ref(fleet_table, 0, 0);
    put_table_ref(fleet_table, 1, 1);
    put_table_ref(fleet_table, 2, -1);
    put_table_ref(fleet_table, 3, 2);
    ops.delete_waypoint = delete_waypoint;
    ops.send_message_ex = send_message_ex;
    ops.context = &trace;
    input.fleet_table = fleet_table;
    input.fleet_table_count = TABLE_SLOTS;
    input.fleet_records = fleet_records;
    input.fleet_record_count = FLEETS;
    input.plord_records = plord_records;
    input.plord_record_count = FLEETS;
    input.fleet_views = views;
    input.order_views = orders;
    input.workspace_count = FLEETS;
    input.ops = &ops;
    input.enumerated_count = 0U;
    input.malformed = 0U;
    stars_kill_used_waypoints_packed(&input);
    CHECK(input.enumerated_count == 2U && input.malformed == 0U);
    CHECK(trace.deletes == 2 && trace.messages == 2 &&
        trace.last_message == 0x0137 && trace.last_fleet == 2);
    CHECK(get16(&fleet_records[0], 98U) == 1U &&
        get16(&fleet_records[124], 98U) == 1U &&
        get16(&fleet_records[248], 98U) == 2U);

    memset(&trace, 0, sizeof trace);
    memset(fleet_table, 0, sizeof fleet_table);
    seed_fleet(&fleet_records[0], &plord_records[0], 11, 3, 0);
    seed_fleet(&fleet_records[124], &plord_records[220], 12, 1, 1);
    put16(&fleet_records[0], 8U, 10U);
    put16(&fleet_records[0], 10U, 20U);
    put16(&fleet_records[124], 8U, 33U);
    put16(&fleet_records[124], 10U, 44U);
    put_order(&plord_records[0], 1, 1, 2, 12, 0x0200U);
    put_order(&plord_records[0], 5, 99, 99, 0, 0U);
    put_table_ref(fleet_table, 0, 1);
    put_table_ref(fleet_table, 1, 0);
    put_table_ref(fleet_table, 2, -1);
    input.enumerated_count = 0U;
    input.malformed = 0U;
    stars_kill_used_waypoints_packed(&input);
    CHECK(input.enumerated_count == 2U && input.malformed == 0U);
    CHECK(get16(&plord_records[0], 22U) == 33U &&
        get16(&plord_records[0], 24U) == 44U &&
        get16(&fleet_records[0], 98U) == 3U && trace.deletes == 0);

    memset(fleet_table, 0, sizeof fleet_table);
    put_table_ref(fleet_table, 0, 2);
    put16(&fleet_records[248], 100U, (StarsU16)(FLEETS + 1));
    input.enumerated_count = 0U;
    input.malformed = 0U;
    stars_kill_used_waypoints_packed(&input);
    CHECK(input.enumerated_count == 0U && input.malformed == 1U);

    printf("KILL_USED_WAYPOINTS_PACKED_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures != 0;
#undef CHECK
}
