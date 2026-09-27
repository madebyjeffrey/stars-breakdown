#include <stdio.h>
#include "core.h"

typedef struct Trace {
    StarsU16 selects;
    StarsU16 colors;
    StarsU16 blits;
    StarsU16 sources[4];
    StarsI16 xs[4];
    StarsU32 rops[4];
    StarsU32 last_color;
} Trace;

static StarsU16 select_object(void *value, StarsU16 hdc, StarsU16 object)
{
    Trace *trace = (Trace *)value;
    (void)hdc;
    ++trace->selects;
    return object == 77U ? 88U : 77U;
}

static void set_color(void *value, StarsU16 hdc, StarsU32 color)
{
    Trace *trace = (Trace *)value;
    (void)hdc;
    ++trace->colors;
    trace->last_color = color;
}

static void bit_blt(void *value, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 source, StarsI16 source_x,
    StarsI16 source_y, StarsU32 rop)
{
    Trace *trace = (Trace *)value;
    StarsU16 n;
    (void)hdc;
    (void)y;
    (void)width;
    (void)height;
    (void)source;
    (void)source_y;
    n = trace->blits;
    if (n < 4U) {
        trace->xs[n] = x;
        trace->sources[n] = (StarsU16)source_x;
        trace->rops[n] = rop;
    }
    ++trace->blits;
}

static StarsU32 player_color(void *value, StarsI16 player)
{
    (void)value;
    return player == 2 ? 0x00010203UL : 0x00ffffffUL;
}

int main(void)
{
    StarsShipsScanFleet fleets[2];
    StarsDrawScanFleetCountInput input;
    Trace trace;
    StarsU16 index;
    StarsU16 slot;

    for (index = 0; index < 2U; ++index) {
        fleets[index].player = index == 0U ? 2 : 3;
        fleets[index].cord = 1;
        fleets[index].order_flags = 2;
        fleets[index].overlap_flags = 0;
        for (slot = 0; slot < 16U; ++slot) {
            fleets[index].ship_counts[slot] = 0;
            fleets[index].ship_designs[slot] = 0;
        }
        fleets[index].ship_counts[0] = index == 0U ? 24 : 0;
    }
    input.fleets = fleets;
    input.fleet_count = 2;
    input.scan.scan_flags = 0x2000U;
    input.scan.local_player = 1;
    input.scan.own_ship_mask = 0;
    input.scan.enemy_category_mask = 0;
    input.scan.hull_category = 0;
    input.scan.context = 0;
    input.numbers_bitmap = 77;
    input.select_object = select_object;
    input.set_color = set_color;
    input.bit_blt = bit_blt;
    input.player_color = player_color;
    input.context = &trace;
    trace.selects = 0;
    trace.colors = 0;
    trace.blits = 0;
    trace.last_color = 0;
    if (stars_draw_scan_fleet_count(20, 30, 4, 5, &input),
        trace.blits != 4U || trace.selects != 2U || trace.colors != 6U ||
        trace.sources[0] != 8U || trace.sources[1] != 8U ||
        trace.sources[2] != 16U || trace.sources[3] != 16U ||
        trace.xs[0] != 16 || trace.xs[1] != 16 ||
        trace.xs[2] != 21 || trace.xs[3] != 21 ||
        fleets[0].overlap_flags != 0x0800U)
        return 1;
    trace.blits = 0;
    trace.selects = 0;
    trace.colors = 0;
    input.scan.scan_flags = 0;
    input.player_color = 0;
    fleets[0].ship_counts[0] = 0;
    fleets[1].ship_counts[0] = 5;
    if (stars_draw_scan_fleet_count(20, 30, 4, 5, &input),
        trace.blits != 1U || trace.selects != 2U || trace.colors != 2U)
        return 2;
    puts("DRAW_SCAN_FLEET_COUNT_SOURCE=PASS");
    return 0;
}
