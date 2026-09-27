#include <stdio.h>
#include <string.h>
#include "core.h"

enum {
    FLEET_BYTES = 124,
    PLANET_BYTES = 56,
    POPUP_BYTES = 22
};

typedef struct Trace {
    StarsExecuteReportClickPopup popup;
    StarsI16 popups;
} Trace;

static int checks;
static int failures;

static void check_at(StarsI16 condition, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
#ifndef TARGET_NATIVE
        fprintf(stderr, "GlobalPD check failed at line %d\n", line);
#else
        (void)line;
#endif
    }
}

#define check(condition) check_at((condition), __LINE__)

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static void put32(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    put16(raw, offset, (StarsU16)value);
    put16(raw, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

static StarsU16 get16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1] << 8));
}

static StarsU32 get32(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU32)get16(raw, offset) |
        ((StarsU32)get16(raw, (StarsU16)(offset + 2U)) << 16);
}

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)context;
    (void)hwnd;
    return 1;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)context;
    (void)hwnd;
    (void)dc;
}

static void select_planet(void *context, StarsI16 id)
{
    (void)context;
    (void)id;
}

static void invalidate(void *context, StarsI16 report, StarsI16 full)
{
    (void)context;
    (void)report;
    (void)full;
}

static void popup(void *context, StarsPoint point,
    const StarsExecuteReportClickPopup *value)
{
    Trace *trace = (Trace *)context;
    (void)point;
    trace->popup = *value;
    ++trace->popups;
}

static StarsI16 best_defense(void *context, StarsU16 *part)
{
    (void)context;
    *part = 0x2211;
    return 1;
}

static StarsI16 installation_max(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 unused)
{
    (void)context; (void)planet; (void)unused;
    return 1200;
}

static StarsI16 installation_operable(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 unused)
{
    (void)context; (void)planet; (void)unused;
    return 25;
}

static void estimate_minerals(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI32 values[4])
{
    (void)context; (void)planet;
    values[0] = 6;
    values[1] = values[2] = values[3] = 0;
}

int main(void)
{
    static StarsExecuteReportClickState state;
    static StarsExecuteReportClickOps ops;
    static Trace trace;
    static StarsU8 fleet[FLEET_BYTES];
    static StarsU8 planet[PLANET_BYTES];
    static StarsU8 defense_part[8];
    static StarsU8 fleet_far_table[8];
    static StarsU16 fleet_row_ids[1];
    static StarsU8 shdef_far_table[4];
    static StarsU8 globalpd_raw[POPUP_BYTES];
    static StarsU8 original_starbase[POPUP_BYTES];
    static StarsU8 original_planet[POPUP_BYTES];
    static StarsU8 original_resource[POPUP_BYTES];
    static StarsU8 original_defense[POPUP_BYTES];
    static StarsU8 original_fleet[POPUP_BYTES];
    static StarsU8 original_mineral[POPUP_BYTES];
    static StarsU8 original_mine[POPUP_BYTES];
    static StarsU8 original_factory[POPUP_BYTES];
    static StarsPoint point;
    StarsI16 index;

    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    memset(fleet, 0, sizeof fleet);
    memset(planet, 0, sizeof planet);
    memset(fleet_far_table, 0, sizeof fleet_far_table);
    memset(shdef_far_table, 0, sizeof shdef_far_table);
    memset(globalpd_raw, 0, sizeof globalpd_raw);
    memset(original_starbase, 0, sizeof original_starbase);
    memset(original_planet, 0, sizeof original_planet);
    checks = failures = 0;
    for (index = 0; index < 8; ++index)
        defense_part[index] = (StarsU8)(0xa0 + index);
    put16(fleet, 0, 40);
    put16(fleet, 6, 0xffffU);
    put16(planet, 0, 7);
    put16(planet, 4, (StarsU16)(1U << 9));
    put16(planet, 44, 3U);
    put32(planet, 24, 1UL);
    point.x = 60;
    point.y = 20;
    state.report_window = 3;
    state.transfer_dialog = -1;
    state.field_width[9] = 30;
    state.field_width[10] = 30;
    state.field_width[11] = 30;
    ops.get_dc = get_dc;
    ops.release_dc = release_dc;
    ops.packed_fleet_record = fleet;
    ops.packed_fleet_size = FLEET_BYTES;
    ops.packed_planet_record = planet;
    ops.packed_planet_size = PLANET_BYTES;
    ops.packed_resources = 50;
    ops.packed_research_percent = 20;
    ops.globalpd_fleet_far = 0x12345678UL;
    ops.globalpd_starbase_fleet_far = 0x89abcdefUL;
    ops.globalpd_starbase_part = 0x1122;
    ops.globalpd_defense_part = defense_part;
    ops.globalpd_defense_part_size = 8;
    ops.globalpd_resource_part = 0x3344;
    ops.select_planet = select_planet;
    ops.invalidate_report = invalidate;
    ops.popup = popup;
    ops.best_defense = best_defense;
    ops.context = &trace;

    stars_execute_report_click(&state, point, 1, 8, 0, &ops);
    check(trace.popup.kind == 3);
    check(get16(trace.popup.raw, 0) == 3);
    check(get32(trace.popup.raw, 2) == 0x12345678UL);
    check(get16(trace.popup.raw, 6) == 1);
    check(get16(trace.popup.raw, 10) == 0xff);

    stars_execute_report_click(&state, point, 0, 1, 0, &ops);
    check(trace.popup.kind == 11);
    check(get32(trace.popup.raw, 2) == 0x89abcdefUL);
    check(get16(trace.popup.raw, 6) == 1);
    check(get16(trace.popup.raw, 8) == 0);
    check(get16(trace.popup.raw, 10) == 0);

    stars_execute_report_click(&state, point, 0, 8, 0, &ops);
    check(trace.popup.kind == 9);
    check(trace.popup.part == 0x2211);
    check(memcmp(trace.popup.raw + 2, defense_part, 8) == 0);
    check(trace.popup.rgi[0] == (StarsI32)0xa3a2a1a0L);
    check(trace.popup.rgi[1] == (StarsI32)0xa7a6a5a4L);

    stars_execute_report_click(&state, point, 0, 12, 0, &ops);
    check(trace.popup.kind == 12);
    check(get16(trace.popup.raw, 2) == 7);
    check(get16(trace.popup.raw, 4) == 50);
    check(get16(trace.popup.raw, 6) == 40);
    check(trace.popup.resource_part == 50U);
    check(trace.popup.rgi[0] == (StarsI32)0x00320007L);
    check(trace.popups == 4);

    fleet_row_ids[0] = 1U;
    put32(fleet_far_table, 0, 0x11112222UL);
    put32(fleet_far_table, 4, 0x56781234UL);
    ops.packed_fleet_row_ids = fleet_row_ids;
    ops.packed_fleet_row_count = 1U;
    ops.packed_fleet_far_table = fleet_far_table;
    ops.packed_fleet_far_table_size = sizeof fleet_far_table;
    stars_execute_report_click(&state, point, 1, 8, 0, &ops);
    check(trace.popup.kind == 3);
    check(get32(trace.popup.raw, 2) == 0x56781234UL);
    check(trace.popups == 5);
    fleet_row_ids[0] = 2U;
    stars_execute_report_click(&state, point, 1, 8, 0, &ops);
    check(trace.popups == 5);

    put16(shdef_far_table, 0, 0xff00U);
    put16(shdef_far_table, 2, 0x2345U);
    ops.packed_shdef_far_table = shdef_far_table;
    ops.packed_shdef_far_table_size = sizeof shdef_far_table;
    stars_execute_report_click(&state, point, 0, 1, 0, &ops);
    check(trace.popup.kind == 11);
    check(get32(trace.popup.raw, 2) == 0x234500b9UL);
    check(trace.popups == 6);
    ops.packed_shdef_far_table_size = 3U;
    stars_execute_report_click(&state, point, 0, 1, 0, &ops);
    check(trace.popups == 6);

    /* Exact 22-byte original capture: starbase click then planet click in
     * the isolated clean2400 player's Planet Summary Report. */
    put16(planet, 0, 10U);
    put16(planet, 44, 0U);
    put16(shdef_far_table, 0, 0x0012U);
    put16(shdef_far_table, 2, 0x19e7U);
    ops.packed_shdef_far_table_size = sizeof shdef_far_table;
    ops.globalpd_raw = globalpd_raw;
    ops.globalpd_raw_size = sizeof globalpd_raw;
    put16(original_starbase, 0, 11U);
    put32(original_starbase, 2, 0x19e70012UL);
    put16(original_starbase, 6, 1U);
    memcpy(original_planet, original_starbase, sizeof original_planet);
    put16(original_planet, 0, 7U);
    put16(original_planet, 2, 10U);
    stars_execute_report_click(&state, point, 0, 1, 0, &ops);
    check(memcmp(trace.popup.raw, original_starbase, POPUP_BYTES) == 0);
    check(memcmp(globalpd_raw, original_starbase, POPUP_BYTES) == 0);
    stars_execute_report_click(&state, point, 0, 2, 0, &ops);
    check(memcmp(trace.popup.raw, original_planet, POPUP_BYTES) == 0);
    check(memcmp(globalpd_raw, original_planet, POPUP_BYTES) == 0);
    check(trace.popups == 8);

    /* Independent live clicks from the same isolated original game.  Reset
     * the retained union between report contexts, which the caller owns. */
    ops.packed_resources = 35;
    ops.packed_research_percent = 15;
    put16(original_resource, 0, 12U);
    put16(original_resource, 2, 10U);
    put16(original_resource, 4, 35U);
    put16(original_resource, 6, 30U);
    stars_execute_report_click(&state, point, 0, 12, 0, &ops);
    check(memcmp(trace.popup.raw, original_resource, POPUP_BYTES) == 0);
    check(memcmp(globalpd_raw, original_resource, POPUP_BYTES) == 0);

    defense_part[0] = 0x00U; defense_part[1] = 0x80U;
    defense_part[2] = 0x09U; defense_part[3] = 0x13U;
    defense_part[4] = 0x9eU; defense_part[5] = 0x18U;
    defense_part[6] = 0x37U; defense_part[7] = 0x11U;
    put16(original_defense, 0, 9U);
    memcpy(original_defense + 2, defense_part, 8);
    stars_execute_report_click(&state, point, 0, 8, 0, &ops);
    check(memcmp(trace.popup.raw, original_defense, POPUP_BYTES) == 0);
    check(memcmp(globalpd_raw, original_defense, POPUP_BYTES) == 0);

    memset(globalpd_raw, 0, sizeof globalpd_raw);
    fleet_row_ids[0] = 1U;
    put32(fleet_far_table, 4, 0x199f018cUL);
    put16(original_fleet, 0, 3U);
    put32(original_fleet, 2, 0x199f018cUL);
    put16(original_fleet, 6, 1U);
    put16(original_fleet, 10, 255U);
    stars_execute_report_click(&state, point, 1, 8, 0, &ops);
    check(memcmp(trace.popup.raw, original_fleet, POPUP_BYTES) == 0);
    check(memcmp(globalpd_raw, original_fleet, POPUP_BYTES) == 0);
    check(trace.popups == 11);

    /* The live mineral -> mine -> factory sequence also proves bytes that
     * installation popups leave untouched in the shared original union. */
    memset(globalpd_raw, 0, sizeof globalpd_raw);
    put16(planet, 4, (StarsU16)((1U << 9) | (1U << 10)));
    put32(planet, 20, 0x00a00a00UL);
    planet[9] = 68U;
    put16(planet, 28, 469U);
    put16(planet, 30, 0U);
    ops.max_mines = installation_max;
    ops.max_factories = installation_max;
    ops.max_operable_mines = installation_operable;
    ops.max_operable_factories = installation_operable;
    ops.estimate_minerals = estimate_minerals;
    point.x = 2;
    put16(original_mineral, 0, 1U);
    put16(original_mineral, 6, 1U);
    put16(original_mineral, 10, 469U);
    put16(original_mineral, 14, 68U);
    put16(original_mineral, 18, 6U);
    stars_execute_report_click(&state, point, 0, 9, 0, &ops);
    check(memcmp(trace.popup.raw, original_mineral, POPUP_BYTES) == 0);
    check(memcmp(globalpd_raw, original_mineral, POPUP_BYTES) == 0);
    memcpy(original_mine, original_mineral, POPUP_BYTES);
    put16(original_mine, 0, 8U);
    put16(original_mine, 2, 10U);
    put16(original_mine, 4, 1200U);
    put16(original_mine, 6, 10U);
    put16(original_mine, 8, 25U);
    put16(original_mine, 10, 0U);
    stars_execute_report_click(&state, point, 0, 6, 0, &ops);
    check(memcmp(trace.popup.raw, original_mine, POPUP_BYTES) == 0);
    check(memcmp(globalpd_raw, original_mine, POPUP_BYTES) == 0);
    memcpy(original_factory, original_mine, POPUP_BYTES);
    put16(original_factory, 10, 1U);
    stars_execute_report_click(&state, point, 0, 7, 0, &ops);
    check(memcmp(trace.popup.raw, original_factory, POPUP_BYTES) == 0);
    check(memcmp(globalpd_raw, original_factory, POPUP_BYTES) == 0);
    check(trace.popups == 14);
#ifndef TARGET_NATIVE
    printf("EXECUTE_REPORT_CLICK_GLOBALPD_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
