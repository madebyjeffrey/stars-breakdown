#include "core.h"

enum {
    MESSAGE_BUTTON_ROW_RIGHT_MARGIN = 0x36,
    MESSAGE_TITLE_LEFT = 4,
    MESSAGE_TITLE_TOP = 4,
    MESSAGE_TEXT_LEFT = 4,
    MESSAGE_TEXT_BOTTOM_MARGIN = 4,
    MESSAGE_TEXT_TOP_EXTRA = 3,
    MESSAGE_DROPDOWN_X = 0x1e,
    MESSAGE_DROPDOWN_WIDTH_REDUCTION = 0x54,
    MESSAGE_DROPDOWN_FLAGS = 4,
    MESSAGE_BUTTON_FLAGS = 5,
    MESSAGE_EDIT_FLAGS = 4,
    MESSAGE_MINMAX_WIDTH_EXTRA = 0x00c6,
    MESSAGE_MINMAX_HEIGHT_EXTRA = 0x0016
};

static StarsI16 message_low_word(StarsU32 value)
{ return (StarsI16)(StarsU16)value; }

static StarsI16 message_high_word(StarsU32 value)
{ return (StarsI16)(StarsU16)(value >> 16); }

static void message_tutor_advanced(StarsMessageWndState *state)
{
    if (state->tutor_active && state->ops->advance_tutor)
        state->ops->advance_tutor(state->context);
}

static StarsI16 message_current_id(StarsMessageWndState *state)
{
    if (state->ops->message_id)
        return state->ops->message_id(state->context,
            state->messages.current_message);
    return state->messages.current_message;
}

static StarsI16 message_filter_bit(const StarsMessageWndState *state,
    StarsI16 message_id)
{
    StarsU16 id;
    if (message_id < 0) return 0;
    id = (StarsU16)message_id;
    if ((id >> 3) >= (StarsU16)sizeof state->messages.filtered)
        return 0;
    return (StarsI16)((state->messages.filtered[id >> 3] >> (id & 7U)) & 1U);
}

static void message_set_filter_bit(StarsMessageWndState *state,
    StarsI16 message_id, StarsI16 value)
{
    StarsU16 id;
    StarsU8 mask;
    if (message_id < 0) return;
    id = (StarsU16)message_id;
    if ((id >> 3) >= (StarsU16)sizeof state->messages.filtered)
        return;
    mask = (StarsU8)(1U << (id & 7U));
    if (value) state->messages.filtered[id >> 3] =
        (StarsU8)(state->messages.filtered[id >> 3] | mask);
    else state->messages.filtered[id >> 3] =
        (StarsU8)(state->messages.filtered[id >> 3] & (StarsU8)~mask);
}

static void message_refresh(StarsMessageWndState *state)
{
    if (state->ops->set_msg_title)
        state->ops->set_msg_title(state->context, state->buttons[0]);
    if (state->ops->set_focus)
        state->ops->set_focus(state->context, state->edit);
}

static void message_finish_navigation(StarsMessageWndState *state,
    StarsI16 direction)
{
    if (state->ops->finish_entry)
        state->ops->finish_entry(state->context, direction);
}

static void message_select_next(StarsMessageWndState *state)
{
    StarsI16 selection;
    if (!state->ops->next_selection) return;
    selection = state->ops->next_selection(state->context,
        state->messages.view_filtered);
    if (selection != -1) state->messages.current_message = selection;
}

static void message_select_previous(StarsMessageWndState *state)
{
    StarsI16 selection;
    if (!state->ops->previous_selection) return;
    selection = state->ops->previous_selection(state->context,
        state->messages.view_filtered);
    if (selection != -1) state->messages.current_message = selection;
}

static void message_switch_mode(StarsMessageWndState *state)
{
    /* The action button commits an outgoing entry, then changes the same
     * child window into the incoming-message view.  The original toggles the
     * mode bit before refreshing the title and returning focus to the edit
     * control, so preserve that ordering for callbacks as well. */
    if (state->send_mode) message_finish_navigation(state, 0);
    state->send_mode = (StarsI16)!state->send_mode;
    message_refresh(state);
}

static void message_toggle_current_filter(StarsMessageWndState *state)
{
    StarsI16 id;
    StarsI16 filtered;
    if (state->messages.current_message < 0) return;
    id = message_current_id(state);
    filtered = message_filter_bit(state, id);
    message_set_filter_bit(state, id, (StarsI16)!filtered);
    if (state->ops->dirty_game) state->ops->dirty_game(state->context, 1);
    message_tutor_advanced(state);
    if (state->ops->set_msg_title)
        state->ops->set_msg_title(state->context, state->buttons[0]);
}

static void message_toggle_view(StarsMessageWndState *state)
{
    StarsI16 id;
    state->messages.view_filtered = (StarsU16)!state->messages.view_filtered;
    if (state->messages.current_message >= 0) {
        id = message_current_id(state);
        if (message_filter_bit(state, id) !=
                (StarsI16)state->messages.view_filtered) {
            message_select_next(state);
            if (state->messages.current_message == -1)
                message_select_previous(state);
        }
    }
}

static void message_accept(StarsMessageWndState *state)
{
    StarsI16 object;
    if (!state->send_mode && state->messages.current_message >= 0 &&
            state->messages.current_message <
            (StarsI16)state->messages.message_count) {
        object = state->object_kind;
        if (object != 0 && state->ops->object_action)
            state->ops->object_action(state->context, object,
                state->object_id, state->object_x, state->object_y);
        message_tutor_advanced(state);
    }
    message_switch_mode(state);
}

static StarsI16 message_command_source(StarsU32 lparam)
{ return message_low_word(lparam); }

static StarsI16 message_command_notify(StarsU32 lparam)
{ return message_high_word(lparam); }

static StarsI32 message_default(StarsMessageWndState *state, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    if (state->ops->default_proc)
        return state->ops->default_proc(state->context, hwnd, message,
            wparam, lparam);
    return 0;
}

/* Genuine semantic reconstruction of MessageWndProc, NE 7:5C92-7218.
 * This is the message child-window controller: WM_CREATE builds the four
 * navigation/action buttons plus recipient editor and scrollbar; WM_SIZE
 * derives the title/text rectangles; keyboard and mouse hit regions select,
 * filter, or send messages; WM_COMMAND routes object links; and WM_PAINT
 * delegates the title/body drawing while preserving the original USER/GDI
 * return conventions. */
StarsI32 stars_message_wnd_proc(StarsMessageWndState *state, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsI16 i;
    StarsI16 width;
    StarsI16 height;
    StarsI16 notification;
    StarsI16 source;
    StarsI16 hit;
    StarsPoint point;
    StarsRect client;
    StarsRect body;
    StarsU16 dc;
    StarsU8 title[128];
    StarsU8 text[1024];
    StarsI16 title_length;
    StarsI16 text_length;

    if (!state || !state->ops) return 0;

    if (message == STARS_MESSAGE_WM_CREATE) {
        for (i = 0; i < 4; ++i) {
            if (state->ops->create_window)
                state->ops->create_window(state->context, &state->buttons[i],
                    1, STARS_MESSAGE_CHILD_STYLE_BUTTON, 100, 100,
                    i == STARS_MESSAGE_BUTTON_DROPDOWN ?
                    STARS_MESSAGE_LAST_BUTTON_WIDTH :
                    STARS_MESSAGE_BUTTON_WIDTH,
                    (StarsI16)((state->arial8_height * 3) / 2 - 1), hwnd);
            if (state->ops->send_child_message)
                state->ops->send_child_message(state->context,
                    state->buttons[i], STARS_MESSAGE_SET_FONT,
                    state->font, 0UL);
        }
        if (state->ops->create_window)
            state->ops->create_window(state->context, &state->dropdown, 2,
                STARS_MESSAGE_CHILD_STYLE_DROPDOWN, 100, 100, 200, 0x50,
                hwnd);
        if (state->ops->send_child_message)
            state->ops->send_child_message(state->context, state->dropdown,
                STARS_MESSAGE_SET_FONT, state->font, 0UL);
        if (state->ops->create_window)
            state->ops->create_window(state->context, &state->edit, 3,
                STARS_MESSAGE_CHILD_STYLE_EDIT, 100, 100, 200, 0x32, hwnd);
        if (state->ops->send_child_message) {
            state->ops->send_child_message(state->context, state->edit,
                STARS_MESSAGE_SET_LIMIT, STARS_MESSAGE_EDIT_LIMIT, 0UL);
            state->ops->send_child_message(state->context, state->edit,
                STARS_MESSAGE_SET_FONT, state->font, 0UL);
        }
        if (state->ops->create_window)
            state->ops->create_window(state->context, &state->scroll, 4,
                STARS_MESSAGE_CHILD_STYLE_SCROLL, 100, 100, 200, 0x32, hwnd);
        if (state->ops->set_msg_title)
            state->ops->set_msg_title(state->context, hwnd);
        if (state->ops->send_child_message) {
            state->ops->send_child_message(state->context, state->dropdown,
                STARS_MESSAGE_ADD_STRING, 0, 0UL);
            for (i = 0; i < state->player_count; ++i) {
                if (state->ops->player_name)
                    title_length = state->ops->player_name(state->context, i,
                        title, (StarsU16)sizeof title);
                else title_length = 0;
                (void)title_length;
                state->ops->send_child_message(state->context, state->dropdown,
                    STARS_MESSAGE_ADD_STRING, 0, (StarsU32)(StarsU16)i);
            }
            state->ops->send_child_message(state->context, state->dropdown,
                STARS_MESSAGE_SELECT_STRING, 0, 0UL);
        }
        return 0;
    }

    if (message == STARS_MESSAGE_WM_SIZE) {
        width = message_low_word(lparam);
        height = message_high_word(lparam);
        for (i = 0; i < 3; ++i)
            if (state->ops->set_position)
                state->ops->set_position(state->context, state->buttons[i],
                    (StarsI16)(width - MESSAGE_BUTTON_ROW_RIGHT_MARGIN),
                    (StarsI16)(state->arial8_height * 2 +
                    ((state->arial8_height * 3) / 2 + 2) * i +
                    MESSAGE_TEXT_TOP_EXTRA), 0, 0, 0x55);
        state->message_text.left = MESSAGE_TEXT_LEFT;
        state->message_text.top = (StarsI16)(state->arial8_height * 2 +
            MESSAGE_TEXT_TOP_EXTRA);
        state->message_text.right = (StarsU16)(width - MESSAGE_TEXT_LEFT);
        state->message_text.bottom = (StarsU16)(height -
            MESSAGE_TEXT_BOTTOM_MARGIN);
        state->message_title.left = MESSAGE_TITLE_LEFT;
        state->message_title.top = MESSAGE_TITLE_TOP;
        state->message_title.right = (StarsU16)(width - MESSAGE_TITLE_LEFT);
        state->message_title.bottom = (StarsU16)(state->arial8_height * 2 -
            MESSAGE_TEXT_BOTTOM_MARGIN);
        body = state->message_text;
        body.left = (StarsU16)(body.left - 4U);
        body.top = (StarsU16)(body.top - 4U);
        body.right = (StarsU16)(body.right + 4U);
        body.bottom = (StarsU16)(body.bottom + 4U);
        if (state->ops->set_position) {
            state->ops->set_position(state->context, state->dropdown,
                (StarsI16)(body.left + MESSAGE_DROPDOWN_X),
                (StarsI16)body.top,
                (StarsI16)(body.right - body.left -
                MESSAGE_DROPDOWN_WIDTH_REDUCTION),
                (StarsI16)(body.bottom - body.top), MESSAGE_DROPDOWN_FLAGS);
            state->ops->set_position(state->context,
                state->buttons[STARS_MESSAGE_BUTTON_DROPDOWN],
                (StarsI16)(body.right - 0x32), (StarsI16)body.top, 0, 0,
                MESSAGE_BUTTON_FLAGS);
            state->ops->set_position(state->context, state->edit,
                (StarsI16)body.left,
                (StarsI16)(body.top + state->ship_dropdown_height + 3),
                (StarsI16)(body.right - body.left),
                (StarsI16)(body.bottom - body.top -
                state->ship_dropdown_height - 3), MESSAGE_EDIT_FLAGS);
        }
        return message_default(state, hwnd, message, wparam, lparam);
    }

    if (message == STARS_MESSAGE_WM_ERASE_BACKGROUND) {
        if (state->ops->get_client_rect)
            state->ops->get_client_rect(state->context, hwnd, &client);
        if (state->ops->fill_rect)
            state->ops->fill_rect(state->context, wparam, &client,
                state->button_brush);
        return 1;
    }
    if (message == STARS_MESSAGE_WM_CONTROL_COLOR) {
        if (message_low_word(lparam) == (StarsI16)state->scroll) {
            if (state->ops->set_background_color)
                state->ops->set_background_color(state->context, wparam,
                    state->button_face_color);
            return (StarsI32)state->button_brush;
        }
        return message_default(state, hwnd, message, wparam, lparam);
    }
    if (message == STARS_MESSAGE_WM_SETCURSOR) {
        if (state->ops->get_cursor_pos)
            state->ops->get_cursor_pos(state->context, &point);
        if (state->ops->screen_to_client)
            state->ops->screen_to_client(state->context, hwnd, &point);
        hit = state->ops->hit_test ? state->ops->hit_test(state->context,
            point) : 0;
        if (hit != 0) {
            if (state->ops->set_cursor)
                state->ops->set_cursor(state->context, state->hand_cursor);
            return 1;
        }
        return message_default(state, hwnd, message, wparam, lparam);
    }
    if (message == STARS_MESSAGE_WM_GETMINMAXINFO) {
        /* The host adapter receives the two track-size words directly in
         * lParam; the original wrote them into MINMAXINFO before chaining. */
        if (state->ops->set_position)
            state->ops->set_position(state->context, 0,
                (StarsI16)(state->win_frame_width * 2 +
                MESSAGE_MINMAX_WIDTH_EXTRA),
                (StarsI16)((state->arial8_height * 13) / 2 +
                MESSAGE_MINMAX_HEIGHT_EXTRA), 0, 0, 0);
        return message_default(state, hwnd, message, wparam, lparam);
    }

    if (message == STARS_MESSAGE_WM_KEYDOWN) {
        if (wparam == STARS_MESSAGE_KEY_DOWN) {
            if (state->send_mode) message_finish_navigation(state, 1);
            else message_select_next(state);
        } else if (wparam == STARS_MESSAGE_KEY_UP) {
            if (state->send_mode) message_finish_navigation(state, -1);
            else message_select_previous(state);
        } else if (!state->send_mode && wparam == STARS_MESSAGE_KEY_HOME) {
            state->messages.current_message = -1;
            message_select_next(state);
        } else if (!state->send_mode && wparam == STARS_MESSAGE_KEY_END) {
            state->messages.current_message = (StarsI16)
                (state->messages.message_count +
                state->messages.player_message_count);
            message_select_previous(state);
        } else return 0;
        state->messages.current_message = state->messages.current_message;
        message_refresh(state);
        message_tutor_advanced(state);
        return 0;
    }
    if (message == STARS_MESSAGE_WM_CHAR) {
        if (wparam == STARS_MESSAGE_CHAR_ACCEPT) {
            message_accept(state);
            return 0;
        }
        if (wparam == STARS_MESSAGE_CHAR_TOGGLE_FILTER) {
            message_toggle_current_filter(state);
            return 0;
        }
        if (wparam == STARS_MESSAGE_CHAR_TOGGLE_VIEW) {
            message_toggle_view(state);
            message_refresh(state);
            return 0;
        }
        return 0;
    }
    if (message == STARS_MESSAGE_WM_COMMAND) {
        source = message_command_source(lparam);
        notification = message_command_notify(lparam);
        if (notification == 0 && state->ops->set_focus)
            state->ops->set_focus(state->context, state->buttons[0]);
        if (notification != 0) return message_default(state, hwnd, message,
            wparam, lparam);
        if (source == (StarsI16)state->buttons[0]) {
            if (state->send_mode) message_finish_navigation(state, -1);
            else message_select_previous(state);
            message_refresh(state);
            return 0;
        }
        if (source == (StarsI16)state->buttons[2]) {
            if (state->send_mode) message_finish_navigation(state, 1);
            else message_select_next(state);
            message_refresh(state);
            return 0;
        }
        if (source == (StarsI16)state->buttons[3]) {
            message_finish_navigation(state, 1000);
            message_refresh(state);
            return 0;
        }
        if (source == (StarsI16)state->buttons[1]) {
            message_accept(state);
            return 0;
        }
        return message_default(state, hwnd, message, wparam, lparam);
    }
    if (message == STARS_MESSAGE_WM_LBUTTONDOWN ||
            message == STARS_MESSAGE_WM_LBUTTONDOUBLECLICK) {
        point.x = message_low_word(lparam);
        point.y = message_high_word(lparam);
        hit = state->ops->hit_test ? state->ops->hit_test(state->context,
            point) : 0;
        if (hit == 1) message_toggle_current_filter(state);
        else if (hit == 2) {
            message_toggle_view(state);
            message_refresh(state);
        } else if (hit == 3) message_switch_mode(state);
        return 0;
    }
    if (message != STARS_MESSAGE_WM_PAINT) return message_default(state, hwnd,
        message, wparam, lparam);

    dc = state->ops->begin_paint ? state->ops->begin_paint(state->context,
        hwnd) : 0;
    if (state->ops->draw_frame)
        state->ops->draw_frame(state->context, dc, &state->message_title);
    title_length = 0;
    if (state->ops->format_title)
        state->ops->format_title(state->context, state->send_mode, title,
            (StarsU16)sizeof title);
    if (state->ops->draw_title)
        state->ops->draw_title(state->context, dc, &state->message_title,
            title, title_length);
    text_length = 0;
    if (state->ops->draw_body)
        state->ops->draw_body(state->context, dc, &state->message_text, text,
            text_length);
    if (state->ops->end_paint)
        state->ops->end_paint(state->context, hwnd, dc);
    return 0;
}
