#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedPopContext {
    unsigned char raw[56];
    StarsI16 selected_player;
    StarsI16 habitability_class;
    StarsI32 max_population;
    StarsI16 desirability_tenths;
    StarsI16 foreign_environment_class;
    StarsI16 lookup_calls;
    StarsI16 lookup_id;
    StarsI16 streams;
    StarsI16 wraps;
    StarsI16 ids[32];
    StarsI16 wrap_ids[8];
} PackedPopContext;

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

static StarsI16 lookup(void *context, StarsI16 id, StarsPlanetPopInfo *info)
{
    PackedPopContext *packed = (PackedPopContext *)context;
    ++packed->lookup_calls;
    packed->lookup_id = id;
    if ((StarsI16)get16(packed->raw, 0) != id)
        return 0;
    info->owner = (StarsI16)get16(packed->raw, 2);
    info->selected_player = packed->selected_player;
    info->mineral_rate = (StarsI32)get32(packed->raw, 28);
    info->habitability_class = packed->habitability_class;
    info->max_population = packed->max_population;
    info->desirability_tenths = packed->desirability_tenths;
    info->population_change = (StarsI32)(signed char)(get32(packed->raw, 20) &
        0xffUL);
    info->foreign_environment_class = packed->foreign_environment_class;
    return 1;
}

static void select_font(void *context, StarsU16 hdc, StarsI16 font)
{
    (void)context;
    (void)hdc;
    (void)font;
}

static void stream(void *context, StarsU16 hdc, StarsI16 *x, StarsI16 y,
    const StarsPlanetPopSegment *segment, StarsI16 print_mode)
{
    PackedPopContext *packed = (PackedPopContext *)context;
    (void)hdc;
    (void)y;
    (void)print_mode;
    if (packed->streams < 32)
        packed->ids[packed->streams] = segment->string_id;
    ++packed->streams;
    *x = (StarsI16)(*x + 2);
}

static void wrap(void *context, StarsU16 hdc, StarsI16 *x, StarsI16 *y,
    const StarsPlanetPopSegment *segment, StarsI16 width,
    StarsI16 print_mode)
{
    PackedPopContext *packed = (PackedPopContext *)context;
    (void)hdc;
    (void)y;
    (void)width;
    (void)print_mode;
    if (packed->wraps < 8)
        packed->wrap_ids[packed->wraps] = segment->string_id;
    ++packed->wraps;
    *x = (StarsI16)(*x + 2);
}

static StarsI32 max_population(void *context, StarsI16 id, StarsI16 player)
{
    PackedPopContext *packed = (PackedPopContext *)context;
    (void)id;
    (void)player;
    return packed->max_population;
}

static StarsI16 desirability(void *context, StarsI16 id, StarsI16 player)
{
    PackedPopContext *packed = (PackedPopContext *)context;
    (void)id;
    (void)player;
    return packed->desirability_tenths;
}

static StarsI32 change_population(void *context, StarsI16 id)
{
    PackedPopContext *packed = (PackedPopContext *)context;
    (void)id;
    return (StarsI32)(signed char)(get32(packed->raw, 20) & 0xffUL);
}

static void install(StarsPlanetPopInfoOps *ops, PackedPopContext *packed)
{
    memset(ops, 0, sizeof *ops);
    ops->lookup_planet = lookup;
    ops->select_font = select_font;
    ops->stream_text = stream;
    ops->wrap_text = wrap;
    ops->max_population = max_population;
    ops->desirability = desirability;
    ops->change_population = change_population;
    ops->context = packed;
}

static void setup(StarsPlanetPopInfoInput *input, StarsPlanetPopInfoOps *ops,
    PackedPopContext *packed)
{
    memset(input, 0, sizeof *input);
    packed->lookup_calls = 0;
    packed->lookup_id = 0;
    packed->streams = 0;
    packed->wraps = 0;
    memset(packed->ids, 0, sizeof packed->ids);
    memset(packed->wrap_ids, 0, sizeof packed->wrap_ids);
    install(ops, packed);
    input->hdc = 1;
    input->planet_id = 99;
    input->packed_planet_record = packed->raw;
    input->packed_planet_size = (StarsU16)sizeof packed->raw;
    input->packed_selected_player = packed->selected_player;
    input->packed_habitability_class = packed->habitability_class;
    input->packed_max_population = packed->max_population;
    input->packed_desirability_tenths = packed->desirability_tenths;
    input->packed_foreign_environment_class =
        packed->foreign_environment_class;
    input->line_height = 8;
    input->font_bold = 1;
    input->viewer_player = 0;
    input->ops = ops;
}

int main(void)
{
    PackedPopContext packed;
    StarsPlanetPopInfoInput input;
    StarsPlanetPopInfoOps ops;
    StarsPoint point;
    int checks;
    int failures;

    memset(&packed, 0, sizeof packed);
    put16(packed.raw, 0, 7);
    put16(packed.raw, 2, 0);
    put16(packed.raw, 4, 0x0207);
    put16(packed.raw, 18, 100);
    put32(packed.raw, 20, 5);
    put32(packed.raw, 28, 10);
    packed.selected_player = 0;
    packed.habitability_class = 4;
    packed.max_population = 100;
    packed.desirability_tenths = 10;
    packed.foreign_environment_class = 0;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof packed.raw == 56);
    CHECK(get16(packed.raw, 0) == 7);
    CHECK(get16(packed.raw, 2) == 0);
    CHECK(get16(packed.raw, 4) == 0x0207);
    CHECK((signed char)(get32(packed.raw, 20) & 0xffUL) == 5);
    CHECK((StarsI32)get32(packed.raw, 28) == 10);

    setup(&input, &ops, &packed);
    point = stars_display_planet_pop_info(&input);
    CHECK(input.planet_id == 7);
    CHECK(packed.lookup_calls == 0);
    CHECK(packed.streams == 8 && packed.wraps == 3);
    CHECK(packed.ids[0] == 0x244 && packed.ids[2] == 0x24f);
    CHECK(packed.ids[5] == 0x24c && packed.wrap_ids[2] == 0x213);
    CHECK(point.x > 4 && point.y > 4);

    put16(packed.raw, 2, 0xffff);
    packed.habitability_class = 2;
    setup(&input, &ops, &packed);
    point = stars_display_planet_pop_info(&input);
    CHECK(packed.lookup_calls == 0 && packed.streams == 2);
    CHECK(packed.wraps == 0 && packed.ids[1] == 0x248);
    CHECK(point.y == 16);

    put16(packed.raw, 2, 1);
    packed.habitability_class = 4;
    packed.foreign_environment_class = 0;
    packed.max_population = 20;
    packed.desirability_tenths = 5;
    setup(&input, &ops, &packed);
    stars_display_planet_pop_info(&input);
    CHECK(packed.ids[0] == 0x245 && packed.ids[2] == 0x246);
    CHECK(packed.ids[4] == 0xc2b && packed.wraps == 2);

    put16(packed.raw, 2, 0);
    packed.habitability_class = 4;
    packed.desirability_tenths = -25;
    setup(&input, &ops, &packed);
    stars_display_planet_pop_info(&input);
    CHECK(packed.streams == 9 && packed.ids[5] == 0x249);
    CHECK(packed.ids[8] == 0x24a && packed.wraps == 0);
    setup(&input, &ops, &packed);
    input.packed_planet_size = 55;
    input.planet_id = 7;
    stars_display_planet_pop_info(&input);
    CHECK(packed.lookup_calls == 1 && packed.lookup_id == 7);
    CHECK(packed.streams > 0);
    if (checks != 21)
        ++failures;
    puts("PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=21 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
