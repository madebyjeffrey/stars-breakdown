#include "core.h"

enum {
    REPORT_CLICK_FLEET = 1,
    REPORT_CLICK_ENEMY_FLEET = 2,
    REPORT_CLICK_BATTLE = 3,
    REPORT_CLICK_POPUP_FLEET = 3,
    REPORT_CLICK_POPUP_STARBASE = 11,
    REPORT_CLICK_POPUP_PLANET = 7,
    REPORT_CLICK_POPUP_INSTALLATION = 8,
    REPORT_CLICK_POPUP_DEFENSE = 9,
    REPORT_CLICK_POPUP_MINERAL = 1,
    REPORT_CLICK_POPUP_RESOURCES = 12
};

#define REPORT_CLICK_RESOURCE_EXEMPT 0x8000U
#define REPORT_CLICK_FLEET_BYTES 124
#define REPORT_CLICK_PLANET_BYTES 56
#define REPORT_CLICK_BATTLE_BYTES 14

static StarsU16 execute_report_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static StarsU32 execute_report_packed_get32(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU32)execute_report_packed_get16(record, offset) |
        ((StarsU32)execute_report_packed_get16(record,
            (StarsU16)(offset + 2)) << 16);
}

/* Win16 GDI MulDiv(a, b, 100): 32-bit intermediate, nearest-integer
 * rounding (halves away from zero), and signed-16 saturation.  Plain C
 * multiplication can wrap before division when int is 16 bits. */
static StarsI16 execute_report_research_share(StarsI16 resources,
    StarsI16 percent)
{
    StarsI32 product;
    StarsI32 magnitude;
    StarsI32 rounded;

    product = (StarsI32)resources * (StarsI32)percent;
    magnitude = product < 0L ? -product : product;
    rounded = (magnitude + 50L) / 100L;
    if (product < 0L) {
        if (rounded > 32768L)
            return (StarsI16)-32768;
        return (StarsI16)-rounded;
    }
    if (rounded > 32767L)
        return (StarsI16)32767;
    return (StarsI16)rounded;
}

static void execute_report_popup_put16(StarsU8 *record, StarsU16 offset,
    StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1] = (StarsU8)(value >> 8);
}

static void execute_report_popup_put32(StarsU8 *record, StarsU16 offset,
    StarsU32 value)
{
    execute_report_popup_put16(record, offset, (StarsU16)value);
    execute_report_popup_put16(record, (StarsU16)(offset + 2U),
        (StarsU16)(value >> 16));
}

static StarsI16 execute_report_get_fleet(const StarsExecuteReportClickOps *ops,
    StarsI16 row, StarsI16 kind, StarsExecuteReportClickFleet *fleet)
{
    const StarsU8 *record;

    record = ops->packed_fleet_record;
    if (record) {
        if (ops->packed_fleet_size < REPORT_CLICK_FLEET_BYTES)
            return 0;
        fleet->id = (StarsI16)execute_report_packed_get16(record, 0);
        fleet->planet_id = (StarsI16)execute_report_packed_get16(record, 6);
        fleet->point.x = (StarsI16)execute_report_packed_get16(record, 8);
        fleet->point.y = (StarsI16)execute_report_packed_get16(record, 10);
        fleet->flags = execute_report_packed_get16(record, 4);
        return 1;
    }
    return ops->get_fleet && ops->get_fleet(ops->context, row, kind,
        fleet);
}

static StarsI16 execute_report_get_planet(
    const StarsExecuteReportClickOps *ops, StarsI16 row,
    StarsExecuteReportClickPlanet *planet)
{
    const StarsU8 *record;
    StarsI16 index;
    StarsU32 services;

    record = ops->packed_planet_record;
    if (record) {
        if (ops->packed_planet_size < REPORT_CLICK_PLANET_BYTES)
            return 0;
        planet->id = (StarsI16)execute_report_packed_get16(record, 0);
        services = execute_report_packed_get32(record, 24);
        planet->raw_status_flags = execute_report_packed_get16(record, 4);
        planet->has_starbase = (StarsU16)((execute_report_packed_get16(
            record, 4) >> 9) & 1U);
        planet->defense_count = (StarsU16)(services & 0x0fffUL);
        planet->flags = (services & 0x00800000UL) != 0UL ?
            REPORT_CLICK_RESOURCE_EXEMPT : 0;
        for (index = 0; index < 3; ++index)
            planet->mineral_concentration[index] = (StarsI16)record[9 + index];
        for (index = 0; index < 4; ++index) {
            planet->mineral_value[index] = (StarsI16)
                execute_report_packed_get16(record,
                    (StarsU16)(28 + index * 4));
            planet->player_value[index] = (StarsI16)
                execute_report_packed_get16(record,
                    (StarsU16)(30 + index * 4));
        }
        planet->mineral_weight = execute_report_packed_get32(record, 20);
        planet->starbase_hull = ops->packed_starbase_hull;
        planet->starbase_design_index = (StarsU16)
            (execute_report_packed_get16(record, 44) & 0x000fU);
        planet->resources = ops->packed_resources;
        planet->research_percent = ops->packed_research_percent;
        return 1;
    }
    return ops->get_planet && ops->get_planet(ops->context, row, planet);
}

static StarsI16 execute_report_get_battle(
    const StarsExecuteReportClickOps *ops, StarsI16 row,
    StarsExecuteReportClickBattle *battle)
{
    const StarsU8 *record;

    record = ops->packed_battle_record;
    if (record) {
        if (ops->packed_battle_size < REPORT_CLICK_BATTLE_BYTES)
            return 0;
        battle->id = execute_report_packed_get16(record, 0);
        battle->point.x = (StarsI16)execute_report_packed_get16(record, 10);
        battle->point.y = (StarsI16)execute_report_packed_get16(record, 12);
        return 1;
    }
    return ops->get_battle && ops->get_battle(ops->context, row, battle);
}

static void popup_clear(const StarsExecuteReportClickOps *ops,
    StarsExecuteReportClickPopup *popup)
{
    StarsI16 index;

    for (index = 0; index < STARS_EXECUTE_REPORT_CLICK_POPUP_SIZE; ++index)
        popup->raw[index] = ops->globalpd_raw &&
            ops->globalpd_raw_size >= STARS_EXECUTE_REPORT_CLICK_POPUP_SIZE ?
            ops->globalpd_raw[index] : 0;
    for (index = 0; index < 5; ++index)
        popup->rgi[index] = 0;
    for (index = 0; index < 8; ++index)
        popup->part_bytes[index] = 0;
    popup->fleet_far = 0;
    popup->starbase_fleet_far = 0;
    popup->resource_part = 0;
    popup->kind = 0;
    popup->planet_id = -1;
    popup->planet_variable = 0;
    popup->planet_value = 0;
    popup->planet_minimum = 0;
    popup->planet_maximum = 0;
    popup->player_value = 0;
    popup->player_minimum = 0;
    popup->player_maximum = 0;
    popup->mineral_index = -1;
    popup->mineral_value = 0;
    popup->part = 0;
    popup->starbase_hull = 0;
}

/* Materialize the original 22-byte GlobalPD overlay after the typed branch
 * has selected its semantic values.  Pointer words are supplied by the
 * caller because the host reconstruction cannot infer Win16 far addresses. */
static void execute_report_popup_prepare(
    const StarsExecuteReportClickOps *ops,
    StarsExecuteReportClickPopup *popup)
{
    StarsI16 index;

    execute_report_popup_put16(popup->raw, 0, (StarsU16)popup->kind);
    if (popup->kind == REPORT_CLICK_POPUP_FLEET) {
        execute_report_popup_put32(popup->raw, 2, popup->fleet_far);
        execute_report_popup_put16(popup->raw, 6,
            (StarsU16)popup->planet_variable);
        execute_report_popup_put16(popup->raw, 10,
            (StarsU16)popup->planet_maximum);
    } else if (popup->kind == REPORT_CLICK_POPUP_PLANET) {
        execute_report_popup_put16(popup->raw, 2,
            (StarsU16)popup->planet_id);
    } else if (popup->kind == REPORT_CLICK_POPUP_INSTALLATION) {
        execute_report_popup_put16(popup->raw, 2,
            (StarsU16)popup->planet_id);
        execute_report_popup_put16(popup->raw, 4,
            (StarsU16)popup->planet_maximum);
        execute_report_popup_put16(popup->raw, 6,
            (StarsU16)popup->planet_value);
        execute_report_popup_put16(popup->raw, 8,
            (StarsU16)popup->planet_minimum);
        execute_report_popup_put16(popup->raw, 10,
            (StarsU16)popup->planet_variable);
    } else if (popup->kind == REPORT_CLICK_POPUP_DEFENSE) {
        for (index = 0; index < 8; ++index)
            popup->raw[2 + index] = popup->part_bytes[index];
    } else if (popup->kind == REPORT_CLICK_POPUP_MINERAL) {
        execute_report_popup_put16(popup->raw, 2,
            (StarsU16)popup->mineral_index);
        execute_report_popup_put16(popup->raw, 4,
            (StarsU16)popup->planet_variable);
        execute_report_popup_put16(popup->raw, 6,
            (StarsU16)popup->planet_value);
        execute_report_popup_put16(popup->raw, 10,
            (StarsU16)popup->planet_maximum);
        execute_report_popup_put16(popup->raw, 12,
            (StarsU16)popup->player_value);
        execute_report_popup_put16(popup->raw, 14,
            (StarsU16)popup->planet_minimum);
        execute_report_popup_put16(popup->raw, 18,
            (StarsU16)popup->mineral_value);
    } else if (popup->kind == REPORT_CLICK_POPUP_RESOURCES) {
        execute_report_popup_put16(popup->raw, 2,
            (StarsU16)popup->planet_id);
        execute_report_popup_put16(popup->raw, 4,
            (StarsU16)popup->planet_variable);
        execute_report_popup_put16(popup->raw, 6,
            (StarsU16)popup->planet_value);
    } else if (popup->kind == REPORT_CLICK_POPUP_STARBASE) {
        execute_report_popup_put32(popup->raw, 2,
            popup->starbase_fleet_far);
        execute_report_popup_put16(popup->raw, 6,
            (StarsU16)popup->planet_value);
        execute_report_popup_put16(popup->raw, 8,
            (StarsU16)popup->planet_minimum);
        execute_report_popup_put16(popup->raw, 10,
            (StarsU16)popup->planet_maximum);
    }
    for (index = 0; index < 5; ++index)
        popup->rgi[index] = (StarsI32)execute_report_packed_get32(
            popup->raw, (StarsU16)(2 + index * 4));
    (void)ops;
}

static void show_popup(const StarsExecuteReportClickOps *ops,
    StarsPoint point, StarsExecuteReportClickPopup *popup)
{
    StarsI16 index;

    execute_report_popup_prepare(ops, popup);
    if (ops->globalpd_raw &&
            ops->globalpd_raw_size >= STARS_EXECUTE_REPORT_CLICK_POPUP_SIZE)
        for (index = 0; index < STARS_EXECUTE_REPORT_CLICK_POPUP_SIZE;
                ++index)
            ops->globalpd_raw[index] = popup->raw[index];
    if (ops->popup)
        ops->popup(ops->context, point, popup);
}

static StarsI16 execute_report_fleet_far(
    const StarsExecuteReportClickOps *ops, StarsI16 row,
    StarsU32 *far_pointer)
{
    StarsU16 index;
    StarsU32 offset;

    if (!ops->packed_fleet_row_ids || !ops->packed_fleet_far_table) {
        *far_pointer = ops->globalpd_fleet_far;
        return 1;
    }
    if (row < 0 || (StarsU16)row >= ops->packed_fleet_row_count)
        return 0;
    index = ops->packed_fleet_row_ids[row];
    offset = (StarsU32)index * 4UL;
    if (offset + 4UL > ops->packed_fleet_far_table_size)
        return 0;
    *far_pointer = execute_report_packed_get32(
        ops->packed_fleet_far_table, (StarsU16)offset);
    return 1;
}

static void select_fleet_row(StarsExecuteReportClickState *state,
    const StarsExecuteReportClickOps *ops, StarsPoint point, StarsI16 row,
    StarsI16 column)
{
    StarsExecuteReportClickFleet fleet;
    StarsExecuteReportClickPopup popup;

    if (!execute_report_get_fleet(ops, row, REPORT_CLICK_FLEET, &fleet))
        return;
    if (state->transfer_dialog != -1) {
        if (ops->message_beep)
            ops->message_beep(ops->context);
        return;
    }
    state->selected_fleet_id = fleet.id;
    if (ops->select_fleet)
        ops->select_fleet(ops->context, fleet.id);
    if (ops->invalidate_report)
        ops->invalidate_report(ops->context, 1, 0);
    if (column >= 3 && column <= 5) {
        if (ops->dest_is_wp0 && !ops->dest_is_wp0(ops->context, &fleet)) {
            if (ops->send_message)
                ops->send_message(ops->context, state->ship_list_window,
                    0x407, 1, 0);
            if (ops->set_scan_waypoint)
                ops->set_scan_waypoint(ops->context, 1);
        }
    } else if (column == 6 || column == 7) {
        if (ops->transfer)
            ops->transfer(ops->context, fleet.id, 2, fleet.planet_id,
                fleet.planet_id == -1 ? 4 : 1, 0);
    } else if (column == 8) {
    popup_clear(ops, &popup);
    popup.kind = 3;
    popup.planet_id = fleet.id;
    popup.planet_variable = 1;
    popup.planet_maximum = 0xff;
    if (!execute_report_fleet_far(ops, row, &popup.fleet_far))
        return;
    show_popup(ops, point, &popup);
    }
}

static void select_enemy_fleet_row(StarsExecuteReportClickState *state,
    const StarsExecuteReportClickOps *ops, StarsPoint point, StarsI16 row)
{
    StarsExecuteReportClickFleet fleet;
    StarsI16 scan_id;
    StarsI16 next_row;

    (void)point;

    if (!state->enemy_fleet_report || row <= -3)
        return;
    next_row = row;
    if (row == -1)
        next_row = (StarsI16)(state->current_enemy_fleet_row + 1);
    else if (row == -2)
        next_row = (StarsI16)(state->current_enemy_fleet_row - 1);
    if (next_row < 0 || next_row >= state->enemy_fleet_rows)
        next_row = next_row < 0 ? (StarsI16)(state->enemy_fleet_rows - 1) : 0;
    if (!execute_report_get_fleet(ops, next_row,
            REPORT_CLICK_ENEMY_FLEET, &fleet))
        return;
    if (state->transfer_dialog != -1) {
        if (ops->message_beep)
            ops->message_beep(ops->context);
        return;
    }
    scan_id = fleet.id;
    if (ops->find_nearest)
        (void)ops->find_nearest(ops->context, fleet.point, 2, &scan_id);
    if (ops->change_scan_selection)
        ops->change_scan_selection(ops->context, fleet.point, 2, fleet.id);
    if (ops->ensure_point_on_screen)
        ops->ensure_point_on_screen(ops->context, fleet.point);
    state->current_enemy_fleet_row = next_row;
    if (ops->invalidate_report)
        ops->invalidate_report(ops->context, 2, 0);
}

static void select_battle_row(StarsExecuteReportClickState *state,
    const StarsExecuteReportClickOps *ops, StarsPoint point, StarsI16 row)
{
    StarsExecuteReportClickBattle battle;

    if (!execute_report_get_battle(ops, row, &battle))
        return;
    if (battle.point.x != state->selected_scan_x ||
            battle.point.y != state->selected_scan_y) {
        if (ops->change_scan_selection)
            ops->change_scan_selection(ops->context, battle.point, 0x8f,
                (StarsI16)battle.id);
        if (ops->center_scan)
            ops->center_scan(ops->context, battle.point);
        state->selected_scan_x = battle.point.x;
        state->selected_scan_y = battle.point.y;
        if (ops->invalidate_report)
            ops->invalidate_report(ops->context, 0, 0);
    } else if (state->vcr_window == 0 && ops->battle_vcr) {
        ops->battle_vcr(ops->context, battle.id);
    }
    (void)point;
}

static void show_starbase_popup(const StarsExecuteReportClickOps *ops,
    StarsPoint point, const StarsExecuteReportClickPlanet *planet,
    StarsI16 player_id)
{
    StarsExecuteReportClickPopup popup;
    StarsU32 table_offset;
    StarsU16 base_offset;
    StarsU16 selector;

    popup_clear(ops, &popup);
    popup.kind = REPORT_CLICK_POPUP_STARBASE;
    popup.planet_id = planet->id;
    popup.planet_value = 1;
    popup.starbase_hull = planet->starbase_hull;
    popup.starbase_fleet_far = ops->globalpd_starbase_fleet_far;
    if (ops->packed_shdef_far_table) {
        if (player_id < 0)
            return;
        table_offset = (StarsU32)(StarsU16)player_id * 4UL;
        if (table_offset + 4UL > ops->packed_shdef_far_table_size)
            return;
        base_offset = execute_report_packed_get16(
            ops->packed_shdef_far_table, (StarsU16)table_offset);
        selector = execute_report_packed_get16(
            ops->packed_shdef_far_table, (StarsU16)(table_offset + 2UL));
        popup.starbase_fleet_far = ((StarsU32)selector << 16) |
            (StarsU16)(base_offset +
            (StarsU16)(planet->starbase_design_index * 0x93U));
    }
    popup.part_bytes[0] = (StarsU8)(ops->globalpd_starbase_part & 0xffU);
    popup.part_bytes[1] = (StarsU8)(ops->globalpd_starbase_part >> 8);
    show_popup(ops, point, &popup);
}

static void show_installation_popup(const StarsExecuteReportClickOps *ops,
    StarsPoint point, const StarsExecuteReportClickPlanet *planet,
    StarsI16 factory)
{
    StarsExecuteReportClickPopup popup;

    popup_clear(ops, &popup);
    popup.kind = REPORT_CLICK_POPUP_INSTALLATION;
    popup.planet_id = planet->id;
    popup.planet_maximum = factory ? (ops->max_factories ?
        ops->max_factories(ops->context, planet, 0) : 0) :
        (ops->max_mines ? ops->max_mines(ops->context, planet, 0) : 0);
    popup.planet_value = factory ? (StarsI16)((planet->mineral_weight >> 20) &
        0x0fffU) : (StarsI16)((planet->mineral_weight >> 8) & 0x0fffU);
    popup.planet_minimum = factory ? (ops->max_operable_factories ?
        ops->max_operable_factories(ops->context, planet, 0) : 0) :
        (ops->max_operable_mines ?
            ops->max_operable_mines(ops->context, planet, 0) : 0);
    popup.planet_variable = (StarsI16)(factory != 0);
    show_popup(ops, point, &popup);
}

static void show_mineral_popup(StarsExecuteReportClickState *state,
    const StarsExecuteReportClickOps *ops, StarsPoint point,
    const StarsExecuteReportClickPlanet *planet, StarsI16 column)
{
    StarsExecuteReportClickPopup popup;
    StarsI32 estimate[4];
    StarsI16 offset;
    StarsI16 index;
    StarsI16 candidate;
    StarsU16 mask;

    offset = 2;
    mask = 1;
    for (index = 0; index < column; ++index) {
        if (((state->visible_fields_low & mask) != 0U ||
                (state->visible_fields_high & mask) != 0U) &&
                (index == 0 || state->first_field <= index))
            offset = (StarsI16)(offset + state->field_width[index] * 2);
        mask = (StarsU16)(mask << 1);
    }
    candidate = 1;
    while (candidate < 4 &&
            candidate * 2 * state->field_width[column] / 3 <=
                point.x - offset)
        ++candidate;
    index = (StarsI16)(candidate - 1);
    if (index < 0) index = 0;
    if (index > 3) index = 3;
    popup_clear(ops, &popup);
    popup.kind = REPORT_CLICK_POPUP_MINERAL;
    popup.planet_id = planet->id;
    popup.planet_variable = (StarsI16)(((StarsU16)planet->id >> 15) & 1U);
    popup.planet_value = (StarsI16)((planet->raw_status_flags >> 10) & 1U);
    popup.mineral_index = index;
    popup.planet_maximum = planet->mineral_value[index];
    popup.player_value = planet->player_value[index];
    popup.planet_minimum = planet->mineral_concentration[index < 3 ? index : 2];
    if (ops->estimate_minerals) {
        ops->estimate_minerals(ops->context, planet, estimate);
        popup.mineral_value = (StarsI16)estimate[index];
    }
    show_popup(ops, point, &popup);
}

static void select_planet_cell(StarsExecuteReportClickState *state,
    const StarsExecuteReportClickOps *ops, StarsPoint point, StarsI16 column,
    StarsI16 row)
{
    StarsExecuteReportClickPlanet planet;
    StarsExecuteReportClickPopup popup;
    StarsU16 threshold;
    StarsI16 defense_part;
    StarsI16 research_share;

    if (state->production_window != 0) {
        if (ops->message_beep)
            ops->message_beep(ops->context);
        return;
    }
    if (!execute_report_get_planet(ops, row, &planet))
        return;
    state->selected_planet_id = planet.id;
    if (ops->select_planet)
        ops->select_planet(ops->context, planet.id);
    if (ops->invalidate_report)
        ops->invalidate_report(ops->context, 0, 0);
    threshold = (StarsU16)(state->field_width[0] * 2 - 8);
    if (column == 0 && planet.has_starbase && point.x > (StarsI16)threshold)
        show_starbase_popup(ops, point, &planet, state->player_id);
    else if (column == 1 && planet.has_starbase)
        show_starbase_popup(ops, point, &planet, state->player_id);
    else if (column == 2 || column == 4) {
        popup_clear(ops, &popup);
        popup.kind = REPORT_CLICK_POPUP_PLANET;
        popup.planet_id = planet.id;
        show_popup(ops, point, &popup);
    } else if (column == 5) {
        if (ops->change_production)
            ops->change_production(ops->context);
    } else if (column == 6 || column == 7) {
        show_installation_popup(ops, point, &planet, (StarsI16)(column == 7));
    } else if (column == 8) {
        if (planet.defense_count != 0U && ops->best_defense) {
            popup_clear(ops, &popup);
            defense_part = 0;
            if (ops->best_defense(ops->context, (StarsU16 *)&defense_part)) {
                popup.kind = REPORT_CLICK_POPUP_DEFENSE;
                popup.planet_id = planet.id;
                popup.part = (StarsU16)defense_part;
                popup.part_bytes[0] = (StarsU8)(defense_part & 0xffU);
                popup.part_bytes[1] = (StarsU8)(defense_part >> 8);
                if (ops->globalpd_defense_part &&
                        ops->globalpd_defense_part_size >= 8U) {
                    StarsI16 index;
                    for (index = 0; index < 8; ++index)
                        popup.part_bytes[index] =
                            ops->globalpd_defense_part[index];
                }
                show_popup(ops, point, &popup);
            }
        }
    } else if (column >= 9 && column <= 11) {
        show_mineral_popup(state, ops, point, &planet, column);
    } else if (column == 12) {
        popup_clear(ops, &popup);
        popup.kind = REPORT_CLICK_POPUP_RESOURCES;
        popup.planet_id = planet.id;
        popup.planet_variable = planet.resources;
        /* The part.hs overlap at +4 is the just-written resource word;
         * the first union word at +2 still owns the selected planet ID. */
        popup.resource_part = (StarsU16)planet.resources;
        research_share = execute_report_research_share(planet.resources,
            planet.research_percent);
        if ((planet.flags & REPORT_CLICK_RESOURCE_EXEMPT) == 0U)
            popup.planet_value = (StarsI16)(planet.resources - research_share);
        else
            popup.planet_value = planet.resources;
        show_popup(ops, point, &popup);
    }
}

void stars_execute_report_click(StarsExecuteReportClickState *state,
    StarsPoint point, StarsI16 report_kind, StarsI16 column, StarsI16 row,
    const StarsExecuteReportClickOps *ops)
{
    StarsU16 dc;

    if (!state || !ops)
        return;
    dc = ops->get_dc ? ops->get_dc(ops->context, state->report_window) : 0;
    if (report_kind == 1)
        select_fleet_row(state, ops, point, row, column);
    else if (report_kind == 2)
        select_enemy_fleet_row(state, ops, point, row);
    else if (report_kind == 3)
        select_battle_row(state, ops, point, row);
    else
        select_planet_cell(state, ops, point, column, row);
    if (ops->release_dc)
        ops->release_dc(ops->context, state->report_window, dc);
}
