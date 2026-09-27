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
    StarsI16 last_error;
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

static StarsI16 alert(void *context, StarsI16 error_id)
{
    Trace *trace = (Trace *)context;
    trace->last_error = error_id;
    return error_id == 0x15 ? 6 : 0;
}

int main(void)
{
    StarsOpenFileInput input;
    StarsOpenFileState state;
    StarsOpenFileOps ops;
    Trace trace;
    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.open_file = open_file;
    ops.close_file = close_file;
    ops.alert = alert;

    input.type = 1U;
    input.player = 2;
    input.game_turn = 0U;
    input.header_type = 8U;
    input.header_version = (StarsU16)((2U << 12) | (49U << 5));
    input.header_turn = 7U;
    input.header_flags = (StarsU16)(2U | (3U << 13));
    if (!stars_open_file(&input, &state, &ops, &trace) ||
            state.game_turn != 7U || ((state.game_flags >> 9) & 7U) != 3U)
        return 1;

    input.type = 2U;
    input.game_turn = 5U;
    input.header_turn = 5U;
    input.header_flags = (StarsU16)(2U | 0x0200U);
    input.gd_flags = 0U;
    if (!stars_open_file(&input, &state, &ops, &trace) ||
            trace.last_error != 0x15)
        return 2;

    input.header_type = 0U;
    if (stars_open_file(&input, &state, &ops, &trace) ||
            trace.last_error != 13 || trace.closes < 1)
        return 3;
    puts("OPEN_FILE_SOURCE=PASS");
    return 0;
}
