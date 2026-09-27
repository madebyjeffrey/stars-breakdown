#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 ship_clicks;
    StarsI16 planet_clicks;
    StarsI16 borders;
    StarsI16 captures;
    StarsI16 releases;
    StarsI16 moves;
    StarsI16 offsets;
    StarsI16 button_inits;
    StarsI16 button_tracks;
    StarsI16 reflows;
    StarsI16 last_column;
    StarsI16 last_index;
    StarsI16 move_x;
    StarsI16 move_y;
} Trace;

static StarsI16 in_rect(void *value, const StarsRect *rect, StarsPoint point)
{
    (void)value;
    return (StarsI16)(point.x >= (StarsI16)rect->left &&
        point.x < (StarsI16)rect->right && point.y >= (StarsI16)rect->top &&
        point.y < (StarsI16)rect->bottom);
}
static StarsU16 get_dc(void *value, StarsU16 window)
{ (void)value; (void)window; return 77; }
static void release_dc(void *value, StarsU16 window, StarsU16 dc)
{ (void)value; (void)window; (void)dc; }
static void capture(void *value, StarsU16 window)
{ Trace *t=(Trace *)value; (void)window; ++t->captures; }
static void release_capture(void *value)
{ Trace *t=(Trace *)value; ++t->releases; }
static void border(void *value, StarsU16 dc, const StarsRect *rect)
{ Trace *t=(Trace *)value; (void)dc; (void)rect; ++t->borders; }
static StarsI16 mouse_move(void *value, StarsPoint *point)
{
    Trace *t=(Trace *)value;
    if (t->moves++ == 0) { point->x=t->move_x; point->y=t->move_y; return 1; }
    return 0;
}
static void offset_rect(void *value, StarsRect *rect, StarsI16 dx, StarsI16 dy)
{ Trace *t=(Trace *)value; rect->left=(StarsU16)(rect->left+dx); rect->right=(StarsU16)(rect->right+dx); rect->top=(StarsU16)(rect->top+dy); rect->bottom=(StarsU16)(rect->bottom+dy); ++t->offsets; }
static void init_button(void *value, StarsPlanetClickButton *button,
    StarsU16 window, const StarsRect *rect, StarsU16 type)
{ Trace *t=(Trace *)value; (void)window; (void)rect; (void)type; ++t->button_inits; button->flags=2; }
static StarsI16 track_button(void *value, StarsPlanetClickButton *button)
{ Trace *t=(Trace *)value; (void)button; ++t->button_tracks; return 0; }
static void click_ship(void *value, StarsPoint point, StarsI16 sks,
    StarsI16 cursor, StarsI16 right)
{ Trace *t=(Trace *)value; (void)point; (void)sks; (void)cursor; (void)right; ++t->ship_clicks; }
static void click_planet(void *value, StarsPoint point, StarsI16 sks,
    StarsI16 cursor, StarsI16 right)
{ Trace *t=(Trace *)value; (void)point; (void)sks; (void)cursor; (void)right; ++t->planet_clicks; }
static void reflow(void *value, StarsI16 column, StarsI16 index, StarsI16 redraw)
{ Trace *t=(Trace *)value; (void)redraw; ++t->reflows; t->last_column=column; t->last_index=index; }

static void reset_tiles(StarsPlanetClickTile *tiles)
{
    memset(tiles,0,sizeof(StarsPlanetClickTile)*3);
    tiles[0].top=4; tiles[0].height=20; tiles[0].kind=2; tiles[0].layout_flags=0;
    tiles[1].top=20; tiles[1].height=20; tiles[1].kind=3; tiles[1].layout_flags=0;
    tiles[2].top=4; tiles[2].height=20; tiles[2].kind=0x20; tiles[2].layout_flags=1;
}

int main(void)
{
    StarsPlanetClickInput input;
    StarsPlanetClickTile tiles[3];
    Trace trace;
    int failures=0;

    memset(&input,0,sizeof input); memset(tiles,0,sizeof tiles);
    memset(&trace,0,sizeof trace);
    reset_tiles(tiles);
    input.selected_kind=2; input.arial8_height=8; input.planet_window=11;
    input.fleet_tiles=tiles; input.fleet_tile_count=3; input.point_in_rect=in_rect;
    input.get_dc=get_dc; input.release_dc=release_dc; input.capture=capture;
    input.release_capture=release_capture; input.draw_border=border;
    input.get_mouse_move=mouse_move; input.offset_rect=offset_rect;
    input.init_button=init_button; input.track_button=track_button;
    input.click_ship_orders=click_ship; input.click_planet_orders=click_planet;
    input.reflow_column=reflow; input.context=&trace;

    trace.move_x=220; trace.move_y=8;
    stars_planet_click(20,8,7,0,&input);
    if (trace.captures!=1 || trace.releases!=1 || trace.offsets!=1 ||
        trace.borders!=4 || tiles[1].kind!=0x20 || tiles[1].layout_flags!=1 ||
        trace.reflows!=2)
        failures |= 1;

    memset(&trace,0,sizeof trace);
    reset_tiles(tiles);
    stars_planet_click(210,8,7,1,&input);
    if (trace.ship_clicks!=1 || trace.captures!=0)
        failures |= 2;

    memset(&trace,0,sizeof trace);
    reset_tiles(tiles);
    stars_planet_click(185,6,7,0,&input);
    if (trace.button_inits!=1 || trace.button_tracks!=1 || trace.reflows!=1 ||
        (tiles[0].layout_flags & 0x80U)==0U)
        failures |= 4;

    memset(&trace,0,sizeof trace);
    stars_planet_click(3,8,7,0,&input);
    if (trace.borders!=0 || trace.ship_clicks!=0 || trace.planet_clicks!=0)
        failures |= 8;

#ifndef TARGET_NATIVE
    if (failures==0) puts("PLANET_CLICK_CHECKS=4 FAILURES=0 RESULT=PASS");
    else printf("PLANET_CLICK_CHECKS=4 FAILURES=%d RESULT=FAIL\n", failures);
#endif
    return failures!=0;
}
