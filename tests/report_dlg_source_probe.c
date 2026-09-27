#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 gets;
    StarsI16 selects;
    StarsI16 releases;
    StarsI16 positions;
    StarsI16 stickies;
    StarsI16 creates;
    StarsI16 tutors;
    StarsI16 set_positions;
    StarsI16 ranges;
    StarsI16 scrolls;
    StarsI16 begins;
    StarsI16 draws;
    StarsI16 ends;
    StarsI16 fills;
    StarsI16 destroys;
    StarsI16 menus;
    StarsI16 checks;
    StarsI16 min_tracks;
    StarsI16 invalidates;
    StarsI16 updates;
    StarsI16 sorts;
    StarsI16 horizontal_layouts;
    StarsI16 popups;
    StarsI16 clicks;
    StarsI16 clears;
    StarsI16 defaults;
    StarsI16 hpos;
    StarsI16 vpos;
    StarsI16 last_popup_column;
    StarsI16 last_click_row;
    StarsI16 last_click_column;
    StarsI16 last_position_height;
    StarsI16 scroll_class_ok;
    StarsU32 last_scroll_style;
    StarsU16 last_scroll_parent;
    StarsU16 last_submenu_parent;
    StarsI16 last_submenu_position;
    StarsU16 last_menu_id;
    StarsU16 last_menu_flags;
    StarsI16 min_track_width;
    StarsI16 min_track_height;
} Trace;

static void get_window_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    ++trace->positions;
    rect->left = 10; rect->top = 20; rect->right = 310; rect->bottom = 220;
}
static void get_client_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd;
    ++((Trace *)context)->positions;
    rect->left = 0; rect->top = 0; rect->right = 300; rect->bottom = 220;
}
static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)context)->gets; return 3;
}
static void select_object(void *context, StarsU16 dc, StarsU16 object)
{
    (void)dc; (void)object; ++((Trace *)context)->selects;
}
static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)hwnd; (void)dc; ++((Trace *)context)->releases;
}
static void set_window_pos(void *context, StarsU16 hwnd, StarsI16 x,
    StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    (void)hwnd; (void)x; (void)y; (void)width; (void)height; (void)flags;
    ++((Trace *)context)->set_positions;
    ((Trace *)context)->last_position_height = height;
}
static void sticky(void *context, StarsU16 hwnd, StarsPoint *point,
    StarsI16 save)
{
    (void)hwnd; (void)point; (void)save; ++((Trace *)context)->stickies;
}
static StarsU16 create_scroll(void *context, const StarsU8 *class_name,
    StarsU32 style, StarsU16 parent)
{
    Trace *trace = (Trace *)context;
    if (strcmp((const char *)class_name, "SCROLLBAR") == 0)
        ++trace->scroll_class_ok;
    trace->last_scroll_style = style;
    trace->last_scroll_parent = parent;
    ++trace->creates;
    return trace->creates == 1 ? 10 : 11;
}
static void advance_tutor(void *context)
{
    ++((Trace *)context)->tutors;
}
static StarsU16 get_metric(void *context, StarsU16 metric)
{
    (void)context; return metric == 2 ? 16 : 0;
}
static void set_scroll_pos(void *context, StarsU16 hwnd, StarsU16 bar,
    StarsI16 position, StarsI16 redraw)
{
    Trace *trace = (Trace *)context;
    (void)bar; (void)redraw;
    ++trace->set_positions;
    if (hwnd == 10) trace->vpos = position;
    if (hwnd == 11) trace->hpos = position;
}
static void set_scroll_range(void *context, StarsU16 hwnd, StarsU16 bar,
    StarsI16 minimum, StarsI16 maximum, StarsI16 page)
{
    (void)hwnd; (void)bar; (void)minimum; (void)maximum; (void)page;
    ++((Trace *)context)->ranges;
}
static StarsI16 get_scroll_pos(void *context, StarsU16 hwnd, StarsU16 bar)
{
    Trace *trace = (Trace *)context;
    (void)bar; return hwnd == 10 ? trace->vpos : trace->hpos;
}
static void scroll_window(void *context, StarsU16 hwnd, StarsI16 dx,
    StarsI16 dy, const StarsRect *rect)
{
    (void)hwnd; (void)dx; (void)dy; (void)rect;
    ++((Trace *)context)->scrolls;
}
static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; (void)rect; ++((Trace *)context)->begins; return 4;
}
static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; (void)rect; ++((Trace *)context)->ends;
}
static void fill_rect(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    (void)dc; (void)rect; (void)brush; ++((Trace *)context)->fills;
}
static void destroy_window(void *context, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)context)->destroys;
}
static StarsU16 get_submenu(void *context, StarsU16 hwnd, StarsI16 position)
{
    Trace *trace = (Trace *)context;
    trace->last_submenu_parent = hwnd;
    trace->last_submenu_position = position;
    ++trace->menus; return 8;
}
static void check_menu_item(void *context, StarsU16 menu, StarsU16 id,
    StarsU16 flags)
{
    Trace *trace = (Trace *)context;
    trace->last_menu_id = id;
    trace->last_menu_flags = flags;
    (void)menu; ++trace->checks;
}
static void set_min_track(void *context, StarsU32 lparam, StarsI16 width,
    StarsI16 height)
{
    Trace *trace = (Trace *)context;
    (void)lparam; trace->min_track_width = width;
    trace->min_track_height = height; ++trace->min_tracks;
}
static void invalidate(void *context, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)context)->invalidates;
}
static void update_window(void *context, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)context)->updates;
}
static StarsI32 default_proc(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    (void)hwnd; (void)message; (void)wparam; (void)lparam;
    ++((Trace *)context)->defaults; return 77;
}
static StarsI16 header_width(void *context, StarsI16 report, StarsI16 field,
    StarsU16 dc, StarsU8 *text, StarsU16 capacity)
{
    (void)report; (void)dc; (void)capacity;
    ++((Trace *)context)->sorts;
    text[0] = (StarsU8)('A' + field); text[1] = 0;
    return (StarsI16)(20 + field * 2);
}
static void sort_cache(void *context, StarsI16 report, StarsI16 column)
{
    (void)report; (void)column; ++((Trace *)context)->sorts;
}
static void set_h_scroll(void *context)
{
    ++((Trace *)context)->horizontal_layouts;
}
static void draw_report(void *context, StarsU16 hwnd, StarsU16 dc,
    const StarsRect *rect)
{
    (void)hwnd; (void)dc; (void)rect; ++((Trace *)context)->draws;
}
static void column_popup(void *context, StarsPoint point, StarsI16 column,
    StarsI16 right_button)
{
    Trace *trace = (Trace *)context;
    (void)point; (void)right_button; ++trace->popups;
    trace->last_popup_column = column;
}
static void execute_click(void *context, StarsPoint point, StarsI16 report,
    StarsI16 column, StarsI16 row)
{
    Trace *trace = (Trace *)context;
    (void)point; (void)report; ++trace->clicks;
    trace->last_click_column = column; trace->last_click_row = row;
}
static void clear_current(void *context)
{
    ++((Trace *)context)->clears;
}

static void install(StarsReportDlgOps *ops)
{
    memset(ops, 0, sizeof *ops);
    ops->get_window_rect = get_window_rect;
    ops->get_client_rect = get_client_rect;
    ops->get_dc = get_dc;
    ops->select_object = select_object;
    ops->release_dc = release_dc;
    ops->set_window_pos = set_window_pos;
    ops->sticky_position = sticky;
    ops->create_scroll = create_scroll;
    ops->advance_tutor = advance_tutor;
    ops->get_system_metrics = get_metric;
    ops->set_scroll_pos = set_scroll_pos;
    ops->set_scroll_range = set_scroll_range;
    ops->get_scroll_pos = get_scroll_pos;
    ops->scroll_window = scroll_window;
    ops->begin_paint = begin_paint;
    ops->end_paint = end_paint;
    ops->fill_rect = fill_rect;
    ops->destroy_window = destroy_window;
    ops->get_submenu = get_submenu;
    ops->check_menu_item = check_menu_item;
    ops->set_min_track = set_min_track;
    ops->invalidate = invalidate;
    ops->update_window = update_window;
    ops->default_proc = default_proc;
    ops->header_width = header_width;
    ops->sort_cache = sort_cache;
    ops->set_h_scroll_bar = set_h_scroll;
    ops->draw_report = draw_report;
    ops->column_popup = column_popup;
    ops->execute_click = execute_click;
    ops->clear_current = clear_current;
}

int main(void)
{
    StarsReportDlgState state;
    StarsReportPopupState popup;
    StarsReportDlgOps ops;
    StarsReportDlgInput input;
    Trace trace;
    StarsI16 failures;
    StarsI16 checks;

    memset(&state, 0, sizeof state);
    memset(&popup, 0, sizeof popup);
    memset(&trace, 0, sizeof trace);
    install(&ops);
    state.popup = &popup;
    state.view.fields = 3;
    state.view.visible_fields = 7UL;
    state.view.report_kind = 1;
    state.view.row_count = 10;
    state.dialog_size.x = 300;
    state.dialog_size.y = 200;
    state.arial8_height = 8;
    state.font_arial8 = 2;
    state.button_face_brush = 3;
    state.frame_window = 9;
    state.game_bits = 0x0800;
    state.sort_column = 1;
    input.state = &state;
    input.ops = &ops;
    input.context = &trace;
    failures = 0;
    checks = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(stars_report_dlg(&input, 20, 1, 0, 0) == 1);
    CHECK(trace.creates == 2 && state.vscroll_window == 10);
    CHECK(state.view.hscroll_window == 11 && state.view.rows_visible == 10);
    CHECK(trace.last_position_height == 121);
    CHECK(trace.sorts == 4 && trace.horizontal_layouts == 1);
    state.game_bits = (StarsU16)(state.game_bits | 0x0200);
    CHECK(stars_report_dlg(&input, 20, 15, 0, 0) == 1);
    CHECK(trace.begins == 1 && trace.draws == 1 && trace.ends == 1);
    CHECK((state.game_bits & 0x0200U) == 0U);
    CHECK(stars_report_dlg(&input, 20, 20, 4, 0) == 1);
    CHECK(trace.fills == 1);
    CHECK(stars_report_dlg(&input, 20, 36, 0, 12) == 0);
    CHECK(trace.min_tracks == 1);
    state.view.column_scroll = 3;
    CHECK(stars_report_dlg(&input, 20, 276, 1, 11UL << 16) == 0);
    CHECK(trace.hpos == 1 && state.view.field_first == 2);
    CHECK(trace.invalidates == 1 && trace.updates == 1);
    state.view.row_count = 20;
    state.view.rows_visible = 5;
    CHECK(stars_report_dlg(&input, 20, 277, 3, 10UL << 16) == 0);
    CHECK(trace.vpos == 4 && state.view.row_first == 4);
    CHECK(trace.scrolls == 1);
    CHECK(stars_report_dlg(&input, 20, 513, 0, 5UL | (4UL << 16)) == 77);
    CHECK(trace.popups == 1 && trace.last_popup_column == 0);
    CHECK(stars_report_dlg(&input, 20, 516, 0, 25UL | (20UL << 16)) == 77);
    CHECK(trace.clicks == 1 && trace.last_click_column == 2 &&
        trace.last_click_row == 4);
    CHECK(stars_report_dlg(&input, 20, 2, 0, 0) == 77);
    CHECK(trace.menus == 1 && trace.checks == 1 && trace.clears == 1 &&
        state.view.report_window == 0);
    CHECK(stars_report_dlg(&input, 20, 273, 2, 0) == 1);
    CHECK(trace.destroys == 1);
#undef CHECK
    if (failures == 0)
        printf("REPORT_DLG_CHECKS=%d FAILURES=0 RESULT=PASS\n", checks);
    else
        printf("REPORT_DLG_CHECKS=%d FAILURES=%d RESULT=FAIL\n",
            checks, failures);
    return failures == 0 ? 0 : 1;
}
