#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 radar_created;
    StarsI16 radar_initialized;
    StarsI16 proxies;
    StarsI16 paints;
    StarsI16 patblts;
    StarsI16 toolbar_draws;
    StarsI16 fills;
    StarsI16 posts;
    StarsI16 tooltips;
    StarsI16 executes;
    StarsI16 button_draws;
    StarsI16 captures;
    StarsI16 releases;
    StarsI16 dc_releases;
    StarsI16 beeps;
    StarsI16 terminates;
    StarsI16 cursors;
    StarsI16 mouse_calls;
} Trace;

static void create_radar(void *c, StarsU16 toolbar, StarsI16 x, StarsI16 y,
    StarsI16 w, StarsI16 h)
{ (void)toolbar;(void)x;(void)y;(void)w;(void)h;++((Trace *)c)->radar_created; }
static void initialize_radar(void *c, StarsU16 radar, StarsI16 view)
{ (void)radar;(void)view;++((Trace *)c)->radar_initialized; }
static void install_proxy(void *c, StarsU16 window)
{ (void)window;++((Trace *)c)->proxies; }
static StarsU16 begin_paint(void *c, StarsU16 hwnd)
{ (void)hwnd;++((Trace *)c)->paints;return 3; }
static void get_client(void *c, StarsU16 hwnd, StarsRect *r)
{ (void)c;(void)hwnd;r->left=0;r->top=0;r->right=140;r->bottom=32; }
static void pat_blt(void *c, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsI16 w, StarsI16 h, StarsU32 rop)
{ (void)dc;(void)x;(void)y;(void)w;(void)h;(void)rop;++((Trace *)c)->patblts; }
static void end_paint(void *c, StarsU16 hwnd, StarsU16 dc)
{ (void)c;(void)hwnd;(void)dc; }
static void draw_toolbar(void *c, StarsU16 dc, const StarsRect *r)
{ (void)dc;(void)r;++((Trace *)c)->toolbar_draws; }
static void fill_rect(void *c, StarsU16 dc, const StarsRect *r, StarsU16 b)
{ (void)dc;(void)r;(void)b;++((Trace *)c)->fills; }
static void set_color(void *c, StarsU16 dc, StarsU32 color)
{ (void)c;(void)dc;(void)color; }
static void set_cursor(void *c, StarsU16 cursor)
{ (void)cursor;++((Trace *)c)->cursors; }
static void post_message(void *c, StarsU16 hwnd, StarsU16 msg, StarsU16 wp,
    StarsU32 lp)
{ (void)hwnd;(void)msg;(void)wp;(void)lp;++((Trace *)c)->posts; }
static void show_tooltip(void *c, StarsI16 id, const StarsRect *r)
{ (void)id;(void)r;++((Trace *)c)->tooltips; }
static void map_points(void *c, StarsU16 from, StarsU16 to, StarsPoint *p,
    StarsU16 count)
{ (void)c;(void)from;(void)to;(void)p;(void)count; }
static StarsI16 get_mouse_move(void *c, StarsPoint *p)
{
    Trace *t=(Trace *)c;++t->mouse_calls;
    if(t->mouse_calls==1){p->x=100;p->y=10;return 1;} return 0;
}
static StarsI16 point_in_rect(void *c, const StarsRect *r, StarsPoint p)
{ (void)c;(void)r;(void)p;return 1; }
static StarsU16 get_dc(void *c, StarsU16 hwnd)
{ (void)hwnd;return (StarsU16)(4+((Trace *)c)->paints); }
static void release_dc(void *c, StarsU16 hwnd, StarsU16 dc)
{ (void)hwnd;(void)dc;++((Trace *)c)->dc_releases; }
static void select_palette(void *c, StarsU16 dc, StarsU16 palette)
{ (void)c;(void)dc;(void)palette; }
static void realize_palette(void *c, StarsU16 dc)
{ (void)c;(void)dc; }
static void set_capture(void *c, StarsU16 hwnd)
{ (void)hwnd;++((Trace *)c)->captures; }
static void release_capture(void *c)
{ ++((Trace *)c)->releases; }
static void execute_button(void *c, StarsI16 button, StarsI16 pressed)
{ (void)button;(void)pressed;++((Trace *)c)->executes; }
static StarsI16 button_down(void *c, StarsI16 button)
{ (void)c;(void)button;return 0; }
static void draw_button(void *c, StarsU16 dc, StarsPoint p, StarsI16 button,
    StarsI16 down)
{ (void)dc;(void)p;(void)button;(void)down;++((Trace *)c)->button_draws; }
static void terminate_focus(void *c, StarsI16 cancel)
{ (void)cancel;++((Trace *)c)->terminates; }
static void beep(void *c, StarsU16 type)
{ (void)type;++((Trace *)c)->beeps; }
static StarsI32 default_proc(void *c, StarsU16 hwnd, StarsU16 msg,
    StarsU16 wp, StarsU32 lp)
{ (void)c;(void)hwnd;(void)msg;(void)wp;(void)lp;return 88; }

static void initialize(StarsTbWndState *s, StarsTbWndOps *o, Trace *t,
    const StarsI16 *buttons)
{
    memset(s,0,sizeof *s);memset(o,0,sizeof *o);memset(t,0,sizeof *t);
    s->toolbar_window=1;s->radar_window=55;s->font=2;s->hand_cursor=3;
    s->button_face_brush=4;s->button_face_color=5;s->arial8_height=8;
    s->radar_view=70;s->window_layout=0;s->last_x=-1;s->last_y=-1;
    s->buttons=buttons;s->button_count=3;s->ops=o;s->context=t;
    o->create_radar=create_radar;o->initialize_radar=initialize_radar;
    o->install_combo_proxy=install_proxy;o->install_edit_proxy=install_proxy;
    o->begin_paint=begin_paint;o->get_client_rect=get_client;o->pat_blt=pat_blt;
    o->draw_toolbar=draw_toolbar;o->end_paint=end_paint;o->fill_rect=fill_rect;
    o->set_background_color=set_color;o->set_cursor=set_cursor;
    o->post_message=post_message;o->show_tooltip=show_tooltip;
    o->map_points=map_points;o->get_mouse_move=get_mouse_move;
    o->point_in_rect=point_in_rect;o->get_dc=get_dc;o->release_dc=release_dc;
    o->select_palette=select_palette;o->realize_palette=realize_palette;
    o->set_capture=set_capture;o->release_capture=release_capture;
    o->execute_button=execute_button;o->button_down=button_down;
    o->draw_button=draw_button;o->terminate_focus=terminate_focus;
    o->message_beep=beep;o->default_proc=default_proc;
}

int main(void)
{
    StarsTbWndState s;StarsTbWndOps o;Trace t;
    const StarsI16 buttons[3]={-3,8,7};int failures=0;
    initialize(&s,&o,&t,buttons);
    if(stars_tb_wnd_proc(&s,1,STARS_MESSAGE_WM_CREATE,0,0)!=0||
            t.radar_created!=1||t.radar_initialized!=1||t.proxies!=2)++failures;
    if(stars_tb_wnd_proc(&s,1,STARS_MESSAGE_WM_PAINT,0,0)!=0||
            t.paints!=1||t.patblts!=3||t.toolbar_draws!=1)++failures;
    if(stars_tb_wnd_proc(&s,1,STARS_MESSAGE_WM_ERASE_BACKGROUND,2,0)!=1||
            t.fills!=1)++failures;
    if(stars_tb_wnd_proc(&s,1,STARS_MESSAGE_WM_CONTROL_COLOR,2,0)!=4)++failures;
    if(stars_tb_wnd_proc(&s,1,STARS_MESSAGE_WM_SETCURSOR,0,0)!=1||
            t.cursors!=1)++failures;
    if(stars_tb_wnd_proc(&s,1,STARS_MESSAGE_WM_COMMAND,0,
            ((StarsU32)8<<16)|55U)!=0||t.posts!=1)++failures;
    if(stars_tb_wnd_proc(&s,1,0x0200,0,((StarsU32)10<<16)|70U)!=0||
            t.tooltips!=1)++failures;
    if(stars_tb_wnd_proc(&s,1,0x0201,0,((StarsU32)10<<16)|70U)!=0||
            t.executes!=1)++failures;
    if(stars_tb_wnd_proc(&s,1,0x0201,0,((StarsU32)10<<16)|100U)!=0||
            t.captures!=1||t.releases!=1||t.button_draws<1||
            t.dc_releases!=1||t.executes!=2)++failures;
    if(stars_tb_wnd_proc(&s,1,0x05f4,0,0)!=0||t.terminates!=1)++failures;
#ifndef TARGET_NATIVE
    printf("TB_WND_PROC_CHECKS=10 FAILURES=%d RESULT=%s\n",failures,
        failures?"FAIL":"PASS");
#endif
    return failures!=0;
}
