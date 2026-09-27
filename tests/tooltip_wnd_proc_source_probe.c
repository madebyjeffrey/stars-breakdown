#include <stdio.h>
#include "core.h"

typedef struct Trace {
    StarsI16 timers;
    StarsI16 kills;
    StarsI16 destroys;
    StarsI16 positions;
    StarsI16 paints;
    StarsI16 erases;
    StarsI16 text_outs;
    StarsI16 inside;
    StarsI16 visible;
    StarsU16 last_delay;
    StarsI16 last_x;
    StarsI16 last_y;
    StarsU32 tick;
} Trace;
static StarsU16 begin_paint(void *c, StarsU16 w, StarsRect *r)
{ (void)w; (void)r; ++((Trace *)c)->paints; return 5U; }
static void end_paint(void *c, StarsU16 w, const StarsRect *r)
{ (void)c; (void)w; (void)r; }
static void client_rect(void *c, StarsU16 w, StarsRect *r)
{ (void)c; (void)w; r->left=0;r->top=0;r->right=80;r->bottom=20; }
static void fill_rect(void *c, StarsU16 dc, const StarsRect *r, StarsU16 b)
{ (void)dc; (void)r; (void)b; ++((Trace *)c)->erases; }
static void frame_rect(void *c, StarsU16 dc, const StarsRect *r, StarsU16 b)
{ (void)c; (void)dc; (void)r; (void)b; }
static StarsU16 select_object(void *c, StarsU16 dc, StarsU16 object)
{ (void)dc; (void)object; ++((Trace *)c)->paints; return 4U; }
static StarsI16 set_bk_mode(void *c, StarsU16 dc, StarsI16 mode)
{ (void)c; (void)dc; (void)mode; return 2; }
static void set_text_color(void *c, StarsU16 dc, StarsU32 color)
{ (void)c; (void)dc; (void)color; }
static void text_out(void *c, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{ (void)dc; (void)x; (void)y; (void)text; (void)length; ++((Trace *)c)->text_outs; }
static StarsI16 get_string(void *c, StarsI16 id, StarsU8 *text)
{ (void)c; (void)id; text[0]='T';text[1]='i';text[2]='p';text[3]=0; return 3; }
static StarsI16 set_timer(void *c, StarsU16 w, StarsU16 id, StarsU16 delay)
{ Trace *t=(Trace *)c;(void)w;(void)id;++t->timers;t->last_delay=delay;return 1; }
static StarsI16 kill_timer(void *c, StarsU16 w, StarsU16 id, StarsU16 unused)
{ (void)w;(void)id;(void)unused;++((Trace *)c)->kills;return 1; }
static StarsU32 get_tick(void *c)
{ return ((Trace *)c)->tick; }
static void get_cursor(void *c, StarsPoint *p)
{ (void)c;p->x=100;p->y=50; }
static StarsI16 point_in_rect(void *c, const StarsRect *r, StarsPoint p)
{ (void)r;(void)p;return ((Trace *)c)->inside; }
static void screen_to_client(void *c, StarsU16 w, StarsPoint *p)
{ (void)c;(void)w;p->x=(StarsI16)(p->x-10);p->y=(StarsI16)(p->y-20); }
static void client_to_screen(void *c, StarsU16 w, StarsPoint *p)
{ (void)c;(void)w;p->x=(StarsI16)(p->x+10);p->y=(StarsI16)(p->y+20); }
static void set_window_pos(void *c, StarsU16 w, StarsU16 z, StarsI16 x,
    StarsI16 y, StarsI16 cx, StarsI16 cy, StarsU16 flags)
{ Trace *t=(Trace *)c;(void)w;(void)z;(void)cx;(void)cy;(void)flags;++t->positions;t->last_x=x;t->last_y=y; }
static void update_window(void *c, StarsU16 w)
{ (void)c; (void)w; }
static StarsU16 is_visible(void *c, StarsU16 w)
{ (void)w;return (StarsU16)((Trace *)c)->visible; }
static void destroy_window(void *c, StarsU16 w)
{ (void)w;++((Trace *)c)->destroys; }

static void initialize(StarsTooltipWndInput *i, Trace *t)
{
    i->tooltip_window=0;i->tooltip_id=7;i->timer_id=-1;
    i->first_visible_tick=0;i->last_visible_tick=0;
    i->tooltip_rect.left=0;i->tooltip_rect.top=0;
    i->tooltip_rect.right=200;i->tooltip_rect.bottom=100;
    i->frame_window=9;i->window_frame_brush=1;i->tooltip_brush=2;i->font=3;
    i->text_color=4;i->font_height=8;i->tip_width=40;i->screen_width=120;
    i->ops.begin_paint=begin_paint;i->ops.end_paint=end_paint;
    i->ops.get_client_rect=client_rect;i->ops.fill_rect=fill_rect;
    i->ops.frame_rect=frame_rect;i->ops.select_object=select_object;
    i->ops.set_background_mode=set_bk_mode;i->ops.set_text_color=set_text_color;
    i->ops.text_out=text_out;i->ops.get_string=get_string;
    i->ops.set_timer=set_timer;i->ops.kill_timer=kill_timer;
    i->ops.get_tick=get_tick;i->ops.get_cursor_pos=get_cursor;
    i->ops.point_in_rect=point_in_rect;i->ops.screen_to_client=screen_to_client;
    i->ops.client_to_screen=client_to_screen;i->ops.set_window_pos=set_window_pos;
    i->ops.update_window=update_window;i->ops.is_visible=is_visible;
    i->ops.destroy_window=destroy_window;i->ops.default_proc=0;
    i->ops.context=t;
    t->timers=t->kills=t->destroys=t->positions=t->paints=t->erases=0;
    t->text_outs=0;t->inside=1;t->visible=1;t->last_delay=0;
    t->last_x=t->last_y=0;t->tick=100;
}

int main(void)
{
    StarsTooltipWndInput input; Trace trace; int failures=0;
    initialize(&input,&trace);
    if(stars_tooltip_wnd_proc(&input,77,1,0,0)!=0L||input.tooltip_window!=77)
        ++failures;
    if(stars_tooltip_wnd_proc(&input,77,0x14,6,0)!=1L||trace.erases!=1)
        ++failures;
    if(stars_tooltip_wnd_proc(&input,77,0x5f3,0,0)!=0L||trace.timers!=1||
            trace.last_delay!=700) ++failures;
    if(stars_tooltip_wnd_proc(&input,77,0x5f3,1,0)!=0L||trace.positions!=1||
            trace.last_delay!=50||trace.last_x!=85||trace.last_y!=62) ++failures;
    trace.tick=200;
    if(stars_tooltip_wnd_proc(&input,77,0x113,0x39e,0)!=0L||
            input.last_visible_tick!=200) ++failures;
    trace.inside=0;
    stars_tooltip_wnd_proc(&input,77,0x113,0x39e,0);
    if(trace.destroys!=1) ++failures;
    if(stars_tooltip_wnd_proc(&input,77,0x0f,0,0)!=0L||trace.text_outs!=1)
        ++failures;
    stars_tooltip_wnd_proc(&input,77,2,0,0);
    if(input.tooltip_window!=0||input.timer_id!=-1||trace.kills!=2) ++failures;
#ifndef TARGET_NATIVE
    printf("TOOLTIP_WND_PROC_CHECKS=8 FAILURES=%d RESULT=%s\n",failures,
        failures?"FAIL":"PASS");
#endif
    return failures!=0;
}
