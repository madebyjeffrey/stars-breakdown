#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 paint;
    StarsI16 fill;
    StarsI16 alert;
    StarsI16 focus;
    StarsI16 ended;
    StarsI16 helped;
    StarsI16 valid;
    StarsU16 result_brush;
} Trace;

static StarsU16 begin_paint(void *c, StarsU16 hwnd)
{ (void)hwnd; ++((Trace *)c)->paint; return 4; }
static void end_paint(void *c, StarsU16 hwnd, StarsU16 dc)
{ (void)c; (void)hwnd; (void)dc; }
static void get_client(void *c, StarsU16 hwnd, StarsRect *r)
{ (void)c; (void)hwnd; r->left=0; r->top=0; r->right=80; r->bottom=40; }
static void get_text_rect(void *c, StarsU16 hwnd, StarsU16 edit, StarsRect *r)
{ (void)c; (void)hwnd; (void)edit; r->left=8; r->top=8; r->right=70; r->bottom=20; }
static StarsU16 get_item(void *c, StarsU16 hwnd, StarsU16 id)
{ (void)c; (void)hwnd; return (StarsU16)(id+1); }
static StarsU16 select_font(void *c, StarsU16 dc, StarsU16 font)
{ (void)c; (void)dc; (void)font; return 0; }
static void set_color(void *c, StarsU16 dc, StarsU32 color)
{ (void)c; (void)dc; (void)color; }
static StarsI16 get_string(void *c, StarsU16 id, StarsU8 *s, StarsU16 n)
{ (void)c; (void)id; if(n){s[0]='h';s[1]=0;} return 1; }
static void draw_text(void *c, StarsU16 dc, const StarsU8 *s, StarsI16 n,
    const StarsRect *r, StarsU16 flags)
{ (void)c;(void)dc;(void)s;(void)n;(void)r;(void)flags; }
static void fill_rect(void *c, StarsU16 dc, const StarsRect *r, StarsU16 b)
{ (void)dc;(void)r;(void)b; ++((Trace *)c)->fill; }
static void send_item(void *c, StarsU16 hwnd, StarsU16 id, StarsU16 msg,
    StarsU16 wp, StarsU32 lp)
{ (void)c;(void)hwnd;(void)id;(void)msg;(void)wp;(void)lp; }
static void set_text(void *c, StarsU16 hwnd, const StarsU8 *s)
{ (void)c;(void)hwnd;(void)s; }
static void sticky(void *c, StarsU16 hwnd, StarsPoint *p, StarsI16 f)
{ (void)c;(void)hwnd;(void)p;(void)f; }
static StarsI16 get_item_text(void *c, StarsU16 hwnd, StarsU16 id,
    StarsU8 *s, StarsU16 n)
{ (void)c;(void)hwnd;(void)id;(void)n; s[0]='x';s[1]=0; return 1; }
static StarsI16 validate(void *c, const StarsU8 *s)
{ (void)s; return ((Trace *)c)->valid; }
static void format_error(void *c, StarsU16 id, StarsU8 *s, StarsU16 n)
{ (void)c;(void)id; if(n){s[0]='e';s[1]=0;} }
static void alert(void *c, const StarsU8 *s, StarsI16 style)
{ (void)s;(void)style; ++((Trace *)c)->alert; }
static void focus(void *c, StarsU16 hwnd)
{ (void)hwnd; ++((Trace *)c)->focus; }
static void end_dialog(void *c, StarsU16 hwnd, StarsI16 result)
{ (void)hwnd;(void)result; ++((Trace *)c)->ended; }
static void win_help(void *c, StarsU16 hwnd, StarsU16 file, StarsU16 id)
{ (void)hwnd;(void)file;(void)id; ++((Trace *)c)->helped; }

static void initialize(StarsMsgDlgInput *input, StarsMsgDlgOps *ops,
    Trace *trace)
{
    memset(input,0,sizeof *input); memset(ops,0,sizeof *ops);
    memset(trace,0,sizeof *trace); input->hwnd=1; input->ops=ops;
    input->button_brush=0x2345; input->button_face_color=0x00abcdefUL;
    input->help_file=8; input->context=trace;
    ops->begin_paint=begin_paint;ops->end_paint=end_paint;
    ops->get_client_rect=get_client;ops->get_text_rect=get_text_rect;
    ops->get_dialog_item=get_item;ops->select_font=select_font;
    ops->set_background_color=set_color;ops->set_text_color=set_color;
    ops->get_string=get_string;ops->draw_text=draw_text;ops->fill_rect=fill_rect;
    ops->send_item_message=send_item;ops->set_window_text=set_text;
    ops->sticky_position=sticky;ops->get_item_text=get_item_text;
    ops->validate_serial=validate;ops->format_error=format_error;
    ops->alert=alert;ops->set_focus=focus;ops->end_dialog=end_dialog;
    ops->win_help=win_help; trace->valid=1;
}

int main(void)
{
    StarsMsgDlgInput input; StarsMsgDlgOps ops; Trace trace; int failures=0;
    initialize(&input,&ops,&trace); input.message=STARS_MSG_DLG_PAINT;
    if(!stars_msg_dlg(&input)||trace.paint!=1)++failures;
    input.message=STARS_MSG_DLG_ERASE_BACKGROUND; input.wparam=2;
    if(!stars_msg_dlg(&input)||trace.fill!=1)++failures;
    input.message=STARS_MSG_DLG_CONTROL_COLOR; input.wparam=2;
    input.lparam=((StarsU32)STARS_MSG_DLG_EDIT_CONTROL_TYPE<<16);
    if(stars_msg_dlg(&input)!=(StarsI16)input.button_brush)++failures;
    input.message=STARS_MSG_DLG_COMMAND; input.wparam=STARS_MSG_DLG_ACCEPT;
    if(!stars_msg_dlg(&input)||trace.ended!=1)++failures;
    trace.valid=0; if(stars_msg_dlg(&input)||trace.alert!=1||trace.focus!=1)++failures;
    input.wparam=STARS_MSG_DLG_HELP;
    if(!stars_msg_dlg(&input)||trace.helped!=1)++failures;
#ifndef TARGET_NATIVE
    printf("MSG_DLG_CHECKS=6 FAILURES=%d RESULT=%s\n",failures,
        failures?"FAIL":"PASS");
#endif
    return failures!=0;
}
