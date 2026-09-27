#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct DrawTutorTrace {
    StarsRect parent;
    StarsRect child;
    StarsU16 begin_dc;
    StarsU16 selected[8];
    StarsU16 selected_count;
    StarsU32 backgrounds[8];
    StarsU16 background_count;
    StarsU32 text_colors[8];
    StarsU16 text_color_count;
    StarsU16 pat_count;
    StarsU16 fill_brush;
    StarsRect fill_rect;
    StarsI16 ids[8];
    StarsI16 lengths[8];
    StarsI16 xs[8];
    StarsI16 ys[8];
    StarsI16 new_lines[8];
    StarsU16 wrap_count;
    StarsU16 end_hwnd;
} DrawTutorTrace;

static StarsU16 begin_paint(void *opaque, StarsU16 hwnd, StarsRect *paint)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    (void)hwnd;
    paint->left = 0U; paint->top = 0U; paint->right = 600U; paint->bottom = 800U;
    trace->begin_dc = 6U;
    return trace->begin_dc;
}

static void end_paint(void *opaque, StarsU16 hwnd, const StarsRect *paint)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    (void)paint;
    trace->end_hwnd = hwnd;
}

static StarsU16 select_object(void *opaque, StarsU16 dc, StarsU16 object)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    (void)dc;
    trace->selected[trace->selected_count++] = object;
    return 0U;
}

static void set_color(void *opaque, StarsU16 dc, StarsU32 color)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    (void)dc;
    if (trace->background_count < 8U)
        trace->backgrounds[trace->background_count++] = color;
}

static void set_text_color(void *opaque, StarsU16 dc, StarsU32 color)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    (void)dc;
    if (trace->text_color_count < 8U)
        trace->text_colors[trace->text_color_count++] = color;
}

static void pat_blt(void *opaque, StarsU16 dc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU32 rop)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    (void)dc; (void)x; (void)y; (void)width; (void)height; (void)rop;
    ++trace->pat_count;
}

static void get_window_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    *rect = hwnd == 42U ? trace->parent : trace->child;
}

static void screen_to_client(void *opaque, StarsU16 hwnd, StarsPoint *point)
{
    (void)opaque; (void)hwnd;
    point->x = (StarsU16)(point->x - 10U);
    point->y = (StarsU16)(point->y - 20U);
}

static StarsU16 get_dialog_item(void *opaque, StarsU16 hwnd, StarsU16 id)
{
    (void)opaque; (void)hwnd; (void)id;
    return 43U;
}

static StarsI16 get_string(void *opaque, StarsU8 *text, StarsI16 id)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    StarsI16 slot = trace->wrap_count;
    trace->ids[slot] = id;
    if (id == 100) { text[0] = 'F'; text[1] = 'i'; text[2] = 'r';
        text[3] = 's'; text[4] = 't'; return 5; }
    if (id == 101) { text[0] = 'S'; text[1] = 'e'; text[2] = 'c';
        text[3] = 'o'; text[4] = 'n'; text[5] = 'd'; return 6; }
    if (id == 102) { text[0] = 'B'; text[1] = 'o'; text[2] = 'l';
        text[3] = 'd'; return 4; }
    text[0] = 'x';
    return 1;
}

static StarsI16 has_break(void *opaque, StarsU8 first)
{
    (void)opaque;
    return first == 'S';
}

static void wrap_text(void *opaque, StarsU16 dc, StarsI16 *x, StarsI16 *y,
    const StarsU8 *text, StarsI16 length, StarsI16 left, StarsI16 width,
    StarsI16 *maximum, StarsI16 new_line, StarsI16 print_text)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    StarsU16 slot = trace->wrap_count;
    (void)dc; (void)text; (void)left; (void)width; (void)maximum;
    (void)print_text;
    trace->lengths[slot] = length; trace->xs[slot] = *x;
    trace->ys[slot] = *y; trace->new_lines[slot] = new_line;
    ++trace->wrap_count;
    *x = (StarsI16)(*x + length);
}

static void fill_rect(void *opaque, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    DrawTutorTrace *trace = (DrawTutorTrace *)opaque;
    (void)dc;
    trace->fill_rect = *rect; trace->fill_brush = brush;
}

int main(void)
{
    StarsDrawTutorOps ops;
    StarsDrawTutorInput input;
    DrawTutorTrace trace;
    (void)memset(&ops, 0, sizeof(ops));
    (void)memset(&input, 0, sizeof(input));
    (void)memset(&trace, 0, sizeof(trace));
    trace.parent.left = 100U; trace.parent.top = 200U;
    trace.parent.right = 500U; trace.parent.bottom = 700U;
    trace.child.top = 600U;
    ops.begin_paint = begin_paint; ops.end_paint = end_paint;
    ops.select_object = select_object; ops.set_background_color = set_color;
    ops.set_text_color = set_text_color; ops.pat_blt = pat_blt;
    ops.get_window_rect = get_window_rect;
    ops.screen_to_client = screen_to_client;
    ops.get_dialog_item = get_dialog_item; ops.get_string = get_string;
    ops.has_break = has_break; ops.wrap_text = wrap_text;
    ops.fill_rect = fill_rect;
    input.hwnd = 42U; input.arial8_height = 12; input.arial8_font = 1U;
    input.button_face_brush = 2U; input.button_shadow_brush = 3U;
    input.button_highlight_brush = 4U; input.button_face_color = 0x11UL;
    input.button_text_color = 0x22UL; input.idt = 100; input.idt_bold = 102;
    input.ops = &ops; input.context = &trace;
    stars_draw_tutor_text(&input);
    if (trace.begin_dc != 6U || trace.end_hwnd != 42U ||
            trace.pat_count != 4U || trace.fill_brush != 2U ||
            trace.wrap_count != 3U || trace.ids[0] != 100 ||
            trace.ids[1] != 101 || trace.ids[2] != 102 ||
            trace.lengths[0] != 5 || trace.lengths[1] != 6 ||
            trace.lengths[2] != 4 || trace.new_lines[0] != 0 ||
            trace.new_lines[1] != 1 || trace.new_lines[2] != 0 ||
            trace.selected_count != 3U || trace.selected[0] != 1U ||
            trace.selected[1] != 3U || trace.selected[2] != 4U ||
            trace.background_count != 3U || trace.backgrounds[0] != 0x11UL ||
            trace.backgrounds[1] != 0x22UL || trace.backgrounds[2] != 0x11UL ||
            trace.text_color_count != 3U || trace.text_colors[0] != 0x22UL ||
            trace.text_colors[1] != 0x11UL || trace.text_colors[2] != 0x22UL)
        return 1;
    puts("DRAW_TUTOR_TEXT_SOURCE=PASS");
    return 0;
}
