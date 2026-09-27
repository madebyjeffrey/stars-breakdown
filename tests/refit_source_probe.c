#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned moves;
static unsigned menus;
static unsigned checks;
static StarsI16 last_x;
static StarsI16 last_y;
static StarsI16 last_width;
static StarsI16 last_height;
static StarsU16 last_menu;
static StarsU16 last_id;
static StarsU16 last_flags;

static void move(void *context, StarsU16 hwnd, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsI16 repaint)
{
    (void)context; (void)hwnd; (void)repaint;
    ++moves; last_x = x; last_y = y;
    last_width = width; last_height = height;
}
static StarsU16 submenu(void *context, StarsU16 menu, StarsU16 position)
{
    (void)context; (void)position;
    ++menus;
    return (StarsU16)(menu + 1U);
}
static void check(void *context, StarsU16 menu, StarsU16 id, StarsU16 flags)
{
    (void)context;
    ++checks; last_menu = menu; last_id = id; last_flags = flags;
}
static StarsRefitOps ops = { move, submenu, check };

static void setup(StarsRefitState *state)
{
    memset(state, 0, sizeof(*state));
    state->frame = 1U; state->scanner = 2U; state->mine = 3U;
    state->planet = 4U; state->toolbar = 5U; state->message = 6U;
    state->dx = 800; state->dy = 600; state->dy_arial8 = 8;
}
static int standard_layout(void)
{
    StarsRefitState state;
    setup(&state);
    state.layout = 0; state.dx_plan_want = 200;
    state.dy_msg_want = 100; state.dy_min_want = 150;
    moves = menus = checks = 0U;
    stars_refit_frame_children(&state, &ops, 0);
    return state.x_top == 200 && state.y1 == 334 && state.y2 == 442 &&
        moves == 5U && menus == 2U && checks == 3U && last_id == 0x84U &&
        last_flags == 0U && last_x == 0 && last_y == 450 &&
        last_width == 200 && last_height == 150 ? 0 : 1;
}
static int split_layout(void)
{
    StarsRefitState state;
    setup(&state);
    state.layout = 2; state.dx2_plan_want = 300;
    state.dy2_msg_want = 200; state.dy2_min_want = 200;
    moves = menus = checks = 0U;
    stars_refit_frame_children(&state, &ops, 0);
    return state.x_top == 300 && state.y1 == 392 && state.y2 == 392 &&
        moves == 5U && menus == 2U && checks == 3U && last_id == 0x84U &&
        last_flags == 8U ? 0 : 1;
}
static int suppressed(void)
{
    StarsRefitState state;
    setup(&state);
    state.iconic = 1;
    moves = menus = checks = 0U;
    stars_refit_frame_children(&state, &ops, 0);
    return moves == 0U && menus == 0U && checks == 0U ? 0 : 1;
}
int main(void)
{
    int failures = standard_layout() + split_layout() + suppressed();
    printf("REFIT_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
