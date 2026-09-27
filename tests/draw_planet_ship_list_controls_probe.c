#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 tile_calls;
    StarsI16 combo_calls;
    StarsI16 button_calls;
    StarsI16 button_ids[3];
    StarsI16 button_x[3];
    StarsI16 button_y[3];
    StarsI16 button_width[3];
    StarsI16 button_height[3];
    StarsI16 show_calls;
    StarsI16 show_ids[3];
    StarsI16 show_values[3];
    StarsI16 enable_calls;
    StarsI16 enable_ids[3];
    StarsI16 enable_values[3];
    StarsI16 resets;
    StarsI16 fills;
    StarsI16 looked_up;
} Trace;

static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 title,
    StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    ++trace->tile_calls;
    if (title != 0x339) return 0;
    rect->left = 10;
    rect->top = 20;
    rect->right = 160;
    rect->bottom = 120;
    return 1;
}

static void show_control(void *context, StarsI16 control, StarsI16 show)
{
    Trace *trace = (Trace *)context;
    if (trace->show_calls < 3) {
        trace->show_ids[trace->show_calls] = control;
        trace->show_values[trace->show_calls] = show;
    }
    ++trace->show_calls;
}

static void set_combo(void *context, StarsI16 left, StarsI16 top,
    StarsI16 width, StarsI16 height)
{
    Trace *trace = (Trace *)context;
    (void)left;
    (void)top;
    (void)width;
    (void)height;
    ++trace->combo_calls;
}

static void set_button(void *context, StarsI16 control, StarsI16 left,
    StarsI16 top, StarsI16 width, StarsI16 height)
{
    Trace *trace = (Trace *)context;
    if (trace->button_calls < 3) {
        trace->button_ids[trace->button_calls] = control;
        trace->button_x[trace->button_calls] = left;
        trace->button_y[trace->button_calls] = top;
        trace->button_width[trace->button_calls] = width;
        trace->button_height[trace->button_calls] = height;
    }
    ++trace->button_calls;
}

static StarsI32 selection(void *context, StarsU16 hdc)
{
    (void)context;
    (void)hdc;
    return 4;
}

static void enable_button(void *context, StarsI16 control, StarsI16 enabled)
{
    Trace *trace = (Trace *)context;
    if (trace->enable_calls < 3) {
        trace->enable_ids[trace->enable_calls] = control;
        trace->enable_values[trace->enable_calls] = enabled;
    }
    ++trace->enable_calls;
}

static StarsI16 lookup(void *context, StarsI16 player, StarsI16 selection_id,
    StarsI16 transfer_index, StarsI16 *kind)
{
    Trace *trace = (Trace *)context;
    (void)player;
    (void)selection_id;
    (void)transfer_index;
    ++trace->looked_up;
    *kind = 2;
    return 1;
}

static void reset_refs(void *context)
{
    ++((Trace *)context)->resets;
}

static void fill_details(void *context, const StarsRect *rect)
{
    (void)rect;
    ++((Trace *)context)->fills;
}

static void check(int condition, int *checks, int *failures)
{
    ++*checks;
    if (!condition) ++*failures;
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
    ops.show_control = show_control;
    ops.reset_references = reset_refs;
    ops.set_combo_position = set_combo;
    ops.set_button_position = set_button;
    ops.combo_selection = selection;
    ops.enable_button = enable_button;
    ops.lookup_transfer = lookup;
    ops.fill_details = fill_details;
    ops.context = &trace;
    input.hdc = 2;
    input.line_height = 8;
    input.combo_height = 40;
    input.player_id = 3;
    input.state.has_object = 1;
    input.state.fleet_owner = 3;
    input.state.transfer_index = -1;
    input.ops = &ops;
    checks = 0;
    failures = 0;

    stars_draw_planet_ship_list(&input);
    check(trace.tile_calls == 1 && trace.combo_calls == 1,
        &checks, &failures);
    check(trace.button_calls == 3 && trace.button_ids[0] == 1 &&
        trace.button_ids[1] == 2 && trace.button_ids[2] == 0,
        &checks, &failures);
    check(trace.button_x[0] == 14 && trace.button_x[1] == 64 &&
        trace.button_x[2] == 114 && trace.button_y[0] == 33,
        &checks, &failures);
    check(trace.button_width[0] == 44 && trace.button_height[0] == 12,
        &checks, &failures);
    check(trace.show_calls == 4 && trace.show_ids[0] == 3 &&
        trace.show_ids[1] == 0 && trace.show_ids[2] == 1 &&
        trace.show_values[0] == 1 && trace.show_values[2] == 1,
        &checks, &failures);
    check(trace.looked_up == 1 && trace.fills == 1 && trace.resets == 1,
        &checks, &failures);
    check(trace.enable_calls == 3 && trace.enable_ids[0] == 0 &&
        trace.enable_values[0] == 1 && trace.enable_ids[1] == 1 &&
        trace.enable_values[1] == 1 && trace.enable_ids[2] == 2 &&
        trace.enable_values[2] == 1, &checks, &failures);

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("DPSLCRES.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "DRAW_PLANET_SHIP_LIST_CONTROL_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    printf("DRAW_PLANET_SHIP_LIST_CONTROL_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
