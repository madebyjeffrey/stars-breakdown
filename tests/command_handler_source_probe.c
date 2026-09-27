#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 last;
    StarsU16 count;
    StarsU16 dialogs;
    StarsU16 reports;
    StarsU16 zoom;
} Trace;

static void mark(Trace *trace, StarsU16 value)
{
    trace->last = value;
    trace->count = (StarsU16)(trace->count + 1U);
}

static void menu(void *context, StarsU16 value) { mark((Trace *)context, value); }
static void dump(void *context, StarsU16 value) { mark((Trace *)context, (StarsU16)(0x100U + value)); }
static void waypoint(void *context, StarsU16 value) { mark((Trace *)context, (StarsU16)(0x200U + value)); }
static StarsI16 game(void *context, StarsU16 hwnd, StarsU16 race_only)
{
    (void)hwnd;
    mark((Trace *)context, (StarsU16)(0x300U + race_only));
    return 1;
}
static void write_ini(void *context) { mark((Trace *)context, 0x400U); }
static void destroy_game(void *context) { mark((Trace *)context, 0x401U); }
static void initialize_menu(void *context) { mark((Trace *)context, 0x402U); }
static void close_frame(void *context, StarsU16 hwnd)
{
    (void)hwnd;
    mark((Trace *)context, 0x403U);
}
static void refit(void *context) { mark((Trace *)context, 0x404U); }
static void ensure_tile(void *context, StarsU16 value)
{
    mark((Trace *)context, (StarsU16)(0x410U + value));
}
static void invalidate(void *context, StarsU16 value)
{
    mark((Trace *)context, (StarsU16)(0x420U + value));
}
static StarsI16 dialog(void *context, StarsU16 value, StarsU16 hwnd)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    trace->dialogs = (StarsU16)(trace->dialogs + 1U);
    mark(trace, (StarsU16)(0x500U + value));
    return 1;
}
static void report(void *context, StarsU16 value)
{
    Trace *trace = (Trace *)context;
    trace->reports = (StarsU16)(trace->reports + 1U);
    mark(trace, value);
}
static void set_zoom(void *context, StarsU16 value, StarsU16 command)
{
    Trace *trace = (Trace *)context;
    trace->zoom = value;
    mark(trace, command);
}

int main(void)
{
    StarsCommandOps ops;
    StarsCommandState state;
    Trace trace;

    memset(&ops, 0, sizeof(ops));
    memset(&state, 0, sizeof(state));
    memset(&trace, 0, sizeof(trace));
    state.player = 0;
    state.game_present = 1U;
    state.scanner_window = 1U;
    state.title_window = 7U;
    state.new_turn_available = 1U;
    ops.set_menu_selection = menu;
    ops.dump = dump;
    ops.delete_waypoint = waypoint;
    ops.open_game = game;
    ops.new_game = game;
    ops.write_ini = write_ini;
    ops.destroy_game = destroy_game;
    ops.initialize_menu = initialize_menu;
    ops.close_frame = close_frame;
    ops.refit = refit;
    ops.ensure_tile_size = ensure_tile;
    ops.invalidate_frame = invalidate;
    ops.dialog = dialog;
    ops.select_report = report;
    ops.set_scan_zoom = set_zoom;

    stars_mdi_command_handler(&state, 9U, 15001U, &ops, &trace);
    if (trace.last != (StarsU16)(15001U + 0xc568U)) return 1;
    stars_mdi_command_handler(&state, 9U, 0x54U, &ops, &trace);
    if (trace.last != 0x101U) return 2;
    state.selected_object_kind = 2U;
    stars_mdi_command_handler(&state, 9U, 0x67U, &ops, &trace);
    if (trace.last != 0x201U) return 3;
    stars_mdi_command_handler(&state, 9U, 0x82U, &ops, &trace);
    if (state.layout != 0U || trace.last != 0x404U) return 4;
    stars_mdi_command_handler(&state, 9U, 0x5fU, &ops, &trace);
    if (trace.dialogs != 1U || trace.last != 0x566U) return 5;
    stars_mdi_command_handler(&state, 9U, 0x8ffU, &ops, &trace);
    if (state.report_kind != 2U || trace.reports != 1U) return 6;
    stars_mdi_command_handler(&state, 9U, 0xf45U, &ops, &trace);
    if (trace.zoom != 4U) return 7;
    stars_mdi_command_handler(&state, 9U, 0x71U, &ops, &trace);
    if (state.game_present != 0U || state.player != -1) return 8;
    puts("COMMAND_HANDLER_SOURCE=PASS");
    return 0;
}
