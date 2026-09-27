#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 paints;
    StarsI16 ends;
    StarsI16 fills;
    StarsI16 positions;
    StarsI16 disabled;
    StarsI16 initialized;
    StarsI16 commands;
    StarsI16 popups;
    StarsI16 menus;
    StarsI16 focus_items;
    StarsI16 list_items;
    StarsI16 measured;
    StarsI16 cursors;
    StarsI16 last_end;
    StarsI16 menu_choice;
    StarsI16 selected_zip;
    StarsI16 zip_dialogs;
} Trace;
static StarsU16 begin(void *c, StarsU16 h, StarsRect *r){(void)h;(void)r;++((Trace*)c)->paints;return 3;}
static void end(void *c, StarsU16 h, StarsRect *r){(void)h;(void)r;++((Trace*)c)->ends;}
static void client(void *c, StarsU16 h, StarsRect *r){(void)c;(void)h;r->left=0;r->top=0;r->right=400;r->bottom=300;}
static void fill(void *c, StarsU16 h, const StarsRect *r, StarsU16 b){(void)h;(void)r;(void)b;++((Trace*)c)->fills;}
static StarsU16 item(void *c, StarsU16 h, StarsU16 id){(void)c;(void)h;return (StarsU16)(id+100);}
static void enable(void *c, StarsU16 h, StarsU16 e){(void)h;if(!e)++((Trace*)c)->disabled;}
static void pos(void *c, StarsU16 h, StarsU16 z, StarsI16 x, StarsI16 y, StarsI16 w, StarsI16 d, StarsU16 f){(void)h;(void)z;(void)x;(void)y;(void)w;(void)d;(void)f;++((Trace*)c)->positions;}
static StarsI16 inside(void *c, const StarsRect *r, StarsPoint p){(void)c;(void)r;return p.x>=0&&p.y>=0;}
static void cursor(void *c, StarsU16 id){(void)id;++((Trace*)c)->cursors;}
static void get_cursor(void *c, StarsPoint *p){(void)c;p->x=5;p->y=5;}
static void draw(void *c, StarsU16 h, StarsU16 dc, const StarsRect *r, StarsI16 f){(void)c;(void)h;(void)dc;(void)r;(void)f;}
static void focus_item(void *c, StarsU32 p, StarsI16 f){(void)p;(void)f;++((Trace*)c)->focus_items;}
static void list_item(void *c, StarsU32 p, StarsI16 f){(void)p;(void)f;++((Trace*)c)->list_items;}
static void measure(void *c, StarsU32 p, StarsI16 h){(void)p;(void)h;++((Trace*)c)->measured;}
static void command(void *c, StarsU16 h, StarsU16 m, StarsU32 p){(void)h;(void)m;(void)p;++((Trace*)c)->commands;}
static void popup(void *c, StarsU16 h, StarsI16 x, StarsI16 y){(void)h;(void)x;(void)y;++((Trace*)c)->popups;}
static StarsI16 menu(void *c, StarsU16 h, StarsI16 x, StarsI16 y){Trace*t=(Trace*)c;(void)h;(void)x;(void)y;++t->menus;return t->menu_choice;}
static void select_zip(void *c, StarsU16 h, StarsI16 i){(void)h;((Trace*)c)->selected_zip=i+1;}
static void open_zip(void *c, StarsU16 h){(void)h;++((Trace*)c)->zip_dialogs;}
static void sticky(void *c, StarsU16 h, StarsI16 save){(void)c;(void)h;(void)save;}
static void initialize(void *c, StarsU16 h, StarsU16 w, StarsU32 l){(void)h;(void)w;(void)l;++((Trace*)c)->initialized;}

int main(void)
{
    StarsProductionDlgState state; StarsProductionDlgOps ops;
    StarsProductionDlgInput input; Trace trace;
    memset(&state,0,sizeof state);memset(&ops,0,sizeof ops);memset(&input,0,sizeof input);memset(&trace,0,sizeof trace);
    state.button_face_brush=5;state.arial8_height=10;state.selected_player_planets=1;state.help_cursor=8;state.zip_valid_count=2;
    state.diamond.left=0;state.diamond.top=0;state.diamond.right=20;state.diamond.bottom=20;
    ops.begin_paint=begin;ops.end_paint=end;ops.get_client_rect=client;ops.fill_rect=fill;ops.get_dialog_item=item;ops.enable_window=enable;ops.set_window_pos=pos;ops.point_in_rect=inside;ops.get_cursor_pos=get_cursor;ops.set_cursor=cursor;ops.draw_dialog=draw;ops.draw_focus_item=focus_item;ops.draw_list_item=list_item;ops.set_measure_height=measure;ops.command_handler=command;ops.popup=popup;ops.popup_menu=menu;ops.select_zip=select_zip;ops.open_zip_dialog=open_zip;ops.sticky_position=sticky;ops.initialize_dialog=initialize;ops.context=&trace;
    input.state=&state;input.ops=&ops;
    if(stars_production_dlg(&input,10,272,0,0)!=1||trace.initialized!=1||trace.disabled!=2||trace.positions==0)return 1;
    if(stars_production_dlg(&input,10,15,0,0)!=1||trace.paints!=1||trace.ends!=1)return 2;
    if(stars_production_dlg(&input,10,20,4,0)!=1||trace.fills!=1)return 3;
    if(stars_production_dlg(&input,10,32,0,0x00050005UL)!=1||trace.cursors!=1)return 4;
    if(stars_production_dlg(&input,10,43,0,0x00010000UL)!=1||trace.list_items!=1)return 5;
    if(stars_production_dlg(&input,10,44,0,0)!=1||trace.measured!=1)return 6;
    if(stars_production_dlg(&input,10,273,1,0)!=0||trace.commands!=1)return 7;
    if(stars_production_dlg(&input,10,513,0,0x00050005UL)!=1||trace.popups!=1)return 8;
    if(stars_production_dlg(&input,10,516,0,0x00050005UL)!=1||trace.menus!=1)return 9;
    trace.menu_choice=1;
    if(stars_production_dlg(&input,10,516,0,0x00050005UL)!=1||trace.selected_zip!=2)return 10;
    trace.menu_choice=3;
    if(stars_production_dlg(&input,10,516,0,0x00050005UL)!=1||trace.zip_dialogs!=1)return 11;
    puts("PRODUCTION_DLG_SOURCE=PASS");return 0;
}
