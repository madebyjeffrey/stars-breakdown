#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 creates;
    StarsI16 sends;
    StarsI16 positions;
    StarsI16 defaults;
    StarsI16 moves;
    StarsI16 finishes;
    StarsI16 titles;
    StarsI16 filters;
    StarsI16 focus;
    StarsI16 paints;
    StarsI16 bodies;
    StarsI16 cursors;
    StarsI16 objects;
    StarsI16 dirty;
    StarsI16 tutor;
    StarsI16 next_value;
    StarsI16 previous_value;
} Trace;

static void create_window(void *c, StarsU16 *out, StarsU16 kind,
    StarsU32 style, StarsI16 x, StarsI16 y, StarsI16 w, StarsI16 h,
    StarsU16 parent)
{
    Trace *t=(Trace *)c; (void)kind;(void)style;(void)x;(void)y;(void)w;
    (void)h;(void)parent; ++t->creates; *out=(StarsU16)(100+t->creates);
}
static void send_child(void *c, StarsU16 hwnd, StarsU16 msg, StarsU16 wp,
    StarsU32 lp)
{ (void)hwnd;(void)msg;(void)wp;(void)lp;++((Trace *)c)->sends; }
static void set_position(void *c, StarsU16 hwnd, StarsI16 x, StarsI16 y,
    StarsI16 w, StarsI16 h, StarsU16 flags)
{ (void)hwnd;(void)x;(void)y;(void)w;(void)h;(void)flags;++((Trace *)c)->positions; }
static StarsI32 default_proc(void *c, StarsU16 hwnd, StarsU16 msg,
    StarsU16 wp, StarsU32 lp)
{ (void)hwnd;(void)msg;(void)wp;(void)lp;++((Trace *)c)->defaults;return 77; }
static StarsI16 async_key(void *c, StarsU16 key)
{ (void)c;(void)key;return 0; }
static void get_point(void *c, StarsPoint *p)
{ (void)c;p->x=5;p->y=6; }
static void screen_to_client(void *c, StarsU16 hwnd, StarsPoint *p)
{ (void)c;(void)hwnd;(void)p; }
static void set_cursor(void *c, StarsU16 cursor)
{ (void)cursor;++((Trace *)c)->cursors; }
static void get_client(void *c, StarsU16 hwnd, StarsRect *r)
{ (void)c;(void)hwnd;r->left=0;r->top=0;r->right=90;r->bottom=50; }
static void fill_rect(void *c, StarsU16 dc, const StarsRect *r, StarsU16 b)
{ (void)c;(void)dc;(void)r;(void)b; }
static void set_color(void *c, StarsU16 dc, StarsU32 color)
{ (void)c;(void)dc;(void)color; }
static StarsU16 begin_paint(void *c, StarsU16 hwnd)
{ (void)hwnd;++((Trace *)c)->paints;return 3; }
static void end_paint(void *c, StarsU16 hwnd, StarsU16 dc)
{ (void)c;(void)hwnd;(void)dc; }
static void draw_frame(void *c, StarsU16 dc, const StarsRect *r)
{ (void)c;(void)dc;(void)r; }
static void draw_title(void *c, StarsU16 dc, const StarsRect *r,
    const StarsU8 *s, StarsI16 n)
{ (void)c;(void)dc;(void)r;(void)s;(void)n; }
static void draw_body(void *c, StarsU16 dc, const StarsRect *r,
    const StarsU8 *s, StarsI16 n)
{ (void)dc;(void)r;(void)s;(void)n;++((Trace *)c)->bodies; }
static StarsI16 hit_test(void *c, StarsPoint p)
{ (void)c;(void)p;return 2; }
static void set_text(void *c, StarsU16 hwnd, const StarsU8 *s)
{ (void)c;(void)hwnd;(void)s; }
static StarsI16 player_name(void *c, StarsI16 player, StarsU8 *s, StarsU16 n)
{ (void)c;(void)player;if(n){s[0]='P';s[1]=0;}return 1; }
static void format_title(void *c, StarsI16 mode, StarsU8 *s, StarsU16 n)
{ (void)mode;++((Trace *)c)->titles;if(n){s[0]='T';s[1]=0;} }
static void set_msg_title(void *c, StarsU16 hwnd)
{ (void)hwnd;++((Trace *)c)->titles; }
static void finish(void *c, StarsI16 direction)
{ (void)direction;++((Trace *)c)->finishes; }
static StarsI16 next_selection(void *c, StarsI16 filtered)
{ (void)filtered;++((Trace *)c)->moves;return ((Trace *)c)->next_value; }
static StarsI16 previous_selection(void *c, StarsI16 filtered)
{ (void)filtered;++((Trace *)c)->moves;return ((Trace *)c)->previous_value; }
static StarsI16 message_id(void *c, StarsI16 current)
{ (void)c;return current; }
static void dirty_game(void *c, StarsI16 dirty)
{ (void)dirty;++((Trace *)c)->dirty; }
static void advance_tutor(void *c)
{ ++((Trace *)c)->tutor; }
static void object_action(void *c, StarsI16 kind, StarsI16 id,
    StarsI16 x, StarsI16 y)
{ (void)kind;(void)id;(void)x;(void)y;++((Trace *)c)->objects; }
static void set_focus(void *c, StarsU16 hwnd)
{ (void)hwnd;++((Trace *)c)->focus; }

static void initialize(StarsMessageWndState *s, StarsMessageWndOps *o,
    Trace *t)
{
    memset(s,0,sizeof *s);memset(o,0,sizeof *o);memset(t,0,sizeof *t);
    s->font=9;s->hand_cursor=8;s->button_brush=7;s->button_face_color=6;
    s->player_count=3;s->arial8_height=8;s->ship_dropdown_height=10;
    s->win_frame_width=3;s->tutor_active=1;s->ops=o;s->context=t;
    s->messages.message_count=4;s->messages.player_message_count=2;
    s->messages.current_message=0;t->next_value=1;t->previous_value=0;
    o->create_window=create_window;o->send_child_message=send_child;
    o->set_position=set_position;o->default_proc=default_proc;
    o->async_key_state=async_key;o->get_cursor_pos=get_point;
    o->screen_to_client=screen_to_client;o->set_cursor=set_cursor;
    o->get_client_rect=get_client;o->fill_rect=fill_rect;
    o->set_background_color=set_color;o->begin_paint=begin_paint;
    o->end_paint=end_paint;o->draw_frame=draw_frame;o->draw_title=draw_title;
    o->draw_body=draw_body;o->hit_test=hit_test;o->set_window_text=set_text;
    o->player_name=player_name;o->format_title=format_title;
    o->set_msg_title=set_msg_title;o->finish_entry=finish;
    o->next_selection=next_selection;o->previous_selection=previous_selection;
    o->message_id=message_id;o->dirty_game=dirty_game;
    o->advance_tutor=advance_tutor;o->object_action=object_action;
    o->set_focus=set_focus;
}

int main(void)
{
    StarsMessageWndState s;StarsMessageWndOps o;Trace t;int failures=0;
    initialize(&s,&o,&t);
    if(stars_message_wnd_proc(&s,1,STARS_MESSAGE_WM_CREATE,0,0)!=0||
            t.creates!=7||t.sends!=12)++failures;
    if(stars_message_wnd_proc(&s,1,STARS_MESSAGE_WM_SIZE,0,
            ((StarsU32)60<<16)|120U)!=77||t.positions!=6)++failures;
    s.send_mode=0;
    stars_message_wnd_proc(&s,1,STARS_MESSAGE_WM_KEYDOWN,
        STARS_MESSAGE_KEY_DOWN,0);
    if(s.messages.current_message!=1||t.tutor!=1)++failures;
    s.messages.current_message=0;
    stars_message_wnd_proc(&s,1,STARS_MESSAGE_WM_CHAR,
        STARS_MESSAGE_CHAR_TOGGLE_FILTER,0);
    if(t.dirty!=1||!s.messages.filtered[0])++failures;
    s.send_mode=1;
    stars_message_wnd_proc(&s,1,STARS_MESSAGE_WM_KEYDOWN,
        STARS_MESSAGE_KEY_UP,0);
    if(t.finishes!=1)++failures;
    stars_message_wnd_proc(&s,1,STARS_MESSAGE_WM_PAINT,0,0);
    if(t.paints!=1||t.bodies!=1)++failures;
    s.buttons[0]=101;s.buttons[1]=102;s.buttons[2]=103;s.buttons[3]=104;
    stars_message_wnd_proc(&s,1,STARS_MESSAGE_WM_COMMAND,0,
        ((StarsU32)0<<16)|102U);
    if(s.send_mode!=0||t.finishes!=2)++failures;
    if(stars_message_wnd_proc(&s,1,STARS_MESSAGE_WM_ERASE_BACKGROUND,2,0)!=1)
        ++failures;
    if(stars_message_wnd_proc(&s,1,STARS_MESSAGE_WM_SETCURSOR,0,0)!=1||
            t.cursors!=1)++failures;
#ifndef TARGET_NATIVE
    printf("MESSAGE_WND_PROC_CHECKS=9 FAILURES=%d RESULT=%s\n",failures,
        failures?"FAIL":"PASS");
#endif
    return failures!=0;
}
