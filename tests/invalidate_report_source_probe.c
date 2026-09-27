#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 rects;
    StarsI16 invalidates;
    StarsI16 full;
    StarsI16 sorts;
    StarsI16 ranges;
    StarsI16 last_report;
    StarsI16 last_sort;
    StarsI16 last_max;
} Trace;

static void get_rect(void *v, StarsU16 w, StarsRect *r)
{
    Trace *t = (Trace *)v;
    (void)w;
    ++t->rects;
    r->left = 1; r->top = 2; r->right = 99; r->bottom = 80;
}
static void invalidate(void *v, StarsU16 w, const StarsRect *r, StarsI16 e)
{
    Trace *t = (Trace *)v;
    (void)w; (void)r;
    ++t->invalidates;
    t->full = e;
}
static void range(void *v, StarsU16 w, StarsU16 b, StarsI16 n,
    StarsI16 m, StarsI16 e)
{
    Trace *t = (Trace *)v;
    (void)w; (void)b; (void)n; (void)e;
    ++t->ranges; t->last_max = m;
}
static void sort_cache(void *v, StarsI16 r, StarsI16 c)
{
    Trace *t = (Trace *)v;
    ++t->sorts; t->last_report = r; t->last_sort = c;
}

int main(void)
{
    StarsInvalidateReportOps ops;
    StarsInvalidateReportInput in;
    StarsInvalidateReportRecord planet, fleet;
    StarsInvalidateReportRecord *current;
    void *ids;
    Trace trace;

    memset(&ops, 0, sizeof ops);
    ops.get_client_rect = get_rect;
    ops.invalidate_rect = invalidate;
    ops.set_scroll_range = range;
    ops.sort_cache = sort_cache;
    memset(&in, 0, sizeof in);
    memset(&planet, 0, sizeof planet);
    memset(&fleet, 0, sizeof fleet);
    memset(&trace, 0, sizeof trace);
    planet.report = 0; planet.sort_column = 3; planet.rows = 12;
    planet.rows_visible = 4; planet.vertical_scroll = 8;
    fleet.report = 1; fleet.sort_column = 5; fleet.rows = 20;
    fleet.rows_visible = 6; fleet.vertical_scroll = 9;
    current = &planet; ids = (void *)0x1234;
    in.report = 0; in.reload = 1; in.report_window = 7;
    in.arial8_height = 8; in.current = &current; in.planet_report = &planet;
    in.fleet_report = &fleet; in.report_ids = &ids; in.ops = &ops;
    in.context = &trace;
    stars_invalidate_report(&in);
    if (planet.cached != 0 || trace.rects != 1 || trace.invalidates != 2 ||
            trace.ranges != 1 || trace.last_max != 8 || trace.sorts != 1)
        return 1;

    memset(&trace, 0, sizeof trace);
    current = &planet; ids = (void *)0x5678;
    in.report = 1; in.reload = 1;
    stars_invalidate_report(&in);
    if (current != &planet || ids != (void *)0x5678 || fleet.cached != 0 ||
            trace.sorts != 1 || trace.last_report != 1)
        return 2;

    memset(&trace, 0, sizeof trace);
    current = (StarsInvalidateReportRecord *)0;
    in.report = 0; in.reload = 1;
    stars_invalidate_report(&in);
    if (current != (StarsInvalidateReportRecord *)0 || trace.sorts != 1)
        return 3;

    puts("INVALIDATE_REPORT_SOURCE_CHECKS=3 FAILURES=0 RESULT=PASS");
    return 0;
}
