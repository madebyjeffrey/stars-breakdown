#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned calls;
static StarsI16 xs[8];
static StarsI16 ys[8];
static StarsI16 widths[8];
static StarsI16 heights[8];
static StarsU32 rops[8];

static void pat_blt(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU32 rop)
{
    (void)context; (void)dc;
    if (calls < 8U) {
        xs[calls] = x; ys[calls] = y; widths[calls] = width;
        heights[calls] = height; rops[calls] = rop;
    }
    ++calls;
}
static StarsInvertOps ops = { pat_blt };
static StarsInvertState state;

static void setup(StarsI16 layout)
{
    memset(&state, 0, sizeof(state));
    state.layout = layout; state.dx = 800; state.dy = 600;
    state.x_top = 200; state.y1 = 300; state.y2 = 400;
    state.dy_arial8 = 8;
}
static int vertical_and_horizontal(void)
{
    StarsInvertPoint point;
    StarsInvertPoint result;
    setup(0); point.x = 100; point.y = 20; calls = 0U;
    result = stars_invert_pane_border(&state, 7U, 3, point, 0, &ops, 0);
    return result.x == 100 && result.y == 20 && calls == 2U &&
        xs[0] == 301 && ys[0] == 0 && widths[0] == 6 &&
        xs[1] == 0 && ys[1] == 321 && widths[1] == 301 &&
        rops[0] == 0x005a0049UL && rops[1] == 0x005a0049UL ? 0 : 1;
}
static int previous_border(void)
{
    StarsInvertPoint point;
    StarsInvertPoint previous;
    StarsInvertPoint result;
    setup(0); point.x = 60; point.y = 0;
    previous.x = 50; previous.y = 0; calls = 0U;
    result = stars_invert_pane_border(&state, 7U, 0, point, &previous,
        &ops, 0);
    return result.x == 60 && result.y == 0 && calls == 3U &&
        xs[0] == 211 && xs[1] == 211 && xs[2] == 261 &&
        widths[0] == 6 && widths[1] == 6 && widths[2] == 6 ? 0 : 1;
}
static int split_lower(void)
{
    StarsInvertPoint point;
    StarsInvertPoint result;
    setup(2); point.x = 300; point.y = 0; calls = 0U;
    result = stars_invert_pane_border(&state, 7U, 4, point, 0, &ops, 0);
    return result.x == 0 && result.y == 0 && calls == 1U &&
        xs[0] == 207 && ys[0] == 401 && widths[0] == 593 &&
        heights[0] == 6 ? 0 : 1;
}
int main(void)
{
    int failures = vertical_and_horizontal() + previous_border() +
        split_lower();
    printf("INVERT_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
