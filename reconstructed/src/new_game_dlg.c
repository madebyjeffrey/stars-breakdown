#include "core.h"

enum {
    NEW_GAME_DLG_WM_PAINT = 15,
    NEW_GAME_DLG_WM_ERASE_BACKGROUND = 20,
    NEW_GAME_DLG_WM_CONTROL_COLOR = 25,
    NEW_GAME_DLG_WM_INIT_DIALOG = 272,
    NEW_GAME_DLG_WM_COMMAND = 273,
    NEW_GAME_DLG_ID_SIZE_FIRST = 0x3e8,
    NEW_GAME_DLG_ID_SIZE_LAST = 0x3ec,
    NEW_GAME_DLG_ID_DENSITY_FIRST = 0x3ed,
    NEW_GAME_DLG_ID_DENSITY_LAST = 0x3f0,
    NEW_GAME_DLG_ID_DISTANCE_FIRST = 0x3f1,
    NEW_GAME_DLG_ID_DISTANCE_LAST = 0x3f4,
    NEW_GAME_DLG_ID_NAME = 0x406,
    NEW_GAME_DLG_ID_OPTIONS_FIRST = 0x3f8,
    NEW_GAME_DLG_ID_OPTIONS_LAST = 0x3fd,
    NEW_GAME_DLG_ID_VICTORY = 0x41a,
    NEW_GAME_DLG_HELP = 0x76,
    NEW_GAME_DLG_OPTION_MESSAGE = 0x401,
    NEW_GAME_DLG_CONTROL_COLOR_EDIT = 6,
    NEW_GAME_DLG_HELP_ID = 0x3f4
};

static void new_game_put16(StarsU8 *record, StarsU16 offset,
    StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static void new_game_sync_packed(const StarsNewGameDlgInput *input)
{
    StarsU16 flags;
    StarsI16 i;

    if (!input->packed_game_record || !input->state)
        return;
    new_game_put16(input->packed_game_record, 4U,
        (StarsU16)input->state->map_size);
    new_game_put16(input->packed_game_record, 6U,
        (StarsU16)input->state->density);
    new_game_put16(input->packed_game_record, 12U,
        (StarsU16)input->state->start_distance);
    flags = (StarsU16)(input->packed_game_record[16] |
        ((StarsU16)input->packed_game_record[17] << 8));
    flags = (StarsU16)((flags & (StarsU16)~0x01f3U) |
        (input->state->options & 0x01f3U));
    new_game_put16(input->packed_game_record, 16U, flags);
    for (i = 0; i < 32; ++i)
        input->packed_game_record[(StarsU16)(32U + (StarsU16)i)] =
            input->state->game_name[i];
}

void stars_new_game_dlg_resource_contract(
    StarsNewGameDlgResourceContract *contract)
{
    static const StarsNewGameDlgResourceControl controls[27] = {
        {0xffffU, 10, 10, 48, 10}, {1030U, 64, 8, 150, 12},
        {1000U, 24, 40, 44, 12}, {1001U, 24, 52, 44, 12},
        {1002U, 24, 64, 44, 12}, {1003U, 24, 76, 44, 12},
        {1004U, 24, 88, 44, 12}, {1005U, 24, 124, 44, 12},
        {1006U, 24, 136, 44, 12}, {1007U, 24, 148, 44, 12},
        {1008U, 24, 160, 44, 12}, {1009U, 96, 40, 48, 12},
        {1010U, 96, 52, 48, 12}, {1011U, 96, 64, 48, 12},
        {1012U, 96, 76, 48, 12}, {1016U, 90, 94, 140, 12},
        {1017U, 90, 107, 140, 12}, {1018U, 90, 120, 140, 12},
        {1019U, 90, 133, 140, 12}, {1020U, 90, 146, 140, 12},
        {1021U, 90, 159, 140, 12}, {1050U, 90, 172, 140, 12},
        {118U, 10, 190, 40, 14}, {2U, 60, 190, 40, 14},
        {1070U, 110, 190, 40, 14}, {1071U, 160, 190, 40, 14},
        {1072U, 210, 190, 40, 14}
    };
    StarsI16 i;

    if (!contract)
        return;
    contract->resource_id = 0x0186U;
    contract->width = 261;
    contract->height = 210;
    contract->font_size = 8;
    contract->control_count = 27;
    for (i = 0; i < 27; ++i)
        contract->controls[i] = controls[i];
}

static void new_game_draw_frame(StarsNewGameDlgInput *input, StarsU16 hwnd,
    StarsU16 dc, StarsU16 outer_id, StarsU16 inner_id, StarsU16 string_id)
{
    StarsRect frame;
    StarsRect inner;
    StarsU8 text[48];
    StarsI16 length;

    frame.left = frame.top = frame.right = frame.bottom = 0;
    inner = frame;
    if (input->ops->get_control_rect)
        input->ops->get_control_rect(input->context, hwnd, &frame);
    if (input->ops->map_points)
        input->ops->map_points(input->context, 0, hwnd, &frame, 1);
    if (input->ops->get_control_rect)
        input->ops->get_control_rect(input->context, hwnd, &inner);
    (void)outer_id;
    (void)inner_id;
    if (input->ops->map_points)
        input->ops->map_points(input->context, 0, hwnd, &inner, 1);
    frame.right = inner.right;
    frame.bottom = inner.bottom;
    if (input->ops->expand_rect)
        input->ops->expand_rect(input->context, &frame,
            input->state->arial8_height, input->state->arial8_height >> 1);
    if (input->ops->draw_frame)
        input->ops->draw_frame(input->context, dc, &frame, -1);
    if (input->ops->select_object)
        input->ops->select_object(input->context, dc, 0);
    if (input->ops->set_background_color)
        input->ops->set_background_color(input->context, dc,
            input->state->button_face_color);
    length = input->ops->get_string ? input->ops->get_string(input->context,
        string_id, text, (StarsU16)sizeof(text)) : 0;
    if (input->ops->text_out)
        input->ops->text_out(input->context, dc,
            (StarsI16)(frame.left + 8),
            (StarsI16)(frame.top - (input->state->arial8_height >> 1)),
            text, length);
}

static void new_game_set_option(StarsNewGameDlgInput *input, StarsU16 hwnd,
    StarsU16 control, StarsU16 mask, StarsU16 shift)
{
    StarsU16 item;
    StarsU16 value;

    item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
        input->context, hwnd, control) : control;
    value = (StarsU16)((input->state->options >> shift) & 1U);
    if (input->ops->send_message)
        input->ops->send_message(input->context, item,
            NEW_GAME_DLG_OPTION_MESSAGE, value, 0);
    (void)mask;
}

static void new_game_disable_range(StarsNewGameDlgInput *input, StarsU16 hwnd,
    StarsU16 first, StarsU16 last)
{
    StarsU16 id;
    StarsU16 item;

    if (!input->ops->enable_window)
        return;
    for (id = first; id <= last; ++id) {
        item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
            input->context, hwnd, id) : id;
        input->ops->enable_window(input->context, item, 0);
    }
}

static StarsI16 new_game_checked_offset(StarsNewGameDlgInput *input,
    StarsU16 hwnd, StarsU16 first, StarsU16 last)
{
    StarsU16 id;

    for (id = first; id <= last; ++id)
        if (input->ops->is_button_checked &&
                input->ops->is_button_checked(input->context, hwnd, id))
            return (StarsI16)(id - first);
    /* The original scans through an exclusive upper sentinel and returns
     * that offset when no radio is checked. */
    return (StarsI16)(last - first + 1);
}

static void new_game_pack_option(StarsNewGameDlgInput *input,
    StarsNewGameDlgState *state, StarsU16 hwnd, StarsU16 control,
    StarsU16 mask, StarsU16 shift)
{
    StarsI16 checked;

    checked = input->ops->is_button_checked ?
        input->ops->is_button_checked(input->context, hwnd, control) : 0;
    state->options = (StarsU16)(state->options & (StarsU16)~mask);
    state->options = (StarsU16)(state->options |
        (StarsU16)((checked & 1) << shift));
}

static StarsI16 new_game_race_index(StarsNewGameDlgInput *input,
    StarsU16 wparam)
{
    StarsU16 index;

    if (input->ops->race_button_index)
        return input->ops->race_button_index(input->context, wparam);
    for (index = 0; index < 4; ++index)
        if (input->state->race_button_ids[index] == wparam)
            return (StarsI16)index;
    return -1;
}

static void new_game_paint(StarsNewGameDlgInput *input, StarsU16 hwnd)
{
    StarsRect paint;
    StarsU16 dc;

    paint.left = paint.top = paint.right = paint.bottom = 0;
    dc = input->ops->begin_paint ? input->ops->begin_paint(
        input->context, hwnd, &paint) : 0;
    new_game_draw_frame(input, hwnd, dc, NEW_GAME_DLG_ID_SIZE_FIRST,
        NEW_GAME_DLG_ID_SIZE_LAST, 0x1c5);
    new_game_draw_frame(input, hwnd, dc, NEW_GAME_DLG_ID_DENSITY_FIRST,
        NEW_GAME_DLG_ID_DENSITY_LAST, 0x1c7);
    new_game_draw_frame(input, hwnd, dc, NEW_GAME_DLG_ID_DISTANCE_FIRST,
        NEW_GAME_DLG_ID_DISTANCE_LAST, 0x1c8);
    if (input->ops->end_paint)
        input->ops->end_paint(input->context, hwnd, &paint);
}

static void new_game_initialize(StarsNewGameDlgInput *input, StarsU16 hwnd)
{
    StarsU16 item;

    if (input->ops->set_title)
        input->ops->set_title(input->context, hwnd, 1);
    if (input->ops->check_radio_button) {
        input->ops->check_radio_button(input->context, hwnd,
            NEW_GAME_DLG_ID_SIZE_FIRST, NEW_GAME_DLG_ID_SIZE_LAST,
            (StarsU16)(NEW_GAME_DLG_ID_SIZE_FIRST + input->state->map_size));
        input->ops->check_radio_button(input->context, hwnd,
            NEW_GAME_DLG_ID_DENSITY_FIRST, NEW_GAME_DLG_ID_DENSITY_LAST,
            (StarsU16)(NEW_GAME_DLG_ID_DENSITY_FIRST + input->state->density));
        input->ops->check_radio_button(input->context, hwnd,
            NEW_GAME_DLG_ID_DISTANCE_FIRST, NEW_GAME_DLG_ID_DISTANCE_LAST,
            (StarsU16)(NEW_GAME_DLG_ID_DISTANCE_FIRST +
                input->state->start_distance));
    }
    item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
        input->context, hwnd, NEW_GAME_DLG_ID_NAME) : NEW_GAME_DLG_ID_NAME;
    if (input->ops->set_window_text)
        input->ops->set_window_text(input->context, item,
            input->state->game_name);
    if (input->ops->send_message)
        input->ops->send_message(input->context, item, 0x415, 0x1f, 0);
    new_game_set_option(input, hwnd, 0x3f8, 0x0001, 0);
    new_game_set_option(input, hwnd, 0x3f9, 0x0002, 1);
    new_game_set_option(input, hwnd, 0x3fa, 0x0020, 5);
    new_game_set_option(input, hwnd, 0x3fb, 0x0080, 7);
    new_game_set_option(input, hwnd, 0x3fc, 0x0010, 4);
    new_game_set_option(input, hwnd, 0x3fd, 0x0040, 6);
    new_game_set_option(input, hwnd, 0x41a, 0x0100, 8);
    if (input->state->read_only) {
        new_game_disable_range(input, hwnd, 1000, 1004);
        new_game_disable_range(input, hwnd, 1005, 1008);
        new_game_disable_range(input, hwnd, 1009, 1012);
        new_game_disable_range(input, hwnd, 0x3f8, 0x3fd);
        new_game_disable_range(input, hwnd, 0x41a, 0x41a);
        new_game_disable_range(input, hwnd, 0x406, 0x406);
    }
    if (input->ops->sticky_position)
        input->ops->sticky_position(input->context, hwnd,
            &input->state->sticky_position, 1);
}

StarsI16 stars_new_game_dlg(StarsNewGameDlgInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect rect;
    StarsU16 item;
    StarsI16 index;
    StarsI16 option;

    (void)lparam;
    if (!input || !input->state || !input->ops)
        return 0;
    if (message == NEW_GAME_DLG_WM_PAINT) {
        new_game_paint(input, hwnd);
        return 1;
    }
    if (message == NEW_GAME_DLG_WM_ERASE_BACKGROUND) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_control_rect)
            input->ops->get_control_rect(input->context, hwnd, &rect);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->context, wparam, &rect,
                input->state->button_face_brush);
        return 1;
    }
    if (message == NEW_GAME_DLG_WM_CONTROL_COLOR) {
        item = (StarsU16)lparam;
        if (input->ops->get_dialog_item) {
            for (option = 1000; option < 0x3fe; ++option)
                if (input->ops->get_dialog_item(input->context, hwnd,
                        (StarsU16)option) == item)
                    break;
            if (option >= 0x3fe && input->ops->get_dialog_item(
                    input->context, hwnd, 0x41a) != item &&
                    (StarsU16)(lparam >> 16) !=
                    NEW_GAME_DLG_CONTROL_COLOR_EDIT)
                return 0;
        }
        if (input->ops->set_background_color)
            input->ops->set_background_color(input->context, wparam,
                input->state->button_face_color);
        return (StarsI16)input->state->button_face_brush;
    }
    if (message == NEW_GAME_DLG_WM_INIT_DIALOG) {
        new_game_initialize(input, hwnd);
        return 1;
    }
    if (message != NEW_GAME_DLG_WM_COMMAND)
        return 0;
    if (wparam == NEW_GAME_DLG_HELP) {
        if (input->ops->win_help)
            input->ops->win_help(input->context, hwnd, (const StarsU8 *)0,
                input->state->help_id ? input->state->help_id :
                NEW_GAME_DLG_HELP_ID);
        return 1;
    }
    index = new_game_race_index(input, wparam);
    if (index < 0)
        return 0;
    if (index != 0) {
        input->state->map_size = new_game_checked_offset(input, hwnd,
            NEW_GAME_DLG_ID_SIZE_FIRST, NEW_GAME_DLG_ID_SIZE_LAST);
        input->state->density = new_game_checked_offset(input, hwnd,
            NEW_GAME_DLG_ID_DENSITY_FIRST, NEW_GAME_DLG_ID_DENSITY_LAST);
        input->state->start_distance = new_game_checked_offset(input, hwnd,
            NEW_GAME_DLG_ID_DISTANCE_FIRST, NEW_GAME_DLG_ID_DISTANCE_LAST);
        new_game_pack_option(input, input->state, hwnd, 0x3f8, 0x0001, 0);
        new_game_pack_option(input, input->state, hwnd, 0x3f9, 0x0002, 1);
        new_game_pack_option(input, input->state, hwnd, 0x3fa, 0x0020, 5);
        new_game_pack_option(input, input->state, hwnd, 0x3fb, 0x0080, 7);
        new_game_pack_option(input, input->state, hwnd, 0x3fc, 0x0010, 4);
        new_game_pack_option(input, input->state, hwnd, 0x3fd, 0x0040, 6);
        new_game_pack_option(input, input->state, hwnd, 0x41a, 0x0100, 8);
        item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
            input->context, hwnd, NEW_GAME_DLG_ID_NAME) :
            NEW_GAME_DLG_ID_NAME;
        if (input->ops->get_dialog_text)
            input->ops->get_dialog_text(input->context, item,
                NEW_GAME_DLG_ID_NAME, input->state->game_name,
                (StarsU16)sizeof(input->state->game_name));
        if (input->state->game_name[0] == 0U && input->ops->default_name)
            input->ops->default_name(input->context, input->state->game_name,
                (StarsU16)sizeof(input->state->game_name));
    }
        if (input->ops->sticky_position)
            input->ops->sticky_position(input->context, hwnd,
                &input->state->sticky_position, 0);
        new_game_sync_packed(input);
        if (input->ops->end_dialog)
        input->ops->end_dialog(input->context, hwnd, index);
    return 1;
}
