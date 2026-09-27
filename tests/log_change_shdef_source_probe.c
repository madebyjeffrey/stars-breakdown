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

typedef struct Trace {
    StarsU16 type;
    StarsU16 length;
    StarsU8 record[160];
    StarsU16 tutor_steps;
} Trace;

static void write_memory(void *context, StarsU16 type, StarsU16 length,
    const StarsU8 *record)
{
    Trace *trace = (Trace *)context;
    trace->type = type;
    trace->length = length;
    memcpy(trace->record, record, length);
}

static StarsU16 write_design(void *context, const StarsU8 *input,
    StarsU8 *output, StarsU16 capacity)
{
    (void)context;
    if (capacity < 3U) return 0U;
    output[0] = input[0];
    output[1] = input[1];
    output[2] = input[2];
    return 3U;
}

static void advance_tutor(void *context)
{ ((Trace *)context)->tutor_steps++; }

int main(void)
{
    static const StarsU8 payload[] = { 7U, 8U, 9U };
    StarsLogChangeShdefInput input;
    StarsLogChangeShdefOps ops;
    Trace trace;
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    input.player = 2;
    input.design_flags = 0x1400U;
    input.tutor_flags = 0x0800U;
    input.design_payload = payload;
    ops.write_design = write_design;
    ops.write_memory = write_memory;
    ops.advance_tutor = advance_tutor;
    if (!stars_log_change_shdef(&input, &ops, &trace) ||
            trace.type != 0x1bU || trace.length != 5U ||
            trace.record[0] != 0x21U || trace.record[1] != 0x05U ||
            trace.record[2] != 7U || trace.record[4] != 9U ||
            trace.tutor_steps != 0U) return 1;

    memset(&trace, 0, sizeof trace);
    input.player = 0;
    input.design_flags = 0x0200U;
    if (!stars_log_change_shdef(&input, &ops, &trace) ||
            trace.type != 0x1bU || trace.length != 2U ||
            trace.record[0] != 0U || trace.record[1] != 0U ||
            trace.tutor_steps != 1U) return 2;

    memset(&trace, 0, sizeof trace);
    input.game_flags = 2U;
    if (!stars_log_change_shdef(&input, &ops, &trace) || trace.length != 0U)
        return 3;
    puts("LOG_CHANGE_SHDEF_SOURCE=PASS");
    return 0;
}
