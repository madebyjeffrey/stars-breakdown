#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 loaded;
    StarsI16 chosen;
    StarsI16 reset;
    StarsI16 generated;
    StarsI16 errors;
} Trace;

static StarsI16 load_race(void *p, StarsI16 player, const StarsU8 *path)
{ Trace *t = (Trace *)p; (void)player; (void)path; ++t->loaded; return 1; }
static StarsI16 choose_race(void *p, StarsI16 player, StarsI16 race,
    StarsI16 difficulty)
{ Trace *t = (Trace *)p; (void)player; (void)race; (void)difficulty; ++t->chosen; return 3; }
static StarsI16 advantage(void *p, StarsI16 player)
{ (void)p; return player == 0 ? -1 : 0; }
static void reset_player(void *p, StarsI16 player)
{ Trace *t = (Trace *)p; (void)player; ++t->reset; }
static StarsI16 random_value(void *p, StarsI16 limit)
{ (void)p; (void)limit; return 0; }
static void get_name(void *p, StarsI16 player, StarsU8 *name, StarsU16 capacity)
{
    const char *text = player == 0 ? "Alpha" : "Beta";
    (void)p; (void)capacity; strcpy((char *)name, text);
}
static void set_name(void *p, StarsI16 player, const StarsU8 *name,
    StarsU16 capacity)
{ (void)p; (void)player; (void)name; (void)capacity; }
static void generate_world(void *p, StarsGameFileState *state)
{ Trace *t = (Trace *)p; (void)state; ++t->generated; }
static void error_report(void *p, StarsI16 code)
{ Trace *t = (Trace *)p; (void)code; ++t->errors; }

int main(void)
{
    static const StarsU8 text[] =
        "Example game\n"
        "1 2 3 1234\n"
        "1 0 1 0 0 1 0\n"
        "2\n"
        "race1.r1\n"
        "#0 0\n"
        "1 20\n"
        "1 8\n"
        "1 1000\n"
        "1 20\n"
        "1 10\n"
        "1 10\n"
        "1 30\n"
        "1 30\n"
        "example.xy\n";
    StarsGameFileOps ops;
    StarsGameFileState state;
    Trace trace;

    memset(&ops, 0, sizeof ops); memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace); ops.context = &trace;
    ops.load_race = load_race; ops.choose_computer_race = choose_race;
    ops.advantage_points = advantage; ops.reset_player = reset_player;
    ops.random_value = random_value; ops.get_name = get_name;
    ops.set_name = set_name; ops.generate_world = generate_world;
    ops.error = error_report;
    if (!stars_gen_new_game_from_file(text, (StarsU16)(sizeof text - 1U),
            &state, &ops)) return 1;
    if (state.map_size != 1 || state.density != 2 || state.start_distance != 3 ||
            state.random_seed != 1234UL || state.player_count != 2) return 2;
    if (trace.loaded != 1 || trace.chosen != 1 || trace.reset != 1 ||
            trace.generated != 1 || trace.errors != 0) return 3;
    if (state.base_name[7] != 0 || state.victory_value[0] != 0 ||
            state.victory_value[1] != 0 || state.victory_value[8] != 1) return 4;
    puts("GEN_NEW_GAME_FROM_FILE_SOURCE=PASS");
    return 0;
}
