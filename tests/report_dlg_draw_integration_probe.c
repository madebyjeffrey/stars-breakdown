#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 begin_calls;
    StarsI16 view_calls;
    StarsI16 header_calls;
    StarsI16 draw_item_calls;
    StarsI16 planet_calls;
    StarsI16 select_calls;
    StarsI16 text_calls;
    StarsI16 last_row;
    StarsI16 last_field;
    StarsI16 last_report;
} Trace;

static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    ++trace->begin_calls;
    rect->left = 0;
    rect->top = 0;
    rect->right = 200;
    rect->bottom = 100;
    return 9;
}

static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context;
    (void)hwnd;
    (void)rect;
}

static StarsI16 report_header(void *context, StarsI16 report,
    StarsI16 field, StarsU8 *text, StarsU16 dc)
{
    Trace *trace = (Trace *)context;
    (void)report;
    (void)field;
    (void)dc;
    ++trace->header_calls;
    text[0] = 'N';
    text[1] = 'a';
    text[2] = 'm';
    text[3] = 'e';
    text[4] = 0;
    return 20;
}

static const StarsReportItemPlanet *planet_at(void *context, StarsI16 row)
{
    static StarsReportItemPlanet planet;
    Trace *trace = (Trace *)context;
    (void)row;
    ++trace->planet_calls;
    memset(&planet, 0, sizeof planet);
    planet.id = 7;
    planet.inhabited = 1;
    planet.name = (const StarsU8 *)"New Eden";
    return &planet;
}

static StarsI16 text_extent(void *context, StarsU16 dc,
    const StarsU8 *text, StarsU16 length)
{
    (void)context;
    (void)dc;
    (void)text;
    return (StarsI16)(length * 4U);
}

static void select_object(void *context, StarsU16 dc, StarsU16 object)
{
    (void)dc;
    (void)object;
    ++((Trace *)context)->select_calls;
}

static void set_color(void *context, StarsU16 dc, StarsU32 color)
{
    (void)context;
    (void)dc;
    (void)color;
}

static void set_mode(void *context, StarsU16 dc, StarsU16 mode)
{
    (void)context;
    (void)dc;
    (void)mode;
}

static void ext_text(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsU16 options, const StarsRect *rect, const StarsU8 *text,
    StarsU16 length)
{
    Trace *trace = (Trace *)context;
    (void)dc;
    (void)x;
    (void)y;
    (void)options;
    (void)rect;
    (void)text;
    (void)length;
    ++trace->text_calls;
}

static void draw_item(void *context, StarsU16 dc, const StarsRect *rect,
    StarsI16 report, StarsI16 row, StarsI16 field)
{
    Trace *trace = (Trace *)context;
    StarsDrawReportItemOps item_ops;
    StarsDrawReportItemInput item_input;

    ++trace->draw_item_calls;
    trace->last_report = report;
    trace->last_row = row;
    trace->last_field = field;
    memset(&item_ops, 0, sizeof item_ops);
    memset(&item_input, 0, sizeof item_input);
    item_ops.planet_at = planet_at;
    item_ops.text_extent = text_extent;
    item_ops.select_object = select_object;
    item_ops.set_text_color = set_color;
    item_ops.set_background_mode = set_mode;
    item_ops.ext_text_out = ext_text;
    item_input.font = 3;
    item_input.color_black = 0;
    item_input.double_dash = (const StarsU8 *)"--";
    item_input.ops = &item_ops;
    item_input.context = context;
    stars_draw_report_item(dc, (StarsRect *)rect, report, row, field,
        &item_input);
}

static void draw_view(void *context, StarsU16 hwnd, StarsU16 dc,
    const StarsRect *rect, const StarsReportViewState *view)
{
    Trace *trace = (Trace *)context;
    StarsDrawReportOps report_ops;
    StarsDrawReportInput report_input;

    (void)hwnd;
    ++trace->view_calls;
    memset(&report_ops, 0, sizeof report_ops);
    memset(&report_input, 0, sizeof report_input);
    report_ops.header_width = report_header;
    report_ops.draw_item = draw_item;
    report_input.view = (StarsReportViewState *)view;
    report_input.arial8_height = 8;
    report_input.font = 3;
    report_input.ops = &report_ops;
    report_input.context = context;
    stars_draw_report(hwnd, dc, rect, &report_input);
}

int main(void)
{
    StarsReportDlgState state;
    StarsReportDlgOps ops;
    StarsReportDlgInput input;
    Trace trace;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    state.view.fields = 1;
    state.view.visible_fields = 1UL;
    state.view.rows_visible = 1;
    state.view.row_count = 1;
    state.view.column_width[0] = 10;
    state.view.report_kind = 0;
    state.arial8_height = 8;
    ops.begin_paint = begin_paint;
    ops.end_paint = end_paint;
    ops.draw_report_view = draw_view;
    input.state = &state;
    input.ops = &ops;
    input.context = &trace;

    CHECK(stars_report_dlg(&input, 20, 15, 0, 0) == 1);
    CHECK(trace.begin_calls == 1 && trace.view_calls == 1);
    CHECK(trace.header_calls == 1 && trace.draw_item_calls == 1);
    CHECK(trace.last_report == 0 && trace.last_row == 0 &&
        trace.last_field == 0);
    CHECK(trace.planet_calls == 1 && trace.text_calls == 1);
    CHECK(trace.select_calls >= 1);
#undef CHECK
    printf("REPORT_DLG_DRAW_INTEGRATION_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures != 0;
}
