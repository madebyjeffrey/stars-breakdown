#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    int select_calls, mode_calls, pat_calls, fill_calls, text_calls;
    int center_calls, frame_calls, header_calls, item_calls;
    StarsU16 last_dc;
    StarsI16 last_row, last_field;
    StarsU8 widths[3];
} Trace;
static void select_object(void *v, StarsU16 dc, StarsU16 object)
{ Trace *t=(Trace *)v; (void)object; t->last_dc=dc; ++t->select_calls; }
static void set_mode(void *v, StarsU16 dc, StarsU16 mode)
{ Trace *t=(Trace *)v; (void)dc; if(mode==1)++t->mode_calls; }
static void pat(void *v, StarsU16 dc, StarsI16 x, StarsI16 y, StarsI16 w,
    StarsI16 h, StarsU32 rop)
{ Trace *t=(Trace *)v; (void)dc;(void)x;(void)y;(void)w;(void)h;if(rop==0x00f00021UL)++t->pat_calls; }
static void fill(void *v, StarsU16 dc, const StarsRect *r, StarsU16 brush)
{ Trace *t=(Trace *)v; (void)dc;(void)r;if(brush==6)++t->fill_calls; }
static void text(void *v, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *s, StarsI16 n)
{ Trace *t=(Trace *)v; (void)dc;(void)x;(void)y;(void)s;if(n==4)++t->text_calls; }
static void center(void *v, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *s, StarsI16 n)
{ Trace *t=(Trace *)v; (void)dc;(void)x;(void)y;(void)s;if(n==0)++t->center_calls; }
static void frame(void *v, StarsU16 dc, const StarsRect *r, StarsU16 kind)
{ Trace *t=(Trace *)v; (void)dc;(void)r;(void)kind;++t->frame_calls; }
static StarsI16 header(void *v, StarsI16 report, StarsI16 field,
    StarsU8 *s, StarsU16 dc)
{ Trace *t=(Trace *)v; (void)report;(void)dc; ++t->header_calls; s[0]=(StarsU8)('A'+field); s[1]=field==0?'a':0; s[2]=field==0?'m':0; s[3]=field==0?'e':0; s[field==0?4:1]=0; return (StarsI16)(20-field*5); }
static void item(void *v, StarsU16 dc, const StarsRect *r, StarsI16 report,
    StarsI16 row, StarsI16 field)
{ Trace *t=(Trace *)v; (void)dc;(void)r;(void)report;t->last_row=row;t->last_field=field;++t->item_calls; }
int main(void)
{
    StarsReportViewState view; StarsDrawReportOps o; StarsDrawReportInput i;
    StarsRect paint; Trace t; int failures=0;
    memset(&view,0,sizeof view); memset(&o,0,sizeof o); memset(&i,0,sizeof i); memset(&t,0,sizeof t);
    view.fields=3; view.field_first=0; view.rows_visible=2; view.row_count=2;
    view.visible_fields=7UL; view.report_kind=1; paint.left=0; paint.top=0; paint.right=200; paint.bottom=100;
    o.select_object=select_object;o.set_background_mode=set_mode;o.pat_blt=pat;o.fill_rect=fill;o.text_out=text;o.center_text=center;o.draw_frame=frame;o.header_width=header;o.draw_item=item;
    i.view=&view;i.arial8_height=8;i.font=3;i.shadow_brush=5;i.button_face_brush=6;i.fill_cells=1;i.ops=&o;i.context=&t;
    stars_draw_report(4,9,&paint,&i);
    if(t.last_dc!=9)failures+=1;if(t.mode_calls!=1)failures+=2;if(t.header_calls!=3)failures+=4;if(t.frame_calls!=3)failures+=8;
    if(t.text_calls!=1)failures+=16;if(t.center_calls!=2)failures+=32;if(t.item_calls!=6)failures+=64;if(t.fill_calls!=9)failures+=128;
    if(view.column_width[0]!=10)failures+=256;if(view.column_width[1]!=7)failures+=512;if(view.column_width[2]!=5)failures+=1024;
    paint.top=50;paint.bottom=60;t.item_calls=0;stars_draw_report(4,9,&paint,&i);if(t.item_calls!=0)failures+=2048;
#ifndef TARGET_NATIVE
    if(failures==0)printf("DRAW_REPORT_CHECKS=3 FAILURES=0 RESULT=PASS\n");else printf("DRAW_REPORT_CHECKS=3 FAILURES=%d RESULT=FAIL\n",failures);
#endif
    return failures!=0;
}
