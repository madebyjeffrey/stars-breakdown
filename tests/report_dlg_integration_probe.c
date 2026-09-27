#include "core.h"
#include <stdio.h>
#include <string.h>

#define main report_dlg_base_probe
#include "report_dlg_source_probe.c"
#undef main

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static void put32(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    put16(raw, offset, (StarsU16)(value & 0xffffUL));
    put16(raw, (StarsU16)(offset + 2), (StarsU16)(value >> 16));
}

static StarsU16 get16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1] << 8));
}

static StarsU32 get32(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU32)get16(raw, offset) |
        ((StarsU32)get16(raw, (StarsU16)(offset + 2)) << 16);
}

static void load_rpt(const StarsU8 *raw, StarsReportDlgState *state)
{
    StarsI16 field;

    state->view.visible_fields = get32(raw, 0);
    state->view.report_kind = (StarsI16)get16(raw, 4);
    state->view.fields = get16(raw, 6);
    state->view.field_first = get16(raw, 8);
    state->sort_column = (StarsI16)get16(raw, 10);
    state->ascending = (StarsI16)get16(raw, 12);
    state->view.row_first = (StarsI16)get16(raw, 14);
    state->dialog_position.x = (StarsI16)get16(raw, 16);
    state->dialog_position.y = (StarsI16)get16(raw, 18);
    state->dialog_size.x = (StarsI16)get16(raw, 20);
    state->dialog_size.y = (StarsI16)get16(raw, 22);
    state->browser_valid = get16(raw, 24);
    for (field = 0; field < 16; ++field)
        state->view.column_width[field] = raw[26 + field];
    state->view.row_count = (StarsI16)get16(raw, 42);
    state->view.rows_visible = get16(raw, 44);
    state->subsort = (StarsI16)get16(raw, 46);
    state->vscroll_window = get16(raw, 48);
    state->view.hscroll_window = get16(raw, 50);
    state->view.column_scroll = get16(raw, 52);
}

static void store_rpt(StarsU8 *raw, const StarsReportDlgState *state)
{
    StarsI16 field;

    put32(raw, 0, state->view.visible_fields);
    put16(raw, 4, (StarsU16)state->view.report_kind);
    put16(raw, 6, state->view.fields);
    put16(raw, 8, state->view.field_first);
    put16(raw, 10, (StarsU16)state->sort_column);
    put16(raw, 12, (StarsU16)state->ascending);
    put16(raw, 14, (StarsU16)state->view.row_first);
    put16(raw, 16, (StarsU16)state->dialog_position.x);
    put16(raw, 18, (StarsU16)state->dialog_position.y);
    put16(raw, 20, (StarsU16)state->dialog_size.x);
    put16(raw, 22, (StarsU16)state->dialog_size.y);
    put16(raw, 24, state->browser_valid);
    for (field = 0; field < 16; ++field)
        raw[26 + field] = state->view.column_width[field];
    put16(raw, 42, (StarsU16)state->view.row_count);
    put16(raw, 44, state->view.rows_visible);
    put16(raw, 46, (StarsU16)state->subsort);
    put16(raw, 48, state->vscroll_window);
    put16(raw, 50, state->view.hscroll_window);
    put16(raw, 52, state->view.column_scroll);
}

int main(void)
{
    StarsU8 raw[54];
    StarsReportDlgState state;
    StarsReportPopupState popup;
    StarsReportDlgOps ops;
    StarsReportDlgInput input;
    Trace trace;
    StarsI16 failures;
    StarsI16 checks;

    memset(raw, 0, sizeof raw);
    put32(raw, 0, 0x0000000dUL);
    put16(raw, 4, 2);
    put16(raw, 6, 4);
    put16(raw, 8, 1);
    put16(raw, 10, 2);
    put16(raw, 12, 1);
    put16(raw, 14, 3);
    put16(raw, 16, 10);
    put16(raw, 18, 20);
    put16(raw, 20, 320);
    put16(raw, 22, 220);
    put16(raw, 24, 1);
    raw[26] = 3; raw[27] = 5; raw[28] = 7; raw[29] = 9;
    put16(raw, 42, 20);
    put16(raw, 44, 5);
    put16(raw, 46, 1);
    put16(raw, 48, 0x21);
    put16(raw, 50, 0x22);
    put16(raw, 52, 6);

    memset(&state, 0, sizeof state);
    memset(&popup, 0, sizeof popup);
    memset(&trace, 0, sizeof trace);
    load_rpt(raw, &state);
    install(&ops);
    state.popup = &popup;
    state.dialog_size.x = 320;
    state.dialog_size.y = 220;
    state.arial8_height = 8;
    state.font_arial8 = 2;
    state.button_face_brush = 3;
    state.frame_window = 9;
    state.game_bits = 0x0800;
    input.state = &state;
    input.ops = &ops;
    input.context = &trace;
    failures = 0;
    checks = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof raw == 54);
    CHECK(state.view.visible_fields == 0x0000000dUL);
    CHECK(state.view.report_kind == 2 && state.view.fields == 4);
    CHECK(state.view.field_first == 1 && state.sort_column == 2);
    CHECK(state.ascending == 1 && state.view.row_first == 3);
    CHECK(state.dialog_position.x == 10 && state.dialog_position.y == 20);
    CHECK(state.dialog_size.x == 320 && state.dialog_size.y == 220);
    CHECK(state.view.column_width[0] == 3 && state.view.column_width[3] == 9);
    CHECK(state.view.row_count == 20 && state.view.rows_visible == 5);
    CHECK(state.subsort == 1 && state.vscroll_window == 0x21);
    CHECK(state.view.hscroll_window == 0x22 && state.view.column_scroll == 6);
    CHECK(stars_report_dlg(&input, 20, 1, 0, 0) == 1);
    store_rpt(raw, &state);
    CHECK(trace.scroll_class_ok == 2 && trace.last_scroll_parent == 20 &&
        trace.last_scroll_style == 0x40000000UL);
    CHECK(popup.report == 2 && popup.field_count == 4 &&
        popup.sort_column == 2 && popup.ascending == 1 &&
        popup.visible_mask == 0x0000000dUL);
    CHECK(get16(raw, 44) == 15);
    CHECK(get16(raw, 48) == 10 && get16(raw, 50) == 11);
    CHECK(raw[26] == 10 && raw[27] == 11 && raw[28] == 12 && raw[29] == 13);
    state.view.column_scroll = 3;
    CHECK(stars_report_dlg(&input, 20, 276, 1, 11UL << 16) == 0);
    store_rpt(raw, &state);
    CHECK(get16(raw, 8) == 3 && get16(raw, 52) == 3);
    state.view.rows_visible = 5;
    CHECK(stars_report_dlg(&input, 20, 36, 0, 0) == 0 &&
        trace.min_track_width == 300 && trace.min_track_height == 220);
    CHECK(stars_report_dlg(&input, 20, 277, 3, 10UL << 16) == 0);
    store_rpt(raw, &state);
    CHECK(get16(raw, 14) == 7 && get16(raw, 42) == 20);
    CHECK(trace.creates == 2 && trace.invalidates == 1 && trace.updates == 2);
    CHECK(stars_report_dlg(&input, 20, 2, 0, 0) == 77 &&
        trace.last_submenu_parent == 9 && trace.last_submenu_position == 4 &&
        trace.last_menu_id == 0x0900U && trace.last_menu_flags == 0U);
#undef CHECK
    if (failures == 0)
        printf("REPORT_DLG_INTEGRATION_CHECKS=%d FAILURES=0 RESULT=PASS\n",
            checks);
    else
        printf("REPORT_DLG_INTEGRATION_CHECKS=%d FAILURES=%d RESULT=FAIL\n",
            checks, failures);
    return failures == 0 ? 0 : 1;
}
