#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct ContentTrace {
    StarsI16 resolve_count;
    StarsI16 format_count;
    StarsI16 emit_count;
    StarsI16 visible_count;
    StarsI16 decorate_count;
    StarsI16 resolve_ids[16];
    StarsI16 format_kinds[16];
    StarsI16 format_v1[16];
    StarsI16 format_v2[16];
    StarsI32 format_values1[16];
    StarsI32 format_values2[16];
    StarsI16 emit_roles[32];
    StarsI16 emit_rows[32];
    StarsI16 emit_hulls[32];
    StarsI16 emit_colors[32];
    StarsI16 decorate_tech;
    StarsI16 decorate_hull;
} ContentTrace;

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
    (void)context;
    (void)hwnd;
    rect->left = 0;
    rect->top = 0;
    rect->right = 160;
    rect->bottom = 90;
    return 1;
}

static StarsI16 resolve_string(void *context, StarsI16 id,
    StarsU8 *buffer, StarsU16 capacity)
{
    ContentTrace *trace = (ContentTrace *)context;
    if (trace->resolve_count < 16)
        trace->resolve_ids[trace->resolve_count] = id;
    ++trace->resolve_count;
    if (capacity < 2U)
        return 0;
    buffer[0] = 'S';
    buffer[1] = 0;
    return 1;
}

static StarsI16 format_text(void *context, const StarsDrawPopupText *text,
    StarsU8 *buffer, StarsU16 capacity)
{
    ContentTrace *trace = (ContentTrace *)context;
    StarsI16 index = trace->format_count;
    if (index < 16) {
        trace->format_kinds[index] = text->format_kind;
        trace->format_v1[index] = (StarsI16)text->value1;
        trace->format_v2[index] = (StarsI16)text->value2;
        trace->format_values1[index] = text->value1;
        trace->format_values2[index] = text->value2;
    }
    ++trace->format_count;
    if (capacity < 2U)
        return 0;
    buffer[0] = 'F';
    buffer[1] = 0;
    return 1;
}

static void emit_text(void *context, const StarsDrawPopupText *text,
    const StarsU8 *buffer, StarsI16 length)
{
    ContentTrace *trace = (ContentTrace *)context;
    StarsI16 index = trace->emit_count;
    (void)buffer;
    (void)length;
    if (index < 32) {
        trace->emit_roles[index] = text->role;
        trace->emit_rows[index] = text->row;
        trace->emit_hulls[index] = text->hull_index;
        trace->emit_colors[index] = text->text_color;
    }
    ++trace->emit_count;
}

static StarsI16 hull_visible(void *context, StarsI16 hull,
    StarsI16 player, StarsI16 visibility)
{
    ContentTrace *trace = (ContentTrace *)context;
    (void)player;
    (void)visibility;
    ++trace->visible_count;
    return hull == 1 ? 0 : 1;
}

static StarsI16 decorate_hull(void *context, StarsI16 tech, StarsI16 hull,
    StarsU8 *buffer, StarsU16 capacity)
{
    ContentTrace *trace = (ContentTrace *)context;
    ++trace->decorate_count;
    trace->decorate_tech = tech;
    trace->decorate_hull = hull;
    if (capacity < 2U)
        return 0;
    buffer[0] = 'H';
    buffer[1] = 0;
    return 1;
}

static void clear_trace(ContentTrace *trace)
{
    memset(trace, 0, sizeof *trace);
}

static void setup(StarsDrawPopupInput *input, StarsDrawPopupOps *ops,
    ContentTrace *trace, StarsU8 *popup, StarsU8 *fleet, StarsU8 *shdef)
{
    memset(input, 0, sizeof *input);
    memset(ops, 0, sizeof *ops);
    ops->get_client_rect = get_rect;
    ops->resolve_string = resolve_string;
    ops->format_text = format_text;
    ops->emit_text = emit_text;
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
    ContentTrace trace;
    int checks = 0;
    int failures = 0;
    int first_failure = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) { ++failures; if (!first_failure) first_failure = checks; } } while (0)

    memset(popup, 0, sizeof popup);
    memset(fleet, 0, sizeof fleet);
    memset(shdef, 0, sizeof shdef);
    put16(popup, 0, 1);
    put16(popup, 2, 0x04d2);
    put16(popup, 6, 1);
    put16(popup, 8, 1);
    put16(popup, 10, 50);
    put16(popup, 12, 4);
    put16(popup, 14, 10);
    put16(popup, 16, 80);
    put32(popup, 18, 0x00020001UL);
    put16(fleet, 12, 3);
    put16(fleet, 14, 2);
    put16(fleet, 16, 1);
    put16(fleet, 44, 0x0280);
    put16(fleet, 48, 0x0101);
    put16(shdef, 0, 0x0600);

    clear_trace(&trace);
    setup(&input, &ops, &trace, popup, fleet, shdef);
    stars_draw_popup(&input);
    CHECK(trace.emit_count == 8);
    CHECK(trace.resolve_count == 4);
    CHECK(trace.format_count == 4);
    CHECK(trace.resolve_ids[0] == 0x265 && trace.resolve_ids[1] == 0x264);
    CHECK(trace.format_kinds[0] == STARS_DRAW_POPUP_FORMAT_LONG);
    CHECK(trace.format_v1[1] == 50 && trace.format_v2[1] == 4);
    CHECK(trace.resolve_ids[2] == 0x51e && trace.resolve_ids[3] == 0x266);

    put16(popup, 0, 2);
    put16(popup, 2, 7);
    put16(popup, 4, 0x5678);
    clear_trace(&trace);
    stars_draw_popup(&input);
    CHECK(trace.emit_count == 3 && trace.resolve_count == 1);
    CHECK(trace.resolve_ids[0] == 0x262);
    CHECK(trace.format_kinds[0] == STARS_DRAW_POPUP_FORMAT_I16 &&
        trace.format_v1[0] == 7);
    CHECK(trace.format_kinds[1] == STARS_DRAW_POPUP_FORMAT_LONG &&
        (StarsU32)trace.format_values1[1] == 0x56780007UL);

    put16(popup, 0, 3);
    put16(popup, 2, 4);
    put16(popup, 10, 1);
    put16(popup, 6, 1);
    clear_trace(&trace);
    stars_draw_popup(&input);
    CHECK(trace.visible_count == 3);
    CHECK(trace.decorate_count == 2 && trace.decorate_tech == 3 &&
        trace.decorate_hull == 2);
    CHECK(trace.emit_count == 9);
    CHECK(trace.emit_hulls[3] == 0 && trace.emit_hulls[6] == 2);
    CHECK(trace.emit_colors[3] == 0xff);
    CHECK(trace.format_kinds[1] == STARS_DRAW_POPUP_FORMAT_PERCENT_PAIR);

    put16(popup, 0, 4);
    input.coordinate_valid = 1;
    input.coordinate_planet = 8;
    input.coordinate_x = 123;
    input.coordinate_y = 456;
    clear_trace(&trace);
    stars_draw_popup(&input);
    CHECK(trace.emit_count == 8 && trace.resolve_count == 4 &&
        trace.format_count == 4);
    CHECK(trace.resolve_ids[0] == 0x25c && trace.resolve_ids[3] == 0x25f);
    CHECK(trace.format_v1[1] == 9 && trace.format_v1[2] == 123 &&
        trace.format_v1[3] == 456);
    CHECK(checks == 21);
#ifndef TARGET_NATIVE
    printf("DRAW_POPUP_CONTENT_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures ? "FAIL" : "PASS");
#endif
    return failures ? first_failure : 0;
#undef CHECK
}
