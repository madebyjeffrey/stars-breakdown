#include <stdio.h>
#include "core.h"

typedef struct Trace {
    StarsI16 popups;
    StarsI16 invalidations;
    StarsI16 tutor_steps;
    StarsI16 commands;
    StarsU16 last_kind;
    StarsU16 last_window;
    StarsI16 next_selection;
    StarsU16 last_command;
} Trace;

static StarsI16 popup(void *context, StarsU16 kind, StarsI16 x, StarsI16 y,
    StarsU16 current, StarsU16 available, StarsI16 selected)
{
    Trace *trace = (Trace *)context;
    (void)x; (void)y; (void)current; (void)available; (void)selected;
    ++trace->popups; trace->last_kind = kind;
    return trace->next_selection;
}
static void cursor(void *context, StarsU16 window, StarsPoint *point)
{ Trace *trace = (Trace *)context; point->x = 12; point->y = 14; trace->last_window = window; }
static void invalidate(void *context, StarsU16 window)
{ Trace *trace = (Trace *)context; (void)window; ++trace->invalidations; }
static void command(void *context, StarsU16 id)
{ Trace *trace = (Trace *)context; ++trace->commands; trace->last_command = id; }
static void tutor(void *context)
{ ++((Trace *)context)->tutor_steps; }

static void initialize(StarsExecuteButtonInput *input, Trace *trace,
    const StarsU16 *ship_flags)
{
    input->game_bits = 0x0800U;
    input->game_bits2 = 0U;
    input->grbit_scan = 0U;
    input->grbit_scan_mines = 0U;
    input->grbit_scan_ship = 0U;
    input->grbit_scan_enemy_ship = 0U;
    input->scan_zoom = 0;
    input->toolbar_window = 10U;
    input->frame_window = 11U;
    input->scanner_window = 12U;
    input->ship_definition_flags = ship_flags;
    input->ship_definition_count = 3U;
    input->ops.popup_menu = popup;
    input->ops.get_cursor_pos = cursor;
    input->ops.screen_to_client = cursor;
    input->ops.invalidate = invalidate;
    input->ops.command_handler = command;
    input->ops.advance_tutor = tutor;
    input->context = trace;
    trace->popups = trace->invalidations = trace->tutor_steps = 0;
    trace->commands = 0; trace->last_kind = trace->last_window = 0;
    trace->next_selection = -1; trace->last_command = 0;
}

int main(void)
{
    StarsExecuteButtonInput input;
    StarsU16 ship_flags[3] = { 0U, 0x0200U, 0U };
    Trace trace;
    int failures = 0;

    initialize(&input, &trace, ship_flags);
    stars_execute_button(&input, 2, 0);
    if (input.grbit_scan != 0U || trace.invalidations != 0 ||
            input.game_bits2 != 0x0040U) ++failures;
    stars_execute_button(&input, 2, 1);
    if ((input.grbit_scan & 0x000fU) != 2U || trace.invalidations != 1 ||
            trace.tutor_steps != 1) ++failures;

    trace.next_selection = 3;
    stars_execute_button(&input, 8, 1);
    if (trace.popups != 1 || trace.last_kind != 8U ||
            (input.grbit_scan_mines & 1U) == 0U ||
            (input.grbit_scan & 0x0040U) == 0U || trace.popups != 1) ++failures;

    trace.next_selection = 4;
    stars_execute_button(&input, 13, 1);
    if (trace.popups != 2 || input.grbit_scan_ship != 1U ||
            (input.grbit_scan & 0x0200U) == 0U) ++failures;

    trace.next_selection = 2;
    stars_execute_button(&input, 16, 1);
    if (trace.popups != 3 || trace.last_command != 0x0f3fU ||
            trace.commands != 1) ++failures;

#ifndef TARGET_NATIVE
    printf("EXECUTE_BUTTON_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
