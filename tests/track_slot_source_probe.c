#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace {
    StarsI16 spins;
    StarsI16 selected;
    StarsI16 drags;
    StarsI16 popups;
    StarsI16 inspected;
    StarsU16 popup_id;
    StarsU16 list_group;
    StarsU16 list_kind;
} ProbeTrace;

static StarsI16 probe_spin(void *opaque, StarsU16 window, StarsI16 direction,
    StarsU16 *bitmap)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)window;
    (void)direction;
    ++trace->spins;
    *bitmap = (StarsU16)(*bitmap + 4U);
    return 0;
}

static void probe_select(void *opaque, StarsI16 slot)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    trace->selected = slot;
}

static StarsI16 probe_list(void *opaque, StarsU16 window, StarsI16 *slot,
    StarsTrackSlotPart *part)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)window;
    *slot = -1;
    part->group = trace->list_group;
    part->kind = trace->list_kind;
    part->hull_flags = 0;
    part->component = 0;
    return 1;
}

static void probe_popup(void *opaque, StarsU16 window, StarsI16 x,
    StarsI16 y, StarsU16 popup_id)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)window;
    (void)x;
    (void)y;
    ++trace->popups;
    trace->popup_id = popup_id;
}

static StarsI16 probe_drag(void *opaque, StarsU16 window, StarsI16 slot,
    const StarsTrackSlotPart *part, StarsI16 x, StarsI16 y)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)window;
    (void)slot;
    (void)part;
    (void)x;
    (void)y;
    ++trace->drags;
    return 1;
}

static void probe_inspect(void *opaque, StarsU16 window, StarsI16 x,
    StarsI16 y, const StarsTrackSlotPart *part)
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    (void)window;
    (void)x;
    (void)y;
    (void)part;
    ++trace->inspected;
}

int main(void)
{
    StarsTrackSlotOps ops;
    StarsTrackSlotInput input;
    ProbeTrace trace;
    StarsI16 failures;
    StarsI16 result;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    ops.track_spin = probe_spin;
    ops.select_slot = probe_select;
    ops.read_list_part = probe_list;
    ops.show_popup = probe_popup;
    ops.drag_part = probe_drag;
    ops.inspect_part = probe_inspect;
    input.window = 10;
    input.slot_dialog = 11;
    input.part_count = 1;
    input.current_bitmap = 4;
    input.spin_rect[0].left = 50;
    input.spin_rect[0].top = 0;
    input.spin_rect[0].right = 60;
    input.spin_rect[0].bottom = 20;
    input.slot_rect[0].left = 0;
    input.slot_rect[0].top = 0;
    input.slot_rect[0].right = 40;
    input.slot_rect[0].bottom = 40;
    input.ops = &ops;
    input.context = &trace;

    input.x = 55;
    input.y = 10;
    result = stars_track_slot(&input);
    failures = result != 1 || input.current_bitmap != 8 || trace.spins != 1;

    input.x = 10;
    input.y = 10;
    input.slot_part[0].kind = STARS_TRACK_PART_EMPTY;
    result = stars_track_slot(&input);
    failures += result != 0 || trace.selected != 0;

    input.slot_part[0].kind = STARS_TRACK_PART_COMPONENT;
    input.build_mode = 4;
    result = stars_track_slot(&input);
    failures += result != 1 || trace.drags != 1;

    input.list_box = 1;
    input.build_mode = 0;
    trace.list_group = 1;
    trace.list_kind = 0;
    input.hull_id = 1;
    result = stars_track_slot(&input);
    failures += result != 1 || trace.popups != 1 ||
        trace.popup_id != STARS_TRACK_POPUP_EMPTY;

    input.list_box = 0;
    input.right_button = 1;
    result = stars_track_slot(&input);
    failures += result != 1 || trace.inspected != 1;

    {
        FILE *report;
        report = fopen("C:\\WTRACK\\WTR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "TRACK_SLOT_CHECKS=5 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("TRACK_SLOT_CHECKS=5 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
