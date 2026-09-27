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
    StarsI16 opens;
    StarsI16 closes;
    StarsU16 last_type;
    StarsU16 last_flags;
} Trace;

static StarsI16 open_file(void *context, StarsU16 type, StarsI16 player,
    StarsU16 mode)
{
    Trace *trace = (Trace *)context;
    (void)type; (void)player; (void)mode;
    trace->opens++;
    return 1;
}

static void close_file(void *context)
{ ((Trace *)context)->closes++; }

static StarsI16 alert(void *context, StarsI16 id)
{ (void)context; (void)id; return 0; }

static void write_record(void *context, StarsU16 type, const StarsU8 *body,
    StarsU16 length)
{
    Trace *trace = (Trace *)context;
    trace->last_type = type;
    trace->last_flags = (length >= 2U) ?
        (StarsU16)(body[0] | ((StarsU16)body[1] << 8)) : 0U;
}

int main(void)
{
    StarsMarkFileInput input;
    StarsMarkFileState state;
    StarsMarkFileOps ops;
    Trace trace;
    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.open_file = open_file;
    ops.close_file = close_file;
    ops.alert = alert;
    ops.write_record = write_record;
    input.type = 2U;
    input.player = 1;
    input.mark_mode = 1U;
    input.value = 1;
    input.header_type = 8U;
    input.header_version = (StarsU16)((2U << 12) | (49U << 5));
    input.header_game_lid = 77UL;
    input.game_lid = 77UL;
    input.header_flags = 1U;
    if (!stars_mark_file(&input, &state, &ops, &trace) ||
            (state.header_flags & 0x0200U) == 0U || trace.last_type != 8U)
        return 1;

    memset(&input, 0, sizeof input);
    input.type = 0x2003U;
    input.player = 2;
    input.mark_mode = 8U;
    input.value = 1;
    input.header_type = 8U;
    input.header_version = (StarsU16)((2U << 12) | (50U << 5));
    input.header_flags = 2U;
    input.player_record_type = 6U;
    if (!stars_mark_file(&input, &state, &ops, &trace) ||
            (state.player_record_flags & 0xe200U) != 0xe200U ||
            trace.last_type != 6U || state.file_open == 0U)
        return 2;
    puts("MARK_FILE_SOURCE=PASS");
    return 0;
}
