#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 popup_calls;
    StarsI16 changed;
    StarsI16 main_changes;
    StarsI16 redraws;
    StarsI16 chosen_kind;
    StarsI16 chosen_id;
    StarsU16 offered_count;
    StarsU16 offered_markers[4];
} Trace;

static StarsI16 popup_menu(void *context, StarsU16 window, StarsI16 x,
    StarsI16 y, StarsU16 count, const StarsU32 *identities,
    const StarsU16 *markers, StarsI16 selected)
{
    Trace *trace = (Trace *)context;
    (void)window; (void)x; (void)y; (void)identities; (void)selected;
    trace->popup_calls++;
    trace->offered_count = count;
    if (count > 0U) trace->offered_markers[0] = markers[0];
    if (count > 1U) trace->offered_markers[1] = markers[1];
    if (count > 2U) trace->offered_markers[2] = markers[2];
    return 1;
}

static void change_selection(void *context,
    const StarsScanSelection *selection, StarsI16 valid)
{
    Trace *trace = (Trace *)context;
    (void)valid;
    trace->changed++;
    trace->chosen_kind = selection->grobj;
    trace->chosen_id = selection->fleet;
}

static void redraw(void *context, StarsI16 before, StarsI16 after)
{
    Trace *trace = (Trace *)context;
    (void)before; (void)after;
    trace->redraws++;
}

static void change_main(void *context, StarsI16 kind, StarsI16 id)
{
    Trace *trace = (Trace *)context;
    trace->main_changes++;
    trace->chosen_kind = kind;
    trace->chosen_id = id;
}

int main(void)
{
    StarsPopupMineralScanChoicesInput input;
    StarsMineralScanFleet fleets[1];
    StarsMineralScanThing things[1];
    Trace trace;
    int failures = 0;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleets[0].identity = 44UL;
    fleets[0].owner = 2;
    fleets[0].point.x = 10;
    fleets[0].point.y = 20;
    things[0].identity = 55UL;
    things[0].index = 7;
    things[0].point.x = 10;
    things[0].point.y = 20;
    input.player = 3;
    input.planet_owner = 3;
    input.selection.x = 10;
    input.selection.y = 20;
    input.selection.planet = 9;
    input.selection.fleet = 44;
    input.selection.grobj = 1;
    input.fleets = fleets;
    input.fleet_count = 1;
    input.things = things;
    input.thing_count = 1;
    input.popup_menu = popup_menu;
    input.change_selection = change_selection;
    input.redraw_selection = redraw;
    input.change_main_selection = change_main;
    input.context = &trace;
    stars_popup_mineral_scan_choices(&input);
    if (trace.popup_calls != 1 || trace.offered_count != 3 ||
            trace.offered_markers[1] != 0x8000U ||
            trace.offered_markers[2] != 0x2000U || trace.changed != 1 ||
            trace.chosen_kind != 2 || trace.chosen_id != 44 ||
            trace.main_changes != 0)
        ++failures;

    /* A selected friendly fleet causes the same redraw/main-selection side
     * effects that the original uses after changing the scanner selection. */
    trace.popup_calls = 0;
    trace.changed = 0;
    trace.main_changes = 0;
    trace.redraws = 0;
    input.selection.grobj = 2;
    input.player = 3;
    fleets[0].owner = 3;
    stars_popup_mineral_scan_choices(&input);
    if (trace.main_changes != 1 || trace.redraws != 2)
        ++failures;

#ifndef TARGET_NATIVE
    printf("POPUP_MINERAL_SCAN_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
