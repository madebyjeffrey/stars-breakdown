#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 sends;
    StarsU16 messages[8];
    StarsU8 text[8][52];
} Trace;
static void name(void *value, StarsI16 id, StarsU8 *out, StarsU16 size)
{ (void)value; (void)size; strcpy((char *)out, id==2 ? "Fleet" : "Thing"); }
static void send(void *value, StarsU16 window, StarsU16 message,
    StarsU16 wparam, const StarsU8 *text)
{ Trace *t=(Trace *)value; StarsI16 n=t->sends++; t->messages[n]=message; if(text)strcpy((char *)t->text[n],(const char *)text); (void)window; (void)wparam; }

int main(void)
{
    StarsPoint planets[4];
    StarsShipDropdownFleet fleets[2];
    StarsShipDropdownThing things[2];
    StarsFillShipSelection selection;
    StarsFillShipDropdownInput input;
    Trace trace;

    planets[2].x=10; planets[2].y=20;
    selection.selected_object_kind=1; selection.selected_id=2;
    selection.selected_fleet_point.x=10; selection.selected_fleet_point.y=20;
    selection.planet_points=planets;
    fleets[0].id=2; fleets[0].owner=1; fleets[0].planet_id=2;
    fleets[0].point.x=10; fleets[0].point.y=20;
    fleets[1].id=3; fleets[1].owner=2; fleets[1].planet_id=4;
    fleets[1].point.x=10; fleets[1].point.y=20;
    things[0].id=7; things[0].kind=1; things[0].owner=1;
    things[0].point.x=10; things[0].point.y=20;
    things[1].id=8; things[1].kind=2; things[1].owner=1;
    things[1].point=things[0].point;
    memset(&trace,0,sizeof trace); memset(&input,0,sizeof input);
    input.dropdown=99; input.current_player=1; input.selection=&selection;
    input.fleets=fleets; input.fleet_count=2; input.things=things;
    input.thing_count=2; input.fleet_name=name; input.thing_name=name;
    input.send_message=send; input.context=&trace;
    stars_fill_ship_dd(-1,&input);
    if(trace.sends!=4||trace.messages[0]!=0x40b||
       trace.messages[1]!=0x403||trace.messages[2]!=0x403||
       trace.messages[3]!=0x40e||strcmp((char *)trace.text[1]," Fleet")!=0||
       strcmp((char *)trace.text[2]," Thing")!=0)return 1;
    puts("FILL_SHIP_DD_SOURCE=PASS"); return 0;
}
