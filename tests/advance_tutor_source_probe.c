#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct T { StarsI16 calls, errors, ended, text, shown, rects, points,
    expanded, invalidated; StarsI16 error_id; StarsU16 show; StarsRect rect; }
    T;
static StarsI16 done(void *v) { T *t=(T *)v; ++t->calls; return t->calls < 2; }
static const StarsU8 *fmt(void *v) { (void)v; return (const StarsU8 *)"TUTOR %d"; }
static void format_title(void *v, StarsU8 *d, const StarsU8 *f, StarsI16 n)
{ T *t=(T *)v; (void)f; ++t->text; sprintf((char *)d,"TUTOR %d",n); }
static void set_text(void *v, StarsU16 w, const StarsU8 *s)
{ T *t=(T *)v; (void)w; (void)s; ++t->text; }
static void show(void *v, StarsI16 n) { T *t=(T *)v; t->show=(StarsU16)n; ++t->shown; }
static void get_rect(void *v, StarsU16 w, StarsRect *r)
{ T *t=(T *)v; (void)w; ++t->rects; r->left=10;r->top=20;r->right=50;r->bottom=60; }
static void point(void *v, StarsU16 w, StarsPoint *p)
{ T *t=(T *)v; (void)w; ++t->points; p->x=(StarsI16)(p->x-2);p->y=(StarsI16)(p->y-3); }
static void expand(void *v, StarsRect *r, StarsI16 x, StarsI16 y)
{ T *t=(T *)v; ++t->expanded; r->left=(StarsU16)(r->left+x);r->top=(StarsU16)(r->top+y);r->right=(StarsU16)(r->right-x);r->bottom=(StarsU16)(r->bottom-y); }
static void inval(void *v, StarsU16 w, const StarsRect *r, StarsI16 e)
{ T *t=(T *)v; (void)w;(void)e;t->rect=*r;++t->invalidated; }
static void error(void *v, StarsI16 n) { T *t=(T *)v;t->error_id=n;++t->errors; }
static void end(void *v, StarsI16 n) { T *t=(T *)v;(void)n;++t->ended; }
int main(void)
{
    StarsAdvanceTutorOps o; StarsAdvanceTutorState s; T t;
    memset(&o,0,sizeof o); memset(&s,0,sizeof s); memset(&t,0,sizeof t);
    o.task_done=done;o.title_format=fmt;o.format_title=format_title;
    o.set_window_text=set_text;o.show_tutor=show;o.get_window_rect=get_rect;
    o.screen_to_client=point;o.expand_rect=expand;o.invalidate_rect=inval;
    o.tutor_error=error;o.end_tutor=end;
    s.flags=0;s.idt=8;s.idt_bold=0;s.window=4;s.arial8_height=8;
    stars_advance_tutor(&s,&o,&t);
    if (s.idt!=16||s.idt_bold!=16||s.ids_error!=-1||t.calls!=2||
        t.shown!=1||t.rects!=1||t.points!=2||t.expanded!=1||
        t.invalidated!=1||t.errors!=0||t.ended!=0) return 1;
    memset(&t,0,sizeof t); memset(&s,0,sizeof s); s.flags=0x10;s.idt=640;
    s.ids_error=0; stars_advance_tutor(&s,&o,&t);
    if (t.errors!=1||t.error_id!=0x20a||t.ended!=1) return 2;
    puts("ADVANCE_TUTOR_SOURCE_CHECKS=2 FAILURES=0 RESULT=PASS"); return 0;
}
