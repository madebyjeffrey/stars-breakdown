#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 draws, hides, shows, positions, texts, rights, references;
    StarsU16 gauges, diamonds, colors, client_rects;
    StarsU16 formatted;
    StarsI16 last_position_flags;
    StarsI16 last_fuel_waypoint;
} Trace;

static StarsI16 get_text(void *c, StarsU16 id, StarsU8 *out, StarsU16 cap)
{ (void)c; (void)id; (void)cap; strcpy((char *)out, "TX"); return 2; }
static StarsI16 draw_tile(void *c, StarsU16 hdc, StarsU16 flags,
    const StarsU8 *title, StarsRect *r)
{ Trace *t = (Trace *)c; (void)hdc; (void)flags; (void)title; ++t->draws;
  r->left = 10; r->top = 20; r->right = 110; r->bottom = 120; return 1; }
static StarsI32 extent(void *c, StarsU16 hdc, const StarsU8 *text, StarsI16 n)
{ (void)c; (void)hdc; (void)text; return (StarsI32)(n * 2); }
static void text_out(void *c, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 n)
{ Trace *t = (Trace *)c; (void)hdc; (void)x; (void)y; (void)text; (void)n;
  ++t->texts; }
static void right_text(void *c, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 n, StarsI16 off)
{ Trace *t = (Trace *)c; (void)hdc; (void)x; (void)y; (void)text; (void)n;
  (void)off; ++t->rights; }
static void font(void *c, StarsU16 hdc, StarsU16 f)
{ (void)c; (void)hdc; (void)f; }
static void fill(void *c, StarsU16 hdc, const StarsRect *r, StarsU16 b)
{ (void)c; (void)hdc; (void)r; (void)b; }
static void color(void *c, StarsU16 hdc, StarsU32 value)
{ Trace *t = (Trace *)c; (void)hdc; (void)value; ++t->colors; }
static StarsI16 location(void *c, StarsI16 n, const StarsShipOrdersWaypoint *w,
    StarsU8 *out, StarsU16 cap)
{ (void)c; (void)n; (void)w; (void)cap; strcpy((char *)out, "LOC"); return 3; }
static StarsI16 distance_text(void *c, const StarsShipOrdersWaypoint *a,
    const StarsShipOrdersWaypoint *b, StarsU8 *out, StarsU16 cap)
{ (void)c; (void)a; (void)b; (void)cap; strcpy((char *)out, "D"); return 1; }
static StarsI16 eta(void *c, const void *fleet, StarsI16 n, StarsU8 *out,
    StarsU16 cap)
{ (void)c; (void)fleet; (void)n; (void)cap; strcpy((char *)out, "E"); return 1; }
static StarsI16 warp(void *c, const void *fleet, StarsI16 n)
{ (void)c; (void)fleet; (void)n; return 6; }
static StarsI32 fuel(void *c, const void *fleet, StarsI16 n)
{ Trace *t = (Trace *)c; (void)fleet; t->last_fuel_waypoint = n; return 20; }
static StarsI16 ideal(void *c, const void *fleet, StarsI16 n)
{ (void)c; (void)fleet; (void)n; return 6; }
static StarsI32 available(void *c, const void *fleet)
{ (void)c; (void)fleet; return 100; }
static StarsI16 format_number(void *c, StarsU8 *out, StarsU16 cap,
    StarsU16 id, StarsI32 n)
{ Trace *t = (Trace *)c; (void)cap; (void)id; (void)n;
  ++t->formatted; strcpy((char *)out, "N"); return 1; }
static void gauge(void *c, StarsU16 hdc, const StarsRect *r, const void *fleet)
{ Trace *t = (Trace *)c; (void)hdc; (void)r; (void)fleet; ++t->gauges; }
static void reference(void *c, StarsU16 n, const StarsRect *r)
{ Trace *t = (Trace *)c; (void)n; (void)r; ++t->references; }
static void report(void *c, StarsU16 window, StarsU16 selection)
{ (void)c; (void)window; (void)selection; }
static void diamond(void *c, StarsU16 hdc, const StarsRect *r, StarsU16 b)
{ Trace *t = (Trace *)c; (void)hdc; (void)r; (void)b; ++t->diamonds; }
static void set_pos(void *c, StarsU16 w, StarsI16 x, StarsI16 y, StarsI16 dx,
    StarsI16 dy, StarsU16 flags)
{ Trace *t = (Trace *)c; (void)w; (void)x; (void)y; (void)dx; (void)dy;
  ++t->positions; t->last_position_flags = (StarsI16)flags; }
static void show(void *c, StarsU16 w, StarsI16 value)
{ Trace *t = (Trace *)c; (void)w; if (value == 0) ++t->hides; else ++t->shows; }
static StarsI16 client_height(void *c, StarsU16 w)
{ Trace *t = (Trace *)c; (void)w; ++t->client_rects; return 48; }

int main(void)
{
    StarsDrawShipOrdersInput i;
    StarsShipOrdersWaypoint w[2];
    Trace t;
    memset(&i, 0, sizeof i); memset(&t, 0, sizeof t);
    memset(w, 0, sizeof w);
    i.tile_flags = 0x0800; i.arial8_height = 10; i.ship_dropdown_height = 12;
    i.ship_list = 11; i.report_combo = 12; i.active_waypoint = 1; i.cord = 2;
    i.report_selection = 1; i.fuel_available = 100; i.waypoints = w;
    i.waypoint_count = 2;
    i.ops.get_text = get_text; i.ops.draw_tile = draw_tile;
    i.ops.text_extent = extent; i.ops.text_out = text_out;
    i.ops.right_text = right_text; i.ops.select_font = font;
    i.ops.fill_rect = fill; i.ops.set_text_color = color;
    i.ops.get_location = location; i.ops.get_distance = distance_text;
    i.ops.get_eta = eta; i.ops.get_warp = warp; i.ops.fuel_use = fuel;
    i.ops.find_ideal_warp = ideal; i.ops.fuel_available = available;
    i.ops.format_number = format_number; i.ops.draw_gauge = gauge;
    i.ops.set_reference = reference; i.ops.send_report = report;
    i.ops.draw_diamond = diamond; i.ops.set_window_pos = set_pos;
    i.ops.show_window = show; i.ops.get_client_height = client_height;
    i.context = &t;
    stars_draw_ship_orders(&i);
    if (t.draws != 1 || t.hides != 2 || t.shows != 2 || t.positions != 2 ||
            t.texts != 5 || t.rights != 4 || t.references != 6 ||
            t.gauges != 1 || t.diamonds != 1 || t.client_rects != 2 ||
            t.formatted != 1 || t.last_fuel_waypoint != 1)
        return 1;
    puts("DRAW_SHIP_ORDERS_SOURCE=PASS");
    return 0;
}
