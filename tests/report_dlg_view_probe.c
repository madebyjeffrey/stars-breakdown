#include "core.h"
#include <stdio.h>
#include <string.h>

#define main report_dlg_base_probe
#include "report_dlg_source_probe.c"
#undef main

static void view_put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static void view_put32(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    view_put16(raw, offset, (StarsU16)(value & 0xffffUL));
    view_put16(raw, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

int main(void)
{
    StarsU8 raw[54];
    StarsReportDlgState state;
    StarsReportPopupState popup;
    StarsReportDlgOps ops;
    StarsReportDlgPackedInput input;
    Trace trace;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    memset(raw, 0, sizeof raw);
    view_put32(raw, 0, 0x0000000dUL);
    view_put16(raw, 4, 2);
    view_put16(raw, 6, 4);
    view_put16(raw, 8, 1);
    view_put16(raw, 10, 2);
    view_put16(raw, 12, 1);
    view_put16(raw, 14, 3);
    view_put16(raw, 16, 10);
    view_put16(raw, 18, 20);
    view_put16(raw, 20, 320);
    view_put16(raw, 22, 220);
    view_put16(raw, 24, 1);
    raw[26] = 3; raw[27] = 5; raw[28] = 7; raw[29] = 9;
    view_put16(raw, 42, 20);
    view_put16(raw, 44, 5);
    view_put16(raw, 46, 1);
    view_put16(raw, 48, 0x21);
    view_put16(raw, 50, 0x22);
    view_put16(raw, 52, 6);

    memset(&state, 0, sizeof state);
    memset(&popup, 0, sizeof popup);
    memset(&trace, 0, sizeof trace);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    install(&ops);
    state.popup = &popup;
    state.arial8_height = 8;
    state.font_arial8 = 2;
    state.button_face_brush = 3;
    state.frame_window = 9;
    state.game_bits = 0x0800;
    input.view_record = raw;
    input.view_size = (StarsU16)sizeof raw;
    input.report.state = &state;
    input.report.ops = &ops;
    input.report.context = &trace;

    CHECK(sizeof raw == STARS_REPORT_DLG_VIEW_SIZE);
    CHECK(stars_report_dlg_packed(&input, 20, 1, 0, 0) == 1);
    CHECK(state.view.visible_fields == 0x0000000dUL);
    CHECK(state.view.report_kind == 2 && state.view.fields == 4);
    CHECK(state.view.field_first == 1 && state.sort_column == 2);
    CHECK(state.ascending == 1 && state.view.row_first == 3);
    CHECK(state.dialog_position.x == 10 && state.dialog_position.y == 20);
    CHECK(state.dialog_size.x == 320 && state.dialog_size.y == 220);
    CHECK(state.browser_valid == 1 && state.view.row_count == 20);
    CHECK(state.view.column_scroll == 6 && popup.report == 2);
    CHECK(popup.field_count == 4 && popup.sort_column == 2 &&
        popup.ascending == 1 && popup.visible_mask == 0x0000000dUL);
    CHECK(trace.scroll_class_ok == 2 && trace.last_scroll_parent == 20);
    CHECK(checks == 13);
    printf("REPORT_DLG_VIEW_CHECKS=%d FAILURES=%d RESULT=%s\n", checks,
        failures, failures == 0 ? "PASS" : "FAIL");
    return failures ? 1 : 0;
#undef CHECK
}
