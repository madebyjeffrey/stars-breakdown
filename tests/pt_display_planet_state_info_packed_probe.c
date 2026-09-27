#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedStateContext {
    unsigned char planet[56];
    unsigned char popup[22];
    StarsI16 streams;
    StarsI16 wraps;
    StarsI16 ids[32];
    StarsI16 wrap_ids[8];
    StarsI16 environment_calls;
    StarsI16 desirability_calls;
} PackedStateContext;

static void put16(unsigned char *raw, int offset, unsigned short value)
{
    raw[offset] = (unsigned char)(value & 0xff);
    raw[offset + 1] = (unsigned char)(value >> 8);
}

static unsigned short get16(const unsigned char *raw, int offset)
{
    return (unsigned short)(raw[offset] |
        ((unsigned short)raw[offset + 1] << 8));
}

static StarsI16 signed16(const unsigned char *raw, int offset)
{
    return (StarsI16)get16(raw, offset);
}

static void load_data(const PackedStateContext *packed,
    StarsPlanetStateInfoData *data)
{
    StarsI16 environment_index;

    environment_index = 0;
    memset(data, 0, sizeof *data);
    data->planet_id = signed16(packed->planet, 0);
    data->planet_var = signed16(packed->popup, 4);
    data->planet_value = signed16(packed->popup, 6);
    data->player_min = signed16(packed->popup, 14);
    data->player_max = signed16(packed->popup, 16);
    data->player_value = signed16(packed->popup, 12);
    data->planet_min = signed16(packed->popup, 8);
    data->planet_max = signed16(packed->popup, 10);
    data->player_value_secondary = (StarsI16)packed->planet[15 +
        environment_index];
    data->planet_max_secondary = (StarsI16)packed->planet[16 +
        environment_index];
    data->planet_min_secondary = (StarsI16)packed->planet[15 +
        environment_index];
    data->player_value_min_secondary = (StarsI16)packed->planet[16 +
        environment_index];
    data->difference_center = signed16(packed->popup, 10);
    data->planet_min_offset = signed16(packed->popup, 4);
    data->planet_max_offset = signed16(packed->popup, 14);
    data->environment_index = environment_index;
}

static void select_font(void *context, StarsU16 hdc, StarsI16 font)
{
    (void)context;
    (void)hdc;
    (void)font;
}

static void stream(void *context, StarsU16 hdc, StarsI16 *x, StarsI16 y,
    const StarsPlanetStateInfoSegment *segment, StarsI16 print_mode)
{
    PackedStateContext *packed = (PackedStateContext *)context;
    (void)hdc;
    (void)y;
    (void)print_mode;
    if (packed->streams < 32) {
        packed->ids[packed->streams] = segment->string_id;
    }
    ++packed->streams;
    *x = (StarsI16)(*x + 100);
}

static void wrap(void *context, StarsU16 hdc, StarsI16 *x, StarsI16 *y,
    const StarsPlanetStateInfoSegment *segment, StarsI16 width,
    StarsI16 print_mode)
{
    PackedStateContext *packed = (PackedStateContext *)context;
    (void)hdc;
    (void)y;
    (void)width;
    (void)print_mode;
    if (packed->wraps < 8)
        packed->wrap_ids[packed->wraps] = segment->string_id;
    ++packed->wraps;
    *x = (StarsI16)(*x + 2);
}

static StarsI16 environment(void *context, StarsI16 variable, StarsI16 value,
    StarsI16 planet_id, StarsI16 environment_index)
{
    PackedStateContext *packed = (PackedStateContext *)context;
    (void)variable;
    (void)planet_id;
    (void)environment_index;
    ++packed->environment_calls;
    return (StarsI16)(value + 1);
}

static StarsI16 desirability(void *context, StarsI16 planet_id,
    StarsI16 environment_index, StarsI16 current_value, StarsI16 candidate)
{
    PackedStateContext *packed = (PackedStateContext *)context;
    (void)planet_id;
    (void)environment_index;
    (void)current_value;
    ++packed->desirability_calls;
    return candidate > 10 ? 20 : 5;
}

static void install(StarsPlanetStateInfoOps *ops,
    PackedStateContext *packed)
{
    memset(ops, 0, sizeof *ops);
    ops->select_font = select_font;
    ops->stream_text = stream;
    ops->wrap_text = wrap;
    ops->environment = environment;
    ops->desirability = desirability;
    ops->context = packed;
}

static void setup(StarsPlanetStateInfoInput *input,
    StarsPlanetStateInfoOps *ops, PackedStateContext *packed)
{
    memset(input, 0, sizeof *input);
    packed->streams = 0;
    packed->wraps = 0;
    packed->environment_calls = 0;
    packed->desirability_calls = 0;
    memset(packed->ids, 0, sizeof packed->ids);
    memset(packed->wrap_ids, 0, sizeof packed->wrap_ids);
    install(ops, packed);
    input->hdc = 1;
    input->print_mode = 1;
    input->line_height = 8;
    input->font_normal = 2;
    input->font_bold = 3;
    load_data(packed, &input->data);
    input->data.planet_id = 99;
    input->packed_planet_record = packed->planet;
    input->packed_planet_size = (StarsU16)sizeof packed->planet;
    input->packed_popupdata = packed->popup;
    input->packed_popupdata_size = (StarsU16)sizeof packed->popup;
    input->packed_environment_index = 0;
    input->ops = ops;
}

int main(void)
{
    PackedStateContext packed;
    StarsPlanetStateInfoInput input;
    StarsPlanetStateInfoOps ops;
    StarsPoint point;
    int checks;
    int failures;

    memset(&packed, 0, sizeof packed);
    put16(packed.planet, 0, 4);
    put16(packed.planet, 2, 2);
    packed.planet[12] = 4;
    packed.planet[15] = 1;
    packed.planet[16] = 2;
    put16(packed.popup, 4, 2);
    put16(packed.popup, 6, 4);
    put16(packed.popup, 8, 3);
    put16(packed.popup, 10, 12);
    put16(packed.popup, 12, 7);
    put16(packed.popup, 14, 5);
    put16(packed.popup, 16, 15);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof packed.planet == 56);
    CHECK(sizeof packed.popup == 22);
    CHECK(signed16(packed.planet, 0) == 4);
    CHECK(signed16(packed.popup, 4) == 2);
    CHECK(signed16(packed.popup, 6) == 4);
    CHECK(signed16(packed.popup, 8) == 3);
    CHECK(signed16(packed.popup, 10) == 12);
    CHECK(signed16(packed.popup, 12) == 7);
    CHECK(signed16(packed.popup, 14) == 5);
    CHECK(signed16(packed.popup, 16) == 15);
    CHECK(packed.planet[12] == 4 && packed.planet[15] == 1);

    setup(&input, &ops, &packed);
    point = stars_display_planet_state_info(&input);
    CHECK(input.data.planet_id == 4 && input.data.planet_value == 4);
    CHECK(input.data.player_min == 5 && input.data.planet_max == 12);
    CHECK(packed.streams == 21 && packed.wraps == 6);
    CHECK(packed.ids[1] == 0x253 && packed.ids[4] == 599);
    CHECK(packed.ids[11] == 0x25a && packed.wrap_ids[0] == 0x166);
    CHECK(packed.wrap_ids[5] == 0x168);
    CHECK(packed.environment_calls > 4 && packed.desirability_calls == 2);
    CHECK(point.x > 100 && point.y > 4);

    put16(packed.popup, 6, 0xffff);
    put16(packed.popup, 14, 0xffff);
    put16(packed.popup, 8, 0xffff);
    setup(&input, &ops, &packed);
    point = stars_display_planet_state_info(&input);
    CHECK(packed.streams == 8 && packed.wraps == 0);
    CHECK(packed.ids[2] == 0x254 && packed.ids[4] == 0x255);
    CHECK(point.y == 32);
    setup(&input, &ops, &packed);
    input.packed_popupdata_size = 21;
    input.data.planet_id = 77;
    point = stars_display_planet_state_info(&input);
    CHECK(input.data.planet_id == 77 && point.x > 4);
    CHECK(input.data.planet_value == -1);
    CHECK(input.data.player_min == -1);
    if (checks != 25)
        ++failures;
    puts("PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
