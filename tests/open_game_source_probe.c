#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsOpenGameState *current_state;
static StarsI16 classify_result;
static StarsI16 load_result;
static unsigned dialogs;
static unsigned destroys;
static unsigned loads;
static unsigned children;
static unsigned title_destroy;
static unsigned hosts;
static unsigned messages;
static unsigned selected;
static unsigned wizards;
static unsigned finds;
static char dialog_path[512];
static char loaded_name[512];

static StarsI16 open_dialog(void *context, StarsU16 hwnd, StarsU8 *filter,
    StarsU16 filter_id)
{ (void)context; (void)hwnd; (void)filter; (void)filter_id; ++dialogs;
    strcpy(current_state->selected_path, dialog_path); return 1; }
static StarsI16 was_race_file(void *context, const StarsU8 *name,
    StarsI16 check_password)
{ (void)context; (void)name; (void)check_password; return classify_result; }
static void set_work_from_date(void *context, StarsI16 type, StarsI16 player)
{ (void)context; (void)type; (void)player; }
static void destroy_game(void *context)
{ (void)context; ++destroys; }
static StarsI16 load_game(void *context, const StarsU8 *path,
    const StarsU8 *name)
{ (void)context; (void)path; ++loads; strcpy(loaded_name, (const char *)name);
    return load_result; }
static void create_children(void *context)
{ (void)context; ++children; }
static void destroy_window(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; ++title_destroy; }
static void bring_up_host(void *context)
{ (void)context; ++hosts; }
static void send_message(void *context, StarsU16 hwnd, StarsU16 msg,
    StarsU16 wparam, StarsU32 lparam)
{ (void)context; (void)hwnd; (void)msg; (void)wparam; (void)lparam; ++messages; }
static void find_select(void *context)
{ (void)context; ++finds; }
static void race_wizard(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; ++wizards; }
static StarsOpenGameOps ops = { open_dialog, was_race_file, set_work_from_date,
    destroy_game, load_game, create_children, destroy_window, bring_up_host,
    send_message, find_select, race_wizard };

static void reset(StarsOpenGameState *state)
{
    memset(state, 0, sizeof(*state)); current_state = state;
    classify_result = 0; load_result = 1; dialogs = destroys = loads = 0U;
    children = title_destroy = hosts = messages = selected = wizards = finds = 0U;
    loaded_name[0] = 0;
}
static int interactive_case(void)
{
    StarsOpenGameState state;
    StarsI16 result;
    reset(&state); state.planet_count = 1U; state.player = 2;
    state.selection_object = 5; strcpy(dialog_path, "C:\\GAMES\\GAME.X");
    result = stars_open_game(&state, 0x20U, 0, &ops, 0);
    return result == 1 && dialogs == 1U && destroys == 1U && loads == 1U &&
        children == 1U && messages == 1U && finds == 1U &&
        state.selection_object == 0 && state.selection_waypoint == -1 &&
        state.selection_id == -1 && strcmp(loaded_name, "C:\\GAMES\\GAME.X") == 0 ? 0 : 1;
}
static int race_only_case(void)
{
    StarsOpenGameState state;
    StarsI16 result;
    reset(&state); classify_result = 1; strcpy(dialog_path, "RACE.R");
    result = stars_open_game(&state, 0x20U, 1, &ops, 0);
    return result == 1 && strcmp(state.race_path, "RACE.R") == 0 &&
        destroys == 0U ? 0 : 1;
}
static int startup_host_case(void)
{
    StarsOpenGameState state;
    StarsI16 result;
    reset(&state); state.ini_flags = 1U; state.player = -1;
    state.serial_number = 4UL; state.title = 7U;
    strcpy(state.startup_path, "C:\\GAMES\\START.X");
    strcpy(state.base_path, "C:\\GAMES\\BASE.X");
    result = stars_open_game(&state, 0x20U, 0, &ops, 0);
    return result == 0 && destroys == 1U && loads == 1U && children == 1U &&
        title_destroy == 1U && hosts == 1U && state.title == 0U &&
        state.file_error_silent == 0 ? 0 : 1;
}
static int startup_failure_case(void)
{
    StarsOpenGameState state;
    StarsI16 result;
    reset(&state); state.ini_flags = 1U; state.serial_number = 0UL;
    strcpy(state.startup_path, "C:\\GAMES\\BROKEN.X");
    strcpy(state.base_path, "C:\\GAMES\\BASE.X"); classify_result = 1;
    result = stars_open_game(&state, 0x20U, 0, &ops, 0);
    return result == -1 && wizards == 0U && state.ini_flags == 0U &&
        state.file_error_silent == 0 ? 0 : 1;
}
int main(void)
{
    int failures = interactive_case() + race_only_case() +
        startup_host_case() + startup_failure_case();
    printf("OPEN_GAME_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
