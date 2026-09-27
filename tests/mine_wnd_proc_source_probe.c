#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Probe {
    int create, font_message, title_bar, frame, extent, center, arrow, survey;
    int end, fill, color, cursor, write_mem, click, default_proc;
    StarsU16 created_window, cursor_id;
    StarsI16 title_length, click_x, click_y;
    StarsU32 written_value;
    StarsMineWndThing thing;
} Probe;

static void get_text(void *v, StarsU16 id, StarsU8 *out, StarsU16 n)
{
    Probe *p = (Probe *)v;
    const char *text = id == 0xffffU ? "BUTTON" : "Survey";
    (void)p;
    if (n != 0) {
        strncpy((char *)out, text, n - 1U);
        out[n - 1U] = 0;
    }
}
static StarsU16 create_window(void *v, const StarsU8 *class_name,
    const StarsU8 *title, StarsU32 style, StarsI16 x, StarsI16 y,
    StarsI16 w, StarsI16 h, StarsU16 parent, StarsU16 menu,
    StarsU16 instance)
{
    Probe *p = (Probe *)v;
    if (strcmp((const char *)class_name, "BUTTON") == 0 &&
            strcmp((const char *)title, "Survey") == 0 &&
            style == 0x40000003UL && x == 100 && y == 100 && w == 0x96 &&
            h == 8 && parent == 9 && menu == 0 && instance == 3)
        ++p->create;
    p->created_window = 42;
    return p->created_window;
}
static StarsI32 send_message(void *v, StarsU16 window, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    Probe *p = (Probe *)v;
    (void)lparam;
    if (window == 42 && message == 0x30 && wparam == 17) ++p->font_message;
    if (window == 42 && message == 0x400 && wparam == 0) return 1;
    return 0;
}
static void set_title_bar(void *v, StarsU16 hwnd)
{ Probe *p = (Probe *)v; if (hwnd == 9) ++p->title_bar; }
static StarsU16 begin_paint(void *v, StarsU16 hwnd, StarsRect *r)
{ (void)v; (void)hwnd; r->left=0; r->top=0; r->right=120; r->bottom=80; return 7; }
static void get_client_rect(void *v, StarsU16 hwnd, StarsRect *r)
{ (void)v; (void)hwnd; r->left=0; r->top=0; r->right=120; r->bottom=80; }
static void draw_frame(void *v, StarsU16 dc, const StarsRect *r, StarsU16 kind)
{ Probe *p=(Probe *)v; if(dc==7&&r->left==4&&r->top==4&&r->right==116&&r->bottom==12&&kind==0)++p->frame; }
static StarsU32 set_color(void *v, StarsU16 dc, StarsU32 color)
{ Probe *p=(Probe *)v; (void)dc; (void)color; ++p->color; return 55UL; }
static StarsI32 text_extent(void *v, StarsU16 dc, const StarsU8 *s, StarsI16 n)
{ Probe *p=(Probe *)v; (void)dc; (void)s; ++p->extent; return (StarsI32)n * 8L; }
static void center_text(void *v, StarsU16 dc, const StarsRect *r,
    const StarsU8 *s, StarsI16 n)
{ Probe *p=(Probe *)v; (void)dc; (void)r; (void)s; p->title_length=n; ++p->center; }
static void draw_arrow(void *v, StarsU16 dc, const StarsRect *r, StarsI16 on)
{ Probe *p=(Probe *)v; (void)dc; (void)r; if(on==1)++p->arrow; }
static void draw_survey(void *v, StarsU16 dc, const StarsRect *r)
{ Probe *p=(Probe *)v; (void)dc; (void)r; ++p->survey; }
static void end_paint(void *v, StarsU16 hwnd, StarsRect *r)
{ Probe *p=(Probe *)v; (void)hwnd; (void)r; ++p->end; }
static void fill_rect(void *v, StarsU16 dc, const StarsRect *r, StarsU16 b)
{ Probe *p=(Probe *)v; (void)dc; (void)r; if(b==6)++p->fill; }
static void get_cursor_pos(void *v, StarsPoint *p)
{ (void)v; p->x=20; p->y=20; }
static void screen_to_client(void *v, StarsU16 hwnd, StarsPoint *p)
{ (void)v; (void)hwnd; (void)p; }
static StarsI16 point_in_rect(void *v, const StarsRect *r, StarsPoint p)
{ (void)v; return p.x >= (StarsI16)r->left && p.y >= (StarsI16)r->top && p.x < (StarsI16)r->right && p.y < (StarsI16)r->bottom; }
static StarsI16 hit_test(void *v, StarsU16 hwnd, StarsI16 x, StarsI16 y)
{ (void)v; (void)hwnd; return x==20 && y==20 ? 9 : 0; }
static void set_cursor(void *v, StarsU16 cursor)
{ Probe *p=(Probe *)v; p->cursor_id=cursor; ++p->cursor; }
static StarsI16 other_at_selection(void *v)
{ (void)v; return 1; }
static void write_mem(void *v, StarsU16 record, StarsU16 size, StarsU32 value)
{ Probe *p=(Probe *)v; if(record==0x2b&&size==4)p->written_value=value; ++p->write_mem; }
static void click(void *v, StarsI16 x, StarsI16 y, StarsU16 message, StarsU16 wparam)
{ Probe *p=(Probe *)v; if(x==5&&y==6&&message==0x201&&wparam==2){p->click_x=x;p->click_y=y;++p->click;} }
static StarsI32 default_proc(void *v, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{ Probe *p=(Probe *)v; (void)hwnd;(void)message;(void)wparam;(void)lparam;++p->default_proc;return 77L; }

int main(void)
{
    StarsMineWndOps o;
    StarsMineWndInput i;
    Probe p;
    int failures = 0;
    memset(&o,0,sizeof o); memset(&i,0,sizeof i); memset(&p,0,sizeof p);
    i.instance=3; i.font=17; i.font_height=8; i.text_color=1;
    i.background_color=2; i.button_face_brush=6; i.hand_cursor=33;
    i.arrow_help_cursor=34; i.button_class=(const StarsU8 *)"BUTTON";
    i.title=(const StarsU8 *)"Mineral"; i.selected_thing=&p.thing;
    p.thing.overlap_0=0x12345678UL; o.get_text=get_text;
    o.create_window=create_window; o.send_message=send_message;
    o.set_title_bar=set_title_bar; o.begin_paint=begin_paint;
    o.get_client_rect=get_client_rect; o.draw_frame=draw_frame;
    o.set_text_color=set_color; o.set_background_color=set_color;
    o.text_extent=text_extent; o.center_text=center_text;
    o.draw_arrow=draw_arrow; o.draw_survey=draw_survey;
    o.other_at_selection=other_at_selection; o.end_paint=end_paint;
    o.fill_rect=fill_rect; o.get_cursor_pos=get_cursor_pos;
    o.screen_to_client=screen_to_client; o.point_in_rect=point_in_rect;
    o.hit_test=hit_test; o.set_cursor=set_cursor; o.write_mem=write_mem;
    o.click=click; o.default_proc=default_proc; i.ops=&o; i.context=&p;
    if(stars_mine_wnd_proc(&i,9,1,0,0)!=0 || p.create!=1 ||
            p.font_message!=1 || p.title_bar!=1 || i.combo_window!=42) ++failures;
    if(stars_mine_wnd_proc(&i,9,0x0f,0,0)!=0 || p.frame!=1 ||
            p.center!=1 || p.arrow!=1 || p.survey!=1 || p.end!=1) ++failures;
    if(stars_mine_wnd_proc(&i,9,0x14,8,0)!=1 || p.fill!=1) ++failures;
    if(stars_mine_wnd_proc(&i,9,0x19,8,0)!=(StarsI32)6 || p.color<3) ++failures;
    if(stars_mine_wnd_proc(&i,9,0x20,0,0)!=1 || p.cursor_id!=33) ++failures;
    if(stars_mine_wnd_proc(&i,9,0x111,0,(StarsU32)42)!=0 ||
            p.write_mem!=1 || p.written_value!=0x12345678UL ||
            p.thing.cargo_value!=1) ++failures;
    if(stars_mine_wnd_proc(&i,9,0x201,2,0x00060005UL)!=0 || p.click!=1) ++failures;
    i.popup_window=77;
    if(stars_mine_wnd_proc(&i,9,0x201,2,0x00060005UL)!=0 || p.click!=1) ++failures;
    if(stars_mine_wnd_proc(&i,9,0x123,0,0)!=77L || p.default_proc!=1) ++failures;
#ifndef TARGET_NATIVE
    printf("MINE_WND_PROC_CHECKS=9 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
