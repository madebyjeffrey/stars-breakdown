#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 hit;
    StarsI16 prepare;
    StarsI16 show;
    StarsI16 cargo;
    StarsI16 popup_calls;
    StarsI16 changed;
    StarsI16 main_changes;
    StarsI16 redraws;
    StarsI16 chosen_kind;
    StarsI16 chosen_id;
    StarsU16 offered_count;
    StarsU16 offered_markers[3];
    StarsPopupMineralScanChoicesInput scan;
} Trace;

static StarsI16 hit_test(void *context, StarsI16 x, StarsI16 y)
{
    Trace *trace = (Trace *)context;
    (void)x;
    (void)y;
    return trace->hit;
}

static StarsI16 prepare(void *context, StarsI16 hit)
{
    Trace *trace = (Trace *)context;
    ++trace->prepare;
    return (StarsI16)(200 + hit);
}

static void show(void *context, StarsI16 kind, StarsI16 x, StarsI16 y)
{
    Trace *trace = (Trace *)context;
    (void)kind;
    (void)x;
    (void)y;
    ++trace->show;
}

static StarsI16 choose_fleet(void *context, StarsU16 window, StarsI16 x,
    StarsI16 y, StarsU16 count, const StarsU32 *identities,
    const StarsU16 *markers, StarsI16 selected)
{
    Trace *trace = (Trace *)context;
    (void)window;
    (void)x;
    (void)y;
    (void)identities;
    (void)selected;
    trace->popup_calls++;
    trace->offered_count = count;
    if (count > 0U) trace->offered_markers[0] = markers[0];
    if (count > 1U) trace->offered_markers[1] = markers[1];
    if (count > 2U) trace->offered_markers[2] = markers[2];
    return count > 1U ? 1 : -1;
}

static void change_selection(void *context,
    const StarsScanSelection *selection, StarsI16 valid)
{
    Trace *trace = (Trace *)context;
    (void)valid;
    ++trace->changed;
    trace->chosen_kind = selection->grobj;
    trace->chosen_id = selection->fleet;
}

static void redraw_selection(void *context, StarsI16 before, StarsI16 after)
{
    Trace *trace = (Trace *)context;
    (void)before;
    (void)after;
    ++trace->redraws;
}

static void change_main_selection(void *context, StarsI16 kind, StarsI16 id)
{
    Trace *trace = (Trace *)context;
    ++trace->main_changes;
    trace->chosen_kind = kind;
    trace->chosen_id = id;
}

static void popup_mineral_choices(void *context, StarsI16 hit)
{
    Trace *trace = (Trace *)context;
    (void)hit;
    stars_popup_mineral_scan_choices(&trace->scan);
}

static void popup_fleet_cargo(void *context, StarsI16 hit)
{
    Trace *trace = (Trace *)context;
    (void)hit;
    ++trace->cargo;
}

static void install(StarsMineClickOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->hit_test = hit_test;
    ops->prepare_popup = prepare;
    ops->show_popup = show;
    ops->popup_mineral_choices = popup_mineral_choices;
    ops->popup_fleet_cargo = popup_fleet_cargo;
    ops->context = trace;
}

int main(void)
{
    StarsMineClickInput mine;
    StarsMineClickOps ops;
    StarsMineralScanFleet fleet;
    StarsMineralScanThing thing;
    Trace trace;
    StarsI16 failures = 0;

    memset(&mine, 0, sizeof mine);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    memset(&fleet, 0, sizeof fleet);
    memset(&thing, 0, sizeof thing);
    fleet.identity = 44UL;
    fleet.owner = 3;
    fleet.point.x = 10;
    fleet.point.y = 20;
    thing.identity = 55UL;
    thing.index = 7;
    thing.point.x = 10;
    thing.point.y = 20;
    trace.scan.window = 1U;
    trace.scan.player = 3;
    trace.scan.planet_owner = 3;
    trace.scan.selection.x = 10;
    trace.scan.selection.y = 20;
    trace.scan.selection.grobj = 1;
    trace.scan.selection.planet = 9;
    trace.scan.selection.fleet = 44;
    trace.scan.fleets = &fleet;
    trace.scan.fleet_count = 1U;
    trace.scan.things = &thing;
    trace.scan.thing_count = 1U;
    trace.scan.popup_menu = choose_fleet;
    trace.scan.change_selection = change_selection;
    trace.scan.redraw_selection = redraw_selection;
    trace.scan.change_main_selection = change_main_selection;
    trace.scan.context = &trace;
    install(&ops, &trace);
    mine.ops = &ops;

    trace.hit = 9;
    stars_mine_click(10, 20, 0x204, 0, &mine);
    if (trace.popup_calls != 1 || trace.offered_count != 3U ||
            trace.offered_markers[1] != 0x8000U ||
            trace.offered_markers[2] != 0x2000U || trace.changed != 1 ||
            trace.chosen_kind != 2 || trace.chosen_id != 44 ||
            trace.main_changes != 1 || trace.redraws != 2)
        ++failures;

    trace.hit = 11;
    stars_mine_click(10, 20, 0x204, 0, &mine);
    stars_mine_click(10, 20, 1, 0, &mine);
    if (trace.cargo != 1 || trace.prepare != 1 || trace.show != 1)
        ++failures;

    trace.hit = 1;
    stars_mine_click(10, 20, 0x204, 0, &mine);
    if (trace.prepare != 1 || trace.show != 1)
        ++failures;
    trace.hit = 2;
    stars_mine_click(10, 20, 1, 0, &mine);
    trace.hit = 3;
    stars_mine_click(10, 20, 1, 0, &mine);
    trace.hit = 6;
    stars_mine_click(10, 20, 1, 0, &mine);
    trace.hit = 8;
    stars_mine_click(10, 20, 1, 0, &mine);
    trace.hit = 10;
    stars_mine_click(10, 20, 1, 0, &mine);
    trace.hit = 12;
    stars_mine_click(10, 20, 1, 0, &mine);
    trace.hit = 13;
    stars_mine_click(10, 20, 1, 0, &mine);
    trace.hit = 14;
    stars_mine_click(10, 20, 1, 0, &mine);
    if (trace.prepare != 9 || trace.show != 9)
        ++failures;

#ifndef TARGET_NATIVE
    printf("MINE_CLICK_INTEGRATION_CHECKS=12 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
#endif
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("MCLIREP.TXT", "w");
        if (!report)
            return 7;
        fprintf(report,
            "MINE_CLICK_INTEGRATION_CHECKS=12 FAILURES=%d RESULT=%s\n",
            failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    return failures != 0;
}
