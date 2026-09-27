#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 dialog_index;
    StarsI16 dialog_calls;
    StarsI16 init_calls;
    StarsI16 prepare_calls;
    StarsI16 copy_calls;
    StarsI16 configure_calls;
    StarsI16 random_calls;
    StarsI16 random_limits[4];
    StarsI16 configured_races[8];
    StarsI16 configured_ai[8];
    StarsI16 configured_randomized[8];
    StarsI16 finalize_calls;
    StarsI16 default_calls;
    StarsI16 names;
    StarsI16 display_names;
    StarsI16 set_count;
    StarsI16 relations;
    StarsI16 save_restore;
    StarsI16 close_title;
    StarsI16 generate;
    StarsI16 host;
    StarsI16 tutor;
    StarsI16 advantage_calls;
    StarsI16 bits_calls;
    StarsU16 last_bits;
    StarsI16 name_calls;
    StarsI16 name_success;
    StarsI16 fail_first_name;
    StarsI16 results[8];
} Trace;

static StarsI16 dialog(void *c, StarsU16 stage, StarsU16 hwnd,
    StarsI16 read_only, StarsI16 step)
{
    Trace *t = (Trace *)c;
    (void)stage;
    (void)hwnd;
    (void)read_only;
    (void)step;
    ++t->dialog_calls;
    return t->results[t->dialog_index++];
}
static StarsI16 get_name(void *c, StarsU8 *name, StarsU16 capacity)
{
    Trace *t = (Trace *)c;
    ++t->name_calls;
    if (t->fail_first_name && t->name_calls == 1)
        return 0;
    if (!t->name_success)
        return 0;
    if (capacity < 5)
        return 0;
    strcpy((char *)name, "ALPHA");
    return 1;
}
static void init_players(void *c, StarsI16 step, StarsI16 ai_level)
{
    Trace *t = (Trace *)c;
    (void)step;
    (void)ai_level;
    ++t->init_calls;
}
static void prepare_simple(void *c, StarsI16 result)
{
    Trace *t = (Trace *)c;
    (void)result;
    ++t->prepare_calls;
}
static void copy_player(void *c, StarsI16 dst, StarsI16 src)
{
    Trace *t = (Trace *)c;
    (void)dst;
    (void)src;
    ++t->copy_calls;
}
static void configure(void *c, StarsI16 dst, StarsI16 race,
    StarsI16 ai_level, StarsI16 randomized)
{
    Trace *t = (Trace *)c;
    (void)dst;
    (void)randomized;
    t->configured_races[t->configure_calls] = race;
    t->configured_ai[t->configure_calls] = ai_level;
    t->configured_randomized[t->configure_calls] = randomized;
    ++t->configure_calls;
}
static StarsI16 random_value(void *c, StarsI16 limit)
{
    Trace *t = (Trace *)c;
    t->random_limits[t->random_calls] = limit;
    ++t->random_calls;
    return (StarsI16)(limit - 1);
}
static void finalize(void *c, StarsI16 index, StarsU8 type,
    StarsI16 kind, StarsI16 source)
{
    Trace *t = (Trace *)c;
    (void)index;
    (void)type;
    (void)kind;
    (void)source;
    ++t->finalize_calls;
    if (kind < 0)
        ++t->default_calls;
}
static StarsI16 advantage(void *c, StarsI16 index)
{
    ++((Trace *)c)->advantage_calls;
    (void)c;
    return index == 1 ? -1 : 0;
}
static void player_name(void *c, StarsI16 index)
{
    Trace *t = (Trace *)c;
    (void)index;
    ++t->names;
}
static void display_name(void *c, StarsI16 index)
{
    Trace *t = (Trace *)c;
    (void)index;
    ++t->display_names;
}
static void set_count(void *c, StarsI16 count)
{
    Trace *t = (Trace *)c;
    if (count == 3)
        ++t->set_count;
}
static void relations(void *c, StarsI16 count)
{
    Trace *t = (Trace *)c;
    if (count == 3)
        ++t->relations;
}
static void lifecycle_save(void *c)
{
    ++((Trace *)c)->save_restore;
}
static void lifecycle_close(void *c)
{
    ++((Trace *)c)->close_title;
}
static void lifecycle_generate(void *c)
{
    ++((Trace *)c)->generate;
}
static void lifecycle_host(void *c)
{
    ++((Trace *)c)->host;
}
static void lifecycle_tutor(void *c)
{
    ++((Trace *)c)->tutor;
}
static void set_bits(void *c, StarsU16 bits)
{
    Trace *t = (Trace *)c;
    ++t->bits_calls;
    t->last_bits = bits;
}

static void install_ops(StarsNewGameWizardOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->dialog = dialog;
    ops->get_game_name = get_name;
    ops->init_players = init_players;
    ops->prepare_simple = prepare_simple;
    ops->copy_player = copy_player;
    ops->configure_computer = configure;
    ops->random_value = random_value;
    ops->finalize_player = finalize;
    ops->advantage_points = advantage;
    ops->ensure_player_name = player_name;
    ops->ensure_display_name = display_name;
    ops->set_player_count = set_count;
    ops->assign_relations = relations;
    ops->save_destroy_restore = lifecycle_save;
    ops->close_title = lifecycle_close;
    ops->generate_world = lifecycle_generate;
    ops->bring_up_host = lifecycle_host;
    ops->start_tutor = lifecycle_tutor;
    ops->set_game_bits = set_bits;
    ops->context = trace;
}

static int normal_case(void)
{
    StarsNewGameWizardInput input;
    StarsNewGameWizardOps ops;
    StarsNewGameWizardPlayer players[16];
    StarsNewGameWizardPlayer temporary[16];
    StarsNewGameWizardPlayer default_player;
    StarsU8 types[16];
    StarsU16 bits;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&players, 0, sizeof players);
    memset(&temporary, 0, sizeof temporary);
    memset(&default_player, 0, sizeof default_player);
    memset(types, 0, sizeof types);
    memset(&trace, 0, sizeof trace);
    trace.results[0] = 0xd3;
    trace.results[1] = 2;
    trace.results[2] = 3;
    trace.name_success = 1;
    types[0] = 29;
    types[1] = 2;
    types[2] = 255;
    players[0].packed_flags_valid = 1;
    players[2].packed_flags_valid = 1;
    temporary[0].race = 4;
    temporary[0].ai_level = 2;
    temporary[0].salt = 0x11223344L;
    temporary[0].relation_group = 7;
    default_player.race = 9;
    default_player.ai_level = 1;
    default_player.salt = 0x55667788L;
    default_player.relation_group = 12;
    bits = 0x1234;
    install_ops(&ops, &trace);
    input.hwnd = 9;
    input.game_bits = &bits;
    input.players = players;
    input.temporary_players = temporary;
    input.default_player = &default_player;
    input.player_types = types;
    input.host_player = -1;
    input.ops = &ops;
    if (!stars_new_game_wizard(&input) || trace.dialog_calls != 3 ||
            trace.prepare_calls != 1 || trace.init_calls != 3 ||
            trace.configure_calls != 2 || trace.copy_calls != 1 ||
            trace.random_calls != 3 || trace.random_limits[0] != 7 ||
            trace.random_limits[1] != 6 || trace.random_limits[2] != 4 ||
            trace.configured_races[0] != 6 ||
            trace.configured_randomized[0] != 1 ||
            trace.configured_races[1] != 5 || trace.configured_ai[1] != 3 ||
            players[0].salt != -1L ||
            players[0].race != 6 || players[0].ai_level != 0 ||
            players[0].packed_flags != 0xe200U ||
            players[1].race != 9 || players[1].ai_level != 1 ||
            players[1].salt != 0x55667788L ||
            players[1].dead != 1 ||
            players[1].relation_group != 12 ||
            players[2].race != 5 || players[2].ai_level != 3 ||
            players[2].packed_flags != 0xae00U ||
            trace.advantage_calls != 1 || trace.finalize_calls != 4 ||
            trace.default_calls != 1 ||
            trace.names != 3 || trace.display_names != 3 ||
            trace.set_count != 1 || trace.relations != 1 ||
            trace.save_restore != 1 || trace.close_title != 1 ||
            trace.generate != 1 || trace.host != 1 || bits != 0x1234 ||
            trace.last_bits != 0x1234 || strcmp((char *)input.game_name,
            "ALPHA") != 0)
        return 0;
    return 1;
}

static int retry_case(void)
{
    StarsNewGameWizardInput input;
    StarsNewGameWizardOps ops;
    StarsNewGameWizardPlayer players[16];
    StarsU8 types[16];
    StarsU16 bits;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(players, 0, sizeof players);
    memset(types, 0, sizeof types);
    memset(&trace, 0, sizeof trace);
    trace.results[0] = 0xd3;
    trace.results[1] = 3;
    trace.results[2] = 3;
    trace.name_success = 1;
    trace.fail_first_name = 1;
    bits = 0x80;
    install_ops(&ops, &trace);
    input.players = players;
    input.player_types = types;
    input.host_player = 1;
    input.game_bits = &bits;
    input.ops = &ops;
    if (stars_new_game_wizard(&input) != 0 || trace.name_calls != 2 ||
            trace.dialog_calls != 3 || bits != 0x80 ||
            trace.generate != 0)
        return 0;
    return 1;
}

static int read_only_case(void)
{
    StarsNewGameWizardInput input;
    StarsNewGameWizardOps ops;
    StarsNewGameWizardPlayer current[2];
    StarsNewGameWizardPlayer players[16];
    StarsU8 types[16];
    StarsU16 bits;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&current, 0, sizeof current);
    memset(players, 0, sizeof players);
    memset(types, 0, sizeof types);
    memset(&trace, 0, sizeof trace);
    current[0].active = 1;
    current[1].active = 1;
    current[1].dead = 1;
    trace.results[0] = 3;
    bits = 0x40;
    install_ops(&ops, &trace);
    input.read_only = 1;
    input.current_player_count = 2;
    input.current_players = current;
    input.players = players;
    input.player_types = types;
    input.game_bits = &bits;
    input.ops = &ops;
    if (stars_new_game_wizard(&input) != 0 || trace.dialog_calls != 1 ||
            trace.name_calls != 0 || types[0] != 2 || types[1] != 0x19U ||
            players[0].active != 1 || bits != 0x40) {
        return 0;
    }
    return 1;
}

static int cancel_and_tutor_case(void)
{
    StarsNewGameWizardInput input;
    StarsNewGameWizardOps ops;
    StarsU16 bits;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    trace.results[0] = 2;
    bits = 0x20;
    install_ops(&ops, &trace);
    input.game_bits = &bits;
    input.ops = &ops;
    if (stars_new_game_wizard(&input) != 0 || trace.tutor != 0 ||
            bits != 0x20)
        return 0;
    memset(&trace, 0, sizeof trace);
    trace.results[0] = 0xd4;
    install_ops(&ops, &trace);
    input.ops = &ops;
    if (stars_new_game_wizard(&input) != 0 || trace.tutor != 1 ||
            bits != 0x20)
        return 0;
    return 1;
}

static int relation_group_case(void)
{
    StarsNewGameWizardInput input;
    StarsNewGameWizardOps ops;
    StarsNewGameWizardPlayer players[16];
    StarsU8 types[16];
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(players, 0, sizeof players);
    memset(types, 0, sizeof types);
    memset(&trace, 0, sizeof trace);
    trace.results[0] = 0xd3;
    trace.results[1] = 2;
    trace.results[2] = 3;
    trace.name_success = 1;
    players[0].relation_group = 4;
    players[1].relation_group = 4;
    players[2].relation_group = 9;
    players[3].dead = 0;
    players[0].packed_flags = (StarsU16)(1U | (4U << 3));
    players[1].packed_flags = (StarsU16)(1U | (4U << 3));
    players[2].packed_flags = (StarsU16)(1U | (9U << 3));
    players[3].packed_flags = 0x0200U;
    players[0].packed_flags_valid = 1;
    players[1].packed_flags_valid = 1;
    players[2].packed_flags_valid = 1;
    players[3].packed_flags_valid = 1;
    types[0] = 1;
    types[1] = 1;
    types[2] = 1;
    types[3] = 1;
    install_ops(&ops, &trace);
    input.players = players;
    input.player_types = types;
    input.relation_groups_valid = 1;
    input.ops = &ops;
    if (!stars_new_game_wizard(&input) || players[0].relation_group != 4 ||
            players[1].relation_group != 31 ||
            players[2].relation_group != 9 ||
            players[3].relation_group != 0 ||
            (players[0].packed_flags & 0x00f8U) != (4U << 3) ||
            (players[1].packed_flags & 0x00f8U) != (31U << 3) ||
            (players[2].packed_flags & 0x00f8U) != (9U << 3) ||
            (players[3].packed_flags & 0x0200U) == 0U ||
            trace.random_calls != 3 ||
            trace.random_limits[0] != 2 || trace.random_limits[1] != 32 ||
            trace.random_limits[2] != 32)
        return 0;
    return 1;
}

int main(void)
{
    if (!normal_case()) {
        puts("normal fail");
        return 1;
    }
    if (!retry_case()) {
        puts("retry fail");
        return 1;
    }
    if (!read_only_case()) {
        puts("read-only fail");
        return 1;
    }
    if (!cancel_and_tutor_case()) {
        puts("cancel/tutor fail");
        return 1;
    }
    if (!relation_group_case()) {
        puts("relation-group fail");
        return 1;
    }
#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("C:\\NWT\\NWR.TXT", "w");
        if (report) {
            fprintf(report,
                "NEW_GAME_WIZARD_CHECKS=30 FAILURES=0 RESULT=PASS\n");
            fclose(report);
        }
    }
#endif
    puts("NEW_GAME_WIZARD_CHECKS=30 FAILURES=0 RESULT=PASS");
    return 0;
}
