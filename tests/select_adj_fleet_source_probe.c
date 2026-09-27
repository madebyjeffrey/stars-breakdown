#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 scan_kind;
    StarsI16 main_id;
    StarsI16 dropdown;
    StarsU16 invalidated;
    StarsU16 redraws;
} Trace;
static void invalid(void *context, StarsI16 a, StarsI16 b)
{ Trace *t=(Trace *)context; (void)a; (void)b; ++t->invalidated; }
static void scan(void *context, StarsPoint point, StarsU16 kind)
{ Trace *t=(Trace *)context; (void)point; t->scan_kind=kind; }
static void redraw(void *context, StarsI16 a, StarsI16 b)
{ Trace *t=(Trace *)context; (void)a; (void)b; ++t->redraws; }
static void main_object(void *context, StarsU16 kind, StarsI16 id)
{ Trace *t=(Trace *)context; (void)kind; t->main_id=id; }
static void center(void *context, StarsPoint point, StarsI16 full)
{ (void)context; (void)point; (void)full; }
static void scroll(void *context)
{ (void)context; }
static void title(void *context, StarsU16 hwnd)
{ (void)context; (void)hwnd; }
static void dropdown(void *context, StarsI16 id)
{ ((Trace *)context)->dropdown=id; }

int main(void)
{
    static const StarsSelectAdjFleetRecord fleets[] = {
        { 10, 3, { 1, 2 } }, { 11, 3, { 3, 4 } },
        { 12, 4, { 5, 6 } }
    };
    static const StarsU16 order[] = { 0U, 1U };
    StarsSelectAdjFleetOps ops;
    StarsSelectAdjFleetInput input;
    Trace trace;
    memset(&ops,0,sizeof ops); memset(&input,0,sizeof input);
    memset(&trace,0,sizeof trace);
    ops.invalidate_report=invalid; ops.change_scan_selection=scan;
    ops.redraw_scan_selection=redraw; ops.change_main_object=main_object;
    ops.center_scanner=center; ops.draw_scroll_bar=scroll;
    ops.invalidate_mine_window=invalid; ops.set_mineral_title=title;
    ops.set_fleet_dropdown=dropdown;
    input.fleets=fleets; input.fleet_count=3U; input.player_fleet_order=order;
    input.player_fleet_count=2U; input.player=3; input.mine_window=9U;
    input.selection_kind=2U; input.selected_point.x=1; input.selected_point.y=2;
    input.report_cached=0U; input.ops=&ops; input.context=&trace;
    stars_select_adj_fleet(0,10,&input);
    if (trace.scan_kind!=0x82U || trace.main_id!=10 || trace.dropdown!=10 ||
            trace.invalidated!=2U || trace.redraws!=2U) return 1;
    memset(&trace,0,sizeof trace); trace.dropdown=-1; input.report_cached=1U;
    input.selected_fleet_id=10; input.selection_kind=0U;
    stars_select_adj_fleet(1,0,&input);
    if (trace.scan_kind!=0x82U || trace.main_id!=11 || trace.dropdown!=-1 ||
            trace.redraws!=2U) return 2;
    memset(&trace,0,sizeof trace); trace.dropdown=-1;
    stars_select_adj_fleet(0,12,&input);
    if (trace.scan_kind!=0x83U || trace.main_id!=0 || trace.redraws!=0U)
        return 3;
    puts("SELECT_ADJ_FLEET_SOURCE=PASS");
    return 0;
}
