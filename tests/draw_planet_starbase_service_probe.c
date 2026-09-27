#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 shield_hull;
    StarsI16 shield_player;
    StarsI16 warp_planet;
    StarsI16 destination;
    StarsI16 button_enabled;
    StarsI16 title_ok;
    StarsI16 destination_text;
    StarsI16 gauge_best;
} Trace;

static StarsI16 text_copy(StarsU8 *buffer, StarsU16 capacity,
    const char *text)
{
    StarsU16 index;
    if (capacity == 0U) return 0;
    for (index = 0U; index + 1U < capacity && text[index] != 0;
         ++index)
        buffer[index] = (StarsU8)text[index];
    buffer[index] = 0;
    return (StarsI16)index;
}

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    (void)context;
    return id == 0x343U ? (const StarsU8 *)"No base" :
        (const StarsU8 *)"Label";
}

static StarsI16 get_text(void *context, StarsU16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    (void)context;
    return text_copy(buffer, capacity, id == 0x560U ? "none" : "Label");
}

static StarsI16 draw_tile(void *context, StarsU16 hdc,
    const StarsPlanetStarbaseTile *tile, const StarsU8 *title,
    StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    (void)tile;
    trace->title_ok = strcmp((const char *)title, "Starbase") == 0;
    rect->left = 10;
    rect->top = 4;
    rect->right = 170;
    rect->bottom = 130;
    return 1;
}

static void text_out(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *buffer, StarsI16 length)
{
    (void)context; (void)hdc; (void)x; (void)y; (void)buffer; (void)length;
}

static void right_text(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *buffer, StarsI16 length, StarsI16 offset)
{
    Trace *trace = (Trace *)context;
    (void)hdc; (void)x; (void)y; (void)offset;
    if (length == 4 && strncmp((const char *)buffer, "Mars", 4) == 0)
        trace->destination_text = 1;
}

static StarsI32 extent(void *context, StarsU16 hdc, const StarsU8 *buffer,
    StarsI16 length)
{
    (void)context; (void)hdc; (void)buffer;
    return (StarsI32)(length * 2);
}

static void button(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsI16 kind, StarsI16 enabled)
{
    Trace *trace = (Trace *)context;
    (void)hdc; (void)rect; (void)kind;
    trace->button_enabled = enabled;
}

static void gauge(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsI16 value, StarsI16 best)
{
    Trace *trace = (Trace *)context;
    (void)hdc; (void)rect; (void)value;
    trace->gauge_best = best;
}

static StarsI32 shield(void *context, StarsI16 hull, StarsI16 player)
{
    Trace *trace = (Trace *)context;
    trace->shield_hull = hull;
    trace->shield_player = player;
    return 25;
}

static StarsI16 warp(void *context, StarsI16 planet, StarsI16 *sign)
{
    Trace *trace = (Trace *)context;
    trace->warp_planet = planet;
    *sign = 0;
    return 5;
}

static StarsI16 get_planet_name(void *context, StarsI16 planet,
    StarsU8 *buffer, StarsU16 capacity)
{
    Trace *trace = (Trace *)context;
    trace->destination = planet;
    return text_copy(buffer, capacity, "Mars");
}

static StarsI16 format_value(void *context, StarsU8 *buffer,
    StarsU16 capacity, StarsI32 value, StarsI16 mode)
{
    (void)context; (void)capacity; (void)mode;
    buffer[0] = (StarsU8)('0' + (value % 10L));
    buffer[1] = 0;
    return 1;
}

int main(void)
{
    StarsPlanetStarbaseTile tile;
    StarsPlanetStarbaseData data;
    StarsPlanetStarbaseOps ops;
    StarsPlanetStarbaseInput input;
    StarsRect references[17];
    Trace trace;
    int checks = 0;
    int failures = 0;

#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    memset(&tile, 0, sizeof tile);
    memset(&data, 0, sizeof data);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(references, 0, sizeof references);
    memset(&trace, 0, sizeof trace);
    strcpy((char *)data.hull_class, "Starbase");
    data.has_starbase = 1;
    data.hull_id = 2;
    data.cargo_capacity = 20;
    data.defense = 15;
    data.mass_warp_packed = 80;
    data.planet_id = 4;
    data.player_id = 7;
    data.destination_id = 2;
    ops.compressed_string = compressed;
    ops.get_text = get_text;
    ops.draw_tile = draw_tile;
    ops.text_out = text_out;
    ops.right_text = right_text;
    ops.text_extent = extent;
    ops.draw_button = button;
    ops.draw_gauge = gauge;
    ops.shield_value = shield;
    ops.warp_value = warp;
    ops.get_planet_name = get_planet_name;
    ops.format_value = format_value;
    ops.context = &trace;
    input.hdc = 3;
    input.line_height = 10;
    input.normal_font = 1;
    input.bold_font = 2;
    input.highlight_color = 0x7f;
    input.button_enabled = 1;
    input.tile = &tile;
    input.data = &data;
    input.reference_rects = references;
    input.ops = &ops;
    stars_draw_planet_starbase(&input);
    CHECK(trace.title_ok == 1);
    CHECK(trace.shield_hull == 2 && trace.shield_player == 7);
    CHECK(trace.warp_planet == 4 && trace.destination == 2);
    CHECK(trace.destination_text == 1);
    CHECK(trace.button_enabled == 1 && trace.gauge_best == 4);
    CHECK(references[13].left == 14 && references[15].left == 68);
    CHECK(references[15].right == 166 && references[16].left == 14);
    (void)checks;
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\DPSBN\\DSBSVR.TXT", "w");
        if (!report)
            return 1;
        if (failures == 0)
            fputs("DRAW_PLANET_STARBASE_SERVICE_CHECKS=7 FAILURES=0 RESULT=PASS\n",
                report);
        else
            fputs("DRAW_PLANET_STARBASE_SERVICE_CHECKS=7 FAILURES=1 RESULT=FAIL\n",
                report);
        fclose(report);
    }
#else
    if (failures == 0)
        puts("DRAW_PLANET_STARBASE_SERVICE_CHECKS=7 FAILURES=0 RESULT=PASS");
    else
        puts("DRAW_PLANET_STARBASE_SERVICE_CHECKS=7 FAILURES=1 RESULT=FAIL");
#endif
    return failures != 0;
#undef CHECK
}
