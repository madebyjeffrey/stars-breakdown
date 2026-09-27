#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsI16 dialog(void *context, StarsU16 stage, StarsU16 hwnd,
    StarsI16 read_only, StarsI16 step)
{
    StarsI16 *calls;
    (void)stage; (void)hwnd; (void)read_only; (void)step;
    calls = (StarsI16 *)context;
    if (*calls == 0) { ++*calls; return 0xd3; }
    if (*calls == 1) { ++*calls; return 2; }
    ++*calls;
    return 3;
}

static StarsI16 name(void *context, StarsU8 *value, StarsU16 capacity)
{
    (void)context;
    if (capacity < 6) return 0;
    strcpy((char *)value, "ALPHA");
    return 1;
}

static StarsI16 advantage(void *context, StarsI16 player)
{
    (void)context; (void)player;
    return 0;
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
    StarsNewGameWizardInput input;
    StarsNewGameWizardOps ops;
    StarsNewGameWizardPlayer players[16];
    StarsU8 types[16];
    StarsU8 packed[384];
    StarsI16 dialog_calls;
    StarsI16 checks;
    StarsI16 failures;
    StarsU16 flags;
    FILE *report;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(players, 0, sizeof players);
    memset(types, 0, sizeof types);
    memset(packed, 0xa5, sizeof packed);
    dialog_calls = 0;
    checks = 0;
    failures = 0;
    types[0] = 29;
    types[1] = (StarsU8)(3U | (5U << 2) | (3U << 5));
    players[0].packed_flags_valid = 1;
    players[0].packed_flags = 1;
    players[0].salt = 0x11223344L;
    players[0].name[0] = 'R';
    players[0].display_name[0] = 'r';
    players[1].packed_flags_valid = 1;
    players[1].packed_flags = 0x1234U;
    players[1].salt = 0x55667788L;
    put16(packed, 6, 0x0001U);
    put16(packed, 84, 0x8000U);
    put16(packed, 192U + 6U, 0x1234U);
    put16(packed, 192U + 84U, 0x4000U);
    ops.dialog = dialog;
    ops.get_game_name = name;
    ops.advantage_points = advantage;
    ops.context = &dialog_calls;
    input.players = players;
    input.player_types = types;
    input.packed_players = packed;
    input.packed_player_stride = 192;
    input.host_player = 0;
    input.ops = &ops;

    failures += check(stars_new_game_wizard(&input) == 1, &checks);
    failures += check(dialog_calls == 3, &checks);
    failures += check(get16(packed, 6) == 0xe201U, &checks);
    failures += check(get16(packed, 192U + 6U) == 0xae34U, &checks);
    failures += check((get16(packed, 84) & 0x8000U) != 0U, &checks);
    failures += check((get16(packed, 192U + 84U) & 0x4000U) != 0U, &checks);
    failures += check(packed[12] == 0xffU && packed[15] == 0xffU, &checks);
    failures += check(packed[192U + 12U] == 0x88U &&
        packed[192U + 15U] == 0x55U, &checks);
    failures += check(packed[128] ==  'R' && packed[160] == 'r', &checks);
    failures += check(packed[192U + 0U] == 0xa5U &&
        packed[192U + 1U] == 0xa5U, &checks);
    flags = get16(packed, 6);
    failures += check((flags & 0x00f8U) == 0U, &checks);
    report = fopen("C:\\NWP\\NWPE1.TXT", "w");
    if (report) {
        fprintf(report,
            "NEW_GAME_WIZARD_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("NEW_GAME_WIZARD_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
