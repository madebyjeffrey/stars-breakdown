#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 popups;
    StarsI16 ship_orders;
    StarsI16 buttons;
    StarsI16 last_kind;
    StarsI16 last_planet_id;
    StarsI16 last_variable;
    StarsI16 last_value;
    StarsI16 race_stat;
} Trace;

static StarsI16 in_rect(void *value, const StarsRect *rect, StarsPoint point)
{
    (void)value;
    return (StarsI16)(point.x >= (StarsI16)rect->left &&
        point.x < (StarsI16)rect->right && point.y >= (StarsI16)rect->top &&
        point.y < (StarsI16)rect->bottom);
}
static const StarsU8 *get_string(void *value, StarsU16 id)
{
    (void)value;
    return id == 0x4eeU ? (const StarsU8 *)"special" :
        (const StarsU8 *)"button";
}
static void init_button(void *value, StarsPlanetClickButton *button,
    StarsU16 window, const StarsRect *rect, StarsU16 type, StarsI16 height,
    const StarsU8 *text)
{
    Trace *trace = (Trace *)value;
    (void)window; (void)rect; (void)type; (void)height; (void)text;
    ++trace->buttons;
    button->flags = 2;
}
static StarsI16 track_button(void *value, StarsPlanetClickButton *button)
{
    (void)value; (void)button;
    return 0;
}
static StarsI16 race_stat(void *value, StarsI16 player, StarsI16 stat)
{
    Trace *trace = (Trace *)value;
    (void)player; (void)stat;
    return trace->race_stat;
}
static StarsI16 resources(void *value, const StarsClickPlanet *planet,
    StarsI16 player)
{
    (void)value; (void)planet; (void)player;
    return 30;
}
static StarsI16 capacity(void *value, const StarsClickPlanet *planet,
    StarsI16 player, StarsI16 kind)
{
    (void)value; (void)planet; (void)player;
    return (StarsI16)(kind == 0 || kind == 2 ? 40 : 12);
}
static StarsI16 warp_machine(void *value, const StarsClickPlanet *planet,
    StarsI16 *two)
{
    (void)value; (void)planet;
    if (two) *two = 1;
    return 5;
}
static void lookup_part(void *value, StarsU8 *part)
{
    (void)value;
    part[0] = 0x20; part[1] = 0x80; part[2] = 7;
}
static void estimate(void *value, const StarsClickPlanet *planet,
    StarsU32 values[4])
{
    (void)value; (void)planet;
    values[0] = 11UL; values[1] = 22UL; values[2] = 33UL; values[3] = 44UL;
}
static StarsI16 muldiv(void *value, StarsI16 a, StarsI16 b, StarsI16 c)
{
    (void)value;
    return (StarsI16)(((StarsI32)a * (StarsI32)b) / c);
}
static void ship_orders(void *value, StarsPoint point, StarsI16 sks,
    StarsI16 cursor, StarsI16 right)
{
    Trace *trace = (Trace *)value;
    (void)point; (void)sks; (void)cursor; (void)right;
    ++trace->ship_orders;
}
static void popup(void *value, StarsU16 window, StarsPoint point,
    const StarsClickPlanetPopup *state)
{
    Trace *trace = (Trace *)value;
    (void)window; (void)point;
    ++trace->popups;
    trace->last_kind = (StarsI16)state->kind;
    trace->last_planet_id = (StarsI16)state->planet_id;
    trace->last_variable = state->planet_variable;
    trace->last_value = state->planet_value;
}

static void clear_rects(StarsRect *rects)
{
    StarsI16 i;
    for (i = 0; i < 18; ++i) {
        rects[i].left = 0; rects[i].right = 100;
        rects[i].top = 300; rects[i].bottom = 320;
    }
}

int main(void)
{
    StarsClickPlanetInput input;
    StarsClickPlanet planet;
    StarsRect rects[18];
    Trace trace;
    StarsPoint point;
    StarsU16 result;
    int failures = 0;

    memset(&input, 0, sizeof input);
    memset(&planet, 0, sizeof planet);
    memset(&trace, 0, sizeof trace);
    clear_rects(rects);
    planet.id = 42; planet.resource_value = 100; planet.research_percent = 100;
    planet.population_values[2] = 77; planet.population_values[3] = 8;
    planet.mine_factory_bits_low = 0x1200;
    planet.mine_factory_bits_high = 0x0000;
    planet.defense_count = 1;
    input.selected_kind = 1; input.current_player = 3;
    input.arial8_height = 8; input.planet_window = 9;
    input.hand_cursor = 11; input.help_cursor = 12;
    input.reference_rects = rects; input.reference_rect_count = 18;
    input.planet = &planet; input.context = &trace;
    input.ops.point_in_rect = in_rect; input.ops.get_string = get_string;
    input.ops.init_button = init_button; input.ops.track_button = track_button;
    input.ops.get_race_stat = race_stat; input.ops.resources_at_planet = resources;
    input.ops.max_capacity = capacity; input.ops.warp_machine = warp_machine;
    input.ops.lookup_part = lookup_part; input.ops.best_defense_part = lookup_part;
    input.ops.best_scanner_part = lookup_part; input.ops.estimate_minerals = estimate;
    input.ops.muldiv = muldiv; input.ops.click_ship_orders = ship_orders;
    input.ops.popup = popup;
    point.x = 10; point.y = 316;

    rects[6].top = 300; rects[6].bottom = 340;
    result = stars_click_in_planet_orders(point, 0, 0, 0, &input);
    if (result != 0 || trace.popups != 1 || trace.last_kind != 1 ||
            trace.last_planet_id != 2 || trace.last_value != 0)
        failures |= 1;

    memset(&trace, 0, sizeof trace); clear_rects(rects);
    rects[7].top = 20; rects[7].bottom = 40; point.y = 25;
    stars_click_in_planet_orders(point, 0, 0, 0, &input);
    if (trace.last_kind != 8 || trace.last_variable != 40 ||
            trace.last_value != 0x12)
        failures |= 2;

    memset(&trace, 0, sizeof trace); clear_rects(rects);
    rects[8].top = 20; rects[8].bottom = 40; point.y = 25;
    planet.mine_factory_bits_low = 0;
    stars_click_in_planet_orders(point, 0, 0, 0, &input);
    if (trace.last_kind != 0x0c || trace.last_value != 70)
        failures |= 4;

    memset(&trace, 0, sizeof trace); clear_rects(rects);
    rects[11].top = 20; rects[11].bottom = 40; point.y = 25;
    planet.scanner_bits_low = 0xf000; planet.scanner_bits_high = 1;
    trace.race_stat = 8;
    result = stars_click_in_planet_orders(point, 0, 1, 0, &input);
    if (result != 12) failures |= 8;
    stars_click_in_planet_orders(point, 0, 0, 0, &input);
    if (trace.last_kind != 10) failures |= 16;

    memset(&trace, 0, sizeof trace); clear_rects(rects);
    rects[13].top = 20; rects[13].bottom = 40; point.y = 25;
    planet.flags = 0;
    result = stars_click_in_planet_orders(point, 0, 1, 0, &input);
    stars_click_in_planet_orders(point, 0, 0, 0, &input);
    if (result != 11 || trace.buttons != 1 || (planet.flags & 0x0100U) == 0U)
        failures |= 32;

    memset(&trace, 0, sizeof trace); clear_rects(rects);
    rects[15].top = 20; rects[15].bottom = 40; point.y = 25;
    stars_click_in_planet_orders(point, 7, 0, 0, &input);
    if (trace.ship_orders != 1) failures |= 64;

#ifndef TARGET_NATIVE
    if (failures == 0) puts("CLICK_PLANET_ORDERS_CHECKS=6 FAILURES=0 RESULT=PASS");
    else printf("CLICK_PLANET_ORDERS_CHECKS=6 FAILURES=%d RESULT=FAIL\n", failures);
#endif
    return failures != 0;
}
