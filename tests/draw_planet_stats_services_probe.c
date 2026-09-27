#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    unsigned char planet[56];
    StarsI16 tile_calls;
    StarsI16 resolve_calls;
    StarsI16 resolved_player;
    StarsI16 values;
    StarsI16 saw_scanner;
    StarsI16 saw_defense;
    StarsI32 saw_mineral_rate;
} Trace;

static StarsI16 draw_tile(void *context, StarsU16 hdc, const void *tile,
    StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    if (tile != trace->planet)
        return 0;
    ++trace->tile_calls;
    rect->left = 10;
    rect->top = 10;
    rect->right = 210;
    rect->bottom = 100;
    return 1;
}

static void resolve_data(void *context, const void *tile, StarsI16 player,
    StarsPlanetStatsData *data)
{
    Trace *trace = (Trace *)context;
    unsigned long services;
    if (tile != trace->planet)
        return;
    ++trace->resolve_calls;
    trace->resolved_player = player;
    services = (unsigned long)trace->planet[24] |
        ((unsigned long)trace->planet[25] << 8) |
        ((unsigned long)trace->planet[26] << 16) |
        ((unsigned long)trace->planet[27] << 24);
    data->mineral_rate_x100 = (StarsI32)(((unsigned long)trace->planet[40] |
        ((unsigned long)trace->planet[41] << 8) |
        ((unsigned long)trace->planet[42] << 16) |
        ((unsigned long)trace->planet[43] << 24)) * 100UL);
    data->resources_available = 55;
    data->resources_total = 70;
    data->scanner_range = (StarsI16)((services >> 12) & 0x1fUL);
    data->defense_capacity = (StarsI16)(services & 0x0fffUL);
    data->scanner_available = 1;
    data->best_defense_available = 1;
    data->survival_percent = 88;
}

static StarsI16 measure(void *context, StarsU16 hdc, StarsI16 label)
{
    (void)context;
    (void)hdc;
    (void)label;
    return 15;
}

static void draw_value(void *context, StarsU16 hdc, StarsI16 field,
    StarsI16 right, StarsI16 offset, StarsI16 y,
    const StarsPlanetStatsData *data)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)right;
    (void)offset;
    (void)y;
    ++trace->values;
    if (field == 0)
        trace->saw_mineral_rate = data->mineral_rate_x100;
    if (field == 2)
        trace->saw_scanner = data->scanner_range;
    if (field == 4)
        trace->saw_defense = data->defense_capacity;
}

static void install(StarsPlanetStatsOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->measure_label = measure;
    ops->draw_value = draw_value;
    ops->resolve_data = resolve_data;
    ops->context = trace;
}

int main(void)
{
    Trace trace;
    StarsPlanetStatsInput input;
    StarsPlanetStatsOps ops;
    int failures;

    memset(&trace, 0, sizeof trace);
    memset(&input, 0, sizeof input);
    trace.planet[24] = 77;
    trace.planet[25] = 0xc0;
    trace.planet[40] = 13;
    install(&ops, &trace);
    input.tile = trace.planet;
    input.player = 4;
    input.line_height = 8;
    input.ops = &ops;
    stars_draw_planet_stats(&input);
    failures = trace.tile_calls != 1 || trace.resolve_calls != 1 ||
        trace.resolved_player != 4 || trace.values != 7 ||
        trace.saw_mineral_rate != 1300L || trace.saw_scanner != 12 ||
        trace.saw_defense != 77;
    printf("DRAW_PLANET_STATS_SERVICES_CHECKS=12 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
