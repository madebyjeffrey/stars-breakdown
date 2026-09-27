#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct LookupTrace {
    unsigned char planet[56];
    StarsI16 loads;
    StarsI16 tile_calls;
    StarsI16 values;
    StarsI16 saw_id;
    StarsI32 saw_mineral_rate;
    StarsI16 saw_scanner;
    StarsI16 saw_defense;
} LookupTrace;

static unsigned short get16(const unsigned char *raw, int offset)
{
    return (unsigned short)(raw[offset] |
        ((unsigned short)raw[offset + 1] << 8));
}

static unsigned long get32(const unsigned char *raw, int offset)
{
    return (unsigned long)get16(raw, offset) |
        ((unsigned long)get16(raw, offset + 2) << 16);
}

static StarsI16 load_data(void *context, const void *raw,
    StarsPlanetStatsData *data)
{
    LookupTrace *trace = (LookupTrace *)context;
    const unsigned char *planet = (const unsigned char *)raw;
    unsigned long services;

    ++trace->loads;
    if (raw != trace->planet)
        return 0;
    services = get32(planet, 24);
    memset(data, 0, sizeof *data);
    data->mineral_rate_x100 = (StarsI32)(get32(planet, 40) * 100UL);
    data->resources_available = 55;
    data->resources_total = 70;
    data->scanner_range = (StarsI16)((services >> 12) & 0x1fUL);
    data->defense_capacity = (StarsI16)(services & 0x0fffUL);
    data->scanner_available = 1;
    data->best_defense_available = 1;
    data->survival_percent = 88;
    return 1;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc, const void *tile,
    StarsRect *rect)
{
    LookupTrace *trace = (LookupTrace *)context;
    (void)hdc;
    (void)tile;
    ++trace->tile_calls;
    rect->left = 10;
    rect->top = 10;
    rect->right = 210;
    rect->bottom = 100;
    return 1;
}

static StarsI16 measure_label(void *context, StarsU16 hdc, StarsI16 label)
{
    (void)context;
    (void)hdc;
    (void)label;
    return 15;
}

static void select_font(void *context, StarsU16 hdc, StarsI16 font)
{
    (void)context;
    (void)hdc;
    (void)font;
}

static void draw_label(void *context, StarsU16 hdc, StarsI16 label,
    StarsI16 x, StarsI16 y)
{
    (void)context;
    (void)hdc;
    (void)label;
    (void)x;
    (void)y;
}

static void draw_value(void *context, StarsU16 hdc, StarsI16 field,
    StarsI16 right, StarsI16 value_offset, StarsI16 y,
    const StarsPlanetStatsData *data)
{
    LookupTrace *trace = (LookupTrace *)context;
    (void)hdc;
    (void)right;
    (void)value_offset;
    (void)y;
    ++trace->values;
    if (field == 0)
        trace->saw_mineral_rate = data->mineral_rate_x100;
    if (field == 2)
        trace->saw_scanner = data->scanner_range;
    if (field == 4)
        trace->saw_defense = data->defense_capacity;
    trace->saw_id = get16(trace->planet, 0);
}

static void set_reference(void *context, StarsI16 id, const StarsRect *rect)
{
    (void)context;
    (void)id;
    (void)rect;
}

static void draw_separator(void *context, StarsU16 hdc, StarsI16 y,
    StarsI16 left, StarsI16 right)
{
    (void)context;
    (void)hdc;
    (void)y;
    (void)left;
    (void)right;
}

static void install(StarsPlanetStatsOps *ops, LookupTrace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->select_font = select_font;
    ops->measure_label = measure_label;
    ops->draw_label = draw_label;
    ops->draw_value = draw_value;
    ops->set_reference = set_reference;
    ops->draw_separator = draw_separator;
    ops->load_data = load_data;
    ops->context = trace;
}

int main(void)
{
    LookupTrace trace;
    StarsPlanetStatsInput input;
    StarsPlanetStatsOps ops;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    memset(&trace, 0, sizeof trace);
    memset(&input, 0, sizeof input);
    trace.planet[0] = 4;
    trace.planet[4] = 7;
    trace.planet[24] = 77;
    trace.planet[25] = 192;
    trace.planet[40] = 13;
    install(&ops, &trace);
    input.hdc = 5;
    input.tile = trace.planet;
    input.line_height = 8;
    input.font_bold = 1;
    input.font_normal = 0;
    input.ops = &ops;
    stars_draw_planet_stats(&input);
    CHECK(trace.loads == 1);
    CHECK(trace.tile_calls == 1);
    CHECK(trace.values == 7);
    CHECK(input.data.mineral_rate_x100 == 1300L);
    CHECK(input.data.scanner_range == 12);
    CHECK(input.data.defense_capacity == 77);
    CHECK(trace.saw_id == 4 && trace.saw_mineral_rate == 1300L);
    CHECK(trace.saw_scanner == 12 && trace.saw_defense == 77);
    CHECK(checks == 9);
    puts("DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
