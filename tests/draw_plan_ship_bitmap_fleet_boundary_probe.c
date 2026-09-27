#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 fleet_id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 args[6];
    StarsI16 tile_calls;
} Trace;

static StarsU16 fleet_ex(void *context, StarsU16 hdc, StarsI16 fleet_id,
    StarsI16 x, StarsI16 y, StarsI16 arg1, StarsI16 arg2, StarsI16 arg3,
    StarsI16 arg4, StarsI16 arg5, StarsI16 arg6)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hdc;
    ++trace->calls;
    trace->fleet_id = fleet_id;
    trace->x = x;
    trace->y = y;
    trace->args[0] = arg1;
    trace->args[1] = arg2;
    trace->args[2] = arg3;
    trace->args[3] = arg4;
    trace->args[4] = arg5;
    trace->args[5] = arg6;
    return 1;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 kind,
    StarsI16 id, StarsRect *rect, const StarsU8 *text)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hdc;
    (void)kind;
    (void)id;
    (void)text;
    ++trace->tile_calls;
    rect->left = 10;
    rect->top = 20;
    rect->right = 210;
    rect->bottom = 100;
    return 1;
}

static StarsI16 fleet_name(void *context, StarsI16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    (void)context;
    (void)id;
    if (capacity < 2U)
        return 0;
    buffer[0] = (StarsU8)'F';
    buffer[1] = 0;
    return 1;
}

int main(void)
{
    StarsDrawPlanShipBitmapInput input;
    StarsPlanShipBitmapOps ops;
    Trace trace;
    int checks;
    int failures;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.fleet_name = fleet_name;
    ops.draw_tile = draw_tile;
    ops.draw_fleet_ex = fleet_ex;
    ops.context = &trace;
    input.hdc = 3;
    input.object_kind = 2;
    input.object_id = 17;
    input.tile_flags = 0x1000;
    input.arial8_height = 8;
    input.ops = &ops;

    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    stars_draw_plan_ship_bitmap(&input);
    CHECK(trace.tile_calls == 1);
    CHECK(trace.calls == 1 && trace.fleet_id == 17);
    CHECK(trace.x == 22 && trace.y == 26);
    CHECK(trace.args[0] == 1 && trace.args[1] == -1 &&
        trace.args[2] == 0 && trace.args[3] == 0 &&
        trace.args[4] == -1 && trace.args[5] == 0);

    memset(&trace, 0, sizeof trace);
    input.global_display_flags = 0x0008;
    stars_draw_plan_ship_bitmap(&input);
    CHECK(trace.calls == 1 && trace.x == 22 && trace.y == 22);
    CHECK(trace.args[0] == 1 && trace.args[1] == -1 &&
        trace.args[2] == 0 && trace.args[3] == 0 &&
        trace.args[4] == -1 && trace.args[5] == 0);
#undef CHECK
    printf("DRAW_PLAN_SHIP_BITMAP_FLEET_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
