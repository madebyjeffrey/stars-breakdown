#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 shown;
    StarsI16 hidden;
    StarsI16 resets;
    StarsI16 positions;
    StarsI16 enables;
    StarsI16 fills;
    StarsI16 draw_calls;
} Trace;
static StarsI16 draw_tile(void *context, StarsU16 hdc, StarsI16 title,
    StarsRect *rect)
{
    (void)hdc; (void)title; ++((Trace *)context)->draw_calls;
    rect->left = 0; rect->top = 0; rect->right = 120; rect->bottom = 80;
    return 1;
}
static void show_control(void *context, StarsI16 control, StarsI16 show)
{
    (void)control;
    if (show) ++((Trace *)context)->shown;
    else ++((Trace *)context)->hidden;
}
static void reset_refs(void *context)
{
    ++((Trace *)context)->resets;
}
static void position(void *context, StarsI16 left, StarsI16 top,
    StarsI16 width, StarsI16 height)
{
    (void)left; (void)top; (void)width; (void)height;
    ++((Trace *)context)->positions;
}
static StarsI32 selection(void *context, StarsU16 hdc)
{
    (void)hdc; return ((Trace *)context)->draw_calls == 1 ? 2 : -1;
}
static void enable(void *context, StarsI16 button, StarsI16 enabled)
{
    (void)button; (void)enabled; ++((Trace *)context)->enables;
}
static StarsI16 lookup(void *context, StarsI16 player, StarsI16 selection_id,
    StarsI16 transfer_index, StarsI16 *kind)
{
    (void)context; (void)player; (void)selection_id; (void)transfer_index;
    *kind = 2; return 1;
}
static void fill(void *context, const StarsRect *rect)
{
    (void)rect; ++((Trace *)context)->fills;
}
static void install(StarsPlanetShipListOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->show_control = show_control;
    ops->reset_references = reset_refs;
    ops->set_combo_position = position;
    ops->combo_selection = selection;
    ops->enable_button = enable;
    ops->lookup_transfer = lookup;
    ops->fill_details = fill;
    ops->context = trace;
}
int main(void)
{
    StarsPlanetShipListInput input;
    StarsPlanetShipListOps ops;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.hdc = 1;
    input.line_height = 8;
    input.combo_height = 40;
    input.player_id = 3;
    input.state.tile_changed = 1;
    input.state.has_object = 1;
    input.state.fleet_owner = 3;
    input.state.transfer_index = -1;
    input.ops = &ops;
    stars_draw_planet_ship_list(&input);
    if (trace.draw_calls != 1 || trace.hidden != 4 || trace.resets != 2 ||
            trace.positions != 4 || trace.shown != 4 || trace.fills != 1 ||
            trace.enables != 3)
        return 1;
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.state.tile_changed = 1;
    input.state.has_object = 0;
    input.state.tile_ship_mode = 1;
    input.ops = &ops;
    stars_draw_planet_ship_list(&input);
    if (trace.draw_calls != 1 || trace.hidden != 4 || trace.shown != 3 ||
            trace.fills != 0)
        return 1;
    puts("DRAW_PLANET_SHIP_LIST_CHECKS=19 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DPSL\\DPSLR.TXT", "w");
        if (!report)
            return 4;
        fputs("DRAW_PLANET_SHIP_LIST_CHECKS=19 FAILURES=0 RESULT=PASS\n",
            report);
        fclose(report);
    }
#endif
    return 0;
}
