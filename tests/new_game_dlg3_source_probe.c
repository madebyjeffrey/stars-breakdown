#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct NewGame3Trace {
    StarsU16 client;
    StarsU16 dc;
    StarsU16 paint_hwnd;
    StarsU16 paint_dc;
    StarsI16 draw_value;
    StarsU16 erase_dc;
    StarsU16 erase_brush;
    StarsU32 background;
    StarsU16 background_dc;
    StarsU16 item_offset;
    StarsU16 item_handle;
    StarsPoint cursor;
    StarsPoint hit_point;
    StarsI16 hit_result;
    StarsU16 cursor_set;
    StarsI16 title_step;
    StarsI16 vc_values[8];
    StarsI16 vc_get_calls;
    StarsI16 vc_set_calls;
    StarsI16 vc_set_index;
    StarsI16 vc_set_value;
    StarsI16 enable_calls;
    StarsU16 enabled_handle;
    StarsU16 enabled_value;
    StarsI16 message_calls;
    StarsU16 message_handle;
    StarsU16 message_id;
    StarsU16 message_wparam;
    StarsU32 message_lparam;
    StarsU32 message_result;
    StarsI16 sticky_mode;
    StarsI16 sticky_calls;
    StarsI16 end_calls;
    StarsI16 end_value;
    StarsU16 help_command;
    StarsU16 help_id;
    StarsI16 track_calls;
    StarsU16 track_hwnd;
    StarsPoint track_point;
    StarsI16 track_keyboard;
} NewGame3Trace;

static StarsU16 begin_paint(void *opaque, StarsU16 hwnd)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    trace->paint_hwnd = hwnd;
    return trace->dc;
}

static void end_paint(void *opaque, StarsU16 hwnd, StarsU16 dc)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    trace->paint_hwnd = hwnd; trace->paint_dc = dc;
}

static void draw(void *opaque, StarsU16 hwnd, StarsU16 dc, StarsI16 value)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    trace->paint_hwnd = hwnd; trace->paint_dc = dc; trace->draw_value = value;
}

static void get_client_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    (void)hwnd;
    rect->left = 1U; rect->top = 2U; rect->right = 3U; rect->bottom = 4U;
    trace->client = hwnd;
}

static void fill_rect(void *opaque, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    (void)rect;
    trace->erase_dc = dc; trace->erase_brush = brush;
}

static StarsU16 get_dialog_item(void *opaque, StarsU16 hwnd, StarsU16 id)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    (void)hwnd;
    trace->item_offset = id;
    return (StarsU16)(id + 1000U);
}

static void set_background(void *opaque, StarsU16 dc, StarsU32 color)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    trace->background_dc = dc; trace->background = color;
}

static void get_cursor_pos(void *opaque, StarsPoint *point)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    *point = trace->cursor;
}

static void screen_to_client(void *opaque, StarsU16 hwnd, StarsPoint *point)
{
    (void)opaque; (void)hwnd;
    point->x = (StarsU16)(point->x - 5U);
    point->y = (StarsU16)(point->y - 7U);
}

static StarsI16 hit_test(void *opaque, StarsPoint point)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    trace->hit_point = point;
    return trace->hit_result;
}

static void set_cursor(void *opaque, StarsU16 cursor)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    trace->cursor_set = cursor;
}

static void set_title(void *opaque, StarsU16 hwnd, StarsI16 step)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    (void)hwnd; trace->title_step = step;
}

static StarsI16 get_vc_check(void *opaque, StarsI16 index)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    ++trace->vc_get_calls;
    return trace->vc_values[index];
}

static void enable_window(void *opaque, StarsU16 hwnd, StarsU16 enable)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    ++trace->enable_calls; trace->enabled_handle = hwnd;
    trace->enabled_value = enable;
}

static StarsU32 send_message(void *opaque, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    ++trace->message_calls; trace->message_handle = hwnd;
    trace->message_id = message; trace->message_wparam = wparam;
    trace->message_lparam = lparam;
    return trace->message_result;
}

static StarsI16 is_button_checked(void *opaque, StarsU16 hwnd, StarsU16 id)
{
    (void)opaque; (void)hwnd; (void)id;
    return 0;
}

static void set_vc_check(void *opaque, StarsI16 index, StarsI16 value)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    ++trace->vc_set_calls; trace->vc_set_index = index;
    trace->vc_set_value = value;
}

static void sticky_position(void *opaque, StarsU16 hwnd, StarsPoint *point,
    StarsI16 initialize)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    ++trace->sticky_calls; trace->sticky_mode = initialize;
    (void)hwnd; (void)point;
}

static void end_dialog(void *opaque, StarsU16 hwnd, StarsI16 result)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    ++trace->end_calls; trace->end_value = result;
    (void)hwnd;
}

static void win_help(void *opaque, StarsU16 hwnd, const StarsU8 *file,
    StarsU16 command, StarsU16 id)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    trace->help_command = command; trace->help_id = id;
    (void)hwnd; (void)file;
}

static StarsI16 track(void *opaque, StarsU16 hwnd, StarsPoint point,
    StarsI16 keyboard)
{
    NewGame3Trace *trace = (NewGame3Trace *)opaque;
    ++trace->track_calls; trace->track_hwnd = hwnd;
    trace->track_point = point; trace->track_keyboard = keyboard;
    return 1;
}

static void setup(StarsNewGameDlg3Input *input, StarsNewGameDlg3Ops *ops,
    NewGame3Trace *trace, StarsU16 *race_ids, StarsPoint *sticky)
{
    (void)memset(input, 0, sizeof(*input));
    (void)memset(ops, 0, sizeof(*ops));
    (void)memset(trace, 0, sizeof(*trace));
    race_ids[0] = 500U; race_ids[1] = 501U;
    race_ids[2] = 502U; race_ids[3] = 503U;
    input->button_face_brush = 17U; input->button_face_color = 0x112233UL;
    input->hand_cursor = 88U; input->help_file = (const StarsU8 *)"HELP";
    input->sticky_point = sticky; input->race_button_ids = race_ids;
    input->race_button_count = 4U; input->read_only = 1U;
    input->ops = ops; input->context = trace; trace->dc = 9U;
    trace->cursor.x = 20U; trace->cursor.y = 30U; trace->hit_result = 2;
    trace->vc_values[0] = 1; trace->vc_values[1] = 2;
    trace->vc_values[3] = 4; trace->vc_values[4] = 5;
    trace->vc_values[5] = 6; trace->vc_values[6] = 7;
    trace->vc_values[7] = 8;
    ops->begin_paint = begin_paint; ops->end_paint = end_paint;
    ops->draw = draw; ops->get_client_rect = get_client_rect;
    ops->fill_rect = fill_rect; ops->get_dialog_item = get_dialog_item;
    ops->set_background_color = set_background;
    ops->get_cursor_pos = get_cursor_pos;
    ops->screen_to_client = screen_to_client; ops->hit_test = hit_test;
    ops->set_cursor = set_cursor; ops->set_title = set_title;
    ops->get_vc_check = get_vc_check; ops->enable_window = enable_window;
    ops->send_message = send_message;
    ops->is_button_checked = is_button_checked;
    ops->set_vc_check = set_vc_check;
    ops->sticky_position = sticky_position; ops->end_dialog = end_dialog;
    ops->win_help = win_help; ops->track = track;
}

int main(void)
{
    StarsNewGameDlg3Input input;
    StarsNewGameDlg3Ops ops;
    NewGame3Trace trace;
    StarsU16 race_ids[4];
    StarsPoint sticky;
    int failures = 0;

    setup(&input, &ops, &trace, race_ids, &sticky);
    if (stars_new_game_dlg3(40U, 0x000fU, 0U, 0UL, &input) != 1 ||
            trace.paint_hwnd != 40U || trace.paint_dc != 9U ||
            trace.draw_value != -1) ++failures;
    if (stars_new_game_dlg3(40U, 0x0014U, 6U, 0UL, &input) != 1 ||
            trace.erase_dc != 6U || trace.erase_brush != 17U) ++failures;
    if (stars_new_game_dlg3(40U, 0x0019U, 6U,
            (StarsU32)(1293U | (1UL << 16)), &input) != 17 ||
            trace.background != 0x112233UL) ++failures;
    if (stars_new_game_dlg3(40U, 0x0019U, 6U,
            (StarsU32)(99U | (6UL << 16)), &input) != 17) ++failures;
    if (stars_new_game_dlg3(40U, 0x0020U, 0U, 0UL, &input) != 1 ||
            trace.hit_point.x != 15U || trace.hit_point.y != 23U ||
            trace.cursor_set != 88U) ++failures;
    if (stars_new_game_dlg3(40U, 0x0110U, 0U, 0UL, &input) != 1 ||
            trace.title_step != 3 || trace.vc_get_calls != 7 ||
            trace.message_calls != 7 || trace.enable_calls != 7 ||
            trace.sticky_mode != 1) ++failures;
    if (stars_new_game_dlg3(40U, 0x0111U, 502U, 0UL, &input) != 1 ||
            trace.end_value != 2 || trace.sticky_mode != 0) ++failures;
    trace.message_result = 1UL;
    if (stars_new_game_dlg3(40U, 0x0111U, 0x0125U, 0UL, &input) != 0 ||
            trace.vc_set_index != 3 || trace.vc_set_value != 1 ||
            trace.draw_value != 8) ++failures;
    if (stars_new_game_dlg3(40U, 0x0111U, 0x0076U, 0UL, &input) != 1 ||
            trace.help_command != 1U || trace.help_id != 0x03fdU) ++failures;
    if (stars_new_game_dlg3(40U, 0x0201U, 3U,
            (StarsU32)(9U | (10UL << 16)), &input) != 1 ||
            trace.track_point.x != 9U || trace.track_point.y != 10U ||
            trace.track_keyboard != 3) ++failures;
    if (failures == 0)
        puts("NEW_GAME_DLG3_SOURCE=PASS");
    else
        puts("NEW_GAME_DLG3_SOURCE=FAIL");
    return failures;
}
