#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 fills;
    StarsU16 text_outs;
    StarsU16 suffixes;
    StarsU16 selected_brush;
    StarsU16 progress_width;
} Trace;

static StarsU16 stock(void *context, StarsU16 which)
{ (void)context; return (StarsU16)(which + 10U); }
static void fill(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsU16 brush)
{ Trace *t=(Trace *)context; (void)hdc; ++t->fills; t->selected_brush=brush;
  t->progress_width=(StarsU16)(rect->right-rect->left); }
static void inflate(void *context, StarsRect *rect, StarsI16 dx, StarsI16 dy)
{ (void)context; rect->left=(StarsU16)(rect->left-dx); rect->right=(StarsU16)(rect->right+dx);
  rect->top=(StarsU16)(rect->top-dy); rect->bottom=(StarsU16)(rect->bottom+dy); }
static StarsU32 set_color(void *context, StarsU16 hdc, StarsU32 color)
{ (void)context; (void)hdc; return color+1UL; }
static StarsU16 set_mode(void *context, StarsU16 hdc, StarsU16 mode)
{ (void)context; (void)hdc; return (StarsU16)(mode+1U); }
static StarsU16 select_font(void *context, StarsU16 hdc, StarsU16 font)
{ (void)context; (void)hdc; return (StarsU16)(font+1U); }
static StarsI16 extent(void *context, StarsU16 hdc, const StarsU8 *text,
    StarsU16 length)
{ (void)context; (void)hdc; (void)text; return (StarsI16)(length*2); }
static void text_out(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsU16 length)
{ Trace *t=(Trace *)context; (void)hdc; (void)x; (void)y; (void)text;
  (void)length; ++t->text_outs; }
static StarsU16 get_string(void *context, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{ (void)context; (void)id; if (capacity>4U) strcpy((char *)out,"UNIT"); return 4U; }
static void right_text(void *context, StarsU16 hdc, StarsI16 right,
    StarsI16 y, const StarsU8 *text, StarsU16 length, StarsI16 offset)
{ Trace *t=(Trace *)context; (void)hdc; (void)right; (void)y; (void)text;
  (void)length; (void)offset; ++t->suffixes; }

int main(void)
{
    StarsDrawProductionOps ops;
    StarsDrawProductionInput input;
    Trace trace;
    static const StarsU8 plain[] = " ITEM";
    static const StarsU8 progress_text[] = "P52  **ITEM";
    memset(&ops,0,sizeof ops); memset(&input,0,sizeof input);
    memset(&trace,0,sizeof trace);
    ops.get_stock_object=stock; ops.fill_rect=fill; ops.inflate_rect=inflate;
    ops.set_text_color=set_color; ops.set_bk_mode=set_mode;
    ops.select_font=select_font; ops.text_extent=extent;
    ops.text_out=text_out; ops.get_string=get_string;
    ops.right_text=right_text;
    input.hdc=1U; input.rect.left=0U; input.rect.top=0U;
    input.rect.right=100U; input.rect.bottom=20U; input.text=plain;
    input.window_brush=1U; input.blue_brush=2U; input.gray_brush=3U;
    input.red_brush=4U; input.green_brush=5U; input.button_face_brush=6U;
    input.window_color=7UL; input.blank_color=8UL; input.hash_color=9UL;
    input.ampersand_color=10UL; input.other_color=11UL;
    input.italic_font=12U; input.normal_font=13U;
    input.ops=&ops; input.context=&trace;
    input.listbox=0; input.inflate=1;
    stars_draw_production_item(&input);
    if (trace.fills!=1U || trace.text_outs!=1U || trace.suffixes!=0U)
        return 1;
    input.text=progress_text; input.listbox=1; input.inflate=0;
    stars_draw_production_item(&input);
    if (trace.fills!=3U || trace.suffixes!=1U) return 2;
    input.text=(const StarsU8 *)"# X"; input.selected=1; input.listbox=2;
    stars_draw_production_item(&input);
    if (trace.selected_brush!=6U) return 3;
    puts("DRAW_PRODUCTION_ITEM_SOURCE=PASS");
    return 0;
}
