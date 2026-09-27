#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace { StarsI16 acquired, released, fills, descriptions, headers, minerals, masses, dialog_calls; StarsI32 last_value; } ProbeTrace;
static StarsU16 probe_dc(void *opaque, StarsU16 window)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; ++t->acquired; return 7; }
static void probe_release(void *opaque, StarsU16 window, StarsU16 dc)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; (void)dc; ++t->released; }
static void probe_rect(void *opaque, StarsU16 window, StarsRect *rect)
{ (void)opaque; (void)window; rect->left = 0; rect->top = 0; rect->right = 300; rect->bottom = 200; }
static void probe_fill(void *opaque, StarsU16 dc, const StarsRect *rect)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)rect; ++t->fills; }
static void probe_font(void *opaque, StarsU16 dc, StarsU16 font)
{ (void)opaque; (void)dc; (void)font; }
static StarsU32 probe_color(void *opaque, StarsU16 dc, StarsU32 color)
{ (void)opaque; (void)dc; return color; }
static StarsI16 probe_dialog(void *opaque, StarsU16 window, StarsBuildSelCompRecord *record)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; ++t->dialog_calls;
    record->flags = t->dialog_calls == 2 ? 0 : 0x0100; record->group = 2; return 1; }
static StarsI16 probe_slot(void *opaque, StarsI16 slot, StarsBuildSelCompRecord *record,
    StarsBuildSelCompRecord *hull)
{ (void)opaque; (void)slot; record->flags = 0x0100; record->group = 3; hull->mass = 9; return 1; }
static void probe_description(void *opaque, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsBuildSelCompRecord *record)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)x; (void)y; (void)record; ++t->descriptions; }
static void probe_header(void *opaque, StarsU16 dc, StarsI16 y,
    const StarsBuildSelCompRecord *record)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)y; (void)record; ++t->headers; }
static void probe_mineral(void *opaque, StarsU16 dc, StarsI16 index, StarsI16 y,
    StarsI32 value)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)index; (void)y; ++t->minerals; t->last_value = value; }
static void probe_mass(void *opaque, StarsU16 dc, StarsI16 y, StarsI32 value)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)y; ++t->masses; t->last_value = value; }
static void probe_restore(void *opaque, StarsU16 window, StarsU16 dc)
{ (void)opaque; (void)window; (void)dc; }

int main(void)
{
    StarsBuildSelCompOps ops;
    StarsBuildSelCompInput input;
    ProbeTrace trace;
    StarsI16 failures;
    memset(&ops, 0, sizeof ops); memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    ops.get_dc = probe_dc; ops.release_dc = probe_release; ops.get_client_rect = probe_rect;
    ops.fill_background = probe_fill; ops.select_font = probe_font;
    ops.set_text_color = probe_color; ops.set_background_color = probe_color;
    ops.resolve_dialog_selection = probe_dialog; ops.resolve_slot = probe_slot;
    ops.draw_description = probe_description; ops.draw_header = probe_header;
    ops.draw_mineral_cost = probe_mineral; ops.draw_mass_cost = probe_mass;
    ops.restore = probe_restore;
    input.window = 10; input.slot_dialog = 11; input.selected_slot = 0;
    input.build_info_y = 20; input.arial8_height = 8; input.technology_level = 2;
    input.mineral_cost[0] = 4; input.mineral_cost[1] = 5; input.mineral_cost[2] = 6;
    input.mass_cost = 9; input.ops = &ops; input.context = &trace;
    stars_draw_build_sel_comp(&input, 0);
    input.selected_slot = -1;
    stars_draw_build_sel_comp(&input, 0);
    failures = trace.acquired != 2 || trace.released != 2;
    failures += trace.headers != 2 || trace.minerals != 6 || trace.masses != 2;
    input.selected_slot = -1;
    input.ops = &ops;
    stars_draw_build_sel_comp(&input, 0);
    failures += trace.descriptions != 1;
    {
        FILE *report;
        report = fopen("C:\\BSCOMP\\BCR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "BUILD_SEL_COMP_CHECKS=3 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("BUILD_SEL_COMP_CHECKS=3 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
