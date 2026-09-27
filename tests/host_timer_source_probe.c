#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned kills;
static unsigned destroys;
static unsigned loads;
static unsigned children;
static unsigned timers;
static unsigned flashes;
static unsigned beeps;
static unsigned shows;
static unsigned hides;
static unsigned ensures;
static unsigned generates;
static unsigned quits;
static unsigned alerts;
static unsigned invalidates;
static unsigned enables;
static unsigned outstanding_calls;
static StarsI16 outstanding_value;
static StarsU16 last_title;
static StarsU16 last_alert;
static char last_loaded[32];
static char last_window[128];

static StarsI16 new_turn(void *context, StarsU16 player)
{ (void)context; (void)player; return 1; }
static void kill_timer(void *context, StarsU16 hwnd, StarsU16 id)
{ (void)context; (void)hwnd; (void)id; ++kills; }
static void destroy_game(void *context)
{ (void)context; ++destroys; }
static StarsI16 load_game(void *context, const StarsU8 *name)
{ (void)context; ++loads; strcpy(last_loaded, (const char *)name); return 1; }
static void create_children(void *context)
{ (void)context; ++children; }
static StarsU16 set_timer(void *context, StarsU16 owner, StarsU16 id,
    StarsU32 interval, StarsU32 proc)
{ (void)context; (void)owner; (void)id; (void)interval; (void)proc; ++timers;
    return 0x99U; }
static void flash_window(void *context, StarsU16 hwnd, StarsU16 invert)
{ (void)context; (void)hwnd; (void)invert; ++flashes; }
static void message_beep(void *context, StarsU16 kind)
{ (void)context; (void)kind; ++beeps; }
static StarsI16 turns_outstanding(void *context)
{ (void)context; ++outstanding_calls; return outstanding_value; }
static const StarsU8 *text(void *context, StarsU16 id)
{ (void)context; return id == 0x31aU ? (const StarsU8 *)"Loaded" :
    (const StarsU8 *)""; }
static void set_window_text(void *context, StarsU16 hwnd, const StarsU8 *value)
{ (void)context; if (hwnd == 0x22U) last_title = 1U;
    strcpy(last_window, (const char *)value); }
static StarsU16 get_window(void *context, StarsU16 hwnd, StarsU16 relation)
{ (void)context; (void)hwnd; return relation == 3U ? 0x33U : 0x22U; }
static void invalidate(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; ++invalidates; }
static void show_progress(void *context)
{ (void)context; ++shows; }
static void hide_progress(void *context)
{ (void)context; ++hides; }
static void ensure_ais(void *context)
{ (void)context; ++ensures; }
static void generate_turn(void *context)
{ (void)context; ++generates; outstanding_value = 1; }
static void post_quit(void *context, StarsI16 value)
{ (void)context; (void)value; ++quits; }
static StarsU16 get_dialog_item(void *context, StarsU16 hwnd, StarsU16 id)
{ (void)context; (void)hwnd; (void)id; return 0x44U; }
static void enable_window(void *context, StarsU16 hwnd, StarsU16 enabled)
{ (void)context; (void)hwnd; (void)enabled; ++enables; }
static void alert(void *context, StarsU16 id, StarsI16 value)
{ (void)context; (void)value; ++alerts; last_alert = id; }

static StarsHostTimerOps ops = { new_turn, kill_timer, destroy_game, load_game,
    create_children, set_timer, flash_window, message_beep, turns_outstanding,
    text, set_window_text, get_window, invalidate, show_progress,
    hide_progress, ensure_ais, generate_turn, post_quit, get_dialog_item,
    enable_window, alert };

static void reset(void)
{
    kills = destroys = loads = children = timers = flashes = beeps = 0U;
    shows = hides = ensures = generates = quits = alerts = invalidates = 0U;
    enables = outstanding_calls = 0U; last_title = 0U; last_alert = 0U;
    last_loaded[0] = 0; last_window[0] = 0;
}
static int load_case(void)
{
    StarsHostTimerState state;
    memset(&state, 0, sizeof(state));
    state.timer_type = 0x0eU; state.timer_id = 7U; state.player = 2U;
    state.frame = 0x22U; state.file_error_silent = 9U;
    state.player_format = (const StarsU8 *)"P%d";
    reset(); stars_host_timer_proc(&state, 0x22U, &ops, 0);
    return kills == 1U && destroys == 1U && loads == 1U && children == 1U &&
        timers == 1U && flashes == 1U && beeps == 1U && state.timer_id == 0x99U &&
        state.timer_type == 0x0fU && state.processing_timer == 0U &&
        state.file_error_silent == 9U && strcmp(last_loaded, "P3") == 0 ? 0 : 1;
}
static int generation_case(void)
{
    StarsHostTimerState state;
    memset(&state, 0, sizeof(state));
    state.frame = 0x22U; state.global_flags = 0x400U;
    state.waiting_format = (const StarsU8 *)"%d waiting";
    state.waiting_suffix = (const StarsU8 *)"s";
    outstanding_value = 0; reset();
    stars_host_timer_proc(&state, 0x11U, &ops, 0);
    return outstanding_calls >= 2U && shows == 1U && hides == 1U &&
        ensures == 2U && generates == 1U && quits == 0U && alerts == 0U &&
        state.processing_timer == 0U ? 0 : 1;
}
static int flashing_case(void)
{
    StarsHostTimerState state;
    memset(&state, 0, sizeof(state)); state.frame = 0x22U;
    state.timer_type = 0x0fU; reset();
    stars_host_timer_proc(&state, 0x11U, &ops, 0);
    return flashes == 1U && kills == 0U && state.processing_timer == 0U ? 0 : 1;
}
int main(void)
{
    int failures = load_case() + generation_case() + flashing_case();
    printf("HOST_TIMER_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
