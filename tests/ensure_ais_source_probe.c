#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned destroys;
static unsigned loads;
static unsigned closes;
static unsigned turns;
static unsigned progress_count;
static StarsI16 progress_values[4];
static StarsI16 turn_player;
static StarsU16 turn_flags;
static StarsU16 loaded_ids[4];

static void destroy_game(void *context, const StarsU8 *path, StarsU16 id)
{ (void)context; (void)path; (void)id; ++destroys; }
static void load_game(void *context, const StarsU8 *path, StarsU16 id)
{
    (void)context; (void)path;
    loaded_ids[loads++] = id;
}
static StarsU16 open_file(void *context, StarsI16 player, StarsU16 mode)
{
    (void)context; (void)mode;
    return player == 2 ? 9U : 0U;
}
static void close_file(void *context, StarsI16 handle)
{ (void)context; (void)handle; ++closes; }
static void do_ai_turn(void *context, StarsI16 player, StarsU16 flags)
{
    (void)context; ++turns; turn_player = player; turn_flags = flags;
}
static void update_progress(void *context, StarsI16 value)
{
    (void)context;
    progress_values[progress_count++] = value;
}
static StarsEnsureAisOps ops = { destroy_game, load_game, open_file,
    close_file, do_ai_turn, update_progress };

static int active_case(void)
{
    StarsEnsureAisState state;
    memset(&state, 0, sizeof(state));
    state.player_count = 3U;
    state.player_flags[0] = 0x0200U;
    state.player_flags[2] = 0x0201U;
    state.file_error_silent = 7U;
    destroys = loads = closes = turns = progress_count = 0U;
    stars_ensure_ais(&state, &ops, 0);
    return destroys == 2U && loads == 2U && loaded_ids[0] == 0x03f0U &&
        loaded_ids[1] == 0x03f4U && closes == 1U && turns == 1U &&
        turn_player == 0 && turn_flags == 0x0200U &&
        progress_count == 3U && progress_values[0] == 113 &&
        progress_values[1] == 226 && progress_values[2] == 340 &&
        state.file_error_silent == 7U && state.flags == 0x0400U ? 0 : 1;
}
static int skip_case(void)
{
    StarsEnsureAisState state;
    memset(&state, 0, sizeof(state));
    state.flags = 0x0400U; state.player_count = 3U;
    destroys = loads = closes = turns = progress_count = 0U;
    stars_ensure_ais(&state, &ops, 0);
    return destroys == 0U && loads == 0U && turns == 0U &&
        progress_count == 0U && state.flags == 0x0400U ? 0 : 1;
}
int main(void)
{
    int failures = active_case() + skip_case();
    printf("AIS_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
