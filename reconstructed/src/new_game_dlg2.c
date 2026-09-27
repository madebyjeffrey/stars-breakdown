#include "core.h"

enum {
    NEW_GAME_DLG2_PACKED_PLAYER_SIZE = 192,
    NEW_GAME_DLG2_RACE_FILE_SIZE = 13
};

void stars_new_game_dlg2_resource_contract(
    StarsNewGameDlg2ResourceContract *contract)
{
    static const StarsNewGameDlg2ResourceControl controls[5] = {
        {1072U, 210, 190, 40, 14, 7, 647U},
        {118U, 10, 190, 40, 14, 5, 431U},
        {2U, 60, 190, 40, 14, 6, 582U},
        {1070U, 110, 190, 40, 14, 7, 499U},
        {1071U, 160, 190, 40, 14, 7, 547U}
    };
    StarsI16 i;

    if (!contract)
        return;
    contract->resource_id = 0x0187U;
    contract->width = 261;
    contract->height = 210;
    contract->font_size = 8;
    contract->control_count = 5;
    for (i = 0; i < 5; ++i)
        contract->controls[i] = controls[i];
}

/* Typed semantic reconstruction of NewGameDlg2, NE 10:87d2-9556.
 * Menu construction/localization, packed race records, password checking, and
 * the Win16 coordinate/layout calls are callback-owned. The row hit-testing,
 * command gating, popup-result decoding, and player-type propagation stay here. */

static StarsI16 dlg2_row(const StarsNewGameDlg2Input *input,
    StarsI16 x, StarsI16 y)
{
    StarsI16 offset;
    StarsI16 row;

    if (input->row_height <= 0 || input->row_height_active <= 0 ||
            x < input->x_diamond ||
            x >= input->x_diamond + input->diamond_width ||
            y < input->row_top)
        return -1;
    offset = (StarsI16)(y - input->row_top);
    row = (StarsI16)(offset / input->row_height);
    if (row < 0 || row >= input->player_type_count ||
            offset % input->row_height >= input->row_height_active)
        return -1;
    return row;
}

static StarsI16 dlg2_command_row(const StarsNewGameDlg2Input *input,
    StarsU16 command)
{
    StarsI16 i;

    for (i = 0; i < input->race_button_count && i < 4; ++i)
        if (input->race_button_ids[i] == command)
            return i;
    return -1;
}

static StarsI16 dlg2_control_color(StarsU16 hwnd, StarsU16 dc,
    StarsU32 l_param, StarsNewGameDlg2Input *input)
{
    StarsU16 item;
    StarsU16 id;
    StarsI16 matched;

    matched = 0;
    if (input->ops->get_dialog_item) {
        for (id = 0x191; id <= 0x1c0; ++id) {
            item = input->ops->get_dialog_item(input->ops->context, hwnd,
                id);
            if (item == (StarsU16)l_param) {
                matched = 1;
                break;
            }
        }
    }
    if (!matched && (StarsU16)(l_param >> 16) != 6U)
        return 0;
    if (input->ops->set_background_color)
        input->ops->set_background_color(input->ops->context, dc,
            input->button_face_color);
    return (StarsI16)input->button_face_brush;
}

static StarsU8 dlg2_decode_selection(StarsI16 selection,
    StarsI16 *is_change)
{
    *is_change = 1;
    if (selection >= 0x10 && selection <= 0x1f)
        return (StarsU8)(((selection - 0x10) << 2) | 1);
    if (selection >= 0x20 && selection <= 0x2f)
        return (StarsU8)(((selection - 0x20) << 2) | 2);
    if (selection >= 0x30)
        return (StarsU8)(((selection - 0x30) << 2) | 3);
    *is_change = 0;
    return 0;
}

static void dlg2_propagate_type(StarsNewGameDlg2Input *input,
    StarsI16 row, StarsU8 type)
{
    StarsI16 i;

    input->player_types[row] = type;
    if (type == 0) {
        for (i = (StarsI16)(row + 1); i < input->player_type_count; ++i)
            input->player_types[i] = 0;
    } else {
        for (i = (StarsI16)(row - 1); i >= 0 &&
                input->player_types[i] == 0; --i)
            input->player_types[i] = type;
    }
}

static void dlg2_commit_packed_race(StarsNewGameDlg2Input *input,
    StarsI16 row)
{
    StarsU16 player_stride;
    StarsU16 file_stride;
    StarsU16 player_offset;
    StarsU16 file_offset;
    StarsU16 i;

    if (row < 0 || row >= input->player_type_count)
        return;
    player_stride = input->packed_player_stride;
    if (player_stride == 0U)
        player_stride = NEW_GAME_DLG2_PACKED_PLAYER_SIZE;
    file_stride = input->race_file_stride;
    if (file_stride == 0U)
        file_stride = NEW_GAME_DLG2_RACE_FILE_SIZE;
    player_offset = (StarsU16)(row * player_stride);
    file_offset = (StarsU16)(row * file_stride);
    if (input->packed_player_record && input->packed_players)
        for (i = 0; i < NEW_GAME_DLG2_PACKED_PLAYER_SIZE; ++i)
            input->packed_players[(StarsU16)(player_offset + i)] =
                input->packed_player_record[i];
    if (input->race_file && input->race_files)
        for (i = 0; i < NEW_GAME_DLG2_RACE_FILE_SIZE; ++i)
            input->race_files[(StarsU16)(file_offset + i)] =
                input->race_file[i];
}

static StarsI16 dlg2_handle_popup(StarsU16 hwnd, StarsI16 message,
    StarsI16 row, StarsI16 x, StarsI16 y, StarsNewGameDlg2Input *input)
{
    StarsI16 selection;
    StarsI16 change;
    StarsI16 action_result;
    StarsU8 old_type;
    StarsU8 new_type;

    if (!input->ops->track_popup)
        return 0;
    old_type = input->player_types[row];
    selection = input->ops->track_popup(input->ops->context, hwnd, old_type,
        message, x, y);
    new_type = old_type;
    if (selection == 0) {
        action_result = input->ops->create_race ?
            input->ops->create_race(input->ops->context, hwnd, row, old_type) :
            -1;
        if (action_result >= 0) {
            dlg2_commit_packed_race(input, row);
            new_type = (StarsU8)((action_result << 2) | 2);
        }
    } else if (selection == 1) {
        action_result = input->ops->open_game ?
            input->ops->open_game(input->ops->context, hwnd, row, old_type) :
            -1;
        if (action_result >= 0) {
            dlg2_commit_packed_race(input, row);
            new_type = (StarsU8)((action_result << 2) | 2);
        }
    } else if (selection == 2) {
        action_result = input->ops->check_password &&
            !input->ops->check_password(input->ops->context, row, old_type) ?
            -1 : (input->ops->edit_race ? input->ops->edit_race(
            input->ops->context, hwnd, row, old_type) : -1);
        if (action_result >= 0) {
            dlg2_commit_packed_race(input, row);
            new_type = (StarsU8)action_result;
        }
    } else if (selection == 3) {
        new_type = 0;
    } else {
        new_type = dlg2_decode_selection(selection, &change);
        if (!change)
            new_type = old_type;
    }
    if (new_type != old_type) {
        dlg2_propagate_type(input, row, new_type);
        if (input->ops->invalidate)
            input->ops->invalidate(input->ops->context, hwnd);
    }
    if (input->ops->set_focus)
        input->ops->set_focus(input->ops->context, hwnd);
    return 0;
}

StarsI16 stars_new_game_dlg2(StarsU16 hwnd, StarsU16 message,
    StarsU16 w_param, StarsI32 l_param, StarsNewGameDlg2Input *input)
{
    StarsI16 row;
    StarsI16 x;
    StarsI16 y;
    StarsI16 button;

    if (!input || !input->ops || !input->player_types ||
            input->player_type_count <= 0)
        return 0;
    if (message == 0x0f) {
        if (input->ops->paint)
            input->ops->paint(input->ops->context, hwnd, w_param);
        return 1;
    }
    if (message == 0x14) {
        if (input->ops->erase_background)
            input->ops->erase_background(input->ops->context, hwnd, w_param);
        return 1;
    }
    if (message == 0x19)
        return dlg2_control_color(hwnd, w_param, (StarsU32)l_param, input);
    if (message == 0x20) {
        if (input->read_only)
            return 0;
        x = (StarsI16)((StarsU32)l_param & 0xffffU);
        y = (StarsI16)((StarsU32)l_param >> 16);
        row = dlg2_row(input, x, y);
        if (row < 0)
            return 0;
        if (input->ops->set_cursor)
            input->ops->set_cursor(input->ops->context, hwnd);
        return 1;
    }
    if (message == 0x110) {
        if (input->ops->set_title_and_layout)
            input->ops->set_title_and_layout(input->ops->context, hwnd);
        if (input->ops->sticky_position)
            input->ops->sticky_position(input->ops->context, hwnd, 1);
        return 1;
    }
    if (message == 0x111) {
        if (w_param == 0x76) {
            if (input->ops->help)
                input->ops->help(input->ops->context, hwnd);
            return 1;
        }
        button = dlg2_command_row(input, w_param);
        if (button < 0)
            return 0;
        if (button != 0 && input->player_types[0] == 0 &&
                !input->read_only) {
            if (input->ops->alert)
                input->ops->alert(input->ops->context, 0x185, 0x10);
            return 0;
        }
        if (input->ops->sticky_position)
            input->ops->sticky_position(input->ops->context, hwnd, 0);
        if (input->ops->end_dialog)
            input->ops->end_dialog(input->ops->context, hwnd, button);
        return 1;
    }
    if (message != 0x201 && message != 0x204)
        return 0;
    if (input->read_only)
        return 0;
    x = (StarsI16)((StarsU32)l_param & 0xffffU);
    y = (StarsI16)((StarsU32)l_param >> 16);
    row = dlg2_row(input, x, y);
    if (row < 0)
        return 0;
    return dlg2_handle_popup(hwnd, message, row, x, y, input);
}
