#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct BarProbe {
    int acquired;
    int released;
    int boxes;
    int text;
} BarProbe;

static StarsU16 get_dc(void *opaque, StarsU16 hwnd)
{
    BarProbe *probe = (BarProbe *)opaque;
    (void)hwnd;
    ++probe->acquired;
    return 9;
}

static void release_dc(void *opaque, StarsU16 hwnd, StarsU16 hdc)
{
    BarProbe *probe = (BarProbe *)opaque;
    (void)hwnd; (void)hdc;
    ++probe->released;
}

static void get_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    (void)opaque; (void)hwnd;
    rect->left = 0; rect->top = 0; rect->right = 320; rect->bottom = 48;
}

static void select_object(void *opaque, StarsU16 hdc, StarsU16 object)
{
    (void)opaque; (void)hdc; (void)object;
}

static StarsU16 set_mode(void *opaque, StarsU16 hdc, StarsU16 mode)
{
    (void)opaque; (void)hdc;
    return mode;
}

static StarsU32 set_color(void *opaque, StarsU16 hdc, StarsU32 color)
{
    (void)opaque; (void)hdc;
    return color;
}

static StarsI16 extent(void *opaque, StarsU16 hdc, const StarsU8 *text,
    StarsI16 length)
{
    (void)opaque; (void)hdc; (void)text;
    return (StarsI16)(length * 2);
}

static void text_out(void *opaque, StarsU16 hdc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length, const StarsRect *rect)
{
    BarProbe *probe = (BarProbe *)opaque;
    (void)hdc; (void)x; (void)y; (void)text; (void)length; (void)rect;
    ++probe->text;
}

static StarsI16 get_string(void *opaque, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    const char *text = "empty";
    (void)opaque; (void)id;
    if (capacity > 0) {
        strncpy((char *)out, text, (size_t)capacity - 1U);
        out[capacity - 1U] = 0;
    }
    return 5;
}

static StarsI16 format_number(void *opaque, StarsI32 value, StarsU8 *out,
    StarsU16 capacity)
{
    (void)opaque;
    if (capacity > 1) {
        out[0] = (StarsU8)('0' + (value < 0 ? 0 : value % 10));
        out[1] = 0;
    }
    return 1;
}

static const StarsU8 *object_name(void *opaque, StarsI16 kind, StarsI16 id)
{
    (void)opaque; (void)kind; (void)id;
    return (const StarsU8 *)"planet";
}

static const StarsU8 *distance_text(void *opaque, StarsPoint from,
    StarsPoint to)
{
    (void)opaque; (void)from; (void)to;
    return (const StarsU8 *)"distance";
}

static const StarsU8 *location_text(void *opaque, StarsI16 kind,
    StarsI16 id, StarsPoint point)
{
    (void)opaque; (void)kind; (void)id; (void)point;
    return (const StarsU8 *)"location";
}

static void pattern(void *opaque, StarsU16 hdc, const StarsRect *rect,
    StarsI16 full_redraw)
{
    BarProbe *probe = (BarProbe *)opaque;
    (void)hdc; (void)rect; (void)full_redraw;
    ++probe->boxes;
}

int main(void)
{
    StarsDrawScannerBarOps ops;
    StarsDrawScannerBarInput input;
    StarsDrawScannerBarState selection;
    BarProbe probe;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&selection, 0, sizeof selection);
    memset(&probe, 0, sizeof probe);
    ops.get_dc = get_dc;
    ops.release_dc = release_dc;
    ops.get_client_rect = get_rect;
    ops.select_object = select_object;
    ops.set_background_mode = set_mode;
    ops.set_background_color = set_color;
    ops.set_text_color = set_color;
    ops.text_extent = extent;
    ops.text_out = text_out;
    ops.get_string = get_string;
    ops.format_number = format_number;
    ops.get_object_name = object_name;
    ops.get_distance = distance_text;
    ops.get_location = location_text;
    ops.draw_lock_light = pattern;
    ops.fill_background = pattern;
    selection.object_flags = 1;
    selection.object_full_flags = 1;
    selection.planet_id = 4;
    selection.object_id = 4;
    selection.point.x = 100;
    selection.point.y = 120;
    selection.target.x = 140;
    selection.target.y = 160;
    input.scanner_window = 3;
    input.scrollbar_height = 24;
    input.font = 1;
    input.button_face_brush = 2;
    input.button_face_color = 0xaaaa;
    input.button_text_color = 0xbbbb;
    input.full_redraw = 1;
    input.selection = selection;
    input.ops = &ops;
    input.context = &probe;
    stars_draw_scanner_sbar(0, (const StarsRect *)0, &input);
    if (probe.acquired != 1 || probe.released != 1 || probe.boxes < 4 ||
            probe.text < 4) return 1;
    printf("DRAW_SCANNER_SBAR_SOURCE=PASS\n");
    return 0;
}
