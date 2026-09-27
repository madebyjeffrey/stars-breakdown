#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 end_calls;
    StarsI16 last_end;
    StarsI16 dispatched_stage;
    StarsI16 alert_calls;
    StarsI16 sticky_calls;
    StarsI16 popup_calls;
    StarsI16 create_calls;
} Trace;

static StarsI16 popup(void *context, StarsU16 hwnd, StarsU8 type,
    StarsI16 message, StarsI16 x, StarsI16 y)
{
    (void)hwnd;
    (void)type;
    (void)message;
    (void)x;
    (void)y;
    ++((Trace *)context)->popup_calls;
    return 0;
}

static StarsI16 create_race(void *context, StarsU16 hwnd, StarsI16 row,
    StarsU8 type)
{
    (void)hwnd;
    (void)row;
    (void)type;
    ++((Trace *)context)->create_calls;
    return 2;
}

static void alert(void *context, StarsI16 string_id, StarsI16 level)
{
    (void)string_id;
    (void)level;
    ++((Trace *)context)->alert_calls;
}

static void sticky(void *context, StarsU16 hwnd, StarsI16 save)
{
    (void)hwnd;
    (void)save;
    ++((Trace *)context)->sticky_calls;
}

static void end_dialog(void *context, StarsU16 hwnd, StarsI16 result)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    ++trace->end_calls;
    trace->last_end = result;
    /* The caller maps the second-page result to the next wizard stage. */
    trace->dispatched_stage = (StarsI16)(result + 2);
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsNewGameDlg2Input input;
    StarsNewGameDlg2Ops ops;
    StarsU8 types[4];
    const StarsU16 buttons[4] = {100U, 101U, 102U, 103U};
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    FILE *report;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    memset(types, 0, sizeof types);
    types[0] = 1;
    input.x_diamond = 10;
    input.diamond_width = 5;
    input.row_top = 6;
    input.row_height = 10;
    input.row_height_active = 9;
    input.player_types = types;
    input.player_type_count = 4;
    input.race_button_ids = buttons;
    input.race_button_count = 4;
    input.ops = &ops;
    ops.track_popup = popup;
    ops.create_race = create_race;
    ops.alert = alert;
    ops.sticky_position = sticky;
    ops.end_dialog = end_dialog;
    ops.context = &trace;

    checks = 0;
    failures = 0;
    failures += check(stars_new_game_dlg2(7U, 0x111U, 101U, 0L,
        &input) == 1, &checks);
    failures += check(trace.end_calls == 1 && trace.last_end == 1 &&
        trace.dispatched_stage == 3, &checks);
    failures += check(trace.sticky_calls == 1, &checks);

    memset(types, 0, sizeof types);
    failures += check(stars_new_game_dlg2(7U, 0x111U, 101U, 0L,
        &input) == 0, &checks);
    failures += check(trace.alert_calls == 1 && trace.end_calls == 1,
        &checks);

    input.read_only = 1;
    failures += check(stars_new_game_dlg2(7U, 0x111U, 102U, 0L,
        &input) == 1 && trace.end_calls == 2 && trace.last_end == 2 &&
        trace.dispatched_stage == 4, &checks);
    input.read_only = 0;
    failures += check(stars_new_game_dlg2(7U, 0x201U, 0U,
        (StarsI32)(((StarsU32)7U << 16) | 12U), &input) == 0,
        &checks);
    failures += check(trace.popup_calls == 1 && trace.create_calls == 1 &&
        types[0] == 10U, &checks);
    failures += check(stars_new_game_dlg2(7U, 0x111U, 100U, 0L,
        &input) == 1 && trace.last_end == 0, &checks);

    report = fopen("C:\\ND2INT\\NWD2IE3.TXT", "w");
    if (report) {
        fprintf(report,
            "NEW_GAME_DLG2_INTEGRATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("NEW_GAME_DLG2_INTEGRATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
