#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 wizard_calls;
    StarsI16 save_calls;
    StarsI16 focus_calls;
    StarsI16 load_calls;
    StarsI16 deletes;
    StarsU16 list_item;
} Trace;

static StarsU16 item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd; (void)id;
    return 0x900U;
}

static void message(void *context, StarsU16 hwnd, StarsU16 msg,
    StarsU16 wparam, StarsU32 lparam)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)lparam;
    if (msg == 0x404U)
        ++trace->deletes;
    if (msg == 0x407U)
        trace->list_item = wparam;
}

static StarsI16 selection(void *context, StarsU16 item_handle)
{
    (void)context; (void)item_handle;
    return 7;
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

static void focus(void *context, StarsU16 hwnd)
{
    ++((Trace *)context)->focus_calls;
    (void)hwnd;
}

static StarsI16 count(void *context, StarsU16 item_handle)
{
    (void)context; (void)item_handle;
    return 8;
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
    StarsU8 temporary[192];
    StarsU8 records[16 * 192];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 i;
    StarsI16 player;
    StarsU8 expected;
    FILE *report;

    memset(&state, 0, sizeof state);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    for (i = 0; i < 192; ++i)
        temporary[i] = (StarsU8)((i * 13 + 7) & 0xff);
    memset(records, 0xa5, sizeof records);
    ops.get_dialog_item = item;
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
    input.packed_player_record = temporary;
    input.packed_player_records = records;
    input.packed_player_stride = 192U;
    input.context = &trace;

    checks = 0;
    failures = 0;
    failures += check(stars_simple_new_game_dlg(&input, 10U, 273U,
        0xd2U, 0L) == 0, &checks);
    failures += check(trace.wizard_calls == 1 && trace.save_calls == 1 &&
        trace.load_calls == 1 && trace.focus_calls == 1 &&
        trace.deletes == 1, &checks);
    for (player = 0; player < 16; ++player) {
        for (i = 0; i < 192; ++i) {
            expected = player == 7 ? temporary[i] : (StarsU8)0xa5;
            failures += check(records[player * 192 + i] == expected, &checks);
        }
    }
    report = fopen("C:\\SNGDP\\SNGDPE1.TXT", "w");
    if (report) {
        fprintf(report,
            "SIMPLE_NEW_GAME_DLG_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("SIMPLE_NEW_GAME_DLG_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
