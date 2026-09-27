#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsU16 item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd;
    return (StarsU16)(id + 0x100U);
}

static StarsI16 checked(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd;
    return id == 1002U || id == 1007U || id == 1010U ||
        id == 0x3f8U || id == 0x3faU || id == 0x3fdU || id == 0x41aU;
}

static void get_text(void *context, StarsU16 control, StarsU16 id,
    StarsU8 *text, StarsU16 capacity)
{
    (void)context; (void)control; (void)id;
    if (capacity >= 5U) {
        strcpy((char *)text, "NEWT");
    }
}

static StarsI16 race(void *context, StarsU16 wparam)
{
    (void)context; (void)wparam;
    return 1;
}

static void sticky(void *context, StarsU16 hwnd, StarsPoint *point,
    StarsI16 save)
{
    (void)context; (void)hwnd; (void)point; (void)save;
}

static void end_dialog(void *context, StarsU16 hwnd, StarsI16 result)
{
    (void)context; (void)hwnd; (void)result;
}

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static StarsU16 get16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] |
        ((StarsU16)raw[(StarsU16)(offset + 1U)] << 8));
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsNewGameDlgState state;
    StarsNewGameDlgOps ops;
    StarsNewGameDlgInput input;
    StarsU8 raw[64];
    StarsI16 checks;
    StarsI16 failures;
    FILE *report;

    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(raw, 0xa5, sizeof raw);
    state.map_size = 0;
    state.density = 0;
    state.start_distance = 0;
    state.options = 0;
    state.race_button_ids[0] = 100U;
    state.race_button_ids[1] = 101U;
    ops.get_dialog_item = item;
    ops.is_button_checked = checked;
    ops.get_dialog_text = get_text;
    ops.race_button_index = race;
    ops.sticky_position = sticky;
    ops.end_dialog = end_dialog;
    input.state = &state;
    input.packed_game_record = raw;
    input.ops = &ops;
    input.context = 0;
    put16(raw, 16U, 0xf000U);
    checks = 0;
    failures = 0;
    failures += check(stars_new_game_dlg(&input, 20U, 273U, 101U, 0L) == 1,
        &checks);
    failures += check(state.map_size == 2 && state.density == 2 &&
        state.start_distance == 1, &checks);
    failures += check(state.options == 0x0161U, &checks);
    failures += check(get16(raw, 4U) == 2U && get16(raw, 6U) == 2U &&
        get16(raw, 12U) == 1U, &checks);
    failures += check(get16(raw, 16U) == 0xf161U, &checks);
    failures += check(raw[32] == 'N' && raw[35] == 'T' && raw[36] == 0,
        &checks);
    failures += check(raw[0] == 0xa5U && raw[2] == 0xa5U &&
        raw[20] == 0xa5U, &checks);
    report = fopen("C:\\NGDP\\NGDPE1.TXT", "w");
    if (report) {
        fprintf(report,
            "NEW_GAME_DLG_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("NEW_GAME_DLG_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
