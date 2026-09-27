#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    char events[128];
    StarsI16 event_count;
    StarsI16 invalidates;
    StarsI16 last_invalidate_kind;
    StarsI16 last_invalidate_arg;
    StarsI16 fill_ship_id;
    StarsI16 fill_battle_slot;
    StarsU16 last_message;
    StarsU16 last_wparam;
    StarsI16 hidden;
    StarsI16 rects_reset;
    StarsI16 tutor;
} Trace;

static void event(Trace *trace, char value)
{
    if (trace->event_count < (StarsI16)sizeof(trace->events))
        trace->events[trace->event_count++] = value;
}

static void invalidate_report(void *value, StarsI16 kind, StarsI16 arg)
{
    Trace *trace = (Trace *)value;
    event(trace, 'R');
    ++trace->invalidates;
    trace->last_invalidate_kind = kind;
    trace->last_invalidate_arg = arg;
}

static StarsI16 lookup_planet(void *value, StarsI16 id,
    StarsChangeMainPlanet *planet)
{
    (void)value;
    if (id < 0 || id > 3)
        return 0;
    planet->id = id;
    planet->point.x = (StarsI16)(10 + id);
    planet->point.y = (StarsI16)(20 + id);
    return 1;
}

static StarsI16 lookup_fleet(void *value, StarsI16 id,
    StarsChangeMainFleet *fleet)
{
    (void)value;
    if (id != 9)
        return 0;
    fleet->id_planet = 3;
    fleet->player = 1;
    fleet->point.x = 13;
    fleet->point.y = 23;
    fleet->planet_slot = 4;
    fleet->order_max = 0x012e;
    fleet->present = 1;
    return 1;
}

static void copy_fleet(void *value, const StarsChangeMainFleet *source,
    StarsChangeMainFleet *destination)
{
    Trace *trace = (Trace *)value;
    event(trace, 'C');
    *destination = *source;
}

static void fill(void *value)
{ event((Trace *)value, 'F'); }
static void fill_battle(void *value, StarsI16 slot)
{ Trace *trace=(Trace *)value; event(trace, 'B'); trace->fill_battle_slot=slot; }
static void fill_ship(void *value, StarsI16 id)
{ Trace *trace=(Trace *)value; event(trace, 'H'); trace->fill_ship_id=id; }
static void send_message(void *value, StarsU16 window, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    Trace *trace=(Trace *)value; (void)window; (void)lparam;
    event(trace, 'M'); trace->last_message=message; trace->last_wparam=wparam;
}
static void show_window(void *value, StarsU16 window, StarsI16 show)
{
    Trace *trace=(Trace *)value; (void)window;
    if (!show) { event(trace, 'S'); ++trace->hidden; }
}
static void draw_plan_ship(void *value, StarsU16 x, StarsU16 mask)
{ Trace *trace=(Trace *)value; (void)x; (void)mask; event(trace, 'D'); }
static void invalidate_rect(void *value, StarsU16 window, StarsI16 erase)
{ Trace *trace=(Trace *)value; (void)window; (void)erase; event(trace, 'I'); }
static void set_title(void *value, StarsU16 window)
{ Trace *trace=(Trace *)value; (void)window; event(trace, 'T'); }
static void advance_tutor(void *value)
{ Trace *trace=(Trace *)value; event(trace, 'A'); ++trace->tutor; }

static void init_input(StarsChangeMainObjInput *input,
    StarsChangeMainSelection *selection, const StarsChangeMainFleet *fleets,
    const StarsPoint *points, const StarsU16 *buttons,
    const StarsU16 *order_dropdowns, StarsRect *rects, Trace *trace)
{
    memset(input, 0, sizeof *input);
    input->f_ai=0; input->current_player=1; input->fleet_count=2;
    input->fleets=fleets; input->planet_points=points; input->scan_flags=0x10;
    input->tutorial_flags=0x0800; input->overlap_flags=0xffff;
    input->selection=selection; input->planet_window=11;
    input->toolbar_window=12; input->planet_production_window=13;
    input->order_edit_window=14; input->ship_dropdown_window=15;
    input->battle_dropdown_window=16; input->ship_list_window=17;
    input->fleet_component_window=18; input->report_combo_window=19;
    input->button_windows=buttons; input->order_dropdown_windows=order_dropdowns;
    input->reference_rects=rects; input->reference_rect_count=0x13;
    input->lookup_planet=lookup_planet; input->lookup_fleet=lookup_fleet;
    input->copy_fleet=copy_fleet; input->invalidate_report=invalidate_report;
    input->fill_planet_production=fill; input->fill_orders=fill;
    input->fill_fleet_components=fill; input->fill_battle=fill_battle;
    input->fill_ship=fill_ship; input->send_message=send_message;
    input->show_window=show_window; input->draw_plan_ship=draw_plan_ship;
    input->invalidate_rect=invalidate_rect; input->set_title=set_title;
    input->advance_tutor=advance_tutor; input->context=trace;
}

static int all_rects_reset(const StarsRect *rects)
{
    StarsI16 i;
    for (i=0; i<0x13; ++i)
        if (rects[i].top!=0xfffbU || rects[i].bottom!=0xfffaU) return 0;
    return 1;
}

int main(void)
{
    StarsChangeMainObjInput input;
    StarsChangeMainSelection selection;
    StarsChangeMainFleet fleets[2];
    StarsPoint points[4];
    StarsU16 buttons[13], order_dropdowns[3];
    StarsRect rects[0x13];
    Trace trace;
    StarsI16 i;
    int failures=0;

    memset(&selection,0,sizeof selection); memset(fleets,0,sizeof fleets);
    memset(points,0,sizeof points); memset(rects,0,sizeof rects);
    memset(&trace,0,sizeof trace);
    for (i=0;i<13;++i) buttons[i]=(StarsU16)(30+i);
    for (i=0;i<3;++i) order_dropdowns[i]=(StarsU16)(50+i);
    points[3].x=13; points[3].y=23;
    fleets[0].id_planet=3; fleets[0].player=1; fleets[0].present=1;
    fleets[1].present=0;
    selection.grobj=2; selection.id=9; selection.planet_id=-1;
    init_input(&input,&selection,fleets,points,buttons,order_dropdowns,rects,&trace);
    stars_change_main_obj_sel(1,3,&input);
    if (selection.grobj!=1 || selection.id!=3 || selection.planet_id!=3 ||
        selection.grobj_full!=3 || selection.active_waypoint!=-1 ||
        selection.scan_waypoint!=-1 || trace.invalidates!=2 ||
        trace.hidden!=23 || trace.fill_ship_id!=-1 || trace.tutor!=1 ||
        (input.overlap_flags & 0x2100U)!=0U || !all_rects_reset(rects))
        failures |= 1;

    memset(&trace,0,sizeof trace); selection.grobj=1; selection.id=3;
    input.context=&trace; input.scan_flags=0; input.tutorial_flags=0;
    stars_change_main_obj_sel(1,2,&input);
    if (trace.hidden!=0 || trace.event_count==0 || trace.fill_ship_id!=-1 ||
        strchr(trace.events, 'D')==0 || trace.events[trace.event_count-1]!='T')
        failures |= 2;

    memset(&trace,0,sizeof trace); selection.grobj=1; selection.id=2;
    selection.planet_id=3; input.context=&trace; input.scan_flags=0;
    stars_change_main_obj_sel(2,9,&input);
    if (selection.grobj!=2 || selection.id!=9 || selection.point.x!=13 ||
        selection.active_waypoint!=0 || selection.grobj_full!=3 ||
        trace.fill_battle_slot!=5 || trace.last_message!=0x040eU ||
        trace.last_wparam!=0x000eU || trace.fill_ship_id!=9)
        failures |= 4;

    memset(&trace,0,sizeof trace); selection.grobj=1; selection.id=2;
    input.context=&trace; input.f_ai=1;
    stars_change_main_obj_sel(1,2,&input);
    if (trace.event_count!=0)
        failures |= 8;

    memset(&trace,0,sizeof trace); input.f_ai=0; input.context=&trace;
    stars_change_main_obj_sel(2,77,&input);
    if (trace.invalidates!=2 || trace.event_count!=2)
        failures |= 16;

#ifndef TARGET_NATIVE
    if (failures==0) puts("CHANGE_MAIN_OBJ_SEL_CHECKS=5 FAILURES=0 RESULT=PASS");
    else printf("CHANGE_MAIN_OBJ_SEL_CHECKS=5 FAILURES=%d RESULT=FAIL\n", failures);
#endif
    return failures != 0;
}
