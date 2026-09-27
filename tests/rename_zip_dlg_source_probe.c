#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 rects, fills, colors, titles, positions, edits, gets, stickies;
    StarsI16 ends, helps;
    StarsU16 title_id, end_result, help_id;
    StarsU32 brush, color;
    StarsRect rect;
    StarsPoint position;
    StarsU8 title[32];
    StarsU8 work[14];
} Trace;

static void get_rect(void *value, StarsU16 dialog, StarsRect *rect)
{ Trace *t=(Trace *)value; t->rects++; rect->left=1; rect->top=2; rect->right=3; rect->bottom=4; (void)dialog; }
static void fill_rect(void *value, StarsU16 dc, const StarsRect *rect, StarsU32 brush)
{ Trace *t=(Trace *)value; t->fills++; t->brush=brush; t->rect=*rect; (void)dc; }
static void set_color(void *value, StarsU16 dc, StarsU32 color)
{ Trace *t=(Trace *)value; t->colors++; t->color=color; (void)dc; }
static const StarsU8 *get_string(void *value, StarsU16 id)
{ Trace *t=(Trace *)value; t->titles++; t->title_id=id; return (const StarsU8 *)"Rename"; }
static void set_text(void *value, StarsU16 window, const StarsU8 *text)
{ Trace *t=(Trace *)value; if (window==9U) strcpy((char *)t->title,(const char *)text); else strcpy((char *)t->work,(const char *)text); }
static void set_pos(void *value, StarsU16 window, StarsI16 x, StarsI16 y, StarsU16 flags)
{ Trace *t=(Trace *)value; t->positions++; t->position.x=x; t->position.y=y; (void)window; (void)flags; }
static void send_edit(void *value, StarsU16 d, StarsU16 id, StarsU16 msg, StarsU16 w, StarsU32 l)
{ Trace *t=(Trace *)value; t->edits++; (void)d; (void)id; (void)msg; (void)w; (void)l; }
static StarsU16 get_item(void *value, StarsU16 d, StarsU16 id)
{ Trace *t=(Trace *)value; t->gets++; (void)d; (void)id; return 77U; }
static void get_text(void *value, StarsU16 d, StarsU16 id, StarsU8 *out, StarsU16 n)
{ Trace *t=(Trace *)value; t->work[0]='N'; t->work[1]='E'; t->work[2]=0; (void)d; (void)id; (void)out; (void)n; }
static void sticky(void *value, StarsU16 d, StarsPoint *p, StarsI16 save)
{ Trace *t=(Trace *)value; t->stickies++; (void)d; (void)p; (void)save; }
static void end_dialog(void *value, StarsU16 d, StarsU16 result)
{ Trace *t=(Trace *)value; t->ends++; t->end_result=result; (void)d; }
static void help(void *value, StarsU16 d, const StarsU8 *file, StarsU16 id)
{ Trace *t=(Trace *)value; t->helps++; t->help_id=id; (void)d; (void)file; }

int main(void)
{
    StarsRenameZipInput input;
    Trace trace;
    StarsI16 result;
    StarsRect expected;

    memset(&input,0,sizeof input); memset(&trace,0,sizeof trace);
    input.sticky_position.x=10; input.sticky_position.y=20;
    input.button_face_brush=55UL; input.button_face_color=66UL;
    input.work[0]='O'; input.work[1]='K'; input.work[2]=0;
    input.help_file=(const StarsU8 *)"HELP.HLP";
    input.get_client_rect=get_rect; input.fill_rect=fill_rect;
    input.set_background_color=set_color; input.get_string=get_string;
    input.set_window_text=set_text; input.set_window_pos=set_pos;
    input.send_dialog_item_message=send_edit; input.get_dialog_item=get_item;
    input.get_dialog_text=get_text; input.sticky_position_call=sticky;
    input.end_dialog=end_dialog; input.win_help=help; input.context=&trace;
    expected.left=1; expected.top=2; expected.right=3; expected.bottom=4;
    result=stars_rename_zip_dlg(9U,0x14U,88U,0UL,&input);
    if(result!=1||trace.rects!=1||trace.fills!=1||trace.brush!=55UL||
       memcmp(&trace.rect,&expected,sizeof expected)!=0)return 1;
    result=stars_rename_zip_dlg(9U,0x19U,88U,0x00060000UL,&input);
    if(result!=55||trace.colors!=1||trace.color!=66UL)return 2;
    result=stars_rename_zip_dlg(9U,0x110U,0,0,&input);
    if(result!=1||trace.title_id!=0x4c3U||strcmp((char *)trace.title,"Rename")!=0||
       trace.positions!=1||trace.position.x!=0x50||trace.position.y!=0x5a||
       trace.edits!=1||trace.gets!=1||trace.stickies!=1)return 3;
    result=stars_rename_zip_dlg(9U,0x111U,1U,0,&input);
    if(result!=1||trace.ends!=1||trace.end_result!=1U||trace.stickies!=2||
       strcmp((char *)trace.work,"NE")!=0)return 4;
    result=stars_rename_zip_dlg(9U,0x111U,0x76U,0,&input);
    if(result!=1||trace.helps!=1||trace.help_id!=0x452U)return 5;
    puts("RENAME_ZIP_DLG_SOURCE=PASS"); return 0;
}
