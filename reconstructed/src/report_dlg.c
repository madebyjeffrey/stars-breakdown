#include "core.h"

enum {
    REPORT_DLG_WM_CREATE = 1,
    REPORT_DLG_WM_DESTROY = 2,
    REPORT_DLG_WM_SIZE = 5,
    REPORT_DLG_WM_PAINT = 15,
    REPORT_DLG_WM_ERASE_BACKGROUND = 20,
    REPORT_DLG_WM_GET_MINMAXINFO = 36,
    REPORT_DLG_WM_COMMAND = 273,
    REPORT_DLG_WM_HORIZONTAL_SCROLL = 276,
    REPORT_DLG_WM_VERTICAL_SCROLL = 277,
    REPORT_DLG_WM_LEFT_BUTTON = 513,
    REPORT_DLG_WM_MIDDLE_BUTTON = 515,
    REPORT_DLG_WM_RIGHT_BUTTON = 516,
    REPORT_DLG_ID_CANCEL = 2,
    REPORT_DLG_TUTOR_BIT = 0x0800,
    REPORT_DLG_INVALIDATED_BIT = 0x0200,
    REPORT_DLG_SCROLL_BAR = 2
};

#define REPORT_DLG_VERTICAL_STYLE 0x40000001UL
#define REPORT_DLG_HORIZONTAL_STYLE 0x40000000UL

static StarsU16 report_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static StarsU32 report_packed_get32(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU32)report_packed_get16(record, offset) |
        ((StarsU32)report_packed_get16(record,
            (StarsU16)(offset + 2)) << 16);
}

static void report_packed_load(const StarsU8 *record,
    StarsReportDlgState *state)
{
    StarsI16 field;

    state->view.visible_fields = report_packed_get32(record, 0);
    state->view.report_kind = (StarsI16)report_packed_get16(record, 4);
    state->view.fields = report_packed_get16(record, 6);
    state->view.field_first = report_packed_get16(record, 8);
    state->sort_column = (StarsI16)report_packed_get16(record, 10);
    state->ascending = (StarsI16)report_packed_get16(record, 12);
    state->view.row_first = (StarsI16)report_packed_get16(record, 14);
    state->dialog_position.x = (StarsI16)report_packed_get16(record, 16);
    state->dialog_position.y = (StarsI16)report_packed_get16(record, 18);
    state->dialog_size.x = (StarsI16)report_packed_get16(record, 20);
    state->dialog_size.y = (StarsI16)report_packed_get16(record, 22);
    state->browser_valid = report_packed_get16(record, 24);
    for (field = 0; field < 16; ++field)
        state->view.column_width[field] = record[26 + field];
    state->view.row_count = (StarsI16)report_packed_get16(record, 42);
    state->view.rows_visible = report_packed_get16(record, 44);
    state->subsort = (StarsI16)report_packed_get16(record, 46);
    state->vscroll_window = report_packed_get16(record, 48);
    state->view.hscroll_window = report_packed_get16(record, 50);
    state->view.column_scroll = report_packed_get16(record, 52);
}

static StarsI16 report_field_visible(const StarsReportDlgState *state,
    StarsI16 field)
{
    StarsU32 bit;

    if (field < 0 || (StarsU16)field >= state->view.fields || field >= 32)
        return 0;
    bit = (StarsU32)1UL << field;
    if ((state->view.visible_fields & bit) == 0UL)
        return 0;
    if (field != 0 && (StarsU16)field < state->view.field_first)
        return 0;
    return 1;
}

static void report_sync_popup(StarsReportDlgState *state)
{
    if (!state->popup)
        return;
    state->popup->report = state->view.report_kind;
    state->popup->field_count = (StarsI16)state->view.fields;
    state->popup->sort_column = state->sort_column;
    state->popup->subsort = state->subsort;
    state->popup->ascending = state->ascending;
    state->popup->visible_mask = state->view.visible_fields;
}

static void report_layout(StarsReportDlgInput *input, StarsU16 hwnd)
{
    StarsReportDlgState *state;
    StarsRect client;
    StarsI16 row_height;
    StarsI16 rows;
    StarsI16 metric;
    StarsI16 flags;
    StarsI16 scroll_height;

    state = input->state;
    client.left = client.top = client.right = client.bottom = 0;
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->context, hwnd, &client);
    row_height = (StarsI16)(state->arial8_height + 4);
    if (row_height <= 0)
        row_height = 1;
    rows = (StarsI16)((client.bottom - 0x24) / row_height);
    if (rows < 0)
        rows = 0;
    if (state->view.row_count <= rows)
        rows = state->view.row_count;
    state->view.rows_visible = (StarsU16)rows;
    if (rows < state->view.row_count) {
        flags = 0x44;
        if (state->view.row_count < (StarsI16)state->view.row_first + rows &&
                state->view.row_first > 0) {
            state->view.row_first = (StarsI16)(state->view.row_count - rows);
            if (state->view.row_first < 0)
                state->view.row_first = 0;
        }
        if (input->ops->set_scroll_pos)
            input->ops->set_scroll_pos(input->context,
                state->vscroll_window, REPORT_DLG_SCROLL_BAR,
                state->view.row_first, 0);
        if (input->ops->set_scroll_range)
            input->ops->set_scroll_range(input->context,
                state->vscroll_window, REPORT_DLG_SCROLL_BAR, 0,
                (StarsI16)(state->view.row_count - rows), 1);
    } else {
        flags = 0x84;
        state->view.row_first = 0;
        if (input->ops->set_scroll_pos)
            input->ops->set_scroll_pos(input->context,
                state->vscroll_window, REPORT_DLG_SCROLL_BAR, 0, 0);
    }
    metric = input->ops->get_system_metrics ?
        (StarsI16)input->ops->get_system_metrics(input->context, 2) : 0;
    scroll_height = (StarsI16)(row_height * rows + 1);
    if (input->ops->set_window_pos)
        input->ops->set_window_pos(input->context,
            state->vscroll_window, (StarsI16)(client.right - metric),
            (StarsI16)(state->arial8_height + 6), metric, scroll_height,
            flags);
    if (input->ops->set_h_scroll_bar)
        input->ops->set_h_scroll_bar(input->context);
}

static void report_initialize(StarsReportDlgInput *input, StarsU16 hwnd)
{
    StarsReportDlgState *state;
    StarsU16 dc;
    StarsI16 field;
    StarsU8 text[42];

    state = input->state;
    if (input->ops->load_view && input->packed_view &&
            !input->ops->load_view(input->context, input->packed_view,
            state))
        return;
    state->view.report_window = hwnd;
    state->current_valid = 1;
    dc = input->ops->get_dc ? input->ops->get_dc(input->context, hwnd) : 0;
    if (input->ops->select_object)
        input->ops->select_object(input->context, dc, state->font_arial8);
    for (field = 0; field < (StarsI16)state->view.fields; ++field) {
        text[0] = 0;
        state->view.column_width[field] = (StarsU8)(input->ops->header_width ?
            input->ops->header_width(input->context,
                state->view.report_kind, field, dc, text,
                (StarsU16)sizeof(text)) / 2 : 0);
    }
    if (input->ops->release_dc)
        input->ops->release_dc(input->context, hwnd, dc);
    report_sync_popup(state);
    if (input->ops->sort_cache)
        input->ops->sort_cache(input->context, state->view.report_kind,
            state->sort_column);
    if (input->ops->set_window_pos)
        input->ops->set_window_pos(input->context, hwnd, 0, 0,
            state->dialog_size.x, state->dialog_size.y, 0x000e);
    if (input->ops->sticky_position)
        input->ops->sticky_position(input->context, hwnd,
            &state->dialog_position, 1);
    if (input->ops->create_scroll) {
        state->vscroll_window = input->ops->create_scroll(input->context,
            (const StarsU8 *)"SCROLLBAR", REPORT_DLG_VERTICAL_STYLE, hwnd);
        state->view.hscroll_window = input->ops->create_scroll(input->context,
            (const StarsU8 *)"SCROLLBAR", REPORT_DLG_HORIZONTAL_STYLE, hwnd);
    }
    if ((state->game_bits & REPORT_DLG_TUTOR_BIT) != 0U &&
            input->ops->advance_tutor)
        input->ops->advance_tutor(input->context);
    report_layout(input, hwnd);
}

static void report_destroy(StarsReportDlgInput *input, StarsU16 hwnd)
{
    StarsReportDlgState *state;
    StarsRect rect;
    StarsU16 submenu;
    StarsU16 menu_id;

    state = input->state;
    if (input->ops->sticky_position)
        input->ops->sticky_position(input->context, hwnd,
            &state->dialog_position, 0);
    rect.left = rect.top = rect.right = rect.bottom = 0;
    if (input->ops->get_window_rect)
        input->ops->get_window_rect(input->context, hwnd, &rect);
    state->dialog_size.x = (StarsI16)(rect.right - rect.left);
    state->dialog_size.y = (StarsI16)(rect.bottom - rect.top);
    state->view.report_window = 0;
    state->browser_valid = 0;
    submenu = input->ops->get_submenu ? input->ops->get_submenu(
        input->context, state->frame_window, 4) : 0;
    if (state->view.report_kind == 0)
        menu_id = 0x08fd;
    else if (state->view.report_kind == 1)
        menu_id = 0x08ff;
    else if (state->view.report_kind == 2)
        menu_id = 0x0900;
    else
        menu_id = 0x0901;
    if (input->ops->check_menu_item)
        input->ops->check_menu_item(input->context, submenu, menu_id, 0);
    state->current_valid = 0;
    if (input->ops->clear_current)
        input->ops->clear_current(input->context);
    if ((state->game_bits & REPORT_DLG_TUTOR_BIT) != 0U &&
            input->ops->advance_tutor)
        input->ops->advance_tutor(input->context);
}

static StarsI32 report_hscroll(StarsReportDlgInput *input, StarsU16 hwnd,
    StarsU16 command, StarsU32 lparam)
{
    StarsReportDlgState *state;
    StarsU16 scroll_window;
    StarsI16 old_position;
    StarsI16 position;
    StarsI16 field;
    StarsI16 remaining;
    StarsU32 bit;

    state = input->state;
    scroll_window = (StarsU16)(lparam >> 16);
    if (scroll_window == 0)
        scroll_window = state->view.hscroll_window;
    old_position = input->ops->get_scroll_pos ?
        input->ops->get_scroll_pos(input->context, scroll_window,
            REPORT_DLG_SCROLL_BAR) : 0;
    position = old_position;
    switch (command) {
    case 0: position = (StarsI16)(old_position - 1); break;
    case 1: position = (StarsI16)(old_position + 1); break;
    case 2: position = (StarsI16)(old_position - 3); break;
    case 3: position = (StarsI16)(old_position + 3); break;
    case 4:
    case 5: position = (StarsI16)(StarsU16)lparam; break;
    case 6: position = 0; break;
    case 7: position = 2000; break;
    default: break;
    }
    if (position > (StarsI16)state->view.column_scroll)
        position = (StarsI16)state->view.column_scroll;
    if (position < 0)
        position = 0;
    if (position != old_position) {
        if (input->ops->set_scroll_pos)
            input->ops->set_scroll_pos(input->context, scroll_window,
                REPORT_DLG_SCROLL_BAR, position, 1);
        position = input->ops->get_scroll_pos ?
            input->ops->get_scroll_pos(input->context, scroll_window,
                REPORT_DLG_SCROLL_BAR) : position;
        if (position != old_position) {
            remaining = position;
            field = 1;
            bit = 2UL;
            while (field < (StarsI16)state->view.fields) {
                if ((state->view.visible_fields & bit) != 0UL) {
                    if (remaining <= 0)
                        break;
                    --remaining;
                }
                ++field;
                bit <<= 1;
            }
            state->view.field_first = (StarsU16)field;
            report_sync_popup(state);
            if (input->ops->invalidate)
                input->ops->invalidate(input->context, hwnd);
            if (input->ops->update_window)
                input->ops->update_window(input->context, hwnd);
        }
    }
    return 0;
}

static StarsI32 report_vscroll(StarsReportDlgInput *input, StarsU16 hwnd,
    StarsU16 command, StarsU32 lparam)
{
    StarsReportDlgState *state;
    StarsRect client;
    StarsRect scroll_rect;
    StarsU16 scroll_window;
    StarsI16 old_position;
    StarsI16 position;
    StarsI16 metric;
    StarsI16 row_height;

    state = input->state;
    scroll_window = (StarsU16)(lparam >> 16);
    if (scroll_window == 0)
        scroll_window = state->vscroll_window;
    old_position = input->ops->get_scroll_pos ?
        input->ops->get_scroll_pos(input->context, scroll_window,
            REPORT_DLG_SCROLL_BAR) : state->view.row_first;
    position = old_position;
    switch (command) {
    case 0: --position; break;
    case 1: ++position; break;
    case 2: position = (StarsI16)(old_position -
        ((StarsI16)state->view.rows_visible - 1)); break;
    case 3: position = (StarsI16)(old_position +
        ((StarsI16)state->view.rows_visible - 1)); break;
    case 4:
    case 5: position = (StarsI16)(StarsU16)lparam; break;
    case 6: position = 0; break;
    case 7: position = 2000; break;
    default: break;
    }
    if (position > (StarsI16)state->view.row_count -
            (StarsI16)state->view.rows_visible)
        position = (StarsI16)state->view.row_count -
            (StarsI16)state->view.rows_visible;
    if (position < 0)
        position = 0;
    if (position != old_position) {
        state->view.row_first = position;
        client.left = client.top = client.right = client.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &client);
        metric = input->ops->get_system_metrics ?
            (StarsI16)input->ops->get_system_metrics(input->context, 2) : 0;
        row_height = (StarsI16)(state->arial8_height + 4);
        scroll_rect.left = 2;
        scroll_rect.top = (StarsU16)(state->arial8_height + 6);
        scroll_rect.right = (StarsU16)(client.right - metric);
        scroll_rect.bottom = (StarsU16)(row_height *
            (StarsI16)state->view.rows_visible + scroll_rect.top);
        if (input->ops->scroll_window)
            input->ops->scroll_window(input->context, hwnd, 0,
                (StarsI16)(row_height * (old_position - position)),
                &scroll_rect);
        if (input->ops->set_scroll_pos)
            input->ops->set_scroll_pos(input->context, scroll_window,
                REPORT_DLG_SCROLL_BAR, position, 1);
        if (input->ops->update_window)
            input->ops->update_window(input->context, hwnd);
    }
    return 0;
}

static void report_mouse(StarsReportDlgInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU32 lparam)
{
    StarsReportDlgState *state;
    StarsPoint point;
    StarsI16 x;
    StarsI16 y;
    StarsI16 row;
    StarsI16 field;
    StarsI16 column;
    StarsI16 right_edge;
    StarsI16 row_height;

    state = input->state;
    x = (StarsI16)(StarsU16)lparam;
    y = (StarsI16)(StarsU16)(lparam >> 16);
    if (x <= 1 || y <= 1)
        return;
    row = -1;
    row_height = (StarsI16)(state->arial8_height + 4);
    if (y >= state->arial8_height + 6) {
        if (row_height <= 0)
            return;
        row = (StarsI16)((y - 2 - row_height) / row_height);
        if (row >= (StarsI16)state->view.rows_visible)
            return;
        row = (StarsI16)(row + state->view.row_first);
    }
    column = -1;
    right_edge = 2;
    for (field = 0; field < (StarsI16)state->view.fields; ++field) {
        if (report_field_visible(state, field)) {
            right_edge = (StarsI16)(right_edge +
                (StarsI16)state->view.column_width[field] * 2);
            if (x < right_edge) {
                column = field;
                break;
            }
        }
    }
    if (column < 0)
        return;
    point.x = x;
    point.y = y;
    if (row < 0) {
        if (input->ops->column_popup)
            input->ops->column_popup(input->context, point, column,
                (StarsI16)(message == REPORT_DLG_WM_RIGHT_BUTTON));
    } else if (input->ops->execute_click) {
        input->ops->execute_click(input->context, point,
            state->view.report_kind, column, row);
    }
    if ((state->game_bits & REPORT_DLG_TUTOR_BIT) != 0U &&
            input->ops->advance_tutor)
        input->ops->advance_tutor(input->context);
    (void)hwnd;
}

StarsI32 stars_report_dlg(StarsReportDlgInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect paint;
    StarsU16 dc;

    if (!input || !input->state || !input->ops)
        return 0;
    if (message == REPORT_DLG_WM_CREATE) {
        report_initialize(input, hwnd);
        return 1;
    }
    if (message == REPORT_DLG_WM_SIZE) {
        report_layout(input, hwnd);
        return 0;
    }
    if (message == REPORT_DLG_WM_PAINT) {
        paint.left = paint.top = paint.right = paint.bottom = 0;
        dc = input->ops->begin_paint ? input->ops->begin_paint(
            input->context, hwnd, &paint) : 0;
        if (input->ops->draw_report_view)
            input->ops->draw_report_view(input->context, hwnd, dc, &paint,
                &input->state->view);
        else if (input->ops->draw_report)
            input->ops->draw_report(input->context, hwnd, dc, &paint);
        if (input->ops->end_paint)
            input->ops->end_paint(input->context, hwnd, &paint);
        input->state->game_bits = (StarsU16)(input->state->game_bits &
            (StarsU16)~REPORT_DLG_INVALIDATED_BIT);
        return 1;
    }
    if (message == REPORT_DLG_WM_ERASE_BACKGROUND) {
        paint.left = paint.top = paint.right = paint.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &paint);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->context, wparam, &paint,
                input->state->button_face_brush);
        return 1;
    }
    if (message == REPORT_DLG_WM_GET_MINMAXINFO) {
        if (input->ops->set_min_track)
            input->ops->set_min_track(input->context, lparam, 300, 220);
        return 0;
    }
    if (message == REPORT_DLG_WM_DESTROY)
        report_destroy(input, hwnd);
    else if (message == REPORT_DLG_WM_COMMAND && wparam ==
            REPORT_DLG_ID_CANCEL) {
        if (input->ops->destroy_window)
            input->ops->destroy_window(input->context, hwnd);
        return 1;
    } else if (message == REPORT_DLG_WM_HORIZONTAL_SCROLL)
        return report_hscroll(input, hwnd, wparam, lparam);
    else if (message == REPORT_DLG_WM_VERTICAL_SCROLL)
        return report_vscroll(input, hwnd, wparam, lparam);
    else if (message == REPORT_DLG_WM_LEFT_BUTTON ||
            message == REPORT_DLG_WM_MIDDLE_BUTTON ||
            message == REPORT_DLG_WM_RIGHT_BUTTON)
        report_mouse(input, hwnd, message, lparam);
    else {
        if (input->ops->default_proc)
            return input->ops->default_proc(input->context, hwnd, message,
                wparam, lparam);
        return 0;
    }
    if (input->ops->default_proc)
        return input->ops->default_proc(input->context, hwnd, message,
            wparam, lparam);
    return 0;
}

StarsI32 stars_report_dlg_packed(StarsReportDlgPackedInput *input,
    StarsU16 hwnd, StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsReportDlgInput report;

    if (!input || !input->view_record || input->view_size <
            STARS_REPORT_DLG_VIEW_SIZE || !input->report.state)
        return 0;
    report_packed_load(input->view_record, input->report.state);
    report = input->report;
    report.packed_view = 0;
    return stars_report_dlg(&report, hwnd, message, wparam, lparam);
}
