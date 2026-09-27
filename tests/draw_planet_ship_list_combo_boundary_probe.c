#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 combo_calls;
    StarsI16 widths[2];
    StarsU16 flags[2];
    StarsI16 lookups;
} Trace;

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 title,
    StarsRect *rect)
{
    (void)context;
    (void)hdc;
    (void)title;
    rect->left = 0;
    rect->top = 0;
    rect->right = 120;
    rect->bottom = 80;
    return 1;
}

static void set_combo(void *context, StarsI16 left, StarsI16 top,
    StarsI16 width, StarsI16 height, StarsU16 flags)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)left;
    (void)top;
    (void)height;
    if (trace->combo_calls < 2) {
        trace->widths[trace->combo_calls] = width;
        trace->flags[trace->combo_calls] = flags;
    }
    ++trace->combo_calls;
}

static StarsI32 selection(void *context, StarsU16 hdc)
{
    (void)context;
    (void)hdc;
    return 1;
}

static StarsI16 lookup(void *context, StarsI16 player, StarsI16 selected,
    StarsI16 transfer_index, StarsI16 *kind)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)player;
    (void)selected;
    (void)transfer_index;
    ++trace->lookups;
    *kind = 2;
    return 1;
}

int main(void)
{
    StarsPlanetShipListInput input;
    StarsPlanetShipListOps ops;
    Trace trace;
    int checks;
    int failures;

    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.draw_tile = draw_tile;
    ops.set_combo_position_ex = set_combo;
    ops.combo_selection = selection;
    ops.lookup_transfer = lookup;
    ops.context = &trace;
    input.line_height = 8;
    input.combo_height = 40;
    input.player_id = 3;
    input.state.has_object = 1;
    input.state.fleet_owner = 3;
    input.state.transfer_index = -1;
    input.ops = &ops;

    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    stars_draw_planet_ship_list(&input);
    CHECK(trace.combo_calls == 1 && trace.widths[0] == 112);
    CHECK(trace.flags[0] == 0x14U && input.state.tile_width == 112);
    stars_draw_planet_ship_list(&input);
    CHECK(trace.combo_calls == 2 && trace.widths[1] == 112);
    CHECK(trace.flags[1] == 0x15U && trace.lookups == 2);
#undef CHECK
    printf("DRAW_PLANET_SHIP_LIST_COMBO_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
