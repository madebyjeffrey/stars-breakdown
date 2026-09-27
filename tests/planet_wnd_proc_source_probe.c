#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 create;
    StarsI16 paint;
    StarsI16 color;
    StarsI16 cursor_hit;
    StarsI16 cursor_set;
    StarsI16 draw_item;
    StarsI16 measure;
    StarsI16 min_size;
    StarsI16 focus;
    StarsI16 ship_command;
    StarsI16 dropdown;
    StarsI16 find_transfer;
    StarsI16 transfer;
    StarsI16 try_planet;
    StarsI16 select_planet;
    StarsI16 select_base;
    StarsI16 build;
    StarsI16 production;
    StarsI16 confirm;
    StarsI16 click;
    StarsI16 active;
    StarsI16 default_proc;
    StarsI16 object_kind;
    StarsI16 action_confirm;
    StarsU16 active_value;
    StarsI16 click_double;
    StarsPoint build_point;
} Trace;
static void create(void *c, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)c)->create;
}
static void paint(void *c, StarsU16 hwnd, StarsU16 dc)
{
    Trace *t = (Trace *)c; (void)hwnd; (void)dc; ++t->paint;
}
static StarsI32 color(void *c, StarsU16 dc, StarsU16 ignored, StarsI32 lp)
{
    Trace *t = (Trace *)c; (void)dc; (void)ignored; (void)lp; ++t->color;
    return 44;
}
static StarsU16 cursor_hit(void *c, StarsU16 hwnd, StarsI16 planet,
    StarsI16 fleet)
{
    Trace *t = (Trace *)c;
    (void)hwnd; (void)planet; (void)fleet; ++t->cursor_hit;
    return 7;
}
static void cursor_set(void *c, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)c)->cursor_set;
}
static void draw_item(void *c, StarsI32 data)
{
    (void)data; ++((Trace *)c)->draw_item;
}
static void measure(void *c, StarsI32 data, StarsI16 height)
{
    (void)data; (void)height; ++((Trace *)c)->measure;
}
static void simple(void *c)
{
    ++((Trace *)c)->min_size;
}
static void focus(void *c)
{
    ++((Trace *)c)->focus;
}
static void ship_command(void *c, StarsU16 hwnd, StarsU16 wparam,
    StarsI32 lparam)
{
    Trace *t = (Trace *)c;
    (void)hwnd; (void)wparam; (void)lparam; ++t->ship_command;
}
static void dropdown(void *c, StarsI16 changed)
{
    (void)changed; ++((Trace *)c)->dropdown;
}
static StarsI16 get_selection(void *c)
{
    (void)c; return 4;
}
static StarsI16 find_transfer(void *c, StarsI16 planet, StarsI16 selection,
    StarsI16 *id, StarsI16 *kind)
{
    Trace *t = (Trace *)c;
    (void)planet; (void)selection; ++t->find_transfer;
    *id = 27; *kind = t->object_kind; return 1;
}
static void transfer(void *c, StarsI16 planet, StarsI16 mode,
    StarsI16 id, StarsI16 kind)
{
    Trace *t = (Trace *)c;
    (void)planet; (void)mode; (void)id; (void)kind; ++t->transfer;
}
static StarsI16 try_planet(void *c, StarsI16 current, StarsI16 ignored)
{
    (void)current; (void)ignored; return ((Trace *)c)->try_planet;
}
static void select_planet(void *c, StarsI16 id, StarsI16 mode)
{
    Trace *t = (Trace *)c; (void)id; (void)mode; ++t->select_planet;
}
static void select_base(void *c, StarsI16 planet, StarsI16 mode)
{
    Trace *t = (Trace *)c; (void)planet; (void)mode; ++t->select_base;
}
static void build(void *c, StarsPoint point)
{
    Trace *t = (Trace *)c; ++t->build; t->build_point = point;
}
static void production(void *c, StarsI16 mode)
{
    Trace *t = (Trace *)c; (void)mode; ++t->production;
}
static StarsI16 confirm(void *c)
{
    Trace *t = (Trace *)c; ++t->confirm; return t->action_confirm;
}
static void click(void *c, StarsI16 x, StarsI16 y, StarsU16 buttons,
    StarsI16 double_click)
{
    Trace *t = (Trace *)c;
    (void)x; (void)y; (void)buttons; ++t->click; t->click_double = double_click;
}
static void active(void *c, StarsU16 hwnd)
{
    Trace *t = (Trace *)c; ++t->active; t->active_value = hwnd;
}
static void default_proc(void *c)
{
    ++((Trace *)c)->default_proc;
}
static void install(StarsPlanetWndProcOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->create_controls = create;
    ops->paint_planet_ship = paint;
    ops->color_control = color;
    ops->cursor_hit_test = cursor_hit;
    ops->set_cursor = cursor_set;
    ops->draw_combo_item = draw_item;
    ops->measure_combo_item = measure;
    ops->set_minimum_size = simple;
    ops->set_focus_frame = focus;
    ops->ship_command = ship_command;
    ops->ship_dropdown_changed = dropdown;
    ops->find_transfer = find_transfer;
    ops->get_dropdown_selection = get_selection;
    ops->transfer = transfer;
    ops->try_select_adj_planet = try_planet;
    ops->select_adj_planet = select_planet;
    ops->select_adj_starbase = select_base;
    ops->build_ship = build;
    ops->change_production = production;
    ops->confirm_production = confirm;
    ops->planet_click = click;
    ops->set_active = active;
    ops->advance_tutor = focus;
    ops->default_window_proc = default_proc;
    ops->context = trace;
}
static StarsI32 point(StarsI16 x, StarsI16 y)
{
    return (StarsI32)(((StarsU32)(StarsU16)y << 16) |
        (StarsU32)(StarsU16)x);
}
static StarsI32 notify(StarsU16 handle, StarsI16 code)
{
    return (StarsI32)(((StarsU32)(StarsU16)code << 16) | handle);
}
int main(void)
{
    StarsPlanetWndProcInput input;
    StarsPlanetWndProcOps ops;
    Trace trace;
    StarsI16 i;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    input.rep_combo = 300;
    input.ship_dropdown = 200;
    input.planet_production_list = 400;
    input.frame_window = 99;
    for (i = 0; i < 13; ++i)
        input.button_handles[i] = (StarsU16)(100 + i);
    input.selected_kind = 1;
    input.selected_planet = 8;
    input.selected_fleet = 12;
    input.shift_down = 1;
    input.ops = &ops;
    if (stars_planet_wnd_proc(50, 1, 0, 0, &input) != 0 ||
            stars_planet_wnd_proc(50, 0x0f, 9, 0, &input) != 0 ||
            stars_planet_wnd_proc(50, 0x14, 0, 0, &input) != 1 ||
            stars_planet_wnd_proc(50, 0x19, 0, 300, &input) != 44 ||
            stars_planet_wnd_proc(50, 0x20, 0, 0, &input) != 1 ||
            stars_planet_wnd_proc(50, 0x24, 0, 0, &input) != 0 ||
            stars_planet_wnd_proc(50, 0x2b, 0, 0, &input) != 1 ||
            stars_planet_wnd_proc(50, 0x2c, 0, 0, &input) != 1 ||
            trace.create != 1 || trace.paint != 1 || trace.color != 1 ||
            trace.cursor_hit != 1 || trace.cursor_set != 1 ||
            trace.draw_item != 1 || trace.measure != 1 || trace.min_size != 1)
        return 1;
    trace.try_planet = 1;
    if (stars_planet_wnd_proc(50, 0x102, 0x66, 0, &input) != 0 ||
            trace.select_base != 0)
        return 1;
    trace.try_planet = 0;
    if (stars_planet_wnd_proc(50, 0x102, 0x46, 0, &input) != 0 ||
            trace.select_base != 1)
        return 1;
    input.selected_kind = 2;
    if (stars_planet_wnd_proc(50, 0x111, 55, notify(200, 1), &input) != 0 ||
            trace.ship_command != 1)
        return 1;
    input.selected_kind = 1;
    if (stars_planet_wnd_proc(50, 0x111, 55, notify(200, 1), &input) != 0 ||
            trace.dropdown != 1)
        return 1;
    if (stars_planet_wnd_proc(50, 0x111, 55, notify(104, 0), &input) != 0 ||
            stars_planet_wnd_proc(50, 0x111, 55, notify(105, 0), &input) != 0 ||
            trace.select_base != 2 || trace.select_planet != 1)
        return 1;
    trace.object_kind = 1;
    if (stars_planet_wnd_proc(50, 0x111, 55, notify(100, 0), &input) != 0 ||
            trace.transfer != 1)
        return 1;
    trace.object_kind = 2;
    if (stars_planet_wnd_proc(50, 0x111, 55, notify(101, 0), &input) != 0 ||
            trace.select_planet != 2)
        return 1;
    if (stars_planet_wnd_proc(50, 0x111, 55, notify(102, 0), &input) != 0 ||
            trace.build != 1 || trace.build_point.x != 0x262)
        return 1;
    if (stars_planet_wnd_proc(50, 0x111, 55, notify(111, 0), &input) != 0 ||
            trace.production != 1)
        return 1;
    trace.action_confirm = 6;
    if (stars_planet_wnd_proc(50, 0x111, 55, notify(112, 0), &input) != 0 ||
            trace.confirm != 1 || trace.production != 2)
        return 1;
    if (stars_planet_wnd_proc(50, 0x111, 55, notify(400, 1), &input) != 0 ||
            trace.paint != 2 || trace.min_size != 1)
        return 1;
    if (stars_planet_wnd_proc(50, 0x201, 7, point(10, 20), &input) != 0 ||
            stars_planet_wnd_proc(50, 0x203, 7, point(11, 21), &input) != 0 ||
            stars_planet_wnd_proc(50, 0x204, 7, point(12, 22), &input) != 0 ||
            trace.click != 3 || trace.click_double != 1 || trace.focus < 8)
        return 1;
    if (stars_planet_wnd_proc(50, 0x222, 0, 0, &input) != 0 ||
            stars_planet_wnd_proc(50, 0x222, 1, 0, &input) != 0 ||
            trace.active != 2 || trace.active_value != 50)
        return 1;
    if (stars_planet_wnd_proc(50, 0x555, 0, 0, &input) != 0 ||
            trace.default_proc == 0)
        return 1;
    puts("PLANET_WND_PROC_CHECKS=43 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\PWP\\PWPR.TXT", "w");
        if (!report)
            return 2;
        fputs("PLANET_WND_PROC_CHECKS=43 FAILURES=0 RESULT=PASS\n", report);
        fclose(report);
    }
#endif
    return 0;
}
