#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 dialog_calls;
    StarsI16 stages[3];
    StarsI16 steps[3];
    StarsI16 end_calls;
    StarsI16 end_result;
    StarsI16 name_calls;
    StarsI16 save_calls;
    StarsI16 close_calls;
    StarsI16 generate_calls;
    StarsI16 host_calls;
    StarsI16 bits_calls;
    StarsU16 last_bits;
    StarsU8 *game_record;
} Trace;

static StarsU16 item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd;
    return (StarsU16)(id + 0x100U);
}

static StarsI16 checked(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd;
    return id == 1002U || id == 1006U || id == 1011U ||
        id == 0x3f8U || id == 0x3fbU || id == 0x3fcU;
}

static void get_text(void *context, StarsU16 control, StarsU16 id,
    StarsU8 *text, StarsU16 capacity)
{
    (void)context; (void)control; (void)id;
    if (capacity >= 8U)
        strcpy((char *)text, "GALAXY");
}

static StarsI16 race_button(void *context, StarsU16 wparam)
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
    Trace *trace = (Trace *)context;
    (void)hwnd;
    ++trace->end_calls;
    trace->end_result = result;
}

static StarsI16 wizard_dialog(void *context, StarsU16 stage, StarsU16 hwnd,
    StarsI16 read_only, StarsI16 step)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)read_only;
    if (trace->dialog_calls < 3) {
        trace->stages[trace->dialog_calls] = (StarsI16)stage;
        trace->steps[trace->dialog_calls] = step;
    }
    ++trace->dialog_calls;
    if (stage == 0U)
        return 0xd3;
    if (stage == 1U)
        return 2;
    return 3;
}

static StarsI16 game_name(void *context, StarsU8 *name, StarsU16 capacity)
{
    Trace *trace = (Trace *)context;
    ++trace->name_calls;
    if (capacity < 8U)
        return 0;
    strcpy((char *)name, "NEWGAME");
    return 1;
}

static void save_restore(void *context)
{
    ++((Trace *)context)->save_calls;
}

static void close_title(void *context)
{
    ++((Trace *)context)->close_calls;
}

static void generate_world(void *context)
{
    Trace *trace = (Trace *)context;
    ++trace->generate_calls;
    if (trace->game_record[32] != 'G' || trace->game_record[37] != 'Y')
        trace->generate_calls = -100;
}

static void bring_up_host(void *context)
{
    ++((Trace *)context)->host_calls;
}

static void set_bits(void *context, StarsU16 bits)
{
    Trace *trace = (Trace *)context;
    ++trace->bits_calls;
    trace->last_bits = bits;
}

static void no_player_name(void *context, StarsI16 index)
{
    (void)context; (void)index;
}

static StarsI16 advantage(void *context, StarsI16 index)
{
    (void)context; (void)index;
    return 0;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsNewGameDlgState game_state;
    StarsNewGameDlgOps game_ops;
    StarsNewGameDlgInput game_input;
    StarsNewGameWizardOps wizard_ops;
    StarsNewGameWizardInput wizard_input;
    StarsNewGameWizardPlayer players[16];
    StarsU8 player_types[16];
    StarsU8 game_record[64];
    StarsU8 player_record[192];
    StarsU16 bits;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    FILE *report;

    memset(&game_state, 0, sizeof game_state);
    memset(&game_ops, 0, sizeof game_ops);
    memset(&game_input, 0, sizeof game_input);
    memset(&wizard_ops, 0, sizeof wizard_ops);
    memset(&wizard_input, 0, sizeof wizard_input);
    memset(players, 0, sizeof players);
    memset(player_types, 0, sizeof player_types);
    memset(game_record, 0xa5, sizeof game_record);
    memset(player_record, 0xa5, sizeof player_record);
    memset(&trace, 0, sizeof trace);
    bits = 0x1234U;
    trace.game_record = game_record;

    game_state.race_button_ids[0] = 100U;
    game_state.race_button_ids[1] = 101U;
    game_ops.get_dialog_item = item;
    game_ops.is_button_checked = checked;
    game_ops.get_dialog_text = get_text;
    game_ops.race_button_index = race_button;
    game_ops.sticky_position = sticky;
    game_ops.end_dialog = end_dialog;
    game_ops.context = &trace;
    game_input.state = &game_state;
    game_input.packed_game_record = game_record;
    game_input.ops = &game_ops;
    game_input.context = &trace;

    checks = 0;
    failures = 0;
    failures += check(stars_new_game_dlg(&game_input, 20U, 273U, 101U, 0L)
        == 1, &checks);
    failures += check(trace.end_calls == 1 && trace.end_result == 1,
        &checks);
    failures += check(game_state.map_size == 2 && game_state.density == 1 &&
        game_state.start_distance == 2, &checks);
    failures += check(game_record[32] == 'G' && game_record[37] == 'Y',
        &checks);

    player_types[0] = 1U;
    players[0].packed_flags_valid = 1;
    players[0].packed_flags = 1U;
    players[0].name[0] = 'P';
    players[0].display_name[0] = 'p';
    memset(&trace, 0, sizeof trace);
    trace.game_record = game_record;
    wizard_ops.dialog = wizard_dialog;
    wizard_ops.get_game_name = game_name;
    wizard_ops.advantage_points = advantage;
    wizard_ops.ensure_player_name = no_player_name;
    wizard_ops.ensure_display_name = no_player_name;
    wizard_ops.save_destroy_restore = save_restore;
    wizard_ops.close_title = close_title;
    wizard_ops.generate_world = generate_world;
    wizard_ops.bring_up_host = bring_up_host;
    wizard_ops.set_game_bits = set_bits;
    wizard_ops.context = &trace;
    wizard_input.game_bits = &bits;
    wizard_input.players = players;
    wizard_input.player_types = player_types;
    wizard_input.packed_players = player_record;
    wizard_input.packed_player_stride = 192U;
    wizard_input.host_player = -1;
    wizard_input.ops = &wizard_ops;
    failures += check(stars_new_game_wizard(&wizard_input) == 1, &checks);
    failures += check(trace.dialog_calls == 3 && trace.stages[0] == 0 &&
        trace.stages[1] == 1 && trace.stages[2] == 2, &checks);
    failures += check(trace.name_calls == 1 &&
        strcmp((char *)wizard_input.game_name, "NEWGAME") == 0, &checks);
    failures += check(trace.save_calls == 1 && trace.close_calls == 1 &&
        trace.generate_calls == 1 && trace.host_calls == 1, &checks);
    failures += check(bits == 0x1234U && trace.bits_calls >= 2 &&
        trace.last_bits == 0x1234U, &checks);
    failures += check(wizard_input.game_player_count == 1 &&
        player_record[128] == 'P' && player_record[160] == 'p', &checks);
    failures += check(game_record[32] == 'G' && game_record[37] == 'Y',
        &checks);
    report = fopen("C:\\NGINT\\NGINTE3.TXT", "w");
    if (report) {
        fprintf(report,
            "NEW_GAME_CREATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("NEW_GAME_CREATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
