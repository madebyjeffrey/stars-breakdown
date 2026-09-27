#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 hit;
    StarsI16 prepare;
    StarsI16 show;
    StarsI16 threshold_menu;
    StarsI16 threshold;
    StarsI16 mine_invalidate;
    StarsI16 scanner_visible;
    StarsI16 invalidate;
    StarsI16 mineral_choices;
    StarsI16 advance;
    StarsI16 cargo;
    StarsI16 last_hit;
    StarsI16 last_popup;
    StarsI16 last_threshold;
    StarsI16 choice;
    StarsI16 selected_index;
    StarsI16 packed_prepare;
    StarsI16 packed_planet;
    StarsI16 packed_fleet;
    StarsI16 packed_thing;
    StarsI16 resolved_minerals;
    StarsI16 resolved_terraform;
} Trace;

static void put16(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}
static StarsI16 hit_test(void *c, StarsI16 x, StarsI16 y)
{
    Trace *t = (Trace *)c; (void)x; (void)y; return t->hit;
}
static StarsI16 prepare(void *c, StarsI16 hit)
{
    Trace *t = (Trace *)c; ++t->prepare; t->last_hit = hit;
    return (StarsI16)(100 + hit);
}
static StarsI16 prepare_data(void *c, StarsI16 hit,
    const StarsMineClickPopupData *data)
{
    Trace *t = (Trace *)c;
    ++t->packed_prepare;
    if (data->hit == hit && data->planet_id == 55 &&
            data->planet_owner == 3 && data->planet_detail == 4 &&
            data->planet_var == 0 && data->planet_value == 12 &&
            data->planet_min == 7 && data->planet_max == 19)
        t->packed_planet = 1;
    if (data->popup_kind == 1 && data->popup_string_id == 501 &&
            data->derived_minerals && data->mineral_output[0] == 19 &&
            data->mineral_output[1] == 11 && data->mineral_output[2] == 7)
        t->resolved_minerals = 1;
    if (data->popup_kind == 5 && data->popup_string_id == 505 &&
            data->derived_terraform && data->terraform_min[0] == 8 &&
            data->terraform_max[0] == 18 && data->player_value == 12 &&
            data->player_min == 7 && data->player_max == 19)
        t->resolved_terraform = 1;
    if (data->fleet_id == 17 && data->fleet_owner == 2 &&
            data->fleet_planet == 55 && data->fleet_ship_count == 6)
        t->packed_fleet = 1;
    if (data->object_id == 21 && data->object_owner == 4 &&
            data->object_kind == 1 && data->object_x == 321 &&
            data->object_y == 654)
        t->packed_thing = 1;
    return (StarsI16)(100 + hit);
}
static StarsI16 resolve_minerals(void *c, StarsMineClickPopupData *data)
{
    (void)c;
    if (data->planet_id != 55 || data->planet_var != 0)
        return 0;
    data->mineral_output[0] = 19;
    data->mineral_output[1] = 11;
    data->mineral_output[2] = 7;
    return 1;
}
static StarsI16 resolve_terraform(void *c, StarsMineClickPopupData *data)
{
    (void)c;
    if (data->planet_id != 55 || data->planet_var != 0)
        return 0;
    data->terraform_min[0] = 8;
    data->terraform_max[0] = 18;
    data->player_value = 12;
    data->player_min = 7;
    data->player_max = 19;
    return 1;
}
static StarsI16 popup_string(void *c, StarsI16 popup_kind, StarsI16 variable)
{
    (void)c;
    return (StarsI16)(500 + popup_kind + variable);
}
static void show(void *c, StarsI16 kind, StarsI16 x, StarsI16 y)
{
    Trace *t = (Trace *)c; (void)x; (void)y; ++t->show; t->last_popup = kind;
}
static StarsI16 threshold_menu(void *c, StarsI16 *values, StarsI16 count,
    StarsI16 selected)
{
    Trace *t = (Trace *)c;
    if (count != 9 || values[0] != 100 || values[3] != 2500 ||
            values[8] != 30000 || selected != t->selected_index)
        return -1;
    ++t->threshold_menu;
    return t->choice;
}
static void set_threshold(void *c, StarsI16 value)
{
    Trace *t = (Trace *)c; ++t->threshold; t->last_threshold = value;
}
static StarsI16 scanner_visible(void *c)
{
    return ((Trace *)c)->scanner_visible;
}
static void invalidate(void *c, StarsI16 value)
{
    (void)value; ++((Trace *)c)->invalidate;
}
static void invalidate_mine(void *c)
{
    ++((Trace *)c)->mine_invalidate;
}
static void mineral_choices(void *c, StarsI16 hit)
{
    Trace *t = (Trace *)c; (void)hit; ++t->mineral_choices;
}
static void advance(void *c, StarsI16 hit)
{
    Trace *t = (Trace *)c; (void)hit; ++t->advance;
}
static void cargo(void *c, StarsI16 hit)
{
    Trace *t = (Trace *)c; (void)hit; ++t->cargo;
}
static void install(StarsMineClickOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->hit_test = hit_test;
    ops->prepare_popup = prepare;
    ops->prepare_popup_data = prepare_data;
    ops->show_popup = show;
    ops->threshold_menu = threshold_menu;
    ops->set_threshold = set_threshold;
    ops->scanner_visible = scanner_visible;
    ops->popup_mineral_choices = mineral_choices;
    ops->advance_scan_selection = advance;
    ops->popup_fleet_cargo = cargo;
    ops->invalidate_mine = invalidate_mine;
    ops->set_default_popup = invalidate;
    ops->context = trace;
    ops->resolve_minerals = resolve_minerals;
    ops->resolve_terraform = resolve_terraform;
    ops->popup_string = popup_string;
}
int main(void)
{
    StarsMineClickInput input;
    StarsMineClickOps ops;
    Trace trace;
    StarsU8 packed_planet[56];
    StarsU8 packed_fleet[124];
    StarsU8 packed_thing[18];

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    memset(packed_planet, 0, sizeof packed_planet);
    memset(packed_fleet, 0, sizeof packed_fleet);
    memset(packed_thing, 0, sizeof packed_thing);
    install(&ops, &trace);
    input.current_graph_max = 1000;
    input.ops = &ops;
    trace.hit = 1;
    stars_mine_click(10, 20, 1, 0, &input);
    if (trace.prepare != 1 || trace.show != 1 || trace.last_hit != 1 ||
            trace.last_popup != 101)
        return 1;
    trace.hit = 2;
    stars_mine_click(10, 20, 1, 0, &input);
    trace.hit = 3;
    stars_mine_click(10, 20, 1, 0, &input);
    if (trace.prepare != 3 || trace.show != 3)
        return 1;
    trace.hit = 5;
    trace.choice = 4;
    trace.selected_index = 2;
    trace.scanner_visible = 1;
    stars_mine_click(10, 20, 1, 0, &input);
    if (trace.threshold_menu != 1 || trace.threshold != 1 ||
            trace.last_threshold != 5000 || input.current_graph_max != 5000 ||
            trace.mine_invalidate != 1 || trace.invalidate != 1)
        return 1;
    trace.hit = 6;
    stars_mine_click(10, 20, 1, 0, &input);
    trace.hit = 8;
    stars_mine_click(10, 20, 1, 0, &input);
    if (trace.prepare != 5 || trace.show != 5)
        return 1;
    trace.hit = 9;
    stars_mine_click(10, 20, 1, 0, &input);
    stars_mine_click(10, 20, 0x204, 0, &input);
    if (trace.advance != 1 || trace.mineral_choices != 1)
        return 1;
    trace.hit = 10;
    stars_mine_click(10, 20, 1, 0, &input);
    if (trace.prepare != 6 || trace.show != 6)
        return 1;
    trace.hit = 11;
    stars_mine_click(10, 20, 0x204, 0, &input);
    stars_mine_click(10, 20, 1, 0, &input);
    if (trace.cargo != 1 || trace.prepare != 7 || trace.show != 7)
        return 1;
    trace.hit = 12;
    stars_mine_click(10, 20, 1, 0, &input);
    trace.hit = 13;
    stars_mine_click(10, 20, 1, 0, &input);
    trace.hit = 14;
    stars_mine_click(10, 20, 1, 0, &input);
    if (trace.prepare != 10 || trace.show != 10)
        return 1;
    /* Packed GlobalPD-facing payloads are decoded only when raw records are
     * supplied; the earlier typed callback path remains unchanged. */
    put16(packed_planet, 0, 55);
    put16(packed_planet, 2, 3);
    packed_planet[4] = 4;
    packed_planet[9] = 7;
    packed_planet[12] = 12;
    packed_planet[15] = 19;
    put16(packed_fleet, 0, 17);
    put16(packed_fleet, 2, 2);
    put16(packed_fleet, 6, 55);
    put16(packed_fleet, 12, 6);
    put16(packed_thing, 0, (StarsU16)((1U << 13) | (4U << 9) | 21U));
    put16(packed_thing, 2, 321);
    put16(packed_thing, 4, 654);
    input.packed_planet_record = packed_planet;
    input.packed_planet_size = sizeof packed_planet;
    input.packed_fleet_record = packed_fleet;
    input.packed_fleet_size = sizeof packed_fleet;
    input.packed_thing_record = packed_thing;
    input.packed_thing_size = sizeof packed_thing;
    trace.hit = 1;
    stars_mine_click(10, 20, 1, 0, &input);
    if (trace.packed_prepare != 1 || trace.packed_planet != 1 ||
            trace.packed_fleet != 1 || trace.packed_thing != 1 ||
            trace.resolved_minerals != 1 || trace.show != 11)
        return 1;
    trace.hit = 6;
    stars_mine_click(10, 20, 1, 0, &input);
    if (trace.resolved_terraform != 1 || trace.show != 12)
        return 1;
    trace.hit = 1;
    stars_mine_click(10, 20, 0x204, 0, &input);
    if (trace.prepare != 10 || trace.show != 12)
        return 1;
    puts("MINE_CLICK_CHECKS=36 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("MCLR.TXT", "w");
        if (!report)
            return 7;
        fprintf(report, "MINE_CLICK_CHECKS=36 FAILURES=0 RESULT=PASS\n");
        fclose(report);
    }
#endif
    return 0;
}
