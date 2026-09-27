#include <stdio.h>
#include <string.h>
#include "core.h"

/* The production ReportDlg hit-test -> ExecuteReportClick dispatch chain.
 * Column masks/widths reflect the four isolated original click coordinates;
 * modal and resource-exempt branches are additional synthetic boundaries. */
typedef struct Trace {
    StarsReportDlgState report;
    StarsReportDlgOps report_ops;
    StarsReportDlgInput report_input;
    StarsExecuteReportClickState click;
    StarsExecuteReportClickOps click_ops;
    StarsExecuteReportClickPopup popup;
    StarsU8 globalpd[22];
    StarsI16 dispatches;
    StarsI16 popups;
    StarsI16 header_popups;
    StarsI16 selections;
    StarsI16 invalidations;
    StarsI16 beeps;
    StarsI16 dc_gets;
    StarsI16 dc_releases;
    StarsI16 last_column;
    StarsI16 last_row;
    StarsI16 resource_exempt;
    StarsI16 resources;
    StarsI16 research_percent;
    StarsI16 has_starbase;
    StarsI16 defense_count;
} Trace;

static const StarsU8 captured_starbase[22] = {
    0x0b, 0x00, 0x12, 0x00, 0xe7, 0x19, 0x01, 0x00
};
static const StarsU8 captured_defense[22] = {
    0x09, 0x00, 0x00, 0x80, 0x09, 0x13, 0x9e, 0x18,
    0x37, 0x11
};

static int checks;
static int failures;

static void check_at(int condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
        fprintf(stderr, "report chain failed at line %d\n", line);
    }
}
#define CHECK(condition) check_at((condition), __LINE__)

static StarsU16 click_get_dc(void *context, StarsU16 hwnd)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    ++trace->dc_gets;
    return 1;
}

static void click_release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)dc;
    ++trace->dc_releases;
}

static StarsI16 get_planet(void *context, StarsI16 row,
    StarsExecuteReportClickPlanet *planet)
{
    Trace *trace = (Trace *)context;
    (void)row;
    memset(planet, 0, sizeof *planet);
    planet->id = 10;
    planet->mineral_weight = (StarsU32)((10UL << 20) | (10UL << 8));
    planet->resources = trace->resources;
    planet->research_percent = trace->research_percent;
    planet->has_starbase = (StarsU16)trace->has_starbase;
    planet->defense_count = (StarsU16)trace->defense_count;
    if (trace->resource_exempt)
        planet->flags = 0x8000U;
    return 1;
}

static void select_planet(void *context, StarsI16 id)
{
    Trace *trace = (Trace *)context;
    if (id == 10)
        ++trace->selections;
}

static void invalidate(void *context, StarsI16 report, StarsI16 full)
{
    Trace *trace = (Trace *)context;
    (void)report; (void)full;
    ++trace->invalidations;
}

static StarsI16 max_installation(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 unused)
{
    (void)context; (void)planet; (void)unused;
    return 1200;
}

static StarsI16 operable_installation(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 unused)
{
    (void)context; (void)planet; (void)unused;
    return 25;
}

static StarsI16 best_defense(void *context, StarsU16 *part)
{
    (void)context;
    *part = 0x8000U;
    return 1;
}

static void show_popup(void *context, StarsPoint point,
    const StarsExecuteReportClickPopup *popup)
{
    Trace *trace = (Trace *)context;
    (void)point;
    ++trace->popups;
    trace->popup = *popup;
}

static void beep(void *context)
{
    ++((Trace *)context)->beeps;
}

static void execute_click(void *context, StarsPoint point,
    StarsI16 report_kind, StarsI16 column, StarsI16 row)
{
    Trace *trace = (Trace *)context;
    ++trace->dispatches;
    trace->last_column = column;
    trace->last_row = row;
    stars_execute_report_click(&trace->click, point, report_kind, column,
        row, &trace->click_ops);
}

static void column_popup(void *context, StarsPoint point,
    StarsI16 column, StarsI16 right_button)
{
    Trace *trace = (Trace *)context;
    (void)point; (void)column; (void)right_button;
    ++trace->header_popups;
}

static void install(Trace *trace)
{
    memset(trace, 0, sizeof *trace);
    trace->report.view.report_kind = 0;
    trace->resources = 35;
    trace->research_percent = 15;
    trace->report.view.fields = 13;
    trace->report.view.rows_visible = 1;
    trace->report.view.row_count = 1;
    trace->report.arial8_height = 20;
    trace->report_ops.execute_click = execute_click;
    trace->report_ops.column_popup = column_popup;
    trace->report_input.state = &trace->report;
    trace->report_input.ops = &trace->report_ops;
    trace->report_input.context = trace;

    trace->click.report_window = 42;
    trace->click.transfer_dialog = -1;
    trace->click_ops.get_dc = click_get_dc;
    trace->click_ops.release_dc = click_release_dc;
    trace->click_ops.get_planet = get_planet;
    trace->click_ops.select_planet = select_planet;
    trace->click_ops.invalidate_report = invalidate;
    trace->click_ops.max_mines = max_installation;
    trace->click_ops.max_operable_mines = operable_installation;
    trace->click_ops.max_factories = max_installation;
    trace->click_ops.max_operable_factories = operable_installation;
    trace->click_ops.best_defense = best_defense;
    trace->click_ops.globalpd_defense_part = captured_defense + 2;
    trace->click_ops.globalpd_defense_part_size = 8U;
    trace->click_ops.globalpd_starbase_fleet_far = 0x19e70012UL;
    trace->click_ops.popup = show_popup;
    trace->click_ops.message_beep = beep;
    trace->click_ops.globalpd_raw = trace->globalpd;
    trace->click_ops.globalpd_raw_size = sizeof trace->globalpd;
    trace->click_ops.context = trace;
}

static void click_cell(Trace *trace, StarsI16 column, StarsI16 x)
{
    trace->report.view.visible_fields = (StarsU32)1UL << column;
    trace->report.view.column_width[column] = 255U;
    stars_report_dlg(&trace->report_input, 42, 513U, 1U,
        ((StarsU32)26U << 16) | (StarsU16)x);
}

int main(void)
{
    Trace trace;
    Trace scrolled;
    Trace starbase;
    Trace defense;
    install(&trace);
    click_cell(&trace, 2, 265);
    CHECK(trace.dispatches == 1 && trace.last_column == 2 &&
        trace.last_row == 0);
    CHECK(trace.popups == 1 && trace.popup.kind == 7 &&
        trace.popup.planet_id == 10);
    CHECK(trace.selections == 1 && trace.invalidations == 1);
    click_cell(&trace, 12, 406);
    CHECK(trace.dispatches == 2 && trace.last_column == 12);
    CHECK(trace.popups == 2 && trace.popup.kind == 12 &&
        trace.popup.planet_variable == 35 && trace.popup.planet_value == 30);
    click_cell(&trace, 6, 160);
    CHECK(trace.dispatches == 3 && trace.last_column == 6);
    CHECK(trace.popups == 3 && trace.popup.kind == 8 &&
        trace.popup.planet_variable == 0 && trace.popup.planet_value == 10 &&
        trace.popup.planet_maximum == 1200 &&
        trace.popup.planet_minimum == 25);
    click_cell(&trace, 7, 205);
    CHECK(trace.dispatches == 4 && trace.last_column == 7);
    CHECK(trace.popups == 4 && trace.popup.kind == 8 &&
        trace.popup.planet_variable == 1 && trace.popup.planet_value == 10);
    CHECK(trace.dc_gets == 4 && trace.dc_releases == 4);

    trace.resource_exempt = 1;
    click_cell(&trace, 12, 406);
    CHECK(trace.dispatches == 5 && trace.popups == 5);
    CHECK(trace.popup.planet_variable == 35 &&
        trace.popup.planet_value == 35);
    trace.resource_exempt = 0;

    trace.click.production_window = 1;
    click_cell(&trace, 12, 406);
    CHECK(trace.dispatches == 6 && trace.popups == 5 && trace.beeps == 1);
    CHECK(trace.dc_gets == 6 && trace.dc_releases == 6);
    trace.click.production_window = 0;

    trace.report.view.visible_fields = (StarsU32)1UL << 12;
    stars_report_dlg(&trace.report_input, 42, 513U, 1U,
        ((StarsU32)10U << 16) | 406U);
    CHECK(trace.header_popups == 1 && trace.dispatches == 6);
    stars_report_dlg(&trace.report_input, 42, 513U, 1U,
        ((StarsU32)26U << 16) | 520U);
    CHECK(trace.dispatches == 6 && trace.popups == 5);
    CHECK(trace.selections == 5 && trace.invalidations == 5);

    /* The retained original "report-columns-after-scroll.bmp" shows Mine
     * immediately before Fact in the same Planet Summary Report view. Its
     * pixel spans imply this coherent visible-field layout (half-pixel widths
     * rounded to the report's even-pixel grid), unlike the isolated cases. */
    install(&scrolled);
    scrolled.report.view.visible_fields =
        (StarsU32)(1UL << 0 | 1UL << 6 | 1UL << 7 | 1UL << 8 |
                   1UL << 9 | 1UL << 10 | 1UL << 11 | 1UL << 12);
    scrolled.report.view.column_width[0] = 72U;
    scrolled.report.view.column_width[6] = 17U;
    scrolled.report.view.column_width[7] = 19U;
    scrolled.report.view.column_width[8] = 26U;
    scrolled.report.view.column_width[9] = 44U;
    scrolled.report.view.column_width[10] = 37U;
    scrolled.report.view.column_width[11] = 35U;
    scrolled.report.view.column_width[12] = 35U;
    stars_report_dlg(&scrolled.report_input, 42, 513U, 1U,
        ((StarsU32)26U << 16) | 160U);
    CHECK(scrolled.dispatches == 1 && scrolled.last_column == 6 &&
        scrolled.popup.kind == 8 && scrolled.popup.planet_variable == 0);
    stars_report_dlg(&scrolled.report_input, 42, 513U, 1U,
        ((StarsU32)26U << 16) | 205U);
    CHECK(scrolled.dispatches == 2 && scrolled.last_column == 7 &&
        scrolled.popup.kind == 8 && scrolled.popup.planet_variable == 1);
    CHECK(scrolled.popups == 2 && scrolled.dc_gets == 2 &&
        scrolled.dc_releases == 2);

    install(&starbase);
    starbase.has_starbase = 1;
    click_cell(&starbase, 1, 190);
    CHECK(starbase.dispatches == 1 && starbase.last_column == 1 &&
        starbase.popup.kind == 11);
    CHECK(memcmp(starbase.globalpd, captured_starbase, 22U) == 0);
    CHECK(starbase.dc_gets == 1 && starbase.dc_releases == 1);

    install(&defense);
    defense.defense_count = 10;
    click_cell(&defense, 8, 171);
    CHECK(defense.dispatches == 1 && defense.last_column == 8 &&
        defense.popup.kind == 9);
    CHECK(memcmp(defense.globalpd, captured_defense, 22U) == 0);
    CHECK(defense.dc_gets == 1 && defense.dc_releases == 1);

    /* Original Win16 GDI MulDiv uses a 32-bit product and rounds .5 away
     * from zero, unlike C's 16-bit intermediate and truncating division. */
    install(&trace);
    trace.resources = 21;
    trace.research_percent = 50;
    click_cell(&trace, 12, 406);
    CHECK(trace.popup.kind == 12 && trace.popup.planet_variable == 21 &&
        trace.popup.planet_value == 10);
    trace.resources = 1000;
    click_cell(&trace, 12, 406);
    CHECK(trace.popup.planet_variable == 1000 &&
        trace.popup.planet_value == 500);
    trace.resources = 32767;
    trace.research_percent = 100;
    click_cell(&trace, 12, 406);
    CHECK(trace.popup.planet_variable == 32767 &&
        trace.popup.planet_value == 0);
    trace.resource_exempt = 1;
    click_cell(&trace, 12, 406);
    CHECK(trace.popup.planet_value == 32767);
    trace.resource_exempt = 0;
    trace.research_percent = 127;
    click_cell(&trace, 12, 406);
    CHECK(trace.popup.planet_variable == 32767 &&
        trace.popup.planet_value == 0);
    trace.resources = 21;
    trace.research_percent = -50;
    click_cell(&trace, 12, 406);
    CHECK(trace.popup.planet_variable == 21 &&
        trace.popup.planet_value == 32);
    CHECK(checks == 32 && failures == 0);
    printf("EXECUTE_REPORT_CLICK_REPORT_CHAIN_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
