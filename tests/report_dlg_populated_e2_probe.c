#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 dc_calls;
    StarsI16 release_calls;
    StarsI16 scroll_creates;
    StarsI16 scroll_pos_calls;
    StarsI16 scroll_range_calls;
    StarsI16 window_pos_calls;
    StarsI16 begin_calls;
    StarsI16 end_calls;
    StarsI16 select_calls;
    StarsI16 mode_calls;
    StarsI16 frame_calls;
    StarsI16 fill_calls;
    StarsI16 text_calls;
    StarsI16 center_calls;
    StarsI16 ext_calls;
    StarsI16 right_calls;
    StarsI16 mineral_calls;
    StarsI16 production_calls;
    StarsI16 header_calls;
    StarsI16 item_calls;
    StarsI16 planet_calls;
    StarsI16 fleet_calls;
    StarsI16 battle_calls;
    StarsI16 submenu_calls;
    StarsI16 menu_checks;
    StarsI16 clear_calls;
    StarsU16 last_scroll_parent;
    StarsU32 last_scroll_style;
    StarsI16 last_report;
    StarsI16 last_row;
    StarsI16 last_field;
    StarsI16 last_menu_id;
    StarsI16 last_mineral_kind;
    StarsI32 last_mineral;
} Trace;

typedef struct Fixture {
    Trace trace;
    StarsReportItemPlanet planet;
    StarsReportItemFleet fleet;
    StarsReportItemBattle battle;
    StarsI16 active_report;
} Fixture;

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static StarsU16 get_metric(void *context, StarsU16 metric)
{
    (void)context;
    return metric == 2 ? 16 : 0;
}

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)hwnd;
    ++trace->dc_calls;
    return 9;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)hwnd;
    (void)dc;
    ++trace->release_calls;
}

static void select_object(void *context, StarsU16 dc, StarsU16 object)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)object;
    ++trace->select_calls;
}

static StarsU16 create_scroll(void *context, const StarsU8 *class_name,
    StarsU32 style, StarsU16 parent)
{
    Trace *trace = &((Fixture *)context)->trace;
    ++trace->scroll_creates;
    if (strcmp((const char *)class_name, "SCROLLBAR") != 0)
        trace->scroll_creates = -100;
    trace->last_scroll_parent = parent;
    trace->last_scroll_style = style;
    return (StarsU16)(0x20 + trace->scroll_creates);
}

static void set_window_pos(void *context, StarsU16 hwnd, StarsI16 x,
    StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)hwnd;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)flags;
    ++trace->window_pos_calls;
}

static void sticky(void *context, StarsU16 hwnd, StarsPoint *point,
    StarsI16 save)
{
    (void)context;
    (void)hwnd;
    (void)point;
    (void)save;
}

static void get_client_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context;
    (void)hwnd;
    rect->left = 0;
    rect->top = 0;
    rect->right = 240;
    rect->bottom = 100;
}

static void set_scroll_pos(void *context, StarsU16 hwnd, StarsU16 bar,
    StarsI16 position, StarsI16 redraw)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)hwnd;
    (void)bar;
    (void)position;
    (void)redraw;
    ++trace->scroll_pos_calls;
}

static void set_scroll_range(void *context, StarsU16 hwnd, StarsU16 bar,
    StarsI16 minimum, StarsI16 maximum, StarsI16 redraw)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)hwnd;
    (void)bar;
    (void)minimum;
    (void)maximum;
    (void)redraw;
    ++trace->scroll_range_calls;
}

static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)hwnd;
    ++trace->begin_calls;
    rect->left = 0;
    rect->top = 0;
    rect->right = 240;
    rect->bottom = 100;
    return 9;
}

static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)hwnd;
    (void)rect;
    ++trace->end_calls;
}

static void set_mode(void *context, StarsU16 dc, StarsU16 mode)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)mode;
    ++trace->mode_calls;
}

static void draw_frame(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 kind)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)rect;
    (void)kind;
    ++trace->frame_calls;
}

static void fill_rect(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)rect;
    (void)brush;
    ++trace->fill_calls;
}

static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)x;
    (void)y;
    (void)text;
    (void)length;
    ++trace->text_calls;
}

static void center_text(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)x;
    (void)y;
    (void)text;
    (void)length;
    ++trace->center_calls;
}

static void text_out_item(void *context, StarsU16 dc, StarsI16 x,
    StarsI16 y, const StarsU8 *text, StarsU16 length)
{
    text_out(context, dc, x, y, text, (StarsI16)length);
}

static void center_text_item(void *context, StarsU16 dc, StarsI16 x,
    StarsI16 y, const StarsU8 *text, StarsU16 length)
{
    center_text(context, dc, x, y, text, (StarsI16)length);
}

static void ext_text(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsU16 options, const StarsRect *rect, const StarsU8 *text,
    StarsU16 length)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)x;
    (void)y;
    (void)options;
    (void)rect;
    (void)text;
    (void)length;
    ++trace->ext_calls;
}

static void right_text(void *context, StarsU16 dc, StarsI16 right,
    StarsI16 top, const StarsU8 *text, StarsI16 length, StarsI16 offset)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)right;
    (void)top;
    (void)text;
    (void)length;
    (void)offset;
    ++trace->right_calls;
}

static StarsI16 text_extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsU16 length)
{
    (void)context;
    (void)dc;
    (void)text;
    return (StarsI16)(length * 4U);
}

static void draw_mineral(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsI16 mineral, StarsI32 value)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)x;
    (void)y;
    trace->last_mineral_kind = mineral;
    trace->last_mineral = value;
    ++trace->mineral_calls;
}

static void draw_production(void *context, StarsU16 dc,
    const StarsRect *rect, const StarsReportItemPlanet *planet)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)dc;
    (void)rect;
    (void)planet;
    ++trace->production_calls;
}

static StarsI16 header_width_dlg(void *context, StarsI16 report,
    StarsI16 field, StarsU16 dc, StarsU8 *text, StarsU16 capacity)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)report;
    (void)dc;
    if (capacity > 0U) text[0] = 'H';
    if (capacity > 1U) text[1] = 0;
    ++trace->header_calls;
    return (StarsI16)(20 - field);
}

static StarsI16 header_width_report(void *context, StarsI16 report,
    StarsI16 field, StarsU8 *text, StarsU16 dc)
{
    Trace *trace = &((Fixture *)context)->trace;
    (void)report;
    (void)dc;
    text[0] = 'H';
    text[1] = 0;
    ++trace->header_calls;
    return (StarsI16)(20 - field);
}

static const StarsReportItemPlanet *planet_at(void *context, StarsI16 row)
{
    Fixture *fixture = (Fixture *)context;
    ++fixture->trace.planet_calls;
    return row == 0 ? &fixture->planet : 0;
}

static const StarsReportItemFleet *fleet_at(void *context, StarsI16 report,
    StarsI16 row)
{
    Fixture *fixture = (Fixture *)context;
    (void)report;
    ++fixture->trace.fleet_calls;
    return row == 0 ? &fixture->fleet : 0;
}

static const StarsReportItemBattle *battle_at(void *context, StarsI16 row)
{
    Fixture *fixture = (Fixture *)context;
    ++fixture->trace.battle_calls;
    return row == 0 ? &fixture->battle : 0;
}

static void set_color(void *context, StarsU16 dc, StarsU32 color)
{
    (void)context;
    (void)dc;
    (void)color;
}

static void draw_item(void *context, StarsU16 dc, const StarsRect *rect,
    StarsI16 report, StarsI16 row, StarsI16 field)
{
    Fixture *fixture = (Fixture *)context;
    StarsDrawReportItemOps ops;
    StarsDrawReportItemInput input;
    StarsRect copy;

    ++fixture->trace.item_calls;
    fixture->trace.last_report = report;
    fixture->trace.last_row = row;
    fixture->trace.last_field = field;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    ops.planet_at = planet_at;
    ops.fleet_at = fleet_at;
    ops.battle_at = battle_at;
    ops.text_extent = text_extent;
    ops.select_object = select_object;
    ops.set_text_color = set_color;
    ops.set_background_mode = set_mode;
    ops.ext_text_out = ext_text;
    ops.right_text_out = right_text;
    ops.text_out = text_out_item;
    ops.center_text = center_text_item;
    ops.fill_rect = fill_rect;
    ops.draw_mineral = draw_mineral;
    ops.draw_production = draw_production;
    input.font = 3;
    input.selection_kind = 1;
    input.selected_planet = fixture->planet.id;
    input.selected_fleet = fixture->fleet.id;
    input.selected_scan_fleet = fixture->fleet.id;
    input.selected_scan_point = fixture->battle.position;
    input.brush_planet_cargo = 10;
    input.brush_planet_warp = 11;
    input.brush_planet_gate = 12;
    input.color_black = 0;
    input.color_selected = 0x7f;
    input.color_warning = 0xff;
    input.color_disabled = 0xff;
    input.double_dash = (const StarsU8 *)"--";
    input.ops = &ops;
    input.context = context;
    copy = *rect;
    stars_draw_report_item(dc, &copy, report, row, field, &input);
}

static void draw_view(void *context, StarsU16 hwnd, StarsU16 dc,
    const StarsRect *rect, const StarsReportViewState *view)
{
    Fixture *fixture = (Fixture *)context;
    StarsDrawReportOps ops;
    StarsDrawReportInput input;

    (void)hwnd;
    fixture->active_report = view->report_kind;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    ops.select_object = select_object;
    ops.set_background_mode = set_mode;
    ops.fill_rect = fill_rect;
    ops.text_out = text_out;
    ops.center_text = center_text;
    ops.draw_frame = draw_frame;
    ops.header_width = header_width_report;
    ops.draw_item = draw_item;
    input.view = (StarsReportViewState *)view;
    input.arial8_height = 8;
    input.font = 3;
    input.shadow_brush = 5;
    input.button_face_brush = 6;
    input.fill_cells = 1;
    input.ops = &ops;
    input.context = context;
    stars_draw_report(hwnd, dc, rect, &input);
}

static StarsU16 get_submenu(void *context, StarsU16 hwnd, StarsI16 position)
{
    Fixture *fixture = (Fixture *)context;
    (void)hwnd;
    (void)position;
    ++fixture->trace.submenu_calls;
    return 8;
}

static void check_menu(void *context, StarsU16 menu, StarsU16 id,
    StarsU16 flags)
{
    Fixture *fixture = (Fixture *)context;
    (void)menu;
    (void)flags;
    ++fixture->trace.menu_checks;
    fixture->trace.last_menu_id = (StarsI16)id;
}

static void clear_current(void *context)
{
    ++((Fixture *)context)->trace.clear_calls;
}

static void init_fixture(Fixture *fixture)
{
    StarsI16 i;

    memset(fixture, 0, sizeof *fixture);
    fixture->planet.id = 7;
    fixture->planet.inhabited = 1;
    fixture->planet.population = 10;
    fixture->planet.max_population = 20;
    fixture->planet.name = (const StarsU8 *)"New Eden";
    fixture->planet.owner_name = (const StarsU8 *)"Terran";
    fixture->planet.production_name = (const StarsU8 *)"Factory";
    fixture->planet.desirability_label = (const StarsU8 *)"Hab";
    fixture->planet.capacity_percent = 72;
    fixture->planet.desirability = 8;
    fixture->planet.optimal_value = 12;
    fixture->planet.resources = 1234;
    fixture->planet.research_percent = 30;
    fixture->planet.defense_survival_percent = 88;
    for (i = 0; i < 3; ++i) {
        fixture->planet.mineral_weight[i] = 100L + i;
        fixture->planet.estimated_minerals[i] = 200L + i;
        fixture->planet.mineral_concentration[i] = (StarsI16)(50 + i);
        fixture->planet.current_installations[i] = (StarsI16)(3 + i);
        fixture->planet.operable_installations[i] = (StarsI16)(4 + i);
        fixture->planet.brush_status[i] = (StarsU16)(10 + i);
    }
    fixture->fleet.id = 4;
    fixture->fleet.position.x = 12;
    fixture->fleet.position.y = 34;
    fixture->fleet.name = (const StarsU8 *)"Fleet 5";
    fixture->fleet.location_name = (const StarsU8 *)"New Eden";
    fixture->fleet.destination_name = (const StarsU8 *)"Beta";
    fixture->fleet.eta_name = (const StarsU8 *)"2 years";
    fixture->fleet.task_name = (const StarsU8 *)"Colonize";
    fixture->fleet.plan_name = (const StarsU8 *)"Plan A";
    fixture->fleet.primary_name = (const StarsU8 *)"Scout";
    fixture->fleet.primary_count = 3;
    fixture->fleet.fuel = 600;
    fixture->fleet.fuel_safe = 1;
    fixture->fleet.cloak_percent = 25;
    fixture->fleet.combat_ship_count = 2;
    fixture->fleet.scanner_visible = 1;
    fixture->fleet.scanner_percent = 80;
    fixture->fleet.weight = 1000;
    for (i = 0; i < 4; ++i) fixture->fleet.minerals[i] = 10L + i * 10L;
    for (i = 0; i < STARS_REPORT_ITEM_SHIP_TYPES; ++i)
        fixture->fleet.ship_counts[i] = 1;
    for (i = 0; i < 6; ++i)
        fixture->fleet.ship_category_count[i] = 10L + i;
    fixture->battle.position.x = 12;
    fixture->battle.position.y = 34;
    fixture->battle.location_name = (const StarsU8 *)"Battle site";
    fixture->battle.cplr = 75;
    fixture->battle.units[5] = 1;
    fixture->battle.units[0xfd] = 9;
    fixture->battle.units[0xfe] = 7;
    fixture->battle.kills[0] = 2;
    fixture->battle.kills[1] = 1;
}

static void put_view(StarsU8 *raw, StarsI16 report, StarsI16 fields)
{
    memset(raw, 0, STARS_REPORT_DLG_VIEW_SIZE);
    put16(raw, 0, 0x7fffU);
    put16(raw, 4, (StarsU16)report);
    put16(raw, 6, (StarsU16)fields);
    put16(raw, 8, 0);
    put16(raw, 10, 0);
    put16(raw, 12, 1);
    put16(raw, 14, 0);
    put16(raw, 20, 320);
    put16(raw, 22, 220);
    put16(raw, 42, 1);
    put16(raw, 44, 1);
    put16(raw, 46, 0);
    put16(raw, 52, 0);
}

static void install_ops(StarsReportDlgOps *ops)
{
    memset(ops, 0, sizeof *ops);
    ops->get_client_rect = get_client_rect;
    ops->get_dc = get_dc;
    ops->select_object = select_object;
    ops->release_dc = release_dc;
    ops->set_window_pos = set_window_pos;
    ops->sticky_position = sticky;
    ops->create_scroll = create_scroll;
    ops->get_system_metrics = get_metric;
    ops->set_scroll_pos = set_scroll_pos;
    ops->set_scroll_range = set_scroll_range;
    ops->begin_paint = begin_paint;
    ops->end_paint = end_paint;
    ops->header_width = header_width_dlg;
    ops->get_submenu = get_submenu;
    ops->check_menu_item = check_menu;
    ops->clear_current = clear_current;
    ops->draw_report_view = draw_view;
}

int main(void)
{
    Fixture fixture;
    StarsReportDlgState state;
    StarsReportDlgOps ops;
    StarsReportDlgPackedInput packed;
    StarsReportPopupState popup;
    StarsU8 raw[STARS_REPORT_DLG_VIEW_SIZE];
    StarsI16 failures;
    StarsI16 checks;
    StarsI16 report;
    StarsI16 fields;

    init_fixture(&fixture);
    memset(&state, 0, sizeof state);
    memset(&popup, 0, sizeof popup);
    install_ops(&ops);
    state.popup = &popup;
    state.arial8_height = 8;
    state.font_arial8 = 2;
    state.button_face_brush = 6;
    state.frame_window = 9;
    state.current_valid = 0;
    packed.report.state = &state;
    packed.report.ops = &ops;
    packed.report.context = &fixture;
    packed.view_record = raw;
    packed.view_size = sizeof raw;
    failures = 0;
    checks = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    for (report = 0; report <= 3; ++report) {
        fields = report == 0 ? 15 : (report == 3 ? 15 : 12);
        put_view(raw, report, fields);
        CHECK(stars_report_dlg_packed(&packed, 20, 1, 0, 0) == 1);
        CHECK(state.view.report_kind == report && state.view.fields ==
            (StarsU16)fields && state.view.row_count == 1);
        CHECK(stars_report_dlg(&packed.report, 20, 15, 0, 0) == 1);
        CHECK(fixture.active_report == report && fixture.trace.item_calls > 0);
        CHECK(report == 0 ? fixture.trace.planet_calls > 0 : 1);
        CHECK((report == 1 || report == 2) ? fixture.trace.fleet_calls > 0 : 1);
        CHECK(report == 3 ? fixture.trace.battle_calls > 0 : 1);
        CHECK(stars_report_dlg(&packed.report, 20, 2, 0, 0) == 0);
    }
    CHECK(fixture.trace.scroll_creates == 8);
    CHECK(fixture.trace.last_scroll_parent == 20);
    CHECK(fixture.trace.begin_calls == 4 && fixture.trace.end_calls == 4);
    CHECK(fixture.trace.dc_calls == 4 && fixture.trace.release_calls == 4);
    CHECK(fixture.trace.header_calls >= 54);
    CHECK(fixture.trace.mode_calls > 20 && fixture.trace.ext_calls > 10);
    CHECK(fixture.trace.mineral_calls > 20 && fixture.trace.right_calls > 10);
    CHECK(fixture.trace.production_calls > 0);
    CHECK(fixture.trace.submenu_calls == 4 && fixture.trace.menu_checks == 4);
    CHECK(fixture.trace.clear_calls == 4);
    CHECK(fixture.trace.last_menu_id == 0x0901);
#undef CHECK
    if (failures == 0)
        printf("REPORT_DLG_POPULATED_E2_CHECKS=%d FAILURES=0 RESULT=PASS\n",
            checks);
    else
        printf("REPORT_DLG_POPULATED_E2_CHECKS=%d FAILURES=%d RESULT=FAIL\n",
            checks, failures);
    return failures == 0 ? 0 : 1;
}
