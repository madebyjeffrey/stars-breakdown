#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 segments;
    StarsI32 total;
    StarsI32 value0;
    StarsI32 value1;
    StarsU16 brush0;
    StarsU16 format_id;
    StarsU16 string_id;
    StarsI16 format_arguments;
    StarsU16 strings;
    StarsU16 centered;
    StarsU16 mode_changes;
    StarsU16 font;
} Trace;

static StarsI32 get_stat(void *context, const void *fleet, StarsI16 stat)
{
    (void)context;
    (void)fleet;
    return stat == 1 ? 80L : 100L;
}

static StarsI16 ideal(void *context, const void *fleet, StarsI16 ignore)
{
    (void)context;
    (void)fleet;
    (void)ignore;
    return 9;
}

static void select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    t->font = font;
}

static StarsI32 draw(void *context, StarsU16 hdc, const StarsRect *rect,
    StarsI16 segments, const StarsI32 *values, const StarsU16 *brushes,
    StarsI32 total)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)rect;
    t->segments = (StarsU16)segments;
    t->total = total;
    t->value0 = values[0];
    t->value1 = values[1];
    t->brush0 = brushes[0];
    return values[0];
}

static StarsU16 set_mode(void *context, StarsU16 hdc, StarsU16 mode)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    ++t->mode_changes;
    return (StarsU16)(mode + 1U);
}

static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *out)
{
    Trace *t = (Trace *)context;
    (void)id;
    ++t->strings;
    t->string_id = id;
    strcpy((char *)out, "TXT");
    return 3;
}

static StarsI16 format_text(void *context, StarsU8 *out, StarsU16 capacity,
    StarsU16 id, StarsI32 value, StarsI32 total, StarsI16 arguments)
{
    Trace *t = (Trace *)context;
    (void)capacity;
    (void)value;
    (void)total;
    strcpy((char *)out, "FMT");
    t->format_id = id;
    t->format_arguments = arguments;
    return 3;
}

static StarsI32 extent(void *context, StarsU16 hdc, const StarsU8 *text,
    StarsI16 length)
{
    (void)context;
    (void)hdc;
    (void)text;
    return length;
}

static void center(void *context, StarsU16 hdc, const StarsRect *rect,
    const StarsU8 *text, StarsI16 length)
{
    Trace *t = (Trace *)context;
    (void)hdc;
    (void)rect;
    (void)text;
    (void)length;
    ++t->centered;
}

int main(void)
{
    StarsDrawFleetGaugeInput input;
    StarsRect rect;
    StarsI32 weights[5] = { 10L, 20L, 30L, 40L, 50L };
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    rect.left = 0U;
    rect.top = 0U;
    rect.right = 100U;
    rect.bottom = 10U;
    input.hdc = 1U;
    input.rect = &rect;
    input.fleet = &input;
    input.mineral_weights = weights;
    input.active_warp_word = (StarsU16)(10U << 4);
    input.active_engine_value = 4L;
    input.ideal_warp_ignore_scoops = 0;
    input.font_arial8 = 7U;
    input.mineral_brushes[0] = 10U;
    input.mineral_brushes[1] = 11U;
    input.mineral_brushes[2] = 12U;
    input.mineral_brushes[3] = 13U;
    input.mineral_brushes[4] = 14U;
    input.ops.get_fleet_stat = get_stat;
    input.ops.find_ideal_warp = ideal;
    input.ops.select_font = select_font;
    input.ops.draw_gauge = draw;
    input.ops.set_bk_mode = set_mode;
    input.ops.get_string = get_string;
    input.ops.format_text = format_text;
    input.ops.text_extent = extent;
    input.ops.center_text = center;
    input.context = &trace;

    stars_draw_fleet_gauge(&input, 2);
    if (trace.segments != 1U || trace.total != 100L || trace.value0 != 30L ||
            trace.brush0 != 12U || trace.format_id != 0x037cU ||
            trace.format_arguments != 1 || trace.centered != 1U ||
            trace.font != 7U)
        return 1;
    memset(&trace, 0, sizeof trace);
    stars_draw_fleet_gauge(&input, 4);
    if (trace.total != 80L || trace.brush0 != 14U ||
            trace.format_id != 0x036cU || trace.format_arguments != 2)
        return 2;
    memset(&trace, 0, sizeof trace);
    stars_draw_fleet_gauge(&input, 5);
    if (trace.segments != 4U || trace.value0 != 10L || trace.value1 != 20L ||
            trace.format_id != 0x036bU || trace.format_arguments != 2)
        return 3;
    memset(&trace, 0, sizeof trace);
    stars_draw_fleet_gauge(&input, 6);
    if (trace.total != 11L || trace.value0 != 10L || trace.brush0 != 12U ||
            trace.strings != 0U || trace.format_id != 0x0369U ||
            trace.format_arguments != 1)
        return 4;
    memset(&trace, 0, sizeof trace);
    input.active_warp_word = 0U;
    stars_draw_fleet_gauge(&input, 6);
    if (trace.value0 != 0L || trace.strings != 1U ||
            trace.string_id != 0x0368U)
        return 5;
    memset(&trace, 0, sizeof trace);
    stars_draw_fleet_gauge(&input, 7);
    if (trace.total != 10L || trace.value0 != 4L || trace.brush0 != 14U ||
            trace.format_id != 0x0369U || trace.format_arguments != 1)
        return 6;
    puts("DRAW_FLEET_GAUGE_SOURCE=PASS");
    return 0;
}
