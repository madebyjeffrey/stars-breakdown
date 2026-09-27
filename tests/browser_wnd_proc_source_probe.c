#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 calls;
    StarsU16 copied_kind;
    StarsI16 popup_x;
    StarsI16 popup_y;
} Trace;

static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{ (void)context; (void)hwnd; rect->left=1; return 7U; }
static void get_client_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{ (void)context; (void)hwnd; rect->top=2; rect->right=40; rect->bottom=30; }
static void draw_component(void *context, StarsU16 dc, StarsU16 hwnd,
    StarsI16 x, StarsI16 y)
{ Trace *t=(Trace *)context; (void)dc; (void)hwnd; (void)x; (void)y; ++t->calls; }
static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{ (void)context; (void)hwnd; (void)rect; }
static void fill_rect(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU32 brush)
{ Trace *t=(Trace *)context; (void)dc; (void)rect; (void)brush; ++t->calls; }
static void copy_hull(void *context, StarsU16 id, StarsU16 starbase)
{ Trace *t=(Trace *)context; (void)id; t->copied_kind=starbase; }
static void normalize_hull(void *context, StarsU16 id)
{ (void)context; (void)id; }
static void set_cursor(void *context, StarsU16 cursor)
{ (void)context; (void)cursor; }
static void popup(void *context, StarsU16 hwnd, StarsI16 x, StarsI16 y)
{ Trace *t=(Trace *)context; (void)hwnd; t->popup_x=x; t->popup_y=y; }
static StarsI32 default_proc(void *context, StarsU16 hwnd, StarsU16 msg,
    StarsU16 wp, StarsU32 lp)
{ (void)context; (void)hwnd; (void)msg; (void)wp; (void)lp; return 9; }

int main(void)
{
    StarsBrowserWndOps ops;
    StarsBrowserWndInput input;
    Trace trace;
    memset(&ops,0,sizeof ops); memset(&input,0,sizeof input);
    memset(&trace,0,sizeof trace);
    ops.begin_paint=begin_paint; ops.get_client_rect=get_client_rect;
    ops.draw_component=draw_component; ops.end_paint=end_paint;
    ops.fill_rect=fill_rect; ops.copy_hull=copy_hull;
    ops.normalize_hull=normalize_hull; ops.set_cursor=set_cursor;
    ops.popup=popup; ops.default_proc=default_proc;
    input.browser_grhst=0x0400U; input.browser_id=3U;
    input.arial_height=8; input.button_face_brush=4UL;
    input.help_cursor=5U; input.ops=&ops; input.context=&trace;
    if (stars_browser_wnd_proc(&input,1U,0x000fU,2U,0UL)!=0 ||
            trace.calls!=1U) return 1;
    if (stars_browser_wnd_proc(&input,1U,0x0014U,2U,0UL)!=1 ||
            trace.calls!=2U) return 2;
    if (stars_browser_wnd_proc(&input,1U,0x0201U,0U,
            0x00190020UL)!=1 || trace.copied_kind!=1U ||
            trace.popup_x!=32 || trace.popup_y!=25 || input.popup_kind!=14U)
        return 3;
    input.browser_grhst=0U;
    if (stars_browser_wnd_proc(&input,1U,0x0201U,0U,
            0x00190020UL)!=9) return 4;
    if (stars_browser_wnd_proc(&input,1U,0x1234U,0U,0UL)!=9) return 5;
    puts("BROWSER_WND_PROC_SOURCE=PASS");
    return 0;
}
