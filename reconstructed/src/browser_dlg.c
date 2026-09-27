#include "core.h"

enum {
    BROWSER_DLG_WM_DESTROY = 2,
    BROWSER_DLG_WM_PAINT = 15,
    BROWSER_DLG_WM_ERASE_BACKGROUND = 20,
    BROWSER_DLG_WM_CONTROL_COLOR = 25,
    BROWSER_DLG_WM_INIT_DIALOG = 272,
    BROWSER_DLG_WM_COMMAND = 273,
    BROWSER_DLG_ID_CANCEL = 2,
    BROWSER_DLG_ID_HULL = 0x10a,
    BROWSER_DLG_ID_COMPONENT = 0x10b,
    BROWSER_DLG_ID_PREVIOUS = 0x42e,
    BROWSER_DLG_ID_NEXT = 0x42f,
    BROWSER_DLG_COMBO_GET_SELECTION = 0x407,
    BROWSER_DLG_COMBO_ADD_STRING = 0x403,
    BROWSER_DLG_COMBO_RESET = 0x40e,
    BROWSER_DLG_TUTOR_BIT = 0x0800,
    BROWSER_DLG_MENU_POSITION = 5,
    BROWSER_DLG_MENU_ID = 0x0100
};

static StarsI16 browser_low(StarsU16 value)
{
    return (StarsI16)(value & 0x00ffU);
}

static void browser_invalidate(StarsBrowserDlgInput *input)
{
    if (input->ops->invalidate && input->state->browser_child_window)
        input->ops->invalidate(input->context,
            input->state->browser_child_window);
}

static StarsI16 browser_lookup(StarsBrowserDlgInput *input)
{
    if (!input->ops->lookup_part)
        return 0;
    return input->ops->lookup_part(input->context,
        input->state->hull_group, input->state->item_word);
}

static void browser_cleanup(StarsBrowserDlgInput *input, StarsU16 hwnd,
    StarsI16 end_dialog)
{
    StarsBrowserDlgState *state;
    StarsU16 menu;

    state = input->state;
    if (input->ops->sticky_position)
        input->ops->sticky_position(input->context, hwnd,
            &state->sticky_position, 0);
    state->browser_window = 0;
    state->browser_valid = 0;
    menu = input->ops->get_submenu ? input->ops->get_submenu(input->context,
        state->frame_window, BROWSER_DLG_MENU_POSITION) : 0;
    if (input->ops->check_menu_item)
        input->ops->check_menu_item(input->context, menu,
            BROWSER_DLG_MENU_ID, 0);
    if (end_dialog && input->ops->end_dialog)
        input->ops->end_dialog(input->context, hwnd, 1);
}

static void browser_set_component(StarsBrowserDlgInput *input,
    StarsI16 checked)
{
    StarsBrowserDlgState *state;
    StarsI32 selection;
    StarsU16 combo;
    StarsU16 mapped;
    StarsI16 result;
    StarsI16 attempts;

    state = input->state;
    combo = input->ops->get_dialog_item ? input->ops->get_dialog_item(
        input->context, state->browser_window, BROWSER_DLG_ID_COMPONENT) : 0;
    selection = input->ops->send_message ? input->ops->send_message(
        input->context, combo, BROWSER_DLG_COMBO_GET_SELECTION, 0, 0) : 0;
    if (selection < 0)
        return;
    if (selection < 2)
        selection = 1;
    mapped = input->ops->group_from_selection ?
        input->ops->group_from_selection(input->context,
            (StarsI16)(selection << 1)) : (StarsU16)selection;
    state->hull_group = mapped;
    state->item_word &= 0xff00U;
    result = browser_lookup(input);
    attempts = 0;
    while (result != 0 && attempts < 256) {
        if (result == 1 || !checked)
            break;
        state->item_word = (StarsU16)((state->item_word & 0xff00U) |
            (StarsU16)((browser_low(state->item_word) + 1) & 0xff));
        result = browser_lookup(input);
        ++attempts;
    }
    state->component_valid = (StarsU16)(result != 0);
    browser_invalidate(input);
}

static void browser_step_component(StarsBrowserDlgInput *input,
    StarsU16 command)
{
    StarsBrowserDlgState *state;
    StarsU16 combo;
    StarsI32 selection;
    StarsI16 checked;
    StarsI16 empty_combo;
    StarsI16 direction;
    StarsI16 old_item;
    StarsU16 old_group;
    StarsI16 result;
    StarsI16 attempts;
    StarsI16 changed;
    StarsI16 hidden;
    StarsI16 item;

    state = input->state;
    combo = input->ops->get_dialog_item ? input->ops->get_dialog_item(
        input->context, state->browser_window, BROWSER_DLG_ID_COMPONENT) : 0;
    selection = input->ops->send_message ? input->ops->send_message(
        input->context, combo, BROWSER_DLG_COMBO_GET_SELECTION, 0, 0) : 0;
    empty_combo = (StarsI16)(selection == 0);
    checked = input->ops->button_checked ? input->ops->button_checked(
        input->context, state->browser_window, BROWSER_DLG_ID_HULL) : 0;
    direction = command == BROWSER_DLG_ID_NEXT ? 1 : -1;
    old_item = browser_low(state->item_word);
    old_group = state->hull_group;
    result = 0;
    attempts = 0;
    changed = 0;
    while (attempts <= 0x15e) {
        item = (StarsI16)((browser_low(state->item_word) + direction) & 0xff);
        state->item_word = (StarsU16)((state->item_word & 0xff00U) |
            (StarsU16)item);
        changed = 1;
        result = browser_lookup(input);
        if (result != 0) {
            hidden = 0;
            if (result != 1 && input->ops->should_hide)
                hidden = input->ops->should_hide(input->context,
                    state->hull_group, state->item_word, result);
            if (result == 1 || !checked || (result != -1 && !hidden))
                break;
        } else if (empty_combo && direction > 0) {
            state->hull_group = input->ops->step_group ?
                input->ops->step_group(input->context, state->hull_group,
                    1) : state->hull_group;
            state->item_word &= 0xff00U;
        } else if (!empty_combo && item == 0) {
            state->component_valid = 0;
            break;
        } else if (empty_combo && direction < 0) {
            if (item < 0x65)
                break;
            state->hull_group = input->ops->step_group ?
                input->ops->step_group(input->context, state->hull_group,
                    -1) : state->hull_group;
            state->item_word = (StarsU16)((state->item_word & 0xff00U) |
                100U);
        } else if (direction > 0) {
            state->item_word &= 0xff00U;
        } else {
            break;
        }
        ++attempts;
    }
    state->component_valid = (StarsU16)(result != 0);
    if ((browser_low(state->item_word) != old_item) ||
            state->hull_group != old_group || changed ||
            (result != 1 && checked)) {
        if (browser_low(state->item_word) == old_item &&
                state->hull_group == old_group && selection != 0) {
            result = browser_lookup(input);
            if (result == 1)
                return;
        }
        if (result == 0)
            state->component_valid = 0;
        browser_invalidate(input);
    }
}

static void browser_initialize(StarsBrowserDlgInput *input, StarsU16 hwnd)
{
    StarsBrowserDlgState *state;
    StarsU16 metric_frame;
    StarsU16 dc;
    StarsU16 combo;
    StarsU16 item;
    StarsI16 label_length;
    StarsI32 extent;
    StarsI16 padding;
    StarsI16 label_width;
    StarsI16 control_y;
    StarsI16 i;
    StarsU16 was_valid;
    const StarsU8 *text;
    StarsU8 label[80];

    state = input->state;
    was_valid = state->browser_valid;
    state->browser_window = hwnd;
    state->browser_valid = 1;
    metric_frame = input->ops->get_system_metrics ?
        input->ops->get_system_metrics(input->context, 7) : 0;
    padding = state->arial8_height < 15 ? 0 : 0x28;
    if (input->ops->set_window_pos)
        input->ops->set_window_pos(input->context, hwnd, 0, 0,
            (StarsI16)(padding + 0x166 + metric_frame * 2),
            (StarsI16)(state->arial10_height + state->arial8_height * 15 +
                0x67 + (input->ops->get_system_metrics ?
                    input->ops->get_system_metrics(input->context, 8) * 2 : 0) +
                (input->ops->get_system_metrics ?
                    input->ops->get_system_metrics(input->context, 4) : 0)),
            6);
    if (input->ops->sticky_position)
        input->ops->sticky_position(input->context, hwnd,
            &state->sticky_position, 1);
    dc = input->ops->get_dc ? input->ops->get_dc(input->context, hwnd) : 0;
    if (input->ops->select_object)
        input->ops->select_object(input->context, dc, state->font_arial8);
    combo = input->ops->get_dialog_item ? input->ops->get_dialog_item(
        input->context, hwnd, BROWSER_DLG_ID_COMPONENT) : 0;
    if (input->ops->get_item_text) {
        label_length = input->ops->get_item_text(input->context, hwnd,
            0x42e, label, (StarsU16)sizeof(label));
        extent = input->ops->get_text_extent ? input->ops->get_text_extent(
            input->context, dc, label, label_length) : 0;
    } else {
        extent = 0;
    }
    label_width = (StarsI16)(extent + 0x0e);
    control_y = (StarsI16)(state->arial10_height + state->arial8_height *
        12 + (state->arial8_height * 3) / 2 + 0x60);
    if (input->ops->set_window_pos) {
        item = input->ops->get_dialog_item(input->context, hwnd, 0x42e);
        input->ops->set_window_pos(input->context, item, 6, 6,
            label_width, (state->arial8_height * 3) / 2, 4);
        item = input->ops->get_dialog_item(input->context, hwnd, 0x42f);
        input->ops->set_window_pos(input->context, item,
            (StarsI16)(padding + 0x15e - label_width), 6, label_width,
            (state->arial8_height * 3) / 2, 4);
        input->ops->set_window_pos(input->context, combo,
            (StarsI16)(label_width + 0x0c), 6,
            (StarsI16)(padding + 0x14c - label_width * 2),
            (StarsI16)(state->arial8_height * 0x12), 4);
        item = input->ops->get_dialog_item(input->context, hwnd,
            BROWSER_DLG_ID_CANCEL);
        input->ops->set_window_pos(input->context, item,
            (StarsI16)(padding + 0x15e - label_width), control_y,
            label_width, (state->arial8_height * 3) / 2, 4);
        item = input->ops->get_dialog_item(input->context, hwnd,
            BROWSER_DLG_ID_HULL);
        input->ops->set_window_pos(input->context, item, 6, control_y,
            (StarsI16)(padding + 0x158 - label_width),
            (state->arial8_height * 3) / 2, 4);
    }
    if (input->ops->select_object)
        input->ops->select_object(input->context, dc, state->font_arial8);
    if (input->ops->release_dc)
        input->ops->release_dc(input->context, hwnd, dc);
    if (!was_valid) {
        state->hull_group = 8;
        state->item_word &= 0xff00U;
    }
    browser_lookup(input);
    if (input->ops->create_window)
        state->browser_child_window = input->ops->create_window(input->context,
            (const StarsU8 *)"BROWSER", 0x50000000UL, 6,
            (StarsI16)((state->arial8_height * 3) / 2 + 0x0c),
            (StarsI16)(padding + 0x158),
            (StarsI16)(state->arial10_height + state->arial8_height * 12 +
                0x4e), hwnd);
    for (i = 0x43f; i < 0x450; ++i) {
        text = input->ops->get_compressed_string ?
            input->ops->get_compressed_string(input->context, (StarsU16)i) :
            (const StarsU8 *)0;
        if (input->ops->send_message)
            input->ops->send_message(input->context, combo,
                BROWSER_DLG_COMBO_ADD_STRING, 0,
                (StarsU32)(unsigned long)text);
    }
    if (input->ops->send_message)
        input->ops->send_message(input->context, combo,
            BROWSER_DLG_COMBO_RESET, 0, 0);
    if ((state->game_bits & BROWSER_DLG_TUTOR_BIT) != 0U &&
            input->ops->advance_tutor)
        input->ops->advance_tutor(input->context);
}

StarsI16 stars_browser_dlg(StarsBrowserDlgInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect rect;
    StarsU16 item;
    StarsI16 notify;
    StarsI16 checked;

    if (!input || !input->state || !input->ops)
        return 0;
    if (message == BROWSER_DLG_WM_DESTROY) {
        browser_cleanup(input, hwnd, 0);
    } else if (message == BROWSER_DLG_WM_PAINT) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->begin_paint)
            item = input->ops->begin_paint(input->context, hwnd, &rect);
        else
            item = 0;
        if (input->ops->end_paint)
            input->ops->end_paint(input->context, hwnd, &rect);
        (void)item;
        return 1;
    } else if (message == BROWSER_DLG_WM_ERASE_BACKGROUND) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &rect);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->context, wparam, &rect,
                input->state->button_face_brush);
        return 1;
    } else if (message == BROWSER_DLG_WM_CONTROL_COLOR) {
        item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
            input->context, hwnd, BROWSER_DLG_ID_PREVIOUS) : 0;
        if (lparam == item || lparam == (StarsU32)(input->ops->get_dialog_item ?
                input->ops->get_dialog_item(input->context, hwnd,
                    BROWSER_DLG_ID_NEXT) : 0) ||
                (StarsU16)(lparam >> 16) == 6U) {
            if (input->ops->set_background_color)
                input->ops->set_background_color(input->context, wparam,
                    input->state->button_face_color);
            return (StarsI16)input->state->button_face_brush;
        }
    } else if (message == BROWSER_DLG_WM_INIT_DIALOG) {
        browser_initialize(input, hwnd);
        return 1;
    } else if (message == BROWSER_DLG_WM_COMMAND) {
        notify = (StarsI16)(StarsU16)(lparam >> 16);
        if (wparam == BROWSER_DLG_ID_CANCEL) {
            browser_cleanup(input, hwnd, 1);
            if ((input->state->game_bits & BROWSER_DLG_TUTOR_BIT) != 0U &&
                    input->ops->advance_tutor)
                input->ops->advance_tutor(input->context);
            return 1;
        }
        if (wparam == BROWSER_DLG_ID_COMPONENT && notify == 1) {
            checked = input->ops->button_checked ?
                input->ops->button_checked(input->context, hwnd,
                    BROWSER_DLG_ID_HULL) : 0;
            browser_set_component(input, checked);
        } else if ((wparam == BROWSER_DLG_ID_NEXT ||
                wparam == BROWSER_DLG_ID_PREVIOUS)) {
            browser_step_component(input, wparam);
        }
        return 0;
    } else {
        if (input->ops->default_proc)
            return (StarsI16)input->ops->default_proc(input->context, hwnd,
                message, wparam, lparam);
        return 0;
    }
    if (input->ops->default_proc)
        return (StarsI16)input->ops->default_proc(input->context, hwnd,
            message, wparam, lparam);
    return 0;
}
