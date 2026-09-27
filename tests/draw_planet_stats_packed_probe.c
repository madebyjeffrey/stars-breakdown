#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedStatsContext {
    unsigned char planet[56];
    StarsPlanetStatsData data;
    StarsI16 tile_calls;
    StarsI16 measure_calls;
    StarsI16 fonts;
    StarsI16 labels;
    StarsI16 values;
    StarsI16 refs;
    StarsI16 separators;
    StarsI16 data_seen;
    StarsI32 last_mineral_rate;
    StarsI16 last_scanner_range;
    StarsI16 last_defense_capacity;
} PackedStatsContext;

static void put16(unsigned char *raw, int offset, unsigned short value)
{
    raw[offset] = (unsigned char)(value & 0xff);
    raw[offset + 1] = (unsigned char)(value >> 8);
}

static void put32(unsigned char *raw, int offset, unsigned long value)
{
    put16(raw, offset, (unsigned short)(value & 0xffffUL));
    put16(raw, offset + 2, (unsigned short)(value >> 16));
}

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

static StarsI16 draw_tile(void *context, StarsU16 hdc, const void *tile,
    StarsRect *rect)
{
    PackedStatsContext *packed = (PackedStatsContext *)context;
    (void)hdc;
    (void)tile;
    ++packed->tile_calls;
    rect->left = 10;
    rect->top = 10;
    rect->right = 210;
    rect->bottom = 100;
    return 1;
}

static void select_font(void *context, StarsU16 hdc, StarsI16 font)
{
    (void)hdc;
    (void)font;
    ++((PackedStatsContext *)context)->fonts;
}

static StarsI16 measure_label(void *context, StarsU16 hdc, StarsI16 label)
{
    (void)hdc;
    (void)label;
    ++((PackedStatsContext *)context)->measure_calls;
    return label & 7;
}

static void draw_label(void *context, StarsU16 hdc, StarsI16 label,
    StarsI16 x, StarsI16 y)
{
    (void)hdc;
    (void)label;
    (void)x;
    (void)y;
    ++((PackedStatsContext *)context)->labels;
}

static void draw_value(void *context, StarsU16 hdc, StarsI16 field,
    StarsI16 right, StarsI16 value_offset, StarsI16 y,
    const StarsPlanetStatsData *data)
{
    PackedStatsContext *packed = (PackedStatsContext *)context;
    (void)hdc;
    (void)field;
    (void)right;
    (void)value_offset;
    (void)y;
    ++packed->values;
    packed->last_mineral_rate = data->mineral_rate_x100;
    packed->last_scanner_range = data->scanner_range;
    packed->last_defense_capacity = data->defense_capacity;
    if (data->mineral_rate_x100 == packed->data.mineral_rate_x100 &&
            data->scanner_range == packed->data.scanner_range &&
            data->defense_capacity == packed->data.defense_capacity &&
            data->resources_available == 55 &&
            data->survival_percent == 88)
        packed->data_seen = 1;
}

static void set_reference(void *context, StarsI16 id, const StarsRect *rect)
{
    (void)id;
    (void)rect;
    ++((PackedStatsContext *)context)->refs;
}

static void draw_separator(void *context, StarsU16 hdc, StarsI16 y,
    StarsI16 left, StarsI16 right)
{
    (void)hdc;
    (void)y;
    (void)left;
    (void)right;
    ++((PackedStatsContext *)context)->separators;
}

static void install(StarsPlanetStatsOps *ops, PackedStatsContext *packed)
{
    memset(ops, 0, sizeof *ops);
    ops->draw_tile = draw_tile;
    ops->select_font = select_font;
    ops->measure_label = measure_label;
    ops->draw_label = draw_label;
    ops->draw_value = draw_value;
    ops->set_reference = set_reference;
    ops->draw_separator = draw_separator;
    ops->context = packed;
}

static void setup(StarsPlanetStatsPackedInput *input, StarsPlanetStatsOps *ops,
    PackedStatsContext *packed)
{
    memset(input, 0, sizeof *input);
    packed->tile_calls = 0;
    packed->measure_calls = 0;
    packed->fonts = 0;
    packed->labels = 0;
    packed->values = 0;
    packed->refs = 0;
    packed->separators = 0;
    packed->data_seen = 0;
    packed->last_mineral_rate = 0;
    packed->last_scanner_range = 0;
    packed->last_defense_capacity = 0;
    memset(&packed->data, 0, sizeof packed->data);
    packed->data.mineral_rate_x100 = (StarsI32)(get32(packed->planet, 40) *
        100UL);
    packed->data.resources_available = 55;
    packed->data.resources_total = 70;
    packed->data.scanner_range = (StarsI16)((get32(packed->planet, 24) >> 12) &
        0x1fUL);
    packed->data.scanner_width = 3;
    packed->data.defense_capacity = (StarsI16)(get32(packed->planet, 24) &
        0x0fffUL);
    packed->data.best_defense_available = 1;
    packed->data.survival_percent = 88;
    packed->data.scanner_available = 1;
    packed->data.special_scanner = 0;
    install(ops, packed);
    input->planet_record = packed->planet;
    input->planet_size = (StarsU16)sizeof packed->planet;
    input->resources_available = 55;
    input->resources_total = 70;
    input->scanner_width = 3;
    input->best_defense_available = 1;
    input->survival_percent = 88;
    input->scanner_available = 1;
    input->special_scanner = 0;
    input->stats.hdc = 5;
    input->stats.tile = packed->planet;
    input->stats.data = packed->data;
    input->stats.line_height = 8;
    input->stats.font_bold = 1;
    input->stats.font_normal = 0;
    input->stats.ops = ops;
}

int main(void)
{
    PackedStatsContext packed;
    StarsPlanetStatsPackedInput input;
    StarsPlanetStatsOps ops;
    int checks;
    int failures;

    memset(&packed, 0, sizeof packed);
    put16(packed.planet, 0, 4);
    put16(packed.planet, 4, 0x0207);
    put32(packed.planet, 24, (12UL << 12) | 77UL);
    put32(packed.planet, 40, 13UL);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof packed.planet == 56);
    CHECK(get16(packed.planet, 0) == 4);
    CHECK(get16(packed.planet, 4) == 0x0207);
    CHECK((get32(packed.planet, 24) & 0x0fffUL) == 77UL);
    CHECK(((get32(packed.planet, 24) >> 12) & 0x1fUL) == 12UL);
    CHECK(get32(packed.planet, 40) == 13UL);

    setup(&input, &ops, &packed);
    stars_draw_planet_stats_packed(&input);
    CHECK(packed.tile_calls == 1 && packed.measure_calls == 7);
    CHECK(packed.labels == 7 && packed.values == 7);
    CHECK(packed.refs == 4 && packed.separators == 2);
    CHECK(packed.data_seen == 1);

    input.stats.hidden = 1;
    stars_draw_planet_stats_packed(&input);
    CHECK(packed.labels == 7 && packed.values == 14);
    CHECK(packed.measure_calls == 14);
    put32(packed.planet, 24, (31UL << 12) | 4095UL);
    put32(packed.planet, 40, 0UL);
    setup(&input, &ops, &packed);
    stars_draw_planet_stats_packed(&input);
    CHECK(packed.last_mineral_rate == 0);
    CHECK(packed.last_scanner_range == 31 &&
        packed.last_defense_capacity == 4095);
    CHECK(checks == 15);
    puts("DRAW_PLANET_STATS_PACKED_CHECKS=15 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
