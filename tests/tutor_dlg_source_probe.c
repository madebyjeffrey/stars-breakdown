#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct TutorTrace {
    StarsRect rect;
    StarsU16 draw_hwnd;
    StarsU16 fill_dc;
    StarsU16 fill_brush;
    StarsU16 menu;
    StarsU16 menu_item;
    StarsU16 menu_check;
    StarsU16 post_hwnd;
    StarsU16 post_message;
    StarsU16 post_wparam;
    StarsU32 post_lparam;
    StarsU16 position_hwnd;
    StarsU16 position_insert;
    StarsI16 position_x;
    StarsI16 position_y;
    StarsI16 position_cx;
    StarsI16 position_cy;
    StarsU16 position_flags;
    StarsPoint sticky;
    StarsI16 sticky_mode;
    StarsU32 proc;
    StarsU32 freed_proc;
    StarsI16 dialog_result;
    StarsU16 dialog_resource;
    StarsU16 dialog_owner;
    StarsI16 show_value;
    StarsU16 alert_id;
    StarsI16 alert_type;
    StarsU16 help_id;
    StarsI16 end_dialog_result;
    StarsI16 end_tutor_value;
    StarsI16 calls;
} TutorTrace;

static void get_client_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    (void)hwnd;
    *rect = trace->rect;
}

static void fill_rect(void *opaque, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->fill_dc = dc;
    trace->fill_brush = brush;
    trace->rect = *rect;
}

static void draw_text(void *opaque, StarsU16 hwnd)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->draw_hwnd = hwnd;
}

static void sticky_position(void *opaque, StarsU16 hwnd,
    StarsPoint *point, StarsI16 initialize)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->sticky_mode = initialize;
    trace->sticky = *point;
    trace->position_hwnd = hwnd;
}

static StarsU16 get_sub_menu(void *opaque, StarsU16 frame, StarsU16 index)
{
    (void)opaque; (void)frame; (void)index;
    return 77U;
}

static void check_menu(void *opaque, StarsU16 menu, StarsU16 item,
    StarsU16 check)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->menu = menu; trace->menu_item = item; trace->menu_check = check;
}

static void post_message(void *opaque, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->post_hwnd = hwnd; trace->post_message = message;
    trace->post_wparam = wparam; trace->post_lparam = lparam;
}

static void end_dialog(void *opaque, StarsU16 hwnd, StarsI16 result)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    (void)hwnd;
    trace->end_dialog_result = result;
}

static void set_window_pos(void *opaque, StarsU16 hwnd, StarsU16 insert,
    StarsI16 x, StarsI16 y, StarsI16 cx, StarsI16 cy, StarsU16 flags)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->position_hwnd = hwnd; trace->position_insert = insert;
    trace->position_x = x; trace->position_y = y;
    trace->position_cx = cx; trace->position_cy = cy;
    trace->position_flags = flags;
}

static StarsU32 make_proc(void *opaque, StarsU16 instance)
{
    (void)opaque; (void)instance;
    return 123UL;
}

static StarsI16 dialog_box(void *opaque, StarsU16 instance,
    StarsU16 resource, StarsU16 owner, StarsU32 proc)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    (void)instance;
    trace->dialog_resource = resource; trace->dialog_owner = owner;
    trace->proc = proc;
    return trace->dialog_result;
}

static void free_proc(void *opaque, StarsU32 proc)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->freed_proc = proc;
}

static void show_tutor(void *opaque, StarsI16 show)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->show_value = show;
}

static void end_tutor(void *opaque, StarsI16 value)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->end_tutor_value = value;
}

static const StarsU8 *format_ids(void *opaque, StarsU16 id)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    trace->alert_id = id;
    return (const StarsU8 *)"alert";
}

static StarsI16 alert(void *opaque, const StarsU8 *text, StarsI16 type)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    (void)text;
    trace->alert_type = type;
    return 0;
}

static void win_help(void *opaque, StarsU16 hwnd, const StarsU8 *file,
    StarsU16 command, StarsU16 id)
{
    TutorTrace *trace = (TutorTrace *)opaque;
    (void)hwnd; (void)file; (void)command;
    trace->help_id = id;
}

static void setup(StarsTutorDialogInput *input,
    StarsTutorDialogOps *ops, TutorTrace *trace, StarsU16 *window,
    StarsU16 *flags, StarsPoint *sticky)
{
    (void)memset(input, 0, sizeof(*input));
    (void)memset(ops, 0, sizeof(*ops));
    (void)memset(trace, 0, sizeof(*trace));
    input->hwnd = 42U; input->frame_window = 50U; input->instance = 9U;
    input->tutor_window = window; input->tutor_flags = flags;
    input->tutor_help_id = 0x1234U; input->sticky_point = sticky;
    input->help_file = (const StarsU8 *)"HELP";
    input->button_face_brush = 11U; input->ops = ops; input->context = trace;
    ops->sticky_position = sticky_position;
    ops->get_sub_menu = get_sub_menu; ops->check_menu = check_menu;
    ops->draw_text = draw_text; ops->get_client_rect = get_client_rect;
    ops->fill_rect = fill_rect; ops->end_dialog = end_dialog;
    ops->post_message = post_message;
    ops->set_window_pos = set_window_pos; ops->make_proc = make_proc;
    ops->dialog_box = dialog_box; ops->free_proc = free_proc;
    ops->show_tutor = show_tutor; ops->end_tutor = end_tutor;
    ops->format_ids = format_ids; ops->alert = alert; ops->win_help = win_help;
}

int main(void)
{
    StarsTutorDialogInput input;
    StarsTutorDialogOps ops;
    TutorTrace trace;
    StarsU16 window;
    StarsU16 flags;
    StarsPoint sticky;
    int failures = 0;

    setup(&input, &ops, &trace, &window, &flags, &sticky);
    input.message = 0x0110U;
    if (stars_tutor_dlg(&input) != 1 || window != 42U ||
            trace.position_insert != 0xffffU || trace.position_flags != 3U ||
            trace.sticky_mode != 1) ++failures;
    input.message = 0x000fU;
    if (stars_tutor_dlg(&input) != 1 || trace.draw_hwnd != 42U) ++failures;
    trace.rect.left = 1; trace.rect.top = 2; trace.rect.right = 3;
    trace.rect.bottom = 4; input.message = 0x0014U; input.wparam = 9U;
    if (stars_tutor_dlg(&input) != 1 || trace.fill_dc != 9U ||
            trace.fill_brush != 11U) ++failures;
    input.message = 0x0102U; input.wparam = 65U; input.lparam = 0x12345678UL;
    if (stars_tutor_dlg(&input) != 0 || trace.post_hwnd != 50U ||
            trace.post_message != 0x0102U || trace.post_wparam != 65U ||
            trace.post_lparam != 0x12345678UL) ++failures;
    flags = (StarsU16)(0x4000U | 0x0010U); input.message = 0x0111U;
    input.wparam = 2U;
    if (stars_tutor_dlg(&input) != 1 || trace.show_value != 0 ||
            trace.alert_id != 0x051aU || trace.alert_type != 0x40 ||
            flags != 0x0010U) ++failures;
    input.wparam = 0x76U;
    if (stars_tutor_dlg(&input) != 1 || trace.help_id != 0x1234U) ++failures;
    trace.dialog_result = 0x09ca; flags = 0U; input.wparam = 0x09c7U;
    if (stars_tutor_dlg(&input) != 0 || trace.proc != 123UL ||
            trace.freed_proc != 123UL || flags != 0x0200U ||
            trace.post_message != 0x0466U ||
            trace.post_wparam != 0x09caU) ++failures;
    input.message = 2U;
    if (stars_tutor_dlg(&input) != 1 || window != 0U ||
            trace.menu != 77U || trace.menu_item != 0x09c5U ||
            trace.menu_check != 0U || trace.end_dialog_result != 1 ||
            trace.end_tutor_value != 1) ++failures;

    if (failures == 0)
        puts("TUTOR_DLG_SOURCE=PASS");
    else
        puts("TUTOR_DLG_SOURCE=FAIL");
    return failures;
}
