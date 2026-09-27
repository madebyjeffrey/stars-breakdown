#include <stdio.h>
#include <string.h>
#include "core.h"

enum {
    FLEET_BYTES = 124,
    PLANET_BYTES = 56,
    BATTLE_BYTES = 14,
    POPUP_BYTES = 22
};

typedef struct Trace {
    StarsI16 gets;
    StarsI16 releases;
    StarsI16 fleet_selects;
    StarsI16 planet_selects;
    StarsI16 invalidates;
    StarsI16 messages;
    StarsI16 waypoints;
    StarsI16 transfers;
    StarsI16 popups;
    StarsI16 beeps;
    StarsI16 scans;
    StarsI16 ensured;
    StarsI16 vcrs;
    StarsI16 centers;
    StarsI16 production;
    StarsI16 maxes;
    StarsI16 last_transfer_planet;
    StarsExecuteReportClickPopup last_popup;
    StarsU8 popup_raw[POPUP_BYTES];
    StarsU8 fleet_raw[FLEET_BYTES];
    StarsU8 planet_raw[PLANET_BYTES];
    StarsU8 battle_raw[BATTLE_BYTES];
} Trace;

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

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)hwnd;
    ++((Trace *)context)->gets;
    return 4;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)hwnd;
    (void)dc;
    ++((Trace *)context)->releases;
}

static void select_fleet(void *context, StarsI16 id)
{
    (void)id;
    ++((Trace *)context)->fleet_selects;
}

static void select_planet(void *context, StarsI16 id)
{
    (void)id;
    ++((Trace *)context)->planet_selects;
}

static void invalidate(void *context, StarsI16 report, StarsI16 full)
{
    (void)report;
    (void)full;
    ++((Trace *)context)->invalidates;
}

static StarsI16 dest(void *context, const StarsExecuteReportClickFleet *fleet)
{
    (void)context;
    (void)fleet;
    return 0;
}

static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    (void)hwnd;
    (void)message;
    (void)wparam;
    (void)lparam;
    ++((Trace *)context)->messages;
}

static void waypoint(void *context, StarsI16 enabled)
{
    (void)enabled;
    ++((Trace *)context)->waypoints;
}

static void transfer(void *context, StarsI16 fleet, StarsI16 mode,
    StarsI16 planet, StarsI16 kind, StarsI16 flags)
{
    (void)fleet;
    (void)mode;
    (void)kind;
    (void)flags;
    ++((Trace *)context)->transfers;
    ((Trace *)context)->last_transfer_planet = planet;
}

static void store_popup_raw(Trace *trace,
    const StarsExecuteReportClickPopup *popup)
{
    memset(trace->popup_raw, 0, sizeof trace->popup_raw);
    put16(trace->popup_raw, 0, (StarsU16)popup->kind);
    if (popup->kind == 8) {
        put16(trace->popup_raw, 2, (StarsU16)popup->planet_id);
        put16(trace->popup_raw, 4, (StarsU16)popup->planet_maximum);
        put16(trace->popup_raw, 6, (StarsU16)popup->planet_value);
        put16(trace->popup_raw, 8, (StarsU16)popup->planet_minimum);
        put16(trace->popup_raw, 10, (StarsU16)popup->planet_variable);
    } else if (popup->kind == 1) {
        put16(trace->popup_raw, 2, (StarsU16)popup->mineral_index);
        put16(trace->popup_raw, 4, (StarsU16)popup->planet_variable);
        put16(trace->popup_raw, 6, (StarsU16)popup->planet_value);
        put16(trace->popup_raw, 10, (StarsU16)popup->planet_maximum);
        put16(trace->popup_raw, 12, (StarsU16)popup->player_value);
        put16(trace->popup_raw, 14, (StarsU16)popup->planet_minimum);
        put16(trace->popup_raw, 18, (StarsU16)popup->mineral_value);
    } else {
        put16(trace->popup_raw, 2, (StarsU16)popup->planet_id);
        put16(trace->popup_raw, 4, (StarsU16)popup->planet_variable);
        put16(trace->popup_raw, 6, (StarsU16)popup->planet_value);
        put16(trace->popup_raw, 8, (StarsU16)popup->planet_minimum);
        put16(trace->popup_raw, 10, (StarsU16)popup->planet_maximum);
        put16(trace->popup_raw, 12, (StarsU16)popup->player_value);
        put16(trace->popup_raw, 14, (StarsU16)popup->player_minimum);
        put16(trace->popup_raw, 16, (StarsU16)popup->player_maximum);
    }
}

static void popup(void *context, StarsPoint point,
    const StarsExecuteReportClickPopup *value)
{
    Trace *trace = (Trace *)context;
    (void)point;
    ++trace->popups;
    trace->last_popup = *value;
    store_popup_raw(trace, value);
}

static void beep(void *context)
{
    ++((Trace *)context)->beeps;
}

static StarsI16 nearest(void *context, StarsPoint point, StarsI16 kind,
    StarsI16 *id)
{
    (void)point;
    (void)kind;
    ++((Trace *)context)->scans;
    *id = 44;
    return 1;
}

static void change_scan(void *context, StarsPoint point, StarsI16 kind,
    StarsI16 id)
{
    (void)point;
    (void)kind;
    (void)id;
    ++((Trace *)context)->scans;
}

static void ensure_point(void *context, StarsPoint point)
{
    (void)point;
    ++((Trace *)context)->ensured;
}

static void battle_vcr(void *context, StarsU16 id)
{
    (void)id;
    ++((Trace *)context)->vcrs;
}

static void center_scan(void *context, StarsPoint point)
{
    (void)point;
    ++((Trace *)context)->centers;
}

static void change_production(void *context)
{
    ++((Trace *)context)->production;
}

static StarsI16 max_value(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 which)
{
    (void)planet;
    (void)which;
    ++((Trace *)context)->maxes;
    return 9;
}

static StarsI16 best_defense(void *context, StarsU16 *part)
{
    (void)context;
    *part = 6;
    return 1;
}

static void estimate(void *context, const StarsExecuteReportClickPlanet *planet,
    StarsI32 *values)
{
    (void)context;
    (void)planet;
    values[0] = 11;
    values[1] = 22;
    values[2] = 77;
    values[3] = 44;
}

int main(void)
{
    static StarsExecuteReportClickState state;
    static StarsExecuteReportClickOps ops;
    static Trace trace;
    static StarsPoint point;
    static StarsU16 planet_flags;

    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    trace.last_transfer_planet = -99;
    put16(trace.fleet_raw, 0, 40);
    put16(trace.fleet_raw, 6, 0xffffU);
    put16(trace.fleet_raw, 8, 10);
    put16(trace.fleet_raw, 10, 20);
    put16(trace.fleet_raw, 4, 0x2400U);
    put16(trace.battle_raw, 0, 2);
    put16(trace.battle_raw, 10, 30);
    put16(trace.battle_raw, 12, 40);
    put16(trace.planet_raw, 0, 7);
    planet_flags = (StarsU16)((1U << 9) | (1U << 10));
    put16(trace.planet_raw, 4, planet_flags);
    trace.planet_raw[9] = 80;
    trace.planet_raw[10] = 70;
    trace.planet_raw[11] = 60;
    put32(trace.planet_raw, 20, (5UL << 20) | (3UL << 8));
    put32(trace.planet_raw, 24, 1UL);
    put16(trace.planet_raw, 28, 12);
    put16(trace.planet_raw, 30, 8);
    put16(trace.planet_raw, 36, 33);
    put16(trace.planet_raw, 38, 13);
    point.x = 10;
    point.y = 20;
    state.report_window = 3;
    state.ship_list_window = 4;
    state.transfer_dialog = -1;
    state.enemy_fleet_report = 1;
    state.enemy_fleet_rows = 3;
    state.field_width[0] = 30;
    state.field_width[9] = 30;
    state.field_width[10] = 30;
    state.field_width[11] = 30;
    ops.get_dc = get_dc;
    ops.release_dc = release_dc;
    ops.packed_fleet_record = trace.fleet_raw;
    ops.packed_fleet_size = FLEET_BYTES;
    ops.packed_planet_record = trace.planet_raw;
    ops.packed_planet_size = PLANET_BYTES;
    ops.packed_battle_record = trace.battle_raw;
    ops.packed_battle_size = BATTLE_BYTES;
    ops.packed_starbase_hull = 3;
    ops.packed_resources = 50;
    ops.packed_research_percent = 20;
    ops.select_fleet = select_fleet;
    ops.select_planet = select_planet;
    ops.invalidate_report = invalidate;
    ops.dest_is_wp0 = dest;
    ops.send_message = send_message;
    ops.set_scan_waypoint = waypoint;
    ops.transfer = transfer;
    ops.popup = popup;
    ops.message_beep = beep;
    ops.find_nearest = nearest;
    ops.change_scan_selection = change_scan;
    ops.ensure_point_on_screen = ensure_point;
    ops.battle_vcr = battle_vcr;
    ops.center_scan = center_scan;
    ops.change_production = change_production;
    ops.max_mines = max_value;
    ops.max_operable_mines = max_value;
    ops.max_factories = max_value;
    ops.max_operable_factories = max_value;
    ops.best_defense = best_defense;
    ops.estimate_minerals = estimate;
    ops.context = &trace;

    if (get16(trace.fleet_raw, 0) != 40 ||
            get16(trace.fleet_raw, 6) != 0xffffU ||
            get16(trace.fleet_raw, 8) != 10 ||
            get16(trace.fleet_raw, 10) != 20)
        return 1;
    stars_execute_report_click(&state, point, 1, 8, 0, &ops);
    if (trace.last_popup.kind != 3 || get16(trace.popup_raw, 0) != 3 ||
            get16(trace.popup_raw, 2) != 40 ||
            get16(trace.popup_raw, 4) != 1 ||
            get16(trace.popup_raw, 10) != 255)
        return 2;
    stars_execute_report_click(&state, point, 1, 6, 0, &ops);
    if (trace.transfers != 1 || trace.last_transfer_planet != -1)
        return 3;
    stars_execute_report_click(&state, point, 0, 6, 0, &ops);
    if (trace.last_popup.kind != 8 || get16(trace.popup_raw, 2) != 7 ||
            get16(trace.popup_raw, 4) != 9 ||
            get16(trace.popup_raw, 6) != 3 ||
            get16(trace.popup_raw, 8) != 9 ||
            get16(trace.popup_raw, 10) != 0)
        return 4;
    stars_execute_report_click(&state, point, 0, 7, 0, &ops);
    if (trace.last_popup.kind != 8 || get16(trace.popup_raw, 6) != 5 ||
            get16(trace.popup_raw, 10) != 1)
        return 5;
    point.x = 50;
    stars_execute_report_click(&state, point, 0, 9, 0, &ops);
    if (trace.last_popup.kind != 1 || get16(trace.popup_raw, 2) != 2 ||
            get16(trace.popup_raw, 6) != 1 ||
            get16(trace.popup_raw, 10) != 33 ||
            get16(trace.popup_raw, 12) != 13 ||
            get16(trace.popup_raw, 14) != 60 ||
            get16(trace.popup_raw, 18) != 77)
        return 6;
    stars_execute_report_click(&state, point, 0, 12, 0, &ops);
    if (trace.last_popup.kind != 12 || get16(trace.popup_raw, 2) != 7 ||
            get16(trace.popup_raw, 4) != 50 ||
            get16(trace.popup_raw, 6) != 40)
        return 7;
    stars_execute_report_click(&state, point, 0, 1, 0, &ops);
    if (trace.last_popup.kind != 11 || get16(trace.popup_raw, 2) != 7 ||
            trace.last_popup.starbase_hull != 3)
        return 10;
    point.x = 60;
    stars_execute_report_click(&state, point, 0, 0, 0, &ops);
    if (trace.last_popup.kind != 11 || trace.last_popup.starbase_hull != 3)
        return 11;
    stars_execute_report_click(&state, point, 0, 2, 0, &ops);
    if (trace.last_popup.kind != 7 || get16(trace.popup_raw, 2) != 7)
        return 12;
    stars_execute_report_click(&state, point, 0, 5, 0, &ops);
    if (trace.production != 1)
        return 13;
    stars_execute_report_click(&state, point, 0, 8, 0, &ops);
    if (trace.last_popup.kind != 9 || trace.last_popup.part != 6)
        return 14;
    put32(trace.planet_raw, 24, 0x00800000UL | 1UL);
    stars_execute_report_click(&state, point, 0, 12, 0, &ops);
    if (trace.last_popup.kind != 12 || trace.last_popup.planet_value != 50)
        return 15;
    put32(trace.planet_raw, 24, 1UL);
    state.production_window = 9;
    {
        StarsI16 gets_before = trace.gets;
        StarsI16 beeps_before = trace.beeps;
        stars_execute_report_click(&state, point, 0, 5, 0, &ops);
        if (trace.gets != (StarsI16)(gets_before + 1) ||
                trace.beeps != (StarsI16)(beeps_before + 1))
            return 16;
    }
    state.production_window = 0;
    state.transfer_dialog = 9;
    {
        StarsI16 transfers_before = trace.transfers;
        StarsI16 beeps_before = trace.beeps;
        stars_execute_report_click(&state, point, 1, 6, 0, &ops);
        if (trace.transfers != transfers_before || trace.beeps != (StarsI16)(beeps_before + 1))
            return 17;
    }
    state.transfer_dialog = -1;
    stars_execute_report_click(&state, point, 2, 0, -1, &ops);
    stars_execute_report_click(&state, point, 3, 0, 0, &ops);
    stars_execute_report_click(&state, point, 3, 0, 0, &ops);
    if (trace.scans < 3 || trace.ensured != 1 || trace.centers != 1 ||
            trace.vcrs != 1)
        return 8;
    if (trace.gets != 17 || trace.releases != 17 || trace.planet_selects != 10)
        return 9;
    puts("EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS");
    return 0;
}
