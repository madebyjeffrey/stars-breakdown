#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct MergeTrace {
    StarsU16 list;
    StarsRect rect;
    StarsU16 fill_dc;
    StarsU16 fill_brush;
    StarsU32 background;
    StarsI16 names;
    StarsI16 special_names;
    StarsI16 added;
    StarsI16 selections_set;
    StarsI16 selections_read;
    StarsI16 selection_values[4];
    StarsU16 last_set_value;
    StarsU32 last_set_index;
    StarsI16 advance_calls;
    StarsI16 confirm_result;
    StarsI16 confirm_calls;
    StarsI16 sticky_calls;
    StarsI16 sticky_mode;
    StarsI16 end_calls;
    StarsI16 end_value;
    StarsI16 help_calls;
    StarsU16 help_id;
} MergeTrace;

static void get_client_rect(void *opaque, StarsU16 hwnd, StarsRect *rect)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    (void)hwnd;
    *rect = trace->rect;
}

static void fill_rect(void *opaque, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    (void)rect;
    trace->fill_dc = dc; trace->fill_brush = brush;
}

static void set_background(void *opaque, StarsU16 dc, StarsU32 color)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    (void)dc; trace->background = color;
}

static StarsU16 get_dialog_item(void *opaque, StarsU16 hwnd, StarsU16 id)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    (void)hwnd; (void)id;
    return trace->list;
}

static void sticky_position(void *opaque, StarsU16 hwnd, StarsPoint *point,
    StarsI16 initialize)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    ++trace->sticky_calls; trace->sticky_mode = initialize;
    (void)hwnd; (void)point;
}

static StarsI16 get_name(void *opaque, StarsI16 fleet_id, StarsU8 *text,
    StarsU16 capacity)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    ++trace->names;
    if (capacity > 2U) { text[0] = 'F'; text[1] = (StarsU8)fleet_id;
        text[2] = 0; }
    return 2;
}

static StarsI16 is_special(void *opaque, StarsI16 fleet_id)
{
    (void)opaque;
    return fleet_id == 22 ? 1 : 0;
}

static void append_special(void *opaque, StarsU8 *text)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    ++trace->special_names;
    (void)text;
}

static void add_string(void *opaque, StarsU16 list, const StarsU8 *text)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    ++trace->added;
    (void)list; (void)text;
}

static StarsU32 send_message(void *opaque, StarsU16 list, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    (void)list;
    if (message == 0x0406U) {
        ++trace->selections_set; trace->last_set_value = wparam;
        trace->last_set_index = lparam;
    } else if (message == 0x0408U) {
        ++trace->selections_read;
        (void)lparam;
        if (wparam < 4U) return (StarsU32)trace->selection_values[wparam];
    }
    return 0UL;
}

static void advance_tutor(void *opaque)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    ++trace->advance_calls;
}

static StarsI16 confirm_merge(void *opaque)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    ++trace->confirm_calls;
    return trace->confirm_result;
}

static void end_dialog(void *opaque, StarsU16 hwnd, StarsI16 result)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    ++trace->end_calls; trace->end_value = result;
    (void)hwnd;
}

static void win_help(void *opaque, StarsU16 hwnd, const StarsU8 *file,
    StarsU16 command, StarsU16 id)
{
    MergeTrace *trace = (MergeTrace *)opaque;
    ++trace->help_calls; trace->help_id = id;
    (void)hwnd; (void)file; (void)command;
}

static void setup(StarsMergeFleetsInput *input, StarsMergeFleetsOps *ops,
    MergeTrace *trace, StarsI16 *indices, const StarsI16 *fleet_ids,
    StarsPoint *sticky)
{
    (void)memset(input, 0, sizeof(*input));
    (void)memset(ops, 0, sizeof(*ops));
    (void)memset(trace, 0, sizeof(*trace));
    trace->list = 77U; trace->rect.right = 100U; trace->rect.bottom = 200U;
    input->button_face_brush = 17U; input->button_face_color = 0x123456UL;
    input->sticky_point = sticky; input->help_file = (const StarsU8 *)"HELP";
    input->list_control_id = 0x51U; input->fleet_count = 3;
    input->merge_indices = indices; input->fleet_ids = fleet_ids;
    input->selected_fleet_id = 22; input->tutor_active = 1U;
    input->ops = ops; input->context = trace; trace->confirm_result = 1;
    ops->get_client_rect = get_client_rect; ops->fill_rect = fill_rect;
    ops->set_background_color = set_background;
    ops->get_dialog_item = get_dialog_item;
    ops->sticky_position = sticky_position; ops->get_fleet_name = get_name;
    ops->is_special = is_special; ops->append_special = append_special;
    ops->add_string = add_string; ops->send_message = send_message;
    ops->advance_tutor = advance_tutor; ops->confirm_merge = confirm_merge;
    ops->end_dialog = end_dialog; ops->win_help = win_help;
}

int main(void)
{
    StarsMergeFleetsInput input;
    StarsMergeFleetsOps ops;
    MergeTrace trace;
    StarsI16 indices[3] = { 0, 1, 2 };
    const StarsI16 fleet_ids[3] = { 10, 22, 30 };
    StarsPoint sticky;
    int failures = 0;

    setup(&input, &ops, &trace, indices, fleet_ids, &sticky);
    if (stars_merge_fleets_dlg(40U, 0x0014U, 6U, 0UL, &input) != 1 ||
            trace.fill_dc != 6U || trace.fill_brush != 17U) ++failures;
    if (stars_merge_fleets_dlg(40U, 0x0019U, 6U, 77UL, &input) != 0 ||
            stars_merge_fleets_dlg(40U, 0x0019U, 6U, 99UL, &input) != 17 ||
            trace.background != 0x123456UL) ++failures;
    if (stars_merge_fleets_dlg(40U, 0x0110U, 0U, 0UL, &input) != 1 ||
            trace.names != 3 || trace.added != 3 || trace.special_names != 1 ||
            trace.selections_set != 3 || trace.advance_calls != 1 ||
            trace.sticky_mode != 1) ++failures;
    trace.selection_values[0] = 1; trace.selection_values[1] = 0;
    trace.selection_values[2] = 1; trace.confirm_result = 0;
    if (stars_merge_fleets_dlg(40U, 0x0111U, 1U, 0UL, &input) != 1 ||
            indices[0] != 0 || indices[1] != -1 || indices[2] != 2 ||
            trace.confirm_calls != 1 || trace.end_calls != 0) ++failures;
    trace.confirm_result = 1;
    if (stars_merge_fleets_dlg(40U, 0x0111U, 1U, 0UL, &input) != 1 ||
            trace.end_value != 1 || trace.sticky_mode != 0) ++failures;
    if (stars_merge_fleets_dlg(40U, 0x0111U, 2U, 0UL, &input) != 1 ||
            trace.end_value != 0) ++failures;
    if (stars_merge_fleets_dlg(40U, 0x0111U, 0x07f8U, 0UL, &input) != 1 ||
            trace.last_set_value != 1U || trace.last_set_index != 2UL ||
            stars_merge_fleets_dlg(40U, 0x0111U, 0x07f9U, 0UL, &input) != 1 ||
            trace.last_set_value != 0U) ++failures;
    if (stars_merge_fleets_dlg(40U, 0x0111U, 0x0076U, 0UL, &input) != 1 ||
            trace.help_calls != 1 || trace.help_id != 0x0453U) ++failures;
    if (failures == 0)
        puts("MERGE_FLEETS_DLG_SOURCE=PASS");
    else
        puts("MERGE_FLEETS_DLG_SOURCE=FAIL");
    return failures;
}
