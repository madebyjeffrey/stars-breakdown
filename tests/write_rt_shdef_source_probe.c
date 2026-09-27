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

typedef struct Trace { StarsU16 type; StarsU16 length; } Trace;

static StarsI16 compress_name(void *context, const StarsU8 *input,
    StarsU8 *output, StarsI16 *capacity)
{
    (void)context; (void)input;
    if (*capacity < 2) return 0;
    output[0] = 0xa1U; output[1] = 0xb2U;
    *capacity = 2;
    return 1;
}

static void write_record(void *context, StarsU16 type, StarsU16 length,
    const StarsU8 *record)
{
    Trace *trace = (Trace *)context;
    (void)record;
    trace->type = type;
    trace->length = length;
}

int main(void)
{
    static const StarsU8 parts[] = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U };
    static const StarsU8 name[] = "Scout";
    StarsWriteShdefInput input;
    StarsWriteShdefOps ops;
    StarsU8 store[128];
    StarsU16 stored;
    Trace trace;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(store, 0, sizeof store);
    memset(&trace, 0, sizeof trace);
    input.flags = 7U;
    input.hull_id = 0x1234U;
    input.bitmap = 2U;
    input.component_count = 2U;
    input.design_points = 77U;
    input.turn = 9U;
    input.built = 0x01020304UL;
    input.existing = 0x05060708UL;
    input.component_data = parts;
    input.component_data_size = (StarsU16)sizeof parts;
    input.name = name;
    ops.compress_name = compress_name;
    ops.write_record = write_record;
    if (!stars_write_rt_shdef(&input, store, (StarsU16)sizeof store,
            &stored, &ops, &trace)) return 1;
    if (stored != 28U || store[0] != 7U || store[2] != 0x34U ||
            store[3] != 2U || store[4] != 77U || store[6] != 2U || store[7] != 9U || store[9] != 4U ||
            store[13] != 8U || store[17] != 1U ||
            store[24] != 8U || store[25] != 2U || store[26] != 0xa1U)
        return 2;
    input.flags = 0U;
    input.component_count = 0U;
    input.name = (const StarsU8 *)"";
    if (stars_write_rt_shdef(&input, (StarsU8 *)0, 0U, (StarsU16 *)0,
            &ops, &trace) == 0 || trace.type != 0x1aU)
        return 3;
    puts("WRITE_RT_SHDEF_SOURCE=PASS");
    return 0;
}
