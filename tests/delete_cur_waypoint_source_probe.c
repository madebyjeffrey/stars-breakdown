#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    int beep;
    int redraw;
    int lookup;
    int find;
    int change;
    int logical;
    int bound;
    int invalidate;
    StarsI16 kind;
    StarsI16 active_seen;
    StarsU16 window;
    StarsI16 erase;
    StarsPoint points[3];
    StarsRect rect;
} Trace;

static void put_word(StarsU8 *p, StarsI16 value)
{
    p[0] = (StarsU8)value;
    p[1] = (StarsU8)((StarsU16)value >> 8);
}

static void set_order(StarsU8 *records, StarsU16 index, StarsI16 x,
    StarsI16 y, StarsU8 marker)
{
    StarsU8 *p = records + (StarsU32)index * 18UL;
    StarsU16 i;
    for (i = 0; i < 18U; ++i) p[i] = marker;
    put_word(p + 4, x);
    put_word(p + 6, y);
}

static void beep(void *v, StarsU16 type)
{ Trace *t = (Trace *)v; if (type == 0x40U) ++t->beep; }
static void redraw(void *v, StarsU16 hdc, StarsU16 visible)
{ Trace *t = (Trace *)v; if (hdc == 0U && visible == 0U) ++t->redraw; }
static void lookup(void *v, StarsI16 player, StarsU8 *fleet)
{ Trace *t = (Trace *)v; (void)fleet; if (player == -1) ++t->lookup; }
static void find_nearest(void *v, StarsPoint p, StarsU16 kind,
    StarsScanSelection *selection)
{ Trace *t = (Trace *)v; ++t->find; t->kind = (StarsI16)kind;
  t->points[0] = p; selection->x = p.x; selection->y = p.y; }
static void change(void *v, const StarsScanSelection *selection, StarsI16 valid)
{ Trace *t = (Trace *)v; if (valid == 1) { ++t->change;
  t->active_seen = selection->waypoint; } }
static void logical(void *v, StarsPoint *p)
{ Trace *t = (Trace *)v; ++t->logical; p->x = (StarsI16)(p->x + 1); p->y = (StarsI16)(p->y + 2); }
static void bound(void *v, StarsRect *r, const StarsPoint *p, StarsI16 count)
{ Trace *t = (Trace *)v; ++t->bound; t->rect.left = (StarsU16)p[0].x;
  t->rect.top = (StarsU16)p[0].y; t->rect.right = (StarsU16)(p[count-1].x + 1);
  t->rect.bottom = (StarsU16)(p[count-1].y + 1); *r = t->rect; }
static void invalidate(void *v, StarsU16 window, const StarsRect *r, StarsI16 erase)
{ Trace *t = (Trace *)v; ++t->invalidate; t->window = window; t->erase = erase; t->rect = *r; }

static int check_guard(void)
{
    StarsDeleteCurWayPointInput input;
    StarsDeleteCurWayPointOps ops;
    StarsU8 records[36], count, fleet[4]; Trace t;
    memset(&t, 0, sizeof t); memset(records, 0, sizeof records);
    ops.message_beep = beep; ops.redraw_scan_selection = redraw;
    ops.lookup_fleet = lookup; ops.find_nearest = find_nearest;
    ops.change_selection = change; ops.logical_to_scan = logical;
    ops.bound_points = bound; ops.invalidate_rect = invalidate;
    count = 2; input.cord = 1; input.active_waypoint = 0;
    input.iord_mac = &count; input.records = records; input.fleet = fleet;
    input.scanner_window = 4; input.scan_flags = 0; input.ops = &ops;
    input.context = &t; stars_delete_cur_waypoint(&input, 0);
    return t.beep == 1 && count == 2 && t.redraw == 0;
}

static int check_delete(void)
{
    StarsDeleteCurWayPointInput input; StarsDeleteCurWayPointOps ops;
    StarsU8 records[72], count = 4, fleet[4]; Trace t;
    memset(&t, 0, sizeof t); set_order(records, 0, 10, 20, 0xa0);
    set_order(records, 1, 30, 40, 0xb1); set_order(records, 2, 50, 60, 0xc2);
    set_order(records, 3, 70, 80, 0xd3);
    ops.message_beep = beep; ops.redraw_scan_selection = redraw;
    ops.lookup_fleet = lookup; ops.find_nearest = find_nearest;
    ops.change_selection = change; ops.logical_to_scan = logical;
    ops.bound_points = bound; ops.invalidate_rect = invalidate;
    input.cord = 4; input.active_waypoint = 1; input.iord_mac = &count;
    input.records = records; input.fleet = fleet; input.scanner_window = 9;
    input.scan_flags = 0x80; input.ops = &ops; input.context = &t;
    stars_delete_cur_waypoint(&input, 0);
    return input.cord == 3 && count == 3 && input.active_waypoint == -2 &&
        records[18] == 0xc2 && t.redraw == 2 && t.lookup == 1 && t.find == 1 &&
        t.change == 1 && t.kind == (StarsI16)0x8f && t.logical == 3 &&
        t.bound == 1 && t.invalidate == 1 && t.window == 9 && t.erase == 1;
}

static int check_duplicate(void)
{
    StarsDeleteCurWayPointInput input; StarsU8 records[90], count = 5;
    memset(records, 0, sizeof records); set_order(records, 0, 1, 1, 0x10);
    set_order(records, 1, 8, 9, 0x20); set_order(records, 2, 8, 9, 0x30);
    set_order(records, 3, 8, 9, 0x40); set_order(records, 4, 4, 5, 0x50);
    input.cord = 5; input.active_waypoint = 2; input.iord_mac = &count;
    input.records = records; input.fleet = 0; input.scanner_window = 0;
    input.scan_flags = 0; input.ops = 0; input.context = 0;
    stars_delete_cur_waypoint(&input, 1);
    return input.cord == 3 && count == 3 && input.active_waypoint == -2 &&
        records[18] == 0x20 && records[36] == 0x50;
}

int main(void)
{
    int failures = 0;
    if (!check_guard()) ++failures;
    if (!check_delete()) ++failures;
    if (!check_duplicate()) ++failures;
    printf("DELETE_CUR_WAYPOINT_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
