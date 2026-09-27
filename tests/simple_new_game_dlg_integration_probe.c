#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 selected;
    StarsI16 end_calls;
    StarsI16 create_calls;
    StarsI16 cancel_calls;
    StarsI16 sticky_calls;
    StarsI16 last_end;
    StarsI16 caller_density;
    StarsI16 caller_size;
    StarsI16 caller_turn;
    StarsI16 wizard_calls;
    StarsI16 load_calls;
    StarsI16 save_calls;
    StarsI16 focus_calls;
    StarsI16 deletes;
    StarsI16 adds;
    StarsI16 limits;
} Trace;

static StarsU16 item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context;
    (void)hwnd;
    (void)id;
    return 0x900U;
}

static StarsI16 checked(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context;
    (void)hwnd;
    return id == 202U || id == 1004U;
}

static void sticky(void *context, StarsU16 hwnd, StarsPoint *point,
    StarsI16 save)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    (void)point;
    (void)save;
    ++trace->sticky_calls;
}

static void end_dialog(void *context, StarsU16 hwnd, StarsI16 result)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    ++trace->end_calls;
    trace->last_end = result;
    if (result == 0x430) {
        ++trace->create_calls;
        trace->caller_density = trace->selected;
    } else if (result == 0xd4)
        ++trace->cancel_calls;
}

static StarsI16 selection(void *context, StarsU16 item_handle)
{
    Trace *trace = (Trace *)context;
    (void)item_handle;
    return trace->selected;
}

static StarsI16 wizard(void *context, StarsU16 hwnd)
{
    ++((Trace *)context)->wizard_calls;
    (void)hwnd;
    return 1;
}

static void load_new(void *context, StarsI16 index)
{
    ++((Trace *)context)->load_calls;
    (void)index;
}

static void save_new(void *context, StarsI16 index)
{
    ++((Trace *)context)->save_calls;
    (void)index;
}

static StarsI16 count(void *context, StarsU16 item_handle)
{
    (void)context;
    (void)item_handle;
    return 8;
}

static void message(void *context, StarsU16 hwnd, StarsU16 msg,
    StarsU16 wparam, StarsU32 lparam)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    (void)wparam;
    (void)lparam;
    if (msg == 0x404U)
        ++trace->deletes;
    else if (msg == 0x403U)
        ++trace->adds;
    else if (msg == 0x40eU)
        ++trace->limits;
}

static void focus(void *context, StarsU16 hwnd)
{
    ++((Trace *)context)->focus_calls;
    (void)hwnd;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsSimpleNewGameState state;
    StarsSimpleNewGameInput input;
    StarsNewGameDlgOps ops;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 old_density;
    StarsI16 old_size;
    StarsI16 old_turn;
    FILE *report;

    memset(&state, 0, sizeof state);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    state.density = 1;
    state.map_size = 1;
    state.turn = 2;
    ops.get_dialog_item = item;
    ops.is_button_checked = checked;
    ops.sticky_position = sticky;
    ops.end_dialog = end_dialog;
    ops.send_message = message;
    ops.context = &trace;
    input.state = &state;
    input.ops = &ops;
    input.race_wizard = wizard;
    input.load_new_race = load_new;
    input.save_new_race = save_new;
    input.list_count = count;
    input.get_selection = selection;
    input.set_focus = focus;
    input.context = &trace;

    checks = 0;
    failures = 0;
    trace.selected = 3;
    failures += check(stars_simple_new_game_dlg(&input, 10U, 273U,
        0x430U, 0L) == 1, &checks);
    failures += check(state.density == 2 && state.map_size == 4,
        &checks);
    failures += check(state.turn == 3 && trace.create_calls == 1 &&
        trace.last_end == 0x430, &checks);
    failures += check(trace.sticky_calls == 1, &checks);

    trace.selected = 7;
    failures += check(stars_simple_new_game_dlg(&input, 10U, 273U,
        0xd2U, 0L) == 0, &checks);
    failures += check(trace.wizard_calls == 1 && trace.load_calls == 1 &&
        trace.save_calls == 1 && trace.focus_calls == 1, &checks);
    failures += check(trace.deletes == 1 && trace.adds == 1 &&
        trace.limits == 1, &checks);
    failures += check(state.density == 2 && state.map_size == 4 &&
        state.turn == 7, &checks);

    old_density = state.density;
    old_size = state.map_size;
    old_turn = state.turn;
    failures += check(stars_simple_new_game_dlg(&input, 10U, 273U,
        0xd4U, 0L) == 1, &checks);
    failures += check(state.density == old_density &&
        state.map_size == old_size && state.turn == old_turn,
        &checks);
    failures += check(trace.cancel_calls == 1 && trace.last_end == 0xd4,
        &checks);
    failures += check(trace.end_calls == 2, &checks);

    report = fopen("C:\\SNGDI\\SNGDIE3.TXT", "w");
    if (report) {
        fprintf(report,
            "SIMPLE_NEW_GAME_DLG_INTEGRATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("SIMPLE_NEW_GAME_DLG_INTEGRATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
