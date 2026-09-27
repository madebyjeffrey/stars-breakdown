#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "core.h"

/* A real Win16 USER/GDI adapter around the production click, popup,
 * DrawPopup, and PopupWndProc sources. The four click coordinates and window
 * rectangles are independent captures from the isolated original game. */
typedef struct LiveCase {
    const char *name;
    StarsI16 column;
    StarsI16 x;
    StarsI16 popup_kind;
    StarsI16 left;
    StarsI16 top;
    StarsI16 right;
    StarsI16 bottom;
} LiveCase;

static const LiveCase live_cases[] = {
    {"PLANET", 2, 265, 7, 396, 281, 609, 361},
    {"RESOURCE", 12, 406, 12, 553, 281, 750, 361},
    {"MINE", 6, 160, 8, 305, 281, 504, 361},
    {"FACTORY", 7, 205, 8, 356, 281, 549, 361}
};

typedef struct LiveResourceCase {
    StarsI16 resources;
    StarsI16 research_percent;
    StarsI16 exempt;
    StarsI16 adjusted;
} LiveResourceCase;

static const LiveResourceCase live_resource_cases[] = {
    {21, 50, 0, 10},
    {1000, 50, 0, 500},
    {32767, 100, 0, 0},
    {32767, 100, 1, 32767},
    {32767, 127, 0, 0},
    {21, -50, 0, 32}
};

static HINSTANCE live_instance;
static HWND live_report;
static HWND live_popup;
static HWND live_last_popup;
static HDC live_paint_dc;
static PAINTSTRUCT live_paint_struct;
static PAINTSTRUCT live_report_paint_struct;
static HFONT live_font;
static FILE *live_file;
static StarsU8 live_globalpd[22];
static StarsExecuteReportClickState live_state;
static StarsExecuteReportClickOps live_click_ops;
static StarsReportDlgState live_report_state;
static StarsReportDlgOps live_report_ops;
static StarsReportDlgInput live_report_input;
static StarsPopupOps live_popup_ops;
static StarsPopupWndProcOps live_wnd_ops;
static StarsPopupWndProcInput live_wnd_input;
static StarsDrawPopupOps live_draw_ops;
static StarsI16 live_popup_variable;
static StarsI16 live_popup_kind;
static StarsI16 live_selected;
static StarsI16 live_invalidated;
static StarsI16 live_click_dc_gets;
static StarsI16 live_click_dc_releases;
static StarsI16 live_paints;
static StarsI16 live_draws;
static StarsI16 live_painted_kind;
static StarsI16 live_painted_pixel;
static StarsI16 live_report_dispatches;
static StarsI16 live_report_column;
static StarsI16 live_report_row;
static StarsI16 live_report_paints;
static StarsI16 live_resources;
static StarsI16 live_research_percent;
static StarsI16 live_resource_exempt;
static int live_checks;
static int live_failures;

static void live_check(const char *name, int condition)
{
    ++live_checks;
    if (!condition)
        ++live_failures;
    fprintf(live_file, "%s=%s\n", name, condition ? "PASS" : "FAIL");
}

static StarsU16 live_word(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU16)(bytes[offset] |
        ((StarsU16)bytes[offset + 1U] << 8));
}

static StarsU16 live_click_get_dc(void *context, StarsU16 hwnd)
{
    (void)context;
    ++live_click_dc_gets;
    return (StarsU16)GetDC((HWND)hwnd);
}

static void live_click_release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)context;
    ++live_click_dc_releases;
    ReleaseDC((HWND)hwnd, (HDC)dc);
}

static StarsI16 live_get_planet(void *context, StarsI16 row,
    StarsExecuteReportClickPlanet *planet)
{
    (void)context;
    (void)row;
    memset(planet, 0, sizeof *planet);
    planet->id = 10;
    planet->mineral_weight = (StarsU32)((10UL << 20) | (10UL << 8));
    planet->resources = live_resources;
    planet->research_percent = live_research_percent;
    if (live_resource_exempt)
        planet->flags = 0x8000U;
    return 1;
}

static void live_select_planet(void *context, StarsI16 id)
{
    (void)context;
    live_selected = id;
}

static void live_invalidate(void *context, StarsI16 report, StarsI16 full)
{
    (void)context;
    (void)report;
    (void)full;
    ++live_invalidated;
    InvalidateRect(live_report, (const RECT FAR *)0, FALSE);
}

static StarsU16 live_report_begin_paint(void *context, StarsU16 hwnd,
    StarsRect *rect)
{
    HDC dc;
    (void)context;
    (void)rect;
    dc = BeginPaint((HWND)hwnd, &live_report_paint_struct);
    ++live_report_paints;
    return (StarsU16)dc;
}

static void live_report_end_paint(void *context, StarsU16 hwnd,
    StarsRect *rect)
{
    (void)context;
    (void)rect;
    EndPaint((HWND)hwnd, &live_report_paint_struct);
}

static void live_report_draw_view(void *context, StarsU16 hwnd,
    StarsU16 dc, const StarsRect *rect,
    const StarsReportViewState *view)
{
    (void)context;
    (void)hwnd;
    (void)rect;
    (void)view;
    TextOut((HDC)dc, 3, 3, "REPORT", 6);
}

static void live_report_execute_click(void *context, StarsPoint point,
    StarsI16 report_kind, StarsI16 column, StarsI16 row)
{
    (void)context;
    ++live_report_dispatches;
    live_report_column = column;
    live_report_row = row;
    stars_execute_report_click(&live_state, point, report_kind, column,
        row, &live_click_ops);
}

static StarsI16 live_max(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 unused)
{
    (void)context;
    (void)planet;
    (void)unused;
    return 1200;
}

static StarsI16 live_operable(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 unused)
{
    (void)context;
    (void)planet;
    (void)unused;
    return 25;
}

static StarsI16 live_client_to_screen(void *context, StarsU16 hwnd,
    StarsPoint *point)
{
    POINT converted;
    (void)context;
    converted.x = point->x;
    converted.y = point->y;
    ClientToScreen((HWND)hwnd, &converted);
    point->x = (StarsI16)converted.x;
    point->y = (StarsI16)converted.y;
    return 1;
}

static StarsU16 live_get_dc(void *context, StarsU16 hwnd)
{
    (void)context;
    return (StarsU16)GetDC((HWND)hwnd);
}

static StarsU16 live_select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    (void)context;
    return (StarsU16)SelectObject((HDC)hdc, (HGDIOBJ)font);
}

static StarsPoint live_measure(void *context,
    const StarsPopupMeasureCase *measure)
{
    StarsPoint result;
    (void)context;
    result.y = 78;
    if (measure->popup_kind == 7)
        result.x = 211;
    else if (measure->popup_kind == 12)
        result.x = 195;
    else
        result.x = live_popup_variable ? 191 : 197;
    return result;
}

static StarsI16 live_metric(void *context, StarsI16 index)
{
    (void)context;
    return (StarsI16)GetSystemMetrics(index);
}

static StarsU16 live_create_popup(void *context, StarsU16 parent,
    StarsU32 style, StarsI16 x, StarsI16 y, StarsI16 width,
    StarsI16 height)
{
    HWND hwnd;
    (void)context;
    hwnd = CreateWindow("starspopup", "", (DWORD)style, x, y, width,
        height, (HWND)parent, (HMENU)0, live_instance, (LPSTR)0);
    live_popup = live_last_popup = hwnd;
    live_wnd_input.popup_window = (StarsU16)hwnd;
    live_wnd_input.popup_kind = (StarsU16)live_popup_kind;
    return (StarsU16)hwnd;
}

static void live_release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)context;
    ReleaseDC((HWND)hwnd, (HDC)dc);
}

static void live_send_font(void *context, StarsU16 hwnd,
    StarsU16 message, StarsU16 font)
{
    (void)context;
    SendMessage((HWND)hwnd, message, (WPARAM)font, 0L);
}

static void live_capture(void *context, StarsU16 hwnd)
{
    (void)context;
    SetCapture((HWND)hwnd);
}

static StarsI16 live_draw_get_rect(void *context, StarsU16 hwnd,
    StarsRect *rect)
{
    RECT actual;
    (void)context;
    GetClientRect((HWND)hwnd, &actual);
    rect->left = (StarsI16)actual.left;
    rect->top = (StarsI16)actual.top;
    rect->right = (StarsI16)actual.right;
    rect->bottom = (StarsI16)actual.bottom;
    return 1;
}

static void live_draw_case(void *context, const StarsDrawPopupCase *popup)
{
    COLORREF color;
    (void)context;
    ++live_draws;
    live_painted_kind = popup->popup_kind;
    TextOut((HDC)popup->hdc, 3, 3, "PLANET 10", 9);
    SetPixel((HDC)popup->hdc, 1, 1, RGB(255, 0, 0));
    color = GetPixel((HDC)popup->hdc, 1, 1);
    live_painted_pixel = color == RGB(255, 0, 0);
}

static void live_wnd_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)live_draw_get_rect(context, hwnd, rect);
}

static StarsU16 live_begin_paint(void *context, StarsU16 hwnd,
    StarsRect *rect)
{
    (void)context;
    (void)rect;
    live_paint_dc = BeginPaint((HWND)hwnd, &live_paint_struct);
    ++live_paints;
    return (StarsU16)live_paint_dc;
}

static void live_draw_popup(void *context, StarsU16 hwnd, StarsU16 dc)
{
    StarsDrawPopupInput input;
    (void)context;
    memset(&input, 0, sizeof input);
    input.hwnd = hwnd;
    input.hdc = dc;
    input.popup_kind = live_popup_kind;
    input.packed_popupdata = live_globalpd;
    input.packed_popupdata_size = sizeof live_globalpd;
    input.font_normal = (StarsU16)live_font;
    input.font_bold = (StarsU16)live_font;
    input.arial8_height = 20;
    input.ops = &live_draw_ops;
    stars_draw_popup(&input);
}

static void live_end_paint(void *context, StarsU16 hwnd,
    const StarsRect *rect)
{
    (void)context;
    (void)rect;
    EndPaint((HWND)hwnd, &live_paint_struct);
}

static void live_destroy(void *context, StarsU16 hwnd)
{
    (void)context;
    DestroyWindow((HWND)hwnd);
    live_popup = 0;
}

static void live_release_capture(void *context)
{
    (void)context;
    ReleaseCapture();
}

static StarsI32 live_default_proc(void *context, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    (void)context;
    return (StarsI32)DefWindowProc((HWND)hwnd, message, (WPARAM)wparam,
        (LPARAM)lparam);
}

static LONG FAR PASCAL live_popup_wndproc(HWND hwnd, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    return (LONG)stars_popup_wnd_proc(&live_wnd_input, (StarsU16)hwnd,
        (StarsU16)message, (StarsU16)wparam, (StarsU32)lparam);
}

static LONG FAR PASCAL live_report_wndproc(HWND hwnd, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    if (live_report_input.state &&
            (message == WM_LBUTTONDOWN || message == WM_PAINT))
        return (LONG)stars_report_dlg(&live_report_input,
            (StarsU16)hwnd, (StarsU16)message, (StarsU16)wparam,
            (StarsU32)lparam);
    return DefWindowProc(hwnd, message, wparam, lparam);
}

static void live_show_popup(void *context, StarsPoint point,
    const StarsExecuteReportClickPopup *popup)
{
    StarsPopupInput input;
    (void)context;
    live_popup_kind = popup->kind;
    live_popup_variable = popup->planet_variable;
    memset(&input, 0, sizeof input);
    input.hwnd = (StarsU16)live_report;
    input.x = point.x;
    input.y = point.y;
    input.popup_kind = popup->kind;
    input.planet_id = popup->planet_id;
    input.font_normal = (StarsU16)live_font;
    input.line_height = 20;
    input.line_height_large = 24;
    input.ops = &live_popup_ops;
    stars_popup(&input);
}

static void live_capture_only(void *context, StarsPoint point,
    const StarsExecuteReportClickPopup *popup)
{
    (void)context;
    (void)point;
    (void)popup;
}

static int live_register_class(const char *name, WNDPROC proc)
{
    WNDCLASS wc;
    memset(&wc, 0, sizeof wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = proc;
    wc.hInstance = live_instance;
    wc.hCursor = LoadCursor((HINSTANCE)0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = name;
    return RegisterClass(&wc) != 0;
}

static void live_setup_ops(void)
{
    memset(&live_state, 0, sizeof live_state);
    memset(&live_click_ops, 0, sizeof live_click_ops);
    memset(&live_report_state, 0, sizeof live_report_state);
    memset(&live_report_ops, 0, sizeof live_report_ops);
    memset(&live_report_input, 0, sizeof live_report_input);
    memset(&live_popup_ops, 0, sizeof live_popup_ops);
    memset(&live_wnd_ops, 0, sizeof live_wnd_ops);
    memset(&live_wnd_input, 0, sizeof live_wnd_input);
    memset(&live_draw_ops, 0, sizeof live_draw_ops);
    memset(live_globalpd, 0, sizeof live_globalpd);
    live_resources = 35;
    live_research_percent = 15;
    live_resource_exempt = 0;

    live_state.report_window = (StarsU16)live_report;
    live_state.transfer_dialog = -1;
    live_click_ops.get_dc = live_click_get_dc;
    live_click_ops.release_dc = live_click_release_dc;
    live_click_ops.get_planet = live_get_planet;
    live_click_ops.select_planet = live_select_planet;
    live_click_ops.invalidate_report = live_invalidate;
    live_click_ops.max_mines = live_max;
    live_click_ops.max_operable_mines = live_operable;
    live_click_ops.max_factories = live_max;
    live_click_ops.max_operable_factories = live_operable;
    live_click_ops.popup = live_show_popup;
    live_click_ops.globalpd_raw = live_globalpd;
    live_click_ops.globalpd_raw_size = sizeof live_globalpd;

    live_report_state.view.report_kind = 0;
    live_report_state.view.fields = 13;
    live_report_state.view.rows_visible = 1;
    live_report_state.view.row_count = 1;
    live_report_state.arial8_height = 20;
    live_report_ops.begin_paint = live_report_begin_paint;
    live_report_ops.end_paint = live_report_end_paint;
    live_report_ops.draw_report_view = live_report_draw_view;
    live_report_ops.execute_click = live_report_execute_click;
    live_report_input.state = &live_report_state;
    live_report_input.ops = &live_report_ops;

    live_popup_ops.client_to_screen = live_client_to_screen;
    live_popup_ops.get_dc = live_get_dc;
    live_popup_ops.select_font = live_select_font;
    live_popup_ops.measure = live_measure;
    live_popup_ops.get_system_metric = live_metric;
    live_popup_ops.create_window = live_create_popup;
    live_popup_ops.release_dc = live_release_dc;
    live_popup_ops.send_message = live_send_font;
    live_popup_ops.set_capture = live_capture;

    live_wnd_ops.get_client_rect = live_wnd_rect;
    live_wnd_ops.begin_paint = live_begin_paint;
    live_wnd_ops.draw_popup = live_draw_popup;
    live_wnd_ops.end_paint = live_end_paint;
    live_wnd_ops.destroy_window = live_destroy;
    live_wnd_ops.release_capture = live_release_capture;
    live_wnd_ops.default_proc = live_default_proc;
    live_wnd_input.ops = &live_wnd_ops;

    live_draw_ops.get_client_rect = live_draw_get_rect;
    live_draw_ops.draw_planet_population = live_draw_case;
    live_draw_ops.draw_factory_mine = live_draw_case;
    live_draw_ops.draw_resource = live_draw_case;
}

int PASCAL WinMain(HINSTANCE instance, HINSTANCE previous,
    LPSTR command, int show)
{
    RECT report_rect;
    RECT popup_rect;
    RECT client_rect;
    POINT screen;
    char class_name[64];
    StarsPoint click;
    HWND popup;
    DWORD style;
    int index;
    int paints_before;
    int draws_before;
    const LiveCase *test;
    (void)previous;
    (void)command;
    (void)show;
    live_instance = instance;
    live_file = fopen("LIVUI.TXT", "w");
    if (!live_file)
        return 2;
    if (!live_register_class("starsreport", live_report_wndproc) ||
            !live_register_class("starspopup", live_popup_wndproc)) {
        fprintf(live_file, "REGISTER=FAIL\n");
        fclose(live_file);
        return 2;
    }
    live_font = (HFONT)GetStockObject(SYSTEM_FONT);
    live_report = CreateWindow("starsreport", "Planet Summary Report",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 340, 312, 600, 400,
        (HWND)0, (HMENU)0, instance, (LPSTR)0);
    if (!live_report) {
        fprintf(live_file, "REPORT_CREATE=FAIL\n");
        fclose(live_file);
        return 2;
    }
    live_setup_ops();
    GetWindowRect(live_report, &report_rect);
    screen.x = screen.y = 0;
    ClientToScreen(live_report, &screen);
    fprintf(live_file, "REPORT_RECT=%d,%d,%d,%d\n",
        report_rect.left, report_rect.top,
        report_rect.right, report_rect.bottom);
    fprintf(live_file, "CLIENT_ORIGIN=%d,%d\n", screen.x, screen.y);
    live_check("REPORT_RECT_MATCH", report_rect.left == 340 &&
        report_rect.top == 312 && report_rect.right == 940 &&
        report_rect.bottom == 712);
    live_check("CLIENT_ORIGIN_MATCH", screen.x == 344 && screen.y == 335);

    for (index = 0; index < 4; ++index) {
        test = &live_cases[index];
        click.x = test->x;
        click.y = 26;
        live_report_state.view.visible_fields =
            (StarsU32)1UL << test->column;
        live_report_state.view.column_width[test->column] = 255U;
        paints_before = live_paints;
        draws_before = live_draws;
        SendMessage(live_report, WM_LBUTTONDOWN, MK_LBUTTON,
            MAKELONG((WORD)click.x, (WORD)click.y));
        popup = live_popup;
        fprintf(live_file, "CASE=%s\n", test->name);
        live_check("REPORT_MESSAGE_DISPATCH", live_report_dispatches ==
            index + 1 && live_report_column == test->column &&
            live_report_row == 0);
        live_check("WINDOW_CREATED", popup != 0 && IsWindow(popup));
        if (!popup || !IsWindow(popup))
            continue;
        GetWindowRect(popup, &popup_rect);
        GetClientRect(popup, &client_rect);
        class_name[0] = 0;
        GetClassName(popup, class_name, sizeof class_name);
        style = (DWORD)GetWindowLong(popup, GWL_STYLE);
        fprintf(live_file, "RECT=%d,%d,%d,%d\n",
            popup_rect.left, popup_rect.top,
            popup_rect.right, popup_rect.bottom);
        fprintf(live_file, "CLIENT=%d,%d\n",
            client_rect.right, client_rect.bottom);
        fprintf(live_file, "STYLE=%08lX\n", (unsigned long)style);
        live_check("RECT_MATCH", popup_rect.left == test->left &&
            popup_rect.top == test->top && popup_rect.right == test->right &&
            popup_rect.bottom == test->bottom);
        live_check("CLASS_MATCH", strcmp(class_name, "starspopup") == 0);
        live_check("PARENT_MATCH", GetParent(popup) == live_report);
        live_check("STYLE_MATCH", style == 0x94800000UL);
        live_check("VISIBLE", IsWindowVisible(popup));
        live_check("CAPTURE_OWNER", GetCapture() == popup);
        live_check("SELECTED_PLANET", live_selected == 10 &&
            live_state.selected_planet_id == 10);
        live_check("INVALIDATED", live_invalidated == index + 1);
        live_check("DC_SYMMETRY", live_click_dc_gets == index + 1 &&
            live_click_dc_releases == index + 1);
        UpdateWindow(live_report);
        live_check("REPORT_REFRESH", live_report_paints >= index + 1);
        InvalidateRect(popup, (const RECT FAR *)0, TRUE);
        UpdateWindow(popup);
        live_check("REAL_PAINT", live_paints > paints_before &&
            live_draws > draws_before && live_painted_kind ==
            test->popup_kind && live_painted_pixel);
        SendMessage(popup, WM_LBUTTONUP, 0, 0L);
        live_check("DESTROYED", !IsWindow(popup) &&
            live_wnd_input.popup_window == 0 &&
            live_wnd_input.popup_kind == 0);
        live_check("CAPTURE_RELEASED", GetCapture() != popup);
    }
    /* These values test Win16 MulDiv width, half rounding, and the exemption
     * path. No original popup size is claimed, so suppress only the new
     * popup window while retaining real report-message dispatch. */
    live_click_ops.popup = live_capture_only;
    live_report_state.view.visible_fields = (StarsU32)1UL << 12;
    live_report_state.view.column_width[12] = 255U;
    for (index = 0; index < 6; ++index) {
        live_resources = live_resource_cases[index].resources;
        live_research_percent = live_resource_cases[index].research_percent;
        live_resource_exempt = live_resource_cases[index].exempt;
        SendMessage(live_report, WM_LBUTTONDOWN, MK_LBUTTON,
            MAKELONG((WORD)406U, (WORD)26U));
        live_check("RESOURCE_MULDIV", live_report_dispatches == index + 5 &&
            live_report_column == 12 && live_word(live_globalpd, 0U) == 12U &&
            live_word(live_globalpd, 4U) ==
                (StarsU16)live_resource_cases[index].resources &&
            live_word(live_globalpd, 6U) ==
                (StarsU16)live_resource_cases[index].adjusted &&
            live_click_dc_gets == index + 5 &&
            live_click_dc_releases == index + 5);
    }
    DestroyWindow(live_report);
    fprintf(live_file, "CHECKS=%d FAILURES=%d RESULT=%s\n",
        live_checks, live_failures, live_failures ? "FAIL" : "PASS");
    fclose(live_file);
    return live_failures ? 1 : 0;
}
