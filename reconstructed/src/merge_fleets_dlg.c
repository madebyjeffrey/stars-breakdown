#include "core.h"

#define MERGE_FLEETS_WM_ERASE_BACKGROUND 0x0014U
#define MERGE_FLEETS_WM_CONTROL_COLOR 0x0019U
#define MERGE_FLEETS_WM_INIT_DIALOG 0x0110U
#define MERGE_FLEETS_WM_COMMAND 0x0111U
#define MERGE_FLEETS_ID_LIST 0x0051U
#define MERGE_FLEETS_ID_OK 1U
#define MERGE_FLEETS_ID_CANCEL 2U
#define MERGE_FLEETS_ID_HELP 0x0076U
#define MERGE_FLEETS_ID_SELECT_ALL 0x07f8U
#define MERGE_FLEETS_ID_SELECT_NONE 0x07f9U
#define MERGE_FLEETS_LIST_ADD_STRING 0x0401U
#define MERGE_FLEETS_LIST_SET_SELECTION 0x0406U
#define MERGE_FLEETS_LIST_GET_SELECTION 0x0408U
#define MERGE_FLEETS_HELP_CONTEXT 0x0453U

static StarsU16 merge_fleets_list(const StarsMergeFleetsInput *input,
    StarsU16 hwnd)
{
    return input->ops->get_dialog_item ?
        input->ops->get_dialog_item(input->context, hwnd,
            input->list_control_id) : input->list_control_id;
}

static StarsI16 merge_fleets_id(const StarsMergeFleetsInput *input,
    StarsI16 slot)
{
    if (!input->merge_indices || !input->fleet_ids)
        return -1;
    return input->fleet_ids[input->merge_indices[slot]];
}

/* NE 17:3376-36B5.  Populate the merge list, preserve its selected rows,
 * and commit or veto the merge while retaining the original tutor hook. */
StarsI16 stars_merge_fleets_dlg(StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam, StarsMergeFleetsInput *input)
{
    const StarsMergeFleetsOps *ops;
    StarsRect rect;
    StarsU8 name[80];
    StarsU16 list;
    StarsI16 index;
    StarsI16 selected;

    (void)lparam;
    if (!input || !input->ops)
        return 0;
    ops = input->ops;
    if (message == MERGE_FLEETS_WM_ERASE_BACKGROUND) {
        if (ops->get_client_rect)
            ops->get_client_rect(input->context, hwnd, &rect);
        if (ops->fill_rect)
            ops->fill_rect(input->context, wparam, &rect,
                input->button_face_brush);
        return 1;
    }
    if (message == MERGE_FLEETS_WM_CONTROL_COLOR) {
        list = merge_fleets_list(input, hwnd);
        if ((StarsU16)lparam != list) {
            if (ops->set_background_color)
                ops->set_background_color(input->context, wparam,
                    input->button_face_color);
            return (StarsI16)input->button_face_brush;
        }
        return 0;
    }
    if (message == MERGE_FLEETS_WM_INIT_DIALOG) {
        if (ops->sticky_position)
            ops->sticky_position(input->context, hwnd, input->sticky_point, 1);
        list = merge_fleets_list(input, hwnd);
        for (index = 0; index < input->fleet_count; ++index) {
            name[0] = 0;
            if (ops->get_fleet_name)
                (void)ops->get_fleet_name(input->context,
                    merge_fleets_id(input, index),
                    name, (StarsU16)sizeof name);
            if (ops->is_special && ops->is_special(input->context,
                    merge_fleets_id(input, index)) &&
                    ops->append_special)
                ops->append_special(input->context, name);
            if (ops->add_string)
                ops->add_string(input->context, list, name);
            selected = (input->fleet_count == 2 ||
                merge_fleets_id(input, index) == input->selected_fleet_id) ?
                1 : 0;
            if (ops->send_message)
                (void)ops->send_message(input->context, list,
                    MERGE_FLEETS_LIST_SET_SELECTION, (StarsU16)selected,
                    (StarsU32)(StarsU16)index);
        }
        if (input->tutor_active && ops->advance_tutor)
            ops->advance_tutor(input->context);
        return 1;
    }
    if (message != MERGE_FLEETS_WM_COMMAND)
        return 0;
    if (wparam == MERGE_FLEETS_ID_OK || wparam == MERGE_FLEETS_ID_CANCEL) {
        list = merge_fleets_list(input, hwnd);
        for (index = 0; index < input->fleet_count; ++index) {
            selected = ops->send_message ? (StarsI16)ops->send_message(
                input->context, list, MERGE_FLEETS_LIST_GET_SELECTION,
                (StarsU16)index, 0UL) : 0;
            if (!selected && input->merge_indices)
                input->merge_indices[index] = -1;
        }
        if (wparam == MERGE_FLEETS_ID_OK && input->tutor_active &&
                ops->confirm_merge && !ops->confirm_merge(input->context))
            return 1;
        if (ops->sticky_position)
            ops->sticky_position(input->context, hwnd, input->sticky_point, 0);
        if (ops->end_dialog)
            ops->end_dialog(input->context, hwnd,
                (StarsI16)(wparam == MERGE_FLEETS_ID_OK));
        return 1;
    }
    if (wparam == MERGE_FLEETS_ID_HELP) {
        if (ops->win_help)
            ops->win_help(input->context, hwnd, input->help_file, 1,
                MERGE_FLEETS_HELP_CONTEXT);
        return 1;
    }
    if (wparam == MERGE_FLEETS_ID_SELECT_ALL ||
            wparam == MERGE_FLEETS_ID_SELECT_NONE) {
        list = merge_fleets_list(input, hwnd);
        for (index = 0; index < input->fleet_count; ++index)
            if (ops->send_message)
                (void)ops->send_message(input->context, list,
                    MERGE_FLEETS_LIST_SET_SELECTION,
                    (StarsU16)(wparam == MERGE_FLEETS_ID_SELECT_ALL),
                    (StarsU32)(StarsU16)index);
        return 1;
    }
    return 0;
}
