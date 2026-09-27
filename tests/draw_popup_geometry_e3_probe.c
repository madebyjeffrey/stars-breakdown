#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct GeometryTrace {
    StarsI16 short_client;
    StarsI16 measure_count;
    StarsI16 emit_count;
    StarsI16 text_color_count;
    StarsI16 separator_count;
    StarsU32 text_colors[8];
    StarsI16 separator_x;
    StarsI16 separator_y;
    StarsI16 separator_width;
    StarsI16 separator_height;
    StarsU16 separator_rop;
    StarsI16 measure_ids[8];
    StarsI16 measure_formats[8];
    StarsI16 emit_roles[48];
    StarsI16 emit_rows[48];
    StarsI16 emit_ids[48];
    StarsI16 emit_formats[48];
    StarsI16 emit_x[48];
    StarsI16 emit_y[48];
    StarsI16 emit_anchors[48];
    StarsI16 emit_fonts[48];
    StarsI16 emit_modes[48];
} GeometryTrace;

static void put16(StarsU8 *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static void put32(StarsU8 *raw, StarsI16 offset, StarsU32 value)
{
    put16(raw, offset, (StarsU16)(value & 0xffffUL));
    put16(raw, (StarsI16)(offset + 2), (StarsU16)(value >> 16));
}

static StarsI16 get_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    GeometryTrace *trace = (GeometryTrace *)context;
    (void)hwnd;
    rect->left = 0;
    rect->top = 0;
    rect->right = 160;
    rect->bottom = trace->short_client ? 8 : 90;
    return 1;
}

static StarsI16 resolve_string(void *context, StarsI16 id,
    StarsU8 *buffer, StarsU16 capacity)
{
    (void)context;
    (void)id;
    if (capacity < 2U)
        return 0;
    buffer[0] = 'S';
    buffer[1] = 0;
    return 1;
}

static StarsI16 format_text(void *context, const StarsDrawPopupText *text,
    StarsU8 *buffer, StarsU16 capacity)
{
    (void)context;
    (void)text;
    if (capacity < 2U)
        return 0;
    buffer[0] = 'F';
    buffer[1] = 0;
    return 1;
}

static StarsI16 measure_text(void *context, const StarsDrawPopupText *text)
{
    GeometryTrace *trace = (GeometryTrace *)context;
    StarsI16 index = trace->measure_count;

    if (index < 8) {
        trace->measure_ids[index] = text->string_id;
        trace->measure_formats[index] = text->format_kind;
    }
    ++trace->measure_count;
    if (text->string_id == 0x25c)
        return 18;
    return text->format_kind == STARS_DRAW_POPUP_FORMAT_LONG ? 20 : 0;
}

static void emit_text(void *context, const StarsDrawPopupText *text,
    const StarsU8 *buffer, StarsI16 length)
{
    GeometryTrace *trace = (GeometryTrace *)context;
    StarsI16 index = trace->emit_count;

    (void)buffer;
    (void)length;
    if (index < 48) {
        trace->emit_roles[index] = text->role;
        trace->emit_rows[index] = text->row;
        trace->emit_ids[index] = text->string_id;
        trace->emit_formats[index] = text->format_kind;
        trace->emit_x[index] = text->x;
        trace->emit_y[index] = text->y;
        trace->emit_anchors[index] = text->anchor;
        trace->emit_fonts[index] = text->font_kind;
        trace->emit_modes[index] = text->background_mode;
    }
    ++trace->emit_count;
}

static StarsU32 set_text_color(void *context, StarsU16 hdc, StarsU32 color)
{
    GeometryTrace *trace = (GeometryTrace *)context;
    if (trace->text_color_count < 8)
        trace->text_colors[trace->text_color_count] = color;
    ++trace->text_color_count;
    (void)hdc;
    return 0x44UL;
}

static void draw_separator(void *context, StarsU16 hdc, StarsI16 x,
    StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 raster_op)
{
    GeometryTrace *trace = (GeometryTrace *)context;
    ++trace->separator_count;
    trace->separator_x = x;
    trace->separator_y = y;
    trace->separator_width = width;
    trace->separator_height = height;
    trace->separator_rop = raster_op;
    (void)hdc;
}

static StarsI16 hull_visible(void *context, StarsI16 hull,
    StarsI16 player, StarsI16 visibility)
{
    (void)context;
    (void)hull;
    (void)player;
    (void)visibility;
    return 1;
}

static StarsI16 decorate_hull(void *context, StarsI16 tech, StarsI16 hull,
    StarsU8 *buffer, StarsU16 capacity)
{
    (void)context;
    (void)tech;
    (void)hull;
    if (capacity < 2U)
        return 0;
    buffer[0] = 'H';
    buffer[1] = 0;
    return 1;
}

static void clear_trace(GeometryTrace *trace)
{
    StarsI16 short_client = trace->short_client;
    memset(trace, 0, sizeof *trace);
    trace->short_client = short_client;
}

static void setup(StarsDrawPopupInput *input, StarsDrawPopupOps *ops,
    GeometryTrace *trace, StarsU8 *popup, StarsU8 *fleet, StarsU8 *shdef)
{
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    ops->get_client_rect = get_rect;
    ops->resolve_string = resolve_string;
    ops->format_text = format_text;
    ops->measure_text = measure_text;
    ops->emit_text = emit_text;
    ops->set_text_color = set_text_color;
    ops->draw_separator = draw_separator;
    ops->hull_visible = hull_visible;
    ops->decorate_hull = decorate_hull;
    ops->context = trace;
    input->packed_popupdata = popup;
    input->packed_popupdata_size = 22;
    input->packed_fleet_record = fleet;
    input->packed_fleet_size = 124;
    input->packed_shdef_record = shdef;
    input->packed_shdef_size = 147;
    input->arial8_height = 8;
    input->ops = ops;
}

int main(void)
{
    StarsU8 popup[22];
    StarsU8 fleet[124];
    StarsU8 shdef[147];
    StarsDrawPopupInput input;
    StarsDrawPopupOps ops;
    GeometryTrace trace;
    int checks = 0;
    int failures = 0;
    int first_failure = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) { ++failures; if (!first_failure) first_failure = checks; } } while (0)

    memset(popup, 0, sizeof popup);
    memset(fleet, 0, sizeof fleet);
    memset(shdef, 0, sizeof shdef);
    put16(popup, 0, 1);
    put32(popup, 2, 0x12345678UL);
    put16(popup, 6, 2);
    put16(popup, 8, 1);
    put16(popup, 10, 50);
    put16(popup, 12, 4);
    put16(popup, 14, 10);
    put16(popup, 16, 80);
    put32(popup, 18, 0x00020001UL);
    put16(fleet, 12, 2);
    put16(fleet, 16, 1);
    put16(fleet, 44, 0x0280);
    put16(shdef, 0, 0x0600);

    memset(&trace, 0, sizeof trace);
    setup(&input, &ops, &trace, popup, fleet, shdef);
    stars_draw_popup(&input);
    CHECK(trace.measure_count == 1 &&
        trace.measure_formats[0] == STARS_DRAW_POPUP_FORMAT_LONG);
    CHECK(trace.emit_count == 8);
    CHECK(trace.emit_x[0] == 80 && trace.emit_y[0] == 4 &&
        trace.emit_anchors[0] == STARS_DRAW_POPUP_ANCHOR_CENTER &&
        trace.emit_fonts[0] == STARS_DRAW_POPUP_FONT_BOLD);
    CHECK(trace.emit_x[1] == 24 && trace.emit_y[1] == 12 &&
        trace.emit_anchors[1] == STARS_DRAW_POPUP_ANCHOR_RIGHT);
    CHECK(trace.emit_x[2] == 24 && trace.emit_y[2] == 12 &&
        trace.emit_anchors[2] == STARS_DRAW_POPUP_ANCHOR_LEFT);
    CHECK(trace.emit_y[3] == 20 && trace.emit_x[3] == 24);
    CHECK(trace.emit_y[7] == 28 && trace.emit_x[7] == 24);
    CHECK(trace.emit_modes[0] == 2 && trace.emit_modes[7] == 2);

    put16(popup, 0, 2);
    clear_trace(&trace);
    stars_draw_popup(&input);
    CHECK(trace.emit_count == 3);
    CHECK(trace.emit_x[0] == 80 && trace.emit_y[0] == 4 &&
        trace.emit_anchors[0] == STARS_DRAW_POPUP_ANCHOR_CENTER);
    CHECK(trace.emit_x[1] == 80 && trace.emit_y[1] == 12 &&
        trace.emit_x[2] == 80 && trace.emit_y[2] == 12);

    put16(popup, 0, 3);
    clear_trace(&trace);
    stars_draw_popup(&input);
    CHECK(trace.emit_count == 8);
    CHECK(trace.emit_x[0] == 4 && trace.emit_y[0] == 4 &&
        trace.emit_fonts[0] == STARS_DRAW_POPUP_FONT_BOLD);
    CHECK(trace.emit_x[1] == 154 &&
        trace.emit_anchors[1] == STARS_DRAW_POPUP_ANCHOR_RIGHT);
    CHECK(trace.emit_x[2] == 156 && trace.emit_y[2] == 4 &&
        trace.emit_anchors[2] == STARS_DRAW_POPUP_ANCHOR_RIGHT);
    CHECK(trace.emit_x[3] == 4 && trace.emit_y[3] == 12 &&
        trace.emit_anchors[3] == STARS_DRAW_POPUP_ANCHOR_LEFT);
    CHECK(trace.emit_x[4] == 156 && trace.emit_y[4] == 12 &&
        trace.emit_anchors[4] == STARS_DRAW_POPUP_ANCHOR_RIGHT);
    CHECK(trace.separator_count == 1 && trace.separator_x == 4 &&
        trace.separator_y == 10 && trace.separator_width == 152 &&
        trace.separator_height == 1 && trace.separator_rop == 0x42);
    CHECK(trace.text_color_count == 3 && trace.text_colors[0] == 0 &&
        trace.text_colors[1] == 0 && trace.text_colors[2] == 0x44UL);

    put16(popup, 0, 4);
    input.coordinate_valid = 1;
    input.coordinate_planet = 8;
    input.coordinate_x = 123;
    input.coordinate_y = 456;
    clear_trace(&trace);
    stars_draw_popup(&input);
    CHECK(trace.measure_count == 1 && trace.measure_ids[0] == 0x25c);
    CHECK(trace.emit_count == 8 && trace.emit_x[0] == 22 &&
        trace.emit_y[0] == 4 &&
        trace.emit_anchors[0] == STARS_DRAW_POPUP_ANCHOR_RIGHT &&
        trace.emit_fonts[0] == STARS_DRAW_POPUP_FONT_BOLD);
    CHECK(trace.emit_x[1] == 22 && trace.emit_y[1] == 4 &&
        trace.emit_anchors[1] == STARS_DRAW_POPUP_ANCHOR_LEFT);
    CHECK(trace.emit_y[3] == 12 && trace.emit_y[5] == 20 &&
        trace.emit_y[7] == 28);

    put16(popup, 0, 3);
    trace.short_client = 1;
    clear_trace(&trace);
    stars_draw_popup(&input);
    CHECK(trace.emit_count == 1 && trace.emit_x[0] == 4 &&
        trace.emit_y[0] == 4 && trace.emit_ids[0] == 0x263 &&
        trace.emit_fonts[0] == STARS_DRAW_POPUP_FONT_NORMAL);
    CHECK(checks == 25);
#ifndef TARGET_NATIVE
    printf("DRAW_POPUP_GEOMETRY_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures ? first_failure : 0;
#undef CHECK
}
