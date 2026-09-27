#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 heap_count;
    StarsU16 failures;
    StarsDestroyGameState *state;
    StarsU16 windows[3];
    StarsU16 window_count;
    StarsU16 prompt_count;
    StarsU16 saved;
    StarsU16 reset_messages;
} Trace;

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
static void reset_heap(void *context, StarsU16 heap)
{
    Trace *trace = (Trace *)context;
    static const StarsU16 expected[] = { 3, 5, 10, 11, 6, 1, 7, 0, 8 };
    if (trace->heap_count >= 9U || heap != expected[trace->heap_count])
        ++trace->failures;
    if (trace->heap_count == 1U && trace->state->planet_count != 0U)
        ++trace->failures;
    if (trace->heap_count == 2U && trace->state->fleet_count != 0U)
        ++trace->failures;
    if (trace->heap_count == 3U && (trace->state->thing_count != 0U ||
            (trace->state->game_flags & 0x1400UL) ||
            (trace->state->game_flags2 & 0x0400U))) ++trace->failures;
    trace->heap_count = (StarsU16)(trace->heap_count + 1U);
}
static StarsI16 prompt_save(void *context)
{
    Trace *trace = (Trace *)context;
    trace->prompt_count = (StarsU16)(trace->prompt_count + 1U);
    return 1;
}
static void reset_messages(void *context)
{
    Trace *trace = (Trace *)context;
    trace->reset_messages = 1U;
    if (trace->state->player != -1 || trace->state->memory_log_current ||
            trace->state->memory_log_previous != -1 || trace->state->message_current ||
            trace->state->ai_data_present || trace->state->game_lid != 0x98765432UL)
        ++trace->failures;
}
static void destroy_window(void *context, StarsU16 hwnd)
{
    Trace *trace = (Trace *)context;
    if (trace->window_count < 3U)
        trace->windows[trace->window_count] = hwnd;
    trace->window_count = (StarsU16)(trace->window_count + 1U);
}
static void save_selection(void *context)
{ ((Trace *)context)->saved = 1U; }

int main(void)
{
    StarsDestroyGameOps ops;
    StarsDestroyGameState state;
    Trace trace;

    memset(&ops, 0, sizeof ops);
    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.state = &state;
    state.message_entry_open = 1U;
    state.player = 0;
    state.dirty = 1U;
    state.selected_kind = 1U;
    state.selected_id = 7;
    state.browser_window = 11U;
    state.report_window = 12U;
    state.popup_window = 13U;
    state.planet_count = 4U; state.fleet_count = 5U; state.thing_count = 6U;
    state.player_count = 2U; state.planet_capacity = 32U; state.turn = 17U;
    state.game_lid = 0x98765432UL; state.salt = 0x12345678UL;
    state.last_tick = 0xdeadbeefUL;
    state.game_flags = 0xffffffffUL; state.game_flags2 = 0xffffU;
    state.memory_log_current = 9; state.memory_log_previous = 8;
    state.message_current = 7; state.ai_data_present = 1U;
    state.selected_point.x = 100; state.selected_point.y = 200;
    state.design_table[3] = 99U;
    ops.reset_heap = reset_heap;
    ops.prompt_save = prompt_save;
    ops.reset_messages = reset_messages;
    ops.destroy_window = destroy_window;
    ops.save_selection = save_selection;
    stars_destroy_cur_game(&state, &ops, &trace);
    if (trace.heap_count != 9U || trace.prompt_count != 1U || !trace.saved)
        return 1;
    if (trace.window_count != 3U || trace.windows[0] != 11U ||
            trace.windows[1] != 12U || trace.windows[2] != 13U)
        return 2;
    if (!trace.reset_messages || state.player != -1 || state.planet_count != 0U ||
            state.design_table[3] != 0U || state.selected_id != -1 ||
            state.popup_window != 0U)
        return 3;
    if (trace.failures || state.player_count || state.planet_capacity || state.turn ||
            state.game_lid || state.salt || state.last_tick || state.ai_data_present ||
            state.memory_log_current || state.memory_log_previous != -1 ||
            state.message_current || state.message_entry_open ||
            state.selected_point.x || state.selected_point.y ||
            state.game_flags != 0xfffeEAffUL || state.game_flags2 != 0xfbffU)
        return 4;
    puts("DESTROY_CUR_GAME_SOURCE=PASS");
    return 0;
}
