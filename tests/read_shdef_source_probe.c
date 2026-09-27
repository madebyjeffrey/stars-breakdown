#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 cost_updates;
    StarsU16 decoded;
} Trace;

void stars_stream_open(StarsU8 *data, StarsU16 name, StarsI16 mode,
    const StarsStreamApi *api, void *context)
{
    (void)data; (void)name; (void)mode; (void)api; (void)context;
}
void stars_stream_write(StarsU8 *data, const StarsU8 *input, StarsU16 count,
    const StarsStreamApi *api, void *context)
{
    (void)data; (void)input; (void)count; (void)api; (void)context;
}
void stars_write_bof(StarsU8 *data, StarsI16 player, StarsI16 type,
    StarsI16 multi, StarsTickCount ticks, StarsWriteTransfer write,
    void *context)
{
    (void)data; (void)player; (void)type; (void)multi; (void)ticks;
    (void)write; (void)context;
}

static StarsU16 lookup_hull(void *context, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    (void)context;
    if (id != 5U || capacity < 143U) return 0U;
    memset(out, 0, capacity);
    out[40] = 10U;
    out[58] = 1U;
    out[61] = 2U;
    out[50] = 3U;
    return 1U;
}

static StarsI16 lookup_part(void *context, StarsU16 group, StarsU16 item,
    StarsU8 *out, StarsU16 capacity)
{
    (void)context;
    if (group != 1U || item != 2U || capacity < 8U) return 0;
    memset(out, 0, capacity);
    out[4] = 4U;
    return 1;
}

static void update_cost(void *context, StarsU8 *design)
{
    Trace *trace = (Trace *)context;
    (void)design;
    trace->cost_updates = (StarsU16)(trace->cost_updates + 1U);
}

static StarsI16 decode_string(void *context, const StarsU8 *input,
    StarsU16 length, StarsU8 *output, StarsU16 *output_length)
{
    Trace *trace = (Trace *)context;
    StarsU16 n = length < 3U ? length : 3U;
    if (*output_length < n) return 0;
    memcpy(output, input, n);
    *output_length = n;
    trace->decoded = n;
    return 1;
}

int main(void)
{
    StarsReadShdefOps ops;
    StarsU8 record[25];
    StarsU8 designs[16 * 147];
    Trace trace;

    memset(&ops, 0, sizeof ops);
    memset(record, 0, sizeof record);
    memset(designs, 0, sizeof designs);
    memset(&trace, 0, sizeof trace);
    record[0] = 7U;
    record[2] = 5U;
    record[3] = 0U;
    record[6] = 1U;
    record[17] = 1U;
    record[18] = 0U;
    record[19] = 2U;
    record[20] = 2U;
    record[21] = 3U;
    record[22] = 'A';
    record[23] = 'B';
    record[24] = 'C';
    ops.lookup_hull = lookup_hull;
    ops.lookup_part = lookup_part;
    ops.update_cost = update_cost;
    ops.decode_string = decode_string;
    if (!stars_read_shdef(record, sizeof record, designs, sizeof designs,
            0, &ops, &trace)) return 1;
    if (trace.cost_updates != 1U || trace.decoded != 3U) return 2;
    if (designs[40] != 18U || (designs[123] & 7U) != 7U ||
            designs[122] != 1U || designs[58] != 1U ||
            designs[59] != 0U || designs[60] != 2U ||
            designs[61] != 2U) return 3;
    puts("READ_SHDEF_SOURCE=PASS");
    return 0;
}
