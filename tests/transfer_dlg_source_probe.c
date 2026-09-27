#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct TransferTrace {
    StarsRect client;
    StarsU16 dc;
    StarsI16 begin_calls;
    StarsI16 end_calls;
    StarsU16 paint_dc;
    StarsI16 draw_calls;
    StarsI16 draw_value;
    StarsI16 fill_calls;
    StarsU16 fill_brush;
    StarsI16 sticky_calls;
    StarsI16 sticky_mode;
    StarsI16 string_calls;
    StarsI16 string_id;
    StarsI16 title_calls;
    StarsU16 title_hwnd;
    StarsI16 resize_calls;
    StarsI16 resize_x;
    StarsI16 resize_y;
    StarsI16 resize_cx;
    StarsI16 resize_cy;
    StarsU16 resize_flags;
    StarsI16 metric4;
    StarsI16 metric7;
    StarsI16 item_calls;
    StarsU16 item_id;
    StarsI16 rect_calls;
    StarsI16 screen_calls;
    StarsI16 position_calls;
    StarsU16 position_hwnd;
    StarsI16 position_x;
    StarsI16 position_y;
    StarsI16 position_flags;
    StarsI16 setup_calls;
    StarsU16 setup_bottom;
    StarsI16 advance_calls;
    StarsI16 end_dialog_calls;
    StarsI16 end_value;
    StarsI16 help_calls;
    StarsU16 help_id;
    StarsI16 track_calls;
    StarsPoint track_point;
} TransferTrace;

static StarsU16 begin_paint(void *opaque, StarsU16 hwnd)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->begin_calls; (void)hwnd;
    return trace->dc;
}

static void end_paint(void *opaque, StarsU16 hwnd, StarsU16 dc)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->end_calls; trace->paint_dc = dc; (void)hwnd;
}

static void draw(void *opaque, StarsU16 hwnd, StarsU16 dc,
    const StarsRect *rect, StarsI16 value)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->draw_calls; trace->draw_value = value;
    (void)hwnd; (void)dc; (void)rect;
}

static void get_client_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->rect_calls; *rect = trace->client; (void)hwnd;
    if (trace->resize_calls)
        rect->bottom = 300U;
}

static void fill_rect(void *opaque, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->fill_calls; trace->fill_brush = brush;
    (void)dc; (void)rect;
}

static void sticky_position(void *opaque, StarsU16 hwnd, StarsPoint *point,
    StarsI16 initialize)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->sticky_calls; trace->sticky_mode = initialize;
    (void)hwnd; (void)point;
}

static StarsI16 get_string(void *opaque, StarsI16 id, StarsU8 *text,
    StarsU16 capacity)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->string_calls; trace->string_id = id;
    if (capacity > 3U) { text[0] = 'X'; text[1] = 'F'; text[2] = 'R';
        text[3] = 0; }
    return 3;
}

static void set_window_text(void *opaque, StarsU16 hwnd, const StarsU8 *text)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->title_calls; trace->title_hwnd = hwnd; (void)text;
}

static void set_window_pos(void *opaque, StarsU16 hwnd, StarsU16 insert,
    StarsI16 x, StarsI16 y, StarsI16 cx, StarsI16 cy, StarsU16 flags)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->position_calls; trace->position_hwnd = hwnd;
    trace->position_x = x; trace->position_y = y; trace->position_flags = flags;
    if (cx || cy) { ++trace->resize_calls; trace->resize_x = x;
        trace->resize_y = y; trace->resize_cx = cx; trace->resize_cy = cy;
        trace->resize_flags = flags; }
    (void)insert;
}

static StarsI16 get_metric(void *opaque, StarsU16 index)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    if (index == 4U) return trace->metric4;
    if (index == 7U) return trace->metric7;
    return 0;
}

static StarsU16 get_dialog_item(void *opaque, StarsU16 hwnd, StarsU16 id)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->item_calls; trace->item_id = id; (void)hwnd;
    return (StarsU16)(id + 100U);
}

static void get_window_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->screen_calls; (void)hwnd;
    rect->left = 50U; rect->top = 60U; rect->right = 80U; rect->bottom = 70U;
}

static void screen_to_client(void *opaque, StarsU16 hwnd, StarsPoint *point)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->screen_calls; point->x = (StarsI16)(point->x - 10);
    point->y = (StarsI16)(point->y - 20); (void)hwnd;
}

static void setup_buttons(void *opaque, const StarsRect *rect)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->setup_calls; trace->setup_bottom = rect->bottom;
}

static void advance_tutor(void *opaque)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->advance_calls;
}

static void end_dialog(void *opaque, StarsU16 hwnd, StarsI16 result)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->end_dialog_calls; trace->end_value = result; (void)hwnd;
}

static void win_help(void *opaque, StarsU16 hwnd, const StarsU8 *file,
    StarsU16 command, StarsU16 id)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->help_calls; trace->help_id = id;
    (void)hwnd; (void)file; (void)command;
}

static StarsI16 track(void *opaque, StarsU16 hwnd, StarsPoint point,
    StarsU16 buttons)
{
    TransferTrace *trace = (TransferTrace *)opaque;
    ++trace->track_calls; trace->track_point = point;
    (void)hwnd; (void)buttons;
    return 1;
}

static void setup(StarsTransferDlgInput *input, StarsTransferDlgOps *ops,
    TransferTrace *trace, StarsPoint *sticky)
{
    (void)memset(input, 0, sizeof(*input));
    (void)memset(ops, 0, sizeof(*ops));
    (void)memset(trace, 0, sizeof(*trace));
    trace->client.right = 300U; trace->client.bottom = 200U;
    trace->dc = 9U; trace->metric4 = 8; trace->metric7 = 12;
    input->button_face_brush = 17U; input->mode = 1U;
    input->valid_hulls = 11; input->sticky_point = sticky;
    input->help_file = (const StarsU8 *)"HELP";
    input->control_ids[0] = 1U; input->control_ids[1] = 0x76U;
    input->control_ids[2] = 2U; input->tutor_active = 1U;
    input->ops = ops; input->context = trace;
    ops->begin_paint = begin_paint; ops->end_paint = end_paint;
    ops->draw = draw; ops->get_client_rect = get_client_rect;
    ops->fill_rect = fill_rect; ops->sticky_position = sticky_position;
    ops->get_string = get_string; ops->set_window_text = set_window_text;
    ops->set_window_pos = set_window_pos; ops->get_system_metric = get_metric;
    ops->get_dialog_item = get_dialog_item;
    ops->get_window_rect = get_window_rect;
    ops->screen_to_client = screen_to_client;
    ops->setup_buttons = setup_buttons; ops->advance_tutor = advance_tutor;
    ops->end_dialog = end_dialog; ops->win_help = win_help; ops->track = track;
}

int main(void)
{
    StarsTransferDlgInput input;
    StarsTransferDlgOps ops;
    TransferTrace trace;
    StarsPoint sticky;
    int failures = 0;

    setup(&input, &ops, &trace, &sticky);
    if (stars_transfer_dlg(40U, 0x000fU, 0U, 0UL, &input) != 1 ||
            trace.begin_calls != 1 || trace.draw_calls != 1 ||
            trace.draw_value != -1 || trace.end_calls != 1) ++failures;
    if (stars_transfer_dlg(40U, 0x0014U, 6U, 0UL, &input) != 1 ||
            trace.fill_calls != 1 || trace.fill_brush != 17U) ++failures;
    if (stars_transfer_dlg(40U, 0x0110U, 0U, 0UL, &input) != 1 ||
            trace.sticky_mode != 1 || trace.string_id != 0x0367 ||
            trace.title_hwnd != 40U || trace.resize_calls != 1 ||
            trace.resize_cx != 300 || trace.resize_cy != 300 ||
            trace.position_calls != 4 || trace.position_x != 24 ||
            trace.position_y != 130 || trace.setup_bottom != 300U ||
            trace.advance_calls != 1) ++failures;
    if (stars_transfer_dlg(40U, 0x0111U, 1U, 0UL, &input) != 1 ||
            trace.end_value != 1 || trace.advance_calls != 2) ++failures;
    input.mode = 0U;
    if (stars_transfer_dlg(40U, 0x0111U, 0x0076U, 0UL, &input) != 1 ||
            trace.help_id != 0x0433U) ++failures;
    input.mode = 1U;
    if (stars_transfer_dlg(40U, 0x0111U, 0x0076U, 0UL, &input) != 1 ||
            trace.help_id != 0x0438U) ++failures;
    if (stars_transfer_dlg(40U, 0x0201U, 7U,
            (StarsU32)(9U | (10UL << 16)), &input) != 1 ||
            trace.track_point.x != 9 || trace.track_point.y != 10) ++failures;
    if (failures == 0)
        puts("TRANSFER_DLG_SOURCE=PASS");
    else
        puts("TRANSFER_DLG_SOURCE=FAIL");
    return failures;
}
