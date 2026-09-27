#include "core.h"

enum {
    CLICK_PLANET_OBJECT = 1,
    CLICK_PLANET_REFERENCE_FIRST = 6,
    CLICK_PLANET_MINE_FACTORY = 7,
    CLICK_PLANET_RESOURCES = 8,
    CLICK_PLANET_PLANET_INFO = 9,
    CLICK_PLANET_DEFENSES = 10,
    CLICK_PLANET_SCANNER = 11,
    CLICK_PLANET_WARP_BUTTON = 13,
    CLICK_PLANET_STARBASE = 14,
    CLICK_PLANET_WARP_MACHINE = 16,
    CLICK_PLANET_SHIP_ORDERS = 15,
    CLICK_PLANET_BUTTON = 17,
    CLICK_PLANET_POPULATION = 1,
    CLICK_PLANET_MINE_POPUP = 8,
    CLICK_PLANET_RESOURCE_POPUP = 0x0c,
    CLICK_PLANET_DEFENSE_POPUP = 9,
    CLICK_PLANET_SCANNER_POPUP = 9,
    CLICK_PLANET_SCANNER_SPECIAL_POPUP = 10,
    CLICK_PLANET_STARBASE_POPUP = 0x0b,
    CLICK_PLANET_WARP_BUTTON_BIT = 0x0100,
    CLICK_PLANET_BUTTON_TIMER = 0x50,
    CLICK_PLANET_BUTTON_TYPE = 8,
    CLICK_PLANET_STATUS_BUTTON_BIT = 0x2000
};

static StarsU32 click_planet_word(StarsU16 low, StarsU16 high)
{
    return (StarsU32)low | ((StarsU32)high << 16);
}

static StarsI16 click_planet_hit(const StarsClickPlanetInput *input,
    StarsU16 index, StarsPoint point)
{
    if (!input->reference_rects || index >= input->reference_rect_count ||
            !input->ops.point_in_rect)
        return 0;
    return input->ops.point_in_rect(input->context,
        &input->reference_rects[index], point);
}

static StarsI16 click_planet_race_stat(const StarsClickPlanetInput *input,
    StarsI16 stat)
{
    if (!input->ops.get_race_stat)
        return 0;
    return input->ops.get_race_stat(input->context, input->current_player,
        stat);
}

static void click_planet_popup(StarsClickPlanetInput *input,
    StarsU16 kind, StarsPoint point)
{
    input->popup_state.kind = kind;
    if (input->ops.popup)
        input->ops.popup(input->context, input->planet_window, point,
            &input->popup_state);
}

static StarsI16 click_planet_track(StarsClickPlanetInput *input,
    StarsU16 bit, StarsU16 text_id, StarsU16 initial_down,
    const StarsRect *rect)
{
    StarsPlanetClickButton button;
    const StarsU8 *text;
    StarsI16 tracking;

    (void)initial_down;
    button.flags = 0;
    text = input->ops.get_string ? input->ops.get_string(input->context,
        text_id) : (const StarsU8 *)0;
    if (input->ops.init_button)
        input->ops.init_button(input->context, &button,
            input->planet_window, rect, CLICK_PLANET_BUTTON_TYPE,
            input->arial8_height, text);
    tracking = 1;
    while (tracking && input->ops.track_button)
        tracking = input->ops.track_button(input->context, &button);
    if ((button.flags & 2U) != 0U)
        input->planet->flags = (StarsU16)(input->planet->flags ^ bit);
    return (StarsI16)((button.flags & 2U) != 0U);
}

static void click_planet_population(StarsClickPlanetInput *input,
    StarsPoint point)
{
    StarsI16 row;
    StarsU32 minerals[4];

    row = (StarsI16)((point.y - input->reference_rects[
        CLICK_PLANET_REFERENCE_FIRST].top) / input->arial8_height);
    input->popup_state.kind = CLICK_PLANET_POPULATION;
    input->popup_state.planet_id = (StarsU16)row;
    input->popup_state.planet_variable = (StarsI16)((StarsU16)row >> 15);
    if (row >= 0 && row < 3) {
        input->popup_state.planet_maximum =
            (StarsI16)input->planet->population_values[row * 2];
        input->popup_state.player_value =
            (StarsI16)input->planet->population_values[row * 2 + 1];
    } else {
        input->popup_state.planet_maximum = 0;
        input->popup_state.player_value = 0;
    }
    input->popup_state.player_minimum =
        (StarsI16)input->planet->current_population_value;
    input->popup_state.player_maximum = 0;
    minerals[0] = minerals[1] = minerals[2] = minerals[3] = 0UL;
    if (input->ops.estimate_minerals)
        input->ops.estimate_minerals(input->context, input->planet, minerals);
    input->popup_state.mineral_values[0] = minerals[0];
    input->popup_state.mineral_values[1] = minerals[1];
    input->popup_state.mineral_values[2] = minerals[2];
    input->popup_state.mineral_values[3] = minerals[3];
    input->popup_state.planet_value =
        (StarsI16)((input->planet->flags >> 10) & 1U);
    input->popup_state.planet_minimum = 0;
    click_planet_popup(input, CLICK_PLANET_POPULATION, point);
}

/* Genuine semantic reconstruction of ClickInPlanetOrders,
 * NE 0A:515C-587D.  The original routine is a hit-region dispatcher for the
 * planet pane.  Its former far pointers are represented by a planet record,
 * popup state, and named UI/service callbacks; the branch order and cursor
 * returns remain explicit. */
StarsU16 stars_click_in_planet_orders(StarsPoint point, StarsI16 sks,
    StarsI16 cursor_query, StarsI16 right_button,
    StarsClickPlanetInput *input)
{
    StarsRect rect;
    StarsI16 race_stat;
    StarsI16 two_machines;
    StarsI16 capacity;
    StarsI16 current_value;
    StarsI16 research_value;
    StarsI16 mine_factory_boundary;
    StarsI16 mine_factory_difference;
    StarsU32 production_bits;

    (void)sks;
    if (!input || !input->planet || input->selected_kind !=
            CLICK_PLANET_OBJECT || right_button)
        return 0;

    if (click_planet_hit(input, 6, point)) {
        if (cursor_query) return input->help_cursor;
        click_planet_population(input, point);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_MINE_FACTORY, point)) {
        if (cursor_query) return input->help_cursor;
        input->popup_state.planet_id = input->planet->id;
        mine_factory_boundary = (StarsI16)input->reference_rects[
            CLICK_PLANET_MINE_FACTORY].top;
        mine_factory_boundary = (StarsI16)(mine_factory_boundary +
            input->arial8_height);
        mine_factory_difference = (StarsI16)(point.y -
            mine_factory_boundary);
        input->popup_state.planet_maximum = (StarsI16)(
            !((StarsU16)mine_factory_difference & 0x8000U));
        if (input->popup_state.planet_maximum == 0) {
            capacity = input->ops.max_capacity ?
                input->ops.max_capacity(input->context, input->planet,
                    input->current_player, 0) : 0;
            input->popup_state.planet_variable = capacity;
            production_bits = click_planet_word(
                input->planet->mine_factory_bits_low,
                input->planet->mine_factory_bits_high);
            input->popup_state.planet_value =
                (StarsI16)((production_bits >> 8) & 0x0fffUL);
            input->popup_state.planet_minimum = input->ops.max_capacity ?
                input->ops.max_capacity(input->context, input->planet,
                    input->current_player, 1) : 0;
        } else {
            capacity = input->ops.max_capacity ?
                input->ops.max_capacity(input->context, input->planet,
                    input->current_player, 2) : 0;
            input->popup_state.planet_variable = capacity;
            production_bits = click_planet_word(
                input->planet->mine_factory_bits_low,
                input->planet->mine_factory_bits_high);
            input->popup_state.planet_value =
                (StarsI16)((production_bits >> 20) & 0x0fffUL);
            input->popup_state.planet_minimum = input->ops.max_capacity ?
                input->ops.max_capacity(input->context, input->planet,
                    input->current_player, 3) : 0;
        }
        click_planet_popup(input, CLICK_PLANET_MINE_POPUP, point);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_RESOURCES, point)) {
        if (cursor_query) return input->help_cursor;
        input->popup_state.planet_id = input->planet->id;
        current_value = input->ops.resources_at_planet ?
            input->ops.resources_at_planet(input->context, input->planet,
                input->current_player) : 0;
        input->popup_state.planet_variable = current_value;
        input->popup_state.planet_value = input->planet->resource_value;
        production_bits = click_planet_word(
            input->planet->scanner_bits_low,
            input->planet->scanner_bits_high);
        if ((production_bits & 0x00800000UL) == 0UL) {
            research_value = input->ops.muldiv ?
                input->ops.muldiv(input->context, current_value,
                    input->planet->research_percent, 100) : 0;
            input->popup_state.planet_value = (StarsI16)(
                input->popup_state.planet_value - research_value);
        }
        click_planet_popup(input, CLICK_PLANET_RESOURCE_POPUP, point);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_PLANET_INFO, point)) {
        if (cursor_query) return input->help_cursor;
        input->popup_state.planet_id = input->planet->id;
        click_planet_popup(input, 7, point);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_DEFENSES, point)) {
        if (input->planet->defense_count == 0U)
            return 0;
        if (cursor_query) return input->help_cursor;
        if (input->ops.best_defense_part)
            input->ops.best_defense_part(input->context,
                input->popup_state.part);
        input->popup_state.planet_id = input->planet->id;
        click_planet_popup(input, CLICK_PLANET_DEFENSE_POPUP, point);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_SCANNER, point)) {
        production_bits = click_planet_word(
            input->planet->scanner_bits_low,
            input->planet->scanner_bits_high);
        race_stat = click_planet_race_stat(input, 0x0e);
        if ((production_bits >> 12 & 0x1fUL) == 0x1fUL && race_stat != 8)
            return 0;
        if (cursor_query) return input->help_cursor;
        if (race_stat == 8) {
            input->popup_state.kind = CLICK_PLANET_SCANNER_SPECIAL_POPUP;
            input->popup_state.text = input->ops.get_string ?
                input->ops.get_string(input->context, 0x4eeU) :
                (const StarsU8 *)0;
        } else {
            input->popup_state.kind = CLICK_PLANET_SCANNER_POPUP;
            if (input->ops.best_scanner_part)
                input->ops.best_scanner_part(input->context,
                    input->popup_state.part);
        }
        click_planet_popup(input, input->popup_state.kind, point);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_WARP_BUTTON, point)) {
        StarsI16 warp_value;
        warp_value = input->ops.warp_machine ?
            input->ops.warp_machine(input->context, input->planet,
                &two_machines) : 0;
        if (!warp_value) return 0;
        if (cursor_query) return input->hand_cursor;
        rect = input->reference_rects[CLICK_PLANET_WARP_BUTTON];
        click_planet_track(input, CLICK_PLANET_WARP_BUTTON_BIT, 0x38aU,
            (StarsU16)((input->planet->flags >> 8) & 1U), &rect);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_STARBASE, point)) {
        if (cursor_query) return input->help_cursor;
        input->popup_state.kind = CLICK_PLANET_STARBASE_POPUP;
        input->popup_state.planet_id = input->planet->id;
        input->popup_state.fleet = input->context;
        input->popup_state.planet_variable = 0;
        input->popup_state.planet_value = 1;
        input->popup_state.planet_minimum = 0;
        input->popup_state.planet_maximum = 0;
        input->popup_state.player_value = 0;
        click_planet_popup(input, CLICK_PLANET_STARBASE_POPUP, point);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_WARP_MACHINE, point)) {
        StarsI16 warp_value;
        warp_value = input->ops.warp_machine ?
            input->ops.warp_machine(input->context, input->planet,
                &two_machines) : 0;
        if (!warp_value) return 0;
        if (cursor_query) return input->hand_cursor;
        input->popup_state.planet_id = 0x0200U;
        input->popup_state.planet_variable = (StarsI16)(
            (input->popup_state.planet_variable & (StarsI16)0xff00U) |
            ((warp_value + 2) & 0x00ff));
        if (input->ops.lookup_part)
            input->ops.lookup_part(input->context,
                input->popup_state.part);
        click_planet_popup(input, CLICK_PLANET_DEFENSE_POPUP, point);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_SHIP_ORDERS, point)) {
        if (input->ops.click_ship_orders)
            input->ops.click_ship_orders(input->context, point, sks, 0, 0);
        return 0;
    }
    if (click_planet_hit(input, CLICK_PLANET_BUTTON, point)) {
        if (cursor_query) return input->hand_cursor;
        rect = input->reference_rects[CLICK_PLANET_BUTTON];
        click_planet_track(input, CLICK_PLANET_STATUS_BUTTON_BIT, 0x38bU,
            (StarsU16)((input->planet->flags >> 13) & 1U), &rect);
    }
    return 0;
}
