#include "core.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 window_texts;
    StarsI16 rects;
    StarsI16 positions;
    StarsI16 radio_checks;
    StarsI16 enables;
    StarsI16 fills;
    StarsI16 begins;
    StarsI16 ends_paint;
    StarsI16 frames;
    StarsI16 selects;
    StarsI16 colors;
    StarsI16 texts;
    StarsI16 control_colors;
    StarsI16 sends;
    StarsI16 renames;
    StarsI16 productions;
    StarsI16 invalidates;
    StarsI16 focus;
    StarsI16 stickies;
    StarsI16 dialogs;
    StarsI16 tutors;
    StarsI16 helps;
    StarsI16 last_fill_count;
    StarsI16 selected_queue;
    StarsI16 production_mode;
} Trace;

static void get_client(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; ++((Trace *)context)->rects;
    rect->left = 0; rect->top = 0; rect->right = 300; rect->bottom = 200;
}
static void get_window(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; ++((Trace *)context)->rects;
    rect->left = 10; rect->top = 10; rect->right = 310; rect->bottom = 210;
}
static void set_pos(void *context, StarsU16 hwnd, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 flags)
{
    (void)hwnd; (void)x; (void)y; (void)width; (void)height; (void)flags;
    ++((Trace *)context)->positions;
}
static StarsU16 begin_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; (void)rect; ++((Trace *)context)->begins; return 4;
}
static void end_paint(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; (void)rect; ++((Trace *)context)->ends_paint;
}
static void map_points(void *context, StarsU16 from, StarsU16 to,
    StarsRect *rect, StarsU16 count)
{
    (void)from; (void)to; (void)rect; (void)count; ++((Trace *)context)->rects;
}
static void expand_rect(void *context, StarsRect *rect, StarsI16 x,
    StarsI16 y)
{
    (void)context; rect->left = (StarsU16)(rect->left - x);
    rect->top = (StarsU16)(rect->top - y);
    rect->right = (StarsU16)(rect->right + x);
    rect->bottom = (StarsU16)(rect->bottom + y);
}
static void draw_frame(void *context, StarsU16 dc, const StarsRect *rect,
    StarsI16 raised)
{
    (void)dc; (void)rect; (void)raised; ++((Trace *)context)->frames;
}
static void select_object(void *context, StarsU16 dc, StarsU16 object)
{
    (void)dc; (void)object; ++((Trace *)context)->selects;
}
static void set_color(void *context, StarsU16 dc, StarsU32 color)
{
    (void)dc; (void)color; ++((Trace *)context)->colors;
}
static void fill_rect(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    (void)dc; (void)rect; (void)brush; ++((Trace *)context)->control_colors;
}
static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    (void)dc; (void)x; (void)y; (void)text; (void)length;
    ++((Trace *)context)->texts;
}
static void set_window_text(void *context, StarsU16 hwnd, const StarsU8 *text)
{
    (void)hwnd; (void)text; ++((Trace *)context)->window_texts;
}
static StarsI32 send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    (void)hwnd; (void)message; (void)wparam; (void)lparam;
    ++((Trace *)context)->sends; return 0;
}
static void check_radio(void *context, StarsU16 hwnd, StarsU16 first,
    StarsU16 last, StarsU16 selected)
{
    (void)hwnd; (void)first; (void)last; (void)selected;
    ++((Trace *)context)->radio_checks;
}
static void enable_buttons(void *context, StarsU16 hwnd, StarsI16 selected)
{
    (void)hwnd; ++((Trace *)context)->enables;
    ((Trace *)context)->selected_queue = selected;
}
static void fill_list(void *context, StarsU16 hwnd,
    const StarsZipProductionQueue *queue)
{
    (void)hwnd; ++((Trace *)context)->fills;
    ((Trace *)context)->last_fill_count = queue->item_count;
}
static StarsI16 rename_dialog(void *context, StarsI16 queue, StarsU8 *text,
    StarsU16 capacity)
{
    const char *value = "Renamed&Queue";
    StarsI16 i;
    Trace *trace = (Trace *)context;
    ++trace->renames;
    (void)queue;
    for (i = 0; i + 1 < (StarsI16)capacity && value[i] != 0; ++i)
        text[i] = (StarsU8)value[i];
    text[i] = 0;
    return 1;
}
static StarsI16 get_production(void *context, StarsI16 index,
    StarsU16 *kind, StarsU16 *code)
{
    Trace *trace = (Trace *)context;
    if (trace->production_mode == 1) {
        if (index == 0) return -1;
        if (index == 1) { *kind = 7; *code = 7; return 1; }
        if (index == 2) { *kind = 0x03ffU; *code = 6; return 1; }
        return 0;
    }
    if (trace->production_mode == 2) {
        if (index < 20) {
            *kind = (StarsU16)index;
            *code = (StarsU16)(index % 7);
            return 1;
        }
        return 0;
    }
    if (index == 0) { *kind = 1; *code = 2; return 1; }
    if (index == 1) { *kind = 2; *code = 6; return 1; }
    return 0;
}
static StarsU16 resource_tech(void *context)
{
    (void)context; return 1;
}
static const StarsU8 *compressed(void *context, StarsU16 id)
{
    (void)context; (void)id; return (const StarsU8 *)"Queue %d";
}
static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *text,
    StarsU16 capacity)
{
    const char *value = id == 0x4c1U ? "Production" : "Tech";
    StarsI16 i;
    (void)context;
    for (i = 0; i + 1 < (StarsI16)capacity && value[i] != 0; ++i)
        text[i] = (StarsU8)value[i];
    if (capacity != 0U) text[i] = 0;
    return i;
}
static void format_default(void *context, StarsU8 *text, const StarsU8 *format,
    StarsI16 number)
{
    (void)context; (void)format;
    text[0] = (StarsU8)'Q'; text[1] = (StarsU8)'u'; text[2] = (StarsU8)'e';
    text[3] = (StarsU8)'u'; text[4] = (StarsU8)'e'; text[5] = (StarsU8)' ';
    text[6] = (StarsU8)('0' + number); text[7] = 0;
}
static StarsU16 get_item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd; return (StarsU16)(id + 100);
}
static void get_dialog_text(void *context, StarsU16 hwnd, StarsU16 id,
    StarsU8 *text, StarsU16 capacity)
{
    (void)context; (void)hwnd; (void)id; (void)text; (void)capacity;
}
static void set_focus(void *context, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)context)->focus;
}
static void sticky(void *context, StarsU16 hwnd, StarsPoint *point,
    StarsI16 save)
{
    (void)hwnd; (void)point; (void)save; ++((Trace *)context)->stickies;
}
static void end_dialog(void *context, StarsU16 hwnd, StarsU16 result)
{
    (void)hwnd; (void)result; ++((Trace *)context)->dialogs;
}
static void win_help(void *context, StarsU16 hwnd, const StarsU8 *file,
    StarsU16 id)
{
    (void)hwnd; (void)file; (void)id; ++((Trace *)context)->helps;
}
static void advance(void *context)
{
    ++((Trace *)context)->tutors;
}

static void install(StarsZipProdDlgOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->get_client_rect = get_client;
    ops->get_window_rect = get_window;
    ops->set_window_pos = set_pos;
    ops->begin_paint = begin_paint;
    ops->end_paint = end_paint;
    ops->map_window_points = map_points;
    ops->expand_rect = expand_rect;
    ops->draw_frame = draw_frame;
    ops->select_object = select_object;
    ops->set_background_color = set_color;
    ops->fill_rect = fill_rect;
    ops->text_out = text_out;
    ops->set_window_text = set_window_text;
    ops->send_message = send_message;
    ops->check_radio_button = check_radio;
    ops->enable_buttons = enable_buttons;
    ops->fill_list = fill_list;
    ops->rename_dialog = rename_dialog;
    ops->get_production = get_production;
    ops->resource_tech = resource_tech;
    ops->compressed_string = compressed;
    ops->get_string = get_string;
    ops->format_default = format_default;
    ops->get_dialog_item = get_item;
    ops->get_dialog_text = get_dialog_text;
    ops->set_focus = set_focus;
    ops->sticky_position = sticky;
    ops->end_dialog = end_dialog;
    ops->win_help = win_help;
    ops->advance_tutor = advance;
    ops->context = trace;
}

int main(void)
{
    StarsZipProdDlgState state;
    StarsZipProdDlgOps ops;
    StarsZipProdDlgInput input;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    state.arial8_height = 12;
    state.button_face_brush = 3;
    state.button_face_color = 0x2345UL;
    state.game_bits = 0x0800;
    state.queue_count = 4;
    state.help_id = 0x452;
    state.tech_label_id[0] = 0x4c6;
    state.tech_label_id[1] = 0x4c7;
    input.state = &state;
    input.ops = &ops;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof(StarsZipProductionQueue) == 40U);
    CHECK(offsetof(StarsZipProductionQueue, name) == 0U);
    CHECK(offsetof(StarsZipProductionQueue, valid) == 13U);
    CHECK(offsetof(StarsZipProductionQueue, no_research) == 14U);
    CHECK(offsetof(StarsZipProductionQueue, item_count) == 15U);
    CHECK(offsetof(StarsZipProductionQueue, items) == 16U);
    CHECK(sizeof(state.queues) == 200U);
    CHECK(stars_zip_prod_dlg(&input, 20, 272, 0, 0) == 1);
    CHECK(trace.radio_checks == 1 && trace.fills == 1);
    CHECK(trace.window_texts >= 5 && trace.stickies == 1);
    state.queues[0].valid = 1;
    state.queues[0].name[0] = (StarsU8)'B';
    state.queues[0].name[1] = (StarsU8)'a';
    state.queues[0].name[2] = 0;
    CHECK(stars_zip_prod_dlg(&input, 20, 15, 0, 0) == 1);
    CHECK(trace.begins == 1 && trace.ends_paint == 1 && trace.frames == 1);
    CHECK(trace.texts == 2);
    CHECK(stars_zip_prod_dlg(&input, 20, 20, 4, 0) == 1);
    CHECK(trace.control_colors == 1);
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 0x432, 0) == 0);
    CHECK(state.selected_queue == 1 && trace.selected_queue == 1);
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 0x817, 0) == 0);
    CHECK(state.queues[1].valid == 0 && state.dirty == 0x20);
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 0x816, 0) == 0);
    CHECK(state.queues[1].valid == 1 && state.queues[1].item_count == 2);
    CHECK(trace.renames == 1 && trace.last_fill_count == 2);
    trace.production_mode = 1;
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 0x433, 0) == 0);
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 0x816, 0) == 0);
    CHECK(state.queues[2].item_count == 1);
    CHECK(state.queues[2].items[0] == 0xffc6U);
    trace.production_mode = 2;
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 0x434, 0) == 0);
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 0x816, 0) == 0);
    CHECK(state.queues[3].item_count == 12);
    CHECK(state.queues[3].items[11] == 0x02c4U);
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 0x41b, 0) == 0);
    CHECK(trace.renames == 4 && trace.focus == 4);
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 0x76, 0) == 1);
    CHECK(trace.helps == 1);
    CHECK(stars_zip_prod_dlg(&input, 20, 273, 1, 0) == 1);
    CHECK(trace.dialogs == 1 && trace.tutors >= 2);
#undef CHECK
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("ZPDR.TXT", "w");
        if (!report)
            return 7;
        fprintf(report, "ZIP_PROD_DLG_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#else
    if (failures == 0)
        printf("ZIP_PROD_DLG_CHECKS=%d FAILURES=0 RESULT=PASS\n", checks);
    else
        printf("ZIP_PROD_DLG_CHECKS=%d FAILURES=%d RESULT=FAIL\n",
            checks, failures);
#endif
    return failures == 0 ? 0 : 1;
}
