#include <stdio.h>
#include <string.h>
#include "core.h"

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
    StarsI16 last_popup_kind;
    StarsI16 last_popup_planet;
    StarsI16 last_popup_index;
    StarsI16 last_popup_variable;
    StarsI16 last_popup_value;
    StarsI16 last_popup_maximum;
    StarsI16 last_transfer_planet;
} Trace;

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)context)->gets; return 4;
}
static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)hwnd; (void)dc; ++((Trace *)context)->releases;
}
static StarsI16 get_fleet(void *context, StarsI16 row, StarsI16 kind,
    StarsExecuteReportClickFleet *fleet)
{
    (void)context; (void)kind; fleet->id = (StarsI16)(40 + row);
    fleet->planet_id = -1; fleet->point.x = 10; fleet->point.y = 20;
    fleet->flags = 0; return 1;
}
static StarsI16 get_planet(void *context, StarsI16 row,
    StarsExecuteReportClickPlanet *planet)
{
    (void)context; (void)row; memset(planet, 0, sizeof *planet);
    planet->id = 7; planet->has_starbase = 1; planet->starbase_hull = 3;
    planet->defense_count = 1;
    planet->mineral_weight = (StarsU32)((5UL << 20) | (3UL << 8));
    planet->mineral_value[0] = 12; planet->player_value[0] = 8;
    planet->mineral_concentration[0] = 80; planet->resources = 50;
    planet->research_percent = 20;
    return 1;
}
static StarsI16 get_battle(void *context, StarsI16 row,
    StarsExecuteReportClickBattle *battle)
{
    (void)context; battle->id = (StarsU16)(row + 2); battle->point.x = 30;
    battle->point.y = 40; return 1;
}
static void select_fleet(void *context, StarsI16 id)
{
    (void)id; ++((Trace *)context)->fleet_selects;
}
static void select_planet(void *context, StarsI16 id)
{
    (void)id; ++((Trace *)context)->planet_selects;
}
static void invalidate(void *context, StarsI16 report, StarsI16 full)
{
    (void)report; (void)full; ++((Trace *)context)->invalidates;
}
static StarsI16 dest(void *context, const StarsExecuteReportClickFleet *fleet)
{
    (void)context; (void)fleet; return 0;
}
static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    (void)hwnd; (void)message; (void)wparam; (void)lparam;
    ++((Trace *)context)->messages;
}
static void waypoint(void *context, StarsI16 enabled)
{
    (void)enabled; ++((Trace *)context)->waypoints;
}
static void transfer(void *context, StarsI16 fleet, StarsI16 mode,
    StarsI16 planet, StarsI16 kind, StarsI16 flags)
{
    (void)fleet; (void)mode; (void)kind; (void)flags;
    ++((Trace *)context)->transfers; ((Trace *)context)->last_transfer_planet = planet;
}
static void popup(void *context, StarsPoint point,
    const StarsExecuteReportClickPopup *value)
{
    Trace *trace = (Trace *)context;
    (void)point; ++trace->popups; trace->last_popup_kind = value->kind;
    trace->last_popup_planet = value->planet_id;
    trace->last_popup_index = value->mineral_index;
    trace->last_popup_variable = value->planet_variable;
    trace->last_popup_value = value->planet_value;
    trace->last_popup_maximum = value->planet_maximum;
}
static void beep(void *context)
{
    ++((Trace *)context)->beeps;
}
static StarsI16 nearest(void *context, StarsPoint point, StarsI16 kind,
    StarsI16 *id)
{
    (void)point; (void)kind; ++((Trace *)context)->scans; *id = 44; return 1;
}
static void change_scan(void *context, StarsPoint point, StarsI16 kind,
    StarsI16 id)
{
    (void)point; (void)kind; (void)id; ++((Trace *)context)->scans;
}
static void ensure_point(void *context, StarsPoint point)
{
    (void)point; ++((Trace *)context)->ensured;
}
static void battle_vcr(void *context, StarsU16 id)
{
    (void)id; ++((Trace *)context)->vcrs;
}
static void center_scan(void *context, StarsPoint point)
{
    (void)point; ++((Trace *)context)->centers;
}
static void change_production(void *context)
{
    ++((Trace *)context)->production;
}
static StarsI16 max_value(void *context,
    const StarsExecuteReportClickPlanet *planet, StarsI16 which)
{
    (void)planet; (void)which; ++((Trace *)context)->maxes; return 9;
}
static StarsI16 best_defense(void *context, StarsU16 *part)
{
    *part = 6; (void)context; return 1;
}
static void estimate(void *context, const StarsExecuteReportClickPlanet *planet,
    StarsI32 *values)
{
    (void)context; (void)planet; values[0] = 77; values[1] = 0;
    values[2] = 0; values[3] = 0;
}

int main(void)
{
    StarsExecuteReportClickState state;
    StarsExecuteReportClickOps ops;
    Trace trace;
    StarsPoint point;

    memset(&state, 0, sizeof state); memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace); point.x = 10; point.y = 20;
    state.report_window = 3; state.ship_list_window = 4;
    state.production_window = 0; state.vcr_window = 0;
    state.transfer_dialog = -1; state.enemy_fleet_report = 1;
    state.enemy_fleet_rows = 3; state.current_enemy_fleet_row = 0;
    state.field_width[0] = 30; state.field_width[9] = 30;
    state.field_width[10] = 30; state.field_width[11] = 30;
    ops.get_dc = get_dc; ops.release_dc = release_dc;
    ops.get_fleet = get_fleet; ops.get_planet = get_planet;
    ops.get_battle = get_battle; ops.select_fleet = select_fleet;
    ops.select_planet = select_planet; ops.invalidate_report = invalidate;
    ops.dest_is_wp0 = dest; ops.send_message = send_message;
    ops.set_scan_waypoint = waypoint; ops.transfer = transfer;
    ops.popup = popup; ops.message_beep = beep; ops.find_nearest = nearest;
    ops.change_scan_selection = change_scan; ops.ensure_point_on_screen = ensure_point;
    ops.battle_vcr = battle_vcr; ops.center_scan = center_scan;
    ops.change_production = change_production; ops.max_mines = max_value;
    ops.max_operable_mines = max_value; ops.max_factories = max_value;
    ops.max_operable_factories = max_value; ops.best_defense = best_defense;
    ops.estimate_minerals = estimate; ops.context = &trace;
    stars_execute_report_click(&state, point, 1, 3, 0, &ops);
    if (trace.fleet_selects != 1 || trace.messages != 1 || trace.waypoints != 1)
        return 1;
    stars_execute_report_click(&state, point, 1, 6, 0, &ops);
    if (trace.transfers != 1 || trace.last_transfer_planet != -1)
        return 2;
    stars_execute_report_click(&state, point, 1, 8, 0, &ops);
    if (trace.popups != 1 || trace.last_popup_kind != 3 ||
            trace.last_popup_variable != 1 || trace.last_popup_maximum != 255)
        return 3;
    stars_execute_report_click(&state, point, 2, 0, -1, &ops);
    if (trace.scans < 2 || trace.ensured != 1)
        return 4;
    stars_execute_report_click(&state, point, 3, 0, 0, &ops);
    if (trace.centers != 1) return 5;
    stars_execute_report_click(&state, point, 3, 0, 0, &ops);
    if (trace.vcrs != 1) return 6;
    stars_execute_report_click(&state, point, 0, 5, 0, &ops);
    if (trace.planet_selects != 1 || trace.production != 1) return 7;
    stars_execute_report_click(&state, point, 0, 6, 0, &ops);
    if (trace.popups != 2 || trace.last_popup_kind != 8 ||
            trace.last_popup_value != 3) return 8;
    stars_execute_report_click(&state, point, 0, 7, 0, &ops);
    if (trace.popups != 3 || trace.last_popup_kind != 8 ||
            trace.last_popup_value != 5) return 9;
    point.x = 50;
    stars_execute_report_click(&state, point, 0, 9, 0, &ops);
    if (trace.popups != 4 || trace.last_popup_kind != 1 ||
            trace.last_popup_index != 2) return 10;
    stars_execute_report_click(&state, point, 0, 12, 0, &ops);
    if (trace.popups != 5 || trace.last_popup_kind != 12 ||
            trace.last_popup_value != 40) return 11;
    if (trace.gets != 11 || trace.releases != 11) return 12;
    puts("EXECUTE_REPORT_CLICK_CHECKS=21 FAILURES=0 RESULT=PASS");
    return 0;
}
