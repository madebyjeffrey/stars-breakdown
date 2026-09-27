#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned titles;
static unsigned creates;
static unsigned invalidates;
static unsigned destroys;
static unsigned scan_bars;
static unsigned refits;
static StarsI16 title_player;
static StarsI16 last_x;
static StarsI16 last_y;
static StarsI16 last_width;
static StarsI16 last_height;
static StarsU16 last_parent;
static StarsU16 last_instance;
static StarsU16 last_destroy;

static void make_title(void *context, StarsI16 player, const StarsU8 *base,
    StarsU8 *out, StarsU16 capacity)
{
    (void)context;
    (void)base;
    ++titles;
    title_player = player;
    if (capacity > 0U) strcpy((char *)out, player == (StarsI16)-1 ?
        "Stars" : "Stars - player");
}
static StarsU16 create_window(void *context, const StarsU8 *class_name,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height,
    StarsU16 parent, StarsU16 instance)
{
    (void)context;
    (void)class_name;
    ++creates;
    last_x = x; last_y = y; last_width = width; last_height = height;
    last_parent = parent; last_instance = instance;
    return (StarsU16)(0x200U + creates);
}
static void invalidate(void *context, StarsU16 hwnd)
{
    (void)context;
    ++invalidates;
    (void)hwnd;
}
static void set_text(void *context, StarsU16 hwnd, const StarsU8 *text)
{
    (void)context;
    (void)hwnd;
    (void)text;
}
static void destroy_window(void *context, StarsU16 hwnd)
{ (void)context; ++destroys; last_destroy = hwnd; }
static void set_scan_bars(void *context, StarsU16 hwnd)
{ (void)context; ++scan_bars; (void)hwnd; }
static void refit(void *context)
{ (void)context; ++refits; }
static StarsCreateChildOps ops = { make_title, create_window, invalidate,
    set_text, destroy_window, set_scan_bars, refit };

static void setup(StarsCreateChildState *state)
{
    memset(state, 0, sizeof(*state));
    state->player = 2; state->frame = 0x111U; state->instance = 0x222U;
    state->mine_width = 31; state->mine_height = 32;
    state->scan_class = (const StarsU8 *)"scan";
    state->mine_class = (const StarsU8 *)"mine";
    state->planet_class = (const StarsU8 *)"planet";
    state->toolbar_class = (const StarsU8 *)"toolbar";
    state->message_class = (const StarsU8 *)"message";
}
static int fresh_case(void)
{
    StarsCreateChildState state;
    setup(&state);
    titles = creates = invalidates = destroys = scan_bars = refits = 0U;
    stars_create_child_windows(&state, &ops, 0);
    return titles == 1U && title_player == 2 && creates == 5U &&
        invalidates == 0U && destroys == 0U && scan_bars == 0U &&
        refits == 1U && state.scanner == 0x201U && state.mine == 0x202U &&
        state.planet == 0x203U && state.toolbar == 0x204U &&
        state.message == 0x205U && last_x == (StarsI16)-500 &&
        last_y == (StarsI16)-500 && last_width == 10 && last_height == 10 &&
        last_parent == 0x111U && last_instance == 0x222U ? 0 : 1;
}
static int reuse_case(void)
{
    StarsCreateChildState state;
    setup(&state);
    state.scanner = 0x301U; state.mine = 0x302U; state.planet = 0x303U;
    state.toolbar = 0x304U; state.message = 0x305U;
    titles = creates = invalidates = destroys = scan_bars = refits = 0U;
    stars_create_child_windows(&state, &ops, 0);
    return creates == 1U && invalidates == 4U && destroys == 1U &&
        scan_bars == 1U && refits == 1U && state.x_scan_top == 1000 &&
        state.y_scan_top == 1000 && last_destroy == 0x305U &&
        state.message == 0x201U ? 0 : 1;
}
static int no_player_case(void)
{
    StarsCreateChildState state;
    setup(&state);
    state.player = (StarsI16)-1;
    titles = creates = invalidates = destroys = scan_bars = refits = 0U;
    stars_create_child_windows(&state, &ops, 0);
    return titles == 1U && title_player == (StarsI16)-1 && creates == 0U &&
        invalidates == 0U && destroys == 0U && refits == 0U ? 0 : 1;
}
int main(void)
{
    int failures = fresh_case() + reuse_case() + no_player_case();
    printf("CHILD_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
