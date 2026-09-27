#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 selection;
    StarsI16 action_result;
    StarsI16 popup_calls;
    StarsI16 create_calls;
    StarsI16 open_calls;
    StarsI16 edit_calls;
    StarsI16 password_calls;
    StarsI16 password_ok;
} Trace;

static StarsI16 popup(void *context, StarsU16 hwnd, StarsU8 type,
    StarsI16 message, StarsI16 x, StarsI16 y)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)type; (void)message; (void)x; (void)y;
    ++trace->popup_calls;
    return trace->selection;
}

static StarsI16 create_race(void *context, StarsU16 hwnd, StarsI16 row,
    StarsU8 type)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)row; (void)type;
    ++trace->create_calls;
    return trace->action_result;
}

static StarsI16 open_game(void *context, StarsU16 hwnd, StarsI16 row,
    StarsU8 type)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)row; (void)type;
    ++trace->open_calls;
    return trace->action_result;
}

static StarsI16 edit_race(void *context, StarsU16 hwnd, StarsI16 row,
    StarsU8 type)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)row; (void)type;
    ++trace->edit_calls;
    return trace->action_result;
}

static StarsI16 password(void *context, StarsI16 row, StarsU8 type)
{
    Trace *trace = (Trace *)context;
    (void)row; (void)type;
    ++trace->password_calls;
    return trace->password_ok;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

static StarsI16 verify_slot(const StarsU8 *records, const StarsU8 *files,
    StarsI16 slot, const StarsU8 *record, const StarsU8 *file,
    StarsI16 *checks)
{
    StarsI16 failures;
    StarsI16 i;

    failures = 0;
    for (i = 0; i < 192; ++i)
        failures += check(records[slot * 192 + i] == record[i], checks);
    for (i = 0; i < 13; ++i)
        failures += check(files[slot * 13 + i] == file[i], checks);
    return failures;
}

int main(void)
{
    StarsNewGameDlg2Input input;
    StarsNewGameDlg2Ops ops;
    StarsU8 types[4];
    StarsU8 temporary[192];
    StarsU8 records[4 * 192];
    StarsU8 race_file[13];
    StarsU8 race_files[4 * 13];
    const StarsU16 buttons[4] = {100U, 101U, 102U, 103U};
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 i;
    StarsI16 slot;
    FILE *report;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    memset(types, 0, sizeof types);
    for (i = 0; i < 192; ++i)
        temporary[i] = (StarsU8)((i * 17 + 9) & 0xff);
    for (i = 0; i < 13; ++i)
        race_file[i] = (StarsU8)('A' + i);
    memset(records, 0xa5, sizeof records);
    memset(race_files, 0x5a, sizeof race_files);
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
    input.packed_player_record = temporary;
    input.packed_players = records;
    input.packed_player_stride = 192U;
    input.race_file = race_file;
    input.race_files = race_files;
    input.race_file_stride = 13U;
    input.ops = &ops;
    ops.track_popup = popup;
    ops.create_race = create_race;
    ops.open_game = open_game;
    ops.edit_race = edit_race;
    ops.check_password = password;
    ops.context = &trace;

    checks = 0;
    failures = 0;
    trace.selection = 0;
    trace.action_result = 3;
    slot = 2;
    failures += check(stars_new_game_dlg2(7U, 0x201U, 0,
        (StarsI32)(((StarsU32)27U << 16) | 12U), &input) == 0, &checks);
    failures += check(trace.create_calls == 1 && types[slot] == 14U,
        &checks);
    failures += verify_slot(records, race_files, slot, temporary, race_file,
        &checks);
    for (slot = 0; slot < 4; ++slot) {
        if (slot == 2)
            continue;
        for (i = 0; i < 192; ++i)
            failures += check(records[slot * 192 + i] == 0xa5, &checks);
        for (i = 0; i < 13; ++i)
            failures += check(race_files[slot * 13 + i] == 0x5a, &checks);
    }

    trace.selection = 1;
    trace.action_result = 2;
    slot = 3;
    failures += check(stars_new_game_dlg2(7U, 0x204U, 0,
        (StarsI32)(((StarsU32)37U << 16) | 12U), &input) == 0, &checks);
    failures += check(trace.open_calls == 1 && types[slot] == 10U,
        &checks);
    failures += verify_slot(records, race_files, slot, temporary, race_file,
        &checks);

    trace.selection = 2;
    trace.action_result = 7;
    trace.password_ok = 0;
    slot = 1;
    failures += check(stars_new_game_dlg2(7U, 0x201U, 0,
        (StarsI32)(((StarsU32)17U << 16) | 12U), &input) == 0, &checks);
    failures += check(trace.password_calls == 1 && trace.edit_calls == 0,
        &checks);
    trace.password_ok = 1;
    failures += check(stars_new_game_dlg2(7U, 0x201U, 0,
        (StarsI32)(((StarsU32)17U << 16) | 12U), &input) == 0, &checks);
    failures += check(trace.edit_calls == 1 && types[slot] == 7U,
        &checks);
    failures += verify_slot(records, race_files, slot, temporary, race_file,
        &checks);

    report = fopen("C:\\ND2PACK\\NWD2PE1.TXT", "w");
    if (report) {
        fprintf(report,
            "NEW_GAME_DLG2_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("NEW_GAME_DLG2_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
