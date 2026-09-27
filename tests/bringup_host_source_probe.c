#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned marks;
static unsigned shows;
static unsigned procs;
static unsigned frees;
static unsigned destroys;
static unsigned progress;
static unsigned hides;
static unsigned ensures;
static unsigned generates;
static unsigned metrics;
static unsigned creates;
static unsigned keys;
static unsigned timers;
static unsigned timer_calls;
static StarsI16 dialog_values[4];
static unsigned dialog_index;
static StarsU16 last_show;
static StarsU16 last_title_class;
static StarsU16 last_title_text;

static void mark_file(void *context, StarsI16 a, StarsI16 b, StarsI16 c,
    StarsI16 d)
{ (void)context; (void)a; (void)b; (void)c; (void)d; ++marks; }
static void show_window(void *context, StarsU16 hwnd, StarsU16 show)
{ (void)context; (void)hwnd; ++shows; last_show = show; }
static StarsU32 make_proc(void *context, StarsU16 instance)
{ (void)context; (void)instance; ++procs; return 0x12345678UL; }
static StarsI16 dialog(void *context, StarsU16 resource, StarsU16 owner,
    StarsU32 proc)
{
    (void)context; (void)resource; (void)owner; (void)proc;
    return dialog_values[dialog_index++];
}
static void free_proc(void *context, StarsU32 proc)
{ (void)context; (void)proc; ++frees; }
static void game(void *context)
{ (void)context; ++destroys; }
static void show_progress(void *context)
{ (void)context; ++progress; }
static void hide_progress(void *context)
{ (void)context; ++hides; }
static void ensure_ais(void *context)
{ (void)context; ++ensures; }
static void generate_turn(void *context)
{ (void)context; ++generates; }
static StarsI16 metric(void *context, StarsU16 id)
{ (void)context; ++metrics; return id ? 600 : 800; }
static StarsU16 create_title(void *context, StarsU16 class_id,
    StarsU16 text_id, StarsU32 style, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 parent)
{
    (void)context; (void)style; (void)x; (void)y; (void)width;
    (void)height; (void)parent;
    last_title_class = class_id; last_title_text = text_id;
    ++creates; return 0x777U;
}
static StarsI16 key_state(void *context, StarsU16 key)
{ (void)context; (void)key; ++keys; return 0; }
static StarsU16 set_timer(void *context, StarsU16 owner, StarsU16 id,
    StarsU32 interval, StarsU32 proc)
{ (void)context; (void)owner; (void)id; (void)interval; (void)proc;
    ++timers; return 0x888U; }
static void host_timer(void *context, StarsU16 hwnd, StarsU16 msg,
    StarsU16 id, StarsU32 time)
{ (void)context; (void)hwnd; (void)msg; (void)id; (void)time; ++timer_calls; }
static StarsBringUpHostOps ops = { mark_file, show_window, make_proc, dialog,
    free_proc, game, show_progress, hide_progress, ensure_ais,
    generate_turn, metric, create_title, key_state, set_timer, host_timer };

static void reset(void)
{
    marks = shows = procs = frees = destroys = progress = hides = 0U;
    ensures = generates = metrics = creates = keys = timers = timer_calls = 0U;
    dialog_index = 0U;
}
static int cancel_case(void)
{
    StarsBringUpHostState state;
    memset(&state, 0, sizeof(state));
    state.frame = 1U; state.instance = 2U;
    state.title_class = 3U; state.title_text = 0x3f8U;
    dialog_values[0] = 0;
    reset();
    stars_bring_up_host_dialog(&state, &ops, 0);
    return marks == 2U && shows == 1U && last_show == 0U &&
        procs == 1U && frees == 1U && destroys == 1U && metrics == 2U &&
        creates == 1U && state.title == 0x777U && state.freeing_title == 0U &&
        timers == 0U ? 0 : 1;
}
static int generation_case(void)
{
    StarsBringUpHostState state;
    memset(&state, 0, sizeof(state));
    state.frame = 1U; state.instance = 2U; state.gd_high_flags = 0x400U;
    state.pass_count = 0U; dialog_values[0] = 1; dialog_values[1] = -1;
    reset();
    stars_bring_up_host_dialog(&state, &ops, 0);
    return marks == 1U && procs == 2U && frees == 2U && progress == 1U &&
        hides == 1U && ensures == 1U && generates == 1U && keys == 0U &&
        shows == 2U && last_show == 2U && timers == 1U &&
        timer_calls == 1U && state.timer_id == 0x888U &&
        state.timer_type == 0x0dU ? 0 : 1;
}
static int startup_mode_case(void)
{
    StarsBringUpHostState state;
    memset(&state, 0, sizeof(state));
    state.frame = 1U; state.ini_flags = 4U;
    dialog_values[0] = 0;
    reset();
    stars_bring_up_host_dialog(&state, &ops, 0);
    return state.ini_flags == 0U && shows == 2U && last_show == 2U &&
        timers == 1U && timer_calls == 1U && marks == 1U ? 0 : 1;
}
int main(void)
{
    int failures = cancel_case() + generation_case() + startup_mode_case();
    printf("HOST_DIALOG_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
