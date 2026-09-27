#include <stdio.h>
#include <string.h>
#include "core.h"

void stars_stream_open(StarsU8 *data, StarsU16 name, StarsI16 mode,
    const StarsStreamApi *api, void *context)
{ (void)data; (void)name; (void)mode; (void)api; (void)context; }
void stars_stream_write(StarsU8 *data, const StarsU8 *input, StarsU16 count,
    const StarsStreamApi *api, void *context)
{ (void)data; (void)input; (void)count; (void)api; (void)context; }
void stars_write_bof(StarsU8 *data, StarsI16 player, StarsI16 type,
    StarsI16 multi, StarsTickCount ticks, StarsWriteTransfer write,
    void *context)
{
    (void)data; (void)player; (void)type; (void)multi; (void)ticks;
    (void)write; (void)context;
}

typedef struct TraceRecord {
    StarsU16 type;
    StarsU16 length;
    StarsU8 data[96];
} TraceRecord;
typedef struct Trace {
    TraceRecord records[8];
    StarsU16 count;
    char name[16];
} Trace;

static void write_record(void *context, StarsU16 type, StarsU16 length,
    const StarsU8 *record)
{
    Trace *trace = (Trace *)context;
    TraceRecord *entry = &trace->records[trace->count++];
    entry->type = type;
    entry->length = length;
    if (length > (StarsU16)sizeof entry->data)
        length = (StarsU16)sizeof entry->data;
    memcpy(entry->data, record, length);
}

static void write_name(void *context, const StarsU8 *name)
{
    Trace *trace = (Trace *)context;
    strcpy(trace->name, (const char *)name);
}

static StarsU16 word_at(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] |
        ((StarsU16)data[offset + 1U] << 8));
}

static StarsU32 long_at(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU32)word_at(data, offset) |
        ((StarsU32)word_at(data, (StarsU16)(offset + 2U)) << 16);
}

int main(void)
{
    static const StarsU32 weights[16] = { 10UL, 0UL, 20UL };
    static const StarsU8 orders[36] = {
        0, 0, 0, 0, 0, 0, 0,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
        0, 0, 0, 0, 0, 0, 1,
        12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22
    };
    StarsWriteFleetInput input;
    StarsWriteFleetOps ops;
    StarsU8 record[96];
    StarsU16 length;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(record, 0, sizeof record);
    memset(&trace, 0, sizeof trace);
    input.fleet_flags = 4U;
    input.ship_counts[0] = 2U;
    input.ship_counts[2] = 300U;
    input.minerals[0] = 5UL;
    input.minerals[1] = 0x100UL;
    input.minerals[2] = 0x10000UL;
    input.overlap_116[0] = 0x11223344UL;
    input.overlap_116[1] = 0x55667788UL;
    input.ship_weights = weights;
    ops.write_record = write_record;
    if (!stars_write_fleet(&input, record, (StarsU16)sizeof record,
            &length, &ops, &trace) || trace.count != 1U ||
            trace.records[0].type != 0x11U || length != 29U ||
            word_at(record, 0U) != 4U || word_at(record, 2U) != 5U ||
            word_at(record, 4U) != 2U || word_at(record, 6U) != 300U ||
            word_at(record, 8U) != 0x39U || record[10] != 5U ||
            word_at(record, 11U) != 0x100U ||
            long_at(record, 13U) != 0x10000UL ||
            long_at(record, 17U) != 0x11223344UL ||
            long_at(record, 21U) != 0x55667788UL ||
            long_at(record, 25U) != 71817UL)
        return 1;

    memset(&input, 0, sizeof input);
    memset(record, 0, sizeof record);
    memset(&trace, 0, sizeof trace);
    input.fleet_flags = 7U;
    input.ship_counts[0] = 4U;
    input.ship_counts[16] = 9U;
    input.ship_counts[18] = 300U;
    input.design_plan = 2U;
    input.cord = 3U;
    input.orders = orders;
    input.order_count = 2U;
    input.name = (const StarsU8 *)"Fleet";
    ops.write_name = write_name;
    if (!stars_write_fleet(&input, record, (StarsU16)sizeof record,
            &length, &ops, &trace) || trace.count != 3U || length != 15U ||
            trace.records[0].type != 0x10U || word_at(record, 0U) != 0x0807U ||
            record[4] != 4U || word_at(record, 5U) != 0U ||
            word_at(record, 7U) != 5U || word_at(record, 9U) != 9U ||
            word_at(record, 11U) != 300U || record[13] != 2U ||
            record[14] != 3U ||
            trace.records[1].type != 0x14U || trace.records[1].length != 8U ||
            trace.records[2].type != 0x13U || trace.records[2].length != 18U ||
            strcmp(trace.name, "Fleet") != 0)
        return 2;
    puts("WRITE_FLEET_SOURCE=PASS");
    return 0;
}
