#include "core.h"

enum {
    SIMPLE_NEW_GAME_WM_PAINT = 15,
    SIMPLE_NEW_GAME_WM_ERASE_BACKGROUND = 20,
    SIMPLE_NEW_GAME_WM_CONTROL_COLOR = 25,
    SIMPLE_NEW_GAME_WM_INIT_DIALOG = 272,
    SIMPLE_NEW_GAME_WM_COMMAND = 273,
    SIMPLE_NEW_GAME_DENSITY_FIRST = 200,
    SIMPLE_NEW_GAME_DENSITY_LAST = 0xcb,
    SIMPLE_NEW_GAME_SIZE_FIRST = 1000,
    SIMPLE_NEW_GAME_SIZE_LAST = 0x3ec,
    SIMPLE_NEW_GAME_LIST = 0x81a,
    SIMPLE_NEW_GAME_CREATE = 0x430,
    SIMPLE_NEW_GAME_CUSTOM_RACE = 0xd2,
    SIMPLE_NEW_GAME_CANCEL = 0xd4,
    SIMPLE_NEW_GAME_HELP = 0x76,
    SIMPLE_NEW_GAME_LIST_RESET = 0x40b,
    SIMPLE_NEW_GAME_LIST_ADD = 0x403,
    SIMPLE_NEW_GAME_LIST_DELETE = 0x404,
    SIMPLE_NEW_GAME_LIST_LIMIT = 0x40e,
    SIMPLE_NEW_GAME_LIST_GET_SELECTION = 0x407,
    SIMPLE_NEW_GAME_CONTROL_COLOR_EDIT = 6,
    SIMPLE_NEW_GAME_DEFAULT_DENSITY = 1,
    SIMPLE_NEW_GAME_DEFAULT_SIZE = 1,
    SIMPLE_NEW_GAME_PACKED_PLAYER_SIZE = 192
};

void stars_simple_new_game_resource_contract(
    StarsSimpleNewGameResourceContract *contract)
{
    static const StarsSimpleNewGameResourceControl controls[16] = {
        {200U, 16, 18, 60, 12, 4, 402U},
        {201U, 16, 32, 60, 12, 8, 817U},
        {202U, 16, 46, 60, 12, 6, 598U},
        {203U, 16, 60, 60, 12, 6, 632U},
        {1000U, 16, 90, 60, 12, 4, 420U},
        {1001U, 16, 104, 60, 12, 5, 505U},
        {1002U, 16, 118, 60, 12, 6, 609U},
        {1003U, 16, 132, 60, 12, 5, 491U},
        {1004U, 16, 146, 60, 12, 4, 393U},
        {2074U, 110, 18, 112, 76, 0, 0U},
        {210U, 110, 32, 112, 12, 18, 1550U},
        {211U, 110, 404, 112, 12, 17, 1376U},
        {212U, 8, 170, 76, 12, 15, 1407U},
        {1072U, 102, 170, 36, 12, 2, 186U},
        {2U, 148, 170, 36, 12, 6, 582U},
        {118U, 194, 170, 36, 12, 5, 431U}
    };
    StarsI16 i;

    if (!contract)
        return;
    contract->resource_id = 0x00d1U;
    contract->width = 240;
    contract->height = 190;
    contract->font_size = 8;
    contract->control_count = 16;
    for (i = 0; i < 16; ++i)
        contract->controls[i] = controls[i];
}

static void simple_new_game_copy_packed_player(
    StarsSimpleNewGameInput *input, StarsI16 selection)
{
    StarsU16 stride;
    StarsU16 offset;
    StarsU16 i;

    if (!input->packed_player_record || !input->packed_player_records ||
            selection < 0 || selection >= 16)
        return;
    stride = input->packed_player_stride;
    if (stride == 0U)
        stride = SIMPLE_NEW_GAME_PACKED_PLAYER_SIZE;
    offset = (StarsU16)(selection * stride);
    for (i = 0; i < SIMPLE_NEW_GAME_PACKED_PLAYER_SIZE; ++i)
        input->packed_player_records[(StarsU16)(offset + i)] =
            input->packed_player_record[i];
}

static StarsI16 simple_new_game_checked(const StarsSimpleNewGameInput *input,
    StarsU16 hwnd, StarsU16 first, StarsU16 last)
{
    StarsU16 id;

    if (!input->ops->is_button_checked)
        return (StarsI16)(last - first);
    for (id = first; id <= last; ++id)
        if (input->ops->is_button_checked(input->ops->context, hwnd, id))
            return (StarsI16)(id - first);
    /* The raw loop tests an exclusive upper sentinel when no radio is set. */
    return (StarsI16)(last - first + 1);
}

static void simple_new_game_paint_frame(const StarsSimpleNewGameInput *input,
    StarsU16 hwnd, StarsU16 dc, StarsI16 id, StarsU16 title_id)
{
    StarsRect rect;
    StarsU8 text[64];
    StarsI16 length;

    rect.left = rect.top = rect.right = rect.bottom = 0;
    if (input->ops->get_control_rect)
        input->ops->get_control_rect(input->ops->context, hwnd, &rect);
    if (input->ops->map_points)
        input->ops->map_points(input->ops->context, 0, hwnd, &rect, 1);
    if (input->ops->expand_rect)
        input->ops->expand_rect(input->ops->context, &rect,
            input->state->arial8_height, input->state->arial8_height >> 1);
    if (input->ops->draw_frame)
        input->ops->draw_frame(input->ops->context, dc, &rect, -1);
    if (input->ops->select_object)
        input->ops->select_object(input->ops->context, dc, 1);
    if (input->ops->set_background_color)
        input->ops->set_background_color(input->ops->context, dc,
            input->state->button_face_color);
    length = input->ops->get_string ? input->ops->get_string(
        input->ops->context, title_id, text, (StarsU16)sizeof text) : 0;
    if (input->ops->text_out)
        input->ops->text_out(input->ops->context, dc,
            (StarsI16)(rect.left + 8),
            (StarsI16)(rect.top - (input->state->arial8_height >> 1)),
            text, length);
    (void)id;
}

static void simple_new_game_paint(const StarsSimpleNewGameInput *input,
    StarsU16 hwnd)
{
    StarsRect paint;
    StarsU16 dc;

    paint.left = paint.top = paint.right = paint.bottom = 0;
    dc = input->ops->begin_paint ? input->ops->begin_paint(
        input->ops->context, hwnd, &paint) : 0;
    simple_new_game_paint_frame(input, hwnd, dc, 200, 0x1c4);
    simple_new_game_paint_frame(input, hwnd, dc, 1000, 0x1c5);
    simple_new_game_paint_frame(input, hwnd, dc, SIMPLE_NEW_GAME_LIST, 0x1c6);
    if (input->ops->end_paint)
        input->ops->end_paint(input->ops->context, hwnd, &paint);
}

static void simple_new_game_initialize(StarsSimpleNewGameInput *input,
    StarsU16 hwnd)
{
    StarsU16 item;
    StarsU16 id;
    StarsU8 text[64];
    StarsI16 length;

    if (input->ops->check_radio_button) {
        input->ops->check_radio_button(input->ops->context, hwnd,
            SIMPLE_NEW_GAME_DENSITY_FIRST, SIMPLE_NEW_GAME_DENSITY_LAST,
            (StarsU16)(SIMPLE_NEW_GAME_DENSITY_FIRST +
                SIMPLE_NEW_GAME_DEFAULT_DENSITY));
        input->ops->check_radio_button(input->ops->context, hwnd,
            SIMPLE_NEW_GAME_SIZE_FIRST, SIMPLE_NEW_GAME_SIZE_LAST,
            (StarsU16)(SIMPLE_NEW_GAME_SIZE_FIRST +
                SIMPLE_NEW_GAME_DEFAULT_SIZE));
    }
    item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
        input->ops->context, hwnd, SIMPLE_NEW_GAME_LIST) : SIMPLE_NEW_GAME_LIST;
    if (input->ops->send_message)
        input->ops->send_message(input->ops->context, item,
            SIMPLE_NEW_GAME_LIST_RESET, 0, 0U);
    for (id = 0; id < 7; ++id) {
        length = input->ops->get_string ? input->ops->get_string(
            input->ops->context, (StarsU16)(0x567U + id), text,
            (StarsU16)sizeof text) : 0;
        text[length] = 0;
        if (input->add_list_string)
            input->add_list_string(input->context, item, (StarsU16)id, text);
    }
    if (input->ops->send_message)
        input->ops->send_message(input->ops->context, item,
            SIMPLE_NEW_GAME_LIST_LIMIT, 0, 0U);
    if (input->ops->sticky_position)
        input->ops->sticky_position(input->ops->context, hwnd,
            &input->state->sticky_position, 1);
}

static StarsI16 simple_new_game_control_color(
    const StarsSimpleNewGameInput *input, StarsU16 hwnd, StarsU16 brush,
    StarsU32 lparam)
{
    StarsU16 item;
    StarsU16 id;

    item = (StarsU16)lparam;
    if (input->ops->get_dialog_item) {
        for (id = SIMPLE_NEW_GAME_DENSITY_FIRST;
             id <= SIMPLE_NEW_GAME_DENSITY_LAST; ++id)
            if (input->ops->get_dialog_item(input->ops->context, hwnd, id) == item)
                break;
        if (id > SIMPLE_NEW_GAME_DENSITY_LAST) {
            for (id = SIMPLE_NEW_GAME_SIZE_FIRST;
                 id <= SIMPLE_NEW_GAME_SIZE_LAST; ++id)
                if (input->ops->get_dialog_item(input->ops->context, hwnd, id) == item)
                    break;
        }
        if (id > SIMPLE_NEW_GAME_SIZE_LAST &&
            (StarsU16)(lparam >> 16) != SIMPLE_NEW_GAME_CONTROL_COLOR_EDIT)
            return 0;
    }
    if (input->ops->set_background_color)
        input->ops->set_background_color(input->ops->context, brush,
            input->state->button_face_color);
    return (StarsI16)input->state->button_face_brush;
}

StarsI16 stars_simple_new_game_dlg(StarsSimpleNewGameInput *input,
    StarsU16 hwnd, StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsU16 item;
    StarsI16 selection;
    StarsI16 result;
    StarsI16 list_count;

    if (!input || !input->state || !input->ops)
        return 0;
    if (message == SIMPLE_NEW_GAME_WM_PAINT) {
        simple_new_game_paint(input, hwnd);
        return 1;
    }
    if (message == SIMPLE_NEW_GAME_WM_ERASE_BACKGROUND) {
        StarsRect rect;
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_control_rect)
            input->ops->get_control_rect(input->ops->context, hwnd, &rect);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->ops->context, wparam, &rect,
                input->state->button_face_brush);
        return 1;
    }
    if (message == SIMPLE_NEW_GAME_WM_CONTROL_COLOR)
        return simple_new_game_control_color(input, hwnd, wparam, lparam);
    if (message == SIMPLE_NEW_GAME_WM_INIT_DIALOG) {
        simple_new_game_initialize(input, hwnd);
        return 1;
    }
    if (message != SIMPLE_NEW_GAME_WM_COMMAND)
        return 0;
    if (wparam == SIMPLE_NEW_GAME_HELP) {
        if (input->ops->win_help)
            input->ops->win_help(input->ops->context, hwnd,
                (const StarsU8 *)0, 0x3ea);
        return 1;
    }
    if (wparam == SIMPLE_NEW_GAME_CANCEL) {
        if (input->ops->sticky_position)
            input->ops->sticky_position(input->ops->context, hwnd,
                &input->state->sticky_position, 0);
        if (input->ops->end_dialog)
            input->ops->end_dialog(input->ops->context, hwnd,
                SIMPLE_NEW_GAME_CANCEL);
        return 1;
    }
    item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
        input->ops->context, hwnd, SIMPLE_NEW_GAME_LIST) : SIMPLE_NEW_GAME_LIST;
    if (wparam == SIMPLE_NEW_GAME_CREATE || wparam == 2 || wparam == 0xd3) {
        input->state->density = simple_new_game_checked(input, hwnd,
            SIMPLE_NEW_GAME_DENSITY_FIRST, SIMPLE_NEW_GAME_DENSITY_LAST);
        input->state->map_size = simple_new_game_checked(input, hwnd,
            SIMPLE_NEW_GAME_SIZE_FIRST, SIMPLE_NEW_GAME_SIZE_LAST);
        selection = input->get_selection ? input->get_selection(input->context,
            item) : input->state->turn;
        input->state->turn = selection;
        if (input->ops->sticky_position)
            input->ops->sticky_position(input->ops->context, hwnd,
                &input->state->sticky_position, 0);
        if (input->ops->end_dialog)
            input->ops->end_dialog(input->ops->context, hwnd, wparam);
        return 1;
    }
    if (wparam == SIMPLE_NEW_GAME_CUSTOM_RACE) {
        selection = input->get_selection ? input->get_selection(input->context,
            item) : input->state->turn;
        input->state->turn = selection;
        if (selection < 7) {
            if (input->load_old_race)
                input->load_old_race(input->context, selection);
        } else if (input->load_new_race)
            input->load_new_race(input->context, selection);
        result = input->race_wizard ? input->race_wizard(input->context, hwnd) : 0;
        if (result) {
            if (input->save_new_race)
                input->save_new_race(input->context, selection);
            simple_new_game_copy_packed_player(input, selection);
            list_count = input->list_count ? input->list_count(
                input->context, item) : 0;
            if (list_count > 7 && input->ops->send_message)
                input->ops->send_message(input->ops->context, item,
                    SIMPLE_NEW_GAME_LIST_DELETE, 7, 0U);
            if (input->ops->send_message)
                input->ops->send_message(input->ops->context, item,
                    SIMPLE_NEW_GAME_LIST_ADD, 0, 0U);
            if (input->ops->send_message)
                input->ops->send_message(input->ops->context, item,
                    SIMPLE_NEW_GAME_LIST_LIMIT, 7, 0U);
        }
        if (input->set_focus)
            input->set_focus(input->context, hwnd);
        return 0;
    }
    return 0;
}
