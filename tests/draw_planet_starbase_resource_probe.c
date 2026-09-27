#include <stdio.h>
#include <string.h>
#include "core.h"

struct ResourceProbe {
    StarsU16 ids[8];
    StarsU16 calls;
    StarsU16 bad;
};

static const char *resource_text(StarsU16 id)
{
    switch (id) {
    case 0x344: return "Dock Capacity";
    case 0x345: return "Armor";
    case 0x346: return "Shields";
    case 0x347: return "Damage";
    case 0x349: return "Mass Driver";
    case 0x475: return "Destination";
    case 0x560: return "none";
    case 0x2ed: return "Unlimited";
    default: return 0;
    }
}

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    (void)context;
    return id == 0x343U ? (const StarsU8 *)"< no starbase >" :
        (const StarsU8 *)"Starbase";
}

static StarsI16 get_text(void *context, StarsU16 id, StarsU8 *output,
    StarsU16 capacity)
{
    struct ResourceProbe *probe = (struct ResourceProbe *)context;
    const char *text = resource_text(id);
    StarsI16 length;
    if (probe->calls < 8U)
        probe->ids[probe->calls] = id;
    ++probe->calls;
    if (!text) {
        ++probe->bad;
        output[0] = 0;
        return 0;
    }
    length = (StarsI16)strlen(text);
    if (capacity != 0U) {
        if (length >= (StarsI16)capacity)
            length = (StarsI16)(capacity - 1U);
        memcpy(output, text, (size_t)length);
        output[length] = 0;
    }
    return length;
}

static StarsI16 draw_tile(void *context, StarsU16 hdc,
    const StarsPlanetStarbaseTile *tile, const StarsU8 *title, StarsRect *rect)
{
    (void)context; (void)hdc; (void)tile; (void)title;
    rect->left = 10U; rect->top = 4U; rect->right = 170U; rect->bottom = 130U;
    return 1;
}

static void text_out(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    (void)context; (void)hdc; (void)x; (void)y; (void)text; (void)length;
}

static void right_text(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length, StarsI16 offset)
{
    (void)context; (void)hdc; (void)x; (void)y; (void)text;
    (void)length; (void)offset;
}

static StarsI16 format_value(void *context, StarsU8 *output, StarsU16 capacity,
    StarsI32 value, StarsI16 mode)
{
    (void)context; (void)mode;
    if (capacity > 1U) {
        output[0] = (StarsU8)('0' + (value % 10L));
        output[1] = 0;
    }
    return 1;
}

static StarsI32 shield_value(void *context, StarsI16 hull, StarsI16 planet)
{
    (void)context; (void)hull; (void)planet;
    return 25L;
}

static StarsI16 warp_value(void *context, StarsI16 planet, StarsI16 *negative)
{
    (void)context; (void)planet;
    *negative = 0;
    return 5;
}

int main(void)
{
    struct ResourceProbe probe;
    StarsPlanetStarbaseTile tile;
    StarsPlanetStarbaseData data;
    StarsPlanetStarbaseOps ops;
    StarsPlanetStarbaseInput input;
    StarsRect references[17];
    const StarsU16 special_ids[2] = {0x560U, 0x2edU};
    const char *special_text[2] = {"none", "Unlimited"};
    StarsU8 buffer[32];
    int checks;
    int failures;
    StarsI16 i;

    memset(&probe, 0, sizeof probe);
    memset(&tile, 0, sizeof tile);
    memset(&data, 0, sizeof data);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(references, 0, sizeof references);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    for (i = 0; i < 2; ++i) {
        StarsI16 length = get_text(&probe, special_ids[i], buffer,
            (StarsU16)sizeof buffer);
        CHECK(length == (StarsI16)strlen(special_text[i]) &&
            strcmp((char *)buffer, special_text[i]) == 0);
    }
    data.has_starbase = 1;
    data.hull_id = 2;
    data.cargo_capacity = 20U;
    data.defense = 15U;
    data.mass_warp_packed = 80U;
    data.planet_id = 4;
    tile.flags = 0x0800U;
    ops.compressed_string = compressed;
    ops.get_text = get_text;
    ops.draw_tile = draw_tile;
    ops.text_out = text_out;
    ops.right_text = right_text;
    ops.format_value = format_value;
    ops.shield_value = shield_value;
    ops.warp_value = warp_value;
    ops.context = &probe;
    input.hdc = 3U;
    input.line_height = 10;
    input.tile = &tile;
    input.data = &data;
    input.reference_rects = references;
    input.ops = &ops;
    stars_draw_planet_starbase(&input);
    CHECK(probe.bad == 0U && probe.calls == 8U);
    CHECK(probe.ids[2] == 0x344U && probe.ids[3] == 0x345U);
    CHECK(probe.ids[4] == 0x346U && probe.ids[5] == 0x347U);
    CHECK(probe.ids[6] == 0x349U && probe.ids[7] == 0x475U);
    CHECK(checks == 7);
    printf("DRAW_PLANET_STARBASE_RESOURCE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures ? 1 : 0;
#undef CHECK
}
