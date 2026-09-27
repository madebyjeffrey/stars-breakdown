#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    int setup_calls;
    int ext_calls;
    int right_calls;
    int center_calls;
    int fill_calls;
    int mineral_calls;
    int production_calls;
    StarsI32 last_mineral;
    StarsI16 last_mineral_kind;
    StarsI16 last_right;
} Trace;

typedef struct Fixture {
    Trace trace;
    StarsReportItemPlanet planet;
    StarsReportItemFleet fleet;
    StarsReportItemBattle battle;
} Fixture;

static const StarsReportItemPlanet *planet_at(void *v, StarsI16 row)
{
    Fixture *f = (Fixture *)v;
    return row == 0 ? &f->planet : 0;
}

static const StarsReportItemFleet *fleet_at(void *v, StarsI16 report,
    StarsI16 row)
{
    Fixture *f = (Fixture *)v;
    (void)report;
    return row == 0 ? &f->fleet : 0;
}

static const StarsReportItemBattle *battle_at(void *v, StarsI16 row)
{
    Fixture *f = (Fixture *)v;
    return row == 0 ? &f->battle : 0;
}

static StarsI16 extent(void *v, StarsU16 dc, const StarsU8 *text,
    StarsU16 length)
{
    (void)v; (void)dc; (void)text;
    return (StarsI16)(length * 4U);
}

static void select_object(void *v, StarsU16 dc, StarsU16 object)
{
    Trace *t = &((Fixture *)v)->trace;
    (void)dc; (void)object; ++t->setup_calls;
}

static void set_color(void *v, StarsU16 dc, StarsU32 color)
{ (void)v; (void)dc; (void)color; }

static void set_mode(void *v, StarsU16 dc, StarsU16 mode)
{ (void)v; (void)dc; (void)mode; }

static void ext_text(void *v, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsU16 options, const StarsRect *rect, const StarsU8 *text,
    StarsU16 length)
{
    Trace *t = &((Fixture *)v)->trace;
    (void)dc; (void)x; (void)y; (void)options; (void)rect; (void)text;
    (void)length; ++t->ext_calls;
}

static void right_text(void *v, StarsU16 dc, StarsI16 right, StarsI16 top,
    const StarsU8 *text, StarsI16 length, StarsI16 offset)
{
    Trace *t = &((Fixture *)v)->trace;
    (void)dc; (void)top; (void)text; (void)length; (void)offset;
    t->last_right = right; ++t->right_calls;
}

static void text_out(void *v, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsU16 length)
{ (void)v; (void)dc; (void)x; (void)y; (void)text; (void)length; }

static void center_text(void *v, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsU16 length)
{
    Trace *t = &((Fixture *)v)->trace;
    (void)dc; (void)x; (void)y; (void)text; (void)length; ++t->center_calls;
}

static void fill_rect(void *v, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{ Trace *t=&((Fixture *)v)->trace; (void)dc;(void)rect;(void)brush;++t->fill_calls; }

static void draw_mineral(void *v, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsI16 mineral, StarsI32 value)
{
    Trace *t = &((Fixture *)v)->trace;
    (void)dc; (void)x; (void)y; t->last_mineral_kind = mineral;
    t->last_mineral = value; ++t->mineral_calls;
}

static void draw_production(void *v, StarsU16 dc, const StarsRect *rect,
    const StarsReportItemPlanet *planet)
{
    Trace *t = &((Fixture *)v)->trace;
    (void)dc; (void)rect; (void)planet; ++t->production_calls;
}

static void init_fixture(Fixture *f)
{
    StarsI16 i;
    memset(f, 0, sizeof *f);
    f->planet.id = 7; f->planet.inhabited = 1; f->planet.population = 10;
    f->planet.max_population = 20; f->planet.owner_name = (const StarsU8 *)"Terran";
    f->planet.name = (const StarsU8 *)"New Eden";
    f->planet.next_name[0] = (const StarsU8 *)"Alpha";
    f->planet.next_name[1] = (const StarsU8 *)"Beta";
    f->planet.production_name = (const StarsU8 *)"Factory";
    f->planet.desirability_label = (const StarsU8 *)"Hab";
    f->planet.capacity_percent = 72; f->planet.desirability = 8;
    f->planet.optimal_value = 12; f->planet.resources = 1234;
    f->planet.research_percent = 30; f->planet.warp_speed = 7;
    f->planet.stargate = 1; f->planet.defense_survival_percent = 88;
    for (i = 0; i < 3; ++i) {
        f->planet.mineral_weight[i] = 100L + i;
        f->planet.estimated_minerals[i] = 200L + i;
        f->planet.mineral_concentration[i] = 50 + i;
        f->planet.current_installations[i] = 3 + i;
        f->planet.operable_installations[i] = 4 + i;
        f->planet.brush_status[i] = (StarsU16)(10 + i);
    }
    f->fleet.id = 4; f->fleet.planet_id = 7; f->fleet.position.x = 12;
    f->fleet.position.y = 34; f->fleet.name = (const StarsU8 *)"Fleet 5";
    f->fleet.location_name = (const StarsU8 *)"New Eden";
    f->fleet.destination_name = (const StarsU8 *)"Beta";
    f->fleet.eta_name = (const StarsU8 *)"2 years";
    f->fleet.task_name = (const StarsU8 *)"Colonize";
    f->fleet.plan_name = (const StarsU8 *)"Plan A";
    f->fleet.primary_name = (const StarsU8 *)"Scout";
    f->fleet.primary_count = 3; f->fleet.fuel = 600; f->fleet.fuel_safe = 1;
    f->fleet.cloak_percent = 25; f->fleet.combat_ship_count = 2;
    f->fleet.scanner_visible = 1; f->fleet.scanner_percent = 80;
    f->fleet.minerals[0] = 10; f->fleet.minerals[1] = 20;
    f->fleet.minerals[2] = 30; f->fleet.minerals[3] = 40;
    f->fleet.weight = 1000;
    for (i = 0; i < STARS_REPORT_ITEM_SHIP_TYPES; ++i)
        f->fleet.ship_counts[i] = 1;
    for (i = 0; i < 6; ++i) f->fleet.ship_category_count[i] = 10 + i;
    f->battle.position.x = 12; f->battle.position.y = 34;
    f->battle.location_name = (const StarsU8 *)"Battle site";
    f->battle.cplr = 75; f->battle.units[5] = 1; f->battle.units[0xfd] = 9;
    f->battle.units[0xfe] = 7; f->battle.kills[0] = 2; f->battle.kills[1] = 1;
}

int main(void)
{
    Fixture f;
    StarsDrawReportItemOps ops;
    StarsDrawReportItemInput input;
    StarsRect rect;
    StarsI16 column;
    int failures = 0;

    init_fixture(&f); memset(&ops, 0, sizeof ops); memset(&input, 0, sizeof input);
    ops.planet_at = planet_at; ops.fleet_at = fleet_at; ops.battle_at = battle_at;
    ops.text_extent = extent; ops.select_object = select_object;
    ops.set_text_color = set_color; ops.set_background_mode = set_mode;
    ops.ext_text_out = ext_text; ops.right_text_out = right_text;
    ops.text_out = text_out; ops.center_text = center_text;
    ops.fill_rect = fill_rect; ops.draw_mineral = draw_mineral;
    ops.draw_production = draw_production;
    input.font = 3; input.selection_kind = 1; input.selected_planet = 7;
    input.selected_fleet = 4; input.selected_scan_fleet = 4;
    input.selected_scan_point = f.battle.position; input.color_black = 0;
    input.color_selected = 0x7f; input.color_warning = 0xff;
    input.color_disabled = 0xff; input.double_dash = (const StarsU8 *)"--";
    input.ops = &ops; input.context = &f;
    rect.left = 10; rect.top = 20; rect.right = 110; rect.bottom = 40;

    for (column = 0; column <= 14; ++column)
        stars_draw_report_item(9, &rect, 0, 0, column, &input);
    for (column = 0; column <= 11; ++column)
        stars_draw_report_item(9, &rect, 1, 0, column, &input);
    for (column = 0; column <= 11; ++column)
        stars_draw_report_item(9, &rect, 2, 0, column, &input);
    for (column = 0; column <= 14; ++column)
        stars_draw_report_item(9, &rect, 3, 0, column, &input);
    if (f.trace.ext_calls < 5) failures |= 1;
    if (f.trace.right_calls < 15) failures |= 2;
    if (f.trace.fill_calls < 3) failures |= 4;
    if (f.trace.mineral_calls < 20) failures |= 8;
    if (f.trace.production_calls != 1) failures |= 16;
    if (f.trace.last_mineral != 5L) failures |= 32;
    if (f.trace.last_mineral_kind != -1) failures |= 64;
#ifndef TARGET_NATIVE
    if (failures == 0)
        printf("DRAW_REPORT_ITEM_CHECKS=4 FAILURES=0 RESULT=PASS\n");
    else
        printf("DRAW_REPORT_ITEM_CHECKS=4 FAILURES=%d RESULT=FAIL\n", failures);
#endif
    return failures != 0;
}
