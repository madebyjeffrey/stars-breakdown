#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 selected; StarsI16 logged; StarsI16 ended; StarsI16 radio; StarsI16 names; StarsI16 sticky; StarsI16 painted; StarsI16 filled; StarsI16 invalidated; StarsI16 help; } Trace;
static void sticky(void *c, StarsU16 h, StarsI16 show){Trace*t=(Trace*)c;(void)h;t->sticky=show?1:2;}
static void paint(void *c, StarsU16 h){Trace*t=(Trace*)c;(void)h;++t->painted;}
static void fill(void *c, StarsU16 h){Trace*t=(Trace*)c;(void)h;++t->filled;}
static void log_change(void *c, StarsI16 g){Trace*t=(Trace*)c;t->logged=g+1;}
static void invalidate(void *c, StarsU16 h){Trace*t=(Trace*)c;(void)h;++t->invalidated;}
static void radio(void *c, StarsU16 a, StarsU16 b, StarsU16 s){Trace*t=(Trace*)c;(void)a;(void)b;t->radio=(StarsI16)s;}
static void add_name(void *c, StarsI16 p){Trace*t=(Trace*)c;(void)p;++t->names;}
static void select_row(void *c, StarsI16 p){Trace*t=(Trace*)c;t->selected=p;}
static StarsI16 get_selection(void *c){return ((Trace*)c)->selected;}
static void end_dialog(void *c, StarsI16 r){Trace*t=(Trace*)c;t->ended=r;}
static void help(void *c, StarsU16 f){Trace*t=(Trace*)c;(void)f;++t->help;}
int main(void)
{
    StarsU8 values[3]; StarsRelationsOps ops; StarsRelationsInput input; Trace trace;
    memset(values,0,sizeof values); memset(&ops,0,sizeof ops); memset(&input,0,sizeof input); memset(&trace,0,sizeof trace);
    ops.sticky_dialog=sticky; ops.paint=paint; ops.fill_background=fill; ops.log_change=log_change;
    ops.invalidate_scanner=invalidate; ops.set_radio=radio; ops.add_name=add_name; ops.select_row=select_row;
    ops.get_selection=get_selection; ops.end_dialog=end_dialog; ops.help=help;
    trace.selected=0; input.player=1; input.player_count=3; input.relation_values=values;
    input.scanner_window=7; input.scan_group=0x12; input.list_control=0x7d3; input.background_brush=4; input.ops=&ops; input.context=&trace;
    if (stars_relations_dialog(1,0x110,0,0,&input)!=1 || trace.names!=2 || trace.radio!=0x7d4) return 1;
    if (stars_relations_dialog(1,0x111,0x7d6,0,&input)!=0 || values[0]!=2U || !input.dirty) return 2;
    if (stars_relations_dialog(1,0x111,2,0,&input)!=1 || trace.ended!=3) return 3;
    if (stars_relations_dialog(1,2,0,0,&input)!=0 || trace.logged!=3 || trace.invalidated!=1) return 4;
    if (stars_relations_dialog(1,0x0f,0,0,&input)!=1 || trace.painted!=1) return 5;
    puts("RELATIONS_DLG_SOURCE=PASS"); return 0;
}
