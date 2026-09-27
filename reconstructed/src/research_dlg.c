#include "core.h"

enum {
    RESEARCH_WM_PAINT = 15,
    RESEARCH_WM_ERASE_BACKGROUND = 20,
    RESEARCH_WM_CONTROL_COLOR = 25,
    RESEARCH_WM_SET_CURSOR = 32,
    RESEARCH_WM_INIT_DIALOG = 272,
    RESEARCH_WM_COMMAND = 273,
    RESEARCH_WM_LEFT_BUTTON_DOWN = 513,
    RESEARCH_WM_LEFT_BUTTON_DOUBLE_CLICK = 515,
    RESEARCH_FIRST_RADIO = 0x431,
    RESEARCH_LAST_RADIO = 0x436,
    RESEARCH_COMBO = 0x43b,
    RESEARCH_OK = 2,
    RESEARCH_HELP = 0x76,
    RESEARCH_HELP_CONTEXT = 0x42e,
    RESEARCH_FONT_MESSAGE = 0x30,
    RESEARCH_COMBO_ADD_STRING = 0x403,
    RESEARCH_COMBO_GET_SELECTION = 0x407,
    RESEARCH_COMBO_SET_SELECTION = 0x40e,
    RESEARCH_TUTOR_BIT = 0x0800
};

static StarsI16 research_string(StarsResearchDlgInput *input, StarsU16 id,
    StarsU8 *buffer, StarsU16 capacity)
{
    if (!input->ops->get_string)
        return 0;
    return input->ops->get_string(input->context, id, buffer, capacity);
}

static StarsI16 research_extent(StarsResearchDlgInput *input, StarsU16 dc,
    StarsU16 id, StarsU8 *buffer, StarsU16 capacity)
{
    StarsI16 length;
    StarsI32 extent;

    length = research_string(input, id, buffer, capacity);
    if (!input->ops->get_text_extent)
        return 0;
    extent = input->ops->get_text_extent(input->context, dc, buffer, length);
    return (StarsI16)extent;
}

static StarsI16 research_max_label_width(StarsResearchDlgInput *input,
    StarsU16 dc, StarsU16 first, StarsU16 count, StarsU8 *buffer,
    StarsU16 capacity)
{
    StarsI16 width;
    StarsI16 candidate;
    StarsU16 index;

    width = 0;
    for (index = 0; index < count; ++index) {
        candidate = research_extent(input, dc, (StarsU16)(first + index),
            buffer, capacity);
        if (candidate > width)
            width = candidate;
    }
    return width;
}

static StarsU16 research_item(StarsResearchDlgInput *input, StarsU16 hwnd,
    StarsU16 id)
{
    if (!input->ops->get_dialog_item)
        return 0;
    return input->ops->get_dialog_item(input->context, hwnd, id);
}

static StarsU16 research_future_to_combo(StarsI16 future)
{
    if (future == 6)
        return 0;
    if (future < 6)
        return (StarsU16)(future + 1);
    return (StarsU16)future;
}

static StarsI16 research_combo_to_future(StarsI32 selection)
{
    if (selection == 0)
        return 6;
    if (selection < 7)
        return (StarsI16)(selection - 1);
    return (StarsI16)selection;
}

static void research_initialize(StarsResearchDlgInput *input, StarsU16 hwnd)
{
    StarsResearchDlgState *state;
    const StarsResearchDlgOps *ops;
    StarsRect window_rect;
    StarsRect client_rect;
    StarsU8 text[96];
    StarsI16 title_width;
    StarsI16 width;
    StarsI16 height;
    StarsI16 y;
    StarsI16 radio_height;
    StarsI16 label_width;
    StarsI16 index;
    StarsU16 dc;
    StarsU16 old_font;
    StarsU16 item;
    StarsU32 total;
    StarsI16 owner;
    const StarsU8 *compressed;

    state = input->state;
    ops = input->ops;
    state->original_pct_research = state->pct_research;
    state->original_current_technology = state->current_technology;
    state->original_future_technology = state->future_technology;
    if (ops->check_radio_button)
        ops->check_radio_button(input->context, hwnd, RESEARCH_FIRST_RADIO,
            RESEARCH_LAST_RADIO,
            (StarsU16)(RESEARCH_FIRST_RADIO + state->current_technology));

    dc = ops->get_dc ? ops->get_dc(input->context, hwnd) : 0;
    old_font = 0;
    if (ops->select_object)
        old_font = ops->select_object(input->context, dc, state->font_arial8);
    title_width = research_extent(input, dc, 0x01bc, text,
        (StarsU16)sizeof text);
    state->dx_res_radio = research_max_label_width(input, dc, 0x0054, 6,
        text, (StarsU16)sizeof text);
    radio_height = (StarsI16)((state->arial8_height * 3) / 2);

    for (index = 0; index < 6; ++index) {
        StarsI16 length;

        length = research_string(input, (StarsU16)(0x0054 + index), text,
            (StarsU16)sizeof text);
        item = research_item(input, hwnd,
            (StarsU16)(RESEARCH_FIRST_RADIO + index));
        if (ops->set_window_text)
            ops->set_window_text(input->context, item, text);
        if (ops->send_message)
            (void)ops->send_message(input->context, item,
                RESEARCH_FONT_MESSAGE, state->font_arial8, 0);
        (void)length;
    }
    state->dx_res_radio = (StarsI16)(state->dx_res_radio + 0x40);
    y = (StarsI16)(state->arial8_height * 4 -
        state->arial8_height / 2 + 1);
    for (index = 0; index < 6; ++index) {
        item = research_item(input, hwnd,
            (StarsU16)(RESEARCH_FIRST_RADIO + index));
        if (ops->set_window_pos)
            ops->set_window_pos(input->context, item, 0, 0x10, y,
                state->dx_res_radio, radio_height, 0x44);
        y = (StarsI16)(y + radio_height);
    }

    item = research_item(input, hwnd, RESEARCH_COMBO);
    for (index = 0; index < 8; ++index) {
        compressed = ops->get_compressed_string ?
            ops->get_compressed_string(input->context,
                (StarsU16)(0x53 + index)) : (const StarsU8 *)0;
        if (ops->combo_add_string && compressed)
            ops->combo_add_string(input->context, item, compressed);
        else if (ops->send_message && compressed)
            (void)ops->send_message(input->context, item,
                RESEARCH_COMBO_ADD_STRING, 0,
                (StarsU32)(unsigned long)compressed);
    }
    if (ops->send_message)
        (void)ops->send_message(input->context, item,
            RESEARCH_COMBO_SET_SELECTION,
            research_future_to_combo(state->future_technology), 0);

    state->dx_res_left = (StarsI16)(state->dx_res_radio + title_width + 0x28);
    label_width = research_max_label_width(input, dc, 0x004c, 6, text,
        (StarsU16)sizeof text);
    state->dx_res_string_right = label_width;
    compressed = ops->get_compressed_string ?
        ops->get_compressed_string(input->context, 0x038f) :
        (const StarsU8 *)0;
    if (compressed && ops->get_text_extent)
        label_width = (StarsI16)(label_width + ops->get_text_extent(
            input->context, dc, compressed, 7) + 0x30);
    state->dx_res_right = label_width;
    y = (StarsI16)(y + state->arial8_height * 10);
    item = research_item(input, hwnd, RESEARCH_OK);
    if (ops->set_window_pos)
        ops->set_window_pos(input->context, item, 0,
            (StarsI16)(state->dx_res_left + state->dx_res_right - 0x98),
            (StarsI16)(y - radio_height - 8), 0x46, radio_height, 0x44);
    item = research_item(input, hwnd, RESEARCH_HELP);
    if (ops->set_window_pos)
        ops->set_window_pos(input->context, item, 0,
            (StarsI16)(state->dx_res_left + state->dx_res_right - 0x4c),
            (StarsI16)(y - radio_height - 8), 0x46, radio_height, 0x44);

    window_rect.left = window_rect.top = window_rect.right =
        window_rect.bottom = 0;
    client_rect.left = client_rect.top = client_rect.right =
        client_rect.bottom = 0;
    if (ops->get_window_rect)
        ops->get_window_rect(input->context, hwnd, &window_rect);
    if (ops->get_client_rect)
        ops->get_client_rect(input->context, hwnd, &client_rect);
    width = (StarsI16)(state->dx_res_left + state->dx_res_right +
        window_rect.right - window_rect.left - client_rect.right);
    height = (StarsI16)(y + window_rect.bottom - window_rect.top -
        client_rect.bottom);
    if (ops->set_window_pos)
        ops->set_window_pos(input->context, hwnd, 0, 0, 0, width, height, 6);
    if (ops->sticky_position)
        ops->sticky_position(input->context, hwnd, &state->sticky_position, 1);
    if (ops->select_object)
        (void)ops->select_object(input->context, dc, old_font);
    if (ops->release_dc)
        ops->release_dc(input->context, hwnd, dc);

    total = 0;
    for (index = 0; index < state->planet_count; ++index) {
        owner = ops->planet_owner ? ops->planet_owner(input->context,
            index) : state->player_id;
        if (owner == state->player_id && ops->resources_at_planet)
            total += ops->resources_at_planet(input->context, index,
                state->player_id);
    }
    state->resource_total = (StarsI32)total;
    state->research_budget = ops->projected_budget ?
        ops->projected_budget(input->context, state->pct_research) : 0;
    if (state->tutor_active && ops->advance_tutor)
        ops->advance_tutor(input->context);
}

static StarsI16 research_control_color(StarsResearchDlgInput *input,
    StarsU16 hwnd, StarsU16 dc, StarsU32 control)
{
    StarsU16 id;

    for (id = RESEARCH_FIRST_RADIO; id <= RESEARCH_LAST_RADIO; ++id) {
        if (control == research_item(input, hwnd, id))
            break;
    }
    if (id <= RESEARCH_LAST_RADIO || (StarsU16)(control >> 16) == 6U) {
        if (input->ops->set_background_color)
            input->ops->set_background_color(input->context, dc,
                input->state->button_face_color);
        return (StarsI16)input->state->button_face_brush;
    }
    return 0;
}

static StarsI16 research_cursor(StarsResearchDlgInput *input, StarsU16 hwnd)
{
    StarsPoint point;
    StarsResearchDlgState *state;

    state = input->state;
    point.x = point.y = 0;
    if (input->ops->get_cursor_pos)
        input->ops->get_cursor_pos(input->context, &point);
    if (input->ops->screen_to_client)
        input->ops->screen_to_client(input->context, hwnd, &point);
    if (point.x > 12 && point.x < state->dx_res_left - 12 &&
            point.y >= state->y_future_technology &&
            point.y < state->y_future_technology +
                state->future_technology_count * state->arial8_height) {
        if (input->ops->set_cursor)
            input->ops->set_cursor(input->context, state->help_cursor);
        return 1;
    }
    if (state->y_technology_note != -1 && point.y >=
            state->y_technology_note && point.y <
            state->y_technology_note + state->arial8_height * 2 &&
            point.x > state->dx_res_left) {
        if (input->ops->set_cursor)
            input->ops->set_cursor(input->context, state->help_cursor);
        return 1;
    }
    return 0;
}

static StarsI16 research_commit(StarsResearchDlgInput *input, StarsU16 hwnd)
{
    StarsResearchDlgState *state;
    StarsI32 selection;
    StarsI16 future;
    StarsU8 packed[2];
    StarsU16 combo;
    StarsI16 changed;

    state = input->state;
    combo = research_item(input, hwnd, RESEARCH_COMBO);
    selection = input->ops->send_message ? input->ops->send_message(
        input->context, combo, RESEARCH_COMBO_GET_SELECTION, 0, 0) : 0;
    future = research_combo_to_future(selection);
    changed = state->current_technology != state->original_current_technology ||
        future != state->original_future_technology ||
        state->pct_research != state->original_pct_research;
    if (changed) {
        state->future_technology = future;
        if (input->ops->commit)
            input->ops->commit(input->context, state->pct_research,
                state->current_technology, future);
        packed[0] = (StarsU8)state->pct_research;
        packed[1] = (StarsU8)((future << 4) | state->current_technology);
        if (input->ops->write_mem)
            input->ops->write_mem(input->context, 0x22, 2, packed, 2);
        if (state->tutor_active && state->player_id == 0) {
            if (input->ops->tutor_mark)
                input->ops->tutor_mark(input->context, 4);
            if (input->ops->advance_tutor)
                input->ops->advance_tutor(input->context);
        }
    }
    if (input->ops->sticky_position)
        input->ops->sticky_position(input->context, hwnd,
            &state->sticky_position, 0);
    if (input->ops->end_dialog)
        input->ops->end_dialog(input->context, hwnd, changed);
    state->pct_research = -1;
    if (state->tutor_active && input->ops->advance_tutor)
        input->ops->advance_tutor(input->context);
    return 1;
}

StarsI16 stars_research_dlg(StarsResearchDlgInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect rect;
    StarsU16 dc;
    StarsU16 item;
    StarsU16 id;
    StarsI16 notify;
    StarsI16 x;
    StarsI16 y;

    if (!input || !input->state || !input->ops)
        return 0;
    if (message == RESEARCH_WM_PAINT) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        dc = input->ops->begin_paint ? input->ops->begin_paint(
            input->context, hwnd, &rect) : 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &rect);
        if (input->ops->draw_dialog)
            input->ops->draw_dialog(input->context, hwnd, dc, &rect, -1);
        if (input->ops->end_paint)
            input->ops->end_paint(input->context, hwnd, &rect);
        return 1;
    }
    if (message == RESEARCH_WM_ERASE_BACKGROUND) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &rect);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->context, wparam, &rect,
                input->state->button_face_brush);
        return 1;
    }
    if (message == RESEARCH_WM_CONTROL_COLOR)
        return research_control_color(input, hwnd, wparam, lparam);
    if (message == RESEARCH_WM_SET_CURSOR)
        return research_cursor(input, hwnd);
    if (message == RESEARCH_WM_INIT_DIALOG) {
        research_initialize(input, hwnd);
        return 1;
    }
    if (message == RESEARCH_WM_LEFT_BUTTON_DOWN ||
            message == RESEARCH_WM_LEFT_BUTTON_DOUBLE_CLICK) {
        x = (StarsI16)(lparam & 0xffffU);
        y = (StarsI16)((lparam >> 16) & 0xffffU);
        return input->ops->track_research ? input->ops->track_research(
            input->context, hwnd, x, y, wparam) : 0;
    }
    if (message != RESEARCH_WM_COMMAND)
        return 0;

    notify = (StarsI16)(StarsU16)(lparam >> 16);
    if (notify == 0 && wparam > 0x430U && wparam < 0x437U) {
        item = input->ops->button_checked ? input->ops->button_checked(
            input->context, hwnd, wparam) : 0;
        if (item) {
            input->state->current_technology = (StarsI16)(wparam -
                RESEARCH_FIRST_RADIO);
            rect.left = rect.top = rect.right = rect.bottom = 0;
            if (input->ops->get_client_rect)
                input->ops->get_client_rect(input->context, hwnd, &rect);
            if (input->ops->draw_dialog)
                input->ops->draw_dialog(input->context, hwnd, 0, &rect, 4);
        }
        return 0;
    }
    if (wparam == RESEARCH_OK)
        return research_commit(input, hwnd);
    if (wparam == RESEARCH_HELP) {
        if (input->ops->win_help)
            input->ops->win_help(input->context, hwnd, input->state->help_file,
                RESEARCH_HELP_CONTEXT);
        return 1;
    }
    (void)id;
    return 0;
}
