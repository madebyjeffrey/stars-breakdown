#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeState {
    int frames;
    int text;
    int graphs;
    int cost_calls;
} ProbeState;

static void select_font(void *opaque, StarsU16 hdc, StarsU16 font)
{
    (void)opaque; (void)hdc; (void)font;
}

static void select_brush(void *opaque, StarsU16 hdc, StarsU16 brush)
{
    (void)opaque; (void)hdc; (void)brush;
}

static void set_color(void *opaque, StarsU16 hdc, StarsU32 color)
{
    (void)opaque; (void)hdc; (void)color;
}

static StarsI16 get_string(void *opaque, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    const char *value;
    (void)opaque;
    value = id == 0x0351 ? "cost" : id == 0x0358 ? "fuel" : "component";
    if (capacity > 0) {
        strncpy((char *)out, value, (size_t)capacity - 1U);
        out[capacity - 1U] = 0;
    }
    return (StarsI16)strlen(value);
}

static StarsI16 format_number(void *opaque, StarsI32 value, StarsU8 *out,
    StarsU16 capacity)
{
    (void)opaque;
    if (capacity > 0) {
        out[0] = (StarsU8)('0' + (value < 0 ? 0 : value % 10));
        if (capacity > 1) out[1] = 0;
    }
    return 1;
}

static StarsI16 extent(void *opaque, StarsU16 hdc, const StarsU8 *text,
    StarsI16 length)
{
    (void)opaque; (void)hdc; (void)text;
    return length;
}

static void text_out(void *opaque, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    ProbeState *state = (ProbeState *)opaque;
    (void)hdc; (void)x; (void)y; (void)text; (void)length;
    ++state->text;
}

static void right_text(void *opaque, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    text_out(opaque, hdc, x, y, text, length);
}

static void wrap_text(void *opaque, StarsU16 hdc, StarsRect *rect,
    const StarsU8 *text, StarsI16 color)
{
    (void)color;
    text_out(opaque, hdc, rect->left, rect->top, text,
        (StarsI16)strlen((const char *)text));
}

static void frame(void *opaque, StarsU16 hdc, const StarsRect *rect,
    StarsI16 style)
{
    ProbeState *state = (ProbeState *)opaque;
    (void)hdc; (void)rect; (void)style;
    ++state->frames;
}

static StarsI32 cost(void *opaque, StarsI16 player,
    const void *part)
{
    ProbeState *state = (ProbeState *)opaque;
    (void)player; (void)part;
    ++state->cost_calls;
    return 1200L;
}

static void graph(void *opaque, StarsU16 hdc, const StarsRect *rect,
    StarsI16 kind, StarsI16 damage, StarsI16 resistance)
{
    ProbeState *state = (ProbeState *)opaque;
    (void)hdc; (void)rect; (void)kind; (void)damage; (void)resistance;
    ++state->graphs;
}

int main(void)
{
    static const StarsDisplayComponentOps ops = {
        select_font, select_brush, 0, set_color, get_string, format_number,
        extent, text_out, right_text, 0, wrap_text, frame, 0, 0, cost, graph
    };
    static const StarsDisplayComponentPart part = {
        1, 2, 0, 4, 7, 80, 3, 1, 12, 9, 0, 0, { 10, 20, 30, 40, 50, 60 },
        (const StarsU8 *)"Engine"
    };
    StarsDisplayComponentInput input;
    ProbeState state;

    memset(&state, 0, sizeof state);
    memset(&input, 0, sizeof input);
    input.hdc = 1;
    input.width = 240;
    input.height = 180;
    input.arial8_height = 8;
    input.arial10_height = 10;
    input.max_mineral_width = 24;
    input.font_arial8 = 1;
    input.font_arial8_bold = 2;
    input.font_arial10 = 3;
    input.brush_shadow = 4;
    input.color_black = 0;
    input.color_disabled = 0x7f;
    input.player = 1;
    input.part = &part;
    input.ops = &ops;
    input.context = &state;
    stars_display_component_info(&input);
    if (state.frames < 2 || state.text < 5 || state.cost_calls != 1) return 1;
    printf("DISPLAY_COMPONENT_SOURCE=PASS\n");
    return 0;
}
