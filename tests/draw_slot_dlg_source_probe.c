#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace { StarsI16 acquired, released, fleet, spins, cargo, fills, empty, parts, selected, plaques; } ProbeTrace;
static StarsU16 probe_dc(void *opaque, StarsU16 window)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; ++t->acquired; return 8; }
static void probe_release(void *opaque, StarsU16 window, StarsU16 dc)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)window; (void)dc; ++t->released; }
static void probe_fleet(void *opaque, StarsU16 dc, StarsI16 x, StarsI16 y, StarsU16 mode)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)x; (void)y; (void)mode; ++t->fleet; }
static void probe_spin(void *opaque, StarsU16 dc, StarsI16 which)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)which; ++t->spins; }
static void probe_cargo(void *opaque, StarsU16 dc, const StarsRect *rect, StarsI16 starbase, StarsI16 dock)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)rect; (void)starbase; (void)dock; ++t->cargo; }
static void probe_fill(void *opaque, StarsU16 dc, const StarsRect *rect, StarsI16 slot)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)rect; (void)slot; ++t->fills; }
static void probe_empty(void *opaque, StarsU16 dc, const StarsRect *rect, const StarsDrawSlotRecord *record)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)rect; (void)record; ++t->empty; }
static void probe_part(void *opaque, StarsU16 dc, const StarsRect *rect, const StarsDrawSlotRecord *record)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)rect; (void)record; ++t->parts; }
static void probe_selected(void *opaque, StarsU16 dc, const StarsRect *rect)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)rect; ++t->selected; }
static void probe_plaque(void *opaque, StarsU16 dc, const StarsRect *rect, StarsI16 a, StarsI16 b, StarsI16 c, StarsI16 d)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)rect; (void)a; (void)b; (void)c; (void)d; ++t->plaques; }
static void probe_restore(void *opaque, StarsU16 window, StarsU16 dc)
{ (void)opaque; (void)window; (void)dc; }

int main(void)
{
    StarsDrawSlotOps ops;
    StarsDrawSlotInput input;
    ProbeTrace trace;
    StarsI16 failures;
    memset(&ops, 0, sizeof ops); memset(&input, 0, sizeof input); memset(&trace, 0, sizeof trace);
    ops.get_dc = probe_dc; ops.release_dc = probe_release; ops.draw_fleet_bitmap = probe_fleet;
    ops.draw_spin_buttons = probe_spin; ops.draw_cargo_panel = probe_cargo; ops.fill_slot = probe_fill;
    ops.draw_empty_slot = probe_empty; ops.draw_part_slot = probe_part;
    ops.draw_selected_frame = probe_selected; ops.draw_plaque = probe_plaque; ops.restore = probe_restore;
    input.window = 10; input.mode = 0; input.draw_index = -1; input.starbase_mode = 0;
    input.cargo_max = 2; input.hull_id = 0x21; input.slot_count = 2;
    input.selected_slot = 1; input.slots[0].flags = 0; input.slots[1].flags = 0x0100;
    input.ops = &ops; input.context = &trace;
    stars_draw_slot_dlg(&input);
    failures = trace.acquired != 1 || trace.released != 1;
    failures += trace.fleet != 1 || trace.spins != 1 || trace.cargo != 1;
    failures += trace.fills != 2 || trace.empty != 1 || trace.parts != 1;
    failures += trace.selected != 1 || trace.plaques != 1;
    input.hdc = 7; input.draw_index = 1; input.popup_window = 2;
    stars_draw_slot_dlg(&input);
    failures += trace.acquired != 1 || trace.fills != 3;
    {
        FILE *report;
        report = fopen("C:\\DRSLOT\\DSR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "DRAW_SLOT_CHECKS=5 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("DRAW_SLOT_CHECKS=5 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
