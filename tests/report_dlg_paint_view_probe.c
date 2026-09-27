#include "core.h"
#include <stdio.h>
#include <string.h>

#define main report_dlg_base_probe
#include "report_dlg_source_probe.c"
#undef main

typedef struct PaintTrace {
    StarsI16 calls;
    StarsU16 hwnd;
    StarsU16 dc;
    StarsI16 report_kind;
    StarsI16 row_first;
    StarsI16 row_count;
    StarsU16 fields;
    StarsU32 visible_fields;
    StarsU8 first_width;
    StarsU16 paint_bottom;
} PaintTrace;

static void draw_view(void *context, StarsU16 hwnd, StarsU16 dc,
    const StarsRect *rect, const StarsReportViewState *view)
{
    PaintTrace *trace = (PaintTrace *)context;
    ++trace->calls;
    trace->hwnd = hwnd;
    trace->dc = dc;
    trace->report_kind = view->report_kind;
    trace->row_first = view->row_first;
    trace->row_count = view->row_count;
    trace->fields = view->fields;
    trace->visible_fields = view->visible_fields;
    trace->first_width = view->column_width[0];
    trace->paint_bottom = rect->bottom;
}

int main(void)
{
    StarsReportDlgState state;
    StarsReportPopupState popup;
    StarsReportDlgOps ops;
    StarsReportDlgInput input;
    PaintTrace trace;
    StarsI16 failures;
    StarsI16 checks;

    memset(&state, 0, sizeof state);
    memset(&popup, 0, sizeof popup);
    memset(&trace, 0, sizeof trace);
    install(&ops);
    ops.draw_report_view = draw_view;
    state.popup = &popup;
    state.view.report_kind = 2;
    state.view.row_first = 4;
    state.view.row_count = 17;
    state.view.fields = 6;
    state.view.visible_fields = 0x00000035UL;
    state.view.column_width[0] = 13;
    state.game_bits = 0x0200;
    input.state = &state;
    input.ops = &ops;
    input.context = &trace;

    failures = 0;
    checks = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(stars_report_dlg(&input, 20, 15, 0, 0) == 1);
    CHECK(trace.calls == 1 && trace.hwnd == 20 && trace.dc == 4);
    CHECK(trace.report_kind == 2 && trace.row_first == 4);
    CHECK(trace.row_count == 17 && trace.fields == 6);
    CHECK(trace.visible_fields == 0x00000035UL && trace.first_width == 13);
    CHECK(trace.paint_bottom == 0);
#undef CHECK
    if (failures == 0)
        printf("REPORT_DLG_PAINT_VIEW_CHECKS=%d FAILURES=0 RESULT=PASS\n",
            checks);
    else
        printf("REPORT_DLG_PAINT_VIEW_CHECKS=%d FAILURES=%d RESULT=FAIL\n",
            checks, failures);
    return failures == 0 ? 0 : 1;
}
